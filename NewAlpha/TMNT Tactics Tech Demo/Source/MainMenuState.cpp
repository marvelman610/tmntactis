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
#include "CollectedAchievements.h"
#include "Box.h"
#include <fstream>
using std::fstream;

enum {SIGNIN, PLAY, CONTINUE, OPTIONS, CREDITS, HOWTOPLAY, ACHIEVEMENTS, EXIT, NULL_END };

CMainMenuState::CMainMenuState()
{
	m_nNumProfiles = 0;
	m_sProfiles[0] = "LOG IN";
	for (int i =1; i < 5; ++i)
	{
		m_sProfiles[i] = "Create New";
	}
	ifstream ifs("Profiles.dat", ios_base::binary);
	if (ifs.is_open())
	{
		for (int i = 1; i < 5; ++i)
		{
			if (!ifs.eof())
				ifs.read(reinterpret_cast<char*>(&m_sProfiles[i]), sizeof(string));
			else
				break;
			++m_nNumProfiles;
		}
	}
}

CMainMenuState::~CMainMenuState()
{
	if (m_bxMsg)
	{delete m_bxMsg; m_bxMsg = NULL;}
	if (m_bxProfile)
	{delete m_bxProfile; m_bxProfile = NULL;}
}

CMainMenuState* CMainMenuState::GetInstance()
{
	static CMainMenuState menuState;
	return &menuState;
}

void CMainMenuState::Enter()
{
	m_bNewGamePressed = false;
	ifstream ifs("Profiles.dat", ios_base::binary);
	if (ifs.is_open())
	{
		for (int i = 1; i < 5; ++i)
		{
			if (!ifs.eof())
				ifs.read(reinterpret_cast<char*>(&m_sProfiles[i]), sizeof(string));
			else
				break;
			++m_nNumProfiles;
		}
	}
	m_bxMsg = NULL; m_bxProfile = NULL;
	m_bDisplayLoadBox = false;
	CBaseMenuState::Enter();
	SetBGImageID(GetAssets()->aMMBGimageID);
	SetBGWidth(GetTM()->GetTextureWidth(GetAssets()->aMMBGimageID));
	SetBGHeight(GetTM()->GetTextureHeight(GetAssets()->aMMBGimageID));
	CenterBGImage();

	SetCurrMenuSelection( PLAY );
	SetMenuX(350); SetMenuY(350);
	SetCursorX(GetMenuX()-80); SetCursorY(GetMenuY()-15);

	GetFMOD()->PlaySound(GetAssets()->aMMmusicID);
	GetFMOD()->SetVolume(GetAssets()->aMMmusicID, GetGame()->GetMusicVolume());

	if (CPlayer::GetInstance()->GetProfName() == "NONE")
	{
		m_bxProfile = new CBox(5, m_sProfiles, 300, 300, 0.11f, true, 35, 35, 25, -1, 0.75f);
		m_bxProfile->SetActive();
		m_bxProfile->SetType(BOX_WITH_BACK);
		m_bxProfile->AcceptInput();
	}
}

