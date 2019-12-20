#include "Player.h"

constexpr float PI = (float)3.141592;

Player::Player(const sf::Vector2f& pos, const sf::Color& c)
	:speed(1.0f), angle(0.0f)
{
	this->pos = pos;

	float bodyLenght = 50.0f;

	this->body.setPointCount(3);
	this->body.setRadius(bodyLenght);
	this->body.setScale(0.5f, 1.0f);
	this->body.setOrigin(this->body.getLocalBounds().width/2.0f, this->body.getLocalBounds().height/2.0f);

	this->body.setPosition(this->pos);
	this->body.setFillColor(c);
}

void Player::move()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		this->body.move(1.0f, 0.0f);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
		this->body.move(-1.0f, 0.0f);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
		this->body.move(0.0f, -1.0f);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		this->body.move(0.0f, 1.0f);

	this->pos = this->body.getPosition();
}

void Player::updateOnPacket(sf::Packet& packet)
{
	float x, y, angle;

	if (packet >> x >> y >> angle);
	else std::cout << "ERROR::RECEPTION_PACKET::ENNEMI_POS" << std::endl;

	this->body.setPosition(x, y);
	this->body.setRotation(angle);
}

void Player::rotate(const sf::Vector2f& mousePos)
{
	this->direction = mousePos - this->pos;
	this->direction /= sqrt(this->direction.x * this->direction.x + this->direction.y * this->direction.y);
	float angle = atan2(-this->direction.y, -this->direction.x) * (180.0f / PI);

	this->angle = angle - 90.0f;
	this->body.setRotation(this->angle);
}

void Player::fillInfosPacket(sf::Packet& packet)
{
	packet.clear();
	packet << this->pos.x << this->pos.y << this->angle;
}

void Player::draw(sf::RenderWindow& window)
{
	window.draw(this->body);
}

sf::Vector2f Player::getPos() const
{
	return this->pos;
}

sf::Vector2f Player::getDir() const
{
	return this->direction;
}

float Player::getAngle() const
{
	return this->angle;
}
