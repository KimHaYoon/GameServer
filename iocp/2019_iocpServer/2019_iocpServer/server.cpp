#include <iostream>
#include <map>
#include <thread>
#include <set>
#include <mutex>
using namespace std;
#include <WS2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")

#include "protocol.h"

#define MAX_BUFFER        1024
constexpr auto VIEW_RANGE = 4;


struct OVER_EX {
	WSAOVERLAPPED over;
	WSABUF   wsabuf[1];
	char   net_buf[MAX_BUFFER];
	bool   is_recv;
};

struct SOCKETINFO
{
	OVER_EX   recv_over;
	SOCKET   socket;
	int      id;
	short   x, y;
	set<int> near_id;
	mutex near_lock;

};

map <int, SOCKETINFO *> clients;
HANDLE   g_iocp;

int new_user_id = 0;

void error_display( const char *msg, int err_no )
{
	WCHAR *lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, err_no,
		MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT ),
		( LPTSTR )&lpMsgBuf, 0, NULL );
	cout << msg;
	wcout << L"에러 " << lpMsgBuf << endl;
	while ( true );
	LocalFree( lpMsgBuf );
}

void Create_NPC()
{
	for ( int npc_id = NPC_ID_START; npc_id < NUM_NPC + NPC_ID_START; ++npc_id )
	{
		clients[npc_id] = new SOCKETINFO;
		clients[npc_id]->id = npc_id;
		clients[npc_id]->x = rand() % 30;
		clients[npc_id]->y = rand() % 30;
		clients[npc_id]->socket = -1;
	}
}

bool is_NPC( int id )
{
	return id >= NPC_ID_START;
}


bool is_near( int a, int b )
{
	return VIEW_RANGE > abs( clients[a]->x - clients[b]->x + clients[a]->y - clients[b]->y );

}

void send_packet( int id, void* buff )
{
	char* packet = reinterpret_cast< char* >( buff );
	int packet_size = packet[0];
	OVER_EX *send_over = new OVER_EX;
	memset( send_over, 0x00, sizeof( OVER_EX ) );
	send_over->is_recv = false;
	memcpy( send_over->net_buf, packet, packet_size );
	send_over->wsabuf[0].buf = send_over->net_buf;
	send_over->wsabuf[0].len = packet_size;
	int ret = WSASend( clients[id]->socket, send_over->wsabuf, 1, 0, 0, &send_over->over, 0 );
	if ( 0 != ret )
	{
		int err_no = WSAGetLastError();
		if ( WSA_IO_PENDING != err_no )
		{
			error_display( "WSARecv Error :", err_no );
		}
	}
}

void send_login_ok_packet( int id )
{
	sc_packet_login_ok packet;
	packet.id = id;
	packet.size = sizeof( packet );
	packet.type = SC_LOGIN_OK;

	send_packet( id, &packet );
}

void send_put_player_packet( int client, int new_id )
{
	sc_packet_put_player packet;
	packet.id = new_id;
	packet.size = sizeof( packet );
	packet.type = SC_PUT_PLAYER;
	packet.x = clients[new_id]->x;
	packet.y = clients[new_id]->y;

	send_packet( client, &packet );

	lock_guard<mutex> lg{ clients[client]->near_lock };
	clients[client]->near_id.insert( new_id );

}

void send_pos_packet( int client, int mover )
{
	sc_packet_pos packet;
	packet.id = mover;
	packet.size = sizeof( packet );
	packet.type = SC_POS;
	packet.x = clients[mover]->x;
	packet.y = clients[mover]->y;

	send_packet( client, &packet );
}

void send_remove_player_packet( int client, int leaver )
{
	sc_packet_remove_player packet;
	packet.id = leaver;
	packet.size = sizeof( packet );
	packet.type = SC_REMOVE_PLAYER;

	send_packet( client, &packet );
}

bool is_near_id( int player, int other )
{
	lock_guard<mutex> lg{ clients[player]->near_lock };
	return ( 0 != clients[player]->near_id.count( other ) );
}

void ProcessPacket( int id, void* buff )
{
	char* packet = reinterpret_cast< char* >( buff );
	short x = clients[id]->x;
	short y = clients[id]->y;

	switch ( packet[1] ) {
	case CS_UP:
		if ( y > 0 )
			--y;
		break;
	case CS_DOWN:
		if ( y < WORLD_HEIGHT - 1 )
			++y;
		break;
	case CS_LEFT:
		if ( x > 0 )
			--x;
		break;
	case CS_RIGHT:
		if ( x < WORLD_WIDTH )
			++x;
		break;
	default:
		cout << "Invalid Pakcet Type Error\n";
		while ( true );
	}
	clients[id]->x = x;
	clients[id]->y = y;

	set<int> temp = clients[id]->near_id;
	for ( auto& cl : temp )
	{
		if ( is_NPC( cl ) )
			continue;

		if ( is_near( cl, id ) == true ) {
			send_pos_packet( cl, id );
		}
		else {
			send_remove_player_packet( cl, id );
			send_remove_player_packet( id, cl );
			clients[id]->near_id.erase( cl );
		}
	}
	for ( auto& cl : clients ) {	// 모든 플레이어들 대상으로 시야 범위 체크
		if ( is_NPC( cl.first ) )
			continue;

		if ( is_near( cl.first, id ) == true ) { // 시야범위 들어온 플레이어들 중
			if ( is_near_id( id, cl.first ) == false ) { // 시야리스트에 없는 플레이어 추가
				if ( cl.first != id ) {
					send_put_player_packet( cl.first, id );
					send_put_player_packet( id, cl.first );
				}
			}
		}
	}

}

