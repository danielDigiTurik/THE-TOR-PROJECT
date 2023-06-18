#pragma once
#include "LoginManager.h"
#include "LoginRequestHandler.h"
#include "IDatabase.h"
#include "MenuRequestHandler.h"

class LoginRequestHandler;
class MenuRequestHandler;

class RequestHandlerFactory
{
public:
	RequestHandlerFactory(IDatabase* _db);
	LoginRequestHandler* createLoginRequestHandler();
	LoginManager* getLoginManager();
	MenuRequestHandler* createMenuRequestHandler(LoggedUser loggedUs);

private:
	LoginManager m_loginManager;
	IDatabase* m_database;
};
