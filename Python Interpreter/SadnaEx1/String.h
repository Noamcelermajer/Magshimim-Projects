#ifndef STRING_H
#define STRING_H
#include "Sequence.h"
class String : public Type
{
	std::string _str;
public:
	String(std::string str);
	virtual const bool isPrintable();
	virtual const std::string toString();
};
#endif // STRING_H