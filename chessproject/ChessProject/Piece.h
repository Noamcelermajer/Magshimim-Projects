#pragma once
#include "Board.h"
#include "code.h"
#include <string>


class Board;
/*
* This is Our abstract class that all pieces will inherite from he
* _isWhite = Is the Piece black or white (color)
* _xPos and _yPos are the (x, y) position in our 2d array
*/
class Piece
{
protected:
	bool _isWhite;
	int _xPos;
	int _yPos;

public:
	Piece(bool isWhite, unsigned xPos, unsigned yPos);
	virtual ~Piece();
	virtual Piece* clone() const = 0;
	//getters..
	virtual int get_y() const;
	virtual int get_x() const;
	virtual bool get_is_white() const;
	//setters...
	virtual void set_x(unsigned x);
	virtual void set_y(unsigned y);
	virtual void set_first_turn();
	
	virtual int is_valid_move(int xDst, int yDst, const Board &b) const = 0;//This function check if the move is valid for each piece (is abstract definition and every child classe will be define it for is purpose(because each Piece as is move))
	virtual char to_string() const = 0;//return char (like all the classe inplemented toString in java c#..)

};
