/////////////////////////////////////////////////////////////////////////////
//	Filename	:	CollectedAchievements.cpp
//
//	Author		:	Jose Sierra (JS)
//
//	Purpose		:	To let the player view their achievements
/////////////////////////////////////////////////////////////////////////////

#include "CollectedAchievements.h"
#include "MainMenuState.h"
#include "CSGD_TextureManager.h"
#include "CSGD_DirectInput.h"
#include "CSGD_FModManager.h"
#include "BitmapFont.h"
#include "Assets.h"
#include "Game.h"
#include "Achievements.h"

CCollectedAchievements::CCollectedAchievements()
{
	for(int i = 0; i < 10; i++)
	m_nAlpha[i] = 100;
	m_pPlayer = CPlayer::GetInstance();
	m_rRect.bottom = 70;
	m_rRect.right = 425;
	m_rRect.left = m_rRect.top = 0;
}

CCollectedAchievements::~CCollectedAchievements()
{
}

CCollectedAchievements* CCollectedAchievements::GetInstance()
{
	static CCollectedAchievements instance;
	return &instance;
}

void CCollectedAchievements::Enter()
{
	CBaseMenuState::Enter();
	SetBGImageID(GetAssets()->aAchievementBGID);
	SetBGHeight(GetTM()->GetTextureHeight(GetAssets()->aAchievementBGID));
	SetBGWidth(GetTM()->GetTextureWidth(GetAssets()->aAchievementBGID));
	//CenterBGImage();
	GetFMOD()->PlaySound(GetAssets()->aMinimalKesoID);
	GetFMOD()->SetVolume(GetAssets()->aMinimalKesoID  , GetGame()->GetMusicVolume());

}

bool CCollectedAchievements::Input(float fElapsedTime, POINT mousePT)
{
	if(GetDI()->JoystickButtonPressed(1,0) || GetDI()->KeyPressed(DIK_ESCAPE) || GetDI()->MouseButtonPressed(MOUSE_LEFT) || GetDI()->KeyPressed(DIK_ESCAPE))
	{
		GetGame()->ChangeState(CMainMenuState::GetInstance());
	}
	return true;
}

