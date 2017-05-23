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
			sf::Packet copy_packet = packet;
			packet >> type;
			
			PacketType packet_type = static_cast<PacketType>(type);
			if (packet_type == PacketType::GAME_PACKET)
			{
				send_packet_to_client_all_clients_expect_sender(packet, client);//wysy³a pakiet do klienta
			}
			else if (packet_type == PacketType::LOGIN_PACKET)
			{
				send_login_info(copy_packet, client);
			}
			else if (packet_type == PacketType::ADD_EXP_PACKET)
			{
				add_exp(copy_packet, client);
			}
			else if (packet_type == PacketType::BUY_SKILL_PACKET)
			{
				buy_skill(copy_packet, client);
			}
			else if (packet_type == PacketType::CHAR_INFO_PACKET)
			{
			}
			else if (packet_type == PacketType::CHAR_KNOWS_SKILL_PACKET)
			{
				send_Hero_Known_Skills(copy_packet, client);
			}
			else if (packet_type == PacketType::CHAR_POINTS_AVAL_PACKET)
			{
				send_Aval_points(copy_packet, client);
			}
			else if (packet_type == PacketType::DELETE_ACC_PACKET)
			{
				delete_Account(copy_packet, client);
			}
			else if (packet_type == PacketType::DELETE_CHAR_PACKET)
			{
				delete_Hero(copy_packet, client);
			}
			else if (packet_type == PacketType::LEVEL_UP_PACKET)
			{
				level_Up(copy_packet, client);
			}
			else if (packet_type == PacketType::NEW_ACC_PACKET)
			{
				new_Account(copy_packet, client);
			}
			else if (packet_type == PacketType::NEW_CHAR_PACKET)
			{
				new_Hero(copy_packet, client);
			}
			else if (packet_type == PacketType::NEW_FIGHT_PACKET)
			{
				new_Fight(copy_packet, client);
			}


		}

	}
}

void ServerManager::send_packet_to_client(sf::Packet & packet, sf::TcpSocket * client)
{
	if (client->send(packet) != sf::Socket::Done) // to jest wysy³anie pakietu wraz ze sprawdzaniem czy zosta³ wys³any poprawnie
	{
		std::cout << "Failed to send packet to: " << client->getRemoteAddress();
	}
}

void ServerManager::add_exp(sf::Packet packet, sf::TcpSocket * sender)
{
	int type, ammount;
	std::string nick;
	packet >> type >> nick >> ammount;
	send_packet_to_client(DbMan.AddExp(nick, ammount),sender);
}

void ServerManager::buy_skill(sf::Packet packet, sf::TcpSocket * sender)
{
	int t, id;
	std::string nick;
	packet >> t >> nick >> id;
	send_packet_to_client(DbMan.buySkill(nick, id),sender);
}

void ServerManager::send_Hero_Known_Skills(sf::Packet packet, sf::TcpSocket * sender)
{
	std::string nick;
	int t;
	packet >> t >> nick;
	send_packet_to_client(DbMan.getCharacterSkills(nick), sender);
}

void ServerManager::send_Aval_points(sf::Packet packet, sf::TcpSocket * sender)
{
	std::string nick;
	int t;
	packet >> t >> nick;
	send_packet_to_client(DbMan.getCharacterAvailablePoints(nick), sender);
}

void ServerManager::delete_Account(sf::Packet packet, sf::TcpSocket * sender)
{
	std::string nick;
	int t;
	packet >> t >> nick;
	send_packet_to_client(DbMan.DeleteAccount(nick), sender);
}

void ServerManager::delete_Hero(sf::Packet packet, sf::TcpSocket * sender)
{
	std::string nick;
	int t;
	packet >> t >> nick;
	send_packet_to_client(DbMan.DeleteHero(nick), sender);
}

void ServerManager::level_Up(sf::Packet packet, sf::TcpSocket * sender)
{
	std::string nick;
	int t;
	packet >> t >> nick;
	send_packet_to_client(DbMan.LevelUp(nick), sender);
}

void ServerManager::new_Account(sf::Packet packet, sf::TcpSocket * sender)
{
	std::string nick,pass,email;
	int t;
	packet >> t >> nick>>pass>>email;
	send_packet_to_client(DbMan.insertNewAccount(nick, pass, email), sender);
}

void ServerManager::new_Hero(sf::Packet packet, sf::TcpSocket * sender)
{
	std::string nick,login;
	int t;
	packet >> t >>login>> nick;
	send_packet_to_client(DbMan.insertNewCharacter(login,nick), sender);
}

void ServerManager::new_Fight(sf::Packet packet, sf::TcpSocket * sender)
{
	std::string winner,loser;
	int t,duration;
	packet >> t >> winner>>loser>>duration;
	send_packet_to_client(DbMan.insertNewFight(winner,loser,duration), sender);
}


void ServerManager::send_login_info(sf::Packet packet, sf::TcpSocket* sender) 
{
	std::string login;
	std::string pass;
	int type;
	packet >> type >> login >> pass;
	std::cout <<"pakiet z danymi do funkcji   "<<" "<< login << " " << pass<<std::endl;
	send_packet_to_client(DbMan.getLoginInfo(login,pass),sender);
}