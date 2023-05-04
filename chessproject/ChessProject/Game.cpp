#include "Game.h"
#include <string>
#include <exception>


Game::Game(bool whiteStarts, std::string boardStr) : _board(boardStr)
{
	_whiteTurn = whiteStarts;
}
Game::Game(Game& g) : _board(g._board)
{
	_whiteTurn = g._whiteTurn;
}
Game::~Game()
{

}

bool Game::get_turn() const
{
	return _whiteTurn;
}
void Game::set_turn_white()
{
	_whiteTurn = true;
}
void Game::set_turn_dark()
{
	_whiteTurn = false;
}

std::string Game::to_string() const
{
	return _board.to_string() + (_whiteTurn ? '0' : '1');
}
const Board& Game::get_board() const
{
	return _board;
}
int Game::move(int xSrc, int ySrc, int xDst, int yDst, bool turn)
{
	return _board.move(xSrc, ySrc, xDst, yDst, turn);
}
