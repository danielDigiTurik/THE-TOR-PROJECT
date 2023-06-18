#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include "json.hpp"
//#include "Room.h"
using json = nlohmann::json;

#define FOUR 4
#define ERROR_STATUS '0'
#define LOGIN_STATUS '1'
#define SIGNUP_STATUS '2'
#define LOGOUT_STATUS '3'
#define MESSAGE_STATUS '4'
#define CHANGEMSG_STATUS '5'
#define DELETEMSG_STATUS '6'
#define DELETECHAT_STATUS '7'
#define GETREQUEST_STATUS '8'
#define CREATECHAT_STATUS '9'

typedef struct ErrorResponse
{
	std::string _message;
}ErrorResponse;

typedef struct LoginResponse
{
	unsigned int status;
}LoginResponse;

typedef struct SignupResponse
{
	unsigned int status;
}SignupResponse;

typedef struct LogoutResponse
{
	unsigned int status;
}LogoutResponse;

typedef struct MessageResponse
{
	unsigned int status;
	std::string message;
}MessageResponse;

typedef struct ChangeMessageResponse
{
	unsigned int status;
	std::string message;
	std::string newMessage;
}ChangeMessageResponse;

typedef struct DeleteMessageResponse
{
	unsigned int status;
	std::string srcUser;
}DeleteMessageResponse;

typedef struct DeleteChatResponse
{
	unsigned int status;
}DeleteChatResponse;

typedef struct GetRequestResponse
{
	unsigned int status;
	std::string url;
}GetRequestResponse;

typedef struct CreateChatResponse
{
	unsigned int status;
	std::string srcUser;
	std::string dstUser;
}CreateChatResponse;

/*
typedef struct CloseRoomResponse
{
	unsigned int status;
}CloseRoomResponse;

typedef struct StartGameResponse
{
	unsigned int status;
}StartGameResponse;

typedef struct LeaveRoomResponse
{
	unsigned int status;
}LeaveRoomResponse;

typedef struct GetRoomStateResponse
{
	unsigned int status;
	bool hasGameBegun;
	std::vector<std::string> players;
	unsigned int questionCount;
	unsigned int answerTimeout;
}GetRoomStateResponse;


typedef struct LeaveGameResponse
{
	unsigned int status;
}LeaveGameResponse;

typedef struct GetQuestionResponse
{
	unsigned int status;
	std::string question;
	std::map<unsigned int, std::string> answers;
}GetQuestionResponse;

typedef struct SubmitAnswerResponse
{
	unsigned int status;
	unsigned int correctAnswerId;
}SubmitAnswerResponse;


typedef struct PlayerResults
{
	std::string username;
	unsigned int correctAnswerCount;
	unsigned int wrongAnswerCount;
	unsigned int averageAnswerCount;
}PlayerResults;

typedef struct GetGameResultsResponse
{
	unsigned int status;
	std::vector<PlayerResults> results;
}GetGameResultsResponse;
*/


class JsonResponsePacketSerializer
{
public:
	static std::string serializeResponse(ErrorResponse error);
	static std::string serializeResponse(LoginResponse login);
	static std::string serializeResponse(SignupResponse signup);

	static std::string serializeResponse(LogoutResponse response);
	static std::string serializeResponse(MessageResponse response);
	static std::string serializeResponse(ChangeMessageResponse response);
	static std::string serializeResponse(DeleteMessageResponse response);
	static std::string serializeResponse(DeleteChatResponse response);
	static std::string serializeResponse(GetRequestResponse response);
	static std::string  serializeResponse(CreateChatResponse response);

private:
	static void intToBytes(std::string& buffer, std::string size);
};
