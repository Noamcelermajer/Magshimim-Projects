#include "type.h"
#include "InterperterException.h"
#include "parser.h"
#include <iostream>

#define WELCOME "Welcome to Magshimim Python Interperter version 1.0 by "
#define YOUR_NAME "Noam"


int main(int argc,char **argv)
{
	std::cout << WELCOME << YOUR_NAME << std::endl;
	std::string input_string;

	do
	{
		std::cout << ">>> ";
		std::getline(std::cin, input_string);
		try
		{
			Type* t = Parser::parseString(input_string);
			if (t->isPrintable())
			{
				std::cout << t->toString() << std::endl;
			}
			if (t->get_isTemp())
			{
				delete t;
			}
		}
		catch (const InterperterException & e)
		{
			std::cout << e.what() << std::endl;
		}
		
	} while (input_string != "quit()");

	
	return 0;
}


