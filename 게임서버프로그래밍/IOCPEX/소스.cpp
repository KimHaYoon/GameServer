#include <iostream> 
#include <map> 
#include <thread>
using namespace std;

#include <WS2tcpip.h> 
#pragma comment(lib, "Ws2_32.lib") 

#define MAX_BUFFER 1024
#define SERVER_PORT 3500

struct OVER_EX {      // send할때마다 overlapped 구조체를 만들어서 해야한다.
	WSAOVERLAPPED over;
	WSABUF wsabuf[1];
	char net_buf[MAX_BUFFER];
	bool is_recv;
};

struct SOCKETINFO {
	OVER_EX recv_over;
	SOCKET socket;
	//char p_buf[MAX_BUFFER];
	//int preve;
	int id;
};

// 게임 내의 모든 것들은 아이디를 통해 접근?
// 파티 클래스 내에 유저의 아이디를 넣어 관리
// 채팅에서 메세지를 줄 때도 유저의 아이디를 통해 접근
// 그래서 id를 재사용하면 안됨 -> 이부분도 고쳐야할 듯 따후훕.. id 재사용 니니연..
//	-> id를 통해 플레이어의 모든 정보들을 접근하기때문에 재사용하면 큰일남..!


map <SOCKET, SOCKETINFO> clients;			// 스레드들이 이 자원을 접근할 때 mutex를 해야함
HANDLE g_iocp;

void error_display(char* msg, int err_no)
{
	WCHAR * lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, err_no,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)& lpMsgBuf, 0, NULL);
	std::cout << msg;
	std::wcout << L" 에러 " << lpMsgBuf << std::endl;
	while (true);			// 비쥬얼에서일시정지 기능
	LocalFree(lpMsgBuf);
}

void do_worker()
{
	while (true)
	{
		DWORD num_byte;
		ULONG key;
		PULONG p_key = &key;
		WSAOVERLAPPED* p_over;

		GetQueuedCompletionStatus(g_iocp, &num_byte, (PULONG_PTR)p_key, &p_over, INFINITE); //  최초에는 해당 쓰레드를 쓰레드 풀 안에 등록하는 기능
															  //   두번째 부터는 쓰레드 풀로부터 쓰레드를 가져와서 iocp를 통해 데이터를 받는다?

		OVER_EX* over_ex = reinterpret_cast<OVER_EX*>(p_over);   // over_ex구조체로 받아온 데이터를 넣는다

		if (true == over_ex->is_recv) {
			SOCKET client_s = static_cast<int>(key);

			if (num_byte == 0) {   //접속종료인지 확인
				closesocket(client_s);
				clients.erase(client_s);
				continue;
			}





			// 클라이언트가 closesocket을 했을 경우
			over_ex->net_buf[num_byte] = 0;
			cout << "From client[" << client_s << "] : ";
			cout << over_ex->net_buf << " (" << num_byte << ") bytes)\n";

			OVER_EX* send_over = new OVER_EX;
			memset(send_over, 0x00, sizeof(OVER_EX));
			send_over->is_recv = false;
			memcpy(send_over->net_buf, over_ex->net_buf, num_byte);
			send_over->wsabuf[0].buf = send_over->net_buf;
			send_over->wsabuf[0].len = num_byte;
			WSASend(client_s, send_over->wsabuf, 1, 0, 0, &send_over->over, 0);
			DWORD flags = 0;
			memset(&over_ex->over, 0x00, sizeof(WSAOVERLAPPED));

			//send를 기다리지않고 리시브상태로 돌아가기 때문에 recv용 overlapped 구조체를 새로 만들어야한다
			WSARecv(client_s, over_ex->wsabuf, 1, 0, &flags, &over_ex->over, 0);
		}
		else {
			SOCKET client_s = static_cast<int>(key);

			if (num_byte == 0) {
				closesocket(client_s);
				clients.erase(client_s);
				delete p_over;
				continue;
			}

			// 클라이언트가 closesocket을 했을 경우
			cout << "TRACE - SEND message [ " << client_s << "] : ";
			cout << over_ex->net_buf << " (" << num_byte << " bytes)\n";

			delete p_over;      // p_over를 delete해야 send했다는 의미이다
		}
	}
}

int main()
{
	wcout.imbue(locale("korean"));
	WSADATA WSAData;
	WSAStartup(MAKEWORD(2, 2), &WSAData);
	SOCKET listenSocket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
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

	g_iocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, 0);   // 핸들값을 리턴받아서 icop를 등록하는 단계
	thread worker_thread{ do_worker };
	thread worker_thread2{ do_worker };
	thread worker_thread3{ do_worker };
	while (true)
	{
		clientSocket = accept(listenSocket, (struct sockaddr *)&clientAddr, &addrLen);
		clients[clientSocket] = SOCKETINFO{};
		memset(&clients[clientSocket], 0, sizeof(struct SOCKETINFO));
		clients[clientSocket].socket = clientSocket;
		clients[clientSocket].recv_over.wsabuf[0].len = MAX_BUFFER;
		clients[clientSocket].recv_over.wsabuf[0].buf = clients[clientSocket].recv_over.net_buf;
		clients[clientSocket].recv_over.is_recv = true;
		flags = 0;
		CreateIoCompletionPort(reinterpret_cast<HANDLE>(clientSocket), g_iocp, clientSocket, 0);   // iocp에 소켓을 등록하는 단계
		int ret = WSARecv(clientSocket, clients[clientSocket].recv_over.wsabuf, 1, NULL,
			&flags, &(clients[clientSocket].recv_over.over), NULL);

		if (0 != ret)
		{
			int err_no = WSAGetLastError();
			if (WSA_IO_PENDING != err_no)
				error_display("WSARecv_Error : ", err_no);
		}
	}
	worker_thread.join();

	closesocket(listenSocket);
	WSACleanup();
}