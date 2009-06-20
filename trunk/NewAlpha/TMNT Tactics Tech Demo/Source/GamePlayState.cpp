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
#include "Factory.h"
#include "WorldMap.h"
#include "Skill.h"
#include "Assets.h"
#include <fstream>

// Constructor
CGamePlayState::CGamePlayState(void)
{
	m_pCurrentMenuState = NULL;
	m_pBattleMap = NULL;
	m_pPlayer = NULL;
	m_pHUD = NULL;
	m_pWorldMap = NULL;
	m_nCurrentMap = 0;
	m_bIsPaused = false;
}

// Destructor
CGamePlayState::~CGamePlayState(void)
{
	m_pPlayer = NULL;
}

// Enter
void CGamePlayState::Enter(void)
{
	m_pHUD = CHUD::GetInstance();
	m_pPlayer = CPlayer::GetInstance();
	m_pWorldMap = CWorldMap::GetInstance();
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
	if(m_pWorldMap && m_nCurrentMap == MAP_WORLD)
	{
		m_pWorldMap->Exit();
		m_pWorldMap = NULL;
	}
 	if(m_pHUD)
 	{
 		m_pHUD = NULL;
	}
// 	if (m_pPlayer)
// 		m_pPlayer = NULL;
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
		player->NewGame();
		CTurtle** turtles = player->GetTurtles();
		int currState;

		for (int i = 0; i < 4; ++i)
		{
			int binIn;
			ifs.read(reinterpret_cast<char*>(&binIn), sizeof(int));
			turtles[i]->SetCurrWeaponIndex(binIn);
			ifs.read(reinterpret_cast<char*>(&binIn), sizeof(int));
			turtles[i]->SetAccuracy(binIn);
			ifs.read(reinterpret_cast<char*>(&binIn), sizeof(int));
			turtles[i]->SetBaseAP(binIn);
			ifs.read(reinterpret_cast<char*>(&binIn), sizeof(int));
			turtles[i]->SetDefense(binIn);
			ifs.read(reinterpret_cast<char*>(&binIn), sizeof(int));
			turtles[i]->SetExperience(binIn);
			ifs.read(reinterpret_cast<char*>(&binIn), sizeof(int));
			turtles[i]->SetLevel(binIn);
			ifs.read(reinterpret_cast<char*>(&binIn), sizeof(int));
			turtles[i]->SetSkillXP(binIn);
			ifs.read(reinterpret_cast<char*>(&binIn), sizeof(int));
			turtles[i]->SetStrength(binIn);
			ifs.read(reinterpret_cast<char*>(&binIn), sizeof(int));
			turtles[i]->SetType(binIn);

			// get the number of skills - active and ALLSKILLS - 
			// for the number of each, load in the ids
			// based on which category the ids are in, move the skill
			// in the turtles' vectors to the appropriate place

			turtles[i]->GetSkills()->clear();
			turtles[i]->GetInactiveSkills()->clear();
			// vectors	- ALLSKILLS
			vector<CSkill> ALLSKILLS = CGame::GetInstance()->GetSkills();
			// inactive...to add to turtle skill vectors
			vector<CSkill> inactive;
			vector<CSkill> active;

			ifs.read(reinterpret_cast<char*>(&binIn), sizeof(int));	// ALLSKILLS vector size

			for (int i2 = 0; i2 < binIn; ++i2)
			{
				int skillID;
				ifs.read(reinterpret_cast<char*>(&skillID), sizeof(int));	// read in the skill ID to add to the inactive vector
				for (unsigned int sInd = 0; sInd < ALLSKILLS.size(); ++sInd)
				{
					if (ALLSKILLS[sInd].GetID() == skillID)	// check if this is the skill to add
					{
						inactive.push_back(ALLSKILLS[sInd]);
						break;
					}
				}
			}
			turtles[i]->SetSkillsInactive(inactive);

			//			- active
			ifs.read(reinterpret_cast<char*>(&binIn), sizeof(int));	// active vector size
			for (int i2 = 0; i2 < binIn; ++i2)
			{
				int skillID;
				ifs.read(reinterpret_cast<char*>(&skillID), sizeof(int));	// read in the skill ID to add to the inactive vector
				for (unsigned int sInd = 0; sInd < ALLSKILLS.size(); ++sInd)
				{
					if (ALLSKILLS[sInd].GetID() == skillID)	// check if this is the skill to add
					{
						active.push_back(ALLSKILLS[sInd]);
						break;
					}
				}
			}
			turtles[i]->SetSkillsActive(active);

			active.clear();
			inactive.clear();

			//			- weapons
			turtles[i]->ClearWeapons();
			ifs.read(reinterpret_cast<char*>(&binIn), sizeof(int));	// num weapons
			POINT pt; pt.x = 0; pt.y = 0;
			int wID;
			for (int i2 =0; i2 < binIn; ++i2)
			{
				ifs.read(reinterpret_cast<char*>(&wID), sizeof(int));
				CBase* weapon = Factory::GetInstance()->CreateWeapon(wID, pt, true);
				turtles[i]->AddWeapon(*weapon);
			}
		}
		for (int i2 = 0; i2 < 10; ++i2)
		{
			bool unlocked;
			ifs.read(reinterpret_cast<char*>(&unlocked), sizeof(bool));
			if (unlocked)
				player->GetAch()->LoadUnlock(i2);
		}

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
			// turtle
			int wi = turtles[i]->GetCurrWeaponIndex();
			ofs.write((char*)&wi, sizeof(int));
			int acc = turtles[i]->GetAccuracy();
			ofs.write((char*)&acc, sizeof(int));
			int bAP = turtles[i]->GetBaseAP();
			ofs.write((char*)&bAP, sizeof(int));
			int def = turtles[i]->GetDefense();
			ofs.write((char*)&def, sizeof(int));
			int exp = turtles[i]->GetExperience();
			ofs.write((char*)&exp, sizeof(int));
			int lvl = turtles[i]->GetLevel();
			ofs.write((char*)&lvl, sizeof(int));
			int sXP = turtles[i]->GetSkillXP();
			ofs.write((char*)&sXP, sizeof(int));
			int str = turtles[i]->GetStrength();
			ofs.write((char*)&str, sizeof(int));
			int type = turtles[i]->GetType();
			ofs.write((char*)&type, sizeof(int));

			// ALLSKILLS
			int iSize = turtles[i]->GetInactiveSkills()->size();
			ofs.write((char*)&iSize, sizeof(int));	// write out the number of skills (inactive)
			vector<CSkill>* iSkills = turtles[i]->GetInactiveSkills();
			for (int i2 = 0; i2 < iSize; ++i2)
			{
				// now write out its ID
				int skill = (*iSkills)[i2].GetID();
				ofs.write((char*)&skill, sizeof(int));
			}

			// active
			int aSize = turtles[i]->GetSkills()->size();
			ofs.write((char*)&aSize, sizeof(int));	// number of active skills
			vector<CSkill>* aSkills = turtles[i]->GetSkills();
			for (int i2 = 0; i2 < aSize; ++i2)
			{
				// now write out its ID
				int skill = (*aSkills)[i2].GetID();
				ofs.write((char*)&skill, sizeof(int));
			}

			// weapons
			int wSize = turtles[i]->GetWeapons()->size();
			ofs.write((char*)&wSize, sizeof(int));
			vector<CBase>* vWeapons = turtles[i]->GetWeapons();
			for (int i2 = 0; i2 < wSize; ++i2)
			{
				int weapon = (*vWeapons)[i2].GetNumType();
				ofs.write((char*)&weapon, sizeof(int));
			}
		}
		CAchievements* ach = player->GetAch();
		for (int i2 = 0; i2<10; ++i2)
		{
			bool unlocked = ach->GetLocked(i2);
			ofs.write((char*)&unlocked, sizeof(bool));
		}
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