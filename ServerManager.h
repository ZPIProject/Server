#pragma once
#include <SFML\Network.hpp>
#include <iostream>
class ServerManager
{
public:
	ServerManager();
	~ServerManager();

	void run();

private:
	sf::TcpListener tcp_listener;
	sf::SocketSelector selector;
	std::vector<sf::TcpSocket*> clients;

	bool server_is_running;

	void listener_is_ready();
	void send_packet_to_client_all_clients_expect_sender(sf::Packet packet, sf::TcpSocket* sender);
	void checking_which_client_send_data_and_processing_it(sf::TcpSocket* client);

	
};

