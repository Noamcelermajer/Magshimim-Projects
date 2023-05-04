#pragma once
#include "Board.h"
#include <iostream>
class Game
{
	Board _board;
	bool _whiteTurn;

public:
	Game(bool whiteStarts, std::string boardStr);
	Game(Game& g);
	~Game();

	void set_turn_white();
	void set_turn_dark();
	bool get_turn() const;
	const Board& get_board() const;
	int move(int xSrc, int ySrc, int xDst, int yDst, bool turn);
	std::string to_string() const;
};

