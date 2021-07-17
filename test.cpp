#include <iostream>
#include <SFML/Graphics.hpp>
#include "game.hpp"
using namespace std;

int main(){
	sf::Font font;
	if (!font.loadFromFile("../Arialn.ttf"))
	{
		cout<<"Fuck you!"<<endl;
		return 0; //very bad but i'm lazy
	}
	sf::Text text;
	text.setFont(font);
	text.setCharacterSize(8);
    text.setFillColor(sf::Color::Black);

	sf::RenderWindow window(sf::VideoMode(800,800), "Snake");
	window.setKeyRepeatEnabled(false);
	
	sf::String st;
	std::string st2;


	//while (window.isOpen()){
	// 	sf::Event event;
	// 	while(window.pollEvent(event)){
	// 		if(event.type == sf::Event::Closed){
	// 			window.close();
	// 		}
	// 		else if (event.type == sf::Event::KeyPressed){
	// 			switch(event.key.code){
	// 				case sf::Keyboard::Up:
	// 					st = "Up!";
	// 					text.setString(st);
	// 					break;
	// 				case sf::Keyboard::Down:
	// 					st2 = "Down!";
	// 					text.setString(st2);
	// 					break;
	// 				case sf::Keyboard::Left:
	// 					text.setString("Left!");
	// 					break;
	// 				case sf::Keyboard::Right:
	// 					text.setString("Right!");
	// 					break;
	// 			}
	// 		}
	// 	}
	//  	window.clear(sf::Color::White);
	//  	window.draw(text);
	//  	window.display();
	//}

	return 0;
}
