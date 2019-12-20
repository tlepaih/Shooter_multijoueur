#include "Tcp.h"

Tcp::Tcp(sf::IpAddress serverIP, const int port) : PORT(port)
{
    cout << "Voulez-vous \210tre un serveur (s) ou un client (c) ? ";
    char who;
    cin >> who;

    /// PARTIE SERVEUR ///
    if (who == 's')
    {
        sf::TcpListener listener; //écoute le PORT
        listener.listen(this->PORT);  //Le serveur écoute le port auquel veut se connecter le client
        listener.accept(this->socket);  //Le serveur accepte la connexion et initialise la socket "client"
        cout << "Nouveau client connect\202 : " << this->socket.getRemoteAddress() << endl;
    }
    /// PARTIE CLIENT ///
    else if (who == 'c')
    {
        if (this->socket.connect(serverIP, this->PORT) == sf::Socket::Done) //Le client envoie une requete de connexion au serveur
            cout << "Connect\202 au serveur" << endl;                            //ayant cette adresse IP et sur ce PORT
        else
            cout << "Le client n'a pas pu se connecter au serveur" << endl;
    }
    // Partie erreur
    else
        cout << "Mauvaise lettre saisie, relance le programme pour la peine" << endl;
}
