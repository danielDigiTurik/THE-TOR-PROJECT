#include <iostream>
#include <WS2tcpip.h>
#include <string>
#include <cstring>
#include <fstream>
#include <sstream>
#include <ctime>
#include <thread>
#include <iomanip>
#include "decrypt_structures.h"

#pragma comment (lib, "ws2_32.lib")

std::string open_new_sock(string msg, string path);

/* Structure for building format message that will send to another stations*/
struct Sender
{
	std::string port;
	int stationNum;
	std::string id;
	int size;
	std::string data;
};

/*
	Function builds the message that will send to the stations.
	Input: struct - represent the format of the message template
	Output: string that soon will sent to one of the stations
*/
std::string buildingMessage(struct Sender& s)
{
	std::string messageTemplate = SIGN + s.port + SIGN + to_string(s.stationNum) + SIGN + s.id + SIGN + to_string(s.size) + SIGN + s.data + SIGN;
	return messageTemplate;
}

/*
	The function takes message and parses it according
	to a predefined message format.
	Input: structure for the message that will create, and the message the station got.
	Output: None
*/
void parsingData(struct Sender& message, std::string data, int my_port)
{
	char sign = char((int)data[0]); // from int symbol to char (like 35 = '#')
	std::string delimiter(1, sign);
	data = data.erase(0, 1); // Delete first char in data

	// Setting parameters
	size_t pos = 0;
	int round = 0;
	std::string token;

	while ((pos = data.find(delimiter)) != std::string::npos)
	{
		token = data.substr(0, pos);
		std::cout << round + 1 << ") " << token << std::endl;
		switch (round) // Inserts parameters into the structure
		{
		case 0:
		{
			std::string next_ports = token; //token= x*x*x
			next_ports = next_ports.substr(5, token.size());
			next_ports.insert(14, '*' + to_string(my_port));
			std::string next_port = next_ports.substr(0, 4);

			if (next_port == "1111")
			{
				cout << "\nlast station\n";
			}
			message.port = next_ports;
			std::cout << "\n" << "next ports: " << next_ports << "\n";
			break;
		}
		case 1:
		{
			message.stationNum = atoi(token.c_str()) + 1;
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
		default:
		{
			if (round > 4)
			{
				message.data += '#';
			}
			message.data += token;
			break;
		}
		}
		data.erase(0, pos + delimiter.length());
		round++;
	}
	cout << "result >> " << message.data;
}

void main()
{
	std::string keystring = diffie_hellman_mode();

	// Initialze winsock
	WSADATA wsData;
	WORD ver = MAKEWORD(2, 2);

	int wsOk = WSAStartup(ver, &wsData);
	if (wsOk != 0)
	{
		cerr << "Can't Initialize winsock! Quitting" << endl;
		return;
	}

	// Create a socket
	SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
	if (listening == INVALID_SOCKET)
	{
		cerr << "Can't create a socket! Quitting" << endl;
		return;
	}

	// Bind the ip address and port to a socket
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(5400);
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
		cout << host << " connected on port " << service << endl;
	}
	else
	{
		inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
		cout << host << " connected on port " <<
			ntohs(client.sin_port) << endl;
	}

	
	// While loop: accept and echo message back to client
	char buf[4096];

	bool run = true;
	SOCKET next_socket;
	bool firstTime = true;
	int bytesReceived;
	while (run)
	{
		ZeroMemory(buf, 4096);
		cout << "123";
		// Wait for client to send data
		bytesReceived = recv(clientSocket, buf, 4096, 0);
		cout << "123";
		if (bytesReceived == SOCKET_ERROR)
		{
			cerr << "Error in recv(). Quitting" << endl;
			break;
		}

		if (bytesReceived == 0)
		{
			cout << "Client disconnected " << endl;
			break;
		}

		std::cout << "\nbuf: " << buf << "\n";

		Sender messageTemplate; // Defining the structure
		std::string buffer = buf;
		parsingData(messageTemplate, buffer, 5400);
		if (messageTemplate.id == "2")
		{
			diffie_hellman_mode();
		}

		char* msg = new char[messageTemplate.data.size() + 1];
		std::copy(messageTemplate.data.begin(), messageTemplate.data.end(), msg);
		msg[messageTemplate.data.size()] = '\0';

		int n = strlen((const char*)msg);

		unsigned char* encryptedMessage = new unsigned char[n];
		for (int i = 0; i < n; i++) {
			encryptedMessage[i] = (unsigned char)msg[i];
		}

		// Free memory
		delete[] msg;

		std::istringstream hex_chars_stream(keystring);
		unsigned char key[16];
		int i = 0;
		unsigned int c;
		while (hex_chars_stream >> std::hex >> c)
		{
			key[i] = c;
			i++;
		}
		unsigned char expandedKey[176];
		KeyExpansion(key, expandedKey);

		int messageLen = strlen((const char*)encryptedMessage);

		unsigned char* decryptedMessage = new unsigned char[messageLen];

		for (int i = 0; i < messageLen; i += 16) {
			AESDecrypt(encryptedMessage + i, expandedKey, decryptedMessage + i);
		}

		std::cout << std::endl;
		std::string decrypted_message((char*)decryptedMessage);

		messageTemplate.data = decrypted_message;
		std::string send_msg_decrypted = buildingMessage(messageTemplate);
		std::cout << messageTemplate.port << "\n\n";
		std::cout << "after decrypted function:  " << decrypted_message << "\n\n";
		if (firstTime)
		{
			next_socket = open_new_sock(messageTemplate.port);
			firstTime = false;
		}

		std::string dMessage = get_message_from_next_station(send_msg_decrypted, next_socket);//,new port
		delete[] encryptedMessage;
		// Echo message back to client
		send(clientSocket, dMessage.data(), dMessage.size() + 1, 0);
	}

	// Close the socket
	closesocket(clientSocket);

	// Cleanup winsock
	WSACleanup();

	system("pause");
}