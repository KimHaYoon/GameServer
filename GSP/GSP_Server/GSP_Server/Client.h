#pragma once
#include "main.h"

class CClient
{
private:
	friend class CClientManager;

private:
	SOCKETINFO* m_pSocketInfo;

private:
	CClient();
	~CClient();

public:
	bool Init( OVER_EX recv_over, SOCKET socket, int id, bool is_connected, bool is_active, POS pos );
};

