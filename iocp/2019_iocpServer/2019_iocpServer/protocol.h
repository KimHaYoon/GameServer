#pragma once

constexpr int MAX_STR_LEN = 50;

#define WORLD_WIDTH		800
#define WORLD_HEIGHT	800

#define SERVER_PORT		3500

#define SINGLE_ID		1

#define NPC_ID_START	10000
#define NUM_NPC			100

#define CS_UP		1
#define CS_DOWN		2
#define CS_LEFT		3
#define CS_RIGHT	4

#define SC_LOGIN_OK			1
#define SC_PUT_PLAYER		2
#define SC_REMOVE_PLAYER	3
#define SC_POS				4
#define SC_CHAT				5

#define MAX_STR_SIZE 100


#pragma pack(push ,1)	// 밑에 정의된 자료구조들을 1바이트단위로 패킹하라는 명령 이걸 사용하지않으면 구조체 사이즈를 컴파일러가 
						// 임의로 조정한다  cpu중에 인티저변수를 읽을때 4바이트씩 4의 배수로 읽어야하는 cpu들이 있다.

struct sc_packet_pos {
	char size;	// 패킷사이즈
	char type;	// 패킷 종류
	int id;
	short x, y;
};

struct sc_packet_remove_player {
	char size;
	char type;
	int id;
};

struct sc_packet_login_ok {
	char size;
	char type;
	int id;
	// 클라이언트에게 아바타의 아이디를 전해준다
};

struct sc_packet_put_player {
	char size;
	char type;
	int id;
	short x, y;
	// 렌더링 정보, 종족, 정보, 성별, 착용아이템, 캐릭터 외형, 이름, 길드
};

struct sc_packet_chat
{
	char size;
	char type;
	int id;
	char chat[50];
};

struct cs_packet_up {
	char	size;
	char	type;
};

struct cs_packet_down {
	char	size;
	char	type;
};

struct cs_packet_left {
	char	size;
	char	type;
};

struct cs_packet_right {
	char	size;
	char	type;
};

#pragma pack (pop)  // 위에 선언한 패킹상태를 원상태로 돌리는것