#pragma once
#include<SFML/Graphics.hpp>

#define MAIN_MENU_ITEMS 3
class Menu
{
public:
	Menu(float width, float height,sf::Font& font);
	Menu();
	~Menu();

	void draw(sf::RenderTarget& target);

	void MoveUp();
	void MoveDown();
private:
	int selectedItemIndex;
	sf::Text text[MAIN_MENU_ITEMS];
};

