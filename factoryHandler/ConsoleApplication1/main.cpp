#include "Communicator.h"
#include "IRequestHandler.h"
#include "LoginRequestHandler.h"
#include "Server.h"
#include "WSAInitializer.h"
#include "JsonResponsePacketSerializer.h"
#include <iterator>

int main()
{
	WSAInitializer WSAObj;
	Server trivia_server = Server();
	trivia_server.run();
	return 0;
}