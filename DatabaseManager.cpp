#include "DatabaseManager.h"


DatabaseManager::DatabaseManager()
{
	driver= get_driver_instance();
	con=driver->connect("sql11.freemysqlhosting.net", "sql11176041", "3gPy5ZIHhs");
	con->setSchema("sql11176041");
	stmt = con->createStatement();
}

sf::Packet DatabaseManager::getLoginInfo(std::string login, std::string pass)
{
	std::cout << "LOGIN DATA RECEIVED: "<<login<<" "<<pass << std::endl;
	sf::Packet result;
	sql::ResultSet *res;
	std::string str = "SELECT loginA FROM Account WHERE loginA='"+login+"' AND passwordA='"+pass+"'";
	res = stmt->executeQuery(str.c_str());
	if (res->next()) {
		std::cout << "LOGGED IN" << std::endl;
		result << true;
	}
	else {
		std::cout << "FAILED TO LOG IN" << std::endl;
 result << false; }
	return result;
}

sf::Packet DatabaseManager::getCharacterSkills(std::string nazwa)
{
	sf::Packet result;
	sql::ResultSet *res;
	std::string str = "SELECT idSkill FROM Knows JOIN Hero ON Knows.idC = Hero.idC WHERE nicknameC = '"+nazwa+"' ";
	res = stmt->executeQuery(str.c_str());
	result << res->rowsCount();
	while (res->next()) {
		result << res->getInt("idSkill");
	}
	return result;
}

sf::Packet DatabaseManager::getCharacterAvailablePoints(std::string name)
{
	sf::Packet result;
	sql::ResultSet *res;
	std::string str = "SELECT skillpointsC FROM Hero WHERE nicknameC = '"+name+"'";
	res = stmt->executeQuery(str.c_str());
	if (res->next()) {
		result << res->getInt("skillpointsC");
	}
	std::cout << result << std::endl;
	return result;
}

sf::Packet DatabaseManager::insertNewAccount(std::string name, std::string password, std::string email)
{

	std::string str = "INSERT INTO Account VALUES ('"+name+"','"+password+"','"+email+"')";
	sf::Packet result;
	try {
		stmt->execute(str.c_str());
		return result<< true;
	}
	catch (sql::SQLException e) {
		return result<< false;
	}
	
	
}

sf::Packet DatabaseManager::insertNewCharacter(std::string login, std::string nick)
{

	std::string str = "INSERT INTO Hero(nicknameC, accountC) VALUES('"+nick+"', '"+login+"')";
	sf::Packet result;
	try {
		stmt->execute(str.c_str());
		return result << true;
	}
	catch (sql::SQLException e) {
		return result << false;
	}

}

sf::Packet DatabaseManager::insertNewFight(std::string winner, std::string loser, int duration)
{

	sf::Packet result;
	int winnerN = 0, loserN = 0;
	sql::ResultSet *res;
	std::string str = "SELECT idC FROM Hero WHERE nicknameC='" + winner +"'";
	res = stmt->executeQuery(str.c_str());
	if (res->next())
		winnerN = res->getInt("idC");
	str= "SELECT idC FROM Hero WHERE nicknameC='"+ loser + "'";
	sql::ResultSet *res2;
	res2= stmt->executeQuery(str.c_str());
	if (res2->next())
		loserN = res2->getInt("idC");
	std::cout << loserN<<std::endl;
 str = "INSERT INTO Fight (winnerC,loserC,dateF,durationF) VALUES (" + std::to_string(winnerN) + "," + std::to_string(loserN) + ",NOW()," + std::to_string(duration) + ")";
 try {
	 std::cout << str<<std::endl;
	stmt->execute(str.c_str());
	 std::cout << "adding fight:  winner= " << winnerN << "  loser= " + loserN << std::endl;
	 return result << true;
 }
 catch (sql::SQLException e) {
	 std::cout << "CATCHED EXCEPTION    " << e.getSQLState();
	 std::cout << "adding fight failed: winner= " << winnerN << "  loser= " + loserN << std::endl;
	 return result << false;
 }
}

