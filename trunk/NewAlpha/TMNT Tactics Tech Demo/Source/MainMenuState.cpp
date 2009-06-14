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
#include "CSGD_FModManager.h"
#include "Game.h"
#include "GamePlayState.h"
#include "BitmapFont.h"
#include "Assets.h"
#include "Player.h"
#include "HowToPlayMenuState.h"
#include "OptionsMenuState.h"
#include "CreditState.h"

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

	SetCurrMenuSelection( PLAY );
	SetMenuX(400); SetMenuY(350);
	SetCursorX(GetMenuX()-80); SetCursorY(GetMenuY()-15);
}

bool CMainMenuState::Input(float fElapsedTime, POINT mousePt)
{
	CBaseMenuState::Input(fElapsedTime, mousePt);

	if (mousePt.y != m_nMouseY)
	{
		int oldSelection = GetCurrMenuSelection(); 
		int newSelection = (mousePt.y - GetMenuY()) / GetMenuItemSpacing();
		if ( oldSelection != newSelection )
		{
			SetCurrMenuSelection( newSelection );
			if (GetCurrMenuSelection() < 0)
				SetCurrMenuSelection(PLAY);
			else if (GetCurrMenuSelection() > NULL_END-1)
				SetCurrMenuSelection(NULL_END-1);
			if (GetFMOD()->IsSoundPlaying(GetAssets()->aMMmenuMoveSnd))
			{
				GetFMOD()->StopSound(GetAssets()->aMMmenuMoveSnd);
				GetFMOD()->ResetSound(GetAssets()->aMMmenuMoveSnd);
			}
			GetFMOD()->PlaySound(GetAssets()->aMMmenuMoveSnd);
			if(!GetFMOD()->SetVolume(GetAssets()->aMMmenuMoveSnd, GetGame()->GetSFXVolume()*0.6f))
				MessageBox(0, "VOLUME NOT SET", "ERROR", MB_OK);
		}
	}
	m_nMouseX = mousePt.x;
	m_nMouseY = mousePt.y;

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
	else if (GetDI()->KeyPressed(DIK_RETURN) || GetDI()->MouseButtonPressed(MOUSE_LEFT))
	{
		GetFMOD()->PlaySound(GetAssets()->aMMmenuClickSnd);
		GetFMOD()->SetVolume(GetAssets()->aMMmenuClickSnd, GetGame()->GetSFXVolume());
		while (GetFMOD()->IsSoundPlaying(GetAssets()->aMMmenuClickSnd)) {}

		switch(GetCurrMenuSelection())
		{
		case PLAY:
			CPlayer::GetInstance()->NewGame();
			CGame::GetInstance()->ChangeState(CGamePlayState::GetInstance());
			break;
		case OPTIONS:
			CGame::GetInstance()->ChangeState(COptionsMenuState::GetInstance());
			break;
		case HOWTOPLAY:
			CGame::GetInstance()->ChangeState(CHowToPlayMenuState::GetInstance());
			break;
		case CREDITS:
			CGame::GetInstance()->ChangeState(CCreditState::GetInstance());
			break;
		case LOAD:
			// TODO:: call to LoadSavedGame would be in the Load Menu State
			CPlayer::GetInstance()->LoadSavedGame("SavedGames/savedGame.dat");
			//CGame::GetInstance()->ChangeState(CLoadMenuState::GetInstance());
			break;
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
	GetBitmapFont()->DrawStringAutoCenter("TMNT",		GetScreenWidth(), 20, 0.09f, 1.5f);
	GetBitmapFont()->DrawStringAutoCenter("TACTICS",	GetScreenWidth(), 100, 0.09f, 1.5f);
	GetBitmapFont()->DrawString("P L A Y",			GetMenuX(), GetMenuY(), 0.09f, 1.0f);
	GetBitmapFont()->DrawString("L O A D",			GetMenuX(), GetMenuY()+GetMenuItemSpacing(), 0.09f, 1.0f);
	GetBitmapFont()->DrawString("O P T I O N S",	GetMenuX(), GetMenuY()+GetMenuItemSpacing() * 2, 0.09f, 1.0f);
	GetBitmapFont()->DrawString("C R E D I T S",	GetMenuX(), GetMenuY()+GetMenuItemSpacing() * 3, 0.09f, 1.0f);
	GetBitmapFont()->DrawString("T U T O R I A L",	GetMenuX(), GetMenuY()+GetMenuItemSpacing() * 4, 0.09f, 1.0f);
	GetBitmapFont()->DrawString("E X I T",			GetMenuX(), GetMenuY()+GetMenuItemSpacing() * 5, 0.09f, 1.0f);
	// Draw menu cursor
	GetTM()->DrawWithZSort(GetAssets()->aMenuCursorImageID, GetCursorX(), GetCursorY() + (GetCurrMenuSelection()*GetMenuItemSpacing()), 0);
	GetTM()->Draw(GetAssets()->aMousePointerID, m_nMouseX-10, m_nMouseY-3);
}

void CMainMenuState::Update(float fElapsedTime)
{

}


void CMainMenuState::Exit()
{
	CBaseMenuState::Exit();
}