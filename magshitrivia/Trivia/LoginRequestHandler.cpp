#include "LoginRequestHandler.h"

bool LoginRequestHandler::isRequestRelevant(const Request& req)
{
	try
	{
		nlohmann::json jsonData = nlohmann::json::parse(req.buffer);
		if (jsonData["type"] != "login" && jsonData["type"] != "signup")
			return false;
	}

	catch (const std::exception& exp)
	{
		exp.what();
		return false;
	}

	if (req.buffer.size() != 0)
		return true; //Meaning there's an actual request.

	return false;
}

RequestResult* LoginRequestHandler::handleRequest(const Request& req)
{
	RequestResult* ret = new RequestResult();
	RequestResult temp;
	nlohmann::json jsonData;

	if (isRequestRelevant(req))
	{
		jsonData = nlohmann::json::parse(req.buffer);

		if (jsonData["username"].dump() != "null")
		{
			if (jsonData["password"].dump() != "null" && jsonData["email"].dump() != "null" && jsonData["type"] == "signup")
			{
				temp = signup(req);
				if(temp.response == "success") 
					login(req);
				ret->response = "signup: " + temp.response;
			}

			else if(jsonData["password"].dump() != "null" && jsonData["type"] == "login")
			{
				temp = login(req);
				ret->response = "login: " + temp.response;
			}

			ret->requestHandler = temp.requestHandler;
			ret->username = jsonData["username"].dump();
		}
		
		else
			ret->response = "invalid";
		return ret;
	}
	
	ret->response = "invalid";
	return ret;
}

RequestResult LoginRequestHandler::login(Request req)
{
	
	JsonPacketDeserializer deserializer;
	LoginRequest logReq = deserializer.deserializeLoginRequest(req);
	RequestResult ret;
	ret.requestHandler = this;
	ret.response = "invalid";

	if (logReq.password != "null" && logReq.username != "null")
	{
		try
		{
			m_loginManager.login(logReq.username, logReq.password);
			ret.response = "success";
		}

		catch (const std::exception& exp)
		{
			std::string resp = exp.what();
			if (resp == "User already logged in")
				ret.response = "err_user_logged_in";

			else if (resp == "User does not exist")
				ret.response = "err_user_doesnt_exist";

			else if (resp == "Password mismatch")
				ret.response = "err_password_mismatch";
		}
	}

	if (ret.response != "success")
		ret.requestHandler = nullptr;

	return ret;
}

RequestResult LoginRequestHandler::signup(Request req)
{
	JsonPacketDeserializer deserializer;
	SignupRequest signReq = deserializer.deserializeSignupRequest(req);
	RequestResult ret;
	ret.requestHandler = this;
	
	if (signReq.email != "null" && signReq.password != "null" && signReq.username != "null")
	{
		try
		{
			m_loginManager.signup(signReq.username, signReq.password, signReq.email);
			ret.response = "success";
		}

		catch (const std::exception& exp)
		{
			std::string resp = exp.what();

			if (resp == "User already exists")
				ret.response = "err_user_exist";
			
			else if (resp == "Invalid characters in username")
				ret.response = "err_invalid_input_username";
			
			else if (resp == "Invalid characters in password")
				ret.response = "err_invalid_input_password";

			else if (resp == "Invalid characters in email")
				ret.response = "err_invalid_input_email";
		}
	}

	if (ret.response != "success")
		ret.requestHandler = nullptr;

	return ret;
}

LoginRequestHandler::LoginRequestHandler(RequestHandlerFactory* loginFactory, LoginManager& loginManager) : m_loginManager(loginManager)
{
	m_loginFactory = loginFactory;
}

LoginRequestHandler::~LoginRequestHandler()
{
}

void LoginRequestHandler::shutDown()
{
	m_loginManager.shutDown();

}