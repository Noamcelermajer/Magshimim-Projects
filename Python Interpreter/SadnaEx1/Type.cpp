#include "type.h"

Type::Type()
{
	_isTemp = false;
}
void Type::set_isTemp(const bool isTemp)
{
	_isTemp = isTemp;
}
const bool Type::get_isTemp()
{
	return _isTemp;
}