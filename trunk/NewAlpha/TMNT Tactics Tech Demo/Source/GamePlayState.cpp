//////////////////////////////////////////////////////////////////////////
//	Filename	:	GamePlayState.cpp
//
//	Author		:	Ramon Johannessen (RJ)
//
//	Purpose		:	To define the CGamePlayState class. This state handles
//					all gameplay code.
//////////////////////////////////////////////////////////////////////////
#include "GamePlayState.h"
#include "CSGD_DirectInput.h"
#include "CSGD_Direct3D.h"
#include "CSGD_TextureManager.h"
#include "BaseMenuState.h"
#include "MainMenuState.h"
#include "Game.h"
#include "Player.h"
#include "Turtle.h"
#include "HUD.h"
#include "WorldMap.h"
#include "ObjectManager.h"
#include "BitmapFont.h"
#include "Achievements.h"
#include <fstream>
#include "WorldMap.h"

// Constructor
CGamePlayState::CGamePlayState(void)
{
	m_pCurrentMenuState = NULL;
	m_pBattleMap = NULL;
	m_pPlayer = NULL;
	m_pHUD = NULL;
	m_pWorldMap  = NULL;

	m_nCurrentMap = 0;
}

// Destructor
CGamePlayState::~CGamePlayState(void)
{
}

// Enter
void CGamePlayState::Enter(void)
{
	//m_pCurrentMenuState = BaseMenuState::GetInstance();
	m_pHUD = CHUD::GetInstance();
	m_pWorldMap = CWorldMap::GetInstance();
	m_pPlayer = CPlayer::GetInstance();
	m_pBattleMap = CBattleMap::GetInstance();

	m_pWorldMap->Enter();
	m_nCurrentMap = MAP_WORLD;
}

// Exit
void CGamePlayState::Exit(void)
{
	if(m_pBattleMap && m_nCurrentMap == MAP_BATTLE)
	{
		m_pBattleMap->Exit();
	}
	if(m_pWorldMap)
	{
		m_pWorldMap->Exit();
		m_pWorldMap = NULL;
	}
 	if(m_pHUD)
 	{
 		m_pHUD = NULL;
	}
	if (m_pPlayer)
		m_pPlayer = NULL;
	if(m_pCurrentMenuState)
	{
		m_pCurrentMenuState->Exit();
		m_pCurrentMenuState = NULL;
	}
}

// Instance
CGamePlayState* CGamePlayState::GetInstance(void)
{
	static CGamePlayState instance;
	return &instance;
}

// Input
bool CGamePlayState::Input(float fElapsedTime, POINT mousePt)
{
	CSGD_DirectInput* pDI = CSGD_DirectInput::GetInstance();

	if(pDI->KeyPressed(DIK_P) || pDI->JoystickButtonPressed(7,0))
	{
		m_bIsPaused = !m_bIsPaused;
	}

	switch (m_nCurrentMap)
	{
	case WORLD_MAP:
		if (!m_pWorldMap->Input(fElapsedTime, mousePt))
			CGame::GetInstance()->ChangeState(CMainMenuState::GetInstance());
		break;
	case BATTLE_MAP:
		if (!m_pBattleMap->Input(fElapsedTime, mousePt))
			CGame::GetInstance()->ChangeState(CMainMenuState::GetInstance());
		break;
	}
	
	return true;
}
// Update
void CGamePlayState::Update(float fElapsedTime)
{
	if(!m_bIsPaused)
	{
		switch (m_nCurrentMap)
		{
		case WORLD_MAP:
			m_pWorldMap->Update(fElapsedTime);
			break;
		case BATTLE_MAP:
			ObjectManager::GetInstance()->UpdateObjects(fElapsedTime);
			ObjectManager::GetInstance()->CheckCollisions();
			m_pBattleMap->Update(fElapsedTime);
			break;
		}
	}
}
// Render
void CGamePlayState::Render(void)
{
	switch (m_nCurrentMap)
	{
	case WORLD_MAP:
		m_pWorldMap->Render();
		break;
	case BATTLE_MAP:
		ObjectManager::GetInstance()->RenderObjects();
		m_pBattleMap->Render();
		break;
	}
}

void CGamePlayState::LoadGame(const char* fileName)
{
	ifstream ifs;
	string fName = fileName;
	string pathFileName = "Resources/SavedGames/" + fName;
	ifs.open(pathFileName.c_str(), ios_base::binary | ios_base::in);
	if (ifs.is_open())
	{
		CPlayer* player = CPlayer::GetInstance();
		CTurtle** turtles = player->GetTurtles();
		CAchievements* ach = player->GetAch();
		int currState;
		for (int i = 0; i < 4; ++i)
		{
			ifs.read(reinterpret_cast<char*>(&turtles[i]), sizeof(CTurtle));
		}
		ifs.read(reinterpret_cast<char*>(ach), sizeof(CAchievements));
		ifs.read(reinterpret_cast<char*>(&currState), sizeof(int));
		player->SetStage(currState);
		ifs.close();
	}
}

void CGamePlayState::SaveGame(const char* fileName)
{
	ofstream ofs;
	string fName = fileName;
	string pathFileName = "Resources/SavedGames/" + fName;
	ofs.open(pathFileName.c_str(), ios_base::binary | ios_base::out);

	CPlayer* player = CPlayer::GetInstance();
	CTurtle** turtles = player->GetTurtles();
	int currStage = player->GetCurrStage();

	if (ofs.is_open())
	{
		for (int i = 0; i < 4; ++i)
		{
			ofs.write((char*)(&turtles[i]), sizeof(CTurtle));
		}
		ofs.write((char*)(player->GetAch()), sizeof(CAchievements));
		ofs.write((char*)(&currStage), sizeof(int));
		ofs.close();
	}
}


void CGamePlayState::ChangeMap(bool bWorldMap, int mapID) // if no parameter sent, goes to WORLD_MAP by default
{
	if(bWorldMap)
		m_nCurrentMap = MAP_WORLD;
	else
		m_nCurrentMap = MAP_BATTLE;

	switch (m_nCurrentMap) // battle map OR world map?
	{
	case WORLD_MAP:
		m_pBattleMap->Exit();
		m_pWorldMap->Enter();
		break;
	case BATTLE_MAP:
		m_pWorldMap->Exit();
		switch (mapID)		// which battle map file to load?
		{
		case LOC_SIMUSA:
			m_pBattleMap->SetBGimageID(CAssets::GetInstance()->aBMLeoBGID);
			m_pBattleMap->Enter("Resources/MapInfo/VG_ZSortTest.dat", "Test", 4);
			break;
		case LOC_IWAMI:
			m_pBattleMap->SetBGimageID(CAssets::GetInstance()->aBMRaphBGID);
			m_pBattleMap->Enter("Resources/MapInfo/VG_lvl1.dat", "Test", 2, true);
			break;
		case LOC_SINARO:
			m_pBattleMap->SetBGimageID(CAssets::GetInstance()->aBMDonBGID);
			m_pBattleMap->Enter("Resources/MapInfo/VG_ZSortTest.dat", "Test", 6);
			break;
		case LOC_YAMATO:
			m_pBattleMap->SetBGimageID(CAssets::GetInstance()->aBMMikeBGID);
			m_pBattleMap->Enter("Resources/MapInfo/VG_lvl1.dat", "Test", 5);
			break;
		}
		break;
	}
}