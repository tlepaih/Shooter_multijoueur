#pragma once

#include <SFML/Graphics.hpp>

#include "TCP.h"
#include <cmath>

class Player
{
public:
	Player(const sf::Vector2f &pos, const sf::Color &c);

	// Local player relatives functions
	void move();
	void rotate(const sf::Vector2f &mousePos);
	void fillInfosPacket(sf::Packet& packet);

	// Distant player relatives functions
	void updateOnPacket(sf::Packet& packet);

	// Draw
	void draw(sf::RenderWindow &window);

	// Utils funtions
	sf::Vector2f getPos() const;
	sf::Vector2f getDir() const;
	float getAngle() const;

private:
	sf::CircleShape body;
	sf::Vector2f direction;
	float angle;

	sf::Vector2f pos;
	float speed;
};

