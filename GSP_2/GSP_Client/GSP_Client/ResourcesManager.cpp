#include "ResourcesManager.h"
#include "Texture.h"

DEFINITION_SINGLE(CResourcesManager)

CResourcesManager::CResourcesManager()
{
}

CResourcesManager::~CResourcesManager()
{
	unordered_map<string, class CTexture*>::iterator	iter = m_mapTexture.begin();
	unordered_map<string, class CTexture*>::iterator	iterEnd = m_mapTexture.end();

	while ( iter != iterEnd )
	{
		delete iter->second;
		iter->second = NULL;
		++iter;
	}

	m_mapTexture.clear();
}

bool CResourcesManager::Init(HINSTANCE hInst, HDC hDC)
{
	m_hInst = hInst;
	m_hDC = hDC;

	CTexture*	pTexture = LoadTexture("BackBuffer", L"Texture/BackBuffer.bmp");
	
	return true;
}

CTexture * CResourcesManager::LoadTexture(const string & strKey, const wchar_t * pFileName, bool bColorKey, COLORREF dwColorKey)
{
	CTexture* pTexture = FindTexture(strKey);

	if (pTexture)
		return pTexture;

	pTexture = new CTexture;

	if (!pTexture->LoadTexture(strKey, m_hInst, m_hDC, pFileName, bColorKey,
		dwColorKey))
	{
		delete pTexture;
		pTexture = NULL;

		return pTexture;
	}

	m_mapTexture.insert(make_pair(strKey, pTexture));

	return pTexture;
}

CTexture * CResourcesManager::FindTexture(const string & strKey)
{
	unordered_map<string, CTexture*>::iterator	iter = m_mapTexture.find(strKey);

	if (iter == m_mapTexture.end())
		return NULL;

	return iter->second;
}
