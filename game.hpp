#ifndef _SNAKEGAME_H_
#define _SNAKEGAME_H_
#include <cstdlib>
#include <ncurses.h>
#include "snake.hpp"
#include "food.hpp"
using std::rand;

class SnakeGame {
		short board_dimension;
		char board_symbol;
		bool gameRun;
		char ** gameState;
		int score;
		Snake snake;
		Food food;
	
	public:
		SnakeGame(short bd):
			board_dimension(bd),
			board_symbol('.'),
			snake(bd/2, bd/2),
			food(rand()%(this->board_dimension),rand()%(this->board_dimension)),
			gameRun(true),
			score(0)
		{
			gameState = new char*[bd];
			for(int i=0;i<bd;i++) gameState[i] = new char[bd];
			this->makeState();
		}

		bool getRun() const {return this->gameRun;}

		void printBoard(WINDOW* win) const {
			wmove(win,0,0);
			waddstr(win, "Score: ");
			waddstr(win, std::to_string(this->score).c_str());
			waddstr(win, " (controls: WASD/Arrow keys, exit:CTRL+Z/C)");
			waddch(win, '\n');

			for(short h=0;h<this->board_dimension;h++){
				for(short w=0;w<this->board_dimension;w++){
					waddch(win, this->gameState[h][w]);
					waddch(win, ' ');
				}
			}
		}
		
		void makeState(){
			for(int i=0;i<this->board_dimension;i++){
				for(int j=0;j<this->board_dimension;j++){
					this->gameState[i][j] = this->board_symbol;
				}
			}
			
			this->gameState[this->food.getY()][this->food.getX()] = this->food.getS();
			
			if(this->gameRun){
				this->gameState[this->snake.getY()[0]][this->snake.getX()[0]] = this->snake.getS(0);
			}
			for(int i=1;i<this->snake.len();i++){
				this->gameState[this->snake.getY()[i]][this->snake.getX()[i]] = this->snake.getS(1);
			}
		}

		void checkLose(){
			bool lost_board = (this->snake.getX()[0] < 0 || this->snake.getY()[0] < 0 || //
					this->snake.getX()[0] == this->board_dimension || 
					this->snake.getY()[0] == this->board_dimension);
			
			bool lost_snake = false;
			for(int i=1;i<this->snake.len();i++){
				if(this->snake.getX()[0]==this->snake.getX()[i] && 
						this->snake.getY()[0]==this->snake.getY()[i]){
					lost_snake = true;
				}
			}

			//Is false if one of the losing condition is true
			this->gameRun = !(lost_board || lost_snake);
		}

		void updateFood(){
			food.replace(std::rand()%(this->board_dimension),
					std::rand()%this->board_dimension);
		}

		void snakeCheckEat(){
			if(this->food.getX() == this->snake.getX()[0] && 
					this->food.getY() == this->snake.getY()[0]){
				this->snake.eat();
				this->updateFood();
				this->score++;
			}
		}

		void move(WINDOW * win){
			int m = wgetch(win);
			if(m==((int)'w') || m==KEY_UP) this->snake.move(0);
			else if(m==((int)'a') || m==KEY_LEFT) this->snake.move(1);
			else if(m==((int)'s') || m==KEY_DOWN) this->snake.move(2);
			else if(m==((int)'d') || m==KEY_RIGHT) this->snake.move(3);
			else snake.move();
			
			this->checkLose();
			this->snakeCheckEat();
			this->makeState();	
		}
};

#endif
