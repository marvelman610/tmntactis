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

enum {SIGNIN, NEWGAME, CONTINUE, OPTIONS, CREDITS, HOWTOPLAY, ACHIEVEMENTS, EXIT, NULL_END };

CMainMenuState::CMainMenuState()
{
	m_bProfilesSaved = false;
	m_nNumProfiles = 0;
	m_sProfiles = new string[5];
	m_sProfiles[0] = "LOG IN";
	for (int i =1; i < 5; ++i)
		m_sProfiles[i] = "CREATE NEW";

	m_fTimer = 0.0f;
	m_bGameLoaded = false;
}

CMainMenuState::~CMainMenuState()
{
// 	if (m_bxMsg)
// 	{delete m_bxMsg; m_bxMsg = NULL;}
// 	if (m_bxProfile)
// 	{delete m_bxProfile; m_bxProfile = NULL;}
	delete[] m_sProfiles;
	m_sProfiles = NULL;
}

CMainMenuState* CMainMenuState::GetInstance()
{
	static CMainMenuState menuState;
	return &menuState;
}

void CMainMenuState::Enter()
{
	m_nCurrProfileInd = -1;
	m_fTimer = 0.0f;
	m_bNewGamePressed = m_bProfileSelected = false;
	if (!m_bProfilesSaved)
	{
		m_nNumProfiles = 0;
		ifstream ifs("Profiles.dat", ios_base::binary);
		if (ifs.is_open())
		{
			for (int i = 1; i < 5; ++i)
			{
				char buff[32];
				ZeroMemory(buff, 32);
				int size = 0;
				ifs.read(reinterpret_cast<char*>(&size), sizeof(int));
				ifs.read(buff, size);
				m_sProfiles[i] = buff;
				char next = ifs.peek();
				if(m_sProfiles[i] != "CREATE NEW")
					++m_nNumProfiles;
				if (next == -1)
					break;
			}
			ifs.close();
		}
	}
	m_bxMsg = NULL; m_bxProfile = NULL;
	m_bDisplayLoadBox = false;
	CBaseMenuState::Enter();
	SetBGImageID(GetAssets()->aMMBGimageID);
	SetBGWidth(GetTM()->GetTextureWidth(GetAssets()->aMMBGimageID));
	SetBGHeight(GetTM()->GetTextureHeight(GetAssets()->aMMBGimageID));
	CenterBGImage();

	SetCurrMenuSelection( NEWGAME );
	SetMenuX(410); SetMenuY(350);
	SetCursorX(GetMenuX()-80); SetCursorY(GetMenuY()-15);

	GetFMOD()->PlaySound(GetAssets()->aMMmusicID);
	GetFMOD()->SetVolume(GetAssets()->aMMmusicID, GetGame()->GetMusicVolume());
	
	if (GetGame()->GetProfName() == "NONE")
	{
		m_bxProfile = new CBox(5, m_sProfiles, 300, 300, 0.11f, true, 35, 35, 25, -1, 0.75f);
		m_bxProfile->SetActive();
		m_bxProfile->SetType(BOX_WITH_BACK);
		m_bxProfile->AcceptInput();
		m_bxProfile->CenterBox();
	}
}

