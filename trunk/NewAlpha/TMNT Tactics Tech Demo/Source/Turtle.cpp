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
	m_nType = OBJECT_TURTLE;
	m_nCurrSelectedSkill = -1;

	SetWidth(64);
	SetHeight(64);
}

CTurtle::~CTurtle(void)
{
	m_vActiveSkills.clear();
	m_vInactiveSkills.clear();
}
void CTurtle::Update(float fElapsedTime)
{
	if(m_vAnimations[m_nCurrAnimation].IsAnimationPlaying() == false)
	{
		m_vAnimations[m_nCurrAnimation].Play();
	}
	m_vAnimations[m_nCurrAnimation].Update(fElapsedTime);
	if(GetExperience() >= (100 * GetLevel()))
	{
		SetExperience(GetExperience()-(100* GetLevel()));
		SetLevel(GetLevel() + 1);
		SetHealthMax((int)((float)GetMaxHealth() * 1.25f));
		SetBaseAP(GetBaseAP()+2);
		SetStrength( (int)( (float)GetStrength() * 1.2f ) );
		SetDefense( (int) ( (float)GetDefense() * 1.2f ) );
		SetAccuracy( (int) ( (float)GetAccuracy() * 1.2f ) );
		SetSpeed( (int) ( (float)GetSpeed() * 1.2f ) );
	}
}
void CTurtle::Render()
{
	m_vAnimations[m_nCurrAnimation].Render((int)GetPosX(),(int)GetPosY(), GetPosZ(), 1.0f, false, m_dwColor);
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
