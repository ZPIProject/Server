#include "ServerManager.h"
#include <SFML\Network\Packet.hpp>


ServerManager::ServerManager()
{
	if (tcp_listener.listen(5522) != sf::Socket::Done)
		return;
	server_is_running = true;
	DbMan = DatabaseManager();
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
		if (selector.wait()) //czeka na dane ze wszystkich socket'�w
		{
			if (selector.isReady(tcp_listener)) //sprawdzamy czy s� jakies przychodz�ce po�aczenia 
			{
				add_client_to_client_list(); //dodanie klienta do
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
	if (selector.isReady(client_to_check)) //sprawdzamy kt�ry klient cos nada�
	{
		sf::Packet packet;
		if (client_to_check.receive(packet) == sf::Socket::Done)//tutaj sprawdzane jest czy pakiet zosta� poprawnie otrzymany
		{
			int type;
			sf::Packet copy_packet = packet;
			packet >> type;
			
			PacketType packet_type = static_cast<PacketType>(type);
			if (packet_type == PacketType::GAME_PACKET)
			{
				send_packet_to_client_all_clients_expect_sender(packet, client);//wysy�a pakiet do klienta
			}
			else if (packet_type == PacketType::LOGIN_PACKET)
			{
				send_login_info(copy_packet, client);
			}


		}

	}
}

void ServerManager::send_packet_to_client(sf::Packet & packet, sf::TcpSocket * client)
{
	if (client->send(packet) != sf::Socket::Done) // to jest wysy�anie pakietu wraz ze sprawdzaniem czy zosta� wys�any poprawnie
	{
		std::cout << "Failed to send packet to: " << client->getRemoteAddress();
	}
}

void ServerManager::send_login_info(sf::Packet packet, sf::TcpSocket* sender) 
//w packet s� informacje o type querry
//�eby wyci�gn�� cos z pakietu robisz packet >> zmienna musisz wiedzie� jak wygl�da dany pakiet �eby go obs�u�y�
{
	std::string login;
	std::string pass;
	int type;
	packet >> type >> login >> pass;
	std::cout <<"pakiet z danymi do funkcji   "<<" "<< login << " " << pass<<std::endl;
	send_packet_to_client(DbMan.getLoginInfo(login,pass),sender);
}