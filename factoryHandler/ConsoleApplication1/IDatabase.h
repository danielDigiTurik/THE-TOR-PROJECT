#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <list>
#include "sqlite3.h"
//#include "Question.h"

class IDatabase
{
public:
    virtual bool doesUserExist(std::string username) = 0;
    virtual bool doesPasswordMatch(std::string username, std::string password) = 0;
    virtual void addNewUser(std::string username, std::string password, std::string email) = 0;
   //virtual std::list<Question> getQuestions(int num) = 0;
   //virtual float getPlayerAverageAnswerTime(std::string name) = 0;
   //virtual int getNumOfCorrectAnswers(std::string name) = 0;
   //virtual int getNumOfTotalAnswers(std::string name) = 0;
   //virtual int getNumOfPlayerGames(std::string name) = 0;
   //virtual std::vector<std::string> getTopThreeScores() = 0;
};
