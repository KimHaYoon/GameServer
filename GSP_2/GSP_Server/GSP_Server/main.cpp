#include "main.h"

POS KeyMessage(const char* key);

int main()
{
	WSADATA WSAData;
	WSAStartup( MAKEWORD( 2, 0 ), &WSAData );
	SOCKET listenSocket = WSASocket( AF_INET, SOCK_STREAM, 0, NULL, 0, 0 );
	SOCKADDR_IN serverAddr;
	memset( &serverAddr, 0, sizeof( SOCKADDR_IN ) );
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons( SERVER_PORT );
	serverAddr.sin_addr.S_un.S_addr = htonl( INADDR_ANY );
	::bind( listenSocket, ( sockaddr * )&serverAddr, sizeof( serverAddr ) );
	listen( listenSocket, 5 );
	SOCKADDR_IN client_addr;
	while ( true ) {
		int addr_size = sizeof( client_addr );
		SOCKET client_socket = accept( listenSocket, ( sockaddr* )&client_addr, &addr_size );

		if ( client_socket == INVALID_SOCKET ) {
			printf( "err" );
			break;
		}
		else {
			printf( "success  \n" );
		}

		// 초반 플레이어의 위치 보내주기
		POS tPlayerPos{ 125, 100 };
		send( client_socket, ( char* )&tPlayerPos, sizeof( POS ), 0 );

		while ( true ) {
			char messageBuffer[MAX_BUFFER];


			// key입력 받기
			int receiveBytes = recv( client_socket, messageBuffer, MAX_BUFFER, 0 );
			if ( receiveBytes > 0 )
			{
				messageBuffer[receiveBytes] = NULL;
				printf( "TRACE - Receive message : %s (%d bytes)\n", messageBuffer, receiveBytes );
			}

			// 받은 key입역을 토대로 위치를 클라이언트에 보내주기
			POS tMovePos = KeyMessage( messageBuffer );
			tPlayerPos.x += tMovePos.x;
			tPlayerPos.y += tMovePos.y;

			printf( "TRACE - Pos : %f  %f\n", tPlayerPos.x, tPlayerPos.y);

			 send( client_socket, ( char* )&tPlayerPos, sizeof( POS ), 0 );
		}

		closesocket( client_socket );
	}
	closesocket( listenSocket );
	WSACleanup();
}

POS KeyMessage( const char * key )
{
	if ( *key == KEY_LEFT )
		return POS{ -100,  0 };

	if ( *key == KEY_RIGHT )
		return POS{ 100,  0 };

	if(*key == KEY_UP )
		return POS{ 0,  -100 };

	if(*key == KEY_DOWN )
		return POS{ 0,  100 };

	return POS();
}
