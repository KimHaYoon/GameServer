#pragma once

#include "main.h"

class CObj 
{
protected:
	friend class CObjectManager;

protected:
	CObj();
	virtual ~CObj() = 0;
	
protected:
	class CScene*	m_pScene;	
	class CTexture* 	m_pTexture;

protected:
	POS				m_tPos;
	_SIZE			m_tSize;

public:
	class CTexture* GetTexture()	const;

	POS GetPos()	const
	{
		return m_tPos;
	}

	_SIZE GetSize()	const
	{
		return m_tSize;
	}

public:
	void SetPos(float x, float y)
	{
		m_tPos = POS(x, y);
	}

	void SetSize(float x, float y)
	{
		m_tSize = _SIZE(x, y);
	}

public:
	class CScene* GetScene()	const
	{
		return m_pScene;
	}


public:
	void SetScene(class CScene* pScene);

public:
	void SetTexture(const string& strKey, const wchar_t* pFullPath = NULL,
		bool bColorKey = false, COLORREF dwColorKey = RGB(255, 0, 255));

public:
	virtual bool Init() = 0;
	virtual void Input() = 0;
	virtual void Update();
	virtual void Render(HDC hDC);
};

