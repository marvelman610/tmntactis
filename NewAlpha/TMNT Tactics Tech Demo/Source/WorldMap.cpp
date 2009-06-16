#include "WorldMap.h"
#include "Assets.h"
#include "BitmapFont.h"
#include "CSGD_TextureManager.h"
#include "CSGD_DirectInput.h"
#include "Game.h"
#include "MainMenuState.h"
#include "GamePlayState.h"
#include "BattleMap.h"
#include "Player.h"
#include "Box.h"
#include "Skill.h"
#define SCROLL_SPEED 200.0f
#define SCROLL_EDGE_DIST 35
#define EXIT	100

CWorldMap::CWorldMap()
{
	m_pTM			= CSGD_TextureManager::GetInstance();
	m_pBitmapFont	= CBitmapFont::GetInstance();
	m_pDI			= CSGD_DirectInput::GetInstance();
	m_pAssets		= CAssets::GetInstance();
	m_pPlayer		= CPlayer::GetInstance();

	m_nMapImageID = m_pAssets->aWMmapID;
	m_nScreenWidth = CGame::GetInstance()->GetScreenWidth();
	m_nScreenHeight = CGame::GetInstance()->GetScreenHeight();
	m_nMapWidth = 1875;
	m_nMapHeight = 1610;

	m_Locations[LOC_SIMUSA].name = "Simusa"; m_Locations[LOC_SIMUSA].imageID = m_pAssets->aWMtempleID; m_Locations[LOC_SIMUSA].mapXY = MAP_POINT(1516,804);
	m_Locations[LOC_SINARO].name = "SINARO"; m_Locations[LOC_SINARO].imageID = m_pAssets->aWMtempleID; m_Locations[LOC_SINARO].mapXY = MAP_POINT(1191,857);
	m_Locations[LOC_IWAMI].name  = "IWAMI";	 m_Locations[LOC_IWAMI].imageID  = m_pAssets->aWMtempleID; m_Locations[LOC_IWAMI].mapXY  = MAP_POINT(530,910);
	m_Locations[LOC_YAMATO].name = "YAMATO"; m_Locations[LOC_YAMATO].imageID = m_pAssets->aWMtempleID; m_Locations[LOC_YAMATO].mapXY = MAP_POINT(998,1064);
	
	m_pCurrLoc = &m_Locations[LOC_SIMUSA];

	string* text = new string[3];
	text[0] = "Double Click"; text[1] = "On a location"; text[2] = "To explore it";
	m_bxHelp = new CBox(3, text, -5, 645, 0.11f, false, 15, 15, 30, -1, 0.5f);
	m_bxHelp->IsMsgBox(true);
	m_bxHelp->SetAlpha(200);
	delete[] text;
	text = new string[4];
	text[0] = "SKILLS"; text[1] = "SAVE"; text[2] = "LOAD"; text[3] = "EXIT";
	m_bxMenu = new CBox(4, text, 830, 565, 0.11f, false, 35, 25, 15, m_pAssets->aBMactionBoxID, 0.5f);
	m_bxMenu->SetActive(); 
	delete[] text;
}

CWorldMap::~CWorldMap()
{
	if (m_bxHelp)
		delete m_bxHelp;
	if (m_bxMenu)
		delete m_bxMenu;
	m_pTM = NULL;
	m_pBitmapFont = NULL;
	m_pAssets = NULL;
	m_pDI = NULL;
	m_pPlayer = NULL;
}

CWorldMap* CWorldMap::GetInstance()
{
	static CWorldMap instance;
	return &instance;
}

void CWorldMap::Enter()
{
	m_bxTrainSkills = m_bxChooseTurtle = m_bxMsg = m_bxLoad = m_bxSave = NULL;
	m_nMapOSx = (m_nMapWidth >> 1) - (m_nScreenWidth >> 1) + 150;
	m_nMapOSy = (m_nMapHeight >> 1) - (m_nScreenHeight >> 1) + 100;
	m_fTimer = 0.0f;
}

void CWorldMap::Exit()
{
	if (m_bxChooseTurtle)
		delete m_bxChooseTurtle;
	if (m_bxTrainSkills)
		delete m_bxTrainSkills;
	if (m_bxLoad)
		delete m_bxLoad;
	if (m_bxSave)
		delete m_bxSave;
	if (m_bxMsg)
		delete m_bxMsg;
}

