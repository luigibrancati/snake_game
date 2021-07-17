#ifndef _SNAKEGAME_H_
#define _SNAKEGAME_H_
#include <cstdlib>
#include <SFML/Graphics.hpp>
#include "snake.hpp"
#include "food.hpp"
using std::rand;

class SnakeGame {
		short board_dimension;
		char board_symbol;
		bool gameRun;
		std::vector<std::vector<char>> gameState;
		int score;
		Snake snake;
		Food food;
		//previous food and snake positions
		//needed for faster update of the board
		short food_prev_x, food_prev_y;
		short snake_prev_hx, snake_prev_hy;
		short snake_prev_tx, snake_prev_ty;
	
	public:
		SnakeGame(short bd):
			board_dimension(bd),
			board_symbol('.'),
			snake(bd/2, bd/2),
			food(rand()%(this->board_dimension),rand()%(this->board_dimension)),
			gameRun(true),
			score(0),
			gameState(bd, std::vector<char>(bd,'.')),
			snake_prev_hx(this->snake.getHX()),
			snake_prev_hy(this->snake.getHY()),
			snake_prev_tx(this->snake.getTX()),
			snake_prev_ty(this->snake.getTY()),
			food_prev_x(this->food.getX()),
			food_prev_y(this->food.getY())
		{
			this->makeState();
		}

		bool getRun() const {return this->gameRun;}
		int  getScore() const {return this->score;}

		std::string getBoard() const {
			std::string t = "Score: " + std::to_string(this->score);
			t = t+" (controls: WASD/Arrow keys, exit:CTRL+Z/C)\n";

			for(short h=0;h<this->board_dimension;h++){
				for(short w=0;w<this->board_dimension;w++){
					t=t+this->gameState[h][w]+" ";
				}
				t=t+"\n";
			}
			t=t+"\n";
			return t;
		}
		
		//fully redraw the board
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

		//only updates food and snake head and tail positions
		//as opposite to makeState which also redraws the void spaces
		void updateState(){
			this->gameState[this->food_prev_y][this->food_prev_x] = this->board_symbol;
			this->gameState[this->food.getY()][this->food.getX()] = this->food.getS();

			this->gameState[this->snake_prev_hy][this->snake_prev_hx] = this->snake.getS(1);
			if(!this->snakeCheckEat()) this->gameState[this->snake_prev_ty][this->snake_prev_tx] = this->board_symbol;
			if(this->gameRun){
				this->gameState[this->snake.getHY()][this->snake.getHX()] = this->snake.getS(0);
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
			food.replace(std::rand()%(this->board_dimension), std::rand()%this->board_dimension);
		}

		bool snakeCheckEat(){
			if(this->gameRun && 
				this->food.getX() == this->snake.getX()[0] && 
				this->food.getY() == this->snake.getY()[0])
			{
				this->snake.eat();
				this->food_prev_x = food.getX();
				this->food_prev_y = food.getY();
				//if snake eats, the tail moves backward -> update
				this->snake_prev_tx = snake.getTX()-1;
				this->snake_prev_ty = snake.getTY()-1;
				this->updateFood();
				this->score++;
				return true;
			}
			return false;
		}

		void move(sf::Event& event){
			this->snake_prev_hx = snake.getHX();
			this->snake_prev_hy = snake.getHY();
			this->snake_prev_tx = snake.getTX();
			this->snake_prev_ty = snake.getTY();
			if(event.key.code==sf::Keyboard::W || event.key.code==sf::Keyboard::Up) this->snake.move(0);
			else if(event.key.code==sf::Keyboard::A || event.key.code==sf::Keyboard::Left) this->snake.move(1);
			else if(event.key.code==sf::Keyboard::S || event.key.code==sf::Keyboard::Down) this->snake.move(2);
			else if(event.key.code==sf::Keyboard::D || event.key.code==sf::Keyboard::Right) this->snake.move(3);
			else snake.move();
			this->checkLose();
			this->updateState();	
		}
};

#endif
