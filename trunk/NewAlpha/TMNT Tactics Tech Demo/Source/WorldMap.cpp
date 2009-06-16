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
#define SCROLL_SPEED 200.0f
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

	m_bxTrainSkills = m_bxChooseTurtle = NULL;

	string* text = new string[3];
	text[0] = "Double Click"; text[1] = "On a location"; text[2] = "To explore it";
	m_bxHelp = new CBox(3, text, -5, 645, 0.11f, false, 15, 15, 30, -1, 0.5f);
	m_bxHelp->IsMsgBox(true);
	delete[] text;
	text = new string[4];
	text[0] = "SKILLS"; text[1] = "SAVE"; text[2] = "LOAD"; text[3] = "EXIT";
	m_bxMenu = new CBox(4, text, 830, 565, 0.11f, false, 35, 25, 15, m_pAssets->aBMactionBoxID, 0.5f);
	m_bxMenu->SetActive(); 
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
	m_nMapOSx = (m_nMapWidth >> 1) - (m_nScreenWidth >> 1) + 100;
	m_nMapOSy = (m_nMapHeight >> 1) - (m_nScreenHeight >> 1) + 100;
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
			m_pBitmapFont->DrawStringAutoCenter(m_Locations[i].name.c_str(), m_nScreenWidth, 720);
		}
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
}

void CWorldMap::Update(float fElapsedTime)
{
	if (m_ptMouse.x < 20)
		m_nMapOSx -= (int)( SCROLL_SPEED * fElapsedTime);
	if (m_ptMouse.x > m_nScreenWidth-20)
		m_nMapOSx += (int)( SCROLL_SPEED * fElapsedTime);
	if (m_ptMouse.y < 20)
		m_nMapOSy -= (int)( SCROLL_SPEED * fElapsedTime);
	if (m_ptMouse.y > m_nScreenHeight-20)
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

}

bool CWorldMap::Input(float fElapsedTime, POINT mouse)
{
	if (m_pDI->KeyPressed(DIK_ESCAPE))
		m_nCurrBtn = EXIT;

	if (m_pDI->MouseButtonPressed(MOUSE_LEFT))
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

	m_ptMouse = mouse;

	// get boxes input (calls update)
	m_bxHelp->Input(mouse);
	m_nCurrBtn = m_bxMenu->Input(mouse);

	if (m_bxChooseTurtle)
		m_nCurrBtn = m_bxChooseTurtle->Input(mouse);
	else if (m_bxTrainSkills)
		m_nCurrBtn = m_bxTrainSkills->Input(mouse);
	else if (m_bxLoad)
		m_nCurrBtn = m_bxLoad->Input(mouse);
	else if (m_bxSave)
		m_nCurrBtn = m_bxSave->Input(mouse);

	return true;
}


bool CWorldMap::HandleButtons()
{
	if (m_bxMenu->IsMouseInBox() || m_nCurrBtn == EXIT)
	{
		if(m_nCurrBtn == 3)
			m_nCurrBtn = EXIT;
		switch (m_nCurrBtn)
		{
		case MENU_BTN_SKILL: // pop up the choose turtle box, then populate skill box with appropriate skills
			{
				m_bxMenu->SetActive(false);
				string* turtles = new string[5]; turtles[0] = "CHOOSE A TURTLE";
				turtles[LEONARDO+1] = "LEONARDO"; turtles[DONATELLO+1] = "DONATELLO"; 
				turtles[RAPHAEL+1] = "RAPHAEL"; turtles[MIKEY+1] = "MIKEY";
				m_bxChooseTurtle = new CBox(5, turtles, 350, 400, 0.11f, true, 35, 35, 25, -1, 0.6f);
				m_bxChooseTurtle->SetType(BOX_WITH_BACK);
				m_bxChooseTurtle->SetActive();
				delete[] turtles;
			}
			break;
		case MENU_BTN_SAVE:
			m_bxMenu->SetActive(false);
			break;
		case MENU_BTN_LOAD:
			m_bxMenu->SetActive(false);
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
	}
	else if (m_bxTrainSkills)
	{
	}
	else if (m_bxLoad)
	{
	}
	else if (m_bxSave)
	{
	}

	return false;
}