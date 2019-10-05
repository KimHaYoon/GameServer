#pragma once
#include "main.h"

class CScene
{
public:
	bool Init();

public:
	void AddKing(int iID, POS tPos);
	DECLARE_SINGLE( CScene )
};

