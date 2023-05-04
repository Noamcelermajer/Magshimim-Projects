#ifndef INDENTATION_EXCEPTION_H
#define INDENTATION_EXCEPTION_H

#include "InterperterException.h"

class IndentationException : public InterperterException
{
public:
	virtual const char* what() const throw();
};

#endif // INDENTATION_EXCEPTION_H