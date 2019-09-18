#include "Rect.h"
#include "Texture.h"


CRect::CRect()
{
}


CRect::~CRect()
{
}

bool CRect::Init()
{
	m_tSize = _SIZE( 100, 100);

	return true;
}

void CRect::Input()
{
	CObj::Input();
}

void CRect::Update()
{
	CObj::Update( );
}

void CRect::Render( HDC hDC )
{
	CObj::Render( hDC );

	HBRUSH Brush, OldBrush;
	Brush = CreateSolidBrush( RGB( 255, 255, 255) );
	OldBrush = ( HBRUSH )SelectObject( hDC, Brush );
	Rectangle( hDC, m_tPos.x, m_tPos.y, m_tPos.x + m_tSize.x, m_tPos.y + m_tSize.y );

	SelectObject( hDC, OldBrush );
	DeleteObject( Brush );
}
