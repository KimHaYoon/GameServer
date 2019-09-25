// 190925Client.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include "pch.h"
#include <iostream>
#include <WS2tcpip.h>
#pragma commnet(lib, "Ws2_32.lib")
				// windows, NT 이후에 사용하는 라이브러리

using namespace std;

#define MAX_BUFFER		1024
#define SERVER_IP		"127.0.0.1"			// 서버 주소 : 자기 자신
#define SERVER_PORT		3500				// 서버랑 클라이언트의 포트 주소를 맞춰야 함

int main()
{
	WSADATA WSAData;
	WSAStartup(MAKEWORD(2, 0), &WSAData);	// 네트워크 기능을 사용하기 위함
											// 인터넷 표준을 사용하기 위해. 사용을 안해주면 MS내의 이상한 프로토콜사용..? 
											// 리눅스나 MAC이나 이런거 안해줘도 됨 -> 빌게이츠의 삽질

	// 소켓 초기화 WSASocket(인터넷 프로토콜을 쓰겠다, TCP/IP, 뒤에껀 무시)
	SOCKET listenSocket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, 0);
					// WSA가 붙어있는 것은 2.0 없는것은 1.0 우리는 업그레이드가 되어있는 API를 사용할 것이기 때문에 WSA가 붙어있는 함수를 사용하자.

	char buffer[INET6_ADDRSTRLEN];

	SOCKADDR_IN serverAddr;
	memset(&serverAddr, 0, sizeof(SOCKADDR_IN));
	serverAddr.sin_family = AF_INET;
	// htons : host to network short  : 모든 자료형은 network short 구조로 송수신 한다? 
	serverAddr.sin_port = htons(SERVER_PORT);				// 서버 포트 : 3500
	serverAddr.sin_addr.S_un.S_addr = inet_pton(AF_INET, SERVER_IP, buffer);
	
	// 이 소켓을 serverAddr의 주소에 연결해라 
	connect(listenSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	// 옛날 API
	// 표준이 IPv6를 사용하게끔 되어있다.


	while (true)
	{
		char messageBuffer[MAX_BUFFER] = {};
		cin >> messageBuffer;
		int bufferLen = strlen(messageBuffer);
		int sendByte = send(listenSocket, messageBuffer, bufferLen, 0);

		if (sendByte > 0)
		{
			cout << "TRACE - Send message : " << messageBuffer << "\t(" << sendByte << "bytes)" << endl;

			// recvBytes만 의미있는 데이터이다.
			int recvBytes = recv(listenSocket, messageBuffer, MAX_BUFFER, 0);
			// 만약 1000byte 중에 200byte만 받으면 나머지 데이터는 날아가는 것이 아니라 recv를 여러번 해주면된다.
			// 서버가 데이터를 보내줄 때까지 멈춰있다.  -> Blocking Call, Blocking I/O, Synchronous I/O
			
			if (recvBytes > 0)
			{
				cout << "TRACE - recv message : " << messageBuffer << "\t(" << recvBytes << "bytes)" << endl;
			}
		}		
	}

	closesocket(listenSocket);
	// 윈도우는 끝날 때에도 WSACleanup()을 꼭 호출해야한다.
	WSACleanup();
}