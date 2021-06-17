#include <iostream>
#include <ncurses.h>
#include "board.hpp"
#include "snake.hpp"
#include "food.hpp"
using namespace std;

int main(){
	int d = 30;
	Board board(d);
	// ncurses configuration
	(void) initscr();      /* inizializza la libreria curses */ 
	keypad(stdscr, TRUE);  /* abilita la mappatura della tastiera */ 
	(void) nonl();         /* non convertire NL->CR/NL in output */ 
	(void) cbreak();       /* prende i caratteri in input uno alla volta, senza attendere il \n */ 
	(void) noecho();       /* nessuna echo dell'input */ 	

	WINDOW * win = newwin(d,2*d, 5,5);
	for(int i=0;i<board.getDim();i++){
		for(int j=0;j<board.getDim();j++){
			waddch(win, board.getS());	
			waddch(win, ' ');	
		}	
	}

	wmove(win, 20, 20);
	waddstr(win, "TEST");
	
	wgetch(win);
	endwin();

	return 0;
}
