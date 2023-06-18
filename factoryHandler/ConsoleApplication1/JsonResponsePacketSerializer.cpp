#include "JsonResponsePacketSerializer.h"

/*
	Function serialize for messages that "leaving the server".
	Input: The error response
	Output: Buffer as std::vector<unsigned char>
*/
std::string JsonResponsePacketSerializer::serializeResponse(ErrorResponse error)
{

	json jsonObj;
	jsonObj["message"] = error._message;
	std::string buffer;
	std::string message = jsonObj.dump();
	std::string msgSize = std::to_string(message.length());

	buffer.push_back(ERROR_STATUS);
	intToBytes(buffer, msgSize);
	for (int i = 0; i < message.length(); i++)
	{
		buffer.push_back(message[i]);
	}

	return buffer;
}

/*
	Function serialize for messages that "leaving the server".
	Input: The login response
	Output: Buffer as std::vector<unsigned char>
*/
std::string JsonResponsePacketSerializer::serializeResponse(LoginResponse login)
{
	std::cout << "FUNCTION: JsonResponsePacketSerializer::serializeResponse(LoginResponse login)";

	json jsonObj;
	jsonObj["status"] = login.status;
	std::string buffer;
	std::string message = jsonObj.dump();
	std::string msgSize = std::to_string(message.length());

	buffer.push_back(LOGIN_STATUS);
	intToBytes(buffer, msgSize);
	for (int i = 0; i < message.length(); i++)
	{
		buffer.push_back(message[i]);
	}
	return buffer;
}

/*
	Function serialize for messages that "leaving the server".
	Input: Response for the sign up
	Output: Buffer as std::vector<unsigned char>
*/
std::string JsonResponsePacketSerializer::serializeResponse(SignupResponse signup)
{
	json jsonObj;
	jsonObj["status"] = signup.status;
	std::string buffer;
	std::string message = jsonObj.dump();
	std::string msgSize = std::to_string(message.length());

	buffer.push_back(SIGNUP_STATUS);
	intToBytes(buffer, msgSize);
	for (int i = 0; i < message.length(); i++)
	{
		buffer.push_back(message[i]);
	}
	return buffer;
}

/*
	Function serialize for messages that "leaving the server".
	Input: Logout response
	Output: Buffer as std::vector<unsigned char>
*/
std::string JsonResponsePacketSerializer::serializeResponse(LogoutResponse response)
{
	json jsonObj;
	jsonObj["status"] = response.status;
	std::string buffer;
	std::string message = jsonObj.dump();
	std::string msgSize = std::to_string(message.length());

	buffer.push_back(LOGOUT_STATUS);
	intToBytes(buffer, msgSize);
	for (int i = 0; i < message.length(); i++)
	{
		buffer.push_back(message[i]);
	}
	return buffer;
}

///////////////////////////////////////////////////////////////////////////////////

/*
	Function serialize for messages that "leaving the server".
	Input: Join room response
	Output: Buffer as std::vector<unsigned char>
*/
std::string JsonResponsePacketSerializer::serializeResponse(MessageResponse response)
{

	json jsonObj;
	jsonObj["status"] = response.status;
	std::string buffer;
	std::string message = jsonObj.dump();
	std::string msgSize = std::to_string(message.length());

	buffer.push_back(MESSAGE_STATUS);
	intToBytes(buffer, msgSize);
	for (int i = 0; i < message.length(); i++)
	{
		buffer.push_back(message[i]);
	}

	return buffer;
}

/*
	Function serialize for messages that "leaving the server".
	Input: Create room response
	Output: Buffer as std::vector<unsigned char>
*/

std::string JsonResponsePacketSerializer::serializeResponse(ChangeMessageResponse response)
{
	json jsonObj;
	jsonObj["status"] = response.status;
	std::string buffer;
	std::string message = jsonObj.dump();
	std::string msgSize = std::to_string(message.length());

	buffer.push_back(CHANGEMSG_STATUS);
	intToBytes(buffer, msgSize);
	for (int i = 0; i < message.length(); i++)
	{
		buffer.push_back(message[i]);
	}

	return buffer;
}

/*
	Function serialize for messages that "leaving the server".
	Input: Get room response
	Output: Buffer as std::vector<unsigned char>
*/
std::string JsonResponsePacketSerializer::serializeResponse(DeleteMessageResponse response)
{
	std::string buffer;
	buffer.push_back(DELETEMSG_STATUS);
	return buffer;
}

/*
	Function serialize for messages that "leaving the server".
	Input: Get players in room response.
	Output: Buffer as std::vector<unsigned char>
*/
std::string JsonResponsePacketSerializer::serializeResponse(DeleteChatResponse response)
{
	std::string buffer;
	buffer.push_back(DELETECHAT_STATUS);
	return buffer;
}

/*
	Function serialize for messages that "leaving the server".
	Input: Get high score response.
	Output: Buffer as std::vector<unsigned char>
*/
std::string JsonResponsePacketSerializer::serializeResponse(GetRequestResponse response)
{
	std::string buffer;
	buffer.push_back(GETREQUEST_STATUS);
	return buffer;
}

/*
	Function serialize for messages that "leaving the server".
	Input: Get personal status
	Output: Buffer as std::vector<unsigned char>
*/
std::string JsonResponsePacketSerializer::serializeResponse(CreateChatResponse response)
{
	std::string buffer;
	buffer.push_back(CREATECHAT_STATUS);
	return buffer;
}

/*
	Function serialize for messages that "leaving the server".
	Input: Get high score response.
	Output: Buffer as std::vector<unsigned char>
*/

void JsonResponsePacketSerializer::intToBytes(std::string& buffer, std::string size)
{
	for (int i = 0; i < 4 - size.length(); i++)
	{
		buffer.push_back('0');
	}

	for (int i = 0; i < size.length(); i++)
	{
		buffer.push_back(size[i]);
	}
}