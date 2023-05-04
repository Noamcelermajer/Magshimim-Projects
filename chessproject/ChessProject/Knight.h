#pragma once
#include "Piece.h"


class Knight : public Piece
{
public:
	Knight(bool isWhite, int xPos, int yPos);
	~Knight();
	virtual Piece* clone() const;

	virtual int is_valid_move(int xDst, int yDst, const Board &b) const;
	virtual char to_string() const;
};

