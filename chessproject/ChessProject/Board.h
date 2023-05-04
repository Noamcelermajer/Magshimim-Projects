#pragma once
#include "Piece.h"
#include <array>
#define SIZE 8


class Piece;
class King;
class Board
{
	King* _whiteKing;//the same White king that is in our std::array<std::array<Piece*, SIZE>, SIZE> _pieces
	//it is usefull for checking chessmate
	King* _blackKing;
	std::array<std::array<Piece*, SIZE>, SIZE> _pieces;
	
	void init(std::string boardStr);//constructor
	void move_piece(int xSrc, int ySrc, int xDst, int yDst);
	bool is_checkmate(bool) const;//checking is chessmate occur
public:
	Board(std::string boardStr);
	Board(const Board& b);
	~Board();
	//getters
	std::array<std::array<Piece*, SIZE>, SIZE> get_pieces() const;
	//setters
	void set_piece(int xPos, int yPos, Piece* p);
	//utility func..
	int move(int xSrc, int ySrc, int xDst, int yDst, bool turn);
	std::array<Piece*, SIZE> operator[](int i) const;
	std::string to_string() const;
};
