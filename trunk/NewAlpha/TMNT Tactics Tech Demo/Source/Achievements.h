#ifndef ACHIEVEMENTS_H
#define ACHIEVEMENTS_H

enum {
	ACH_NEWGAME, 
	ACH_LVLUP, 
	ACH_TURTLESDEAD, 
	ACH_ALLUNLOCKED, 
	ACH_FIRSTMAPCOMPLETE,
	ACH_KILL30, 
	ACH_MEGAKILL, 
	ACH_KILL_SHREDDER,
	ACH_UNLOCKSKILLS, 
	ACH_QUICKFINGERS, 
	};

#include "CSGD_TextureManager.h"
#include "Assets.h"

class CAchievements
{
	CSGD_TextureManager* m_pTM;
	struct Acheivement 
	{
		bool bUnlocked;
		int imageID;
		string title;
	};
	int m_nUnlockCount;
	Acheivement m_bAcheivements[10];

public:
	CAchievements()
	{
		m_pTM = CSGD_TextureManager::GetInstance();
		SetToLocked();
// 		m_bAcheivements[ACH_NEWGAME].imageID =		m_pTM->LoadTexture();
// 		m_bAcheivements[ACH_LVLUP].imageID =		m_pTM->LoadTexture();	
// 		m_bAcheivements[ACH_TURTLESDEAD].imageID =	m_pTM->LoadTexture();
// 		m_bAcheivements[ACH_ALLUNLOCKED].imageID =	m_pTM->LoadTexture();
// 		m_bAcheivements[ACH_FIRSTMAPCOMPLETE].imageID = m_pTM->LoadTexture();
// 		m_bAcheivements[ACH_KILL30].imageID =		m_pTM->LoadTexture();	
// 		m_bAcheivements[ACH_MEGAKILL].imageID =		m_pTM->LoadTexture();
// 		m_bAcheivements[ACH_KILL_SHREDDER].imageID= m_pTM->LoadTexture();
// 		m_bAcheivements[ACH_UNLOCKSKILLS].imageID =	m_pTM->LoadTexture();
// 		m_bAcheivements[ACH_QUICKFINGERS].imageID =	m_pTM->LoadTexture();

		m_bAcheivements[ACH_NEWGAME].title =		"PARTY TIME";
		m_bAcheivements[ACH_LVLUP].title =			"COWABUNGA";
		m_bAcheivements[ACH_TURTLESDEAD].title =	"FLIPS NOW";
		m_bAcheivements[ACH_ALLUNLOCKED].title =	"YOU'RE GONNA BREAK THE GAME";
		m_bAcheivements[ACH_FIRSTMAPCOMPLETE].title = "STRIKE HARD AND FADE AWAY";
		m_bAcheivements[ACH_KILL30].title =			"A LESSON IN PAIN";
		m_bAcheivements[ACH_MEGAKILL].title =		"MEGA KILL";
		m_bAcheivements[ACH_KILL_SHREDDER].title =	"AVENGED";
		m_bAcheivements[ACH_UNLOCKSKILLS].title =	"TRUE NINJA";
		m_bAcheivements[ACH_QUICKFINGERS].title =	"QUICK FINGERS";
	}

	void Unlock(int id)
	{
		m_bAcheivements[id].bUnlocked = true;
		++m_nUnlockCount;
		if (m_nUnlockCount == 10)
			m_bAcheivements[ACH_ALLUNLOCKED].bUnlocked = true;
	}

	void SetToLocked()
	{
		for (int i = 0; i < 10; ++i)
			m_bAcheivements[i].bUnlocked = false;
	}
};


#endif