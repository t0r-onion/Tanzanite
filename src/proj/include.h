#ifndef TANZANITE_INCLUDE_H
#define TANZANITE_INCLUDE_H

#include <iostream>
#include <thread>
#include <vector>
#include <sstream>
#include <fstream>
#include <cstring>
#include "utils/utilities.h"
#include "client.h"
#include "response.h"

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#else
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#endif
#include "utils/logger.h"

#ifdef _WIN32
#define CLOSESOCKET closesocket
#else
#define CLOSESOCKET close
#endif

#endif