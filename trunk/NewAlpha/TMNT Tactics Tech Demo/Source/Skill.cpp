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
#include "BitmapFont.h"
#include "Achievements.h"
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
CSkill::CSkill() 
{
	m_strName = "NONE";
	m_nType = -1;
	m_nDamage = -1;
	m_nRange = -1;
	m_nSkillCost = -1;
	m_nSkillID	= -1;
	m_nCurrAmountSuccessful = 0;
	m_nMaxCombinationAmount = -1;
	m_bAttacking = m_bTrigger = m_bQTEfailed = false;
	m_pCombination  = NULL;
	m_pPlayer		= CPlayer::GetInstance();
	m_pBattleMap	= CBattleMap::GetInstance();
	m_pBitmapFont	= CBitmapFont::GetInstance();
	m_pDI			= CSGD_DirectInput::GetInstance();
	m_pD3D			= CSGD_Direct3D::GetInstance();
	m_pTM			= CSGD_TextureManager::GetInstance();
}

CSkill::CSkill(string name, int type, int skillID, int dmg, int range, int cost, int combAmt, float duration)
{
	m_strName = name;
	m_nType = type;
	m_nDamage = dmg;
	m_nRange = range;
	m_nSkillCost = cost;
	m_nSkillID	= skillID;
	m_nCurrAmountSuccessful = 0;
	m_nMaxCombinationAmount = combAmt;
	m_bAttacking = m_bTrigger = m_bQTEfailed = m_bRenderDmg = false;
	m_pCombination = NULL;
	m_pPlayer		= CPlayer::GetInstance();
	m_pBattleMap	= CBattleMap::GetInstance();
	m_pBitmapFont	= CBitmapFont::GetInstance();
	m_pDI			= CSGD_DirectInput::GetInstance();
	m_pD3D			= CSGD_Direct3D::GetInstance();
	m_pTM			= CSGD_TextureManager::GetInstance();
	
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
	bool timerEvent = m_Timer.Update(fElapsedTime);

	// either the time is up, the combination has been done successfully, or they entered a wrong key
	if (timerEvent || m_nCurrAmountSuccessful == m_nMaxCombinationAmount || m_bQTEfailed)
	{
		if (m_nCurrAmountSuccessful == m_nMaxCombinationAmount && !m_bAttacking)	// the skill was finished correctly
		{
			m_Timer.ResetTimer();
			skill->SetCurrAmtSuccessful(0);
			skill->ClearComb();
			m_pPlayer->GetTurtles()[m_pBattleMap->GetCurrActive()]->SetCurrAnim(4);	// now start the first attack animation
			m_bAttacking = true;
			m_Timer.StartTimer();	//TODO::ask matt which animations to set to NOT looping, so i can check for when they're done, instead of a timer
		}
		else if (m_bQTEfailed)	// the skill was not finished correctly
		{
			m_Timer.ResetTimer();
			m_pPlayer->GetTurtles()[m_pBattleMap->GetCurrActive()]->SetCurrAnim(2);
			skill->ClearComb();
			skill->SetCurrAmtSuccessful(0);
			delete[] m_pCombination;
			m_bComplete = true; 
			m_bTrigger = m_bAttacking = false;
			return;
		}
		// TODO:: acts as signal that skill is finished (for now), change it to trigger at end of the animations that will be executed for each specific skill
		if (m_bRenderDmg)	
		{
			m_bRenderDmg = m_bAttacking = false;
			m_bComplete = true;	 // TODO:: move this when i can stop the animations from looping
			if (!m_pPlayer->GetAch()->GetLocked(ACH_QUICKFINGERS))
				m_pPlayer->GetAch()->Unlock(ACH_QUICKFINGERS);
			m_pPlayer->GetTurtles()[m_pBattleMap->GetCurrActive()]->SetCurrAnim(2);
			m_Timer.ResetTimer();
			m_QTEtimer.ResetTimer();
			return;
		}
	}
	if (m_bAttacking)	// the attack is currently underway - that means animate, draw particles, and do damage accordingly
	{
		if (m_Timer.GetElapsed() > 1.0f/*!m_pPlayer->GetTurtles()[m_pBattleMap->GetCurrActive()]->GetCurrAnim()->IsAnimationPlaying()*/)
		{
			m_Timer.ResetTimer();
			m_bTrigger = true;	// notify the skill that one attack has been executed
			m_bRenderDmg = true;
			m_Timer.StartTimer(0.5f);	// damage render timer
		}
	}
	
	m_pUpdatePtr(fElapsedTime, skill, ps);

	if (!m_QTEtimer.IsTimerRunning())	// starting the skill (the very beginning)
	{
		m_bComplete = false;
		m_QTEtimer.StartTimer((float)m_nMaxCombinationAmount);
		SetComb(m_nMaxCombinationAmount);
		m_bQTEfailed = m_bRenderDmg = false;
	}
	else if (m_QTEtimer.GetElapsed() > (float)m_nMaxCombinationAmount && !m_bAttacking)
		m_bQTEfailed = true;
}

