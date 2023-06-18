#include <iostream>
#include <WS2tcpip.h>
#include <string>

#pragma comment (lib, "ws2_32.lib")

#define SIGN '#'

struct Sender
{
	std::string port;
	int stationNum;
	std::string id;
	int size;
	std::string data;
};

/*
	The function takes message and parses it according
	to a predefined message format.
	Input: structure for the message that will create, and the message the station got.
	Output: None
*/
void parsingData(struct Sender& message, std::string data)
{
	char sign = char((int)data[0]); // from int symbol to char (like 35 = '#')
	std::string delimiter(1, sign);
	data = data.erase(0, 1); // Delte first char in data

	// Setting parameters
	size_t pos = 0;
	int round = 0;
	std::string token;
	bool isLastStation = false;

	while ((pos = data.find(delimiter)) != std::string::npos)
	{
		token = data.substr(0, pos);
		std::cout << round + 1 << ") " << token << std::endl;
		switch (round) // Inserts parameters into the structure
		{
		case 0:
		{
			message.port = token;
			break;
		}
		case 1:
		{
			message.stationNum = atoi(token.c_str());
			break;
		}
		case 2:
		{
			message.id = token;
			break;
		}
		case 3:
		{
			message.size = atoi(token.c_str());
			break;
		}
		case 4:
		{
			message.data = token;
			break;
		}
		}
		data.erase(0, pos + delimiter.length());
		round++;
	}
}

std::string buildingMessage(struct Sender& s)
{
	std::string messageTemplate = SIGN + s.port + SIGN + std::to_string(s.stationNum) + SIGN + s.id + SIGN + std::to_string(s.size) + SIGN + s.data + SIGN;
	std::cout << messageTemplate << std::endl;
	return messageTemplate;
}


void main()
{
	// Initialze winsock
	WSADATA wsData;
	WORD ver = MAKEWORD(2, 2);


	int wsOk = WSAStartup(ver, &wsData);
	if (wsOk != 0)
	{
		std::cerr << "Can't Initialize winsock! Quitting" << std::endl;
		return;
	}

	// Create a socket
	SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
	if (listening == INVALID_SOCKET)
	{
		std::cerr << "Can't create a socket! Quitting" << std::endl;
		return;
	}

	// Bind the ip address and port to a socket
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(1111);
	hint.sin_addr.S_un.S_addr = INADDR_ANY; // Could also use inet_pton .... 

	bind(listening, (sockaddr*)&hint, sizeof(hint));

	// Tell Winsock the socket is for listening 
	listen(listening, SOMAXCONN);

	// Wait for a connection
	sockaddr_in client;
	int clientSize = sizeof(client);

	SOCKET clientSocket = accept(listening, (sockaddr*)&client, &clientSize);

	char host[NI_MAXHOST];		// Client's remote name
	char service[NI_MAXSERV];	// Service (i.e. port) the client is connect on

	ZeroMemory(host, NI_MAXHOST); // same as memset(host, 0, NI_MAXHOST);
	ZeroMemory(service, NI_MAXSERV);

	if (getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0)
	{
		std::cout << host << " connected on port " << service << std::endl;
	}
	else
	{
		inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
		std::cout << host << " connected on port " << ntohs(client.sin_port) << std::endl;
	}

	// Close listening socket
	//closesocket(listening);

	// While loop: accept and echo message back to client
	char buf[4096];

	while (true)
	{
		ZeroMemory(buf, 4096);

		// Wait for client to send data
		int bytesReceived = recv(clientSocket, buf, 4096, 0);
		if (bytesReceived == SOCKET_ERROR)
		{
			std::cerr << "Error in recv(). Quitting" << std::endl;
			break;
		}

		if (bytesReceived == 0)
		{
			std::cout << "Client disconnected " << std::endl;
			break;
		}

		std::cout << "\nbuf: " << buf << "\n" << std::endl;
		Sender messageTemplate;
		std::string buffer = buf;

		parsingData(messageTemplate, buffer);
		std::string message = buildingMessage(messageTemplate);

		send(clientSocket, message.c_str(), message.size(), 0);
		//break;
	}

	// Close the socket
	closesocket(clientSocket);

	// Cleanup winsock
	WSACleanup();

	system("pause");
}
