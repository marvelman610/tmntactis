////////////////////////////////////////////////////////
//   File Name	:	"Turtle.h"
//
//   Author		:	Matthew Di Matteo (MD)
//
//   Purpose	:	To provide a game object to be used by the player
//					as a part of their party in the game
////////////////////////////////////////////////////////

#include "Turtle.h"

CTurtle::CTurtle(void)
{
}

CTurtle::~CTurtle(void)
{
}
void CTurtle::Update(float fElapsedTime)
{
}
void CTurtle::Render()
{

}
void CTurtle::SetAttribute(int ap,int hp,int strength,int defense,int accuracy,int speed,int level, int experience,int range)
{
	SetBaseAP(ap);
	SetHealthMax(hp);
	SetStrength(strength);
	SetDefense(defense);
	SetAccuracy(accuracy);
	SetSpeed(speed);
	SetLevel(level);
	SetExperience(range);
}