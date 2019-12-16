#pragma once
#include "main.h"

class CNetwork
{
	SOCKET m_Sock;
	char	m_cKey;

	PLAYERINFO			m_tPlayerInfo;

	int			m_iCurPlayerSize;
	int			m_iPrePlayerSize;

	map<int, PLAYERINFO> m_Players;

public:
	bool Init( const string& strServerIP = "127.0.0.1" );
	void Update();
	void Render( HDC hDC );

	void SetKey( char cKey );
	POS GetPlayerStartPos() const;
	int	GetMyID()	const;

	DECLARE_SINGLE( CNetwork )
};

