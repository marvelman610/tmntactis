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
	m_bxHelp = new CBox(3, text, 5, 5, 0.11f, false, 15, 15, 30, -1, 0.5f);
	m_bxHelp->IsMsgBox(true);
	delete[] text;
}

CWorldMap::~CWorldMap()
{
	m_pTM = NULL;
	m_pBitmapFont = NULL;
	m_pAssets = NULL;
	m_pDI = NULL;
	m_pPlayer = NULL;
	delete m_bxHelp;
}

CWorldMap* CWorldMap::GetInstance()
{
	static CWorldMap instance;
	return &instance;
}

void CWorldMap::Enter()
{
	m_nMapOSx = (m_nMapWidth >> 1) - (m_nScreenWidth >> 1) + 100;
	m_nMapOSy = (m_nMapHeight >> 1) - (m_nScreenHeight >> 1);
}

void CWorldMap::Exit()
{

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
		CGame::GetInstance()->ChangeState(CMainMenuState::GetInstance());

	if (m_pDI->MouseButtonPressed(MOUSE_LEFT))
	{
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

	// get boxes input
	m_bxHelp->Input(mouse);

	return true;
}