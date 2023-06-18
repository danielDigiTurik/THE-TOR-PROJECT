#include "Communicator.h"
#include "JsonResponsePacketSerializer.h"
#include "IRequestHandler.h"
#include <vector>
#include <algorithm>

#define HEADER_SIZE 5
int idCounter = 1;
std::map<SOCKET, std::string> connectedClients;


// Constructor for Communicator's class
Communicator::Communicator(RequestHandlerFactory* handlerFactor) : m_handlerFactor(handlerFactor)
{
	m_serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (m_serverSocket == INVALID_SOCKET)
	{
		throw std::exception(__FUNCTION__ " - socket");
	}
}

// Destructor for Communicator's class
Communicator::~Communicator()
{
	closesocket(m_serverSocket); // Closing socket
}

/*
	Function create a new open socket that will wait for client to connect.
	Input: None
	Output: None
*/
void Communicator::bindAndListen()
{
	struct sockaddr_in sa = { 0 };

	sa.sin_port = htons(PORT);
	sa.sin_family = AF_INET;
	sa.sin_addr.s_addr = INADDR_ANY;

	if (bind(m_serverSocket, (struct sockaddr*) & sa, sizeof(sa)) == SOCKET_ERROR)
	{
		throw std::exception(__FUNCTION__ " - bind");
	}
	std::cout << "binded" << std::endl;

	if (listen(m_serverSocket, SOMAXCONN) == SOCKET_ERROR)
	{
		throw std::exception(__FUNCTION__ " - listen");
	}
	std::cout << "listening to port: " << PORT << std::endl;

	while (true)
	{
		std::cout << "accepting client..." << std::endl;
		accept();
	}
}

/*
	Function will accept new client.
	Input: None
	Output: None
*/
void Communicator::accept()
{
	SOCKET client_socket = ::accept(m_serverSocket, NULL, NULL);
	IRequestHandler* login = this->m_handlerFactor->createLoginRequestHandler();

	m_clients.insert(std::pair<SOCKET, IRequestHandler*>(client_socket, login));
	if (client_socket == INVALID_SOCKET)
		throw std::exception(__FUNCTION__);

	std::cout << "Client accepted !" << std::endl;

	// Create the client thread
	std::thread clientThread(&Communicator::handleNewClient, this, client_socket);
	clientThread.detach();
}

/*
	Function will accept new client.
	Input: Socket that represent the new client.
	Output: None

	**Here we got help from Lesson 13, David and from the Interet**
*/
void Communicator::handleNewClient(SOCKET sock)
{
	bool isConnected = true;

	while (isConnected)
	{
		char header[HEADER_SIZE] = { 0 };
		char* data = nullptr;
		int size = 0;
		RequestInfo r;
		std::string sizeStr = "";
		recv(sock, header, HEADER_SIZE, 0);
		for (int i = 1; i < HEADER_SIZE; i++)
		{
			sizeStr += header[i];
		}
		size = atoi(sizeStr.c_str());
		if (size)
		{
			data = new char[size];
			recv(sock, data, size, 0);
			std::string buffer;
			r.id = header[0];
			std::cout << "REQUEST ID: " << header[0] << ", " << header << "\nsize=" << size << std::endl;
			for (int i = 0; i < size; i++)
			{
				buffer.push_back(data[i]);
			}
			auto search = connectedClients.find(sock);
			if (search == connectedClients.end())
			{
				connectedClients.insert(std::pair<SOCKET, std::string>(sock, buffer.substr(0, buffer.find(','))));
			}

			std::cout << "buf = " << data << std::endl;
			r.buffer = buffer;
			time_t now = time(0);
			r.recievalTime = now;
			try
			{
				RequestResult result = m_clients.at(sock)->handleRequest(r);//this sock
				std::string resultStr = result.response;
				auto it = m_clients.begin();
				SOCKET s = it->first;
				std::cout << "socket: " << sock << std::endl;

				std::string returnMessage;
				for (auto& it : connectedClients)
				{
					returnMessage += std::to_string(it.first) + ":" + it.second + ",";
				}
				if (r.id == '3')//logout
					// remove here and in the bottom too: socket from (std::map)connectedClients
					isConnected = false;
				if (send(sock, returnMessage.c_str(), returnMessage.size(), 0) == INVALID_SOCKET)
				{
					std::cout << "ERROR INVALID SOCKET" << std::endl;
					isConnected = false;
				}
				else
				{
					m_clients[sock] = result.newHandler;
				}
			}
			catch (std::string e)
			{
				std::cout << "Message: " << e << std::endl;
				ErrorResponse error;
				error._message = e;

				std::string errorVec = JsonResponsePacketSerializer::serializeResponse(error);
				std::string errorStr(errorVec.begin(), errorVec.end());
				send(sock, errorStr.c_str(), errorStr.size(), 0);
			}
			catch (std::exception e)
			{
				std::cout << "Error: " << e.what() << std::endl;
				isConnected = false;
			}
		}
	}
}


/*
	Function will call the function "bindAndListen()".
	Input: None
	Output: None
*/
void Communicator::startHandleRequests()
{	
	this->bindAndListen();
}
