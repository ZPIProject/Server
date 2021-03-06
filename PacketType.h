#pragma once
enum class PacketType
{
	LOGIN_PACKET,//0
	GAME_PACKET,  //1
	CHAR_KNOWS_SKILL_PACKET, //2 
	CHAR_INFO_PACKET,  //not implemented
	CHAR_POINTS_AVAL_PACKET, //4
	NEW_ACC_PACKET,   //5
	NEW_CHAR_PACKET,//6
	NEW_FIGHT_PACKET,//7
	BUY_SKILL_PACKET,//8
	LEVEL_UP_PACKET,//9
	ADD_EXP_PACKET, //10
	DELETE_ACC_PACKET, //11
	DELETE_CHAR_PACKET,//12
	CHAR_LIST,//13
	GET_CHAR_LVL,//14
	GET_CHAR_EXP//15
};