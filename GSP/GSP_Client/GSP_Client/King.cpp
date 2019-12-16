#include "King.h"
#include "Input.h"
#include "ObjectManager.h"
#include "Network.h"

CKing::CKing()
{
}

CKing::~CKing()
{
}

bool CKing::Init()
{
	m_tPos = POS( 125, 100 );
	m_tSize = _SIZE( 45, 100 );

	SetTexture( "King", L"Texture/King.bmp", true );
	return true;
}

void CKing::Input()
{
	CObj::Input();
	CObj* pBoard = GET_SINGLE( CObjectManager )->FindObject( "Board0" );

	GET_SINGLE( CNetwork )->SetKey( KEY_NONE );

	if ( KEYDOWN( "MoveLeft" ) )
	{
		if ( m_tPos.x > 100 )
			GET_SINGLE( CNetwork )->SetKey( KEY_LEFT );
	}

	if ( KEYDOWN( "MoveRight" ) )
	{
		if ( m_tPos.x < 700 )
			GET_SINGLE( CNetwork )->SetKey( KEY_RIGHT );
	}

	if ( KEYDOWN( "MoveUp" ) )
	{
		if ( m_tPos.y > 0 )
			GET_SINGLE( CNetwork )->SetKey( KEY_UP );
	}

	if ( KEYDOWN( "MoveDown" ) )
	{
		if ( m_tPos.y < 700 )
			GET_SINGLE( CNetwork )->SetKey( KEY_DOWN );
	}
}

void CKing::Update()
{
	CObj::Update();
}

void CKing::Render( HDC hDC )
{
	CObj::Render( hDC );
	
	string strMyKing = "King" + to_string( GET_SINGLE( CNetwork )->GetMyID() );

	if ( m_strTag == strMyKing )
		TextOutA( hDC, m_tPos.x + 15, m_tPos.y, "Me", 2);

	else
		TextOutA( hDC, m_tPos.x + 2, m_tPos.y, m_strTag.c_str(), m_strTag.size() );
}
