#include "Scene.h"
#include "ObjectManager.h"
#include "Rect.h"
#include "Obj.h"
#include "King.h"
#include "Network.h"

DEFINITION_SINGLE( CScene )

CScene::CScene()
{
}

CScene::~CScene()
{
}

bool CScene::Init()
{
	for ( int i = 0; i < 8; ++i )
	{
		for ( int j = 0; j < 8; ++j )
		{
			string strTag = "Board" + to_string(j + i * 4);
			
			CObj* pObj = GET_SINGLE( CObjectManager )->CreateObject<CRect>( strTag );
			_SIZE tSize = pObj->GetSize();
			pObj->SetPos(j * tSize.x, i * tSize.y);

			if ( 0 == i % 2 )
			{
				if ( 1 == ( j + i * 4 ) % 2 )
				{
					( (CRect* )pObj )->SetColor( RGB( 255, 205, 160 ) );
				}

				else
				{
					( (CRect* )pObj )->SetColor( RGB( 208, 139, 72) );
				}
			}

			else
			{
				if ( 0 == ( j + i * 4 ) % 2 )
				{
					( (CRect* )pObj )->SetColor( RGB( 255, 205, 160 ) );
				}
				
				else
				{
					( (CRect* )pObj )->SetColor( RGB( 208, 139, 72 ) );
				}
			}
		}
	}
	return true;
}

void CScene::AddKing( int iID, POS tPos )
{
	CObj* pObj = GET_SINGLE( CObjectManager )->FindObject( "King" + to_string( iID ) );
	
	if(!pObj )
		pObj = GET_SINGLE( CObjectManager )->CreateObject<CKing>( "King" + to_string(iID) );

	pObj->SetPos( tPos.x, tPos.y );
}

void CScene::DeleteKing(int iID)
{
	CObj* pObj = GET_SINGLE(CObjectManager)->FindObject("King" + to_string(iID));

	if(pObj)
		pObj->Die();
}