void CWorldMap::Render()
{
	m_pTM->DrawWithZSort(m_nMapImageID, -m_nMapOSx, -m_nMapOSy, 1.0f, 1.0f, 1.0f);
	m_pBitmapFont->DrawStringAutoCenter("World Map", m_nScreenWidth, 20);
	for (int i = 0; i < NUM_LOCATIONS; ++i)
	{
		m_pTM->DrawWithZSort(m_Locations[i].imageID, m_Locations[i].mapXY.x-m_nMapOSx, m_Locations[i].mapXY.y-m_nMapOSy, 
			0.9f, 1.0f, 1.0f, NULL, 0.0f, 0.0f, 0.0f, m_Locations[i].color);
		if (m_Locations[i].bSelected)	// selected, display on bottom of screen
		{
			string name = "CURRENT LOCATION - " + m_Locations[i].name;
			m_pBitmapFont->DrawStringAutoCenter(name.c_str(), m_nScreenWidth, 65, 0.09f, 0.5);
		}
		if (!m_bxSave && !m_bxLoad && !m_bxTrainSkills && !m_bxChooseTurtle && !m_bxMsg)
			m_pBitmapFont->DrawString(m_Locations[i].name.c_str(), m_Locations[i].mapXY.x - 45-m_nMapOSx, m_Locations[i].mapXY.y - 20-m_nMapOSy, 
				0.0f, 0.5f, m_Locations[i].color);
	}
	m_pTM->DrawWithZSort(m_pAssets->aMousePointerID, m_ptMouse.x-10, m_ptMouse.y-3, 0.0f);

	// draw boxes

	m_bxHelp->Render();
	m_bxMenu->Render();
	if (m_bxChooseTurtle)
	{
		m_bxChooseTurtle->Render();
	}
	else if (m_bxTrainSkills)
	{
		m_bxTrainSkills->Render();
	}
	else if (m_bxLoad)
	{
		m_bxLoad->Render();
	}
	else if (m_bxSave)
	{
		m_bxSave->Render();
	}
	else if (m_bxMsg)
		m_bxMsg->Render();
}

void CWorldMap::Update(float fElapsedTime)
{
	if (m_ptMouse.x < SCROLL_EDGE_DIST || m_pDI->KeyDown(DIK_A))
		m_nMapOSx -= (int)( SCROLL_SPEED * fElapsedTime);
	if (m_ptMouse.x > m_nScreenWidth-SCROLL_EDGE_DIST || m_pDI->KeyDown(DIK_D))
		m_nMapOSx += (int)( SCROLL_SPEED * fElapsedTime);
	if (m_ptMouse.y < SCROLL_EDGE_DIST || m_pDI->KeyDown(DIK_W))
		m_nMapOSy -= (int)( SCROLL_SPEED * fElapsedTime);
	if (m_ptMouse.y > m_nScreenHeight-SCROLL_EDGE_DIST || m_pDI->KeyDown(DIK_S))
		m_nMapOSy += (int)( SCROLL_SPEED * fElapsedTime);

	if (m_nMapOSy < 0)
		m_nMapOSy = 0;
	if (m_nMapOSy > m_nMapHeight-m_nScreenHeight)
		m_nMapOSy = m_nMapHeight-m_nScreenHeight;
	if (m_nMapOSx < 0)
		m_nMapOSx = 0;
	if (m_nMapOSx > m_nMapWidth-m_nScreenWidth)
		m_nMapOSx = m_nMapWidth-m_nScreenWidth;


	for (int i = 0; i < NUM_LOCATIONS; ++i)
	{
		if (m_Locations[i].MouseInRect(m_ptMouse, m_nMapOSx, m_nMapOSy))
		{
			m_Locations[i].color = D3DCOLOR_XRGB(255,0,0);
			m_Locations[i].bHovering = true;
		}
		else
		{
			if (!m_Locations[i].bSelected)
				m_Locations[i].color = D3DCOLOR_XRGB(255,255,255);
			m_Locations[i].bHovering = false;
		}
	}
	if (m_bTrained)
		m_fTimer += fElapsedTime;
	if (m_bTrained && m_fTimer >= 2.0f)
	{
		m_bTrained = false;
		delete m_bxMsg; m_bxMsg = NULL;
		string* turtles = new string[5]; turtles[0] = "CHOOSE A TURTLE";
		char cost[16];
		string temp;
		for (int i = 0; i < 4; ++i)
		{
			temp = m_pPlayer->GetTurtles()[i]->GetName();
			temp += " - SKILL PTS";
			sprintf_s(cost, " %i", m_pPlayer->GetTurtles()[i]->GetSkillXP());
			temp += cost;
			turtles[i+1] = temp;
		}
		m_bxChooseTurtle = new CBox(5, turtles, 100, 250, 0.11f, true, 35, 35,15, -1, 0.75f);
		m_bxChooseTurtle->SetType(BOX_WITH_BACK);
		m_bxChooseTurtle->SetActive();
		delete[] turtles;
		m_fTimer = 0.0f;
	}
}

