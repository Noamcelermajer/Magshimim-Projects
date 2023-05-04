#ifndef SYNTAX_EXCEPTION_H
#define SYNTAX_EXCEPTION_H
#include "InterperterException.h"

class SyntaxException : public InterperterException
{
public:
	virtual const char* what() const throw();

};

#endif // SYNTAX_EXCEPTION_H