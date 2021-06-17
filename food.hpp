#ifndef _FOOD_H_
#define _FOOD_H_

#include <iostream>

class Food {
	short pos_x;
	short pos_y;
	char symbol;
	public:
	Food(short x, short y, char s='x'):
		pos_x(x),
		pos_y(y),
		symbol(s)
	{}

	short getX() const {return this->pos_x;}
	short getY() const {return this->pos_y;}
	char getS() const {return this->symbol;}

	void replace(short x, short y){
		this->pos_x=x;
		this->pos_y=y;
	}
};

#endif