void do_worker()
{
	while ( true ) {
		DWORD num_byte;
		ULONG key;
		PULONG p_key = &key;
		WSAOVERLAPPED *p_over;

		GetQueuedCompletionStatus( g_iocp, &num_byte, p_key, &p_over, INFINITE ); // 리턴값을 받아서 에러 처리를 해야한다.
		SOCKET client_s = clients[key]->socket;
		if ( num_byte == 0 ) {

			closesocket( client_s );
			clients.erase( key );
			for ( auto &cl : clients )
			{
				send_remove_player_packet( cl.first, key );
				if ( is_near_id( cl.first, key ) == true ) {
					cl.second->near_id.erase( key );
				}
			}
			continue;
		}  // 클라이언트가 closesocket을 했을 경우

		OVER_EX *over_ex = reinterpret_cast< OVER_EX * > ( p_over );

		if ( true == over_ex->is_recv ) {
			ProcessPacket( key, over_ex->net_buf );

			DWORD flags = 0;
			memset( &over_ex->over, 0x00, sizeof( WSAOVERLAPPED ) );
			WSARecv( client_s, over_ex->wsabuf, 1, 0, &flags, &over_ex->over, 0 );
		}
		else {
			delete over_ex;
		}
	}
}

void do_ai()
{
	for ( auto& npc : clients )
	{
		if ( !is_NPC( npc.second->id ) )
		{
			continue;
		}
		int x = npc.second->x;
		int y = npc.second->y;

		set<int> old_view_list;
		for ( auto& obj : clients )
		{
			if ( is_near( npc.second->id, obj.second->id ) )
			{
				old_view_list.insert( obj.second->id );
			}
		}

		switch ( rand() % 4 )
		{
		case 0:
			if(y > 0 )
				--y;
			break;
		case 1:
			if(y < WORLD_HEIGHT - 1 )
				++y;
			break;
		case 2:
			if ( x > 0 )
				--x;
			break;
		case 3:
			if( x < WORLD_WIDTH - 1 )
				++x;
			break;
		default:
			break;
		}

		npc.second->x = x;
		npc.second->y = y;
		set<int> new_view_list;
		for ( auto& obj : clients )
		{
			if ( is_near( npc.second->id, obj.second->id ) )
			{
				old_view_list.insert( obj.second->id );
			}
		}
		for ( auto& pc : clients )
		{
			if ( is_NPC( pc.second->id ) )
				continue;
			if ( !is_near( pc.second->id, npc.second->id ) )
				continue;
			send_pos_packet( pc.second->id, npc.second->id );
		}
	}
}
int main()
{
	wcout.imbue( std::locale( "korean" ) );

	Create_NPC();


	WSADATA WSAData;
	WSAStartup( MAKEWORD( 2, 2 ), &WSAData );
	SOCKET listenSocket = WSASocket( AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED );
	SOCKADDR_IN serverAddr;
	memset( &serverAddr, 0, sizeof( SOCKADDR_IN ) );
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons( SERVER_PORT );
	serverAddr.sin_addr.S_un.S_addr = htonl( INADDR_ANY );
	::bind( listenSocket, ( struct sockaddr* )&serverAddr, sizeof( SOCKADDR_IN ) );
	listen( listenSocket, 5 );
	SOCKADDR_IN clientAddr;
	int addrLen = sizeof( SOCKADDR_IN );
	memset( &clientAddr, 0, addrLen );
	SOCKET clientSocket;
	DWORD flags;

	g_iocp = CreateIoCompletionPort( INVALID_HANDLE_VALUE, NULL, NULL, 0 );
	thread worker_thread{ do_worker };
	thread worker_thread2{ do_worker };
	thread worker_thread3{ do_worker };
	thread ai_thead { do_ai };

	while ( true ) {
		clientSocket = accept( listenSocket, ( struct sockaddr * )&clientAddr, &addrLen );
		int user_id = new_user_id++;
		clients[user_id] = new SOCKETINFO;
		//memset(clients[user_id], 0, sizeof(struct SOCKETINFO));
		clients[user_id]->socket = clientSocket;
		clients[user_id]->recv_over.wsabuf[0].len = MAX_BUFFER;
		clients[user_id]->recv_over.wsabuf[0].buf = clients[user_id]->recv_over.net_buf;
		clients[user_id]->recv_over.is_recv = true;
		flags = 0;
		CreateIoCompletionPort( reinterpret_cast< HANDLE >( clientSocket ), g_iocp, user_id, 0 );

		send_login_ok_packet( user_id );
		clients[user_id]->x = 4;
		clients[user_id]->y = 4;
		for ( auto& cl : clients ) {
			int other_player = cl.first;

			if ( is_NPC( user_id ) || is_NPC( other_player ) )
				continue;

			if ( true == is_near( other_player, user_id ) ) {
				send_put_player_packet( other_player, user_id );

				if ( other_player != user_id ) {
					send_put_player_packet( user_id, other_player );
				}
			}
		}
		memset( &clients[user_id]->recv_over.over, 0, sizeof( clients[user_id]->recv_over.over ) );
		int ret = WSARecv( clientSocket, clients[user_id]->recv_over.wsabuf, 1, NULL,
			&flags, &( clients[user_id]->recv_over.over ), NULL );
		if ( 0 != ret ) {
			int err_no = WSAGetLastError();
			if ( WSA_IO_PENDING != err_no )
				error_display( "WSARecv Error :", err_no );
		}
	}
	ai_thead.join();
	worker_thread.join();
	closesocket( listenSocket );
	WSACleanup();
}
