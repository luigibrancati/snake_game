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
int x0_val, y0_val;

void printBoard(WINDOW*, const Board&, const Food&, const Snake&, int, bool);	
bool checkLose(const Board&, const Snake&);
short convertMove(WINDOW*);
static void fine(int, int);
void runGame();
int set_speed_input(int);

int main(){
	std::srand(std::time(NULL));
	
	// ncurses configuration
	(void) initscr();      /* inizializza la libreria curses */ 
	keypad(stdscr, TRUE);  /* abilita la mappatura della tastiera */ 
	(void) nonl();         /* non convertire NL->CR/NL in output */ 
	(void) cbreak();       /* prende i caratteri in input uno alla volta, senza attendere il \n */ 
	(void) noecho();       /* nessuna echo dell'input */ 	
	

	getmaxyx(stdscr, y0_val,x0_val);
	cout<<y0_val<<x0_val<<endl;
	runGame();
	return 0;
}

void runGame(){
	WINDOW * win = newwin(2,50,0,0);
	int board_d, w_height, w_width, move, speed_val=1, score = 0;
	char str[5], speed[5];
	bool gameState = true;
	
	waddstr(win, "WELCOME TO SHITTY SNAKE!\n");
	waddstr(win, "Choose the dimension of the board: ");
	wgetstr(win, str);
	board_d = std::atoi(str);
	board_d = (board_d<15? 15:board_d);
	wclear(win);
	wrefresh(win);

	waddstr(win, "Choose the speed (1=fastest, 2=slower, etc.): ");
	wgetstr(win, speed);
	speed_val = std::atoi(speed);
	wclear(win);
	wrefresh(win);

	w_height = board_d+3; //3 rows more to print Score and loosing message
	w_width = 2*board_d; //twice because i print ". " as a board cell	
	win = newwin(w_height,w_width,(y0_val-w_height)/2,(x0_val-w_width)/2);
	keypad(win, TRUE);  /* abilita la mappatura della tastiera */ 

	Board board(board_d);
	Food food(std::rand()%board_d,std::rand()%board_d);
	Snake snake(board_d/2, board_d/2);
	
	//first print	
	printBoard(win,board,food,snake,score, gameState);
	wrefresh(win);
	snake.move(convertMove(win));
	nodelay(win, true);//don't wait for input after this

	while(gameState){
		snake.move(convertMove(win));
		printBoard(win, board, food, snake, score, gameState);
		wrefresh(win);
		gameState = !checkLose(board,snake);
		if(food.getX() == snake.getX()[0] && food.getY() == snake.getY()[0]){
			snake.eat();
			food.replace(std::rand()%board_d,std::rand()%board_d);
			score++;
		}
		usleep(set_speed_input(speed_val));
	}
	
	nodelay(win, false); //wait input
	usleep(base_speed);
	printBoard(win,board,food,snake,score, gameState);
	waddstr(win, "You lost!");
	wrefresh(win);
	wgetch(win);
	fine(0, score);
}

int set_speed_input(int speed_val){
	return (speed_val==KEY_ENTER?base_speed:speed_val*base_speed);
}

void printBoard(WINDOW* win, const Board& board, const Food& food, const Snake& snake, int score, bool print_head){
	int wx=0, wy=0;
	wmove(win,0,0);
	waddstr(win, "Score: ");
	waddstr(win, std::to_string(score).c_str());
	waddstr(win, " (controls: WASD/Arrow keys, exit:CTRL+Z/C)");
	waddch(win, '\n');

	for(short h=0;h<board.getDim();h++){
		for(short w=0;w<board.getDim();w++){
			waddch(win, board.getS());
			waddch(win, ' ');
		}
	}
	getyx(win, wy, wx);
	
	//print food	
	wmove(win, food.getY(), 2*food.getX());
	waddch(win, food.getS());

	//print snake
	if(print_head){
		wmove(win, snake.getY()[0], 2*snake.getX()[0]);
		waddch(win, snake.getS(0));
	}
	for(int i=1;i<snake.len();i++){
		wmove(win, snake.getY()[i], 2*snake.getX()[i]);
		waddch(win, snake.getS(1));
	}
	wmove(win, wy, wx);
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
	if(m==((int)'w') || m==KEY_UP) return 0;
	else if(m==((int)'a') || m==KEY_LEFT) return 1;
	else if(m==((int)'s') || m==KEY_DOWN) return 2;
	else if(m==((int)'d') || m==KEY_RIGHT) return 3;
	return -1;
}

static void fine(int sig, int score){
	endwin();
	cout<<"You lost! Final score: "<<score<<endl;
}