bool CWorldMap::Input(float fElapsedTime, POINT mouse)
{
	if(m_pDI->JoystickIsUnplugged() == true)
	{
		m_ptMouse = mouse;
	}

	if (m_bTrained)	//don't accept input while trained msg box is displaying
		return true;
	if (m_pDI->KeyPressed(DIK_ESCAPE) || m_pDI->JoystickButtonPressed(1,0) )
	{m_nCurrBtn = EXIT;HandleButtons();}

	if (m_pDI->MouseButtonPressed(MOUSE_LEFT) || m_pDI->JoystickButtonPressed(0,0) )
	{
		if(HandleButtons()) // don't check any world map input if we're handling a button
			return true;
		for (int i = 0; i < NUM_LOCATIONS; ++i)
		{
			if (m_Locations[i].bHovering == true)
			{
				if (m_Locations[i].bSelected)	// going into the battle map
				{

					CGamePlayState::GetInstance()->ChangeMap(false, i); // i = currently selected map
				}
				m_Locations[i].bSelected = true;
				m_Locations[i].color = D3DCOLOR_XRGB(255,0,0);
			}
			else
			{
				m_Locations[i].bSelected = false;
				m_Locations[i].color = D3DCOLOR_XRGB(255,255,255);
			}
		}
	}

	if( m_pDI->JoystickGetLStickXAmount() || m_pDI->JoystickGetLStickYAmount() )	//m_pDI->JoystickGetLStickXNormalized())
	{
		m_ptMouse.x = m_ptMouse.x + (m_pDI->JoystickGetLStickXAmount()/1000);		//m_pDI->JoystickGetLStickXNormalized();
		m_ptMouse.y = m_ptMouse.y + (m_pDI->JoystickGetLStickYAmount()/1000);		//m_pDI->JoystickGetLStickYNormalized();
	}

	//if(m_pDI->JoystickDPadPressed(0,0))//left
	//{
	//	for( int i = 0; i < 4; i++)
	//	{
	//		if(m_Locations[i].bHovering == false)
	//		{
	//			m_ptMouse.x = m_Locations[LOC_SIMUSA].mapXY.x - m_nMapOSx;
	//			m_ptMouse.y = m_Locations[LOC_SIMUSA].mapXY.y - m_nMapOSy;
	//		}
	//		else
	//		{
	//			if(i > 0)
	//			{
	//				i--;
	//				m_ptMouse.x = m_Locations[i].mapXY.x- m_nMapOSx;
	//				m_ptMouse.y = m_Locations[i].mapXY.y- m_nMapOSy;
	//			}
	//			else
	//			{
	//				m_ptMouse.x = m_Locations[3].mapXY.x- m_nMapOSx;
	//				m_ptMouse.y = m_Locations[3].mapXY.y- m_nMapOSy;
	//			}
	//		}
	//	}
	//}
	//if(m_pDI->JoystickDPadPressed(1,0))//right
	//{
	//	for( int i = 0; i < 4; i++)
	//	{
	//		if(m_Locations[i].bHovering == false)
	//		{
	//			m_ptMouse.x = m_Locations[LOC_SIMUSA].mapXY.x- m_nMapOSx;
	//			m_ptMouse.y = m_Locations[LOC_SIMUSA].mapXY.y- m_nMapOSy;
	//		}
	//		else
	//		{
	//			if(i < 3)
	//			{
	//				i++;
	//				m_ptMouse.x = m_Locations[i].mapXY.x- m_nMapOSx;
	//				m_ptMouse.y = m_Locations[i].mapXY.y- m_nMapOSy;
	//			}
	//			else
	//			{
	//				m_ptMouse.x = m_Locations[0].mapXY.x- m_nMapOSx;
	//				m_ptMouse.y = m_Locations[0].mapXY.y- m_nMapOSy;
	//			}
	//		}
	//	}
	//}

	// get boxes input (calls update)

	m_bxHelp->Input(m_ptMouse);
	m_nCurrBtn = m_bxMenu->Input(m_ptMouse);
	if(m_nCurrBtn == 3)
		m_nCurrBtn = MENU_BTN_EXIT;

	if (m_bxChooseTurtle)
		m_nCurrBtn = m_bxChooseTurtle->Input(m_ptMouse);
	else if (m_bxTrainSkills)
		m_nCurrBtn = m_bxTrainSkills->Input(m_ptMouse);
	else if (m_bxLoad)
		m_nCurrBtn = m_bxLoad->Input(m_ptMouse);
	else if (m_bxSave)
		m_nCurrBtn = m_bxSave->Input(m_ptMouse);

	return true;
}