bool CMainMenuState::Input(float fElapsedTime, POINT mousePt)
{
	if (m_fTimer < 0.25f)	// stop any input, before the screen is actually loaded
		return true;
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
				SetCurrMenuSelection(NEWGAME);
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
		// canceled
		if (GetDI()->KeyPressed(DIK_ESCAPE) && m_bxProfile->GetInputIndex() == -1)
		{delete m_bxProfile; m_bxProfile = NULL;return true;}

		int input = m_bxProfile->Input(mousePt, fElapsedTime);
		int index = m_bxProfile->GetInputIndex();
		
		if ((GetDI()->MouseButtonPressed(MOUSE_LEFT) || (GetDI()->KeyPressed(DIK_RETURN)) && m_bxProfile->GetItems()[index].size() > 0) && index < 100)
		{
			// have signed in to a profile
			if (input == BTN_ENTER && index > -1 && m_bxProfile->GetItems()[index] != "CREATE NEW"
				&& m_bxProfile->GetItems()[index].size() > 0)
			{
				m_bProfileSelected = true;
				m_Timer.StartTimer(1.5f);
				// set the profile name
				m_sProfiles[index] = m_bxProfile->GetItems()[index];
				string profName = m_sProfiles[index];
				CGame::GetInstance()->SetProfName(profName);
				if (m_bxProfile->GetMadeNew() && !m_bxProfile->GetOverwrote())
				{
					m_bProfilesSaved = false;
					++m_nNumProfiles;
				}
				delete m_bxProfile;
				m_bxProfile = NULL;

				// save the profile names
				if (!m_bProfilesSaved)
				{
					ofstream ofs("Profiles.dat", ios_base::binary);
					for (int i= 1; i < m_nNumProfiles+1; ++i)
					{
						int size = m_sProfiles[i].size();
						ofs.write((char*)(&size), 4);
						const char* sz = m_sProfiles[i].c_str();
						ofs.write(sz, size);
					}
					m_bProfilesSaved = true;
					ofs.close();
				}

				if (m_bNewGamePressed)
					return true;
				else	// they have simply selected a profile to sign in with
				{
					// load the profile's saved game if one exists
					string fileName = CGame::GetInstance()->GetProfName() + ".dat";
					string pathFileName = "Resources/SavedGames/" + fileName;
					ifstream ifs;
					ifs.open(pathFileName.c_str(), ios_base::binary | ios_base::in);
					if (ifs.is_open())
					{
						ifs.close();
						CGamePlayState::GetInstance()->LoadGame(fileName.c_str());
						m_bGameLoaded = true;
					}
					else
						m_bGameLoaded = false;
				}
				m_nCurrProfileInd = index;
				string* items;
				if (!m_bGameLoaded)
				{
					items = new string[3];
					items[0] = "SIGNED IN TO"; items[1] = "-" + profName + "-"; items[2] = "NO SAVED GAME";
					m_bxMsg = new CBox(3, items, 300, 300, 0.11f, true);
					m_bxMsg->IsMsgBox(true); m_bxMsg->CenterText(); m_bxMsg->CenterBox();
					delete[] items;
				} 
				else
				{
					items = new string[3];
					items[0] = "SIGNED IN TO"; items[1] = "-" + profName + "-"; items[2] = "GAME LOADED";
					m_bxMsg = new CBox(3, items, 300, 300, 0.11f, true);
					m_bxMsg->IsMsgBox(true); m_bxMsg->CenterText(); m_bxMsg->CenterBox();
				}
			}
			else if (input == BTN_BACK) // save nothing, load nothing
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
		if (GetCurrMenuSelection() < NEWGAME)
			SetCurrMenuSelection(NULL_END-1);
	}
	else if(GetDI()->JoystickDPadPressed(3, 0) ) //0 = left, 1 = right, 2 = up, 3 = down
	{
		SetCurrMenuSelection(GetCurrMenuSelection() +1);
		if (GetCurrMenuSelection() == NULL_END)
			SetCurrMenuSelection(NEWGAME);
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
				m_bxProfile->CenterBox();
			}
			break;
		case NEWGAME:
			{
				m_bNewGamePressed = true;
				string profName = CGame::GetInstance()->GetProfName();
				if (profName == "NONE")
				{
					m_bxProfile = new CBox(5, m_sProfiles, 300, 300, 0.11f, true, 35, 35, 25, -1, 0.75f);
					m_bxProfile->SetActive();
					m_bxProfile->SetType(BOX_WITH_BACK);
					m_bxProfile->AcceptInput();
					m_bxProfile->CenterBox();
					break;
				}
				else
				{
					CPlayer::GetInstance()->NewGame();
					CGame::GetInstance()->ChangeState(CGamePlayState::GetInstance());
				}
				break;

			}
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
				if (CGame::GetInstance()->GetProfName() == "NONE")
				{
					m_bxProfile = new CBox(5, m_sProfiles, 250, 300, 0.11f, true, 35, 35, 25, -1, 0.75f);
					m_bxProfile->SetActive();
					m_bxProfile->SetType(BOX_WITH_BACK);
					m_bxProfile->AcceptInput();
					m_bxProfile->CenterBox();
				}
				else if (m_bGameLoaded)
				{
					CGame::GetInstance()->ChangeState(CGamePlayState::GetInstance());
				}
				else	// start a new game
				{
					CPlayer::GetInstance()->NewGame();
					CGame::GetInstance()->ChangeState(CGamePlayState::GetInstance());
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
	float scaleTitle = 2.5f;
	float scaleOptions = 1.75f;
	DWORD color = D3DCOLOR_XRGB(0, 255, 0);	// draw bitmap font text green!!
	GetTM()->DrawWithZSort(GetAssets()->aMousePointerID, m_nMouseX-10, m_nMouseY-3, 0.0f);
	GetBitmapFont()->ChangeBMFont(CAssets::GetInstance()->aBitmapFontBubblyID, 16,15,20);
	GetBitmapFont()->DrawStringAutoCenter("TMNT",		GetScreenWidth(), 20, 0.09f, scaleTitle, D3DCOLOR_ARGB(255,0,255,0));
	//GetBitmapFont()->DrawStringAutoCenter("TMNT",		GetScreenWidth()+6, 26, 0.091f, 1.5f, D3DCOLOR_ARGB(255,255,0,0));
	GetBitmapFont()->DrawStringAutoCenter("TACTICS",	GetScreenWidth(), 100, 0.09f, scaleTitle, D3DCOLOR_ARGB(255,0,255,0));
	//GetBitmapFont()->DrawStringAutoCenter("TACTICS",	GetScreenWidth()+6, 106, 0.091f, 1.5f, D3DCOLOR_ARGB(255,255,0,0));
	if (!m_bxProfile && !m_bxMsg)
	{
		// Draw menu item text
		GetBitmapFont()->DrawString("SIGN IN",		GetMenuX(), GetMenuY(), 0.09f, scaleOptions, color);
		GetBitmapFont()->DrawString("NEW GAME",		GetMenuX(), GetMenuY()+GetMenuItemSpacing(), 0.09f, scaleOptions, color);
		GetBitmapFont()->DrawString("CONTINUE",		GetMenuX(), GetMenuY()+GetMenuItemSpacing() * 2, 0.09f, scaleOptions, color);
		GetBitmapFont()->DrawString("OPTIONS",		GetMenuX(), GetMenuY()+GetMenuItemSpacing() * 3, 0.09f, scaleOptions, color);
		GetBitmapFont()->DrawString("CREDITS",		GetMenuX(), GetMenuY()+GetMenuItemSpacing() * 4, 0.09f, scaleOptions, color);
		GetBitmapFont()->DrawString("TUTORIAL",		GetMenuX(), GetMenuY()+GetMenuItemSpacing() * 5, 0.09f, scaleOptions, color);
		GetBitmapFont()->DrawString("ACHIEVEMENTS", GetMenuX(), GetMenuY()+GetMenuItemSpacing() * 6, 0.09f, scaleOptions, color);
		GetBitmapFont()->DrawString("EXIT",			GetMenuX(), GetMenuY()+GetMenuItemSpacing() * 7, 0.09f, scaleOptions, color);
		// Draw menu cursor
		GetTM()->DrawWithZSort(GetAssets()->aMenuCursorImageID, GetCursorX(), GetCursorY() + (GetCurrMenuSelection()*GetMenuItemSpacing()), 0.01f);
	}
	GetBitmapFont()->Reset();
	if (m_bxProfile)
		m_bxProfile->Render();
	else if (m_bxMsg)
		m_bxMsg->Render();
}

void CMainMenuState::Update(float fElapsedTime)
{
	m_fTimer += fElapsedTime;
	bool timerEvents = m_Timer.Update(fElapsedTime);
	
	if (timerEvents)
	{
		if (m_bProfileSelected)
		{ delete m_bxMsg; m_bxMsg = NULL; }
		if (m_bNewGamePressed)
		{
			CPlayer::GetInstance()->NewGame();
			CGame::GetInstance()->ChangeState(CGamePlayState::GetInstance());
		}
		m_Timer.ResetTimer();
	}
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
			int size = m_sProfiles[i].size();
			ofs.write((char*)(&size), 4);
			const char* sz = m_sProfiles[i].c_str();
			ofs.write(sz, size);
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