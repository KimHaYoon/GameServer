#include "Client.h"



CClient::CClient()
{
}


CClient::~CClient()
{
	m_pSocketInfo->socket
	SAFE_DELETE( m_pSocketInfo );
}

bool CClient::Init( OVER_EX recv_over, SOCKET socket, int id, bool is_connected, bool is_active, POS pos )
{
	return false;
}
