#include "Circle.h"
#include "Input.h"


CCircle::CCircle()
{
}


CCircle::~CCircle()
{
}

bool CCircle::Init()
{
	m_tPos = POS( 100, 100 );
	m_tSize = _SIZE( 100, 100 );
	return true;
}

void CCircle::Input()
{
	CObj::Input();

	if ( KEYDOWN( "MoveLeft" ) )
	{
		if ( m_tPos.x > 100 )
			m_tPos.x -= 100;
	}

	if ( KEYDOWN( "MoveRight" ) )
	{
		if ( m_tPos.x < 800 )
			m_tPos.x += 100;
	}

	if ( KEYDOWN( "MoveUp" ) )
	{
		if ( m_tPos.y > 100 )
		m_tPos.y -= 100;
	}

	if ( KEYDOWN( "MoveDown" ) )
	{
		if ( m_tPos.y < 800 )
			m_tPos.y += 100;
	}
}

void CCircle::Update()
{
	CObj::Update();
}

void CCircle::Render( HDC hDC )
{
	CObj::Render( hDC );

	HBRUSH Brush, OldBrush;
	Brush = CreateSolidBrush( RGB( 255, 0, 255 ) );
	OldBrush = ( HBRUSH )SelectObject( hDC, Brush );
	Ellipse( hDC, m_tPos.x, m_tPos.y, m_tPos.x + m_tSize.x, m_tPos.y + m_tSize.y );

	SelectObject( hDC, OldBrush );
	DeleteObject( Brush );
}
