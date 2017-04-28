#include "ServerManager.h"



ServerManager::ServerManager()
{
	if (tcp_listener.listen(5522) != sf::Socket::Done)
		return;
	server_is_running = true;
}


ServerManager::~ServerManager()
{
	for (std::vector<sf::TcpSocket*>::iterator it = clients.begin(); it != clients.end(); it++)
		delete *it;
}

void ServerManager::run()
{
	selector.add(tcp_listener);
	while (server_is_running)
	{
		if (selector.wait()) //czeka na dane ze wszystkich socket'ów
		{
			if (selector.isReady(tcp_listener)) //sprawdzamy czy s¹ jakies przychodz¹ce po³aczenia 
			{
				add_client_to_client_list(); //dodanie klienta do
			}
			else //bedziemy sprawdzac który klient chce cos wys³aæ wywo³uje sie tylko wtedy, gdy do listenera nie chc¹ po³¹czyæ siê gracze
			{
				for (std::vector<sf::TcpSocket*>::iterator it = clients.begin(); it != clients.end(); it++)
				{
					checking_which_client_send_data_and_processing_it(*it);
					
				}
			}
		}
	}
}

void ServerManager::udp_server_run()
{
	/*while (server_is_running)
	{
		
	}*/
}

void ServerManager::add_client_to_client_list()
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
	if (selector.isReady(client_to_check)) //sprawdzamy który klient cos nada³
	{
		sf::Packet packet;
		if (client_to_check.receive(packet) == sf::Socket::Done)//tutaj sprawdzane jest czy pakiet zosta³ poprawnie otrzymany
		{
			int type;
			packet >> type;
			PacketType packet_type = static_cast<PacketType>(type);
			if (packet_type == PacketType::GamePacket)
			{
				send_packet_to_client_all_clients_expect_sender(packet, client);//wysy³a pakiet do klienta
			}
			else if (packet_type == PacketType::LoginPacket)
			{
				get_db_querry_result(packet, client);
			}


		}

	}
}

void ServerManager::get_db_querry_result(sf::Packet packet, sf::TcpSocket* sender) //interfejs funkcji które bedziesz tworzy³ 
//w packet s¹ informacje o type querry
//¿eby wyci¹gn¹æ cos z pakietu robisz packet >> zmienna musisz wiedzieæ jak wygl¹da dany pakiet ¿eby go obs³u¿yæ
{
	sf::Packet packet_to_send; // tworzy pakiet do wys³ania, wype³niasz ten pakiet

	if (sender->send(packet_to_send) != sf::Socket::Done) // to jest wysy³anie pakietu wraz ze sprawdzaniem czy zosta³ wys³any poprawnie
	{
		std::cout << "Failed to send packet to: " << sender->getRemoteAddress();
	}
}