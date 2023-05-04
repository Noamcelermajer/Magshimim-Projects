#include "King.h"

#define MAX_RANGE 1
 
King::King(bool isWhite, unsigned xPos, unsigned yPos) : Piece(isWhite, xPos, yPos)
{
}
King::King(const King& k) : Piece(k._isWhite, k._xPos, k._yPos)
{

}
King::~King()
{
}
/*used for copy-ctor*/
King* King::clone() const
{
	return new King(*this);
}

/*
Checks if king is in check.
The function gets new positions to check if king will be checked in new position
*/
bool King::is_checked(int xPos, int yPos, const Board& b) const
{
	int returnCode = 0;
	if (is_checked_by_king(xPos, yPos, b))
	{
		return true;
	}
	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			if (b[i][j] != nullptr)
				if (b[i][j]->get_is_white() != _isWhite)
				{
					returnCode = b[i][j]->is_valid_move(xPos, yPos, b);
					if (returnCode == GOOD_MOVE)
						// check if enemy can move to kings position (eat him)
						return true;
				}
		}
	}
	return false;
}
/*THis function check the case that the king checked by other king*/
bool King::is_checked_by_king(int xPos, int yPos, const Board& b) const
{
	for (int i = xPos - 1; i <= xPos + 1; i++)
		for (int j = yPos - 1; j <= yPos + 1; j++)
			if ((i == _xPos && j == _yPos) || (i<0 || j<0 || i>SIZE - 1 || j>SIZE - 1))
				continue;
			else if (b[i][j] != nullptr)
				if (b[i][j]->to_string() == (_isWhite ? 'k' : 'K'))
					return true;
	return false;
}
/*Check if king move is valid by checking if it inn range of 1 from src cuz the king can move far from dis*/
int King::is_valid_move(int xDst, int yDst, const Board &b) const
{
	if ((abs(xDst - _xPos) == MAX_RANGE && abs(yDst - _yPos) == MAX_RANGE) ||
		(abs(xDst - _xPos) == MAX_RANGE && abs(yDst - _yPos) == 0) ||
		(abs(xDst - _xPos) == 0 && abs(yDst - _yPos) == 1)//out of bouds;
		)
	{
		if (b[xDst][yDst] == nullptr || b[xDst][yDst]->get_is_white() != _isWhite)
		{
			Board toCheck(b);
			delete toCheck.get_pieces()[_xPos][_yPos];
			toCheck.set_piece(_xPos, _yPos, nullptr);
			if (toCheck[xDst][yDst] != nullptr)
				delete toCheck[xDst][yDst];
			toCheck.set_piece(xDst, yDst, (*this).clone());
			// we need to check board as if king is in new position
			if (!is_checked(xDst, yDst, toCheck))
				return GOOD_MOVE;//good move
			else
				return MOVEMENT_MAKE_CHECK;//made a check
		}
		else
			return ALREDY_PIECE_IN_DST;//mean that the dst piece in taken by allies
	}
	return INVALID_MOVEMENT;
}
/*ToString like the classes inplemented in java and c#*/
char King::to_string() const
{
	return _isWhite ? 'K' : 'k';
}
