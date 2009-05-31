//////////////////////////////////////////////////////////////////////////
//	Filename	:	MainMenuState.cpp
//
//	Author		:	Ramon Johannessen (RJ)
//
//	Purpose		:	The Main Menu will be the first menu loaded when the game
//					starts. It will display all menu selections available.
//////////////////////////////////////////////////////////////////////////

#include "MainMenuState.h"
#include "CSGD_TextureManager.h"
#include "CSGD_DirectInput.h"
#include "Game.h"
#include "GamePlayState.h"
#include "BitmapFont.h"
#include "Assets.h"
#include "Player.h"

enum {PLAY, LOAD, OPTIONS, CREDITS, HOWTOPLAY, EXIT, NULL_END };

CMainMenuState::CMainMenuState()
{

}

CMainMenuState::~CMainMenuState()
{

}

CMainMenuState* CMainMenuState::GetInstance()
{
	static CMainMenuState menuState;
	return &menuState;
}
void CMainMenuState::Enter()
{
	CBaseMenuState::Enter();
	SetBGImageID(GetAssets()->aMMBGimageID);
	SetBGWidth(GetTM()->GetTextureWidth(GetAssets()->aMMBGimageID));
	SetBGHeight(GetTM()->GetTextureHeight(GetAssets()->aMMBGimageID));
	CenterBGImage();
	//m_fmsBGMusicID		= m_pAssets->m_fmsMMBGmusic;

	SetCurrMenuSelection( PLAY );
	SetMenuX(400);
	SetCursorX(GetMenuX()-80);
	SetCursorY(335);
	//m_pFMODsys->Play(FMOD_CHANNEL_FREE, m_fmsBGMusicID, false, FMOD_CHANNEL_REUSE);
}

bool CMainMenuState::Input(float fElapsedTime, POINT mousePt)
{
	if (GetDI()->KeyPressed(DIK_DOWN))
	{
		SetCurrMenuSelection(GetCurrMenuSelection() +1);
		if (GetCurrMenuSelection() == NULL_END)
			SetCurrMenuSelection(PLAY);
	}
	else if (GetDI()->KeyPressed(DIK_UP))
	{
		SetCurrMenuSelection(GetCurrMenuSelection() -1);
		if (GetCurrMenuSelection() < PLAY)
			SetCurrMenuSelection(NULL_END-1);
	}
	else if (GetDI()->KeyPressed(DIK_RETURN))
	{
		switch(GetCurrMenuSelection())
		{
		case PLAY:
			CPlayer::GetInstance()->NewGame();
			CGame::GetInstance()->ChangeState(CGamePlayState::GetInstance());
			break;
		case OPTIONS:
			//CGame::GetInstance()->ChangeState(COptionsMenuState::GetInstance());
			break;
		case HOWTOPLAY:
			//CGame::GetInstance ()->ChangeState(CHowToPlayMenuState::GetInstance ());
			break;
		case CREDITS:
			//CGame::GetInstance ()->ChangeState(CCreditsMenuState::GetInstance ());
			break;
		case LOAD:
			// TODO:: call to LoadSavedGame would be in the Load Menu State
			CPlayer::GetInstance()->LoadSavedGame("SavedGames/savedGame.dat");
			//CGame::GetInstance()->ChangeState(CLoadMenuState::GetInstance());
		case EXIT:
			CGame::GetInstance()->SetIsRunning(false);
			return false;
			//break;
		}
	}
	return true;
}

void CMainMenuState::Render()
{
	CBaseMenuState::Render();
	// Draw menu item text
	GetBitmapFont()->DrawStringAutoCenter("T M N T",		GetScreenWidth(), 20, 1.5f);
	GetBitmapFont()->DrawStringAutoCenter("T A C T I C S",	GetScreenWidth(), 100, 1.5f);
	GetBitmapFont()->DrawString("P L A Y",			GetMenuX(), 350, 1.0f);
	GetBitmapFont()->DrawString("L O A D",			GetMenuX(), 350+GetMenuItemSpacing(), 1.0f);
	GetBitmapFont()->DrawString("O P T I O N S",	GetMenuX(), 350+GetMenuItemSpacing() * 2, 1.0f);
	GetBitmapFont()->DrawString("C R E D I T S",	GetMenuX(), 350+GetMenuItemSpacing() * 3, 1.0f);
	GetBitmapFont()->DrawString("T U T O R I A L",	GetMenuX(), 350+GetMenuItemSpacing() * 4, 1.0f);
	GetBitmapFont()->DrawString("E X I T",			GetMenuX(), 350+GetMenuItemSpacing() * 5, 1.0f);
	// Draw menu cursor
	GetTM()->DrawWithZSort(GetAssets()->aMenuCursorImageID, GetCursorX(), GetCursorY() + (GetCurrMenuSelection()*GetMenuItemSpacing()), 0);
}

void CMainMenuState::Update(float fElapsedTime)
{

}


void CMainMenuState::Exit()
{
	CBaseMenuState::Exit();
}