bool CMainMenuState::Input(float fElapsedTime, POINT mousePt)
{
	CBaseMenuState::Input(fElapsedTime, mousePt);

	if (mousePt.y != m_nMouseY && !m_bxProfile && !m_bxMsg)
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

	// entering a profile name
	if (m_bxProfile)
	{
		int input = m_bxProfile->Input(mousePt);
		if (GetDI()->MouseButtonPressed(MOUSE_LEFT) || (GetDI()->KeyPressed(DIK_RETURN) && m_bxProfile->GetItems()[m_bxProfile->GetInputIndex()].size() > 0))
		{
			int index = m_bxProfile->GetInputIndex();
			if (input == BTN_ENTER && index > -1 && m_bxProfile->GetItems()[index] != "Create New"
				&& m_bxProfile->GetItems()[index].size() > 0)
			{
				// set the profile name
				CPlayer::GetInstance()->SetProfileName(m_bxProfile->GetItems()[index]);
				m_sProfiles[index] = m_bxProfile->GetItems()[index];
				delete m_bxProfile;
				m_bxProfile = NULL;
				++m_nNumProfiles;

				// save the profile names
				ofstream ofs("Profiles.dat", ios_base::binary);
				for (int i= 1; i < m_nNumProfiles+1; ++i)
				{
					ofs.write((char*)(&m_sProfiles[i]), sizeof(m_sProfiles[i]));
				}
				ofs.close();

				// load the profile's saved game if one exists

//				ifstream ifs("SavedFiles.dat", ios_base::binary);
// 				if (ifs.is_open())
// 				{
// 					ifs.close();
// 					// load the save game..it exists already
// 					string fileName = CPlayer::GetInstance()->GetProfName() + ".dat";
// 					CGamePlayState::GetInstance()->LoadGame(fileName.c_str());
// 				}
				if (m_bNewGamePressed)
				{
					CPlayer::GetInstance()->NewGame();
					CGame::GetInstance()->ChangeState(CGamePlayState::GetInstance());
				}
					// no saved game yet...

			}
			else if (input == BTN_BACK) // save nothing
			{
				delete m_bxProfile; 
				m_bxProfile = NULL;
			}
		}
		return true;
	}

	if(GetDI()->JoystickDPadPressed(2, 0) ) //0 = left, 1 = right, 2 = up, 3 = down
	{
		SetCurrMenuSelection(GetCurrMenuSelection() -1);
		if (GetCurrMenuSelection() < PLAY)
			SetCurrMenuSelection(NULL_END-1);
	}
	else if(GetDI()->JoystickDPadPressed(3, 0) ) //0 = left, 1 = right, 2 = up, 3 = down
	{
		SetCurrMenuSelection(GetCurrMenuSelection() +1);
		if (GetCurrMenuSelection() == NULL_END)
			SetCurrMenuSelection(PLAY);
	}
	// a = 0, b = 1, x = 2, y = 3, lb = 4, rb = 5, select = 6, start = 7, lstick button = 8
	// rstick button = 9,  triggers??
	/*if(GetDI()->JoystickButtonPressed(0,0) )
	{
	}*/

	if (GetDI()->KeyPressed(DIK_DOWN))
	{
		SetCurrMenuSelection(GetCurrMenuSelection() +1);
		if (GetCurrMenuSelection() == NULL_END)
			SetCurrMenuSelection(SIGNIN);
	}
	else if (GetDI()->KeyPressed(DIK_UP))
	{
		SetCurrMenuSelection(GetCurrMenuSelection() -1);
		if (GetCurrMenuSelection() < SIGNIN)
			SetCurrMenuSelection(NULL_END-1);
	}
	else if ((GetDI()->KeyPressed(DIK_RETURN) || GetDI()->MouseButtonPressed(MOUSE_LEFT) 
		|| GetDI()->JoystickButtonPressed(0,0) || GetDI()->JoystickButtonPressed(7,0)))
	{
		GetFMOD()->PlaySound(GetAssets()->aMMmenuClickSnd);
		GetFMOD()->SetVolume(GetAssets()->aMMmenuClickSnd, GetGame()->GetSFXVolume());
		while (GetFMOD()->IsSoundPlaying(GetAssets()->aMMmenuClickSnd)) {}

		switch(GetCurrMenuSelection())
		{
		case SIGNIN:
			{
				m_bxProfile = new CBox(5, m_sProfiles, 300, 300, 0.11f, true, 35, 35, 25, -1, 0.75f);
				m_bxProfile->SetActive();
				m_bxProfile->SetType(BOX_WITH_BACK);
				m_bxProfile->AcceptInput();
			}
			break;
		case PLAY:
			m_bNewGamePressed = true;
			if (CPlayer::GetInstance()->GetProfName() == "NONE")
			{
				m_bxProfile = new CBox(5, m_sProfiles, 300, 300, 0.11f, true, 35, 35, 25, -1, 0.75f);
				m_bxProfile->SetActive();
				m_bxProfile->SetType(BOX_WITH_BACK);
				m_bxProfile->AcceptInput();
				break;
			}
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
		case CONTINUE:
			{
					// no saved
				if (CPlayer::GetInstance()->GetProfName() == "NONE")
				{
					m_bxProfile = new CBox(5, m_sProfiles, 250, 300, 0.11f, true, 35, 35, 25, -1, 0.75f);
					m_bxProfile->SetActive();
					m_bxProfile->SetType(BOX_WITH_BACK);
					m_bxProfile->AcceptInput();
				}
				else
				{
					string name = CPlayer::GetInstance()->GetProfName() + ".dat";
					CGamePlayState::GetInstance()->LoadGame(name.c_str());
				}
				break;
			}
		case ACHIEVEMENTS:
			{
				CGame::GetInstance()->ChangeState(CCollectedAchievements::GetInstance());
			}
			break;
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
	GetBitmapFont()->DrawStringAutoCenter("TMNT",		GetScreenWidth(), 20, 0.09f, 1.5f, D3DCOLOR_ARGB(255,0,255,0));
	GetBitmapFont()->DrawStringAutoCenter("TMNT",		GetScreenWidth()+6, 26, 0.091f, 1.5f, D3DCOLOR_ARGB(255,255,0,0));
	GetBitmapFont()->DrawStringAutoCenter("TACTICS",	GetScreenWidth(), 100, 0.09f, 1.5f, D3DCOLOR_ARGB(255,0,255,0));
	GetBitmapFont()->DrawStringAutoCenter("TACTICS",	GetScreenWidth()+6, 106, 0.091f, 1.5f, D3DCOLOR_ARGB(255,255,0,0));
	if (m_bxProfile)
		m_bxProfile->Render();
	else if (m_bxMsg)
		m_bxMsg->Render();
	else
	{
		// Draw menu item text
		GetBitmapFont()->DrawString("SIGN IN",		GetMenuX(), GetMenuY(), 0.09f, 1.0f, color);
		GetBitmapFont()->DrawString("NEW GAME",		GetMenuX(), GetMenuY()+GetMenuItemSpacing(), 0.09f, 1.0f, color);
		GetBitmapFont()->DrawString("CONTINUE",		GetMenuX(), GetMenuY()+GetMenuItemSpacing() * 2, 0.09f, 1.0f, color);
		GetBitmapFont()->DrawString("OPTIONS",		GetMenuX(), GetMenuY()+GetMenuItemSpacing() * 3, 0.09f, 1.0f, color);
		GetBitmapFont()->DrawString("CREDITS",		GetMenuX(), GetMenuY()+GetMenuItemSpacing() * 4, 0.09f, 1.0f, color);
		GetBitmapFont()->DrawString("TUTORIAL",		GetMenuX(), GetMenuY()+GetMenuItemSpacing() * 5, 0.09f, 1.0f, color);
		GetBitmapFont()->DrawString("ACHIEVEMENTS", GetMenuX(), GetMenuY()+GetMenuItemSpacing() * 6, 0.09f, 1.0f, color);
		GetBitmapFont()->DrawString("EXIT",			GetMenuX(), GetMenuY()+GetMenuItemSpacing() * 7, 0.09f, 1.0f, color);
		// Draw menu cursor
		GetTM()->DrawWithZSort(GetAssets()->aMenuCursorImageID, GetCursorX(), GetCursorY() + (GetCurrMenuSelection()*GetMenuItemSpacing()), 0.01f);
	}
}

void CMainMenuState::Update(float fElapsedTime)
{

}


void CMainMenuState::Exit()
{
	m_bNewGamePressed = false;
	GetFMOD()->StopSound(GetAssets()->aMMmusicID);
	GetFMOD()->ResetSound(GetAssets()->aMMmusicID);

	// saving any profile info -- the actual saved game file will be the profile name + .dat
	if (m_nNumProfiles > 0)
	{
		fstream ofs("Profiles.dat", ios_base::binary);
		for (int i= 1; i < m_nNumProfiles+1; ++i)
		{
			ofs.write((char*)(&m_sProfiles[i]), sizeof(m_sProfiles[i]));
		}
		ofs.close();
	}
	if (m_bxMsg)
	{
		delete m_bxMsg;
		m_bxMsg = NULL;
	}
	if (m_bxProfile)
	{
		delete m_bxProfile;
		m_bxProfile = NULL;
	}
	m_bDisplayLoadBox = false;
	CBaseMenuState::Exit();
}