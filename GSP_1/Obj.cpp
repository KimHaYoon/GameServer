#include "Obj.h"
#include "Texture.h"

CObj::CObj()
{
}

CTexture * CObj::GetTexture() const
{
	return nullptr;
}

void CObj::SetScene( CScene * pScene )
{
}

void CObj::SetTexture( const string & strKey, const wchar_t * pFullPath, bool bColorKey, COLORREF dwColorKey )
{
}

bool CObj::Init()
{
	return false;
}

void CObj::Input()
{
}

void CObj::Update()
{
}

void CObj::Render( HDC hDC )
{
	POS	tPos = m_tPos;

	if ( tPos.x + m_tSize.x< 0 ||
		tPos.x  > WINX ||
		tPos.y + m_tSize.y < 0 ||
		tPos.y  > WINY )
		return;

	if ( m_pTexture )
	{
		if ( !m_pTexture->GetColorKeyEnable() )
		{
			// BitBlt 함수는 특정 DC를 대상이 되는 DC에 출력해주는 함수이다.
			// 1번 인자는 대상이 되는 DC로 여기 DC에다가 출력하게 된다.
			// 2번, 3번 인자는 x, y인데 이 값은 출력 대상의 DC에서 좌표를 지정해준다.
			// 좌표는 좌 상단 점이 된다.
			// 4번, 5번 인자는 가로, 세로의 출력 크기이다.
			// 2, 3번 인자의 좌표로부터 4번, 5번 인자의 가로, 세로 크기만큼
			// 6번인자에 들어오는 DC를 그려낸다.
			// 7, 8번 인자는 이미지의 출력 시작점을 설정한다.
			int	x, y;
			x = tPos.x;
			y = tPos.y;

			UINT	iWidth, iHeight;
			iWidth = m_pTexture->GetWidth();
			iHeight = m_pTexture->GetHeight();

			UINT	iImageX = 0, iImageY = 0;

			BitBlt( hDC, x, y, iWidth, iHeight, m_pTexture->GetMemDC(), iImageX, iImageY, SRCCOPY );
		}

		else
		{
			// ColorKey를 제외하고 출력한다.
			int	x, y;
			x = tPos.x;
			y = tPos.y;

			UINT	iWidth, iHeight;
			iWidth = m_pTexture->GetWidth();
			iHeight = m_pTexture->GetHeight();

			UINT	iImageX = 0, iImageY = 0;

			TransparentBlt( hDC, x, y, iWidth, iHeight,
				m_pTexture->GetMemDC(), iImageX, iImageY, iWidth, iHeight,
				m_pTexture->GetColorKey() );
		}
	}
}
