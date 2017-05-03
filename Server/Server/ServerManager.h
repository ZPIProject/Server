#pragma once
#include <SFML\Network.hpp>
#include <iostream>
#include "DatabaseManager.h"
#include "PacketType.h"
class ServerManager
{
public:
	ServerManager();
	~ServerManager();

	void run();
	void udp_server_run();

private:
	sf::TcpListener tcp_listener;
	sf::SocketSelector selector;
	std::vector<sf::TcpSocket*> clients;
	std::vector<sf::UdpSocket*> udp_clients;
	DatabaseManager DbMan;
	bool server_is_running;

	void add_client_to_client_list();
	void send_login_info(sf::Packet packet, sf::TcpSocket* sender);
	void send_packet_to_client_all_clients_expect_sender(sf::Packet packet, sf::TcpSocket* sender);
	void checking_which_client_send_data_and_processing_it(sf::TcpSocket* client);
	void send_packet_to_client(sf::Packet& packet, sf::TcpSocket* client);
	
};
