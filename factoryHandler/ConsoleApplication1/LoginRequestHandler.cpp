#include "LoginRequestHandler.h"

/*
    Constructor for LoginRequestHandler's class.
    Input: Class parameters (RequestHandlerFactory*,LoginManager*).
    Output: None
*/
LoginRequestHandler::LoginRequestHandler(RequestHandlerFactory* rFactory, LoginManager* LogM)
{
    this->m_handlerFactory = rFactory;
    this->m_loginManager = LogM;
}

/*
    Destructor for LoginRequestHandler's class.
    Input: None
    Output: None
*/
LoginRequestHandler::~LoginRequestHandler()
{
    delete m_handlerFactory;
    delete m_loginManager;
}

/*
    Function checkes if the requestId is for login or signup.
    Input: RequestInfo
    Output: True/False
*/
bool LoginRequestHandler::isRequestRelevant(RequestInfo request)
{
    if (request.id == LOGIN_ID || request.id == SIGNUP_ID)
    {
        return true;
    }
    else
    {
        return false;
    }
}

/*
    Function find out if the request is for login/signup/neither
    Input && Output: RequestInfo
*/
RequestResult LoginRequestHandler::handleRequest(RequestInfo request)
{
    RequestResult result;

    if (!isRequestRelevant(request))
    {
        LogoutResponse response;
        response.status = 3;
        //result.response = JsonResponsePacketSerializer::serializeResponse(response);
        return result;
    }

    if (LOGIN_ID == request.id)
    {
        return login(request);
    }
    else
    {
        return signup(request);
    }
}

/*
    Function do login with the all data that it got
    Input: RequestInfo with the message and data.
    Output: RequestResult
*/
RequestResult LoginRequestHandler::login(RequestInfo request)
{
    RequestResult result;
    LoginRequest logReq = JsonRequestPacketDeserializer::deserializeLoginRequest(request.buffer);
    LoginResponse response;
    response.status = 1; // Login id
    LoggedUser user = LoggedUser(logReq.username);
    result.newHandler = this->m_handlerFactory->createMenuRequestHandler(user);
   // result.response = JsonResponsePacketSerializer::serializeResponse(response);
    m_loginManager->login(logReq.username, logReq.password);
    return result;
}

/*
    Function do signup with the all data that it got
    Input: RequestInfo with the message and data.
    Output: RequestResult
*/
RequestResult LoginRequestHandler::signup(RequestInfo request)
{
    RequestResult result;
    SignupRequest signReq = JsonRequestPacketDeserializer::deserializeSignUpRequest(request.buffer);
    SignupResponse response;
    response.status = 2; // Signup id
    LoggedUser user = LoggedUser(signReq.username);
    result.newHandler = this->m_handlerFactory->createMenuRequestHandler(user);
    //result.response = JsonResponsePacketSerializer::serializeResponse(response);
    m_loginManager->signup(signReq.username, signReq.password, signReq.email);
    return result;
}