#pragma once
#include "Piece.h"
class Queen :
	public Piece
{
public:
	Queen(bool isWhite,int xPos, int yPos);
	Queen(const Queen& q);
	~Queen();

	virtual Queen* clone() const;
	virtual int is_valid_move(int xDst, int yDst, const Board &b) const;
	virtual char to_string() const;
};
