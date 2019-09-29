#pragma once
#include "main.h"

class CNetwork
{
	SOCKET m_Sock;
	char	m_cKey;

	POS			m_tPlayerStartPos;

public:
	bool Init( const string& strServerIP = "127.0.0.1" );
	void Update();
	void Render( HDC hDC );

	void SetKey( char cKey );
	POS GetPlayerStartPos() const;
	DECLARE_SINGLE( CNetwork )
};

