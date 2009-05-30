////////////////////////////////////////////////////////
//   File Name	:	"CPlayer.cpp"
//
//   Author		:	Matthew Di Matteo (MD)
//
//   Purpose	:	To provide a game object to be used as the player
//					being controlled by user during the game.
////////////////////////////////////////////////////////

#include "Player.h"
#include "Turtle.h"
#include "Factory.h"

CPlayer::CPlayer(void)
{
	m_pLeonardo = Factory::CreateTurtle();
	m_pDonatello = Factory::CreateTurtle();
	m_pRaphael = Factory::CreateTurtle();
	m_pMikey = Factory::CreateTurtle();

}

CPlayer::~CPlayer(void)
{
}
CPlayer* CPlayer::GetInstance()
{
	static CPlayer instance;
	return &instance;
}
