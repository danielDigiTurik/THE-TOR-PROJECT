#pragma once
#include <iostream>
#include "IDatabase.h"
#include "LoggedUser.h"
#include "SqliteDatabase.h"
#include <vector>
#include <string>
#include <iterator>


class LoginManager
{
private:
    IDatabase* _db;
    static std::vector<LoggedUser> _loggedUsers;

public:
    LoginManager(IDatabase* _db);
    void signup(std::string username, std::string pass, std::string email);
    void login(std::string username, std::string pass);
    void logout(std::string username);
};