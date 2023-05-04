#ifndef BOOLEAN_H
#define BOOLEAN_H
#include "type.h"
class Boolean : public Type
{
	bool _Boolean;
public :
	Boolean(bool b);
	virtual const bool isPrintable();
	virtual const std::string toString();

};

#endif // BOOLEAN_H