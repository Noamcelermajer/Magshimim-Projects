#include "Pawn.h"



Pawn::Pawn(bool isWhite, int xPos, int yPos) : Piece(isWhite, xPos, yPos)
{
	_firstTurn = true;
}
Pawn::~Pawn()
{
}
Pawn::Pawn(const Pawn& r) : Piece(r._isWhite, r._xPos, r._yPos)
{
	_firstTurn = r._firstTurn;
}
Pawn* Pawn::clone() const
{
	return new Pawn(*this);
}
/*This function is checking if the dst point if a valid move for our pawn by doing multyple check on him*/
int Pawn::is_valid_move(int xDst, int yDst, const Board &b) const
{
	if (abs(xDst - _xPos) == 1 && abs(yDst - _yPos) == 0)//can go only 1 case away or 1 case away and 1 case lifted if he want to eat or 2 case away if first turn
	{
		if (_isWhite ? (xDst > _xPos) : (xDst < _xPos))//this piece is really difficult because we need to calculate diferently if it black or white Piece because black piece are upside down...
			return INVALID_MOVEMENT;
		else
			if (b[xDst][yDst] == nullptr)//mean that  everything gucci
				return GOOD_MOVE;
	}
	else if (abs(xDst - _xPos) == 2 && abs(yDst - _yPos) == 0)//away by 2 only if it first time
	{
		if (!_firstTurn)//mean it not the first time...cheaters....
			return INVALID_MOVEMENT;
		if (_isWhite ? (xDst > _xPos) : (xDst < _xPos))//checking is not to far away or lifted 
			return INVALID_MOVEMENT;
		else
			if (b[_isWhite ? (_xPos - 1) : (_xPos + 1)][yDst] == nullptr && b[xDst][yDst] == nullptr)//good
				return GOOD_MOVE;
	}
	else if (abs(xDst - _xPos) == 1 && abs(yDst - _yPos) == 1)//if he is eating someone
	{
		if (_isWhite ? (xDst > _xPos) : (xDst < _xPos))//check that is not going behind the Pawn restriction...
			return INVALID_MOVEMENT;
		else
			if (b[xDst][yDst] == nullptr)//can eat empty GO BACK!!!!
				return INVALID_MOVEMENT;
			else if (b[xDst][yDst]->get_is_white() == _isWhite)//CAN EAT ALLIES!!!!
				return ALREDY_PIECE_IN_DST;
			else
				return GOOD_MOVE;
	}
	return INVALID_MOVEMENT;
}
void Pawn::set_first_turn()
{
	_firstTurn = false;
}
char Pawn::to_string() const
{
	return _isWhite ? 'P' : 'p';
}
