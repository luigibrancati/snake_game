#include <iostream>
#include <ctime>
#include <cstdlib>
#include "board.hpp"
#include "food.hpp"
#include "snake.hpp"
using std::cout; using std::cin; using std::endl;

void printInfo(const Snake& snake){
	cout<<"Snake pos:";
	for(int i=0;i<snake.len();i++){
		cout<<"("<<snake.getX()[i]<<","<<snake.getY()[i]<<") ";
	}
	cout<<endl;
	cout<<"Length: "<<snake.len()<<endl;
}

void printBoard(const Board & board, const Food & food, const Snake & snake){
	//printInfo(snake);
	for(short h=0;h<board.getDim();h++){
			cout<<'\t';
			for(short w=0;w<board.getDim();w++){
				if(snake.on(w,h)){
					cout<<snake.getS(w,h)<<" ";
				}
				else if(w==food.getX() && h==food.getY()){
					cout<<food.getS()<<" ";
				}
				else{
					cout<<board.getS()<<" ";
				}	
			}
			cout<<endl;
		}
}

bool checkLose(Board& board, Snake& snake){
	bool lost_board = (snake.getX()[0] < 0 || snake.getY()[0] < 0 || //
			snake.getX()[0] == board.getDim() || snake.getY()[0] == board.getDim());
	
	bool lost_snake = false;
	for(int i=1;i<snake.len();i++){
		if(snake.getX()[0]==snake.getX()[i] && snake.getY()[0]==snake.getY()[i]){
			lost_snake = true;
		}
	}
	return lost_board || lost_snake;
}

short convertMove(){
	char m;
	cin >> m;
	if(m=='w') return 0;
	else if(m=='a') return 1;
	else if(m=='s') return 2;
	else if(m=='d') return 3;
	return -1;
}

void runGame(){
	short d, move;
	cout << "Dimension: ";
	cin >> d;

	Board board(d);
	Food food(std::rand()%(d-1)+1,std::rand()%(d-1)+1);
	Snake snake(d/2, d/2);
	bool gameState = true;
	printBoard(board,food,snake);

	while(gameState){
		snake.move(convertMove());
		gameState = !checkLose(board,snake);
		if(food.getX() == snake.getX()[0] && food.getY() == snake.getY()[0]){
			snake.eat();
			food.replace(std::rand()%(d-1)+1,std::rand()%(d-1)+1);
		}
		printBoard(board, food, snake);	
	}

	cout << "You lost!" << endl;
}

int main(){
	std::srand(std::time(NULL));
	runGame();
	return 0;
}
