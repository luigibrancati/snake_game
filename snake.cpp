#include <iostream>
#include <ctime>
#include <cstdlib>
#include <ncurses.h>
#include <unistd.h>
#include "game.hpp"
using std::cout; using std::cin; using std::endl;

const int base_speed = 50000;
int x0_val, y0_val;

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
	
	
	//take the maximum size of main window, used later in runGame
	getmaxyx(stdscr, y0_val,x0_val);
	cout<<y0_val<<x0_val<<endl;
	//run game
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

	w_height = board_d+3; //3 rows more to print Score and losing message
	w_width = 2*board_d; //twice because I print ". " as a board cell	
	win = newwin(w_height,w_width,(y0_val-w_height)/2,(x0_val-w_width)/2);
	keypad(win, TRUE);  /* abilita la mappatura della tastiera */ 

	SnakeGame game(board_d);
	
	//first print	
	(void) noecho();       /* nessuna echo dell'input */ 	
	game.printBoard(win);
	wrefresh(win);
	game.move(win);
	nodelay(win, true);//don't wait for input after this

	while(game.getRun()){
		game.move(win);
		game.printBoard(win);
		wrefresh(win);
		usleep(set_speed_input(speed_val));
	}
	
	nodelay(win, false); //wait input
	usleep(base_speed);
	game.printBoard(win);
	waddstr(win, "You lost!");
	wrefresh(win);
	wgetch(win);
	fine(0, score);
}

int set_speed_input(int speed_val){
	return (speed_val==KEY_ENTER?base_speed:speed_val*base_speed);
}

static void fine(int sig, int score){
	endwin();
	cout<<"You lost! Final score: "<<score<<endl;
}
