#pragma once
enum class PacketType
{
	LOGIN_PACKET,
	GAME_PACKET,  
	CHAR_KNOWS_SKILL_PACKET,  
	CHAR_INFO_PACKET,  //not implemented
	CHAR_POINTS_AVAL_PACKET,   
	NEW_ACC_PACKET,   
	NEW_CHAR_PACKET,
	NEW_FIGHT_PACKET,
	BUY_SKILL_PACKET,
	LEVEL_UP_PACKET, //not implemented
	ADD_EXP_PACKET, //not implemented
	DELETE_ACC_PACKET, //not implemented
	DELETE_CHAR_PACKET //not implemented
};