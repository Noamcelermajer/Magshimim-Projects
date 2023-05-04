#pragma once
#include "Piece.h"
class Pawn :
	public Piece
{
	bool _firstTurn;
public:
	Pawn(bool isWhite,int xPos, int yPos);
	~Pawn();
	Pawn(const Pawn& r);
	virtual Pawn* clone() const;
	virtual int is_valid_move(int xDst, int yDst, const Board &b) const;
	virtual void set_first_turn();
	virtual char to_string() const;
};

