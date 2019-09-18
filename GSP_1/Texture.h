#pragma once
#include "main.h"
class CTexture 
{
private:
	friend class CResourcesManager;

public:
	CTexture();
	~CTexture();

private:
	wstring		m_strPath;
	wstring		m_strFileName;
	wstring		m_strFullPath;
	string		m_strKey;

private:
	HDC			m_hMemDC;
	HBITMAP		m_hBmp;
	HBITMAP		m_hOldBmp;
	BITMAP		m_tInfo;
	bool		m_bColorKey;
	COLORREF	m_dwColorKey;

public:
	UINT GetWidth()	const
	{
		return m_tInfo.bmWidth;
	}

	UINT GetHeight()	const
	{
		return m_tInfo.bmHeight;
	}

	HDC GetMemDC()	const
	{
		return m_hMemDC;
	}
	
	bool GetColorKeyEnable()	const
	{
		return m_bColorKey;
	}

	COLORREF GetColorKey()	const
	{
		return m_dwColorKey;
	}

	void* GetPixelData()	const
	{
		return m_tInfo.bmBits;
	}

	string GetKey() const
	{
		return m_strKey;
	}

	wstring GetFullPath()	const
	{
		return m_strFullPath;
	}


public:
	bool LoadTexture(const string& strKey, HINSTANCE hInst, HDC hDC,
		const wchar_t* pFullPath, bool bColorKey = false, COLORREF dwColorKey = RGB(255, 0, 255));
};

