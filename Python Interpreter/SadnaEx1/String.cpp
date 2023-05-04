#include "String.h"


String::String(std::string str)
{
	_str = str;
}
const bool String::isPrintable()
{
	return true;
}
const std::string String::toString()
{
	return _str;
}