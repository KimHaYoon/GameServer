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

#pragma comment(lib, "Ws2_32.lib")

#include "Define.h"
#include "Struct.h"
#include "protocol.h"

using namespace std;