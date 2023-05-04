#ifndef TYPE_H
#define TYPE_H
#include <iostream>
#include <string>
class Type
{
	bool _isTemp;
public: 
	Type();
	void set_isTemp(const bool isTemp);
	const bool get_isTemp();
	virtual const bool isPrintable() = 0;
	virtual const std::string toString() = 0;
};





#endif //TYPE_H
