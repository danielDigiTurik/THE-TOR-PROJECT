#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include <sstream>
#include <vector>
#include <WS2tcpip.h>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include "sqlite3.h"
#include <io.h>
#include "encrypt_structures.h"

#pragma comment(lib, "ws2_32.lib")
using namespace std;

void addMessageToDB(std::string message)
{
	sqlite3* db;
	std::string dbFileName = "messagesDB.db";
	int doesFileExist = _access(dbFileName.c_str(), 0);
	int opened = sqlite3_open(dbFileName.c_str(), &db);

	if (SQLITE_OK != opened)
	{
		db = nullptr;
		std::cout << "Failed to open DB" << std::endl;
	}
	std::string sqlStatement = "INSERT INTO MESSAGES (message) VALUES ('" + message + "');";
	char* errMsg = nullptr;
	int res = sqlite3_exec(db, sqlStatement.c_str(), nullptr, nullptr, &errMsg); // Sends to db
	if (SQLITE_OK != res)
	{
		std::cout << "ERROR" << std::endl;
	}
}

std::string open_new_sock(std::string msg, std::string path, SOCKET sock)
{
	/*std::string port_build;
	std::string ipAddress = "127.0.0.1"; // IP Address of the server
	port_build = path.substr(0, 4);
	int port = stoi(port_build); // Listening port # on the server
	std::cout << "\n" << "message send to: " << port_build << std::endl;
	std::cout << "\n" << port << std::endl;

	// Initialize WinSock
	WSAData data;
	WORD ver = MAKEWORD(2, 2);
	int wsResult = WSAStartup(ver, &data);
	if (wsResult != 0)
	{
		std::cerr << "Can't start Winsock, Err #" << wsResult << std::endl;
		return "ERROR";
	}

	// Create socket
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET)
	{
		std::cerr << "Can't create socket, Err #" << WSAGetLastError() << std::endl;
		WSACleanup();
		return "ERROR";
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
		std::cerr << "Can't connect to server, Err #" << WSAGetLastError() << std::endl;
		closesocket(sock);
		WSACleanup();
		return "ERROR";
	}*/

	// Do-while loop to send and receive data
	char buf[4096];
	std::string userInput = msg;

	// Prompt the user for some text
	std::cout << "> " << std::endl;

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
				std::cout << "FIRST STATION> " << std::string(buf, 0, bytesReceived) << std::endl;
				return std::string(buf, 0, bytesReceived);
			}
		}
	}
	// Gracefully close down everything
	//closesocket(sock); gilat
	WSACleanup();
	return "ERROR";
}

