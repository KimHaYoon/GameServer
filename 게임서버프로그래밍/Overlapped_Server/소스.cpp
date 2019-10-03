#pragma warning (disable:4996)

#include <iostream>
#include <map>
using namespace std;
#include <WS2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")
#define MAX_BUFFER 1024
#define SERVER_PORT 3500

struct SOCKETINFO {
	WSAOVERLAPPED overlapped;					// Window가 사용하는 자료구조, 운영체제가 자체적으로 사용하기 때문에 하나 만들어서 초기화한 후 넣어줘야 한다.
	WSABUF dataBuffer;							// 실제 데이터를 저장할 버퍼뿐만 아니라 WSA버퍼도 필요하다. 
	SOCKET socket;
	char messageBuffer[MAX_BUFFER];				// 데이터가 아직 도착하지 않아도 버퍼를 운영체제에 등록을 해야한다. 그래서 소켓마다 데이터를 받을 버퍼를 미리 가지고 있는다.
};

map <SOCKET, SOCKETINFO> clients;
void CALLBACK recv_callback(DWORD Error, DWORD dataBytes, LPWSAOVERLAPPED overlapped, DWORD lnFlags);
void CALLBACK send_callback(DWORD Error, DWORD dataBytes, LPWSAOVERLAPPED overlapped, DWORD lnFlags);
void CALLBACK recv_callback(DWORD Error, DWORD dataBytes, LPWSAOVERLAPPED overlapped, DWORD lnFlags);

void CALLBACK recv_callback(DWORD Error, DWORD dataBytes, LPWSAOVERLAPPED overlapped, DWORD lnFlags)
{
	SOCKET client_s = reinterpret_cast<int>(overlapped->hEvent);
	if (dataBytes == 0) {
		closesocket(clients[client_s].socket);
		clients.erase(client_s);
		return;
	} // 클라이언트가 closesocket을 했을 경우
	clients[client_s].messageBuffer[dataBytes] = 0;
	cout << "From client : " << clients[client_s].messageBuffer << " (" << dataBytes << ") bytes)\n";
	clients[client_s].dataBuffer.len = dataBytes;
	memset(&(clients[client_s].overlapped), 0x00, sizeof(WSAOVERLAPPED));		// 0으로 초기화해준다. 초기화를 안한다면 이상한 값이 들어가서 오동작을 발생하기 때문에 꼭! 해줘야한다.
	clients[client_s].overlapped.hEvent = (HANDLE)client_s;
	WSASend(client_s, &(clients[client_s].dataBuffer), 1, &dataBytes, 0, &(clients[client_s].overlapped), send_callback);
}

void CALLBACK send_callback(DWORD Error, DWORD dataBytes, LPWSAOVERLAPPED overlapped, DWORD lnFlags)
{
	DWORD receiveBytes = 0;
	DWORD flags = 0;
	SOCKET client_s = reinterpret_cast<int>(overlapped->hEvent);
	if (dataBytes == 0) {
		closesocket(clients[client_s].socket);
		clients.erase(client_s);
		return;
	} // 클라이언트가 closesocket을 했을 경우
	clients[client_s].dataBuffer.len = MAX_BUFFER;
	clients[client_s].dataBuffer.buf = clients[client_s].messageBuffer;
	cout << "Sent : " << clients[client_s].messageBuffer << " (" << dataBytes << " bytes)\n";
	memset(&(clients[client_s].overlapped), 0x00, sizeof(WSAOVERLAPPED));
	clients[client_s].overlapped.hEvent = (HANDLE)client_s;
	WSARecv(client_s, &clients[client_s].dataBuffer, 1, 0, &flags,
		&(clients[client_s].overlapped), recv_callback);
}

int main()
{
	WSADATA WSAData;
	WSAStartup(MAKEWORD(2, 2), &WSAData);
	SOCKET listenSocket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);				// 플래그를 꼭 추가해주어야 overlapped I/O가 작동을 한다.
	SOCKADDR_IN serverAddr;
	memset(&serverAddr, 0, sizeof(SOCKADDR_IN));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(SERVER_PORT);
	serverAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	::bind(listenSocket, (struct sockaddr*)&serverAddr, sizeof(SOCKADDR_IN));
	listen(listenSocket, 5);
	SOCKADDR_IN clientAddr;
	int addrLen = sizeof(SOCKADDR_IN);
	memset(&clientAddr, 0, addrLen);
	SOCKET clientSocket;
	DWORD flags;
	while (true) {
		clientSocket = accept(listenSocket, (struct sockaddr *)&clientAddr, &addrLen);
		clients[clientSocket] = SOCKETINFO{};
		memset(&clients[clientSocket], 0, sizeof(struct SOCKETINFO));
		clients[clientSocket].socket = clientSocket;
		clients[clientSocket].dataBuffer.len = MAX_BUFFER;
		clients[clientSocket].dataBuffer.buf = clients[clientSocket].messageBuffer;
		flags = 0;
		clients[clientSocket].overlapped.hEvent = (HANDLE)clients[clientSocket].socket;		// 오버랩트 구조체를 건들이면 안됐는데 건들이네? 오잉?
		WSARecv(clients[clientSocket].socket, &clients[clientSocket].dataBuffer, 1, NULL,
			&flags, &(clients[clientSocket].overlapped), recv_callback);
	}
	closesocket(listenSocket);
	WSACleanup();
}