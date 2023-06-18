#include "LoggedUser.h"

LoggedUser::LoggedUser(std::string username)
{
    this->_username = username;
}

std::string LoggedUser::getUserName()
{
    return this->_username;
}

bool LoggedUser::operator==(const LoggedUser& other)
{
    if (this->_username == other._username)
    {
        return true;
    }
    else
    {
        return false;
    }
}