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
#include "Box.h"

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
	m_bDisplayLoadBox = false;
	CBaseMenuState::Enter();
	SetBGImageID(GetAssets()->aMMBGimageID);
	SetBGWidth(GetTM()->GetTextureWidth(GetAssets()->aMMBGimageID));
	SetBGHeight(GetTM()->GetTextureHeight(GetAssets()->aMMBGimageID));
	CenterBGImage();

	SetCurrMenuSelection( PLAY );
	SetMenuX(320); SetMenuY(350);
	SetCursorX(GetMenuX()-80); SetCursorY(GetMenuY()-15);

	GetFMOD()->PlaySound(GetAssets()->aMMmusicID);
	GetFMOD()->SetVolume(GetAssets()->aMMmusicID, GetGame()->GetMusicVolume());
}

bool CMainMenuState::Input(float fElapsedTime, POINT mousePt)
{
	CBaseMenuState::Input(fElapsedTime, mousePt);

	if (mousePt.y != m_nMouseY)
	{
		int oldSelection = GetCurrMenuSelection(); 
		int newSelection = (mousePt.y - GetMenuY()) / GetMenuItemSpacing();
		if (newSelection < 0)
			newSelection = 0;
		else if (newSelection > NULL_END-1)
			newSelection = NULL_END-1;
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
	// loading "screen"
	if (m_bDisplayLoadBox)
	{
		int input = m_bxLoadGame->Input(mousePt);
		if (GetDI()->MouseButtonPressed(MOUSE_LEFT))
		{
			if (input == 1)
			{
				const char* file = m_bxLoadGame->GetItems()[1].c_str();
				CPlayer::GetInstance()->LoadSavedGame(file);
			}
			else if (input == 100)
			{ delete m_bxLoadGame; m_bxLoadGame = NULL; m_bDisplayLoadBox = false;}
		}
		return true;
	}

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
	else if ((GetDI()->KeyPressed(DIK_RETURN) || GetDI()->MouseButtonPressed(MOUSE_LEFT)))
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
			{
				// TODO:: get the current profile's saved game, set up the load game box, handle results elsewhere
				string* sLoadGame = new string[2];
				sLoadGame[0] = "LOAD GAME"; sLoadGame[1] = "Bob's saved game";
				m_bxLoadGame = new CBox(2, sLoadGame, 230, 300, 0.11f, true);
				m_bxLoadGame->SetType(BOX_WITH_BACK);
				m_bxLoadGame->SetActive();
				m_bDisplayLoadBox = true;
				delete[] sLoadGame;
				break;
			}
		case EXIT:
			CGame::GetInstance()->SetIsRunning(false);
			return false;
		}
	}
	return true;
}

void CMainMenuState::Render()
{
	CBaseMenuState::Render();
	DWORD color = D3DCOLOR_XRGB(0, 255, 0);	// draw bitmap font text green!!
	GetTM()->DrawWithZSort(GetAssets()->aMousePointerID, m_nMouseX-10, m_nMouseY-3, 0.0f);
	GetBitmapFont()->DrawStringAutoCenter("TMNT",		GetScreenWidth(), 20, 0.09f, 1.5f, color);
	GetBitmapFont()->DrawStringAutoCenter("TACTICS",	GetScreenWidth(), 100, 0.09f, 1.5f, color);
	if (m_bxLoadGame)
		m_bxLoadGame->Render();
	else
	{
		// Draw menu item text
		GetBitmapFont()->DrawString("N E W  G A M E",	GetMenuX(), GetMenuY(), 0.09f, 1.0f, color);
		GetBitmapFont()->DrawString("L O A D",			GetMenuX(), GetMenuY()+GetMenuItemSpacing(), 0.09f, 1.0f, color);
		GetBitmapFont()->DrawString("O P T I O N S",	GetMenuX(), GetMenuY()+GetMenuItemSpacing() * 2, 0.09f, 1.0f, color);
		GetBitmapFont()->DrawString("C R E D I T S",	GetMenuX(), GetMenuY()+GetMenuItemSpacing() * 3, 0.09f, 1.0f, color);
		GetBitmapFont()->DrawString("T U T O R I A L",	GetMenuX(), GetMenuY()+GetMenuItemSpacing() * 4, 0.09f, 1.0f, color);
		GetBitmapFont()->DrawString("E X I T",			GetMenuX(), GetMenuY()+GetMenuItemSpacing() * 5, 0.09f, 1.0f, color);
		// Draw menu cursor
		GetTM()->DrawWithZSort(GetAssets()->aMenuCursorImageID, GetCursorX(), GetCursorY() + (GetCurrMenuSelection()*GetMenuItemSpacing()), 0.01f);
	}
}

void CMainMenuState::Update(float fElapsedTime)
{

}


void CMainMenuState::Exit()
{
	GetFMOD()->StopSound(GetAssets()->aMMmusicID);
	GetFMOD()->ResetSound(GetAssets()->aMMmusicID);
	if (m_bxLoadGame)
	{
		delete m_bxLoadGame;
		m_bxLoadGame = NULL;
	}
	m_bDisplayLoadBox = false;
	CBaseMenuState::Exit();
}