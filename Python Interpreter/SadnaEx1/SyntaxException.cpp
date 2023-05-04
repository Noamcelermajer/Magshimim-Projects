#include "SyntaxException.h"
const char* SyntaxException::what() const throw()
{
	return "SyntaxError: invalid syntax";
}

