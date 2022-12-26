#include "Menu.h"

Menu::Menu(float width, float height, sf::Font& font)
{
	selectedItemIndex = 0;

	text[0].setFont(font);
	text[0].setFillColor(sf::Color::Green);
	text[0].setString("Play");
	text[0].setPosition(sf::Vector2f(width/2 - text[0].getGlobalBounds().width / 2,height/(MAIN_MENU_ITEMS+1)*1));

	text[1].setFont(font);
	text[1].setCharacterSize(32);
	text[1].setFillColor(sf::Color::White);
	text[1].setString("Settings");
	text[1].setPosition(sf::Vector2f(width / 2 - text[1].getGlobalBounds().width / 2, height / (MAIN_MENU_ITEMS + 1) * 2));

	text[2].setFont(font);
	text[2].setFillColor(sf::Color::White);
	text[2].setString("Exit");
	text[2].setPosition(sf::Vector2f(width / 2 - text[2].getGlobalBounds().width / 2, height / (MAIN_MENU_ITEMS + 1) * 3));
}

Menu::Menu()
{

}

Menu::~Menu()
{

}

void Menu::draw(sf::RenderTarget& target)
{
	for (int i = 0; i < MAIN_MENU_ITEMS; i++)
	{
		//printf("%f\n", text[i].getPosition().y);
		target.draw(text[i]);
	}
}

void Menu::MoveUp()
{
	if (selectedItemIndex - 1 >= 0)
	{
		text[selectedItemIndex].setFillColor(sf::Color::White);
		selectedItemIndex--;
		text[selectedItemIndex].setFillColor(sf::Color::Green);
	}
	else
	{
		text[selectedItemIndex].setFillColor(sf::Color::White);
		selectedItemIndex = MAIN_MENU_ITEMS-1;
		text[selectedItemIndex].setFillColor(sf::Color::Green);
	}
}

void Menu::MoveDown()
{
	if (selectedItemIndex + 1 < MAIN_MENU_ITEMS)
	{
		text[selectedItemIndex].setFillColor(sf::Color::White);
		selectedItemIndex++;
		text[selectedItemIndex].setFillColor(sf::Color::Green);
	}
	else
	{
		text[selectedItemIndex].setFillColor(sf::Color::White);
		selectedItemIndex = 0;
		text[selectedItemIndex].setFillColor(sf::Color::Green);
	}
}
