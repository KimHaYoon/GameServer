#include <iostream> 
#include <map> 
#include <thread>
using namespace std;

#include <WS2tcpip.h> 
#pragma comment(lib, "Ws2_32.lib") 

#define MAX_BUFFER 1024
#define SERVER_PORT 3500

struct OVER_EX {      // send�Ҷ����� overlapped ����ü�� ���� �ؾ��Ѵ�.
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

// ���� ���� ��� �͵��� ���̵� ���� ����?
// ��Ƽ Ŭ���� ���� ������ ���̵� �־� ����
// ä�ÿ��� �޼����� �� ���� ������ ���̵� ���� ����
// �׷��� id�� �����ϸ� �ȵ� -> �̺κе� ���ľ��� �� �����g.. id ���� �ϴϿ�..
//	-> id�� ���� �÷��̾��� ��� �������� �����ϱ⶧���� �����ϸ� ū�ϳ�..!


map <SOCKET, SOCKETINFO> clients;			// ��������� �� �ڿ��� ������ �� mutex�� �ؾ���
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
	std::wcout << L" ���� " << lpMsgBuf << std::endl;
	while (true);			// ����󿡼��Ͻ����� ���
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

		GetQueuedCompletionStatus(g_iocp, &num_byte, (PULONG_PTR)p_key, &p_over, INFINITE); //  ���ʿ��� �ش� �����带 ������ Ǯ �ȿ� ����ϴ� ���
															  //   �ι�° ���ʹ� ������ Ǯ�κ��� �����带 �����ͼ� iocp�� ���� �����͸� �޴´�?

		OVER_EX* over_ex = reinterpret_cast<OVER_EX*>(p_over);   // over_ex����ü�� �޾ƿ� �����͸� �ִ´�

		if (true == over_ex->is_recv) {
			SOCKET client_s = static_cast<int>(key);

			if (num_byte == 0) {   //������������ Ȯ��
				closesocket(client_s);
				clients.erase(client_s);
				continue;
			}





			// Ŭ���̾�Ʈ�� closesocket�� ���� ���
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

			//send�� ��ٸ����ʰ� ���ú���·� ���ư��� ������ recv�� overlapped ����ü�� ���� �������Ѵ�
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

			// Ŭ���̾�Ʈ�� closesocket�� ���� ���
			cout << "TRACE - SEND message [ " << client_s << "] : ";
			cout << over_ex->net_buf << " (" << num_byte << " bytes)\n";

			delete p_over;      // p_over�� delete�ؾ� send�ߴٴ� �ǹ��̴�
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

	g_iocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, 0);   // �ڵ鰪�� ���Ϲ޾Ƽ� icop�� ����ϴ� �ܰ�
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
		CreateIoCompletionPort(reinterpret_cast<HANDLE>(clientSocket), g_iocp, clientSocket, 0);   // iocp�� ������ ����ϴ� �ܰ�
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