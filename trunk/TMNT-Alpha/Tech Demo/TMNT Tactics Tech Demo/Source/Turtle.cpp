////////////////////////////////////////////////////////
//   File Name	:	"Turtle.h"
//
//   Author		:	Matthew Di Matteo (MD)
//
//   Purpose	:	To provide a game object to be used by the player
//					as a part of their party in the game
////////////////////////////////////////////////////////

#include "Turtle.h"
#include "Animation.h"

CTurtle::CTurtle(void)
{
	m_ncurrFrame = 0;
}

CTurtle::~CTurtle(void)
{
}
void CTurtle::Update(float fElapsedTime)
{
	if(m_vAnimations[m_ncurrFrame].IsAnimationPlaying() == false)
	{
		m_vAnimations[m_ncurrFrame].Play();
	}
	m_vAnimations[m_ncurrFrame].Update(fElapsedTime);
}
void CTurtle::Render()
{
	//m_vAnimations[m_ncurrFrame].Render(50,50,0);
	m_vAnimations[m_ncurrFrame].Render((int)GetPosX()+m_vAnimations[0].GetFrames()[0].nAnchorX,(int)GetPosY()+m_vAnimations[0].GetFrames()[0].nAnchorY,0.5f);
}									

void CTurtle::SetAttributes(int ap,int hp,int strength,int defense,int accuracy,int speed,int level, int experience,int range)
{
	SetBaseAP(ap);
	SetCurrAP(ap);
	SetHealthMax(hp);
	SetHealth(hp);
	SetStrength(strength);
	SetDefense(defense);
	SetAccuracy(accuracy);
	SetSpeed(speed);
	SetLevel(level);
	SetExperience(experience);
	SetRange(range);
}