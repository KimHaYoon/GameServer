#include "main.h"

map<int, SOCKETINFO>				g_Clients;
PLAYERINFO									g_Players[10]{};
list<int>											g_CloseIdx;

POS KeyMessage(const char & key);
int FindClientID( const SOCKET& socket );

int main()
{
	WSADATA WSAData;
	WSAStartup( MAKEWORD( 2, 2 ), &WSAData );
	SOCKET listenSocket = WSASocket( AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED );
	SOCKADDR_IN serverAddr;
	memset( &serverAddr, 0, sizeof( SOCKADDR_IN ) );
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons( SERVER_PORT );
	serverAddr.sin_addr.S_un.S_addr = htonl( INADDR_ANY );
	::bind( listenSocket, ( struct sockaddr* )&serverAddr, sizeof( SOCKADDR_IN ) );
	listen( listenSocket, 10 );
	SOCKADDR_IN clientAddr;
	int addrLen = sizeof( SOCKADDR_IN );
	memset( &clientAddr, 0, addrLen );
	SOCKET clientSocket;
	DWORD flags;

	for(int i =0 ; i < 10; ++i )
		g_CloseIdx.push_back( i );

	while ( true )
	{
		clientSocket = accept( listenSocket, ( struct sockaddr * )&clientAddr, &addrLen );				// 연결한 클라이언트에 
		int iID = g_CloseIdx.front();
		g_CloseIdx.pop_front();

		cout << "ID - " << iID << endl;
		cout << "CloseIdx size = " << g_CloseIdx.size() << endl;

		g_Players[iID] = PLAYERINFO{ iID, true, POS{125, 100} };
		g_Clients[iID] = SOCKETINFO{};
		memset( &g_Clients[iID], 0, sizeof( SOCKETINFO ) );
		g_Clients[iID].socket = clientSocket;
		g_Clients[iID].dataBuffer.len = sizeof( PLAYERINFO );
		g_Clients[iID].dataBuffer.buf = (char*)&g_Players[iID];
		g_Clients[iID].overlapped.hEvent = ( HANDLE )g_Clients[iID].socket;

		WSASend( g_Clients[iID].socket, &g_Clients[iID].dataBuffer, 1, NULL, 0, &( g_Clients[iID].overlapped ), send_callback );
	}
	closesocket( listenSocket );
	WSACleanup();
}

POS KeyMessage( const char & key )
{
	if ( key == KEY_LEFT )
		return POS{ -100,  0 };

	if ( key == KEY_RIGHT )
		return POS{ 100,  0 };

	if ( key == KEY_UP )
		return POS{ 0,  -100 };

	if ( key == KEY_DOWN )
		return POS{ 0,  100 };

	return POS();
}

int FindClientID( const SOCKET & socket )
{
	for ( auto client : g_Clients )
	{
		if ( client.second.socket == socket )
			return client.first;
	}

	return 0;
}
