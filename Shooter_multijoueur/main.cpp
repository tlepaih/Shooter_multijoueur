#include "TCP.h"
#include "Player.h"

constexpr auto IP = "192.168.43.228";

struct Tir {
	sf::CircleShape shape;
	sf::Vector2f pos;
	sf::Vector2f speed;

	bool ennemi;
};

bool updateTirsAllies(sf::RenderWindow& window, std::vector<Tir>& v, Player& myPlayer);

void updateTirsEnnemi(sf::RenderWindow& window, std::vector<Tir>& v);
void updateTirsEnnemi(sf::RenderWindow& window, std::vector<Tir>& v, sf::Packet& packet);

void fillInfosNewTir(sf::Packet& packet, const std::vector<Tir>& v);

int main(void)
{
	// Liaison reseau TCP
	Tcp tcp(IP);
	// Packet commun à la réception et à l'envoi des données
	sf::Packet packet;

	sf::RenderWindow window(sf::VideoMode(1200, 900), "2D Multiplayer Shooter");

	// Creation of players
	Player myPlayer(sf::Vector2f(600.0f, 450.0f), sf::Color::Blue);
	Player ennemiPlayer(sf::Vector2f(600.0f, 450.0f), sf::Color::Red);

	sf::Vector2f mousePos = (sf::Vector2f)sf::Mouse::getPosition(window);

	std::vector<Tir> mesTirs;
	std::vector<Tir> ennemiTirs;

	bool newTir = false;

	sf::Clock timer;

	while (window.isOpen())
	{
		sf::Event e;
		while (window.pollEvent(e))
		{
			if (e.type == sf::Event::Closed)
				window.close();
			if (e.type == sf::Event::MouseMoved)
				mousePos = (sf::Vector2f)sf::Mouse::getPosition(window);
		}

		// Clear
		window.clear();
		packet.clear();

		// Update players
		myPlayer.move();
		myPlayer.rotate(mousePos);

		// Update tirs
		newTir = updateTirsAllies(window, mesTirs, myPlayer);

		// Envoi des données à l'autre joueur
		packet.clear();
		myPlayer.fillInfosPacket(packet);
		tcp.send(packet);
		tcp.send(newTir);
		if (newTir) {
			fillInfosNewTir(packet, mesTirs);
			tcp.send(packet);
		}

		// Réception des données de l'autre joueur
		tcp.receive(packet);
		ennemiPlayer.updateOnPacket(packet);
		tcp.receive(newTir);
		if (newTir) {
			packet.clear();
			tcp.receive(packet);
			updateTirsEnnemi(window, ennemiTirs, packet);
		}
		else {
			updateTirsEnnemi(window, ennemiTirs);
		}

		// Draw
		myPlayer.draw(window);
		ennemiPlayer.draw(window);

		// Display
		window.display();
	}

	return 0;
}

bool updateTirsAllies(sf::RenderWindow &window, std::vector<Tir> &v, Player &myPlayer)
{
	static sf::Clock timer;

	Tir* nouveauTir;
	bool nTir = false;

	if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && timer.getElapsedTime().asSeconds() >= 0.0f) {
		timer.restart();

		nouveauTir = new Tir;
		nouveauTir->ennemi = false;
		nouveauTir->pos = myPlayer.getPos() + (myPlayer.getDir() * 10.0f);

		nouveauTir->speed = myPlayer.getDir() * 1.0f;

		float radiusTir = 8.0f;
		nouveauTir->shape.setRadius(radiusTir);
		nouveauTir->shape.setOrigin(radiusTir, radiusTir);
		nouveauTir->shape.setPosition(nouveauTir->pos);
		nouveauTir->shape.setFillColor(sf::Color::Yellow);

		v.push_back(*nouveauTir);
		nTir = true;
	}

	if (v.size() > 0) {
		for (unsigned i = 0; i < v.size(); ++i) {
			if (v[i].pos.x < -10.0f || v[i].pos.x > window.getSize().x + 10.0f || v[i].pos.y < -10.0f || v[i].pos.y > window.getSize().y + 10.0f)
				v.erase(v.begin() + i);
		}
		if (v.size() == 0)
			nTir = false;
	}

	for (unsigned i = 0; i < v.size(); ++i) {
		v[i].shape.move(v[i].speed);
		v[i].pos = v[i].shape.getPosition();

		window.draw(v[i].shape);
	}
	return nTir;
}

void updateTirsEnnemi(sf::RenderWindow &window, std::vector<Tir> &v)
{
	if (v.size() > 0) {
		for (unsigned i = 0; i < v.size(); ++i) {
			if (v[i].pos.x < -10.0f || v[i].pos.x > window.getSize().x + 10.0f || v[i].pos.y < -10.0f || v[i].pos.y > window.getSize().y + 10.0f)
				v.erase(v.begin() + i);
		}
	}

	for (unsigned i = 0; i < v.size(); ++i) {
		v[i].shape.move(v[i].speed);
		v[i].pos = v[i].shape.getPosition();

		window.draw(v[i].shape);
	}
}

void updateTirsEnnemi(sf::RenderWindow& window, std::vector<Tir>& v, sf::Packet &packet)
{
	float x, y, sX, sY;
	Tir* nouveauTir = new Tir;

	if (packet >> x >> y >> sX >> sY);
	else std::cout << "ERROR::RECEPTION_PACKET::ENNEMI_TIR" << std::endl;

	nouveauTir->speed = sf::Vector2f(sX, sY);
	nouveauTir->pos = sf::Vector2f(x, y);
	float radiusTir = 8.0f;
	nouveauTir->shape.setRadius(radiusTir);
	nouveauTir->shape.setOrigin(radiusTir, radiusTir);
	nouveauTir->shape.setPosition(nouveauTir->pos);
	nouveauTir->shape.setFillColor(sf::Color::Color(255, 150, 0));
	
	nouveauTir->ennemi = true;

	v.push_back(*nouveauTir);

	if (v.size() > 0) {
		for (unsigned i = 0; i < v.size(); ++i) {
			if (v[i].pos.x < -10.0f || v[i].pos.x > window.getSize().x + 10.0f || v[i].pos.y < -10.0f || v[i].pos.y > window.getSize().y + 10.0f)
				v.erase(v.begin() + i);
		}
	}

	for (unsigned i = 0; i < v.size(); ++i) {
		v[i].shape.move(v[i].speed);
		v[i].pos = v[i].shape.getPosition();

		window.draw(v[i].shape);
	}
}

void fillInfosNewTir(sf::Packet& packet, const std::vector<Tir>& v) {
	packet.clear();
	packet << v.back().pos.x << v.back().pos.y << v.back().speed.x << v.back().speed.y;
}
