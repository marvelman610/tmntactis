////////////////////////////////////////////////////////
//   File Name	:	"Skill.h"
//
//   Author		:	Matthew Di Matteo (MD), Ramon Johannessen (RJ)
//
//   Purpose	:	To provide the player with a skill 
//					to deal more damage to the opponent
////////////////////////////////////////////////////////
#include "Skill.h"
// prototypes for all types of skills' renders and updates
#define Prototypes 1
#if Prototypes 
void RenderSwordSpin();
void UpdateSwordSpin(float elapsedTime);
void RenderSwordJab();
void UpdateSwordJab(float elapsedTime);
void RenderFlipBackstab();
void UpdateFlipBackstab(float elapsedTime);
void RenderCounterAttack();
void UpdateCounterAttack(float elapsedTime);
void RenderKnockBack();
void UpdateKnockBack(float elapsedTime);
void RenderStaffSpin();
void UpdateStaffSpin(float elapsedTime);
void RenderStaffUppercut();
void UpdateStaffUppercut(float elapsedTime);
void RenderBackflipAway();
void UpdateBackflipAway(float elapsedTime);
void RenderCreateBomb();
void UpdateCreateBomb(float elapsedTime);
void RenderFlyingSaiStab();
void UpdateFlyingSaiStab(float elapsedTime);
void RenderSaiFury();
void UpdateSaiFury(float elapsedTime);
void RenderNunchuckSkullSplitter();
void UpdateNunchuckSkullSplitter(float elapsedTime);
void RenderNunchuckSpin();
void UpdateNunchuckSpin(float elapsedTime);
void RenderRollAway();
void UpdateRollAway(float elapsedTime);
#endif

CSkill::~CSkill() {}
CSkill::CSkill() {}

CSkill::CSkill(string name, int type, int skillID, int dmg, int range, int cost, int combAmt)
{
	m_strName = name;
	m_nType = type;
	m_nDamage = dmg;
	m_nRange = range;
	m_nSkillCost = cost;
	m_nCurrAmountSuccessful = -1;
	m_nMaxCombinationAmount = combAmt;
	SetFunctions(skillID);
}

void CSkill::SetFunctions(int skillID)
{
	switch (skillID)
	{
	case SWORD_SPIN:
		m_pRenderPtr = RenderSwordSpin;
		m_pUpdatePtr = UpdateSwordSpin;
		break;
	case SWORD_JAB:
		m_pRenderPtr = RenderSwordJab;
		m_pUpdatePtr = UpdateSwordJab;
		break;
	case FLIP_BACKSTAB:
		m_pRenderPtr = RenderFlipBackstab;
		m_pUpdatePtr = UpdateFlipBackstab;
		break;
	case COUNTER_ATTACK:
		m_pRenderPtr = RenderCounterAttack;
		m_pUpdatePtr = UpdateCounterAttack;
		break;
	case KNOCK_BACK:
		m_pRenderPtr = RenderKnockBack;
		m_pUpdatePtr = UpdateKnockBack;
		break;
	case STAFF_SPIN:
		m_pRenderPtr = RenderStaffSpin;
		m_pUpdatePtr = UpdateStaffSpin;
		break;
	case STAFF_UPPERCUT:
		m_pRenderPtr = RenderStaffUppercut;
		m_pUpdatePtr = UpdateStaffUppercut;
		break;
	case BACKFLIP_AWAY:
		m_pRenderPtr = RenderBackflipAway;
		m_pUpdatePtr = UpdateBackflipAway;
		break;
	case CREATE_BOMB:
		m_pRenderPtr = RenderCreateBomb;
		m_pUpdatePtr = UpdateCreateBomb;
		break;
	case FLYING_SAI_STAB:
		m_pRenderPtr = RenderFlyingSaiStab;
		m_pUpdatePtr = UpdateFlyingSaiStab;
		break;
	case SAI_FURY:
		m_pRenderPtr = RenderSaiFury;
		m_pUpdatePtr = UpdateSaiFury;
		break;
	case NUNCHUCK_SKULL_SPLITTER:
		m_pRenderPtr = RenderNunchuckSkullSplitter;
		m_pUpdatePtr = UpdateNunchuckSkullSplitter;
		break;
	case NUNCHUCK_SPIN:
		m_pRenderPtr = RenderNunchuckSpin;
		m_pUpdatePtr = UpdateNunchuckSpin;
		break;
	case ROLL_AWAY:
		m_pRenderPtr = RenderRollAway;
		m_pUpdatePtr = UpdateRollAway;
		break;
	}
}

void RenderSwordSpin()
{

}

void UpdateSwordSpin( float elapsedTime )
{

}

void RenderSwordJab()
{

}

void UpdateSwordJab( float elapsedTime )
{

}

void RenderFlipBackstab()
{

}

void UpdateFlipBackstab( float elapsedTime )
{

}

void RenderCounterAttack()
{

}

void UpdateCounterAttack( float elapsedTime )
{

}

void RenderKnockBack()
{

}

void UpdateKnockBack( float elapsedTime )
{

}

void RenderStaffSpin()
{

}

void UpdateStaffSpin( float elapsedTime )
{

}

void RenderStaffUppercut()
{

}

void UpdateStaffUppercut( float elapsedTime )
{

}

void RenderBackflipAway()
{

}

void UpdateBackflipAway( float elapsedTime )
{

}

void RenderCreateBomb()
{

}

void UpdateCreateBomb( float elapsedTime )
{

}

void RenderFlyingSaiStab()
{

}

void UpdateFlyingSaiStab( float elapsedTime )
{

}

void RenderSaiFury()
{

}

void UpdateSaiFury( float elapsedTime )
{

}

void RenderNunchuckSkullSplitter()
{

}

void UpdateNunchuckSkullSplitter( float elapsedTime )
{

}

void RenderNunchuckSpin()
{

}

void UpdateNunchuckSpin( float elapsedTime )
{

}

void RenderRollAway()
{

}

void UpdateRollAway( float elapsedTime )
{

}