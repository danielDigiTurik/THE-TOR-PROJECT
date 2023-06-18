#pragma once
#include "IRequestHandler.h"
#include "RequestHandlerFactory.h"
#include "JsonResponsePacketSerializer.h"
//#include "StatisticsManager.h"

class RequestHandlerFactory;
class LoggedUser;
class RoomManager;

class MenuRequestHandler : public IRequestHandler
{
public:
	bool isRequestRelevant(RequestInfo request);
	RequestResult handleRequest(RequestInfo request);
	MenuRequestHandler(LoginManager loginManager, LoggedUser loggedUs, RequestHandlerFactory* handlerFactory);
	//MenuRequestHandler(LoginManager loginManager, StatisticsManager* statManager, LoggedUser loggedUs, RequestHandlerFactory* handlerFactory, RoomManager* roomManager);

private:
	LoggedUser _user;
	RoomManager* _roomManager;
	RequestHandlerFactory* m_handlerFactory;
	LoginManager loginManager;
	RequestResult signOut(RequestInfo request);
	RequestResult getMessage(RequestInfo request);
	RequestResult changeMessage(RequestInfo request);
	RequestResult deleteMessage(RequestInfo request);
	RequestResult deleteChat(RequestInfo request);
	RequestResult getRequest(RequestInfo request);
	RequestResult createChat(RequestInfo request);

};
