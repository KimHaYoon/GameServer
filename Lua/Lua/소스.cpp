#include <iostream>

extern "C"
{
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

using namespace std;

void lua_error( lua_State* L, const char* fmt, ... );
int addnum_c( lua_State* L );

int main()
{
	int rows, cols;
	char buf[256];
	lua_State* L = luaL_newstate();
	luaL_openlibs( L );
	luaL_loadfile( L, "Dragon.lua" );

	lua_register( L, "addnum_c", addnum_c );			// 등록할 함수를 미리 선언해주고 루아를 실행한다.

	if ( 0 != lua_pcall( L, 0, 0, 0 ) )
	{
		lua_error( L, "error running function dragon.lua : %s", lua_tostring( L, -1 ) );
	}


	lua_getglobal( L, "cols" );
	int result = ( int )lua_tonumber( L, -1 );

	lua_pop( L, 1 );
	cout << "Result = " << result << endl;
	lua_close( L );


	/*lua_getglobal( L, "plustwo" );
	lua_pushnumber( L, 0 );
	if ( 0 != lua_pcall( L, 1, 1, 0 ) )
	{
		lua_error( L, "error running function plustwo : %s", lua_tostring( L, -1 ) );
	}

	int result = ( int )lua_tonumber( L, -1 );

	lua_pop( L, 1 );
	cout << "Result = " << result << endl;
	lua_close( L );*/

	//lua_getglobal( L, "rows" );
	//rows = ( int )lua_tonumber( L, -1 );			// 맨 위에있는 값을 꺼내서 넘겨라
	//lua_pop( L, 1 );

	//lua_getglobal( L, "cols" );
	//cols = ( int )lua_tonumber( L, -1 );
	//lua_pop( L, 1 );

	//cout << "Cols = " << cols << ", Rows = " << rows << endl;

	//lua_close( L );

	/*while ( NULL != fgets( buf, sizeof( buf ), stdin ) )
	{
		luaL_loadbuffer( L, buf, strlen( buf ), "line" );
		lua_pcall( L, 0, 0, 0 );
	}
	lua_close( L );*/

	system( "pause" );
}

void lua_error( lua_State * L, const char * fmt, ... )
{
	va_list argp;
	va_start( argp, fmt );

	vfprintf( stderr, fmt, argp );
	va_end( argp );

	lua_close( L );

	while ( true ) { }

	exit( EXIT_FAILURE );

}

int addnum_c( lua_State* L )
{
	int a = ( int )lua_tonumber( L, -2 );
	int b = ( int )lua_tonumber( L, -1 );
	lua_pop( L, 3 );				// 파라미터 뿐만아니라 함수도 꺼내줘야하기 때문에 3개를 꺼낸다.
	lua_pushnumber( L, a + b );

	return 1;
}
