#pragma once

#include <stdio.h>
#include <WS2tcpip.h>
#include <map>
#include <iostream>
#include <algorithm>
#include <list>
#include <concurrent_unordered_map.h>
#include <chrono>
#include <mutex>
#include <thread>
#include <chrono>
#include <queue>
#include <set>

#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "lua53.lib")

#include "Define.h"
#include "Struct.h"
#include "protocol.h"

extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

using namespace std;
using namespace chrono;
using namespace concurrency;