#pragma once

#include <mysql_connection.h>
#include <mysql_driver.h>
#include <mysql_error.h>

#include <cppconn\driver.h>
#include <cppconn\exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <SFML\Network.hpp>
class DatabaseManager {
private:
	sql::Driver *driver;
	sql::Connection *con;
	sql::Statement *stmt;
public:
	DatabaseManager();
	sf::Packet getLoginInfo(std::string login, std::string pass);
	sf::Packet getCharacterSkills(std::string name);
	sf::Packet getCharacterAvailablePoints(std::string name);
	sf::Packet insertNewAccount(std::string name, std::string password, std::string email);
	sf::Packet insertNewCharacter(std::string login, std::string nick);
	sf::Packet insertNewFight(std::string winner, std::string loser, int duration);
	sf::Packet buySkill(std::string nick, int skillId);
	sf::Packet LevelUp(std::string nick);
	sf::Packet AddExp(std::string nick, int ammount);
	sf::Packet DeleteAccount(std::string name);
	sf::Packet DeleteHero(std::string nick);
};