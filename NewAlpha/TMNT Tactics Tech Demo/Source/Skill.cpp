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

#if Prototypes 
void RenderSwordSpin(CSkill* skill);
void UpdateSwordSpin(float elapsedTime, CSkill* skill);
void RenderFlipBackstab(CSkill* skill);
void UpdateFlipBackstab(float elapsedTime, CSkill* skill);
void RenderCounterAttack(CSkill* skill);
void UpdateCounterAttack(float elapsedTime, CSkill* skill);
void RenderKnockBack(CSkill* skill);
void UpdateKnockBack(float elapsedTime, CSkill* skill);
void RenderStaffSpin(CSkill* skill);
void UpdateStaffSpin(float elapsedTime, CSkill* skill);
void RenderStaffUppercut(CSkill* skill);
void UpdateStaffUppercut(float elapsedTime, CSkill* skill);
void RenderBackflipAway(CSkill* skill);
void UpdateBackflipAway(float elapsedTime, CSkill* skill);
void RenderCreateBomb(CSkill* skill);
void UpdateCreateBomb(float elapsedTime, CSkill* skill);
void RenderFlyingSaiStab(CSkill* skill);
void UpdateFlyingSaiStab(float elapsedTime, CSkill* skill);
void RenderSaiFury(CSkill* skill);
void UpdateSaiFury(float elapsedTime, CSkill* skill);
void RenderNunchuckSkullSplitter(CSkill* skill);
void UpdateNunchuckSkullSplitter(float elapsedTime, CSkill* skill);
void RenderNunchuckSpin(CSkill* skill);
void UpdateNunchuckSpin(float elapsedTime, CSkill* skill);
void RenderRollAway(CSkill* skill);
void UpdateRollAway(float elapsedTime, CSkill* skill);
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


void CSkill::Update(float fElapsedTime, CSkill* skill)
{
	m_pUpdatePtr(fElapsedTime,skill);
	m_fTimer += fElapsedTime;
	// TODO::add quick-time event code here
	if (m_fTimer > 5.0f)
		m_bComplete = true;
}

void CSkill::Render(CSkill* skill)
{
	m_pRenderPtr(skill);
}

//////////////////////////////////////////////////////////////////////////
//	SWORD SPIN
void RenderSwordSpin(CSkill* skill)
{
	
}

void UpdateSwordSpin( float elapsedTime, CSkill* skill )
{

}

//////////////////////////////////////////////////////////////////////////
//	SWORD JAB

void RenderSwordJab(CSkill* skill)
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

void RenderFlipBackstab(CSkill* skill)
{

}

void UpdateFlipBackstab( float elapsedTime, CSkill* skill )
{

}

//////////////////////////////////////////////////////////////////////////
//	COUNTER-ATTACK

void RenderCounterAttack(CSkill* skill)
{

}

void UpdateCounterAttack( float elapsedTime, CSkill* skill )
{

}

//////////////////////////////////////////////////////////////////////////
//	KNOCK BACK

void RenderKnockBack(CSkill* skill)
{

}

void UpdateKnockBack( float elapsedTime, CSkill* skill )
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

void RenderStaffSpin(CSkill* skill)
{

}

void UpdateStaffSpin( float elapsedTime, CSkill* skill )
{

}

//////////////////////////////////////////////////////////////////////////
//	STAFF UPPERCUT

void RenderStaffUppercut(CSkill* skill)
{

}

void UpdateStaffUppercut( float elapsedTime, CSkill* skill )
{

}

//////////////////////////////////////////////////////////////////////////
//	BACK FLIP AWAY

void RenderBackflipAway(CSkill* skill)
{

}

void UpdateBackflipAway( float elapsedTime, CSkill* skill )
{

}

//////////////////////////////////////////////////////////////////////////
//	CREATE BOMB

void RenderCreateBomb(CSkill* skill)
{

}

void UpdateCreateBomb( float elapsedTime, CSkill* skill )
{

}

//////////////////////////////////////////////////////////////////////////
//	SAI STAB

void RenderFlyingSaiStab(CSkill* skill)
{

}

void UpdateFlyingSaiStab( float elapsedTime, CSkill* skill )
{

}

//////////////////////////////////////////////////////////////////////////
//	SAI FURY

void RenderSaiFury(CSkill* skill)
{

}

void UpdateSaiFury( float elapsedTime, CSkill* skill )
{

}

//////////////////////////////////////////////////////////////////////////
//	SKULL SPLITTER

void RenderNunchuckSkullSplitter(CSkill* skill)
{

}

void UpdateNunchuckSkullSplitter( float elapsedTime, CSkill* skill )
{

}

//////////////////////////////////////////////////////////////////////////
//	NUNCHUCK SPIN

void RenderNunchuckSpin(CSkill* skill)
{

}

void UpdateNunchuckSpin( float elapsedTime, CSkill* skill )
{

}

//////////////////////////////////////////////////////////////////////////
//	ROLL AWAY

void RenderRollAway(CSkill* skill)
{

}

void UpdateRollAway( float elapsedTime, CSkill* skill )
{

}

//////////////////////////////////////////////////////////////////////////