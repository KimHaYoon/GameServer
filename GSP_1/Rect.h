#pragma once

#include "Obj.h"
class CRect :
	public CObj
{
public:
	CRect();
	~CRect();

	// CObj��(��) ���� ��ӵ�
	virtual bool Init();
	virtual void Input();
	virtual void Update();
	virtual void Render( HDC hDC );
};

