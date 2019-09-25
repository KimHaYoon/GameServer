// 190925Server.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include "pch.h"
#include <iostream>
#include <WS2tcpip.h>
#pragma commnet(lib, "Ws2_32.lib")

#define MAX_BUFFER		1024
#define SERVER_PORT		3500				// 서버랑 클라이언트의 포트 주소를 맞춰야 함
											// 서버는 클라이언트의 주소를 알 필요가 없다.

using namespace std;

int main()
{
	WSADATA WSAData;
	WSAStartup(MAKEWORD(2, 0), &WSAData);	

	SOCKET listenSocket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, 0);

	SOCKADDR_IN serverAddr;
	memset(&serverAddr, 0, sizeof(SOCKADDR_IN));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(SERVER_PORT);				
	serverAddr.sin_addr.S_un.S_addr = INADDR_ANY;			// 모든 주소에서 오는 것을 다 받겠다.
															// 클라이언트 주소를 입력하면 그 IP주소를 가진 클라이언트만 받는다.
															// 보안이 강화시킬 때 사용한다. ex) 회사 내 서버망
	
	// bind() : 포트주소를 선점
	::bind(listenSocket, (sockaddr*)&serverAddr, sizeof(listenSocket));	
	
	// listen() : 그 주소로 오는 것을 받아라, 5 : backlog(클라이언트 대기열의 길이, 5개의 클라이언트만 접속이 가능하도록 제한두는 것)
	listen(listenSocket, 5);			

	SOCKADDR_IN client_addr;
	while (true)
	{
		char messageBuffer[MAX_BUFFER] = {};
		int addr_size;

		// accept는 접속한 클라이언트의 socket을 반환하는 데, accept()가 호출된 횟수만큼의 클라이언트 대기열의 번호가 호출 된다.
		SOCKET client_socket = accept(listenSocket, (sockaddr*)&client_addr, &addr_size);
		// 접속한 클라이언트의 주소를 알기위해 sockaddr을 넣어준다.
		// socket하나당 클라이언트 하나. 인터넷은 1:1 통신이기 때문에 클라이언트 여러개를 받으려면 socket을 그만큼 선언해줘야한다.

		while (true)
		{
			int recvBytes = recv(listenSocket, messageBuffer, MAX_BUFFER, 0);

			if (recvBytes > 0)
			{
				cout << "TRACE - recv message : " << messageBuffer << "\t(" << recvBytes << "bytes)" << endl;
			}
		}
	}
}