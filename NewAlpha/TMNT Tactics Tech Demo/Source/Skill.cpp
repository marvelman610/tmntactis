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
#include "ParticleSystem.h"

// prototypes for all types of skills' renders and updates
#define Prototypes 1

#if Prototypes 
void RenderSwordJab(CSkill* skill, CParticleSystem* ps);
void UpdateSwordJab( float elapsedTime, CSkill* skill, CParticleSystem* ps );
void RenderSwordSpin(CSkill* skill, CParticleSystem* ps);
void UpdateSwordSpin(float elapsedTime, CSkill* skill, CParticleSystem* ps);
void RenderFlipBackstab(CSkill* skill, CParticleSystem* ps);
void UpdateFlipBackstab(float elapsedTime, CSkill* skill, CParticleSystem* ps);
void RenderCounterAttack(CSkill* skill, CParticleSystem* ps);
void UpdateCounterAttack(float elapsedTime, CSkill* skill, CParticleSystem* ps);
void RenderKnockBack(CSkill* skill, CParticleSystem* ps);
void UpdateKnockBack(float elapsedTime, CSkill* skill, CParticleSystem* ps);
void RenderStaffSpin(CSkill* skill, CParticleSystem* ps);
void UpdateStaffSpin(float elapsedTime, CSkill* skill, CParticleSystem* ps);
void RenderStaffUppercut(CSkill* skill, CParticleSystem* ps);
void UpdateStaffUppercut(float elapsedTime, CSkill* skill, CParticleSystem* ps);
void RenderBackflipAway(CSkill* skill, CParticleSystem* ps);
void UpdateBackflipAway(float elapsedTime, CSkill* skill, CParticleSystem* ps);
void RenderCreateBomb(CSkill* skill, CParticleSystem* ps);
void UpdateCreateBomb(float elapsedTime, CSkill* skill, CParticleSystem* ps);
void RenderFlyingSaiStab(CSkill* skill, CParticleSystem* ps);
void UpdateFlyingSaiStab(float elapsedTime, CSkill* skill, CParticleSystem* ps);
void RenderSaiFury(CSkill* skill, CParticleSystem* ps);
void UpdateSaiFury(float elapsedTime, CSkill* skill, CParticleSystem* ps);
void RenderNunchuckSkullSplitter(CSkill* skill, CParticleSystem* ps);
void UpdateNunchuckSkullSplitter(float elapsedTime, CSkill* skill, CParticleSystem* ps);
void RenderNunchuckSpin(CSkill* skill, CParticleSystem* ps);
void UpdateNunchuckSpin(float elapsedTime, CSkill* skill, CParticleSystem* ps);
void RenderRollAway(CSkill* skill, CParticleSystem* ps);
void UpdateRollAway(float elapsedTime, CSkill* skill, CParticleSystem* ps);
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
	m_pPlayer		= CPlayer::GetInstance();
	m_pBattleMap	= CBattleMap::GetInstance();
	
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


void CSkill::Update(float fElapsedTime, CSkill* skill, CParticleSystem* ps)
{
	m_pUpdatePtr(fElapsedTime,skill, ps);
	m_fTimer += fElapsedTime;
	// TODO::add quick-time event code here

	// if the attack is done, check if the enemy target is dead
	// if so...remove and gain experience and skill
	if (m_fTimer > 5.0f)
	{
		m_bComplete = true; 
		m_fTimer = 0.0f;
	}
	m_pRenderPtr(skill, ps);
}

CBase* CSkill::Attack(CBase* target)
{
	CPlayer* pPlayer		= CPlayer::GetInstance();
	CBattleMap* pBattleMap	= CBattleMap::GetInstance();
	CBase* character		= pBattleMap->GetCurrChar();

	int damage = (int)((float)(character->GetStrength() - target->GetDefense() + GetDmg() + character->GetAccuracy()) * 1.5f);
	target->SetHealth(target->GetHealth() - damage);
	return target;
}

