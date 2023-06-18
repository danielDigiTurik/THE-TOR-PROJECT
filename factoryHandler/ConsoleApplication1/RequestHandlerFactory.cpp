#include "RequestHandlerFactory.h"

RequestHandlerFactory::RequestHandlerFactory(IDatabase* _db) : m_database(_db), m_loginManager(_db){}

LoginRequestHandler* RequestHandlerFactory::createLoginRequestHandler()
{
	LoginRequestHandler* request = new LoginRequestHandler(this, &this->m_loginManager);
	return request;
}

LoginManager* RequestHandlerFactory::getLoginManager()
{
	return &this->m_loginManager;
}

MenuRequestHandler* RequestHandlerFactory::createMenuRequestHandler(LoggedUser loggedUs)
{
	MenuRequestHandler* menu = new MenuRequestHandler(this->m_loginManager, loggedUs, this);
	return menu;
}