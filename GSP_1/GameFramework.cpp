#include "GameFramework.h"
#include "Input.h"
#include "ObjectManager.h"
#include "Texture.h"
#include "ResourcesManager.h"


CGameFramework::CGameFramework() :
	m_pBackBuffer(NULL)
{
}


CGameFramework::~CGameFramework()
{
}

void CGameFramework::Logic()
{
	Input();
	Update();
	Render();
}

bool CGameFramework::Init(HWND hWnd, HINSTANCE  hInst)
{
	m_hWnd = hWnd;
	m_hDC = GetDC( hWnd );

	if ( !GET_SINGLE( CInput )->Init( m_hWnd ) )
		return false;

	if ( !GET_SINGLE( CResourcesManager)->Init( hInst, m_hDC) )
		return false;

	m_pBackBuffer = GET_SINGLE( CResourcesManager )->FindTexture( "BackBuffer" );
}

void CGameFramework::Input()
{
	GET_SINGLE( CInput )->Update();
}

void CGameFramework::Update()
{
	GET_SINGLE( CObjectManager )->Update();
}

void CGameFramework::Render()
{
	GET_SINGLE( CObjectManager )->Render(m_pBackBuffer->GetMemDC());

	// 마지막으로 백버퍼를 화면에 그린다.
	BitBlt( m_hDC, 0, 0, WINX, WINY, m_pBackBuffer->GetMemDC(),
		0, 0, SRCCOPY );
}
