#include "Knight.h"



Knight::Knight(bool isWhite, int xPos, int yPos) : Piece(isWhite, xPos, yPos)
{
}
Knight::~Knight()
{
}
//used in copy-ctor
Piece* Knight::clone() const
{
	return new Knight(*this);
}
/*This func if the knight move is valid by checking if it uin range the knight can go only in 'L'*/
int Knight::is_valid_move(int xDst, int yDst, const Board &b) const
{
	if ((abs(_xPos - xDst) == 2 && abs(_yPos - yDst) == 1) || (abs(_xPos - xDst) == 1 && abs(_yPos - yDst) == 2))//check if we are on range
	{
		if (b[xDst][yDst] != nullptr)
		{
			if (b[xDst][yDst]->get_is_white() == _isWhite)//check if there no allies in the dst point
				return ALREDY_PIECE_IN_DST;
		}
		return GOOD_MOVE;//pass all test congratulation
	}
	else
		return INVALID_MOVEMENT;
}

char Knight::to_string() const
{
	return _isWhite ? 'N' : 'n';
}