void CSkill::Render()
{
	// there is still time to input a direction
	if (m_QTEtimer.GetElapsed() <= (float)m_nMaxCombinationAmount && m_QTEtimer.IsTimerRunning() && !m_bAttacking)
	{
		switch (GetComb()[GetCurrAmtSuccessful()])
		{
		case UP:
			{
				m_pTM->DrawWithZSort(CAssets::GetInstance()->aBMqteUpID, QTEX, QTEY, 0.0f, 1.5f, 1.5f);
				if (m_pDI->KeyPressed(DIK_UP))
				{
					SetCurrAmtSuccessful(GetCurrAmtSuccessful()+1);
					m_pBattleMap->PlaySFX(CAssets::GetInstance()->aBMpickupSnd);
				}
				else if (m_pDI->KeyPressed(DIK_DOWN) || m_pDI->KeyPressed(DIK_LEFT) || m_pDI->KeyPressed(DIK_RIGHT))
					m_bQTEfailed = true;
			}
			break;
		case DOWN:
			{
				m_pTM->DrawWithZSort(CAssets::GetInstance()->aBMqteDownID, QTEX, QTEY, 0.0f, 1.5f, 1.5f);
				if (m_pDI->KeyPressed(DIK_DOWN))
				{
					SetCurrAmtSuccessful(GetCurrAmtSuccessful()+1);
					m_pBattleMap->PlaySFX(CAssets::GetInstance()->aBMpickupSnd);
				}
				else if (m_pDI->KeyPressed(DIK_UP) || m_pDI->KeyPressed(DIK_LEFT) || m_pDI->KeyPressed(DIK_RIGHT))
					m_bQTEfailed = true;
			}
			break;
		case LEFT:
			{
				m_pTM->DrawWithZSort(CAssets::GetInstance()->aBMqteLeftID, QTEX, QTEY, 0.0f, 1.5f, 1.5f);
				if (m_pDI->KeyPressed(DIK_LEFT))
				{
					SetCurrAmtSuccessful(GetCurrAmtSuccessful()+1);
					m_pBattleMap->PlaySFX(CAssets::GetInstance()->aBMpickupSnd);
				}
				else if (m_pDI->KeyPressed(DIK_UP) || m_pDI->KeyPressed(DIK_DOWN) || m_pDI->KeyPressed(DIK_RIGHT))
					m_bQTEfailed = true;
			}
			break;
		case RIGHT:
			{
				m_pTM->DrawWithZSort(CAssets::GetInstance()->aBMqteRightID, QTEX, QTEY, 0.0f, 1.5f, 1.5f);
				if (m_pDI->KeyPressed(DIK_RIGHT))
				{
					SetCurrAmtSuccessful(GetCurrAmtSuccessful()+1);
					m_pBattleMap->PlaySFX(CAssets::GetInstance()->aBMpickupSnd);
				}
				else if (m_pDI->KeyPressed(DIK_UP) || m_pDI->KeyPressed(DIK_LEFT) || m_pDI->KeyPressed(DIK_DOWN))
					m_bQTEfailed = true;
			}
			break;
		}
	}
	if (m_bRenderDmg)
	{
		int offset = (int)(m_Timer.GetElapsed()*20.0f )-15;

		m_pBitmapFont->ChangeBMFont(CAssets::GetInstance()->aBitmapFontBubblyID,16,15,20);

		//player xp gain
		int nXP = 20;
		char tempXP[16];
		sprintf_s(tempXP, "+%i", nXP);
		m_pBitmapFont->DrawString(tempXP,(int)m_pPlayer->GetTurtles()[m_pBattleMap->GetCurrActive()]->GetPosX()+5, 
										 (int)m_pPlayer->GetTurtles()[m_pBattleMap->GetCurrActive()]->GetPosY()-offset,
										 0.3f,0.9f,D3DCOLOR_XRGB(255,0,0));

		// damage to target
		char tempDmg[16];
		sprintf_s(tempDmg, "-%i", m_nDmgToRender);
		m_pBitmapFont->DrawString(tempXP,(int)m_pBattleMap->GetCurrEnemyTarget()->GetPosX()+5,
										 (int)m_pBattleMap->GetCurrEnemyTarget()->GetPosY()-offset,
										 0.4f,0.9f,D3DCOLOR_XRGB(255,255,0));

		m_pBitmapFont->Reset();
	}
}

