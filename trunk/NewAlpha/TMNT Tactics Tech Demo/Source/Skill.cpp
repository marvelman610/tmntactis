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
#include "Assets.h"
#include "CSGD_DirectInput.h"
#include "CSGD_Direct3D.h"
#include "CSGD_TextureManager.h"
#include "Assets.h"
#include "Box.h"

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

#define QTEX 500
#define QTEY 300

CSkill::~CSkill() 
{
	m_pPlayer		= NULL;
	m_pBattleMap	= NULL;
	m_pDI			= NULL;
	m_pD3D			= NULL;
	m_pTM			= NULL;
	m_pCombination = NULL;
}
CSkill::CSkill() {}

CSkill::CSkill(string name, int type, int skillID, int dmg, int range, int cost, int combAmt, float duration)
{
	m_strName = name;
	m_nType = type;
	m_nDamage = dmg;
	m_nRange = range;
	m_nSkillCost = cost;
	m_nCurrAmountSuccessful = 0;
	m_nMaxCombinationAmount = combAmt;
	m_pPlayer		= CPlayer::GetInstance();
	m_pBattleMap	= CBattleMap::GetInstance();
	m_pDI			= CSGD_DirectInput::GetInstance();
	m_pD3D			= CSGD_Direct3D::GetInstance();
	m_pTM			= CSGD_TextureManager::GetInstance();
	
	m_fTimer = 0.0f;
	m_fDuration = duration;
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
	m_pUpdatePtr(fElapsedTime, skill, ps);
	m_fTimer += fElapsedTime;
	
	// there is still time to input a direction
	if (m_fTimer <= (float)m_nMaxCombinationAmount)
	{
		//m_pD3D->Clear(255, 255, 255);
		m_pD3D->DeviceBegin();
		m_pD3D->SpriteBegin();

		switch (skill->GetComb()[skill->GetCurrAmtSuccessful()])
		{
		case UP:
			{
				m_pTM->DrawWithZSort(CAssets::GetInstance()->aBMqteUpID, QTEX, QTEY, 0.0f, 1.5f, 1.5f);
				if (m_pDI->KeyPressed(DIK_UP))
				{
					skill->SetCurrAmtSuccessful(skill->GetCurrAmtSuccessful()+1);
				}
				else if (m_pDI->KeyPressed(DIK_DOWN) || m_pDI->KeyPressed(DIK_LEFT) || m_pDI->KeyPressed(DIK_RIGHT))
					m_bComplete = true;
			}
			break;
		case DOWN:
			{
				m_pTM->DrawWithZSort(CAssets::GetInstance()->aBMqteDownID, QTEX, QTEY, 0.0f, 1.5f, 1.5f);
				if (m_pDI->KeyPressed(DIK_DOWN))
				{
					skill->SetCurrAmtSuccessful(skill->GetCurrAmtSuccessful()+1);
				}
				else if (m_pDI->KeyPressed(DIK_UP) || m_pDI->KeyPressed(DIK_LEFT) || m_pDI->KeyPressed(DIK_RIGHT))
					m_bComplete = true;
			}
			break;
		case LEFT:
			{
				m_pTM->DrawWithZSort(CAssets::GetInstance()->aBMqteLeftID, QTEX, QTEY, 0.0f, 1.5f, 1.5f);
				if (m_pDI->KeyPressed(DIK_LEFT))
				{
					skill->SetCurrAmtSuccessful(skill->GetCurrAmtSuccessful()+1);
				}
				else if (m_pDI->KeyPressed(DIK_UP) || m_pDI->KeyPressed(DIK_DOWN) || m_pDI->KeyPressed(DIK_RIGHT))
					m_bComplete = true;
			}
			break;
		case RIGHT:
			{
				m_pTM->DrawWithZSort(CAssets::GetInstance()->aBMqteRightID, QTEX, QTEY, 0.0f, 1.5f, 1.5f);
				if (m_pDI->KeyPressed(DIK_RIGHT))
				{
					skill->SetCurrAmtSuccessful(skill->GetCurrAmtSuccessful()+1);
				}
				else if (m_pDI->KeyPressed(DIK_UP) || m_pDI->KeyPressed(DIK_LEFT) || m_pDI->KeyPressed(DIK_DOWN))
					m_bComplete = true;
			}
			break;
		}

		m_pD3D->SpriteEnd();
		m_pD3D->DeviceEnd();
		m_pD3D->Present();
	}

	// if the attack is done, set stats and gain experience and skill
	// if target is dead...remove 
	if (m_fTimer > (float)m_nMaxCombinationAmount)
	{
		m_bComplete = true; 
		m_pUpdatePtr(fElapsedTime, skill, ps);
		m_fTimer = 0.0f;
	}

	// Renders the current skill's particles
	m_pRenderPtr(skill, ps);
}

