////////////////////////////////////////////////////////
//   File Name	:	"CPlayer.cpp"
//
//   Author		:	Matthew Di Matteo (MD)
//
//   Purpose	:	To provide a game object to be used as the player
//					being controlled by user during the game.
////////////////////////////////////////////////////////

#include "Player.h"

CPlayer::CPlayer(void)
{
}

CPlayer::~CPlayer(void)
{
}
CPlayer* CPlayer::GetInstance()
{
	static CPlayer instance;
	return &instance;
}
