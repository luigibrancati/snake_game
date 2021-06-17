#include <iostream>
#include <ctime>
#include <cstdlib>
#include <ncurses.h>
#include <unistd.h>
#include "board.hpp"
#include "food.hpp"
#include "snake.hpp"
using std::cout; using std::cin; using std::endl;

const int base_speed = 50000;

void printBoard(WINDOW*, const Board&, const Food&, const Snake&, int);	
bool checkLose(const Board&, const Snake&);
short convertMove(WINDOW*);
static void fine(int, int);
void runGame();

int main(){
	std::srand(std::time(NULL));
	
	// ncurses configuration
	(void) initscr();      /* inizializza la libreria curses */ 
	keypad(stdscr, TRUE);  /* abilita la mappatura della tastiera */ 
	(void) nonl();         /* non convertire NL->CR/NL in output */ 
	(void) cbreak();       /* prende i caratteri in input uno alla volta, senza attendere il \n */ 
	(void) noecho();       /* nessuna echo dell'input */ 	
	
	runGame();
	return 0;
}

void runGame(){
	WINDOW * win = newwin(2,50,0,0);
	int d, move, speed_val=1, score = 0;
	char str[3], speed[3];
	
	waddstr(win, "WELCOME TO SHITTY SNAKE!\n");
	waddstr(win, "Choose the dimension of the board: ");
	wgetstr(win, str);
	d = std::atoi(str);
	wclear(win);
	wrefresh(win);
	waddstr(win, "Choose the speed (1=fastest, 2=slower, etc.): ");
	wgetstr(win, speed);
	speed_val = std::atoi(speed);
	wclear(win);
	wrefresh(win);

	win = newwin(d+3,2*d,0,0);
	Board board(d);
	Food food(std::rand()%d,std::rand()%d);
	Snake snake(d/2, d/2);
	bool gameState = true;
	
	printBoard(win,board,food,snake,score);
	wrefresh(win);
	snake.move(convertMove(win));
	nodelay(win, true);

	while(gameState){
		snake.move(convertMove(win));
		printBoard(win, board, food, snake, score);
		wrefresh(win);
		gameState = !checkLose(board,snake);
		if(food.getX() == snake.getX()[0] && food.getY() == snake.getY()[0]){
			snake.eat();
			food.replace(std::rand()%(d-2)+1,std::rand()%(d-2)+1);
			score++;
		}
		usleep(speed_val*base_speed);
	}

	nodelay(win, false);
	printBoard(win,board,food,snake,score);
	waddstr(win, "You lost!");
	wrefresh(win);
	wgetch(win);
	fine(0, score);
}

void printBoard(WINDOW * win, const Board & board, const Food & food, const Snake & snake, int score){
	wmove(win,0,0);
	waddstr(win, "Score: ");
	waddstr(win, std::to_string(score).c_str());
	waddstr(win, " (w=UP,a=LEFT,s=DOWN,d=RIGHT)");
	waddch(win, '\n');
	for(short h=0;h<board.getDim();h++){
		for(short w=0;w<board.getDim();w++){
			if(snake.on(w,h)){
				waddch(win,snake.getS(w,h));
			}
			else if(w==food.getX() && h==food.getY()){
				waddch(win, food.getS());
			}
			else{
				waddch(win, board.getS());
			}	
			waddch(win,' ');
		}
	}
}

bool checkLose(const Board& board, const Snake& snake){
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

short convertMove(WINDOW * win){
	int m = wgetch(win);
	if(m==((int)'w')) return 0;
	else if(m==((int)'a')) return 1;
	else if(m==((int)'s')) return 2;
	else if(m==((int)'d')) return 3;
	return -1;
}

static void fine(int sig, int score){
	endwin();
	cout<<"You lost! Final score: "<<score<<endl;
}
