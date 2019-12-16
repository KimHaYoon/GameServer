#pragma once

#include "main.h"

class CClientManager
{
private:
	concurrent_unordered_map<int, class CClient*> m_mapClients;

public:
	bool Init();

public:
	void AddClient( OVER_EX recv_over, SOCKET socket, int id, bool is_connected, bool is_active, POS pos );

	DECLARE_SINGLE(CClientManager)
};

