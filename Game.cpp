#include "Game.h"

void Game::initVariables()
{
	this->window = nullptr;
	this->windowSizeX = 640;
	this->windowSizeY = 360;
	this->windowAspectRatio = this->windowSizeX / this->windowSizeY;

	this->clock = sf::Clock::Clock();
	this->previousTime = clock.getElapsedTime();
	this->fps = 0.f;
	this->delta = 0.f;

	this->enemyTexture.loadFromFile("Textures/rocket.png");
	this->smallEnemyTexture.loadFromFile("Textures/ValuableAsteroid.png");

	this->explosionTexture.loadFromFile("Textures/explosion.png");

	this->endGame = false;
	this->inMenus = true;
	this->points = 0;
	this->health = 10;
	this->enemySpawnTimerMax = 0.5f;
	this->enemySpawnTimer = this->enemySpawnTimerMax;
	this->maxEnemies = 10;
	this->mouseHeld = false;
}

void Game::initWindow()
{
	this->window = new sf::RenderWindow(sf::VideoMode(640, 360), "SFML Test");
	this->view.setSize(static_cast<float>(this->windowSizeX), static_cast<float>(this->windowSizeY));
	this->view.setCenter(view.getSize().x / 2, view.getSize().y / 2);
	this->view = getLetterboxView(this->view, this->windowSizeX, this->windowSizeY);
	this->menu = Menu(this->view.getSize().x,this->view.getSize().y,this->font);
	//this->window->setFramerateLimit(60);
}

void Game::initEnemies()
{
	this->enemy = sf::Sprite(enemyTexture);
	this->enemy.setPosition(10.f,10.f);
}

Game::Game()
{
	this->initVariables();
	this->initText();
	this->initWindow();
	this->initBackground();
	this->initFonts();
	this->initEnemies();
}

Game::~Game()
{
	delete this->window;
}

const bool Game::running() const
{
	return this->window->isOpen();
}

void Game::pollEvents()
{
	while (this->window->pollEvent(this->ev))
	{
		if (this->ev.type == sf::Event::Closed) window->close();

		if (this->ev.type == sf::Event::Resized) {
			this->windowSizeX = this->ev.size.width;
			this->windowSizeY = this->ev.size.height;
			this->view = getLetterboxView(this->view, this->ev.size.width, this->ev.size.height);
		}

		if (this->ev.type == sf::Event::KeyPressed)
		{
			switch (this->ev.key.code)
			{
			case sf::Keyboard::Up:
				menu.MoveUp();
				break;
			case sf::Keyboard::Down:
				menu.MoveDown();
				break;
			}
		}
	}
}

void Game::update()
{
	this->pollEvents();
	this->updateMousePositions();
	if (!this->endGame)
	{
		this->updateEnemies();
	}

	this->updateBackground();
	this->updateTimeVariables();
	this->updateText();

	if (this->health <= 0)
	{
		this->endGame = true;
	}
}

void Game::updateMousePositions()
{
	this->mousePosWindow = sf::Mouse::getPosition(*this->window);
	this->mousePosView = this->window->mapPixelToCoords(this->mousePosWindow);
}

void Game::render()
{
	this->window->clear();

	this->window->setView(this->view);

	this->renderBackground(*this->window);

	this->renderEnemies(*this->window);

	this->renderExplosions(*this->window);

	this->renderText(*this->window);

	if (this->inMenus)
		this->renderMenu(*this->window);

	this->window->display();
}

void Game::updateEnemies()
{
	if (this->enemies.size() < this->maxEnemies)
	{
		if (this->enemySpawnTimer >= this->enemySpawnTimerMax)
		{
			this->spawnEnemy();
			this->enemySpawnTimer = 0.f;
		}
		else
		{
			this->enemySpawnTimer += 1.f*this->delta;
		}
	}

	for (size_t i = 0; i < this->enemies.size(); i++)
	{
		bool deleted = false;

		this->enemies[i].move(0.f, 100.f*this->delta);
		
		int frame = (static_cast<int>(this->enemies[0].getPosition().y) % 20 < 10) ? 1 : 0;//static_cast<int>(floor(static_cast<float>(20 % static_cast<int>(this->enemies[i].getPosition().y))/10+0.5f))+1;

		if (this->enemies[i].getTexture() == &enemyTexture)
		{
			sf::IntRect textureRect = this->enemies[i].getTextureRect();
			if (textureRect.left != textureRect.width * frame)
			{
				textureRect.left = textureRect.width * frame;
				enemies[i].setTextureRect(textureRect);
			}
		}

		if (this->enemies[i].getPosition().y > this->view.getSize().y)
		{
			this->enemies.erase(this->enemies.begin() + i);
			this->health -= 1;
		}
	}
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		if (this->mouseHeld == false)
		{
			this->mouseHeld = true;
			bool deleted = false;
			for (size_t i = 0; i < this->enemies.size() && deleted == false; i++)
			{
				if (this->enemies[i].getGlobalBounds().contains(this->mousePosView))
				{
					if (this->enemies[i].getTexture() == &smallEnemyTexture)
					{
						this->points += 2;
					}
					else
					{
						this->points += 1;
					}
					explosion enemyExplode = explosion(this->enemies[i].getPosition() + sf::Vector2f(enemies[i].getGlobalBounds().width/2.f, enemies[i].getGlobalBounds().height/2.f) - sf::Vector2f(64.f/2.f,64.f/2.f), this->explosionTexture);
					explosions.push_back(enemyExplode);
					deleted = true;
					this->enemies.erase(this->enemies.begin() + i);
				}
			}
		}
	}
	else
	{
		this->mouseHeld = false;
	}
}

