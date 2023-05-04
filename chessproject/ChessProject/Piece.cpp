#include "Piece.h"



Piece::Piece(bool isWhite, unsigned xPos, unsigned yPos)
{
	_isWhite = isWhite;
	_xPos = xPos;
	_yPos = yPos;
}
Piece::~Piece()
{
	_isWhite = false;
	_xPos = 0;
	_yPos = 0;
}
//Getters
int Piece::get_y() const
{
	return _yPos;
}
int Piece::get_x() const
{
	return _xPos;
}
bool Piece::get_is_white() const
{
	return _isWhite;
}
//setters
void Piece::set_x(unsigned x)
{
	_xPos = x;
}

void Piece::set_y(unsigned y)
{
	_yPos = y;
}

void Piece::set_first_turn()
{
}
