#ifndef INTEGER_H
#define INTEGER_H
#include "type.h"
class Integer : public Type
{
	int _integer;
public: 
	Integer(int n);
	virtual const bool isPrintable();
	virtual const std::string toString();

};

#endif // INTEGER_H