#include "Queen.h"


Queen::Queen(bool isWhite, int xPos, int yPos) : Piece(isWhite, xPos, yPos)
{
}
Queen::Queen(const Queen& q) : Piece(q._isWhite, q._xPos, q._yPos)
{

}
Queen::~Queen()
{
}

Queen* Queen::clone() const
{
	return new Queen(*this);
}
/*THsi fucntion  like all other check if given dst are good and valid mov for the queen :0 this function is like an Rook+bishop function*/
int Queen::is_valid_move(int xDst, int yDst, const Board &b) const
{
	if (xDst == _xPos && _yPos == yDst)//can move to himself....
		return COORDINATE_EQUALS_CURRENT;
	if (b[xDst][yDst] != nullptr && b[xDst][yDst]->get_is_white() == _isWhite)
		return ALREDY_PIECE_IN_DST;	// if same color
	if (_xPos == xDst && _yPos != yDst) //go straight like rook  either up/ down
	{
		for (int i = _yPos < yDst ? _yPos + 1 : _yPos - 1; _yPos < yDst ? i < yDst : i > yDst; _yPos < yDst ? i++ : i--)
			if (b[_xPos][i] != nullptr)//something block the way...
				return INVALID_MOVEMENT;
		return GOOD_MOVE;
	}
	else if (_xPos != xDst && _yPos == yDst)//go straight like rook  either left/ rights
	{
		for (int i = _xPos < xDst ? _xPos + 1 : _xPos - 1; _xPos < xDst ? i < xDst : i > xDst; _xPos < xDst ? i++ : i--)
			if (b[i][_yPos] != nullptr)//something block the way...
				return INVALID_MOVEMENT;
		return GOOD_MOVE;
	}
	else
	{
		int x = 0, y = 0, xdir = 0, ydir = 0;
		if (_xPos == xDst && _yPos == yDst)//can move to itself
			return COORDINATE_EQUALS_CURRENT;
		if (b[_xPos][_yPos] == nullptr)//no piece found in src
			return NO_PIECE_SELECTED;
		if (b[xDst][yDst] != nullptr && b[xDst][yDst]->get_is_white() == _isWhite)//already ally in the dst
			return ALREDY_PIECE_IN_DST;

		x = xDst - _xPos;
		y = yDst - _yPos;

		xdir = x < 0 ? -1 : 1;
		ydir = y < 0 ? -1 : 1;

		x *= xdir;
		y *= ydir;

		if (x != y)//checking if the queen go vertically by checking if the of both abs(difference) are equals else mean it not verticall
		{
			return INVALID_MOVEMENT;
		}
		for (int i = _xPos + xdir, j = _yPos + ydir; i != xDst; i += xdir, j += ydir)//bishop(vertically) either downLeft/upLeft/downRight/upRight
		{
			if (b[i][j] != NULL)
			{
				return INVALID_MOVEMENT;
			}
		}
		return GOOD_MOVE;
	}
}
char Queen::to_string() const
{
	return _isWhite ? 'Q' : 'q';
}
