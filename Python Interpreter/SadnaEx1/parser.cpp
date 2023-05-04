#include "parser.h"
#include <iostream>

Type* Parser::parseString(std::string str)
{
	if (str != "quit()")
	{
		Type* t;
		if (!str.rfind(" ", 0) || !str.rfind('\t', 0))
			throw IndentationException();
		Helper::trim(str);
		t = getType(str);
		if (t == nullptr)
			throw SyntaxException();
		else
		{
			t->set_isTemp(true);
			return t;
		}
		
	}
	return NULL;
}

Type* Parser::getType(std::string &str)
{
	if (str.size() > 0)
	{
		if (Helper::isInteger(str))
		{
			Helper::removeLeadingZeros(str);
			return new Integer(std::stoi(str));
		}
		else if (Helper::isString(str))
		{
			return new String(str);
		}
		else if (Helper::isBoolean(str))
		{
			if (str == "False")
				return new Boolean(false);
			return new Boolean(true);
		}
	}
	return nullptr;
}

bool Parser::isLegalVarName(const std::string& str)
{
	for (int i = 0; i < str.size(); i++)
	{
		if (Helper::isLetter(str.c_str()[i]) || Helper::isDigit(str.c_str()[i]) || Helper::isUnderscore(str.c_str()[i]))
			return false;
	}
	if (Helper::isDigit(str[0]))
		return false;
	return true;
}

bool Parser::makeAssignment(std::string& str)
{
	std::size_t pos = str.find("=");
	if (pos == std::string::npos)
		return false;
	std::string name = str.substr(0, pos);
	Helper::trim(name);
	std::string value = str.substr(pos+1 , str.size()-1);
	Helper::trim(value);
	if (!isLegalVarName(name))
		throw SyntaxException();
	Type* t;
	t = getType(str);
	if (t == nullptr)
		throw SyntaxException();
	//_variables.insert({ name, t });
	return true;
}