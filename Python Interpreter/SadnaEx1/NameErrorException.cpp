#include "NameErrorException.h"

const char* NameErrorException::what() const throw()
{
	std::string tmp = "NameError : name" + _name + "is not defined";
	return tmp.c_str();
}