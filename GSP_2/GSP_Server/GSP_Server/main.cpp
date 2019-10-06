#include "main.h"

map<int, SOCKETINFO>				g_Clients;
PLAYERINFO									g_Players[10]{};
list<int>											g_CloseIdx;

POS KeyMessage(const char & key);
int FindClientID( const SOCKET& socket );

void CALLBACK recv_callback( DWORD Error, DWORD dataBytes, LPWSAOVERLAPPED overlapped, DWORD lnFlags );
void CALLBACK send_callback( DWORD Error, DWORD dataBytes, LPWSAOVERLAPPED overlapped, DWORD lnFlags );


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

void CALLBACK recv_callback( DWORD Error, DWORD dataBytes, LPWSAOVERLAPPED overlapped, DWORD lnFlags )
{
	SOCKET client_s = reinterpret_cast< int >( overlapped->hEvent );
	int iID = FindClientID( client_s );

	if ( dataBytes == 0 ) {
		closesocket( g_Clients[iID].socket );
		g_Clients.erase( iID );
		g_CloseIdx.push_back( iID );
		g_Players[iID] = PLAYERINFO{};
		return;
	} // 클라이언트가 closesocket을 했을 경우

	g_Clients[iID].messageBuffer[dataBytes] = 0;

	// 받은 키값으로 움직인다.
	KEY tKey{};
	memcpy( &tKey, g_Clients[iID].messageBuffer, sizeof( KEY ) );
	cout << "From client : " << tKey.iID << " --> " << tKey.cKey << " (" << dataBytes << ") bytes)\n";

	POS		tMovePos = KeyMessage( tKey.cKey );
	g_Players[iID].tPos += tMovePos;
	g_Clients[iID].dataBuffer.len = sizeof(g_Players);
	g_Clients[iID].dataBuffer.buf = ( char* )&g_Players;
	memset( &( g_Clients[iID].overlapped ), 0x00, sizeof( WSAOVERLAPPED ) );
	g_Clients[iID].overlapped.hEvent = ( HANDLE )client_s;
	WSASend( client_s, &( g_Clients[iID].dataBuffer ), 1, &dataBytes, 0, &( g_Clients[iID].overlapped ), send_callback );
}

void CALLBACK send_callback( DWORD Error, DWORD dataBytes, LPWSAOVERLAPPED overlapped, DWORD lnFlags )
{
	DWORD receiveBytes = 0;
	DWORD flags = 0;
	SOCKET client_s = reinterpret_cast< int >( overlapped->hEvent );
	int iID = FindClientID( client_s );

	if ( dataBytes == 0 ) {
		closesocket( g_Clients[iID].socket );
		g_Clients.erase( iID );
		g_CloseIdx.push_back( iID );
		g_Players[iID] = PLAYERINFO{};
		return;
	} // 클라이언트가 closesocket을 했을 경우

	PLAYERINFO tInfo{};
	memcpy( &tInfo, g_Clients[iID].messageBuffer, sizeof( PLAYERINFO ) );
	cout << "Sent : " << tInfo.iID << " --> " << tInfo.tPos.x << ", " << tInfo.tPos.y << " (" << dataBytes << " bytes)\n";

	// 키 값을 받는다.
	g_Clients[iID].dataBuffer.len = sizeof(KEY);
	g_Clients[iID].dataBuffer.buf = g_Clients[iID].messageBuffer;
	memset( &( g_Clients[iID].overlapped ), 0x00, sizeof( WSAOVERLAPPED ) );
	g_Clients[iID].overlapped.hEvent = ( HANDLE )client_s;
	WSARecv( client_s, &g_Clients[iID].dataBuffer, 1, 0, &flags, 	&( g_Clients[iID].overlapped ), recv_callback );
}