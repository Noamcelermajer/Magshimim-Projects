#include "Pipe.h"
#include "Game.h"
#include <iostream>
#include <thread>
#include <string>
#define ASCII_A 'a'
#define ASCII_0 '0'

bool codeIsValid(std::string code);
int proceedMove(string msgFromGraphics, Game& gameInstance);

using namespace std;
int main()
{
	int xDst = 0, yDst = 0;
	int ySrc = 0, xSrc = 0;
	srand(time_t(NULL));
	Pipe p;
	bool isConnect = p.connect();
	Game gameInstance(true, "rnbkqbnrpppppppp################################PPPPPPPPRNBKQBNR");
	string ans, code;//get my code
	while (!isConnect)
	{
		cout << "cant connect to graphics" << endl;
		cout << "Do you try to connect again or exit? (0-try again, 1-exit)" << endl;
		cin >> ans;

		if (ans == "0")
		{
			cout << "trying connect again.." << endl;
			Sleep(5000);
			isConnect = p.connect();
		}
		else
		{

		}
	}

	gameInstance.set_turn_white();
	char msgToGraphics[1024];
	// msgToGraphics should contain the board string accord the protocol

	strcpy_s(msgToGraphics, (gameInstance.to_string()).c_str());

	p.sendMessageToGraphics(msgToGraphics);   // send the board string

	// get message from graphics
	string msgFromGraphics = p.getMessageFromGraphics();
	while (msgFromGraphics != "quit")
	{
		code = std::to_string(proceedMove(msgFromGraphics, gameInstance));
		// according the protocol. Ex: e2e4           (move e2 to e4)
		strcpy_s(msgToGraphics,code.c_str());



		// return result to graphics		
		p.sendMessageToGraphics(msgToGraphics);
		if (codeIsValid(code))
		{
			if (gameInstance.get_turn())
				gameInstance.set_turn_dark();
			else
				gameInstance.set_turn_white();
		}
		
		// get message from graphics
		msgFromGraphics = p.getMessageFromGraphics();


	}

	p.close();
	return 0;
}
int proceedMove(string msgFromGraphics, Game& gameInstance)
{
	int ySrc = msgFromGraphics[0] - ASCII_A;
	int xSrc = 8 - (msgFromGraphics[1] - ASCII_0);

	int yDst = msgFromGraphics[2] - ASCII_A;
	int xDst = 8 - (msgFromGraphics[3] - ASCII_0);
	return gameInstance.move(xSrc, ySrc, xDst, yDst, gameInstance.get_turn());
}
bool codeIsValid(std::string code)
{
	if (code == "1" || code == "0")
		return true;
	else
		return false;
}