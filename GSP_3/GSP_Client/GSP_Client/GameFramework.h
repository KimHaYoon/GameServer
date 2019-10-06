#pragma once
#include "main.h"

class CGameFramework
{
private:
	HDC		m_hDC;
	HWND m_hWnd;
	class CTexture* m_pBackBuffer;

	bool m_bServer;

public:
	CGameFramework();
	~CGameFramework();

public:
	void Logic();
	bool Init( HWND hWnd, HINSTANCE  hInst );
	void ServerOn( const string& strServerIP );

private:
	void Input();
	void Update();
	void Render();
};

