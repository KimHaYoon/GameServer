#include "King.h"
#include "Input.h"
#include "ObjectManager.h"

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

	if ( KEYDOWN( "MoveLeft" ) )
	{
		if ( m_tPos.x > 100 )
			m_tPos.x -= pBoard->GetSize().x;
	}

	if ( KEYDOWN( "MoveRight" ) )
	{
		if ( m_tPos.x < 700 )
			m_tPos.x += pBoard->GetSize().x;
	}

	if ( KEYDOWN( "MoveUp" ) )
	{
		if ( m_tPos.y > 0 )
			m_tPos.y -= pBoard->GetSize().y;
	}

	if ( KEYDOWN( "MoveDown" ) )
	{
		if ( m_tPos.y < 700 )
			m_tPos.y += pBoard->GetSize().y;
	}
}

void CKing::Update()
{
	CObj::Update();
}

void CKing::Render( HDC hDC )
{
	CObj::Render( hDC );
}
