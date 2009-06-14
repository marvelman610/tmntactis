/////////////////////////////////////////////////////////////////////////////
//	Filename	:	CCreditState.cpp
//
//	Author		:	Jose Sierra (JS)
//
//	Purpose		:	To let the player view the game credits
/////////////////////////////////////////////////////////////////////////////
#include "CreditState.h"
#include "MainMenuState.h"
#include "CSGD_TextureManager.h"
#include "CSGD_DirectInput.h"
#include "CSGD_FModManager.h"
#include "BitmapFont.h"
#include "Assets.h"
#include "Game.h"

CCreditState::CCreditState()
{
}

CCreditState::~CCreditState()
{
}

CCreditState* CCreditState::GetInstance()
{
	static CCreditState creditState;
	return &creditState;
}

void CCreditState::Enter()
{
	CBaseMenuState::Enter();
	GetFMOD()->PlaySound(GetAssets()->aCMmusicID);
	GetFMOD()->SetVolume(GetAssets()->aCMmusicID, GetGame()->GetMusicVolume());
}

bool CCreditState::Input(float fElapsedTimes, POINT mousePT)
{
	
	if(GetDI()->KeyPressed(DIK_ESCAPE))
	{
		GetGame()->ChangeState(CMainMenuState::GetInstance());
	}
	return true;
}

void CCreditState::Render()
{
	GetBitmapFont()->DrawString("ESC", 440, 384, 1.0f, D3DCOLOR_ARGB(255,255,0,0));
}

void CCreditState::Exit()
{
	GetFMOD()->StopSound(GetAssets()->aCMmusicID);
	GetFMOD()->ResetSound(GetAssets()->aCMmusicID);
	CBaseMenuState::Exit();
}