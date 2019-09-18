#pragma once

typedef struct _tagPos
{
	float x;
	float y;


	_tagPos() :
		x( 0.f ),
		y( 0.f )
	{
	}

	_tagPos( float _x, float _y ) :
		x( _x ),
		y( _y )
	{
	}
}POS, _SIZE;