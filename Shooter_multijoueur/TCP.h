#pragma once

#include <SFML/Network.hpp>
#include <iostream>
#include <type_traits>

using namespace std;

class Tcp
{
public:
    Tcp(sf::IpAddress serverIP, const int port = 53000);

    template<typename T> void send(const T &data)
    {
        this->packet.clear();
        this->packet << data;
        this->socket.send(this->packet);
    }
    void send(sf::Packet &packet)
    {
        this->packet.clear();
        this->socket.send(packet);
    }

    template<typename T> void receive(T &data)
    {
        this->socket.receive(this->packet);
        if (this->packet >> data);
        else cout << "Erreur de r\202ception de donn\202es" << endl;
    }
    void receive(sf::Packet &packet)
    {
        this->socket.receive(packet);
    }

private:
    sf::Packet packet; //Packet qui nous sert à envoyer les données
    const int PORT; //port du server sur lequel le client va se connecter

    sf::TcpSocket socket; // **CÔTÉ CLIENT** Socket auquel le client envoie une demande de connexion (serveur donc)
                          // **CÔTÉ SERVEUR** Socket du client auquel le serveur se connecte
};

/// Surcharges d'opérateurs pour les Packets ///
inline sf::Packet& operator <<(sf::Packet& packet, sf::Vector2f& vect) {
    return packet << vect.x << vect.y;
}
inline sf::Packet& operator >>(sf::Packet& packet, sf::Vector2f& vect) {
    return packet >> vect.x >> vect.y;
}
/// ///////////////////////////////////////// ///

