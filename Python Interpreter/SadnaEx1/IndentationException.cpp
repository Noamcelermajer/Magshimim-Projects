#include "IndentationException.h"

const char* IndentationException::what() const throw()
{
	return "IndentationError: unexpected indent";
}