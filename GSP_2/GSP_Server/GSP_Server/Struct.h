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

	void operator +=( const _tagPos& pos )
	{
		x += pos.x;
		y += pos.y;
	}
}POS, _SIZE;


typedef struct _tagSocketInfo
{
	WSAOVERLAPPED	overlapped;
	WSABUF						dataBuffer;
	SOCKET						socket;
	char							messageBuffer[MAX_BUFFER];
}SOCKETINFO;

typedef struct _tagPlayerInfo
{
	int			iID;
	bool		bLogin;
	POS			tPos;

	_tagPlayerInfo( int iID, bool bLogin, POS tPos ) : 
		iID{iID }, bLogin{bLogin },	tPos{tPos }
	{
		
	}

	_tagPlayerInfo()
	{
		bLogin = false;
	}
}PLAYERINFO;

typedef struct _tagKey
{
	int			iID;
	char		cKey;
}KEY;