void CSkill::SetComb(int num)
{
	m_pCombination = new int[num];
	for (int i = 0; i < num; ++i)
		m_pCombination[i] = rand() % 4;	// set to a random direction ( one of four)
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
//	SWORD SPIN

void RenderSwordSpin(CSkill* skill, CParticleSystem* ps)
{
	
}

void UpdateSwordSpin( float elapsedTime, CSkill* skill, CParticleSystem* ps )
{
	CPlayer* pPlayer		= CPlayer::GetInstance();
	CBattleMap* pBattleMap	= CBattleMap::GetInstance();
	CBase* character		= pBattleMap->GetCurrChar();
	CBase* target			= pBattleMap->GetCurrEnemyTarget();
	// init particles...
	if (skill->GetTimer() == 0.0f)
	{
		ps[BLOOD].Emit(target->GetPosX(), target->GetPosY());
		ps[BLOOD].m_bActive = true; ps[BLOOD].m_bLoop = false;
		skill->SetComb(skill->GetMaxCombAmt());

	}
	if (skill->IsComplete())
	{
		// FORMULA: str - def + dmg + accuracy * 1.5 + random(-5 to 5) + (QTE amt successful * 5)
		int damage = (int)((float)(character->GetStrength() - target->GetDefense() + 
			skill->GetDmg() + character->GetAccuracy()) * 1.5f + rand() % (5 + 4) - 4) + (skill->GetCurrAmtSuccessful() * 5);
		target->SetHealth(target->GetHealth() - damage);
		ps[BLOOD].m_bActive = false;
		skill->ClearComb();
		skill->SetCurrAmtSuccessful(0);
	}
	ps[BLOOD].UpdateParticle(elapsedTime);
	ps[BLOOD].DrawParticle(CAssets::GetInstance()->aBloodParticle);
}

//////////////////////////////////////////////////////////////////////////
//	SWORD JAB

void RenderSwordJab(CSkill* skill, CParticleSystem* ps)
{
	
}

void UpdateSwordJab( float elapsedTime, CSkill* skill, CParticleSystem* ps )
{
	CPlayer* pPlayer		= CPlayer::GetInstance();
	CBattleMap* pBattleMap	= CBattleMap::GetInstance();
	CBase* character		= pBattleMap->GetCurrChar();
	CBase* target			= pBattleMap->GetCurrEnemyTarget();
	// init particles...
	if (skill->GetTimer() == 0.0f)
	{
		ps[BLOOD].Emit(target->GetPosX(), target->GetPosY());
		ps[BLOOD].m_bActive = true; ps[BLOOD].m_bLoop = false;
		skill->SetComb(skill->GetMaxCombAmt());

	}
	if (skill->IsComplete())
	{
		// FORMULA: str - def + dmg + accuracy * 1.5 + random(-5 to 5) + (QTE amt successful * 5)
		if (skill->GetCurrAmtSuccessful() > 0)
		{
			int damage = (int)((float)(character->GetStrength() - target->GetDefense() + 
				skill->GetDmg() + character->GetAccuracy()) * 1.5f + rand() % (5 + 4) - 4) + (skill->GetCurrAmtSuccessful() * 5);
			target->SetHealth(target->GetHealth() - damage);
		}
		ps[BLOOD].m_bActive = false;
		skill->ClearComb();
		skill->SetCurrAmtSuccessful(0);
	}
	ps[BLOOD].UpdateParticle(elapsedTime);
	ps[BLOOD].DrawParticle(CAssets::GetInstance()->aBloodParticle);
}

//////////////////////////////////////////////////////////////////////////
//	SFLIP BACKSTAB

void RenderFlipBackstab(CSkill* skill, CParticleSystem* ps)
{

}

void UpdateFlipBackstab( float elapsedTime, CSkill* skill, CParticleSystem* ps )
{
	CPlayer* pPlayer		= CPlayer::GetInstance();
	CBattleMap* pBattleMap	= CBattleMap::GetInstance();
	CBase* character		= pBattleMap->GetCurrChar();
	CBase* target			= pBattleMap->GetCurrEnemyTarget();
	// init particles...
	if (skill->GetTimer() == 0.0f)
	{
		ps[BLOOD].Emit(target->GetPosX(), target->GetPosY());
		ps[BLOOD].m_bActive = true; ps[BLOOD].m_bLoop = false;
		skill->SetComb(skill->GetMaxCombAmt());

	}
	if (skill->IsComplete())
	{
		// FORMULA: str - def + dmg + accuracy * 1.5 + random(-5 to 5) + (QTE amt successful * 5)
		if (skill->GetCurrAmtSuccessful() > 0)
		{
			int damage = (int)((float)(character->GetStrength() - target->GetDefense() + 
				skill->GetDmg() + character->GetAccuracy()) * 1.5f + rand() % (5 + 4) - 4) + (skill->GetCurrAmtSuccessful() * 5);
			target->SetHealth(target->GetHealth() - damage);
		}
		ps[BLOOD].m_bActive = false;
		skill->ClearComb();
		skill->SetCurrAmtSuccessful(0);
	}
	ps[BLOOD].UpdateParticle(elapsedTime);
	ps[BLOOD].DrawParticle(CAssets::GetInstance()->aBloodParticle);

}

//////////////////////////////////////////////////////////////////////////
//	COUNTER-ATTACK

void RenderCounterAttack(CSkill* skill, CParticleSystem* ps)
{

}

void UpdateCounterAttack( float elapsedTime, CSkill* skill, CParticleSystem* ps )
{
	CPlayer* pPlayer		= CPlayer::GetInstance();
	CBattleMap* pBattleMap	= CBattleMap::GetInstance();
	CBase* character		= pBattleMap->GetCurrChar();
	CBase* target			= pBattleMap->GetCurrEnemyTarget();
	// init particles...
	if (skill->GetTimer() == 0.0f)
	{
		ps[BLOOD].Emit(target->GetPosX(), target->GetPosY());
		ps[BLOOD].m_bActive = true; ps[BLOOD].m_bLoop = false;
		skill->SetComb(skill->GetMaxCombAmt());

	}
	if (skill->IsComplete())
	{
		// FORMULA: str - def + dmg + accuracy * 1.5 + random(-5 to 5) + (QTE amt successful * 5)
		if (skill->GetCurrAmtSuccessful() > 0)
		{
			int damage = (int)((float)(character->GetStrength() - target->GetDefense() + 
				skill->GetDmg() + character->GetAccuracy()) * 1.5f + rand() % (5 + 4) - 4) + (skill->GetCurrAmtSuccessful() * 5);
			target->SetHealth(target->GetHealth() - damage);
		}
		ps[BLOOD].m_bActive = false;
		skill->ClearComb();
		skill->SetCurrAmtSuccessful(0);
	}
	ps[BLOOD].UpdateParticle(elapsedTime);
	ps[BLOOD].DrawParticle(CAssets::GetInstance()->aBloodParticle);

}

//////////////////////////////////////////////////////////////////////////
//	KNOCK BACK

void RenderKnockBack(CSkill* skill, CParticleSystem* ps)
{

}

void UpdateKnockBack( float elapsedTime, CSkill* skill, CParticleSystem* ps )
{
	CPlayer* pPlayer		= CPlayer::GetInstance();
	CBattleMap* pBattleMap	= CBattleMap::GetInstance();
	CBase* character		= pBattleMap->GetCurrChar();
	CBase* target			= pBattleMap->GetCurrEnemyTarget();
	// init particles...
	if (skill->GetTimer() == 0.0f)
	{
		ps[BLOOD].Emit(target->GetPosX(), target->GetPosY());
		ps[BLOOD].m_bActive = true; ps[BLOOD].m_bLoop = false;
		skill->SetComb(skill->GetMaxCombAmt());

	}
	if (skill->IsComplete())
	{
		// FORMULA: str - def + dmg + accuracy * 1.5 + random(-5 to 5) + (QTE amt successful * 5)
		if (skill->GetCurrAmtSuccessful() > 0)
		{
			int damage = (int)((float)(character->GetStrength() - target->GetDefense() + 
				skill->GetDmg() + character->GetAccuracy()) * 1.5f + rand() % (5 + 4) - 4) + (skill->GetCurrAmtSuccessful() * 5);
			target->SetHealth(target->GetHealth() - damage);
		}
		ps[BLOOD].m_bActive = false;
		skill->ClearComb();
		skill->SetCurrAmtSuccessful(0);
	}
	ps[BLOOD].UpdateParticle(elapsedTime);
	ps[BLOOD].DrawParticle(CAssets::GetInstance()->aBloodParticle);
}

//////////////////////////////////////////////////////////////////////////
//	STAFF SPIN

void RenderStaffSpin(CSkill* skill, CParticleSystem* ps)
{

}

void UpdateStaffSpin( float elapsedTime, CSkill* skill, CParticleSystem* ps )
{
	CPlayer* pPlayer		= CPlayer::GetInstance();
	CBattleMap* pBattleMap	= CBattleMap::GetInstance();
	CBase* character		= pBattleMap->GetCurrChar();
	CBase* target			= pBattleMap->GetCurrEnemyTarget();
	// init particles...
	if (skill->GetTimer() == 0.0f)
	{
		ps[BLOOD].Emit(target->GetPosX(), target->GetPosY());
		ps[BLOOD].m_bActive = true; ps[BLOOD].m_bLoop = false;
		skill->SetComb(skill->GetMaxCombAmt());

	}
	if (skill->IsComplete())
	{
		// FORMULA: str - def + dmg + accuracy * 1.5 + random(-5 to 5) + (QTE amt successful * 5)
		if (skill->GetCurrAmtSuccessful() > 0)
		{
			int damage = (int)((float)(character->GetStrength() - target->GetDefense() + 
				skill->GetDmg() + character->GetAccuracy()) * 1.5f + rand() % (5 + 4) - 4) + (skill->GetCurrAmtSuccessful() * 5);
			target->SetHealth(target->GetHealth() - damage);
		}
		ps[BLOOD].m_bActive = false;
		skill->ClearComb();
		skill->SetCurrAmtSuccessful(0);
	}
	ps[BLOOD].UpdateParticle(elapsedTime);
	ps[BLOOD].DrawParticle(CAssets::GetInstance()->aBloodParticle);
}

//////////////////////////////////////////////////////////////////////////
//	STAFF UPPERCUT

void RenderStaffUppercut(CSkill* skill, CParticleSystem* ps)
{

}

void UpdateStaffUppercut( float elapsedTime, CSkill* skill, CParticleSystem* ps )
{
	CPlayer* pPlayer		= CPlayer::GetInstance();
	CBattleMap* pBattleMap	= CBattleMap::GetInstance();
	CBase* character		= pBattleMap->GetCurrChar();
	CBase* target			= pBattleMap->GetCurrEnemyTarget();
	// init particles...
	if (skill->GetTimer() == 0.0f)
	{
		ps[BLOOD].Emit(target->GetPosX(), target->GetPosY());
		ps[BLOOD].m_bActive = true; ps[BLOOD].m_bLoop = false;
		skill->SetComb(skill->GetMaxCombAmt());

	}
	if (skill->IsComplete())
	{
		// FORMULA: str - def + dmg + accuracy * 1.5 + random(-5 to 5) + (QTE amt successful * 5)
		if (skill->GetCurrAmtSuccessful() > 0)
		{
			int damage = (int)((float)(character->GetStrength() - target->GetDefense() + 
				skill->GetDmg() + character->GetAccuracy()) * 1.5f + rand() % (5 + 4) - 4) + (skill->GetCurrAmtSuccessful() * 5);
			target->SetHealth(target->GetHealth() - damage);
		}
		ps[BLOOD].m_bActive = false;
		skill->ClearComb();
		skill->SetCurrAmtSuccessful(0);
	}
	ps[BLOOD].UpdateParticle(elapsedTime);
	ps[BLOOD].DrawParticle(CAssets::GetInstance()->aBloodParticle);
}

//////////////////////////////////////////////////////////////////////////
//	BACK FLIP AWAY

void RenderBackflipAway(CSkill* skill, CParticleSystem* ps)
{

}

void UpdateBackflipAway( float elapsedTime, CSkill* skill, CParticleSystem* ps )
{
	CPlayer* pPlayer		= CPlayer::GetInstance();
	CBattleMap* pBattleMap	= CBattleMap::GetInstance();
	CBase* character		= pBattleMap->GetCurrChar();
	CBase* target			= pBattleMap->GetCurrEnemyTarget();
	// init particles...
	if (skill->GetTimer() == 0.0f)
	{
		ps[BLOOD].Emit(target->GetPosX(), target->GetPosY());
		ps[BLOOD].m_bActive = true; ps[BLOOD].m_bLoop = false;
		skill->SetComb(skill->GetMaxCombAmt());

	}
	if (skill->IsComplete())
	{
		// FORMULA: str - def + dmg + accuracy * 1.5 + random(-5 to 5) + (QTE amt successful * 5)
		if (skill->GetCurrAmtSuccessful() > 0)
		{
			int damage = (int)((float)(character->GetStrength() - target->GetDefense() + 
				skill->GetDmg() + character->GetAccuracy()) * 1.5f + rand() % (5 + 4) - 4) + (skill->GetCurrAmtSuccessful() * 5);
			target->SetHealth(target->GetHealth() - damage);
		}
		ps[BLOOD].m_bActive = false;
		skill->ClearComb();
		skill->SetCurrAmtSuccessful(0);
	}
	ps[BLOOD].UpdateParticle(elapsedTime);
	ps[BLOOD].DrawParticle(CAssets::GetInstance()->aBloodParticle);
}

//////////////////////////////////////////////////////////////////////////
//	CREATE BOMB

void RenderCreateBomb(CSkill* skill, CParticleSystem* ps)
{

}

void UpdateCreateBomb( float elapsedTime, CSkill* skill, CParticleSystem* ps )
{
	CPlayer* pPlayer		= CPlayer::GetInstance();
	CBattleMap* pBattleMap	= CBattleMap::GetInstance();
	CBase* character		= pBattleMap->GetCurrChar();
	CBase* target			= pBattleMap->GetCurrEnemyTarget();
	// init particles...
	if (skill->GetTimer() == 0.0f)
	{
		ps[BLOOD].Emit(target->GetPosX(), target->GetPosY());
		ps[BLOOD].m_bActive = true; ps[BLOOD].m_bLoop = false;
		skill->SetComb(skill->GetMaxCombAmt());

	}
	if (skill->IsComplete())
	{
		// FORMULA: str - def + dmg + accuracy * 1.5 + random(-5 to 5) + (QTE amt successful * 5)
		if (skill->GetCurrAmtSuccessful() > 0)
		{
			int damage = (int)((float)(character->GetStrength() - target->GetDefense() + 
				skill->GetDmg() + character->GetAccuracy()) * 1.5f + rand() % (5 + 4) - 4) + (skill->GetCurrAmtSuccessful() * 5);
			target->SetHealth(target->GetHealth() - damage);
		}
		ps[BLOOD].m_bActive = false;
		skill->ClearComb();
		skill->SetCurrAmtSuccessful(0);
	}
	ps[BLOOD].UpdateParticle(elapsedTime);
	ps[BLOOD].DrawParticle(CAssets::GetInstance()->aBloodParticle);
}

//////////////////////////////////////////////////////////////////////////
//	SAI STAB

void RenderFlyingSaiStab(CSkill* skill, CParticleSystem* ps)
{

}

void UpdateFlyingSaiStab( float elapsedTime, CSkill* skill, CParticleSystem* ps )
{
	CPlayer* pPlayer		= CPlayer::GetInstance();
	CBattleMap* pBattleMap	= CBattleMap::GetInstance();
	CBase* character		= pBattleMap->GetCurrChar();
	CBase* target			= pBattleMap->GetCurrEnemyTarget();
	// init particles...
	if (skill->GetTimer() == 0.0f)
	{
		ps[BLOOD].Emit(target->GetPosX(), target->GetPosY());
		ps[BLOOD].m_bActive = true; ps[BLOOD].m_bLoop = false;
		skill->SetComb(skill->GetMaxCombAmt());

	}
	if (skill->IsComplete())
	{
		// FORMULA: str - def + dmg + accuracy * 1.5 + random(-5 to 5) + (QTE amt successful * 5)
		if (skill->GetCurrAmtSuccessful() > 0)
		{
			int damage = (int)((float)(character->GetStrength() - target->GetDefense() + 
				skill->GetDmg() + character->GetAccuracy()) * 1.5f + rand() % (5 + 4) - 4) + (skill->GetCurrAmtSuccessful() * 5);
			target->SetHealth(target->GetHealth() - damage);
		}
		ps[BLOOD].m_bActive = false;
		skill->ClearComb();
		skill->SetCurrAmtSuccessful(0);
	}
	ps[BLOOD].UpdateParticle(elapsedTime);
	ps[BLOOD].DrawParticle(CAssets::GetInstance()->aBloodParticle);
}

//////////////////////////////////////////////////////////////////////////
//	SAI FURY

void RenderSaiFury(CSkill* skill, CParticleSystem* ps)
{

}

void UpdateSaiFury( float elapsedTime, CSkill* skill, CParticleSystem* ps )
{
	CPlayer* pPlayer		= CPlayer::GetInstance();
	CBattleMap* pBattleMap	= CBattleMap::GetInstance();
	CBase* character		= pBattleMap->GetCurrChar();
	CBase* target			= pBattleMap->GetCurrEnemyTarget();
	// init particles...
	if (skill->GetTimer() == 0.0f)
	{
		ps[BLOOD].Emit(target->GetPosX(), target->GetPosY());
		ps[BLOOD].m_bActive = true; ps[BLOOD].m_bLoop = false;
		skill->SetComb(skill->GetMaxCombAmt());

	}
	if (skill->IsComplete())
	{
		// FORMULA: str - def + dmg + accuracy * 1.5 + random(-5 to 5) + (QTE amt successful * 5)
		if (skill->GetCurrAmtSuccessful() > 0)
		{
			int damage = (int)((float)(character->GetStrength() - target->GetDefense() + 
				skill->GetDmg() + character->GetAccuracy()) * 1.5f + rand() % (5 + 4) - 4) + (skill->GetCurrAmtSuccessful() * 5);
			target->SetHealth(target->GetHealth() - damage);
		}
		ps[BLOOD].m_bActive = false;
		skill->ClearComb();
		skill->SetCurrAmtSuccessful(0);
	}
	ps[BLOOD].UpdateParticle(elapsedTime);
	ps[BLOOD].DrawParticle(CAssets::GetInstance()->aBloodParticle);
}

//////////////////////////////////////////////////////////////////////////
//	SKULL SPLITTER

void RenderNunchuckSkullSplitter(CSkill* skill, CParticleSystem* ps)
{

}

void UpdateNunchuckSkullSplitter( float elapsedTime, CSkill* skill, CParticleSystem* ps )
{
	CPlayer* pPlayer		= CPlayer::GetInstance();
	CBattleMap* pBattleMap	= CBattleMap::GetInstance();
	CBase* character		= pBattleMap->GetCurrChar();
	CBase* target			= pBattleMap->GetCurrEnemyTarget();
	// init particles...
	if (skill->GetTimer() == 0.0f)
	{
		ps[BLOOD].Emit(target->GetPosX(), target->GetPosY());
		ps[BLOOD].m_bActive = true; ps[BLOOD].m_bLoop = false;
		skill->SetComb(skill->GetMaxCombAmt());

	}
	if (skill->IsComplete())
	{
		// FORMULA: str - def + dmg + accuracy * 1.5 + random(-5 to 5) + (QTE amt successful * 5)
		if (skill->GetCurrAmtSuccessful() > 0)
		{
			int damage = (int)((float)(character->GetStrength() - target->GetDefense() + 
				skill->GetDmg() + character->GetAccuracy()) * 1.5f + rand() % (5 + 4) - 4) + (skill->GetCurrAmtSuccessful() * 5);
			target->SetHealth(target->GetHealth() - damage);
		}
		ps[BLOOD].m_bActive = false;
		skill->ClearComb();
		skill->SetCurrAmtSuccessful(0);
	}
	ps[BLOOD].UpdateParticle(elapsedTime);
	ps[BLOOD].DrawParticle(CAssets::GetInstance()->aBloodParticle);
}

//////////////////////////////////////////////////////////////////////////
//	NUNCHUCK SPIN

void RenderNunchuckSpin(CSkill* skill, CParticleSystem* ps)
{

}

void UpdateNunchuckSpin( float elapsedTime, CSkill* skill, CParticleSystem* ps )
{
	CPlayer* pPlayer		= CPlayer::GetInstance();
	CBattleMap* pBattleMap	= CBattleMap::GetInstance();
	CBase* character		= pBattleMap->GetCurrChar();
	CBase* target			= pBattleMap->GetCurrEnemyTarget();
	// init particles...
	if (skill->GetTimer() == 0.0f)
	{
		ps[BLOOD].Emit(target->GetPosX(), target->GetPosY());
		ps[BLOOD].m_bActive = true; ps[BLOOD].m_bLoop = false;
		skill->SetComb(skill->GetMaxCombAmt());

	}
	if (skill->IsComplete())
	{
		// FORMULA: str - def + dmg + accuracy * 1.5 + random(-5 to 5) + (QTE amt successful * 5)
		if (skill->GetCurrAmtSuccessful() > 0)
		{
			int damage = (int)((float)(character->GetStrength() - target->GetDefense() + 
				skill->GetDmg() + character->GetAccuracy()) * 1.5f + rand() % (5 + 4) - 4) + (skill->GetCurrAmtSuccessful() * 5);
			target->SetHealth(target->GetHealth() - damage);
		}
		ps[BLOOD].m_bActive = false;
		skill->ClearComb();
		skill->SetCurrAmtSuccessful(0);
	}
	ps[BLOOD].UpdateParticle(elapsedTime);
	ps[BLOOD].DrawParticle(CAssets::GetInstance()->aBloodParticle);
}

//////////////////////////////////////////////////////////////////////////
//	ROLL AWAY

void RenderRollAway(CSkill* skill, CParticleSystem* ps)
{

}

void UpdateRollAway( float elapsedTime, CSkill* skill, CParticleSystem* ps )
{
	CPlayer* pPlayer		= CPlayer::GetInstance();
	CBattleMap* pBattleMap	= CBattleMap::GetInstance();
	CBase* character		= pBattleMap->GetCurrChar();
	CBase* target			= pBattleMap->GetCurrEnemyTarget();
	// init particles...
	if (skill->GetTimer() == 0.0f)
	{
		ps[BLOOD].Emit(target->GetPosX(), target->GetPosY());
		ps[BLOOD].m_bActive = true; ps[BLOOD].m_bLoop = false;
		skill->SetComb(skill->GetMaxCombAmt());

	}
	if (skill->IsComplete())
	{
		// FORMULA: str - def + dmg + accuracy * 1.5 + random(-5 to 5) + (QTE amt successful * 5)
		if (skill->GetCurrAmtSuccessful() > 0)
		{
			int damage = (int)((float)(character->GetStrength() - target->GetDefense() + 
				skill->GetDmg() + character->GetAccuracy()) * 1.5f + rand() % (5 + 4) - 4) + (skill->GetCurrAmtSuccessful() * 5);
			target->SetHealth(target->GetHealth() - damage);
		}
		ps[BLOOD].m_bActive = false;
		skill->ClearComb();
		skill->SetCurrAmtSuccessful(0);
	}
	ps[BLOOD].UpdateParticle(elapsedTime);
	ps[BLOOD].DrawParticle(CAssets::GetInstance()->aBloodParticle);
}

//////////////////////////////////////////////////////////////////////////