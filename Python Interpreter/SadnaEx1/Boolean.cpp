#include "Boolean.h"

Boolean::Boolean(bool b)
{
	_Boolean = b;
}
const bool Boolean::isPrintable()
{
	return true;
}
const std::string Boolean::toString()
{
	return _Boolean ? "true" : "false";
}