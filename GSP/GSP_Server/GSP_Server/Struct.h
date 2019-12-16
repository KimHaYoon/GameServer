#pragma once

enum EVENT_TYPE
{
	EV_RECV,
	EV_SEND,
	EV_MOVE,
	EV_PLAYER_MOVE_NOTIFY,
	EV_MOVE_TARGET,
	EV_ATTACK,
	EV_HEAL
};

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

struct OVER_EX
{
	WSAOVERLAPPED	over;
	WSABUF			wsabuf[1];
	char			net_buf[MAX_BUFFER];
	EVENT_TYPE		event_type;
};

struct SOCKETINFO
{
	OVER_EX		recv_over;
	SOCKET		socket;
	int			id;
	bool		is_connected;
	bool		is_active;
	POS			pos;
	set<int>	near_id;
	mutex		near_lock;
	lua_State*	L;
	mutex		vm_lock;
};

struct EVENT
{
	int obj_id;
	high_resolution_clock::time_point wakeup_time;
	int	event_type;
	int target_obj;

	constexpr bool operator <( const EVENT& left ) const
	{
		return wakeup_time > left.wakeup_time;
	}
};