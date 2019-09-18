#include "ObjectManager.h"
#include "Obj.h"

DEFINITION_SINGLE(CObjectManager)

CObjectManager::CObjectManager()
{
}

CObjectManager::~CObjectManager()
{
	list<class CObj*>::iterator	iter = m_ObjList.begin();
	list<class CObj*>::iterator	iterEnd = m_ObjList.end();

	while ( iter != iterEnd )
	{
		delete *iter;
		++iter;
	}

	m_ObjList.clear();
}

bool CObjectManager::Init()
{
	return true;
}

void CObjectManager::Input()
{
	list<CObj*>::iterator	iter;
	list<CObj*>::iterator	iterEnd = m_ObjList.end();

	for ( iter = m_ObjList.begin(); iter != iterEnd; ++iter )
	{
		( *iter )->Input( );
	}
}

void CObjectManager::Update()
{
	list<CObj*>::iterator	iter;
	list<CObj*>::iterator	iterEnd = m_ObjList.end();

	for ( iter = m_ObjList.begin(); iter != iterEnd;)
	{
		( *iter )->Update( );
	}
}

void CObjectManager::Render(HDC hDC)
{
	list<CObj*>::iterator	iter;
	list<CObj*>::iterator	iterEnd = m_ObjList.end();

	for ( iter = m_ObjList.begin(); iter != iterEnd;)
	{
		( *iter )->Render( hDC );
		++iter;
	}
}
