#include "Integer.h"

Integer::Integer(int n) 
{
	_integer = n;
}
const bool Integer::isPrintable()
{
	return true;
}
const std::string Integer::toString()
{
	return std::to_string(_integer);
}