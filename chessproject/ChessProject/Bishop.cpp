#include "Bishop.h"
#include <iostream>// 
#include <cmath>// for abs but we do not use it
#define  ABS 1
Bishop::Bishop(bool isWhite, int xPos, int yPos) : Piece(isWhite, xPos, yPos)//calling the ctor of piece :)
{
}
Bishop::~Bishop()
{
	_isWhite = false;
	_xPos = 0;
	_yPos = 0;
}

Bishop* Bishop::clone() const//used in ctor(we used a virtual copy-ctor) 
{
	return new Bishop(*this);
}

int Bishop::is_valid_move(int xDst, int yDst, const Board &b) const
{
	int x = 0, y = 0, xdir = 0, ydir = 0;// ydir and xdir are remplacement to abs() ...
	if (_xPos == xDst && _yPos == yDst)
		return COORDINATE_EQUALS_CURRENT;
	if (b[_xPos][_yPos] == nullptr)
		return NO_PIECE_SELECTED;
	if (b[xDst][yDst] != nullptr && b[xDst][yDst]->get_is_white() == _isWhite)
		return ALREDY_PIECE_IN_DST;
	
	x = xDst - _xPos;
	y = yDst - _yPos;

	xdir = x < 0 ? -ABS : ABS;
	ydir = y < 0 ? -ABS : ABS;

	x *= xdir;
	y *= ydir;
	
	if (x != y)//if abs(xDst - _xPos) != abs(yDst - _yPos) mean is not vertical Simple geo :-)
	{
		return INVALID_MOVEMENT;
	}
	for (int i = _xPos + xdir, j = _yPos + ydir; i != xDst; i += xdir, j += ydir)//loop that go in vertical (downleft/upleft/upRight/downRight) depending on xdir and ydir (the test that we jsut do..)
	{
		if (b[i][j] != NULL)//if there a Piece in the way no matter wich color the bishop can pass
		{
			return INVALID_MOVEMENT;
		}
	}
	return GOOD_MOVE;
}
char Bishop::to_string() const
{
	return _isWhite ? 'B' : 'b';
}
