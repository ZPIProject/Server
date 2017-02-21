#include "ServerManager.h"



ServerManager::ServerManager()
{
	if (tcp_listener.listen(50000) != sf::Socket::Done)
		return;
	selector.add(tcp_listener);
	server_is_running = true;
}


ServerManager::~ServerManager()
{
	for (std::vector<sf::TcpSocket*>::iterator it = clients.begin(); it != clients.end(); it++)
		delete *it;
}

void ServerManager::run()
{
	while (server_is_running)
	{
		if (selector.wait()) //czeka na dane ze wszystkich socket'�w
		{
			if (selector.isReady(tcp_listener)) //sprawdzamy czy s� jakies przychodz�ce po�aczenia 
			{
				listener_is_ready();
			}
			else //bedziemy sprawdzac kt�ry klient chce cos wys�a� wywo�uje sie tylko wtedy, gdy do listenera nie chc� po��czy� si� gracze
			{
				for (std::vector<sf::TcpSocket*>::iterator it = clients.begin(); it != clients.end(); it++)
				{
					checking_which_client_send_data_and_processing_it(*it);
					
				}
			}
		}
	}
}

void ServerManager::listener_is_ready()
{
	std::cout << "Incomming connection\n";
	sf::TcpSocket* client = new sf::TcpSocket;
	if (tcp_listener.accept(*client) == sf::Socket::Done)
	{
		clients.push_back(client);
		selector.add(*client);
		std::cout << "Client " << client->getRemoteAddress() << " was added\n";
	}
}

void ServerManager::send_packet_to_client_all_clients_expect_sender(sf::Packet packet, sf::TcpSocket* sender)
{
	for (sf::TcpSocket* client : clients)
	{
		if (client != sender)
		{
			if (client->send(packet) != sf::Socket::Done)
			{
				std::cout << "Failed to send packet to: " << client->getRemoteAddress();
			}
		}
	}
}

void ServerManager::checking_which_client_send_data_and_processing_it(sf::TcpSocket* client)
{
	sf::TcpSocket& client_to_check = *client;
	if (selector.isReady(client_to_check)) //sprawdzamy kt�ry klient cos nada�
	{
		sf::Packet packet;
		if (client_to_check.receive(packet) == sf::Socket::Done)
		{

			send_packet_to_client_all_clients_expect_sender(packet, client);//wysy�a pakiet do klienta

		}

	}
}