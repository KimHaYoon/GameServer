#include "Network.h"
#include "ObjectManager.h"
#include "Obj.h"

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
	WSAStartup( MAKEWORD( 2, 0 ), &WSAData );
	m_Sock = WSASocket( AF_INET, SOCK_STREAM, 0, NULL, 0, 0 );
	sockaddr_in serverAddr;
	memset( &serverAddr, 0, sizeof( SOCKADDR_IN ) );
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons( SERVER_PORT );
	inet_pton( AF_INET, strServerIP.c_str(), &serverAddr.sin_addr );
	int iCheck = connect( m_Sock, ( struct sockaddr * )&serverAddr, sizeof( serverAddr ) );
	if ( iCheck == 0 )
	{
		recv( m_Sock, ( char* )&m_tPlayerStartPos, sizeof( POS ), 0 );
		return true;
	}

	return false;
}

void CNetwork::Update()
{
	send( m_Sock, &m_cKey, sizeof(m_cKey), 0 );

	POS tPos{  };
	recv( m_Sock, (char*)&tPos, sizeof(tPos), 0 );

	CObj* pObj = GET_SINGLE( CObjectManager )->FindObject( "King" );

	if ( !pObj )
		return;

	pObj->SetPos( tPos.x, tPos.y );
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
	return m_tPlayerStartPos;
}