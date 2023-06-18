#pragma once
#include "IDatabase.h"
#include "sqlite3.h"
#include <map>
#include <string>
#include <io.h>

class SqliteDatabase : public IDatabase
{
private:
    sqlite3* _db;
public:
    SqliteDatabase();
    bool doesUserExist(std::string username) override;
    bool doesPasswordMatch(std::string username, std::string password) override;
    void addNewUser(std::string username, std::string password, std::string email) override;
};
