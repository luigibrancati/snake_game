#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <sstream>
#include <unistd.h>
#include <SFML/Graphics.hpp>
#include "game.hpp"
using std::cout; using std::cin; using std::endl; using std::string;

const int base_speed = 50000;

void fine(sf::RenderWindow&, sf::Text&, int);
void runGame(sf::RenderWindow&, sf::Text&);
int set_speed_input(int);
void wreset(sf::RenderWindow&);
string getWindowInput(sf::RenderWindow&, sf::Text&); 

int main(){
	std::srand(std::time(NULL));
	sf::Font font;
	if (!font.loadFromFile("../Arialn.ttf"))
	{
		std::cout<<"Fuck you!"<<std::endl;
		return 0;
	}
	sf::Text text;
	text.setFont(font);
	text.setCharacterSize(20);
	text.setFillColor(sf::Color::White);
	sf::RenderWindow window(sf::VideoMode(1200,1000),"Snake!");
	runGame(window, text);
	return 0;
}

void runGame(sf::RenderWindow& window, sf::Text& text){
	int board_d, speed;
	std::string input;
	
	wreset(window);
	text.setString("WELCOME TO SHITTY SNAKE!\nChoose the dimension of the board:");
	window.draw(text);
	window.display();
	input = getWindowInput(window, text);
	if(!input.empty()){
		board_d = std::stoi(input);
		board_d = board_d%80;
	}
	else{
		board_d = 20;
	}

	wreset(window);
	text.setString("Choose the speed (1=fastest, 2=slower, etc.): ");
	window.draw(text);
	window.display();
	input = getWindowInput(window, text);
	if(!input.empty()){
		speed = std::stoi(input);
		speed = speed%5;
	}
	else{
		speed = 2;
	}

	SnakeGame game(board_d);
	sf::Event dir;
	cout<<board_d<<speed<<endl;
	while(window.isOpen() && game.getRun()){ 	
		wreset(window);
		text.setString("");
		while(window.pollEvent(dir)){
			if(dir.type == sf::Event::Closed) window.close();
			else{
				game.move(dir);
				text.setString(game.getBoard());
				window.draw(text);
				window.display();
				usleep(speed*base_speed);
			}
		}
	}
	usleep(base_speed);
	fine(window,text,5);
}

void fine(sf::RenderWindow& window, sf::Text& text, int score){
	if(window.isOpen()){
		wreset(window);
		string output = "You lost! Final Score: "+std::to_string(score)+"\n";
		output=output+"[R] Retry [Q] Quit";
		text.setString(output);
		wreset(window);
		window.draw(text);
		window.display();
	
		sf::Event event;
		while(window.waitEvent(event)){
			if(event.type == sf::Event::Closed){
				window.close();
			}
			else if (event.type == sf::Event::KeyPressed){
				switch(event.key.code){
					case sf::Keyboard::R:
						runGame(window, text);
						break;
					case sf::Keyboard::Q:
						window.close();
						return;
				}
			}
		}
	}
}

int set_speed_input(int speed_val){
	return (speed_val==-1?base_speed:speed_val*base_speed);
}

void wreset(sf::RenderWindow& window){
	window.clear(sf::Color::Black);
}

string getWindowInput(sf::RenderWindow& window, sf::Text& text){
	string u_input="";
	sf::Event event;
	while(window.waitEvent(event)){
		if(event.type == sf::Event::Closed){
			window.close();
			return u_input;
		}
		else if (event.type == sf::Event::TextEntered){
			unsigned short unicode = event.text.unicode;
			if(unicode==13){
				return u_input;
			}
			if(unicode<=57 && unicode>=48){
				u_input+=(char)unicode;
				text.setString(text.getString()+u_input.back());
				wreset(window);
				window.draw(text);
				window.display();
			}
		}
	}
	return u_input;
}
