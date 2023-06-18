#pragma once

#pragma comment(lib, "Ws2_32.lib")
#include <WinSock2.h>
#include <Windows.h>

class WSAInitializer
{
public:
    WSAInitializer();
    ~WSAInitializer();
};