void main()
{
	std::string ipAddress = "127.0.0.1"; // IP Address of the server
	int port = 8826; // Listening port # on the server
	std::cout << "\nport=" << port;

	// Initialize WinSock
	WSAData data;
	WORD ver = MAKEWORD(2, 2);
	int wsResult = WSAStartup(ver, &data);
	if (wsResult != 0)
	{
		std::cerr << "Can't start Winsock, Err #" << wsResult << std::endl;
		return;
	}

	// Create socke
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET)
	{
		std::cerr << "Can't create socket, Err #" << WSAGetLastError() << std::endl;
		WSACleanup();
		return;
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
		std::cerr << "Can't connect to server, Err #" << WSAGetLastError() << std::endl;
		closesocket(sock);
		WSACleanup();
		return;
	}

	// Define new parameter
	char id;
	std::string userInput;
	std::string header;
	std::string username;

	// LOGIN USER || SIGN UP USER
	do
	{
		std::cout << "Type a number between 1-2: " << std::endl; // Type a number and press enter
		std::cin >> id; // Get user input from the keyboard
	} while ((int)id - 48 < 1 || (int)id - 48 > 2);
	getchar();

	// Prompt the user for some text
	std::cout << "> " << std::endl;
	getline(std::cin, userInput);

	char* char_userInput = new char[userInput.size() + 1];
	std::copy(userInput.begin(), userInput.end(), char_userInput);
	char_userInput[userInput.size()] = '\0';
	
	username = userInput.substr(0, userInput.find(','));
	// 20009amit,dvir
	std::string zeroes = "00"; // header[5] = id + _ + msgSize (how much zeroes should be for making the header arr with constant size (=5))
	if (username.size() < 9)
	{
		zeroes += "0";
	}
	header = header.assign(1, id) + zeroes + std::to_string(userInput.size()) + char_userInput;
	std::cout << "\nheader = " << header << std::endl;
	const char* sendHeader = header.c_str();
	send(sock, sendHeader, header.size() + 1, 0);

	string keys = diffie_hellman_mode("5400*8000*2022*1111");
	std::stringstream ss(keys);
	int key1, key2, key3;
	ss >> key1 >> key2 >> key3;
	std::cout << key1 << " " << key2 << " " << key3 << std::endl;

	std::string key_for_station1 = buildKey(key1);
	std::string key_for_station2 = buildKey(key2);
	std::string key_for_station3 = buildKey(key3);

	delete[] char_userInput;
	///////////////////////////////////////////////////////////////////
	int originalLen;
	std::cout << "----------CLIENT----------\n" << std::endl;
	std::string path = get_path();
	std::cout << "\n" << "your path is " << path << "\n" << std::endl;
	string rand_result = path;

	path = buildPATH(path);

	std::string s_port_build;
	std::string s_ipAddress = "127.0.0.1"; // IP Address of the server
	s_port_build = path.substr(0, 4);
	int s_port = stoi(s_port_build); // Listening port # on the server
	std::cout << "\n" << "message send to: " << s_port_build << std::endl;
	std::cout << "\n" << s_port << std::endl;

	// Initialize WinSock
	WSAData s_data;
	WORD s_ver = MAKEWORD(2, 2);
	int s_wsResult = WSAStartup(s_ver, &s_data);
	if (s_wsResult != 0)
	{
		std::cerr << "Can't start Winsock, Err #" << s_wsResult << std::endl;
	}

	// Create socket
	SOCKET second_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (second_sock == INVALID_SOCKET)
	{
		std::cerr << "Can't create socket, Err #" << WSAGetLastError() << std::endl;
		WSACleanup();
	}

	// Fill in a hint structure
	sockaddr_in s_hint;
	s_hint.sin_family = AF_INET;
	s_hint.sin_port = htons(s_port);
	inet_pton(AF_INET, s_ipAddress.c_str(), &s_hint.sin_addr);

	// Connect to server
	int s_connResult = connect(second_sock, (sockaddr*)&s_hint, sizeof(s_hint));
	if (s_connResult == SOCKET_ERROR)
	{
		std::cerr << "Can't connect to server, Err #" << WSAGetLastError() << std::endl;
		closesocket(second_sock);
		WSACleanup();
	}

	while (id != 3)
	{
		do
		{
			std::cout << "Type a number between 3-9: " << std::endl; // Type a number and press enter
			std::cin >> id; // Get user input from the keyboard
		} while ((int)id - 48 < 3 || (int)id - 48 > 9);
		getchar();

		std::cout << ">" << std::endl;
		getline(std::cin, userInput);

		char_userInput = new char[userInput.size() + 1];
		originalLen = strlen((const char*)char_userInput);
		std::copy(userInput.begin(), userInput.end(), char_userInput);
		char_userInput[userInput.size()] = '\0';

		unsigned char* encryptedMessage = (unsigned char*)char_userInput;

		std::istringstream hex_chars_stream1(key_for_station1);
		unsigned char key1[16];
		int i = 0;
		unsigned int c1;
		while (hex_chars_stream1 >> std::hex >> c1)
		{
			key1[i] = c1;
			i++;
		}
		std::istringstream hex_chars_stream2(key_for_station2);
		unsigned char key2[16];
		i = 0;
		unsigned int c2;
		while (hex_chars_stream2 >> std::hex >> c2)
		{
			key2[i] = c2;
			i++;
		}

		std::istringstream hex_chars_stream3(key_for_station3);
		unsigned char key3[16];
		i = 0;
		unsigned int c3;
		while (hex_chars_stream3 >> std::hex >> c3)
		{
			key3[i] = c3;
			i++;
		}
		std::cout << "\npath is:" << rand_result << "\n";
		if (rand_result == "123")
		{
			encryptedMessage = padToSixteenBytes((char*)encryptedMessage, key3);
			encryptedMessage = padToSixteenBytes((char*)encryptedMessage, key2);
			encryptedMessage = padToSixteenBytes((char*)encryptedMessage, key1);
			cout << "\n\n" << "encrypted" << "\n\n";

		}
		else if (rand_result == "132")
		{
			encryptedMessage = padToSixteenBytes((char*)encryptedMessage, key2);
			encryptedMessage = padToSixteenBytes((char*)encryptedMessage, key3);
			encryptedMessage = padToSixteenBytes((char*)encryptedMessage, key1);
			cout << "\n\n" << "encrypted" << "\n\n";

		}
		else if (rand_result == "213")
		{
			encryptedMessage = padToSixteenBytes((char*)encryptedMessage, key3);
			encryptedMessage = padToSixteenBytes((char*)encryptedMessage, key1);
			encryptedMessage = padToSixteenBytes((char*)encryptedMessage, key2);
			cout << "\n\n" << "encrypted" << "\n\n";

		}
		else if (rand_result == "231")
		{
			encryptedMessage = padToSixteenBytes((char*)encryptedMessage, key1);
			encryptedMessage = padToSixteenBytes((char*)encryptedMessage, key3);
			encryptedMessage = padToSixteenBytes((char*)encryptedMessage, key2);
			cout << "\n\n" << "encrypted" << "\n\n";

		}
		else if (rand_result == "321")
		{
			encryptedMessage = padToSixteenBytes((char*)encryptedMessage, key1);
			encryptedMessage = padToSixteenBytes((char*)encryptedMessage, key2);
			encryptedMessage = padToSixteenBytes((char*)encryptedMessage, key3);
			cout << "\n\n" << "encrypted" << "\n\n";

		}
		else if (rand_result == "312")
		{
			encryptedMessage = padToSixteenBytes((char*)encryptedMessage, key2);
			encryptedMessage = padToSixteenBytes((char*)encryptedMessage, key1);
			encryptedMessage = padToSixteenBytes((char*)encryptedMessage, key3);
			cout << "\n\n" << "encrypted" << "\n\n";
		}

		std::cout << std::endl; // New line
		std::string encrypted_msg = (const char*)encryptedMessage; // Changing the kind of the encryptedMessage's parameter

		if (sizeof(encryptedMessage) > 0) // Make sure the user has typed in something
		{
			Sender messageTemplate; // Defining the structure

			// Inserts parameters into the structure
			messageTemplate.port = path;
			messageTemplate.data = encrypted_msg;
			messageTemplate.id = "1"; // KEY or DECRYPTION --> after diffie-hellman it will change
			messageTemplate.stationNum = 1;
			messageTemplate.size = userInput.size();

			std::string msg = buildingMessage(messageTemplate);
			const char* msg_structure = msg.c_str(); // Changing the variable type (for using 'send' function)
			int msg_size = messageSize(messageTemplate, msg);
			std::cout << "path = " << path << "\nmsg = " << msg << std::endl;
			std::string result = open_new_sock(msg, path, second_sock);

			addMessageToDB(userInput);
			header = header.assign(1, id) + zeroes + std::to_string(username.size()) + (username).c_str();
			std::cout << "\nheader = " << header << std::endl;
			const char* sendHeader = header.c_str();
			send(sock, sendHeader, header.size() + 1, 0);
		}
		delete[] encryptedMessage;
		getchar();
	}
	closesocket(sock);

	// Gracefully close down everything
	WSACleanup();
}