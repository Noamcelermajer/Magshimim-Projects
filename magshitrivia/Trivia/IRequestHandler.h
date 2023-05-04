#pragma once
#include "RequestStructs.h"

class IRequestHandler
{
public:
	virtual bool isRequestRelevant(const Request& req) = 0;
	virtual RequestResult* handleRequest(const Request& req) = 0;
};