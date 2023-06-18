#include "JsonRequestPacketDeserializer.h"

/*
	Function will deserialize the messages that came into this server.
	Input: buffer
	Output: A full LoginRequest
*/
LoginRequest JsonRequestPacketDeserializer::deserializeLoginRequest(std::string buffer)
{
	// Setting parameters
	LoginRequest login;
	std::string delimiter = ",";
	size_t pos = 0;
	int round = 0;
	std::string token;
	std::string data;
	for (int i = 0; i < buffer.size(); i++)
	{
		data += buffer[i];
	}
	data += ",";

	while ((pos = data.find(delimiter)) != std::string::npos)
	{
		token = data.substr(0, pos);
		switch (round) // Inserts parameters into the structure
		{
		case 0:
		{
			login.username = token;
			break;
		}
		case 1:
		{
			login.password = token;
			break;
		}
		}
		data.erase(0, pos + delimiter.length());
		round++;
	}
	return login;
}

/*
	Function will deserialize the messages that came into this server.
	Input: buffer
	Output: A full SignupRequest
*/
SignupRequest JsonRequestPacketDeserializer::deserializeSignUpRequest(std::string buffer)
{
	// Setting parameters
	SignupRequest signUp;
	std::string delimiter = ",";
	size_t pos = 0;
	int round = 0;
	std::string token;
	std::string data;
	for (int i = 0; i < buffer.size(); i++)
	{
		data += buffer[i];
	}
	data += ",";

	while ((pos = data.find(delimiter)) != std::string::npos)
	{
		token = data.substr(0, pos);
		switch (round) // Inserts parameters into the structure
		{
		case 0:
		{
			signUp.username = token;
			break;
		}
		case 1:
		{
			signUp.password = token;
			break;
		}
		case 2:
		{
			signUp.email = token;
			break;
		}
		}
		data.erase(0, pos + delimiter.length());
		round++;
	}
	return signUp;
}

/*
	Function will deserialize the messages that came into this server.
	Input: buffer
	Output: A full CreateRoomRequest.
*/
CreateRoomRequest JsonRequestPacketDeserializer::deserializeCreateRoomRequest(std::string buffer)
{
	CreateRoomRequest roomRequest;
	std::string bufferStr;

	for (int i = 0; i < buffer.size(); i++)
	{
		bufferStr += buffer[i];
	}
	std::cout << "buffer" << bufferStr << std::endl;

	auto jsonObj = json::parse(bufferStr);
	std::string data = jsonObj.dump();
	json::iterator it = jsonObj.begin();

	roomRequest.answerTimeout = it.value();
	it++;
	roomRequest.maxUsers = it.value();
	it++;
	roomRequest.questionCount = it.value();
	it++;
	roomRequest.roomName = it.value();

	return roomRequest;
}