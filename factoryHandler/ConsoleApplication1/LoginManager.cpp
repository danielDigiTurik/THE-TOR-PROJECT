#include "LoginManager.h"

std::vector<LoggedUser> LoginManager::_loggedUsers;

// Constructor for LoginManager's class
LoginManager::LoginManager(IDatabase* _db)
{
	this->_db = _db;
}

/*
	Function will check and add user if user really exist.
	Input: Signup parameters (username, password, email).
	Output: None
*/
void LoginManager::signup(std::string username, std::string pass, std::string email)
{
	if (!_db->doesUserExist(username))
	{
		_db->addNewUser(username, pass, email);
		LoggedUser user = LoggedUser(username);
		this->_loggedUsers.push_back(user);
		std::cout << "user added" << std::endl;
	}
	else
	{
		throw std::string("user already exists");
	}
}

/*
	Function will check and add user if user really exist.
	Input: Signup parameters (username, password, email).
	Output: None
*/
void LoginManager::login(std::string username, std::string pass)
{
	if (_db->doesUserExist(username)) // Check if user exists
	{
		if (_db->doesPasswordMatch(username, pass)) // Check the password
		{
			std::vector<LoggedUser>::iterator userIt = find(_loggedUsers.begin(), _loggedUsers.end(), username);
			if (userIt == _loggedUsers.end())
			{
				this->_loggedUsers.push_back(LoggedUser(username));
				std::cout << "logged" << std::endl;
			}
			else
			{
				throw std::string("user already logged in");
			}
		}
		else
		{
			throw std::string("password not match");
		}
	}
	else
	{
		throw std::string("user does not exist");
	}
}

/*
	Function logout by getting username.
	Input: username
	Output: None
*/
void LoginManager::logout(std::string username)
{
	std::vector<LoggedUser>::iterator userIt = find(_loggedUsers.begin(), _loggedUsers.end(), username);

	if (userIt != _loggedUsers.end())
	{
		std::cout << "logout" << std::endl;
		this->_loggedUsers.erase(userIt);
	}
	else
	{
		throw std::string("This user is not logged");
	}
}
