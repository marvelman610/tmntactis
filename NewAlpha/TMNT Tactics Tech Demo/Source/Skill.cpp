////////////////////////////////////////////////////////
//   File Name	:	"Skill.h"
//
//   Author		:	Matthew Di Matteo (MD), Ramon Johannessen (RJ)
//
//   Purpose	:	To provide the player with a skill 
//					to deal more damage to the opponent
////////////////////////////////////////////////////////
#include "Skill.h"
#include "BattleMap.h"
#include "Player.h"

// string CSkill::m_strName = "";
// float CSkill::m_fTimer = 0.0f;
// int CSkill::m_nType = NULL;
// int CSkill::m_nDamage = NULL;
// int CSkill::m_nRange = NULL;
// int CSkill::m_nSkillCost = NULL;
// int CSkill::m_nCurrAmountSuccessful = -1;
// int CSkill::m_nMaxCombinationAmount = NULL;
// bool CSkill::m_bComplete = false;
// CPlayer* CSkill::m_pPlayer = NULL;
// CBattleMap* CSkill::m_pBattleMap = NULL;
// prototypes for all types of skills' renders and updates
#define Prototypes 1
typedef CSkill cs;

#if Prototypes 
void RenderSwordSpin();
void UpdateSwordSpin(float elapsedTime);
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
	m_pPlayer = CPlayer::GetInstance();
	m_pBattleMap = CBattleMap::GetInstance();
	m_fTimer = 0.0f;
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
//  		m_pRenderPtr = &CSkill::RenderSwordJab;
//  		m_pUpdatePtr = &CSkill::UpdateSwordJab;
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


void CSkill::Update(float fElapsedTime)
{
	m_pUpdatePtr(fElapsedTime);
	m_fTimer += fElapsedTime;
	// TODO::add quick-time event code here
	if (m_fTimer > 5.0f)
		m_bComplete = true;
}

void CSkill::Render()
{
	m_pRenderPtr();
}

//////////////////////////////////////////////////////////////////////////
//	SWORD SPIN
void RenderSwordSpin()
{
	
}

void UpdateSwordSpin( float elapsedTime )
{

}

//////////////////////////////////////////////////////////////////////////
//	SWORD JAB

void RenderSwordJab()
{
	
}

// void CSkill::UpdateSwordJab( float elapsedTime )
// {
// 	CBase* target  = cs::GetBattleMap()->GetCurrEnemyTarget();
// 	CPlayer* player= cs::GetPlayer();
// 	CBattleMap* bm = cs::GetBattleMap();
// 	CBase* character=&bm->GetChars()[bm->GetCurrActive()];
// 
// 	int damage = character->GetStrength() - target->GetDefense() + cs::GetDmg() + character->GetAccuracy();
// 
// 	target->SetHealth(target->GetHealth() - damage);
// }

//////////////////////////////////////////////////////////////////////////
//	SFLIP BACKSTAB

void RenderFlipBackstab()
{

}

void UpdateFlipBackstab( float elapsedTime )
{

}

//////////////////////////////////////////////////////////////////////////
//	COUNTER-ATTACK

void RenderCounterAttack()
{

}

void UpdateCounterAttack( float elapsedTime )
{

}

//////////////////////////////////////////////////////////////////////////
//	KNOCK BACK

void RenderKnockBack()
{

}

void UpdateKnockBack( float elapsedTime )
{
// 	CBase* target  = cs::GetBattleMap()->GetCurrEnemyTarget();
// 	CPlayer* player= cs::GetPlayer();
// 	CBattleMap* bm = cs::GetBattleMap();
// 	CBase* character=&bm->GetChars()[bm->GetCurrActive()];
// 
// 	int damage = character->GetStrength() - target->GetDefense() + cs::GetDmg() + character->GetAccuracy();
// 
// 	target->SetHealth(target->GetHealth() - damage);
}

//////////////////////////////////////////////////////////////////////////
//	STAFF SPIN

void RenderStaffSpin()
{

}

void UpdateStaffSpin( float elapsedTime )
{

}

//////////////////////////////////////////////////////////////////////////
//	STAFF UPPERCUT

void RenderStaffUppercut()
{

}

void UpdateStaffUppercut( float elapsedTime )
{

}

//////////////////////////////////////////////////////////////////////////
//	BACK FLIP AWAY

void RenderBackflipAway()
{

}

void UpdateBackflipAway( float elapsedTime )
{

}

//////////////////////////////////////////////////////////////////////////
//	CREATE BOMB

void RenderCreateBomb()
{

}

void UpdateCreateBomb( float elapsedTime )
{

}

//////////////////////////////////////////////////////////////////////////
//	SAI STAB

void RenderFlyingSaiStab()
{

}

void UpdateFlyingSaiStab( float elapsedTime )
{

}

//////////////////////////////////////////////////////////////////////////
//	SAI FURY

void RenderSaiFury()
{

}

void UpdateSaiFury( float elapsedTime )
{

}

//////////////////////////////////////////////////////////////////////////
//	SKULL SPLITTER

void RenderNunchuckSkullSplitter()
{

}

void UpdateNunchuckSkullSplitter( float elapsedTime )
{

}

//////////////////////////////////////////////////////////////////////////
//	NUNCHUCK SPIN

void RenderNunchuckSpin()
{

}

void UpdateNunchuckSpin( float elapsedTime )
{

}

//////////////////////////////////////////////////////////////////////////
//	ROLL AWAY

void RenderRollAway()
{

}

void UpdateRollAway( float elapsedTime )
{

}

//////////////////////////////////////////////////////////////////////////