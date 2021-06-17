#ifndef _BOARD_H_
#define _BOARD_H_

#include<iostream>
#include<cstdlib>
#include "food.hpp"
using std::cout; using std::endl;

class Board {
	short dimension;
	char symbol;
	public:
	Board(short d, char s='.'):
		dimension(d),
		symbol(s)
	{}

	short getDim() const {return this->dimension;};
	char getS() const {return this->symbol;};
};

#endif
