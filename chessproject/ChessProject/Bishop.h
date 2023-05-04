#pragma once
#include "Piece.h"
class Bishop :
	public Piece
{ 
public:
	Bishop(bool isWhite, int xPos, int yPos);
	~Bishop();

	virtual Bishop* clone() const;

	virtual int is_valid_move(int xDst, int yDst, const Board &b) const;
	virtual char to_string() const;
};

