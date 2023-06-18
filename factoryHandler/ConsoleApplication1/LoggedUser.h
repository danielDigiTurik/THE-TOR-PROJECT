#pragma once
#include <iostream>
#include <string>
#include <vector>

class LoggedUser
{
private:
    std::string _username;

public:
    LoggedUser(std::string username);
    std::string getUserName();
    bool operator==(const LoggedUser& other);
};