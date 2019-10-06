#pragma once

#include "Obj.h"
class CRect :
	public CObj
{
private:
	COLORREF m_dwColorKey;

public:
	CRect();
	~CRect();

	// CObj을(를) 통해 상속됨
	virtual bool Init();
	virtual void Input();
	virtual void Update();
	virtual void Render( HDC hDC );

public:
	void SetColor( COLORREF dwColorKey );
};

