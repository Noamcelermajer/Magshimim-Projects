#ifndef _HELPER_H
#define _HELPER_H

#include <string>

// this is service class with static functions
class Helper
{
public:
	// check if the parameter is valid Integer value
	static bool isInteger(const std::string& s);

	// check if the parameter is valid Boolean value
	static bool isBoolean(const std::string& s);

	// check if the parameter is valid String value
	static bool isString(const std::string& s);

	// check if the parameter is digit
	static bool isDigit(char c);

	// check if the parameter is lower letter
	static bool isLowerLetter(char c);	
	
	// check if the parameter is upper letter
	static bool isUpperLetter(char c);

	// check if the parameter is  letter
	static bool isLetter(char c);

	// check if the parameter is underscore ('_')
	static bool isUnderscore(char c);

	// remove whitespace from the begining and the end of the string
	static void trim(std::string &str);

	// remove whitespace from the end of the string
	static void rtrim(std::string &str);  

	// remove whitespace from the beginning of the string
	static void ltrim(std::string &str); 

	// remove leading zeros from the beginning of the string
	static void removeLeadingZeros(std::string &str); 

};

#endif