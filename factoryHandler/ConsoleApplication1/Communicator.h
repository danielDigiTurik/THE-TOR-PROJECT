#pragma once

#include "WSAInitializer.h"
#include "IRequestHandler.h"
#include <iostream>
#include <thread>
#include <string>
#include <map>
#include "RequestHandlerFactory.h"

#define PORT 8826

class Communicator
{
public:
    Communicator(RequestHandlerFactory* handlerFactor);
    ~Communicator();
    void startHandleRequests();

private:

    SOCKET m_serverSocket;
    std::map <SOCKET, IRequestHandler*> m_clients;
    void bindAndListen();
    void handleNewClient(SOCKET sock);
    void accept();
    std::string kindOfRequest(char);
    RequestHandlerFactory* m_handlerFactor;
};