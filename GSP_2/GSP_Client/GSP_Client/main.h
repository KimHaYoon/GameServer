#pragma once
#pragma comment (lib, "msimg32.lib")
#pragma  comment (lib, "Ws2_32.lib")
#pragma warning(disable : 4996)

#include <WS2tcpip.h>
#include <windows.h>
#include "resource.h"
#include "Define.h"
#include "Struct.h"

#include <vector>
#include <unordered_map>
#include <iostream>
#include <string>
#include <algorithm>
#include <map>

using namespace std;


bool GetWindowActive();