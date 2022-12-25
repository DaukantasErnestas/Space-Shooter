#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

class explosion : public sf::Drawable
{
public:
	sf::Sprite sprite;
	explosion(sf::Vector2f pos, sf::Texture& texture)
	{
		counter = 0.f;
		sprite = sf::Sprite(texture, sf::IntRect(0, 0, 64, 64));
		sprite.setPosition(pos);
	}

	bool update(float elapsed)
	{
		counter += elapsed;
		int frame = static_cast<int>(floor(counter*30));
		printf("%d\n", frame);
		if (frame > 27)
		{
			return true;
		}
		sprite.setTextureRect(sf::IntRect(0+64*frame,0,64,64));
		return false;
	}
private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		target.draw(sprite, states);
	}
private:
	float counter;
};

