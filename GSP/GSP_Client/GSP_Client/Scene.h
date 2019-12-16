#pragma once
#include "main.h"

class CScene
{
public:
	bool Init();

public:
	void AddKing(int iID, POS tPos);
	void DeleteKing(int iID);

	DECLARE_SINGLE( CScene )
};

