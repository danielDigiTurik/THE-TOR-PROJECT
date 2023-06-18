#include "MenuRequestHandler.h"

int roomCounter = 1;

// Constructor for MenuRequestHandler's class
MenuRequestHandler::MenuRequestHandler(LoginManager logManager, LoggedUser loggedUs, RequestHandlerFactory* handlerFactory) : m_handlerFactory(handlerFactory), loginManager(logManager), _user(loggedUs)
{

}

/*
	Function check if the request still relevent.
	Input: RequestInfo
	Output: True/False
*/
bool MenuRequestHandler::isRequestRelevant(RequestInfo request)
{
	if (request.id == LOGIN_STATUS || request.id == SIGNUP_STATUS || request.id == LOGOUT_STATUS)
	{
		return true;
	}
	else
	{
		return true; //?
	}
}

/*
	Function check and take the request that requested to.
	Input: RequestInfo
	Output: RequestResult
*/
RequestResult MenuRequestHandler::handleRequest(RequestInfo request)
{
	RequestResult result;
	if (!isRequestRelevant(request))
	{
		result.response;
		return result;
	}
	else
	{
		std::cout << "id:" << request.id << "\n";
		switch (request.id)
		{
		case LOGIN_STATUS:
		{
			throw std::string("user already logged in");
			break;
		}
		case SIGNUP_STATUS:
		{
			throw std::string("user already exists");
			break;
		}
		case LOGOUT_STATUS:
		{
			result = signOut(request);
			break;
		}
		case MESSAGE_STATUS:
		{
			std::cout <<"getMessage";
			result = getMessage(request);
			std::cout << result.response;
			break;
		}
		case CHANGEMSG_STATUS:
		{
			result = changeMessage(request);
			break;
		}
		case DELETEMSG_STATUS:
		{
			result = deleteMessage(request);
			break;
		}
		case DELETECHAT_STATUS:
		{
			result = deleteChat(request);
			break;
		}
		case GETREQUEST_STATUS:
		{
			result = getRequest(request);
			break;
		}
		case CREATECHAT_STATUS:
		{
			result = createChat(request);
			break;

		default:
		{
			std::cout << "Error" << std::endl;
			break;
		}
		}
		}
		return result;
	}
}

/*
	Function will do signOut.
	Input: RequestInfo
	Output: RequestResult
*/
RequestResult MenuRequestHandler::signOut(RequestInfo request)
{
	RequestResult result;

	LogoutResponse response;
	response.status = LOGOUT_STATUS;
	result.response = JsonResponsePacketSerializer::serializeResponse(response);
	this->loginManager.logout(this->_user.getUserName());
	return result;
}

int callbackCheckMessage(void* data, int argc, char** argv, char** azColName)
{
	std::string* msg = (std::string*)data;
	*msg = argv[0];
	return 0;
}

/*
	Function will returns the rooms.
	Input: RequestInfo
	Output: RequestResult
*/
RequestResult MenuRequestHandler::getMessage(RequestInfo request)
{
	RequestResult result;
	MessageResponse response;

	sqlite3* db;
	std::string dbFileName = "C:/Users/magshimim/Documents/BECKY/POC/client/client/messagesDB.db";
	int doesFileExist = _access(dbFileName.c_str(), 0);
	int opened = sqlite3_open(dbFileName.c_str(), &db);

	if (SQLITE_OK != opened)
	{
		db = nullptr;
		std::cout << "Failed to open DB" << std::endl;
	}
	std::string sqlStatement = "SELECT * FROM MESSAGES;";
	std::string new_message = "";
	char* errMsg = nullptr;

	int res = sqlite3_exec(db, sqlStatement.c_str(), callbackCheckMessage, &new_message, &errMsg); // Sends to db

	if (SQLITE_OK != res)
		std::cout << "ERROR" << std::endl;
	response.status = MESSAGE_STATUS;
	response.message = new_message;
	result.response = new_message;
	result.newHandler = this;
	std::cout << new_message;

	return result;
}

/*
	Function will get players that in the room.
	Input: RequestInfo
	Output: RequestResult
*/
RequestResult MenuRequestHandler::changeMessage(RequestInfo request)
{
	RequestResult result;
	ChangeMessageResponse response;
	result.response = JsonResponsePacketSerializer::serializeResponse(response);
	result.newHandler = this;
	return result;
}

/*
	Function will do signOut.
	Input: RequestInfo
	Output: RequestResult
*/
RequestResult MenuRequestHandler::deleteMessage(RequestInfo request)
{
	RequestResult result;
	DeleteMessageResponse response;
	response.status = DELETEMSG_STATUS;
	result.response = JsonResponsePacketSerializer::serializeResponse(response);
	result.newHandler = this;
	return result;
}

/*
	Function will get and return the high score between the players.
	Input: RequestInfo
	Output: RequestResult
*/
RequestResult MenuRequestHandler::deleteChat(RequestInfo request)
{
	RequestResult result;
	DeleteChatResponse response;
	response.status = DELETECHAT_STATUS;
	result.response = JsonResponsePacketSerializer::serializeResponse(response);
	result.newHandler = this;
	return result;
}

/*
	Function will take user and join it to the room.
	Input: RequestInfo
	Output: RequestResult
*/
RequestResult MenuRequestHandler::getRequest(RequestInfo request)
{
	std::cout << "\joinRoom1\n";

	RequestResult result;
	GetRequestResponse response;
	response.status = GETREQUEST_STATUS;
	result.response = JsonResponsePacketSerializer::serializeResponse(response);

	//result.newHandler = this->m_handlerFactory->createRoomMemberRequestHandler(this->_roomManager->getRoom(joinReq.roomId), _user);
	return result;
}

/*
	Function will create a new room.
	Input: RequestInfo
	Output: RequestResult
*/
RequestResult MenuRequestHandler::createChat(RequestInfo request)
{
	RequestResult result;
	CreateRoomRequest createReq = JsonRequestPacketDeserializer::deserializeCreateRoomRequest(request.buffer);
	CreateChatResponse response;
	response.status = CREATECHAT_STATUS;
	result.response = JsonResponsePacketSerializer::serializeResponse(response);
	//result.newHandler = this->m_handlerFactory->createRoomAdminRequestHandler(this->_roomManager->getRoom(data.id), _user);
	std::cout << "\ncreateRoom2\n";

	return result;
}