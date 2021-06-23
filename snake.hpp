#ifndef _SNAKE_H_
#define _SNAKE_H_

#include <iostream>
#include <vector>
#include <algorithm>
#include "food.hpp"

short abs(short a){
	return (a>0? a:-a);
}

class Snake {
	std::vector<short> x;
	std::vector<short> y;
	char symbols[2];
	enum Direction {UP,LEFT,DOWN,RIGHT};
	Direction curr_dir;

	public:
	
	Snake(short x, short y, char s[2]):
		x({x,++x}),
		y({y,y}),
		curr_dir(LEFT)
	{
		symbols[0]=s[0];
		symbols[1]=s[1];
	}

	Snake(short x, short y):
		x({x,++x}),
		y({y,y}),
		curr_dir(LEFT)
	{
		symbols[0]='>';
		symbols[1]='O';
	}

	std::vector<short> getX() const {return this->x;}
	std::vector<short> getY() const {return this->y;}
	char getS(short s) const {return this->symbols[s];}
	short len() const {return this->x.size();}

	void eat(){
		short tx = this->x[this->x.size()-1];
		short ty = this->y[this->y.size()-1];
		switch(this->curr_dir){
			case UP:
				this->x.push_back(tx);
				this->y.push_back(ty+1);
				break;
			case LEFT:
				this->x.push_back(tx+1);
				this->y.push_back(ty);
				break;
				break;
			case DOWN:
				this->x.push_back(tx);
				this->y.push_back(ty-1);
				break;
				break;
			case RIGHT:
				this->x.push_back(tx-1);
				this->y.push_back(ty);
				break;
				break;
		}
	}
	
	bool move_allowed(short dir){
		return ((dir!=-1) && (abs(((short) this->curr_dir)-dir)!=2));
	}

	void move(short dir=-1){
		for(short i=(this->x.size()-1);i>0;i--){
			this->x[i]=this->x[i-1];
			this->y[i]=this->y[i-1];
		}
		
		this->curr_dir = (this->move_allowed(dir)? Direction(dir):this->curr_dir);

		switch(this->curr_dir){
			case UP:
				this->symbols[0]='v';
				this->y[0]-=1;
				break;
			case LEFT:
				this->symbols[0]='>';
				this->x[0]-=1;
				break;
			case DOWN:
				this->symbols[0]='^';
				this->y[0]+=1;
				break;
			case RIGHT:
				this->symbols[0]='<';
				this->x[0]+=1;
				break;
		}
	}
};

#endif
