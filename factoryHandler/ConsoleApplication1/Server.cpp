#include "Server.h"
#include "Communicator.h"
#include "SqliteDatabase.h"
#include "RequestHandlerFactory.h"

Server::Server() :
    m_database(new SqliteDatabase), m_requestHandlerFactory(m_database), m_communicator(&m_requestHandlerFactory){}

void Server::run()
{
    std::string exitStr = "";

    std::thread t_connector(&Communicator::startHandleRequests, this->m_communicator);
    t_connector.detach();

    while (exitStr != EXIT_CONDITION)
    {
        std::cin >> exitStr;
    }
    std::cout << "EXIT :(" << std::endl;
}