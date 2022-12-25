#pragma once

#include <stdio.h>
#include <iostream>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

class Stars : public sf::Drawable, public sf::Transformable
{
public:
    sf::Texture texture;
    Stars(unsigned int count = 1) :
        m_particles(count),
        m_vertices(sf::Quads, count*4),
        m_lifetime(sf::seconds(3.f))
    {
        if (!texture.loadFromFile("Textures/star.png"))
        {
            //error
        }
    }

    void updatePosition(sf::View view)
    {
        for (size_t i = 0; i < m_particles.size(); i++)
        {
            float randX = static_cast<float>(rand() % static_cast<int>(view.getSize().x));
            float randY = static_cast<float>(rand() % static_cast<int>(view.getSize().y));
            float size = static_cast<float>(rand() % 10);
            float lifetime = (static_cast<float>(rand() % 60) - 20)/10;
            sf::Uint8 color = static_cast<sf::Uint8>((rand() % 128) + 127);
            int colorToAdd = rand() % 3;
            float colorContrast = static_cast<float>(rand() % 10)/10.f+1.f;
            sf::Color vertColor;
            if (colorToAdd == 0) {
                vertColor = sf::Color(color, color / colorContrast, color / colorContrast, 255);

            }
            else if (colorToAdd == 1) {
                vertColor = sf::Color(color / colorContrast, color / colorContrast, color, 255);
            }
            else
            {
                vertColor = sf::Color(color, color, color, 255);
            }
            Particle& p = m_particles[i];
            p.lifetime = lifetime;
            m_vertices[i * 4].position = sf::Vector2f(randX, randY);
            m_vertices[i * 4].texCoords = sf::Vector2f(0.f + 10.f*size, 0.f);
            m_vertices[i * 4 + 1].position = sf::Vector2f(randX + 10.f, randY);
            m_vertices[i * 4 + 1].texCoords = sf::Vector2f(10.f + 10.f * size, 0.f);
            m_vertices[i * 4 + 2].position = sf::Vector2f(randX + 10.f, randY + 10.f);
            m_vertices[i * 4 + 2].texCoords = sf::Vector2f(10.f + 10.f * size, 10.f);
            m_vertices[i * 4 + 3].position = sf::Vector2f(randX, randY + 10.f);
            m_vertices[i * 4 + 3].texCoords = sf::Vector2f(0.f + 10.f * size, 10.f);
            m_vertices[i * 4].color = vertColor;
            m_vertices[i * 4 + 1].color = vertColor;
            m_vertices[i * 4 + 2].color = vertColor;
            m_vertices[i * 4 + 3].color = vertColor;
        }
    }

    void update(float elapsed)
    {
        for (std::size_t i = 0; i < m_particles.size(); ++i)
        {
            // atnaujinti particle
            Particle& p = m_particles[i];
            p.lifetime -= elapsed;
            //std::cout << p.lifetime << std::endl;
            // kai lifetime baigiasi tai resetinti
            if (p.lifetime < -(m_lifetime.asSeconds()))
            {
                resetParticle(i);
            }

            // atnaujinti alpha
            float ratio = (p.lifetime) / m_lifetime.asSeconds();
            sf::Uint8 alpha = static_cast<sf::Uint8>(abs(ratio) * 255);
            m_vertices[i * 4].color.a = alpha;
            m_vertices[i * 4 + 1].color.a = alpha;
            m_vertices[i * 4 + 2].color.a = alpha;
            m_vertices[i * 4 + 3].color.a = alpha;
        }
    }

private:

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        // pritaikyti transform kad galetu rotate
        states.transform *= getTransform();

        // prideti texture
        states.texture = &texture;

        // draw
        target.draw(m_vertices, states);
    }

private:

    struct Particle
    {
        float lifetime;
    };

    void resetParticle(std::size_t index)
    {
        // random color change laikas
        m_particles[index].lifetime = 3.f;
    }

    std::vector<Particle> m_particles;
    sf::VertexArray m_vertices;
    sf::Time m_lifetime;
};