// void CSkill::Render(CSkill* skill, CParticleSystem* ps)
// {
// 	m_pRenderPtr(skill, ps);
// }

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
//	SWORD SPIN

void RenderSwordSpin(CSkill* skill, CParticleSystem* ps)
{
	
}

void UpdateSwordSpin( float elapsedTime, CSkill* skill, CParticleSystem* ps )
{

}

//////////////////////////////////////////////////////////////////////////
//	SWORD JAB

void RenderSwordJab(CSkill* skill, CParticleSystem* ps)
{
	
}

void UpdateSwordJab( float elapsedTime, CSkill* skill, CParticleSystem* ps )
{
}

//////////////////////////////////////////////////////////////////////////
//	SFLIP BACKSTAB

void RenderFlipBackstab(CSkill* skill, CParticleSystem* ps)
{

}

void UpdateFlipBackstab( float elapsedTime, CSkill* skill, CParticleSystem* ps )
{

}

//////////////////////////////////////////////////////////////////////////
//	COUNTER-ATTACK

void RenderCounterAttack(CSkill* skill, CParticleSystem* ps)
{

}

void UpdateCounterAttack( float elapsedTime, CSkill* skill, CParticleSystem* ps )
{

}

//////////////////////////////////////////////////////////////////////////
//	KNOCK BACK

void RenderKnockBack(CSkill* skill, CParticleSystem* ps)
{

}

void UpdateKnockBack( float elapsedTime, CSkill* skill, CParticleSystem* ps )
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

void RenderStaffSpin(CSkill* skill, CParticleSystem* ps)
{

}

void UpdateStaffSpin( float elapsedTime, CSkill* skill, CParticleSystem* ps )
{

}

//////////////////////////////////////////////////////////////////////////
//	STAFF UPPERCUT

void RenderStaffUppercut(CSkill* skill, CParticleSystem* ps)
{

}

void UpdateStaffUppercut( float elapsedTime, CSkill* skill, CParticleSystem* ps )
{

}

//////////////////////////////////////////////////////////////////////////
//	BACK FLIP AWAY

void RenderBackflipAway(CSkill* skill, CParticleSystem* ps)
{

}

void UpdateBackflipAway( float elapsedTime, CSkill* skill, CParticleSystem* ps )
{

}

//////////////////////////////////////////////////////////////////////////
//	CREATE BOMB

void RenderCreateBomb(CSkill* skill, CParticleSystem* ps)
{

}

void UpdateCreateBomb( float elapsedTime, CSkill* skill, CParticleSystem* ps )
{

}

//////////////////////////////////////////////////////////////////////////
//	SAI STAB

void RenderFlyingSaiStab(CSkill* skill, CParticleSystem* ps)
{

}

void UpdateFlyingSaiStab( float elapsedTime, CSkill* skill, CParticleSystem* ps )
{

}

//////////////////////////////////////////////////////////////////////////
//	SAI FURY

void RenderSaiFury(CSkill* skill, CParticleSystem* ps)
{

}

void UpdateSaiFury( float elapsedTime, CSkill* skill, CParticleSystem* ps )
{

}

//////////////////////////////////////////////////////////////////////////
//	SKULL SPLITTER

void RenderNunchuckSkullSplitter(CSkill* skill, CParticleSystem* ps)
{

}

void UpdateNunchuckSkullSplitter( float elapsedTime, CSkill* skill, CParticleSystem* ps )
{

}

//////////////////////////////////////////////////////////////////////////
//	NUNCHUCK SPIN

void RenderNunchuckSpin(CSkill* skill, CParticleSystem* ps)
{

}

void UpdateNunchuckSpin( float elapsedTime, CSkill* skill, CParticleSystem* ps )
{

}

//////////////////////////////////////////////////////////////////////////
//	ROLL AWAY

void RenderRollAway(CSkill* skill, CParticleSystem* ps)
{

}

void UpdateRollAway( float elapsedTime, CSkill* skill, CParticleSystem* ps )
{

}

//////////////////////////////////////////////////////////////////////////