sf::Packet DatabaseManager::buySkill(std::string nick, int skillId)
{
	sf::Packet result;
	int nickid = 0;
	sql::ResultSet *res;
	std::string str = "SELECT idC FROM Hero WHERE nicknameC='" + nick + "'";
	res = stmt->executeQuery(str.c_str());
	if (res->next())
		nickid = res->getInt("idC");
	std::cout <<"Hero has nick: " <<nickid<<std::endl;
	
	str = "INSERT INTO Knows (idSkill,idC) VALUES ("+ std::to_string(skillId) +","+ std::to_string(nickid)+")";
	try {
		std::cout << str << std::endl;
		stmt->execute(str.c_str());
		std::cout << "adding Skill:  Hero= " << nickid << "  SkillId= " + skillId << std::endl;
		str = " UPDATE Hero SET skillpointsC = skillpointsC-1 WHERE nicknameC='"+nick+"';";
		stmt->execute(str.c_str());
		return result << true;
	}
	catch (sql::SQLException e) {
		std::cout << "CATCHED EXCEPTION    " << e.getErrorCode();
		std::cout << " adding Skill failed:  Hero= " << nickid << "  SkillId= " + skillId << std::endl;
		return result << false;
	}
}

sf::Packet DatabaseManager::LevelUp(std::string nick)
{
	sf::Packet result;
	std::string str = "UPDATE Hero SET levelC = levelC + 1 WHERE nicknameC = '"+nick+"'";
	try {
		std::cout << str << std::endl;
		stmt->execute(str.c_str());
		std::cout << "LevelUp:  nick= " << nick << std::endl;
		str = "UPDATE Hero SET skillpointsC = skillpointsC+1 WHERE nicknameC='" + nick + "';";
		stmt->execute(str.c_str());
		return result << true;
	}
	catch (sql::SQLException e) {
		std::cout << "CATCHED EXCEPTION    " << e.getErrorCode();
		std::cout << "LevelUp failed:  nick= " << nick<< std::endl;
		return result << false;
	}
}

sf::Packet DatabaseManager::AddExp(std::string nick, int ammount)
{
	sf::Packet result;
	std::string str = "UPDATE Hero SET expC = expC+"+std::to_string(ammount)+" WHERE nicknameC='"+nick+"'";
	try {
		std::cout << str << std::endl;
		stmt->execute(str.c_str());
		std::cout << "AddExp:  nick= " << nick <<" ammount= "<<ammount<< std::endl;
		return result << true;
	}
	catch (sql::SQLException e) {
		std::cout << "CATCHED EXCEPTION    " << e.getErrorCode();
		std::cout << "AddExp Failed:  nick= " << nick << " ammount= " << ammount << std::endl;
		return result << false;
	}
}

sf::Packet DatabaseManager::DeleteAccount(std::string name)
{
	sf::Packet result;
	std::string str = "DELETE FROM Account WHERE loginA = '"+name+"'";
	try {
		std::cout << str << std::endl;
		stmt->execute(str.c_str());
		std::cout << "Delete account:  nick= " << name <<std::endl;
		return result << true;
	}
	catch (sql::SQLException e) {
		std::cout << "CATCHED EXCEPTION    " << e.getErrorCode();
		std::cout << "Delete account failed:  nick= " << name << std::endl;
		return result << false;
	}
}

sf::Packet DatabaseManager::DeleteHero(std::string nick)
{
	sf::Packet result;
	std::string str = "DELETE FROM Hero WHERE nicknameC = '"+nick+"'";
	try {
		std::cout << str << std::endl;
		stmt->execute(str.c_str());
		std::cout << "Delete account:  nick= " << nick << std::endl;
		return result << true;
	}
	catch (sql::SQLException e) {
		std::cout << "CATCHED EXCEPTION    " << e.getErrorCode();
		std::cout << "Delete account failed:  nick= " << nick << std::endl;
		return result << false;
	}
}

sf::Packet DatabaseManager::CharList(std::string name)
{
	sf::Packet result;
	std::string str = "SELECT nicknameC FROM Hero WHERE accountC = '" + name + "'";
	sql::ResultSet *res;
	res = stmt->executeQuery(str.c_str());
	result << (int)res->rowsCount();
	while (res->next()) {
		result << res->getString("nicknameC");
		std::cout<< res->getString("nicknameC");
	}
	return result;
}

sf::Packet DatabaseManager::getCharLevel(std::string name)
{
	sf::Packet result;
	sql::ResultSet *res;
	std::string str = "SELECT levelC FROM Hero WHERE nicknameC = '" + name + "'";
	res = stmt->executeQuery(str.c_str());
	if (res->next()) {
		result << res->getInt("levelC");
	}
	std::cout << result << std::endl;
	return result;
}

sf::Packet DatabaseManager::getCharExp(std::string name)
{
	sf::Packet result;
	sql::ResultSet *res;
	std::string str = "SELECT expC FROM Hero WHERE nicknameC = '" + name + "'";
	res = stmt->executeQuery(str.c_str());
	if (res->next()) {
		result << res->getInt("expC");
	}
	std::cout << result << std::endl;
	return result;
}
