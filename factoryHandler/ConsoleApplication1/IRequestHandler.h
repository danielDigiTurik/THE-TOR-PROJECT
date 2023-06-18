#pragma once
#include <iostream>
#include <ctime>
#include <vector>
#include "JsonRequestPacketDeserializer.h"
#include "JsonResponsePacketSerializer.h"

class IRequestHandler;
class JsonRequestPacketDeserializer;
class JsonResponsePacketSerializer;

typedef struct RequestResult
{
    std::string response;
    IRequestHandler* newHandler;
}RequestResult;

typedef struct RequestInfo
{
    char id;
    time_t recievalTime;
    std::string buffer;

}RequestInfo;

class IRequestHandler
{
public:
    virtual ~IRequestHandler() {};
    virtual bool isRequestRelevant(RequestInfo request) = 0;
    virtual RequestResult handleRequest(RequestInfo request) = 0;
};