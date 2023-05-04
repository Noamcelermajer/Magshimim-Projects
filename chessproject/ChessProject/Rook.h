#pragma once
#include "Piece.h"


class Rook :
	public Piece
{
public:
	Rook(bool isWhite, int xPos, int yPos);
	Rook(const Rook& r);
	~Rook();

	virtual Rook* clone() const;

	virtual int is_valid_move(int xDst, int yDst, const Board &b) const;
	virtual char to_string() const;
};
