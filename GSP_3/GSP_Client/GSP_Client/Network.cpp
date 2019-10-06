#include "Network.h"
#include "ObjectManager.h"
#include "Obj.h"
#include "Scene.h"

#define  SERVER_PORT 3500
#define MAX_BUFFER 1024

DEFINITION_SINGLE( CNetwork )

CNetwork::CNetwork()
{
	m_cKey = 0;
}


CNetwork::~CNetwork()
{
	closesocket( m_Sock );
	WSACleanup();
}

bool CNetwork::Init( const string& strServerIP )
{
	WSADATA WSAData;
	WSAStartup( MAKEWORD( 2, 2 ), &WSAData );
	m_Sock = WSASocket( AF_INET, SOCK_STREAM, 0, NULL, 0, 0 );
	sockaddr_in serverAddr;
	memset( &serverAddr, 0, sizeof( SOCKADDR_IN ) );
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons( SERVER_PORT );
	inet_pton( AF_INET, strServerIP.c_str(), &serverAddr.sin_addr );
	int iCheck = connect( m_Sock, ( struct sockaddr * )&serverAddr, sizeof( serverAddr ) );

	if ( iCheck == 0 )
	{
		recv( m_Sock, ( char* )&m_tPlayerInfo, sizeof( PLAYERINFO ), 0 );
		m_Players[m_tPlayerInfo.iID] = m_tPlayerInfo;
		GET_SINGLE( CScene )->AddKing( m_tPlayerInfo.iID, m_tPlayerInfo.tPos);
		m_iCurPlayerSize = 1;

		return true;
	}

	return false;
}

void CNetwork::Update()
{
	KEY tKey{ m_tPlayerInfo.iID,m_cKey };
	send( m_Sock, (char*)&tKey, sizeof( KEY ), 0 );

	cout << m_cKey << endl;

	m_iPrePlayerSize = m_iCurPlayerSize;

	PLAYERINFO tPlayers[10]{};
	recv( m_Sock, ( char* )&tPlayers, sizeof( tPlayers ), 0 );

	for ( int i = 0; i < 10; ++i )
	{
		if ( tPlayers[i].bLogin)
			GET_SINGLE( CScene )->AddKing( tPlayers[i].iID, tPlayers[i].tPos );
	}
}

void CNetwork::Render( HDC hDC )
{
}

void CNetwork::SetKey( char cKey)
{
	m_cKey = cKey;
}

POS CNetwork::GetPlayerStartPos() const
{
	return m_tPlayerInfo.tPos;
}

int CNetwork::GetMyID() const
{
	return m_tPlayerInfo.iID;
}
