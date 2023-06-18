#include "SqliteDatabase.h"

/*
	Got ideas from the Gallery's project and hus helpers pages (with the queries)
*/
int callbackCheckPassword(void* data, int argc, char** argv, char** azColName);
int callbackDoesExist(void* data, int argc, char** argv, char** azColName);
int callbackGetNum(void* data, int argc, char** argv, char** azColName);

SqliteDatabase::SqliteDatabase()
{
	// There isn't "CREATE TABLE.." querie because the database already built
	std::string dbFileName = "usersDB.db";
	int doesFileExist = _access(dbFileName.c_str(), 0);
	int res = sqlite3_open(dbFileName.c_str(), &this->_db);

	if (SQLITE_OK != res)
	{
		_db = nullptr;
		std::cout << "Failed to open DB" << std::endl;
	}
}

void SqliteDatabase::addNewUser(std::string username, std::string password, std::string email)
{
	std::string sqlStatement = "INSERT INTO Users (USERNAME, PASSWORD, EMAIL) VALUES ('" + username + "','" + password + "','" + email + "');";
	char* errMsg = nullptr;
	int res = sqlite3_exec(_db, sqlStatement.c_str(), nullptr, nullptr, &errMsg); // Sends to db
	if (SQLITE_OK != res)
	{
		std::cout << "ERROR" << std::endl;
	}
}

bool SqliteDatabase::doesPasswordMatch(std::string username, std::string password)
{
	std::string sqlStatement = "SELECT PASSWORD FROM USERS WHERE USERNAME='" + username + "';";
	char* errMsg = nullptr;
	std::string new_password = "";
	int res = sqlite3_exec(_db, sqlStatement.c_str(), callbackCheckPassword, &new_password, &errMsg); // Sends to db
	if (SQLITE_OK != res)
		std::cout << "ERROR" << std::endl;

	if (new_password == password) // Checking if the password correct
	{
		return true;
	}
	return false;
}

int callbackCheckPassword(void* data, int argc, char** argv, char** azColName)
{
	std::string* password = (std::string*)data;
	*password = argv[0];
	return 0;
}

bool SqliteDatabase::doesUserExist(std::string username)
{
	std::string sqlStatement = "SELECT USERNAME FROM USERS WHERE USERNAME = '" + username + "';";
	char* errMsg = nullptr;
	std::string new_username = "";
	int res = sqlite3_exec(_db, sqlStatement.c_str(), callbackDoesExist, &new_username, &errMsg); // Sends to db
	if (new_username == username) // Checking if username really exist
	{
		return true;
	}
	return false;
}

int callbackDoesExist(void* data, int argc, char** argv, char** azColName)
{
	std::string* new_data = (std::string*)data;
	*new_data = argv[0]; //takes the username from the statment
	return 0;
}