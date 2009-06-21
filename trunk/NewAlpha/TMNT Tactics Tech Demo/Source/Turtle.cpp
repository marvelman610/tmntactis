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
#include "BattleMap.h"
#include "Assets.h"

CTurtle::CTurtle(void)
{
	m_nType = OBJECT_TURTLE;
	m_nCurrSelectedSkill = -1;
	m_bIsAlive = true;
	SetVelX(100.0f);
	SetVelY(50.0f);
	SetWidth(50);
	SetHeight(80);
	m_nCurrWeapon = 0;
	m_pOwner = NULL;
	m_nNumType = -1;
}

CTurtle::~CTurtle(void)
{
	m_vActiveSkills.clear();
	m_vInactiveSkills.clear();
}
void CTurtle::Update(float fElapsedTime)
{
	CBase::Update(fElapsedTime);
	m_vAnimations[m_nCurrAnimation].Update(fElapsedTime);
	if(GetExperience() >= (100 * GetLevel()))
	{
		CBattleMap::GetInstance()->PlaySFX(CAssets::GetInstance()->aBMcowabungaSnd);
		SetExperience(0/*GetExperience()-(100* GetLevel())*/);
		SetLevel(GetLevel() + 1);
		SetHealthMax((int)((float)GetMaxHealth() * 1.25f));
		SetHealth((int)((float)GetMaxHealth()));
		SetBaseAP(GetBaseAP()+2);
		SetStrength( (int)( (float)GetStrength() * 1.2f ) );
		SetDefense( (int) ( (float)GetDefense() * 1.2f ) );
		SetAccuracy( (int) ( (float)GetAccuracy() * 1.2f ) );
		SetSpeed( (int) ( (float)GetSpeed() * 1.2f ) );
	}
	if( GetHealth() <= 0)
	{
		if(GetAlive() == true)
		{
			CBattleMap::GetInstance()->DecrementNumChars();
			CBattleMap::GetInstance()->DecrementNumTurtles();
		}
		SetAlive(false);
	}
}
void CTurtle::Render()
{
	if (!m_bIsAlive)
		SetCurrAnim(9);
	m_vAnimations[m_nCurrAnimation].Render((int)GetPosX()+m_vAnimations[0].GetFrames()[0].nAnchorX,(int)GetPosY()+m_vAnimations[0].GetFrames()[0].nAnchorY, GetPosZ(), 1.0f, m_dwColor);

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
