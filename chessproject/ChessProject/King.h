#pragma once
#include "Piece.h"
class King : public Piece
{
	bool is_checked_by_king(int xPos, int yPos, const Board& b) const;


public:
	King(bool isWhite, unsigned xPos, unsigned yPos);
	King(const King& k);
	~King();
	virtual King* clone() const;
	bool is_checked(int xPos, int yPos, const Board& b) const;
	virtual int is_valid_move(int xDst, int yDst, const Board &b) const;
	virtual char to_string() const;
};

