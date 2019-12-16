#include "ClientManager.h"

DEFINITION_SINGLE(CClientManager)


CClientManager::CClientManager()
{
}


CClientManager::~CClientManager()
{
}

bool CClientManager::Init()
{
	return true;
}

void CClientManager::AddClient( OVER_EX recv_over, SOCKET socket, int id, bool is_connected, bool is_active, POS pos )
{
}