void CCollectedAchievements::Render()
{
	//draw each of the achievements 
	//write description next to each
	GetTM()->DrawWithZSort(GetAssets()->aAchievementBGID, 0,0,0.1f, 0.8f,0.8f, NULL, 0.0f, 0.0f, 0.0f, D3DCOLOR_XRGB(125,125,125));
	GetTM()->Draw(GetAssets()->aAvenged, 10,0, 0.8f,0.8f, &m_rRect, 0.0f,0.0f,0.0f, D3DCOLOR_ARGB(m_nAlpha[7], 255,255,255));
	GetBitmapFont()->DrawString("KILLED THE SHREDDER", 390, 10,0.05f, 0.6f,D3DCOLOR_ARGB(255,0,255,0));
	GetBitmapFont()->DrawString("KILLED THE SHREDDER", 392, 12,0.051f, 0.6f,D3DCOLOR_ARGB(255,255,0,0));
	GetTM()->Draw(GetAssets()->aBreakGame, 10,70, 0.8f,0.8f, &m_rRect, 0.0f,0.0f,0.0f, D3DCOLOR_ARGB(m_nAlpha[3], 255,255,255));
	GetBitmapFont()->DrawString("ALL ACHIEVEMENTS UNLOCKED", 390, 80, 0.05f, 0.6f,D3DCOLOR_ARGB(255,0,255,0));
	GetBitmapFont()->DrawString("ALL ACHIEVEMENTS UNLOCKED", 392, 82, 0.051f, 0.6f,D3DCOLOR_ARGB(255,255,0,0));
	GetTM()->Draw(GetAssets()->aCowabunga, 10,150, 0.8f,0.8f, &m_rRect, 0.0f,0.0f,0.0f, D3DCOLOR_ARGB(m_nAlpha[1], 255,255,255));
	GetBitmapFont()->DrawString("LEVEL UP", 390, 160,0.05f, 0.6f,D3DCOLOR_ARGB(255,0,255,0));
	GetBitmapFont()->DrawString("LEVEL UP", 392, 162,0.051f, 0.6f,D3DCOLOR_ARGB(255,255,0,0));
	GetTM()->Draw(GetAssets()->aFlipNow, 10,230, 0.8f,0.8f, &m_rRect, 0.0f,0.0f,0.0f, D3DCOLOR_ARGB(m_nAlpha[2], 255,255,255));
	GetBitmapFont()->DrawString("ALL TURTLES DEAD", 390, 240,0.05f, 0.6f,D3DCOLOR_ARGB(255,0,255,0));
	GetBitmapFont()->DrawString("ALL TURTLES DEAD", 392, 242,0.051f, 0.6f,D3DCOLOR_ARGB(255,255,0,0));
	GetTM()->Draw(GetAssets()->aLessonInPain, 10,310, 0.8f,0.8f, &m_rRect, 0.0f,0.0f,0.0f, D3DCOLOR_ARGB(m_nAlpha[5], 255,255,255));
	GetBitmapFont()->DrawString("KILL 30 ENEMIES", 390, 320,0.05f, 0.6f,D3DCOLOR_ARGB(255,0,255,0));
	GetBitmapFont()->DrawString("KILL 30 ENEMIES", 392, 322,0.051f, 0.6f,D3DCOLOR_ARGB(255,255,0,0));
	GetTM()->Draw(GetAssets()->aMegaKill, 10,390, 0.8f,0.8f, &m_rRect, 0.0f,0.0f,0.0f, D3DCOLOR_ARGB(m_nAlpha[6], 255,255,255));
	GetBitmapFont()->DrawString("1 TURTLE KILLS 3 NINJAS", 400, 390,0.05f, 0.6f,D3DCOLOR_ARGB(255,0,255,0));
	GetBitmapFont()->DrawString("1 TURTLE KILLS 3 NINJAS", 402, 392,0.051f, 0.6f,D3DCOLOR_ARGB(255,255,0,0));
	GetTM()->Draw(GetAssets()->aPartyTime, 10,470, 0.8f,0.8f, &m_rRect, 0.0f,0.0f,0.0f, D3DCOLOR_ARGB(m_nAlpha[0], 255,255,255));
	GetBitmapFont()->DrawString("START NEW GAME", 390, 480,0.05f, 0.6f,D3DCOLOR_ARGB(255,0,255,0));
	GetBitmapFont()->DrawString("START NEW GAME", 392, 482,0.051f, 0.6f,D3DCOLOR_ARGB(255,255,0,0));
	GetTM()->Draw(GetAssets()->aQuickFingers, 10,550, 0.8f,0.8f, &m_rRect, 0.0f,0.0f,0.0f, D3DCOLOR_ARGB(m_nAlpha[9], 255,255,255));
	GetBitmapFont()->DrawString("100 PERCENT ON QTE", 390, 560,0.05f, 0.6f,D3DCOLOR_ARGB(255,0,255,0));
	GetBitmapFont()->DrawString("100 PERCENT ON QTE", 392, 562,0.051f, 0.6f,D3DCOLOR_ARGB(255,255,0,0));
	GetTM()->Draw(GetAssets()->aStrikeHard, 10,630, 0.8f,0.8f, &m_rRect, 0.0f,0.0f,0.0f, D3DCOLOR_ARGB(m_nAlpha[4], 255,255,255));
	GetBitmapFont()->DrawString("FIRST MAP COMPLETE", 390, 640,0.05f, 0.6f,D3DCOLOR_ARGB(255,0,255,0));
	GetBitmapFont()->DrawString("FIRST MAP COMPLETE", 392, 642,0.051f, 0.6f,D3DCOLOR_ARGB(255,255,0,0));
	GetTM()->Draw(GetAssets()->aTrueNinja, 10,710, 0.8f,0.8f, &m_rRect, 0.0f,0.0f,0.0f, D3DCOLOR_ARGB(m_nAlpha[8], 255,255,255));
	GetBitmapFont()->DrawString("UNLOCK ALL SKILLS", 390, 720,0.05f, 0.6f,D3DCOLOR_ARGB(255,0,255,0));
	GetBitmapFont()->DrawString("UNLOCK ALL SKILLS", 392, 722,0.051f, 0.6f,D3DCOLOR_ARGB(255,255,0,0));

	GetBitmapFont()->DrawString("ESC OR CLICK",		  825, 700, 0.05f, 0.4f, D3DCOLOR_ARGB(255,0,255,0));
	GetBitmapFont()->DrawString("TO GO BACK",		  840, 735, 0.05f, 0.4f, D3DCOLOR_ARGB(255,0,255,0));

}

void CCollectedAchievements::Exit()
{
	GetFMOD()->StopSound(GetAssets()->aMinimalKesoID);
	GetFMOD()->ResetSound(GetAssets()->aMinimalKesoID);
	CBaseMenuState::Exit();
}

void CCollectedAchievements::Update(float fElapsedTime)
{
	CBaseMenuState::Update(fElapsedTime);
	for(int i = 0; i< 10; i++)
	{
		if(m_pPlayer->GetAch()->GetLocked(i) == true)
		{
			m_nAlpha[i] = 255;
		}
		else
		{
			m_nAlpha[i] = 100;
		}
	}
}