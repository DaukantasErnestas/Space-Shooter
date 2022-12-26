#pragma once

#include "star.h"
#include "menu.h"
#include "explosion.h"

#include <stdio.h>
#include <vector>
#include <ctime>
#include <sstream>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>

class Game
{
private:
	sf::RenderWindow* window;
	int windowSizeX;
	int windowSizeY;
	int windowAspectRatio;

	sf::View view;

	sf::Event ev;

	sf::Clock clock;
	sf::Time previousTime;
	sf::Time currentTime;
	float fps;
	float delta;

	sf::Vector2i mousePosWindow;
	sf::Vector2f mousePosView;

	sf::Font font;

	sf::Text uiText;

	bool endGame;
	bool inMenus;
	unsigned points;
	int health;
	float enemySpawnTimer;
	float enemySpawnTimerMax;
	int maxEnemies;
	bool mouseHeld;

	std::vector<sf::Sprite> enemies;
	std::vector<explosion> explosions;
	sf::Texture explosionTexture;
	Stars stars;
	sf::Texture enemyTexture;
	sf::Texture smallEnemyTexture;
	sf::Sprite enemy;

	sf::RectangleShape background;

	Menu menu;

	void initVariables();
	void initBackground();
	void initWindow();
	void initFonts();
	void initText();
	void initEnemies();
public:
	Game();
	virtual ~Game();

	const bool running() const;
	const bool getEndGame() const;
	const sf::View getLetterboxView(sf::View view, int windowWidth, int windowHeight) const;

	void spawnEnemy();

	void pollEvents();
	void updateMousePositions();
	void updateTimeVariables();

	void updateBackground();
	void updateText();
	void updateEnemies();
	void update();

	void renderBackground(sf::RenderTarget& target);
	void renderText(sf::RenderTarget& target);
	void renderEnemies(sf::RenderTarget& target);
	void renderExplosions(sf::RenderTarget& target);
	void renderMenu(sf::RenderTarget& target);
	void render();
};