void CSkill::SetComb(int num)
{
	m_pCombination = new int[num];
	for (int i = 0; i < num; ++i)
	{
		int dir = 0;
		if (i > 0)
		{
			while (dir == m_pCombination[i-1])
			{
				// find a different direction than the one before this one
				dir = rand() % 4;
			}
		}
		else
			dir = rand() % 4;
		m_pCombination[i] = dir;	// set to a random direction ( one of four)
	}
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

	//	if the skill consists of more than one animation, this needs to happen when each one is finished
	if (skill->Trigger()) 
	{
		ps[NINJA_BLOOD].Emit(target->GetPosX(), target->GetPosY());
		ps[NINJA_BLOOD].m_bActive = true; ps[NINJA_BLOOD].m_bLoop = false;

		// FORMULA: str - def + dmg + accuracy * 1.5 + random(-5 to 5) + (QTE amt successful * 5)
		skill->DmgToRender((int)((float)(character->GetStrength() - target->GetDefense() + 
			skill->GetDmg() + character->GetAccuracy()) * 1.5f + rand() % (5 + 4) - 5) + (skill->GetCurrAmtSuccessful() * 5));
		target->SetHealth(target->GetHealth() - skill->DmgToRender());
		ps[NINJA_BLOOD].m_bActive = false;
		skill->Trigger(false);
	}
	ps[NINJA_BLOOD].UpdateParticle(elapsedTime);
	ps[NINJA_BLOOD].DrawParticle(CAssets::GetInstance()->aBloodParticle);
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

	//	if the skill consists of more than one animation, this needs to happen when each one is finished
	if (skill->Trigger()) 
	{
		ps[NINJA_BLOOD].Emit(target->GetPosX(), target->GetPosY());
		ps[NINJA_BLOOD].m_bActive = true; ps[NINJA_BLOOD].m_bLoop = false;

		// FORMULA: str - def + dmg + accuracy * 1.5 + random(-5 to 5) + (QTE amt successful * 5)
		skill->DmgToRender((int)((float)(character->GetStrength() - target->GetDefense() + 
			skill->GetDmg() + character->GetAccuracy()) * 1.5f + rand() % (5 + 4) - 5) + (skill->GetCurrAmtSuccessful() * 5));
		target->SetHealth(target->GetHealth() - skill->DmgToRender());
		ps[NINJA_BLOOD].m_bActive = false;
		skill->Trigger(false);
	}
	ps[NINJA_BLOOD].UpdateParticle(elapsedTime);
	ps[NINJA_BLOOD].DrawParticle(CAssets::GetInstance()->aBloodParticle);
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
	//	if the skill consists of more than one animation, this needs to happen when each one is finished
	if (skill->Trigger()) 
	{
		ps[NINJA_BLOOD].Emit(target->GetPosX(), target->GetPosY());
		ps[NINJA_BLOOD].m_bActive = true; ps[NINJA_BLOOD].m_bLoop = false;

		// FORMULA: str - def + dmg + accuracy * 1.5 + random(-5 to 5) + (QTE amt successful * 5)
		skill->DmgToRender( (int)((float)(character->GetStrength() - target->GetDefense() + 
			skill->GetDmg() + character->GetAccuracy()) * 1.5f + rand() % (5 + 4) - 5) + (skill->GetCurrAmtSuccessful() * 5) );
		target->SetHealth(target->GetHealth() - skill->DmgToRender());
		ps[NINJA_BLOOD].m_bActive = false;
		skill->Trigger(false);
	}
	ps[NINJA_BLOOD].UpdateParticle(elapsedTime);
	ps[NINJA_BLOOD].DrawParticle(CAssets::GetInstance()->aBloodParticle);

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
	//	if the skill consists of more than one animation, this needs to happen when each one is finished
	if (skill->Trigger()) 
	{
		ps[NINJA_BLOOD].Emit(target->GetPosX(), target->GetPosY());
		ps[NINJA_BLOOD].m_bActive = true; ps[NINJA_BLOOD].m_bLoop = false;

		// FORMULA: str - def + dmg + accuracy * 1.5 + random(-5 to 5) + (QTE amt successful * 5)
		skill->DmgToRender((int)((float)(character->GetStrength() - target->GetDefense() + 
			skill->GetDmg() + character->GetAccuracy()) * 1.5f + rand() % (5 + 4) - 5) + (skill->GetCurrAmtSuccessful() * 5) );
		target->SetHealth(target->GetHealth() - skill->DmgToRender());
		ps[NINJA_BLOOD].m_bActive = false;
		skill->Trigger(false);
	}
	ps[NINJA_BLOOD].UpdateParticle(elapsedTime);
	ps[NINJA_BLOOD].DrawParticle(CAssets::GetInstance()->aBloodParticle);

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
	//	if the skill consists of more than one animation, this needs to happen when each one is finished
	if (skill->Trigger()) 
	{
		ps[NINJA_BLOOD].Emit(target->GetPosX(), target->GetPosY());
		ps[NINJA_BLOOD].m_bActive = true; ps[NINJA_BLOOD].m_bLoop = false;

		// FORMULA: str - def + dmg + accuracy * 1.5 + random(-5 to 5) + (QTE amt successful * 5)
		skill->DmgToRender( (int)((float)(character->GetStrength() - target->GetDefense() + 
			skill->GetDmg() + character->GetAccuracy()) * 1.5f + rand() % (5 + 4) - 5) + (skill->GetCurrAmtSuccessful() * 5) );
		target->SetHealth(target->GetHealth() - skill->DmgToRender());
		ps[NINJA_BLOOD].m_bActive = false;
		skill->Trigger(false);
	}
	ps[NINJA_BLOOD].UpdateParticle(elapsedTime);
	ps[NINJA_BLOOD].DrawParticle(CAssets::GetInstance()->aBloodParticle);
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
	//	if the skill consists of more than one animation, this needs to happen when each one is finished
	if (skill->Trigger()) 
	{
		ps[NINJA_BLOOD].Emit(target->GetPosX(), target->GetPosY());
		ps[NINJA_BLOOD].m_bActive = true; ps[NINJA_BLOOD].m_bLoop = false;

		// FORMULA: str - def + dmg + accuracy * 1.5 + random(-5 to 5) + (QTE amt successful * 5)
		skill->DmgToRender( (int)((float)(character->GetStrength() - target->GetDefense() + 
			skill->GetDmg() + character->GetAccuracy()) * 1.5f + rand() % (5 + 4) - 5) + (skill->GetCurrAmtSuccessful() * 5) );
		target->SetHealth(target->GetHealth() - skill->DmgToRender());
		ps[NINJA_BLOOD].m_bActive = false;
		skill->Trigger(false);
	}
	ps[NINJA_BLOOD].UpdateParticle(elapsedTime);
	ps[NINJA_BLOOD].DrawParticle(CAssets::GetInstance()->aBloodParticle);
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
	//	if the skill consists of more than one animation, this needs to happen when each one is finished
	if (skill->Trigger()) 
	{
		ps[NINJA_BLOOD].Emit(target->GetPosX(), target->GetPosY());
		ps[NINJA_BLOOD].m_bActive = true; ps[NINJA_BLOOD].m_bLoop = false;

		// FORMULA: str - def + dmg + accuracy * 1.5 + random(-5 to 5) + (QTE amt successful * 5)
		skill->DmgToRender( (int)((float)(character->GetStrength() - target->GetDefense() + 
			skill->GetDmg() + character->GetAccuracy()) * 1.5f + rand() % (5 + 4) - 5) + (skill->GetCurrAmtSuccessful() * 5) );
		target->SetHealth(target->GetHealth() - skill->DmgToRender());
		ps[NINJA_BLOOD].m_bActive = false;
		skill->Trigger(false);
	}
	ps[NINJA_BLOOD].UpdateParticle(elapsedTime);
	ps[NINJA_BLOOD].DrawParticle(CAssets::GetInstance()->aBloodParticle);
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
	//	if the skill consists of more than one animation, this needs to happen when each one is finished
	if (skill->Trigger()) 
	{
		ps[NINJA_BLOOD].Emit(target->GetPosX(), target->GetPosY());
		ps[NINJA_BLOOD].m_bActive = true; ps[NINJA_BLOOD].m_bLoop = false;

		// FORMULA: str - def + dmg + accuracy * 1.5 + random(-5 to 5) + (QTE amt successful * 5)
		skill->DmgToRender( (int)((float)(character->GetStrength() - target->GetDefense() + 
			skill->GetDmg() + character->GetAccuracy()) * 1.5f + rand() % (5 + 4) - 5) + (skill->GetCurrAmtSuccessful() * 5) );
		target->SetHealth(target->GetHealth() - skill->DmgToRender());
		ps[NINJA_BLOOD].m_bActive = false;
		skill->Trigger(false);
	}
	/*ps[BLOOD].UpdateParticle(elapsedTime);
	ps[BLOOD].DrawParticle(CAssets::GetInstance()->aBloodParticle);*/
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
	//	if the skill consists of more than one animation, this needs to happen when each one is finished
	if (skill->Trigger()) 
	{
		ps[NINJA_BLOOD].Emit(target->GetPosX(), target->GetPosY());
		ps[NINJA_BLOOD].m_bActive = true; ps[NINJA_BLOOD].m_bLoop = false;

		// FORMULA: str - def + dmg + accuracy * 1.5 + random(-5 to 5) + (QTE amt successful * 5)
		skill->DmgToRender( (int)((float)(character->GetStrength() - target->GetDefense() + 
			skill->GetDmg() + character->GetAccuracy()) * 1.5f + rand() % (5 + 4) - 5) + (skill->GetCurrAmtSuccessful() * 5) );
		target->SetHealth(target->GetHealth() - skill->DmgToRender());
		ps[NINJA_BLOOD].m_bActive = false;
		skill->Trigger(false);
	}
	ps[NINJA_BLOOD].UpdateParticle(elapsedTime);
	ps[NINJA_BLOOD].DrawParticle(CAssets::GetInstance()->aBloodParticle);
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
	//	if the skill consists of more than one animation, this needs to happen when each one is finished
	if (skill->Trigger()) 
	{
		ps[NINJA_BLOOD].Emit(target->GetPosX(), target->GetPosY());
		ps[NINJA_BLOOD].m_bActive = true; ps[NINJA_BLOOD].m_bLoop = false;

		// FORMULA: str - def + dmg + accuracy * 1.5 + random(-5 to 5) + (QTE amt successful * 5)
		skill->DmgToRender( (int)((float)(character->GetStrength() - target->GetDefense() + 
			skill->GetDmg() + character->GetAccuracy()) * 1.5f + rand() % (5 + 4) - 5) + (skill->GetCurrAmtSuccessful() * 5) );
		target->SetHealth(target->GetHealth() - skill->DmgToRender());
		ps[NINJA_BLOOD].m_bActive = false;
		skill->Trigger(false);
	}
	ps[NINJA_BLOOD].UpdateParticle(elapsedTime);
	ps[NINJA_BLOOD].DrawParticle(CAssets::GetInstance()->aBloodParticle);
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
	//	if the skill consists of more than one animation, this needs to happen when each one is finished
	if (skill->Trigger()) 
	{
		ps[NINJA_BLOOD].Emit(target->GetPosX(), target->GetPosY());
		ps[NINJA_BLOOD].m_bActive = true; ps[NINJA_BLOOD].m_bLoop = false;

		// FORMULA: str - def + dmg + accuracy * 1.5 + random(-5 to 5) + (QTE amt successful * 5)
		skill->DmgToRender( (int)((float)(character->GetStrength() - target->GetDefense() + 
			skill->GetDmg() + character->GetAccuracy()) * 1.5f + rand() % (5 + 4) - 5) + (skill->GetCurrAmtSuccessful() * 5) );
		target->SetHealth(target->GetHealth() - skill->DmgToRender());
		ps[NINJA_BLOOD].m_bActive = false;
		skill->Trigger(false);
	}
	ps[NINJA_BLOOD].UpdateParticle(elapsedTime);
	ps[NINJA_BLOOD].DrawParticle(CAssets::GetInstance()->aBloodParticle);
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
	//	if the skill consists of more than one animation, this needs to happen when each one is finished
	if (skill->Trigger()) 
	{
		ps[NINJA_BLOOD].Emit(target->GetPosX(), target->GetPosY());
		ps[NINJA_BLOOD].m_bActive = true; ps[NINJA_BLOOD].m_bLoop = false;

		// FORMULA: str - def + dmg + accuracy * 1.5 + random(-5 to 5) + (QTE amt successful * 5)
		skill->DmgToRender( (int)((float)(character->GetStrength() - target->GetDefense() + 
			skill->GetDmg() + character->GetAccuracy()) * 1.5f + rand() % (5 + 4) - 5) + (skill->GetCurrAmtSuccessful() * 5) );
		target->SetHealth(target->GetHealth() - skill->DmgToRender());
		ps[NINJA_BLOOD].m_bActive = false;
		skill->Trigger(false);
	}
	ps[NINJA_BLOOD].UpdateParticle(elapsedTime);
	ps[NINJA_BLOOD].DrawParticle(CAssets::GetInstance()->aBloodParticle);
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
	//	if the skill consists of more than one animation, this needs to happen when each one is finished
	if (skill->Trigger()) 
	{
		ps[NINJA_BLOOD].Emit(target->GetPosX(), target->GetPosY());
		ps[NINJA_BLOOD].m_bActive = true; ps[NINJA_BLOOD].m_bLoop = false;

		// FORMULA: str - def + dmg + accuracy * 1.5 + random(-5 to 5) + (QTE amt successful * 5)
		skill->DmgToRender( (int)((float)(character->GetStrength() - target->GetDefense() + 
			skill->GetDmg() + character->GetAccuracy()) * 1.5f + rand() % (5 + 4) - 5) + (skill->GetCurrAmtSuccessful() * 5) );
		target->SetHealth(target->GetHealth() - skill->DmgToRender());
		ps[NINJA_BLOOD].m_bActive = false;
		skill->Trigger(false);
	}
	ps[NINJA_BLOOD].UpdateParticle(elapsedTime);
	ps[NINJA_BLOOD].DrawParticle(CAssets::GetInstance()->aBloodParticle);
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
	//	if the skill consists of more than one animation, this needs to happen when each one is finished
	if (skill->Trigger()) 
	{
		ps[NINJA_BLOOD].Emit(target->GetPosX(), target->GetPosY());
		ps[NINJA_BLOOD].m_bActive = true; ps[NINJA_BLOOD].m_bLoop = false;

		// FORMULA: str - def + dmg + accuracy * 1.5 + random(-5 to 5) + (QTE amt successful * 5)
		skill->DmgToRender( (int)((float)(character->GetStrength() - target->GetDefense() + 
			skill->GetDmg() + character->GetAccuracy()) * 1.5f + rand() % (5 + 4) - 5) + (skill->GetCurrAmtSuccessful() * 5) );
		target->SetHealth(target->GetHealth() - skill->DmgToRender());
		ps[NINJA_BLOOD].m_bActive = false;
		skill->Trigger(false);
	}
	/*ps[BLOOD].UpdateParticle(elapsedTime);
	ps[BLOOD].DrawParticle(CAssets::GetInstance()->aBloodParticle);*/
}

//////////////////////////////////////////////////////////////////////////