#include "Scene.h"
#include "ObjectManager.h"
#include "Rect.h"
#include "Obj.h"
#include "Circle.h"

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
			pObj->SetPos(100 + j * tSize.x, 100 + i * tSize.y);
		}
	}

	CObj* pObj = GET_SINGLE( CObjectManager )->CreateObject<CCircle>( "Circle" );


	return true;
}