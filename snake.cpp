#include <iostream>
#include <ctime>
#include <cstdlib>
#include <ncurses.h>
#include <string>
#include <unistd.h>
#include "game.hpp"
using std::cout; using std::cin; using std::endl;

const int base_speed = 50000;
int x0_val = 0, y0_val = 0;

void fine(WINDOW*, int, int);
void runGame(WINDOW*);
int set_speed_input(int);
void wreset(WINDOW*);

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
	WINDOW * win = newwin(2, 50, 0, 0);
	keypad(win, TRUE);  /* abilita la mappatura della tastiera */ 
	runGame(win);
	return 0;
}

void runGame(WINDOW * win){
	int board_d, w_height, w_width, speed_val=1;
	char str[5], speed[5];

	wreset(win);
	mvwin(win, 0, 0);
	wresize(win, 2, 50);
	(void) echo(); //add input echo	
	waddstr(win, "WELCOME TO SHITTY SNAKE!\n");
	waddstr(win, "Choose the dimension of the board: ");
	wgetstr(win, str);
	board_d = std::atoi(str);
	board_d = (board_d<15? 15:board_d);
	wreset(win);
	waddstr(win, "Choose the speed (1=fastest, 2=slower, etc.): ");
	wgetstr(win, speed);
	speed_val = std::atoi(speed);
	wreset(win);

	w_height = board_d+3; //3 rows more to print Score and losing message
	w_width = 2*board_d; //twice because I print ". " as a board cell
	mvwin(win,(y0_val-w_height)/2,(x0_val-w_width)/2);
	wresize(win, w_height, w_width);
	wreset(win);

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
	
	usleep(base_speed);
	fine(win, 0, game.getScore());
}

int set_speed_input(int speed_val){
	return (speed_val==KEY_ENTER?base_speed:speed_val*base_speed);
}

void fine(WINDOW * win, int sig, int score){
	char out;
	std::string output = "You lost! Final Score: "+std::to_string(score)+"\n";

	nodelay(win, false); //wait input
	wreset(win);
	waddstr(win, output.c_str());
	waddstr(win, "[R] Retry [Q] Quit");
	wrefresh(win);
	out = wgetch(win);
	if(out == 'R' || out == 'r') runGame(win);
	else if (out == 'Q' || out=='q') endwin();
	else fine(win, sig, score);
}

void wreset(WINDOW * win){
	wclear(win);
	wmove(win, 0, 0);
	wrefresh(win);
}
