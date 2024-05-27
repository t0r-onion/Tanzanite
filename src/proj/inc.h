#pragma once

#ifndef INC_H
#define INC_H

#include <iostream>
#include <thread>
#include <vector>
#include <sstream>
#include <fstream>
#include <cstring>

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

const int PORT = 8080;
const int BUFFER_SIZE = 1024;

#ifdef _WIN32
#define CLOSESOCKET closesocket
#else
#define CLOSESOCKET close
#endif

#ifdef _WIN32
#include <windows.h>
#include <psapi.h>

size_t get_memory_usage ( )
{
    PROCESS_MEMORY_COUNTERS_EX pmc;
    GetProcessMemoryInfo ( GetCurrentProcess ( ) , ( PROCESS_MEMORY_COUNTERS* ) &pmc , sizeof ( pmc ) );
    return pmc.WorkingSetSize;
}
#else
#include <unistd.h>
#include <sys/resource.h>

size_t get_memory_usage ( )
    {
    struct rusage usage;
    getrusage ( RUSAGE_SELF , &usage );
    return usage.ru_maxrss * 1024L; // ru_maxrss is in kilobytes, convert to bytes
}
#endif

#include "response/response.h"
#include "utils/utilities.h"

#endif