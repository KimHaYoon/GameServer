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
		SAFE_DELETE( *iter );
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
		++iter;
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

//CObj * CObjectManager::FindObject( const string & strName )
//{
//	auto iter = find_if( m_ObjList.begin(), m_ObjList.end(), [&strName](CObj* a) {
//		if ( a->GetTag() == strName )
//			return a;
//	} );
//
//	if ( iter == m_ObjList.end() )
//		return NULL;
//
//	return *iter;
//}
