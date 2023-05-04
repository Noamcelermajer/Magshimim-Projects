#ifndef PARSER_H
#define PARSER_H

#include "Integer.h"
#include "Boolean.h"
#include "String.h"
#include "IndentationException.h"
#include "SyntaxException.h"
#include "InterperterException.h"
#include "type.h"
#include "Helper.h"
#include <string>
#include <unordered_map>
#include <iostream>
#include <sstream>

class Parser
{
public:
	static Type* parseString(std::string str);
	static Type* getType(std::string &str);

private:
	static bool isLegalVarName(const std::string& str);
	static bool makeAssignment(std::string& str);


};

#endif //PARSER_H
