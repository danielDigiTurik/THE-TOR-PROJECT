#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include "JsonResponsePacketSerializer.h"

typedef struct LoginRequest
{
	std::string username;
	std::string password;
}LoginRequest;

typedef struct SignupRequest
{
	std::string username;
	std::string password;
	std::string email;
}SignupRequest;

typedef struct CreateRoomRequest
{
	std::string roomName;
	unsigned int maxUsers;
	unsigned int questionCount;
	unsigned int answerTimeout;
}CreateRoomRequest;

class JsonRequestPacketDeserializer
{
public:
	static LoginRequest deserializeLoginRequest(std::string);
	static SignupRequest deserializeSignUpRequest(std::string);
	static CreateRoomRequest deserializeCreateRoomRequest(std::string);
};

