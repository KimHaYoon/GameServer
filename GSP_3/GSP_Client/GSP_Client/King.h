#pragma once
#include "Obj.h"
class CKing :
	public CObj
{
public:
	CKing();
	~CKing();

	virtual bool Init();
	virtual void Input();
	virtual void Update();
	virtual void Render( HDC hDC );
};