bool CWorldMap::HandleButtons()
{
	if (m_bxMenu->IsMouseInBox() || m_nCurrBtn == MENU_BTN_EXIT)
	{
		switch (m_nCurrBtn)
		{
		case MENU_BTN_SKILL: // pop up the choose turtle box, then populate skill box with appropriate skills
			{
				m_bxMenu->SetActive(false);
				string* turtles = new string[5]; turtles[0] = "CHOOSE A TURTLE";
				char cost[16];
				string temp;
				for (int i = 0; i < 4; ++i)
				{
					temp = m_pPlayer->GetTurtles()[i]->GetName();
					temp += " - SKILL PTS";
					sprintf_s(cost, " %i", m_pPlayer->GetTurtles()[i]->GetSkillXP());
					temp += cost;
					turtles[i+1] = temp;
				}
				m_bxChooseTurtle = new CBox(5, turtles, 100, 250, 0.11f, true, 35, 35,15, -1, 0.75f);
				m_bxChooseTurtle->SetType(BOX_WITH_BACK);
				m_bxChooseTurtle->SetActive();
				delete[] turtles;
			}
			break;
		case MENU_BTN_SAVE:
			{
				m_bxMenu->SetActive(false);
				string* sSaveGame = new string[2];
				sSaveGame[0] = "SAVE GAME"; sSaveGame[1] = "MY SAVED GAME...";
				m_bxSave = new CBox(2, sSaveGame, 230, 300, 0.11f, true);
				m_bxSave->SetType(BOX_WITH_BACK);
				m_bxSave->SetActive();
				delete[] sSaveGame;
			}
			break;
		case MENU_BTN_LOAD:
			{
				m_bxMenu->SetActive(false);
				// TODO:: get the current profile's saved game, set up the load game box, handle results elsewhere
				string* sLoadGame = new string[2];
				sLoadGame[0] = "LOAD GAME"; sLoadGame[1] = "Bob's saved game";
				m_bxLoad = new CBox(2, sLoadGame, 230, 300, 0.11f, true);
				m_bxLoad->SetType(BOX_WITH_BACK);
				m_bxLoad->SetActive();
				delete[] sLoadGame;
			}
			break;
		case MENU_BTN_EXIT:
			{
				if (m_bxChooseTurtle)
				{
					m_bxMenu->SetActive();
					delete m_bxChooseTurtle; m_bxChooseTurtle = NULL;
				}
				else if (m_bxTrainSkills)
				{
					m_bxMenu->SetActive();
					delete m_bxTrainSkills; m_bxTrainSkills = NULL;
				}
				else if (m_bxLoad)
				{
					m_bxMenu->SetActive();
					delete m_bxLoad; m_bxLoad = NULL;
				}
				else if (m_bxSave)
				{
					m_bxMenu->SetActive();
					delete m_bxSave; m_bxSave = NULL;
				}
				else
					CGame::GetInstance()->ChangeState(CMainMenuState::GetInstance());
				break;
			}

		}
	}
	else if (m_bxChooseTurtle)
	{
		delete m_bxChooseTurtle; m_bxChooseTurtle = NULL;
		int numTrainedSkills = m_pPlayer->GetTurtles()[m_nCurrBtn-1]->GetSkills()->size();
		int numUntrainedSkills = m_pPlayer->GetTurtles()[m_nCurrBtn-1]->GetInactiveSkills()->size();
		
		// the index of the first untrained skill available
		// NOTE: this index does not offset for the title index being [0]
		m_nFirstTrainable = numTrainedSkills+1;	
		string* skills = new string[numTrainedSkills+numUntrainedSkills+1]; 
		skills[0] = "Train Skills";

		char avail[16];
		skills[0] += " - SP";
		sprintf_s(avail, " %i", m_pPlayer->GetTurtles()[m_nCurrBtn-1]->GetSkillXP());
		skills[0] += avail;

		string temp;
		char cost[16];
		// trained skills
		vector<CSkill> vskills = *(m_pPlayer->GetTurtles()[m_nCurrBtn-1]->GetSkills());
		for (int i = 0; i < numTrainedSkills; ++i)
		{
			skills[i+1] = vskills[i].GetSkillName();
			skills[i+1] += " - Trained";  
		}
		// untrained skills
		vskills = *(m_pPlayer->GetTurtles()[m_nCurrBtn-1]->GetInactiveSkills());
		for (int i = 0; i < numUntrainedSkills; ++i)
		{
			skills[numTrainedSkills+i+1] = vskills[i].GetSkillName();
			sprintf_s(cost, " - Cost %i", vskills[i].GetSkillCost());
			temp = cost;
			skills[numTrainedSkills+i+1] += cost; 
		}
		m_bxTrainSkills = new CBox(numUntrainedSkills+numTrainedSkills+1, skills, 150, 270, 0.11f, true, 25, 35, 15, -1, 0.7f);
		m_bxTrainSkills->SetType(BOX_WITH_BACK); m_bxTrainSkills->SetActive();
		delete[] skills;
		m_nTurtleSkillTrainIndex = m_nCurrBtn-1;
	}
	else if (m_bxTrainSkills)
	{
		// only valid if it's an untrained skill
		if (m_nFirstTrainable <= m_nCurrBtn)
		{
			CSkill chosenSkill = (*m_pPlayer->GetTurtles()[m_nTurtleSkillTrainIndex]->GetInactiveSkills())[m_nCurrBtn-m_nFirstTrainable];
			if (chosenSkill.GetSkillCost() <= m_pPlayer->GetTurtles()[m_nTurtleSkillTrainIndex]->GetSkillXP())
			{
				// trained skills
				vector<CSkill>* vskills = m_pPlayer->GetTurtles()[m_nTurtleSkillTrainIndex]->GetSkills();
				vskills->push_back(chosenSkill);
	
				// untrained skills
				vector<CSkill>* vskillsPtr = m_pPlayer->GetTurtles()[m_nTurtleSkillTrainIndex]->GetInactiveSkills();
				vector<CSkill>::iterator iter = vskillsPtr->begin();
				for (unsigned int i = 0; i < vskillsPtr->size(); ++i)
				{
					if (i == m_nCurrBtn-m_nFirstTrainable)
					{
						vskillsPtr->erase(iter);
						break;
					}
					++iter;
				}
				delete m_bxTrainSkills; m_bxTrainSkills = NULL;
				m_bTrained = true;
				string* msg = new string[1]; msg[0] = "SKILL TRAINED!";
				m_bxMsg = new CBox(1, msg, 250, 300, 0.11f, false, 25, 35, 25, -1, 0.7f);
				m_bxMsg->IsMsgBox(true);
			}
			else
			{
				delete m_bxTrainSkills; m_bxTrainSkills = NULL;
				m_bTrained = true;
				string* msg = new string[1]; msg[0] = "NOT ENOUGH SKILL PTS!";
				m_bxMsg = new CBox(1, msg, 250, 300, 0.11f, false, 25, 35, 25, -1, 0.7f);
				m_bxMsg->IsMsgBox(true);
			}
		}
	}
	else if (m_bxLoad)
	{
	}
	else if (m_bxSave)
	{
	}

	return false;
}