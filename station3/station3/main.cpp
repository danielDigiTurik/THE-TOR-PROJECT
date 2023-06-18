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
using namespace std;
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

struct key_exchange
{
	std::string port;
	int prime;
	int primitive_root;
	int dh_key;
	int stationNum;
	int size;
	std::string data;
};

void random()
{
	int thing = rand() + time(NULL);
	srand(thing);
}

uint64_t compute_dh_key2(uint64_t prime, uint64_t primitive_root, uint64_t secret) {
	uint64_t result = 1;
	primitive_root %= prime;
	while (secret > 0) {
		if (secret & 1) {
			result = (result * primitive_root) % prime;
		}
		secret = secret >> 1;
		primitive_root = (primitive_root * primitive_root) % prime;
	}
	return result;
}

// Function to generate a random prime number
int generate_prime() {
	random();
	// Randomly generate a number between 2 and 100
	int prime = rand() % 99 + 2;

	// Check if the number is prime
	for (int i = 2; i < prime; i++) {
		if (prime % i == 0) {
			// If the number is not prime, try again
			return generate_prime();
		}
	}
	// Return the prime number
	return prime;
}

// Function to generate a random primitive root of a given prime number
int generate_primitive_root(int prime) {
	// Randomly generate a number between 2 and the prime - 1
	random();
	int root = rand() % (prime - 2) + 2;
	int p;
	// pow(3, 4);
	 // Check if the number is a primitive root
	for (int i = 2; i < prime; i++) {
		p = pow(root, i);
		if (p % prime == 1) {
			// If the number is not a primitive root, try again
			return generate_primitive_root(prime);
		}
	}
	// Return the primitive root
	return root;
}

// Function to compute the Diffie-Hellman key
int compute_dh_key(int prime, int primitive_root, int secret) {
	// Compute the Diffie-Hellman key using the formula: key = (primitive_root ^ secret) % prime
	int to_key = pow(primitive_root, secret);
	int key = to_key % prime;
	return key;
}

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

void parsingData2d(struct key_exchange& message, std::string data, int my_port)
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
			message.prime = atoi(token.c_str());
			break;
		}
		case 2:
		{
			message.primitive_root = atoi(token.c_str());
			break;
		}
		case 3:
		{
			message.dh_key = atoi(token.c_str());
			break;
		}
		case 4:
		{
			message.stationNum = atoi(token.c_str()) + 1;
			break;
		}
		case 5:
		{
			message.size = atoi(token.c_str());
			break;
		}
		default:
		{
			if (round > 5)
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
}

std::string get_message_from_next_station(string msg, SOCKET sock)
{
	// Do-while loop to send and receive data
	char buf[4096];
	string userInput = msg;

	// Prompt the user for some text
	cout << "> ";

	if (userInput.size() > 0)		// Make sure the user has typed in something
	{
		// Send the text
		int sendResult = send(sock, userInput.c_str(), userInput.size() + 1, 0);

		if (sendResult != SOCKET_ERROR)
		{
			// Wait for response
			ZeroMemory(buf, 4096);
			int bytesReceived = recv(sock, buf, 4096, 0);
			if (bytesReceived > 0)
			{
				// Echo response to console
				cout << "FIRST STATION> " << string(buf, 0, bytesReceived) << endl;
				return string(buf, 0, bytesReceived);
			}
		}
	}
	// Gracefully close down everything
	//closesocket(sock); // gilat
	WSACleanup();
	return "ERROR";
}

SOCKET open_new_sock(string path)
{
	std::string port_build;
	string ipAddress = "127.0.0.1"; // IP Address of the server
	port_build = path.substr(0, 4);
	int port = stoi(port_build); // Listening port # on the server
	std::cout << "\n" << "message send to: " << port_build;
	std::cout << "\n" << port;

	// Initialize WinSock
	WSAData data;
	WORD ver = MAKEWORD(2, 2);
	int wsResult = WSAStartup(ver, &data);
	if (wsResult != 0)
	{
		cerr << "Can't start Winsock, Err #" << wsResult << endl;
	}

	// Create socket
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET)
	{
		cerr << "Can't create socket, Err #" << WSAGetLastError() << endl;
		WSACleanup();
	}

	// Fill in a hint structure
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(port);
	inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);

	// Connect to server
	int connResult = connect(sock, (sockaddr*)&hint, sizeof(hint));
	if (connResult == SOCKET_ERROR)
	{
		cerr << "Can't connect to server, Err #" << WSAGetLastError() << endl;
		closesocket(sock);
		WSACleanup();
	}
	return sock;

}

