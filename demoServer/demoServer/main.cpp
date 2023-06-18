#include <iostream>
#include <WS2tcpip.h>
#include <string>
#include <cstdlib>
#include <time.h>

#pragma comment (lib, "ws2_32.lib")

using namespace std;
void random() 
{
    int thing = rand() + time(NULL);
    srand(thing);
}

void send_path(std::string msg) {
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
	hint.sin_port = htons(8400);
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

	// Close listening socket
	closesocket(listening);

	// While loop: accept and echo message back to client
	char buf[10000];
	
		ZeroMemory(buf, 10000);

		// Wait for client to send data
		int bytesReceived = recv(clientSocket, buf, 10000, 0);
		if (bytesReceived == SOCKET_ERROR)
		{
			cerr << "Error in recv(). Quitting" << endl;
			//break;
		}

		if (bytesReceived == 0)
		{
			cout << "Client disconnected " << endl;
			//break;
		}

		cout << string(buf, 0, bytesReceived) << endl;
#pragma warning(disable : 4996)
		strcpy(buf, msg.c_str());

		// Echo message back to client
   send(clientSocket, buf, bytesReceived + 1, 0);

	// Close the socket
	closesocket(clientSocket);

	// Cleanup winsock
	WSACleanup();
}

int main()
{
    // This program will create same sequence of
    // random numbers on every program run
    int station1 = 0;
    int station2 = 0;
    int station3 = 0;
    for (int i = 0; i < 3; i++) {
        int a = 0;
        while (a == 0) {
            random();
            a = rand() % 4;
        }
        if (!(a == station1 || a == station2 || a == station3 || a==0)) {
            if (i == 0) { station1 = a; };
            if (i == 1) { station2 = a; };
            if (i == 2) { station3 = a; };
        }
        else { i--; }
    }
    printf("%d", station1);
    printf("%d", station2);
    printf("%d", station3);

    std::string s1 = std::to_string(station1);
    std::string s2 = std::to_string(station2);
    std::string s3 = std::to_string(station3);
    std::string fine = s1+"'th station, and then "+s2+ "'th station, and then "+s3+"'th station.    (will be port number only.)";//will be port number only.
    std::string fin = s1+s2+s3;
    cout << "\n" << fine;

    send_path(fin);

    return 0;
	//system("pause");
}