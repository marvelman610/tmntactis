//////////////////////////////////////////////////////////////////////////
//	Filename	:	HowToPlayMenuState.cpp
//
//	Author		:	Ramon Johannessen (RJ)
//
//	Purpose		:	The How to Play Menu will display information on how to
//					play the game. 
//////////////////////////////////////////////////////////////////////////
#include "HowToPlayMenuState.h"
#include "BaseMenuState.h"
#include "MainMenuState.h"
#include "CSGD_TextureManager.h"
#include "CSGD_DirectInput.h"
#include "BitmapFont.h"
#include "Assets.h"
#include "Game.h"

// if we have any menu options in this menu, 
//	add to this enum
enum {BACK, NULL_END};

CHowToPlayMenuState* CHowToPlayMenuState::GetInstance()
{
	static CHowToPlayMenuState menuState;
	return &menuState;
}
void CHowToPlayMenuState::Enter()
{
	CBaseMenuState::Enter();
	SetBGImageID(GetAssets()->aHTPMSelectID);
 	SetBGWidth(GetTM()->GetTextureWidth(GetAssets()->aHTPMSelectID));
 	SetBGHeight(GetTM()->GetTextureHeight(GetAssets()->aHTPMSelectID));
	//CenterBGImage();

	//m_fmsBGMusicID		= m_pAssets->m_fmsMMBGmusic;

	SetCurrMenuSelection( BACK );
	SetCursorX(350);
	SetCursorY(335);
	//m_pFMODsys->Play(FMOD_CHANNEL_FREE, m_fmsBGMusicID, false, FMOD_CHANNEL_REUSE);
}

void CHowToPlayMenuState::Exit()
{

}

bool CHowToPlayMenuState::Input(float fElapsedTime, POINT mousePT)
{
	if(GetDI()->KeyPressed(DIK_RETURN))
	{
		if(GetBGImageID() == GetAssets()->aHTPMSelectID)
		{
			SetBGImageID(GetAssets()->aHTPMMoveID);
		}
		else if(GetBGImageID() == GetAssets()->aHTPMMoveID)
		{
			SetBGImageID(GetAssets()->aHTPMAttackID);
		}
		else if(GetBGImageID() == GetAssets()->aHTPMAttackID)
		{
			SetBGImageID(GetAssets()->aHTPMSpecialID);
		}
		else if(GetBGImageID() == GetAssets()->aHTPMSpecialID)
		{
			SetBGImageID(GetAssets()->aHTPMItemID);
		}
		else if(GetBGImageID() == GetAssets()->aHTPMItemID)
		{
			SetBGImageID(GetAssets()->aHTPMSelectID);
		}
	}
	if (GetDI()->KeyPressed(DIK_ESCAPE))
	{
		CGame::GetInstance()->ChangeState(CMainMenuState::GetInstance());
	}
	return true;
}

void CHowToPlayMenuState::Render()
{
	CBaseMenuState::Render();

	// TODO:: add the how to play info here
	CBitmapFont::GetInstance()->DrawString("ESC", 512, 730);
}

void CHowToPlayMenuState::Update(float fElapsedTime)
{
	CBaseMenuState::Update(fElapsedTime);
	// TODO:: add update code here

}