void Game::renderEnemies(sf::RenderTarget& target)
{
	for (auto& e : this->enemies)
	{
		target.draw(e);
	}
}

void Game::initFonts()
{
	this->font.loadFromFile("Fonts/rainyhearts.ttf");
}

void Game::initText()
{
	this->uiText.setFont(this->font);
	this->uiText.setCharacterSize(16);
	this->uiText.setFillColor(sf::Color::White);
	this->uiText.setOutlineColor(sf::Color::Black);
	this->uiText.setOutlineThickness(1);
	this->uiText.setString("N/A");
}

const bool Game::getEndGame() const
{
	return this->endGame;
}

void Game::initBackground()
{
	this->background.setFillColor(sf::Color(15, 15, 25, 255));
	this->background.setSize(sf::Vector2f(static_cast<float>(this->view.getSize().x), static_cast<float>(this->view.getSize().y)));
	this->stars = Stars(2000);
	this->stars.updatePosition(this->view);
}

void Game::updateText()
{
	std::stringstream ss;
	ss << "Points: " << this->points << "\n"
		<< "Health: " << this->health << "\n"
		<< "FPS: " << this->fps << "\n";
	this->uiText.setString(ss.str());
}

const sf::View Game::getLetterboxView(sf::View view, int windowWidth, int windowHeight) const
{
	float windowRatio = windowWidth / (float)windowHeight;
	float viewRatio = view.getSize().x / (float)view.getSize().y;
	float sizeX = 1;
	float sizeY = 1;
	float posX = 0;
	float posY = 0;

	bool horizontalSpacing = true;
	if (windowRatio < viewRatio)
		horizontalSpacing = false;

	if (horizontalSpacing) {
		sizeX = viewRatio / windowRatio;
		posX = (1 - sizeX) / 2.f;
	}

	else {
		sizeY = windowRatio / viewRatio;
		posY = (1 - sizeY) / 2.f;
	}

	view.setViewport(sf::FloatRect(posX, posY, sizeX, sizeY));

	return view;
}

void Game::updateBackground()
{
	this->background.setFillColor(sf::Color(15, 15, 25, 255));
	this->background.setSize(sf::Vector2f(static_cast<float>(this->view.getSize().x), static_cast<float>(this->view.getSize().y)));
}

void Game::renderBackground(sf::RenderTarget& target)
{
	target.draw(this->background);

	target.draw(stars);
}

void Game::updateTimeVariables()
{
	currentTime = clock.getElapsedTime();
	this->fps = 1.0f / (currentTime.asSeconds() - previousTime.asSeconds());
	this->delta = (currentTime - previousTime).asSeconds();
	stars.update((currentTime - previousTime).asSeconds());
	for (size_t i = 0; i < this->explosions.size(); i++)
	{
		bool readyToDelete = this->explosions[i].update((this->currentTime - this->previousTime).asSeconds());
		if (readyToDelete)
		{
			printf("This");
			explosions.erase(this->explosions.begin() + i);
		}
	}
	previousTime = currentTime;
}

void Game::renderExplosions(sf::RenderTarget& target)
{
	for (auto& e : this->explosions)
	{
		target.draw(e);
	}
}

void Game::renderMenu(sf::RenderTarget& target)
{
	menu.draw(target);
}

void Game::renderText(sf::RenderTarget& target)
{
	target.draw(this->uiText);
}

void Game::spawnEnemy()
{
	int type = rand() % 10;

	switch (type)
	{
	case 0:
		this->enemy = sf::Sprite(smallEnemyTexture, sf::IntRect(0, 0, 10, 10));
		break;
	default:
		this->enemy = sf::Sprite(enemyTexture, sf::IntRect(0, 0, 32, 50));
		break;
	}

	this->enemy.setPosition(
		static_cast<float>(rand() % static_cast<int>(this->view.getSize().x - this->enemy.getGlobalBounds().width)),
		0.f - this->enemy.getGlobalBounds().height
	);

	this->enemies.push_back(this->enemy);
}

