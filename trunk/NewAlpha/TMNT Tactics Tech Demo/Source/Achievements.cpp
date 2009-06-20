#include "Achievements.h"
#include "CSGD_TextureManager.h"
#include "Assets.h"
#include "CSGD_Direct3D.h"
#include "BattleMap.h"
#include "CSGD_FModManager.h"

CAchievements::CAchievements()
{
	m_pTM = CSGD_TextureManager::GetInstance();
	m_pAssets = CAssets::GetInstance();
	m_pD3D = CSGD_Direct3D::GetInstance();
	m_fTimer = 0.0f;
	SetToLocked();
	m_pAcheivements[ACH_NEWGAME].imageID =			m_pAssets->aPartyTime;
	m_pAcheivements[ACH_LVLUP].imageID =			m_pAssets->aCowabunga;	
	m_pAcheivements[ACH_TURTLESDEAD].imageID =		m_pAssets->aFlipNow;
	m_pAcheivements[ACH_ALLUNLOCKED].imageID =		m_pAssets->aBreakGame;
	m_pAcheivements[ACH_FIRSTMAPCOMPLETE].imageID = m_pAssets->aStrikeHard;
	m_pAcheivements[ACH_KILL30].imageID =			m_pAssets->aLessonInPain;	
	m_pAcheivements[ACH_MEGAKILL].imageID =			m_pAssets->aMegaKill;
	m_pAcheivements[ACH_KILL_SHREDDER].imageID=		m_pAssets->aAvenged;
	m_pAcheivements[ACH_UNLOCKSKILLS].imageID =		m_pAssets->aTrueNinja;
	m_pAcheivements[ACH_QUICKFINGERS].imageID =		m_pAssets->aQuickFingers;

	m_pAcheivements[ACH_NEWGAME].title =		"PARTY TIME";
	m_pAcheivements[ACH_LVLUP].title =			"COWABUNGA";
	m_pAcheivements[ACH_TURTLESDEAD].title =	"FLIPS NOW";
	m_pAcheivements[ACH_ALLUNLOCKED].title =	"YOU'RE GONNA BREAK THE GAME";
	m_pAcheivements[ACH_FIRSTMAPCOMPLETE].title = "STRIKE HARD AND FADE AWAY";
	m_pAcheivements[ACH_KILL30].title =			"A LESSON IN PAIN";
	m_pAcheivements[ACH_MEGAKILL].title =		"MEGA KILL";
	m_pAcheivements[ACH_KILL_SHREDDER].title =	"AVENGED";
	m_pAcheivements[ACH_UNLOCKSKILLS].title =	"TRUE NINJA";
	m_pAcheivements[ACH_QUICKFINGERS].title =	"QUICK FINGERS";

	m_pAcheivements[ACH_NEWGAME].description =			"Started a new game!";
	m_pAcheivements[ACH_LVLUP].description =			"First turtle leveled up.";
	m_pAcheivements[ACH_TURTLESDEAD].description =		"All turtles dead...good job.";
	m_pAcheivements[ACH_ALLUNLOCKED].description =		"All achievements unlocked.";
	m_pAcheivements[ACH_FIRSTMAPCOMPLETE].description = "First victory in battle!";
	m_pAcheivements[ACH_KILL30].description =			"Killed 30 enemies!";
	m_pAcheivements[ACH_MEGAKILL].description =			"One turtle killed 3 enemies on a map!";
	m_pAcheivements[ACH_KILL_SHREDDER].description =	"Shredder has been defeated.";
	m_pAcheivements[ACH_UNLOCKSKILLS].description =		"All skills unlocked.";
	m_pAcheivements[ACH_QUICKFINGERS].description =		"A perfect skill execution.";
}

CAchievements::~CAchievements()
{
	m_pTM = NULL;
	m_pAssets = NULL;
	m_pD3D = NULL;
}

void CAchievements::Update(float fElapsedTime)
{
	if (m_nCurrUnlocked > -1)
	{
		if (m_fTimer == 0.0f)
			CBattleMap::GetInstance()->PlaySFX(m_pAssets->aBMpickupSnd);
		m_fTimer += fElapsedTime;
	}
}

bool CAchievements::Render()
{
	// only renders something if an achievement has recently been unlocked
	if (m_nCurrUnlocked > -1)
	{
		if (m_fTimer < 3.0f)
		{
			// display achievement image
			m_pTM->DrawWithZSort(m_pAcheivements[m_nCurrUnlocked].imageID, 350, 300, 0.0f);
			return true;
		}
		else
		{m_fTimer = 0.0f; m_nCurrUnlocked=-1;}
	}
	return false;
}

void CAchievements::LoadUnlock(int id)
{
	m_pAcheivements[id].bUnlocked = true;
	m_nCurrUnlocked = -1;
	m_nUnlockCount++;
}

void CAchievements::SetToLocked()
{
	for (int i = 0; i < 10; ++i)
		m_pAcheivements[i].bUnlocked = false;
	m_nCurrUnlocked = -1;
	m_nUnlockCount = 0;
}


void CAchievements::Unlock(int id)
{
	m_pAcheivements[id].bUnlocked = true;
	m_nCurrUnlocked = id;
	if (id == ACH_FIRSTMAPCOMPLETE)
		CBattleMap::GetInstance()->PlaySFX(m_pAssets->aStrikeHardSnd);
	++m_nUnlockCount;
	if (m_nUnlockCount == 10)
		m_pAcheivements[ACH_ALLUNLOCKED].bUnlocked = true;
}