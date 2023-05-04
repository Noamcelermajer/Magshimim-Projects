#include <iostream>
#include <string>
#include "MemoryAccess.h"
#include "AlbumManager.h"
#include "DatabaseAccess.h"
#include <time.h>
#include <chrono>

//define section
#define PROGRAMMER_NAME "Noam Celermajer"


//function definition section
std::string current_time();
std::string date();
void print_info();
int getCommandNumberFromUser()
{
	std::string message("\nPlease enter any command(use number): ");
	std::string numericStr("0123456789");
	
	std::cout << message << std::endl;
	std::string input;
	std::getline(std::cin, input);
	
	while (std::cin.fail() || std::cin.eof() || input.find_first_not_of(numericStr) != std::string::npos) {

		std::cout << "Please enter a number only!" << std::endl;

		if (input.find_first_not_of(numericStr) == std::string::npos) {
			std::cin.clear();
		}

		std::cout << std::endl << message << std::endl;
		std::getline(std::cin, input);
	}
	
	return std::atoi(input.c_str());
}

int main(void)
{
	// initialization data access
	DatabaseAccess dataAccess;

	// initialize album manager
	AlbumManager albumManager(dataAccess);


	std::string albumName;
	std::cout << "Welcome to Gallery!" << std::endl;
	std::cout << "===================" << std::endl;
	std::cout << "Type " << HELP << " to a list of all supported commands" << std::endl;
	
	do {
		int commandNumber = getCommandNumberFromUser();
		
		try	{
			albumManager.executeCommand(static_cast<CommandType>(commandNumber));
		} catch (std::exception& e) {	
			std::cout << e.what() << std::endl;
		}
	} 
	while (true);
}

void print_info()
{
	std::cout << "Programmer :" << PROGRAMMER_NAME << std::endl << "Current Date&Time : " << date() << " " << current_time() << std::endl;
}

std::string current_time()
{
	time_t now = time(NULL);
	struct tm tstruct;
	char time[40];
	tstruct = *localtime(&now);
	strftime(time, sizeof(time), "%X", &tstruct);
	return time;
}
std::string date()
{
	std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

	char buffer[100] = "";
	std::strftime(buffer, sizeof(buffer), "%Y/%m/%d", std::localtime(&now));
	return buffer;
}