std::string buildingMessage2d(struct key_exchange& s)
{
	std::string messageTemplate = SIGN + s.port + SIGN + to_string(s.prime) + SIGN + to_string(s.primitive_root) + SIGN + to_string(s.dh_key) + SIGN + to_string(s.stationNum) + SIGN + to_string(s.size) + SIGN;
	return messageTemplate;
}
int messageSize2d(struct key_exchange& s, std::string msg)
{
	int size = 0;
	size = s.port.length() + to_string(s.stationNum).length() + to_string(s.prime).length() + to_string(s.size).length() + to_string(s.dh_key).length() + to_string(s.primitive_root).length() + msg.length() + 5;
	return size;
}

std::string buildKey(int x)
{
	char hex_str[100];
	std::string hex_key;
	std::stringstream stream;
	stream << std::setfill('0') << std::hex;
	for (int i = 0; i < 16; i++) {
		stream.str("");
		int value = x - i;
		if (value < 0) { value = x + i; }
		stream << std::setw(2) << value;
		hex_key += stream.str() + " ";
	}
	std::cout << "\nHex key: " << hex_key << std::endl;
	return hex_key;
}

std::string diffie_hellman_mode()
{
	// Initialze winsock
	WSADATA wsData;
	WORD ver = MAKEWORD(2, 2);

	int wsOk = WSAStartup(ver, &wsData);
	if (wsOk != 0)
	{
		cerr << "Can't Initialize winsock! Quitting" << endl;
		return "";
	}

	// Create a socket
	SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
	if (listening == INVALID_SOCKET)
	{
		cerr << "Can't create a socket! Quitting" << endl;
		return "";
	}

	// Bind the ip address and port to a socket
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(3400);
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
	ZeroMemory(buf, 4096);

	// Wait for client to send data
	int bytesReceived = recv(clientSocket, buf, 4096, 0);
	if (bytesReceived == SOCKET_ERROR)
	{
		cerr << "Error in recv(). Quitting -my error" << endl;
	}
	if (bytesReceived == 0)
	{
		cout << "Client disconnected " << endl;
	}
	key_exchange messageTemplate; // Defining the structure
	std::string buffer = buf;
	parsingData2d(messageTemplate, buffer, 3400);

	random();
	int secret = rand();
	cout << "\nsecret:" << secret;

	// Compute the Diffie-Hellman key
	int my_key = static_cast<int>(compute_dh_key2(static_cast<uint64_t>(messageTemplate.prime), static_cast<uint64_t>(messageTemplate.primitive_root), static_cast<uint64_t>(secret)));
	stringstream strs;
	strs << my_key;
	string temp_str = strs.str();
	char* char_type = (char*)temp_str.c_str();
	send(clientSocket, char_type, sizeof(char_type), 0);
	int shared_key = static_cast<int>(compute_dh_key2(static_cast<uint64_t>(messageTemplate.prime), static_cast<uint64_t>(messageTemplate.dh_key), static_cast<uint64_t>(secret)));

	// Print the final shared key
	cout << "\nShared key: " << shared_key << endl;
	std::string my_key_ = buildKey(shared_key);
	cout << "\n station num: " << messageTemplate.stationNum << "\n";

	if (messageTemplate.stationNum == 2)
	{
		string ipAddress = "127.0.0.1"; // IP Address of the server
		int port = 8080; // Listening port # on the server
		std::cout << "\n" << port;

		// Initialize WinSock
		WSAData data;
		WORD ver = MAKEWORD(2, 2);
		int wsResult = WSAStartup(ver, &data);
		if (wsResult != 0)
		{
			cerr << "Can't start talking to proxy" << wsResult << endl;
			return "";
		}

		// Create socket
		SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
		if (sock == INVALID_SOCKET)
		{
			cerr << "Can't create socket, Err #" << WSAGetLastError() << endl;
			WSACleanup();
			return "";
		}
		// Fill in a hint structure
		sockaddr_in hint;
		hint.sin_family = AF_INET;
		hint.sin_port = htons(port);
		inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);

		// Connect to server
		int connResult = connect(sock, (sockaddr*)&hint, sizeof(hint));
		if (connResult == SOCKET_ERROR)
		{
			cerr << "Can't connect to server, Err #" << WSAGetLastError() << " \n--------diffie hellman error - PLEASE MAKE SURE ALL STATIONS ARE RUNNING--------" << endl;
			closesocket(sock);
			WSACleanup();
			return "";
		}
		key_exchange messageTemplate2; // Defining the structure

		// Inserts parameters into the structure
		messageTemplate2.port = "5400*8000*2022*1111";
		messageTemplate2.prime = messageTemplate.prime;
		messageTemplate2.primitive_root = messageTemplate.primitive_root;
		messageTemplate2.dh_key = messageTemplate.dh_key;
		messageTemplate2.stationNum = 2;

		std::string msg = buildingMessage2d(messageTemplate2);
		const char* msg_structure = msg.c_str(); // Changing the variable type (for using 'send' function)
		int msg_size = messageSize2d(messageTemplate2, msg);
		int sendResult = send(sock, msg_structure, msg_size, 0); // Send the text
		char client_dh_key_str2[100];//dh_key2
		char client_dh_key_str3[100];//dh_key3
		if (sendResult != SOCKET_ERROR)
		{
			ZeroMemory(client_dh_key_str2, 100);
			ZeroMemory(client_dh_key_str3, 100);
			int bytesReceived = recv(sock, client_dh_key_str2, sizeof(client_dh_key_str2), 0);

			if (bytesReceived > 0)
			{
				const char* myConstCharPointer = client_dh_key_str2;
				send(clientSocket, myConstCharPointer, sizeof(myConstCharPointer), 0);//send back to client
			}
			
			bytesReceived = recv(sock, client_dh_key_str3, sizeof(client_dh_key_str3), 0);
			if (bytesReceived > 0)
			{
				const char* myConstCharPointer = client_dh_key_str3;
				send(clientSocket, myConstCharPointer, sizeof(myConstCharPointer), 0);//send back to client
			}
		}
	}
	if (messageTemplate.stationNum == 3)
	{
		string ipAddress = "127.0.0.1"; // IP Address of the server
		int port = 3400; // Listening port # on the server
		std::cout << "\n" << port;

		// Initialize WinSock
		WSAData data;
		WORD ver = MAKEWORD(2, 2);
		int wsResult = WSAStartup(ver, &data);
		if (wsResult != 0)
		{
			cerr << "Can't start talking to proxy" << wsResult << endl;
			return "";
		}

		// Create socket
		SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
		if (sock == INVALID_SOCKET)
		{
			cerr << "Can't create socket, Err #" << WSAGetLastError() << endl;
			WSACleanup();
			return "";
		}
		// Fill in a hint structure
		sockaddr_in hint;
		hint.sin_family = AF_INET;
		hint.sin_port = htons(port);
		inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);

		// Connect to server
		int connResult = connect(sock, (sockaddr*)&hint, sizeof(hint));
		if (connResult == SOCKET_ERROR)
		{
			cerr << "Can't connect to server, Err #" << WSAGetLastError() << " \n--------diffie hellman error - PLEASE MAKE SURE ALL STATIONS ARE RUNNING--------" << endl;
			closesocket(sock);
			WSACleanup();
			return "";
		}
		key_exchange messageTemplate3; // Defining the structure

		// Inserts parameters into the structure
		messageTemplate3.port = "5400*8000*2022*1111";
		messageTemplate3.prime = messageTemplate.prime;
		messageTemplate3.primitive_root = messageTemplate.primitive_root;
		messageTemplate3.dh_key = messageTemplate.dh_key;
		messageTemplate3.stationNum = 3;

		std::string msg = buildingMessage2d(messageTemplate3);
		const char* msg_structure = msg.c_str(); // Changing the variable type (for using 'send' function)
		int msg_size = messageSize2d(messageTemplate3, msg);
		int sendResult = send(sock, msg_structure, msg_size, 0); // Send the text
		char client_dh_key_str3[100];
		if (sendResult != SOCKET_ERROR)
		{
			ZeroMemory(client_dh_key_str3, 100);
			int bytesReceived = recv(sock, client_dh_key_str3, sizeof(client_dh_key_str3), 0);

			if (bytesReceived > 0)
			{
				const char* myConstCharPointer = client_dh_key_str3;
				send(clientSocket, myConstCharPointer, sizeof(myConstCharPointer), 0);//send back to client
			}
		}
	}
	closesocket(listening);
	return my_key_;
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
	//ports: 1- 5000 2- 8080 3- 3400
	
	// Bind the ip address and port to a socket
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(2022);
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
		cout << "1";

		// Wait for client to send data
		int bytesReceived = recv(clientSocket, buf, 4096, 0);
		cout << "1";

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
		parsingData(messageTemplate, buffer, 2022);

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

		// Read in the key
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
		firstTime = false;
		// Echo message back to client
		send(clientSocket, dMessage.data(), dMessage.size() + 1, 0);

	}

	// Close the socket
	closesocket(clientSocket);

	// Cleanup winsock
	WSACleanup();

	system("pause");
}