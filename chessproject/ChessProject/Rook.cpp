#include "Rook.h"



Rook::Rook(bool isWhite, int xPos, int yPos) : Piece(isWhite, xPos, yPos)
{
}
Rook::Rook(const Rook& r) : Piece(r._isWhite, r._xPos, r._yPos)
{}
Rook::~Rook()
{
}

Rook* Rook::clone() const
{
	return new Rook(*this);
}
/*Thsi function checking if the rook move is valid (really easy the rook can go up /down left right)*/
int Rook::is_valid_move(int xDst, int yDst, const Board &b) const
{
	if (xDst == _xPos && _yPos == yDst)//can move to himself
		return COORDINATE_EQUALS_CURRENT;
	if (b[xDst][yDst] != nullptr && b[xDst][yDst]->get_is_white() == _isWhite)//can eat allies
		return ALREDY_PIECE_IN_DST;	// if same color
	if (_xPos == xDst && _yPos != yDst)// case that the rook go up or down
	{
		for (int i = _yPos < yDst ? _yPos + 1 : _yPos - 1; _yPos < yDst ? i < yDst : i > yDst; _yPos < yDst ? i++ : i--)
			if (b[_xPos][i] != nullptr)
				return INVALID_MOVEMENT;
	}
	else if (_xPos != xDst && _yPos == yDst)//case that rook go left r right
	{
		for (int i = _xPos < xDst ? _xPos + 1 : _xPos - 1; _xPos < xDst ? i < xDst : i > xDst; _xPos < xDst ? i++ : i--)
			if (b[i][_yPos] != nullptr)//mean that a piece no matter wich color stop him from going to is destination
				return INVALID_MOVEMENT;
	}
	else
		return INVALID_MOVEMENT;
	return GOOD_MOVE;
}
char Rook::to_string() const
{
	return _isWhite ? 'R' : 'r';
}
