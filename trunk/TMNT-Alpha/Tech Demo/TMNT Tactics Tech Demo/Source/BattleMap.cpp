//////////////////////////////////////////////////////////////////////////
//	Filename	:	CBattleMap.cpp
//
//	Author		:	Ramon Johannessen (RJ)
//
//	Purpose		:	To execute the individual battles that take place,
//					drawing the isometric tile map, updating, getting input, etc...
//////////////////////////////////////////////////////////////////////////
#include "BattleMap.h"
#include "Assets.h"
#include "Game.h"
#include "Tile.h"
#include "CSGD_Direct3D.h"
#include "CSGD_DirectInput.h"
#include "CSGD_TextureManager.h"
#include "ObjectManager.h"
#include "Factory.h"
#include "Animation.h"
//#include "fmod.hpp"
#include "MainMenuState.h"
#include "BitmapFont.h"
#include "ParticleSystem.h"
#include "HUD.h"
#include "Player.h"
#include "Turtle.h"
#include "Ninja.h"
#include "Box.h"
#include <fstream>
#include <exception>

#define BOUNDING_BOXES 0
#define CAM_EDGE_DIST_TO_MOVE 40	// how close to edge of the screen the mouse has to get to start scrolling
#define SCROLLSPEED 150.0f

CBattleMap::CBattleMap(void)
{
	srand((unsigned int)(time(0)));
	m_pPlayer		= CPlayer::GetInstance();
	m_pAssets		= CAssets::GetInstance();
	m_pTM			= CSGD_TextureManager::GetInstance();
	m_pGame			= CGame::GetInstance();
	m_pDI			= CSGD_DirectInput::GetInstance();
	m_pD3D			= CSGD_Direct3D::GetInstance();
	m_pBitmapFont   = CBitmapFont::GetInstance();
	//  Sm_pFMOD		= m_pGame->GetFMODSystem();
	m_pHUD			= CHUD::GetInstance();
}

CBattleMap::~CBattleMap(void)
{
}

CBattleMap* CBattleMap::GetInstance()
{
	static CBattleMap instance;
	return &instance;
}
void CBattleMap::Enter(char* szFileName, char* szMapName, int nNumEnemies)
{
	SetMousePtr(m_pAssets->aMousePointerID);
	m_bIsMouseAttack = m_bOutOfRange = false;

	m_fTimer = 0.0f;

	string text[3];
	text[0] = "SPECIAL"; text[1] = "ITEM"; text[2] = "END TURN";
	m_bxActionBox = new CBox(3, text, 400, 500, 0.1f, 128, -1, 55);
	m_bxActionBox->SetActive();

	m_pTilesL1 = NULL;
	m_pTilesL2 = NULL;
	m_pFreeTiles = NULL;
	//m_pMoveableTiles = NULL;
	m_nHoverCharacter = -1; m_nCurrCharacter = -1; m_nMoveDirection = -1; m_nCurrMouseTileTarget = -1; m_ncurrTargetTile = -1; m_nCurrTarget = -1;
	m_nCurrSelectedTile = -1;
	m_nNumCols = 0; m_nNumRows = 0;
	m_nTotalNumTiles = 0;
	m_nTileWidth = 0; m_nTileHeight = 0;
	m_nMapHeight = 0; m_nMapWidth = 0;
	m_nFreeTileCount = 0;
	m_fScrollX = 0.0f; m_fScrollY = 0.0f;
	m_nScrenWidth = m_pGame->GetScreenWidth();
	m_nScreenHeight = m_pGame->GetScreenHeight();
	m_nOffsetX = m_nOffsetY = 0;

	m_strCurrVersion = "TED-Version-1.0";	// current tile editor's version number

	m_pParticleSys = new CParticleSystem();
	m_pParticleSys->Load("Resources/ParticleInfo/VG_Test.dat");

	m_nNumCharacters = 4+nNumEnemies;
	m_nNumEnemiesLeft = nNumEnemies;
	m_szFileName = szFileName;
	m_szMapName  = szMapName;
	m_bIsPlayersTurn = (bool)(rand() % 2);

	LoadMapInfo();
	// will be used to set ALL the characters' start positions according to
	// the battle map's spawn points
	CreateEnemies();
	SetStartPositions();
}
void CBattleMap::Exit()
{
	delete[] m_pTilesL1;
	delete[] m_pTilesL2;
	delete[] m_pFreeTiles;
	delete m_pParticleSys;
	m_vCharacters.clear();

	if (m_pAssets)
		m_pAssets = NULL;
	if (m_pTM)
		m_pTM = NULL;
	if (m_pGame)
		m_pGame = NULL;
	if (m_pDI)
		m_pDI = NULL;
	if (m_pHUD)
		m_pHUD = NULL;
	if (m_pPlayer)
		m_pPlayer = NULL;
	// 	if (m_pBitmapFont)
	// 		m_pBitmapFont = NULL;
	// 	if (m_pFMOD)
	// 		m_pFMOD = NULL;

}
//////////////////////////////////////////////////////////////////////////
// TODO:: determine if we have to draw all opaque objects first, then any
//			transparent ones last?
//////////////////////////////////////////////////////////////////////////
void CBattleMap::Render()
{
	//m_pTM->DrawWithZSort(m_pAssets->aBMbgID, 0, 0, 1.0f, 1.0f, 1.0f, NULL, 0.0f, 0.0f, 0.0f, D3DCOLOR_XRGB(100, 100, 100));

	//////////////////////////////////////////////////////////////////////////
	//TODO::this will be put in the hud eventually
	if (m_bIsPlayersTurn)
		m_pD3D->DrawText("PLAYER'S TURN", 10, 600, 0,0,0);
	else
		m_pD3D->DrawText("ENEMY'S TURN", 10, 620, 0,0,0);
	if (m_bNotEnoughAP && m_fTimer < 3)
	{
		m_pD3D->DrawText("Not enough AP", 10, 700, 255, 0, 0);
	}
	else if (m_bOutOfRange && m_fTimer < 3)
	{
		m_pD3D->DrawText("Out of range", 10, 700, 255, 0, 0);
	}
	if (m_fTimer > 3)
	{
		m_bNotEnoughAP = m_bOutOfRange = false;
		m_fTimer = 0.0f;
	}

	if (m_nCurrCharacter > -1)
	{
		char apText[64];
		sprintf_s(apText, "AP: %i", m_vCharacters[m_nCurrCharacter].GetCurrAP());
		m_pD3D->DrawText(apText, 10, 640, 0,0,0);
		if (m_nCurrTarget > -1)
		{
			char distText[64];
			sprintf_s(distText, "Dist to Target: %i", m_nDistanceToTarget);
			m_pD3D->DrawText(distText, 10, 660, 0,0,0);
		}
	}
	if (m_nCurrTarget > -1)
	{
		char ninjaHealth[64];
		sprintf_s(ninjaHealth, "Health: %i", m_vCharacters[m_nCurrTarget].GetHealth());
		m_pD3D->DrawText(ninjaHealth, 10, 500, 0, 0, 0);
	}
	//////////////////////////////////////////////////////////////////////////

	if (m_nHoverCharacter != -1 && m_bIsPlayersTurn)
		DrawHover();
	else if (m_nHoverCharacter == -1 && m_nCurrCharacter == -1)
		SetMousePtr(m_pAssets->aMousePointerID);
	else if (m_nHoverCharacter == -1 && m_nCurrCharacter > -1)
		SetMousePtr(m_pAssets->aMouseMoveID);
	// draw the current mouse pointer
	m_pTM->DrawWithZSort(GetMousePtr(), m_ptMouseScreenCoord.x-10, m_ptMouseScreenCoord.y-3, 0.0f);

	// draw layer one & two
	MY_POINT mapPT;
	for (int x = 0; x < m_nNumCols; ++x)
	{
		for (int y = 0; y < m_nNumRows; ++y)
		{
			int tileID = y*m_nNumCols+x;
			if (m_pTilesL1[tileID].DestXID() == -1)
				continue;
			mapPT.x = x; mapPT.y = y;
			mapPT = IsoTilePlot(mapPT, GetOffsetX(), GetOffsetY());

			// Layer one
// 			if (m_pTilesL1[tileID].Flag() == FLAG_OBJECT_EDGE)
// 				m_pTilesL1[tileID].SetAlpha(150);
// 			else
// 				m_pTilesL1[tileID].SetAlpha(255);
			if (m_pTilesL1[tileID].Alpha() != 255 && m_bIsPlayersTurn)
			{
				m_pTM->DrawWithZSort(m_pTilesL1[tileID].ImageID(), mapPT.x, mapPT.y, depth.GROUND, 1.0f, 1.0f, 
					m_pTilesL1[tileID].SourceRect(), 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(m_pTilesL1[tileID].Alpha(),0,0,255)/*D3DCOLOR_XRGB(60,60,255)*/);
			} 
			else
			{
				m_pTM->DrawWithZSort(m_pTilesL1[tileID].ImageID(), mapPT.x, mapPT.y, depth.GROUND, 1.0f, 1.0f, 
					m_pTilesL1[tileID].SourceRect(), 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(255,255,255,255));
			}
			// Layer two
			if (m_pTilesL2[tileID].DestXID() != -1)
			{
				m_pTM->DrawWithZSort(m_pTilesL2[tileID].ImageID(), mapPT.x, mapPT.y, depth.GROUNDL2, 1.0f, 1.0f, 
					m_pTilesL2[tileID].SourceRect(), 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(m_pTilesL2[tileID].Alpha(),255,255,255));
			}
			
			// draws selection rectangle on the currently selected tile, character, and enemy (and draws arrow)
			if (m_bIsPlayersTurn)
			{
				if (m_nCurrSelectedTile == tileID)
				{
					m_pTM->DrawWithZSort(m_pAssets->aBMcursorID, mapPT.x, mapPT.y, depth.SELECTION, 1.0f, 1.0f);
				}
				else if (m_nHoverCharacter > -1 && m_nHoverCharacter < 4 && m_vCharacters[m_nHoverCharacter].GetCurrTile() == tileID)
				{
					m_pTM->DrawWithZSort(m_pAssets->aBMgreenSquareID, mapPT.x, mapPT.y, depth.SELECTION, 1.0f, 1.0f);
				}
				else if (m_nHoverCharacter > 3 && m_vCharacters[m_nHoverCharacter].GetCurrTile() == tileID)
				{
					m_pTM->DrawWithZSort(m_pAssets->aBMcursorID, mapPT.x, mapPT.y, depth.SELECTION, 1.0f, 1.0f);
				}
				if (m_nCurrCharacterTile == tileID)
				{
					m_pTM->DrawWithZSort(m_pAssets->aBMgreenSquareID, mapPT.x, mapPT.y, depth.SELECTION, 1.0f, 1.0f);
					m_pTM->DrawWithZSort(m_pAssets->aBMcurrSelectedArrowID, (int)m_vCharacters[m_nCurrCharacter].GetPosX()+8, 
						(int)m_vCharacters[m_nCurrCharacter].GetPosY()-32, depth.ARROW);
				}
				else if (m_ncurrTargetTile == tileID)
				{
					m_pTM->DrawWithZSort(m_pAssets->aBMcursorID, mapPT.x, mapPT.y, depth.SELECTION, 1.0f, 1.0f);
					m_pTM->DrawWithZSort(m_pAssets->aBMcurrTargetArrowID, (int)m_vCharacters[m_nCurrTarget].GetPosX()+8,
						(int)m_vCharacters[m_nCurrTarget].GetPosY()-32, depth.ARROW);
				}
			}
		}
	}
	// draw the free placed layer
	for (int i = 0; i < m_nFreeTileCount; ++i)
	{
		// if the character is inside a map object, alpha that object out
		// also, see if the mouse is inside a free-placed object
		RECT srcRect = *m_pFreeTiles[i].SourceRect();
		srcRect.left = m_pFreeTiles[i].DestX() + GetFreeTileXos();
		srcRect.top  = m_pFreeTiles[i].DestY() + GetFreeTileYos(); 
		srcRect.right= srcRect.left + m_pFreeTiles[i].Width();
		srcRect.bottom=srcRect.top + m_pFreeTiles[i].Height();
		if (ObjectManager::GetInstance()->CheckObjectsToAlpha(&srcRect) || (
			m_ptMouseScreenCoord.x > srcRect.left && m_ptMouseScreenCoord.x < srcRect.right &&
			m_ptMouseScreenCoord.y > srcRect.top  && m_ptMouseScreenCoord.y < srcRect.bottom)
			)
			m_pFreeTiles[i].SetAlpha(130);
		else
			m_pFreeTiles[i].SetAlpha(255);

		m_pTM->DrawWithZSort(m_pFreeTiles[i].ImageID(), srcRect.left, srcRect.top,
			depth.OBJECTS, 1.0f, 1.0f, m_pFreeTiles[i].SourceRect(), 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(m_pFreeTiles[i].Alpha(),255,255,255));
#if BOUNDING_BOXES
		// drawing bounding boxes
		CSGD_Direct3D::GetInstance()->DrawLine(srcRect.left, srcRect.top, srcRect.right, srcRect.top, 255, 0, 0);
		CSGD_Direct3D::GetInstance()->DrawLine(srcRect.left, srcRect.bottom, srcRect.right, srcRect.bottom, 255, 0, 0);
		CSGD_Direct3D::GetInstance()->DrawLine(srcRect.left, srcRect.top, srcRect.left, srcRect.bottom, 255, 0, 0);
		CSGD_Direct3D::GetInstance()->DrawLine(srcRect.right, srcRect.top, srcRect.right, srcRect.bottom, 255, 0, 0);
		if (m_nCurrCharacter!=-1)
		{
			char szCurrTile[64]; sprintf_s(szCurrTile, "CurrTile:%i", m_vCharacters[m_nCurrCharacter].GetCurrTile() );
			CSGD_Direct3D::GetInstance()->DrawText(szCurrTile, 5, 200);
		}
#endif
	}
	if (m_bIsPlayersTurn)
		DrawBoxes();
	m_pHUD->Render();
	//m_pParticleSys->DrawParticle();
	DrawDebugInfo();
}

void CBattleMap::Update(float fElapsedTime)
{
	//CPlayer::GetInstance()->Update(fElapsedTime);
	m_pParticleSys->UpdateParticle(fElapsedTime, m_ptMouseScreenCoord);
	if (m_nMoveDirection != -1)
	{
		POINT newPt = m_vCharacters[m_nCurrCharacter].GetMapCoord();
		switch(m_nMoveDirection)
		{
		case MOVE_ADD_X:
			++newPt.x;
			if (newPt.x > m_nNumCols-1)
				newPt.x = m_nNumCols-1;
			break;
		case MOVE_ADD_Y:
			++newPt.y;
			if (newPt.y > m_nNumRows-1)
				newPt.y = m_nNumRows-1;
			break;
		case MOVE_MINUS_X:
			--newPt.x;
			if (newPt.x < 2)
				newPt.x = 2;
			break;
		case MOVE_MINUS_Y:
			--newPt.y;
			if (newPt.y < 2)
				newPt.y = 2;
			break;
		}
		int id = newPt.y * m_nNumCols + newPt.x;
		if (m_pTilesL1[id].Flag() != FLAG_OBJECT_EDGE && m_pTilesL1[id].Flag() != FLAG_COLLISION && 
			(m_vCharacters[m_nCurrCharacter].GetCurrAP() >= (abs(newPt.x - m_vCharacters[m_nCurrCharacter].GetMapCoord().x) +
				abs(newPt.y - m_vCharacters[m_nCurrCharacter].GetMapCoord().y) ) * 2))
		{
			m_vCharacters[m_nCurrCharacter].SetCurrTile(newPt, GetOffsetX(), GetOffsetY(), m_nTileWidth, m_nTileHeight, m_nNumCols);
			m_pPlayer->GetTurtles()[m_nCurrCharacter]->SetCurrTile(newPt, GetOffsetX(), GetOffsetY(), m_nTileWidth, m_nTileHeight, m_nNumCols);
			m_nCurrCharacterTile = m_pPlayer->GetTurtles()[m_nCurrCharacter]->GetCurrTile();
			m_vCharacters[m_nCurrCharacter].DecrementCurrAP(2);
			CalculateRanges();
		}
		m_nMoveDirection = -1;
	}
	if (m_bNotEnoughAP || m_bOutOfRange)
	{
		m_fTimer += fElapsedTime;
	}
}

bool CBattleMap::Input(float fElapsedTime, POINT mouse)
{
	m_ptMouseScreenCoord = mouse;
	mouse.x -= (LONG)m_fScrollX;
	mouse.y -= (LONG)m_fScrollY;
	//if (m_pDI->KeyPressed(DIK_ESCAPE))
	//	return false;

	// Keyboard input
	if (!HandleKeyBoardInput(fElapsedTime))
		return false;	
	// Mouse movement (edge of screen to move camera)
	if (m_bIsPlayersTurn)
	{
		if (m_ptMouseScreenCoord.x < CAM_EDGE_DIST_TO_MOVE)
			MoveCamLeft(fElapsedTime);
		if (m_ptMouseScreenCoord.x > m_nScrenWidth-CAM_EDGE_DIST_TO_MOVE)
			MoveCamRight(fElapsedTime);
		if (m_ptMouseScreenCoord.y < CAM_EDGE_DIST_TO_MOVE)
			MoveCamUp(fElapsedTime);
		if (m_ptMouseScreenCoord.y > m_nScreenHeight-CAM_EDGE_DIST_TO_MOVE)
			MoveCamDown(fElapsedTime);
	
		int xID, yID;
		// transform the mouse into map coordinates
		xID = ((m_nTileWidth * (mouse.y )) + (m_nTileHeight * (mouse.x - m_nIsoCenterTopX ))) / (m_nTileWidth * m_nTileHeight);
		yID = ((m_nTileWidth * (mouse.y )) - (m_nTileHeight * (mouse.x - m_nIsoCenterTopX ))) / (m_nTileWidth * m_nTileHeight);
		// these check for the mouse being off the map
		// if it is, it is reset to the lowest row/column
		if (xID >= m_nNumCols && yID >= m_nNumRows)
		{
			yID = m_nNumRows-1;
			xID = m_nNumCols-1;
		}	
		else if (xID < 2 && yID < 2)
			yID = xID = 2;
		else if (xID < 2 && yID >= m_nNumRows)
		{
			xID = 2;
			yID = m_nNumRows-1;
		}
		else if (xID >= m_nNumCols && yID < 2)
		{
			yID = 2;
			xID = m_nNumCols-1;
		}
		else if (xID < 2)
			xID = 2;
		else if (yID < 2)
			yID = 2;
		else if (xID >= m_nNumCols)
			xID = m_nNumCols-1;
		else if (yID >= m_nNumRows)
			yID = m_nNumRows-1;
	
		m_nCurrSelectedTile = yID * m_nNumCols + xID;	// get the tile ID
		// Mouse -- determine if the mouse is over a character

		// debugging
		if (m_pDI->KeyPressed(DIK_D))
			int i = 0;

		HandleMouseInput(fElapsedTime, mouse, xID, yID);
	}

	return true;
}

void CBattleMap::CreateEnemies()
{
	for (int i = 0; i < m_nNumEnemiesLeft; ++i)
	{
		CNinja* ninja = Factory::GetInstance()->CreateNinja();
		m_vCharacters.push_back((CBase)*ninja);
		m_vEnemies.push_back((CBase*)ninja);
	}
}

void CBattleMap::LoadMapInfo()
{
	ifstream ifs;
	ifs.exceptions(~ios_base::goodbit);
	
	try
	{
		ifs.open(m_szFileName, ios_base::in | ios_base::binary);
	}
	catch(ios_base::failure &)
	{
		if (!ifs.is_open())
		{
			char szBuffer[128];
			sprintf_s(szBuffer, "Failed to open file: %s", m_szFileName );
			MessageBox(0, szBuffer, "Incorrect version.", MB_OK);
			m_pGame->ChangeState(CMainMenuState::GetInstance());
		}
		if (ifs.eof())
		{ifs.close();return;}		
	}
	//read input from the given binary file
	string version, eat, name, tilesetName, fileName;
	char file[256];
	char buff[256];
	ZeroMemory(buff, 256);
	byte size;
	ifs.read(reinterpret_cast<char*>(&size), 1);
	ifs.read(buff, size);

	version = buff;
	try
	{
		if (version == m_strCurrVersion)	// make sure we are loading the current version
		{
			// Read in basic map info:
			// Total number of tiles on map
			// number of columns
			// number of rows
			ifs.read(reinterpret_cast<char*>(&m_nTotalNumTiles), 4);
			ifs.read(reinterpret_cast<char*>(&m_nNumCols), 4);
			ifs.read(reinterpret_cast<char*>(&m_nNumRows), 4);

			// the exact center of the map's x and y, offset by half height and width
			m_nTileWidth = 64; m_nTileHeight = 32;	// hard-coded, grid units are 64x32
			//////////////////////////////////////////////////////////////////////////
			//	set up the map so that it centers at the beginning
			m_nMapWidth = m_nTileWidth * m_nNumCols; m_nMapHeight = m_nTileHeight * m_nNumRows;

			m_nIsoCenterLeftY = -(m_nTileHeight<<1)+(m_nTileHeight>>1) - ((m_nMapHeight >> 1) - (m_nScreenHeight >> 1));
			m_nIsoCenterTopX = (int)((((float)(m_nNumCols)) / 2.0f) * m_nTileWidth - (m_nTileWidth<<1));
			m_nFreeTileOSx = (m_nTileWidth<<1);	
			m_nfreeTileOSy = (m_nTileHeight<<1);
			// allocate memory for layer 1, 2, and 3(free placed tiles)
			m_pTilesL1  = new CTile[m_nNumRows*m_nNumCols];
			m_pTilesL2  = new CTile[m_nNumRows*m_nNumCols];
			m_pFreeTiles= new CFreeTile[m_nNumRows*m_nNumCols];
			SetOffsetX((int)m_fScrollX + m_nIsoCenterTopX - (m_nTileWidth >> 1));
			SetOffsetY((int)m_fScrollY + m_nIsoCenterLeftY - (m_nTileHeight >> 1));
			SetFTosX((int)m_fScrollX - ((m_nMapWidth >> 1) - (m_nScrenWidth >> 1)) - (m_nTileWidth >> 1));
			SetFTosY((int)m_fScrollY - m_nTileHeight);
		}
		else // didn't have the correct version number...
		{
			char szBuffer[128];
			sprintf_s(szBuffer, "Current version: %s ...does not match loaded version: %s", m_strCurrVersion.c_str(), version.c_str());
			MessageBox(0, szBuffer, "Incorrect version.", MB_OK);
			PostQuitMessage(1);
		}
	}
	catch(ios_base::failure &)
	{
		if (!ifs.eof())
			throw;
		else
			ifs.close();
	}

	int tilesetCount = 0;	// how many tilesets are we going to be drawing from?
	int xID, yID, destID, sourceID, flag, width, height; // tile variables
	string trigger;	// tile trigger string
	byte red, green, blue; // for tileset key color

	try 
	{
		ZeroMemory(buff, 256);
		ifs.read(reinterpret_cast<char*>(&size), 1);
		ifs.read(buff, size);
		name = buff;
		//////////////////////////////////////////////////////////////////////////
		// Loading in tilesets
		// if there's a tileset..load the image with the correct key color
		while (name == "Tileset")
		{
			ZeroMemory(buff, size);
			ifs.read(reinterpret_cast<char*>(&size), 1);
			ifs.read(buff, size);
			fileName = buff;
			strcpy_s(file, fileName.c_str());

			ZeroMemory(buff, 256);
			ifs.read(reinterpret_cast<char*>(&size), 1);
			ifs.read(buff, size);
			name = buff;

			ifs.read(reinterpret_cast<char*>(&red), 1);
			ifs.read(reinterpret_cast<char*>(&green), 1);
			ifs.read(reinterpret_cast<char*>(&blue), 1);
			// store the tileset information so we can determine where each tile comes from
			m_pTilesets[tilesetCount].id = m_pTM->LoadTexture(file, D3DCOLOR_ARGB(255,red, green, blue));
			m_pTilesets[tilesetCount++].name = name;

			ZeroMemory(buff, 256);
			ifs.read(reinterpret_cast<char*>(&size), 1);
			ifs.read(buff, size);
			name = buff;
		}
	}
	catch (ios_base::failure &)
	{
		if (!ifs.eof())
			throw;
		else
			ifs.close();
	}
	int imageID; int sPos;
	sPos = ifs.tellg();
	try
	{
		//////////////////////////////////////////////////////////////////////////
		// Layer ONE
		int count = 0;
		while (name == "Layer1")
		{
			// read in tile's tileset name (which tileset it came from)
			ZeroMemory(buff, 256);
			ifs.read(reinterpret_cast<char*>(&size), 1);
			ifs.read(buff, size);
			tilesetName = buff;

			// determine which image id this tile should have
			for (int tsIndex = 0; tsIndex < tilesetCount; ++tsIndex)
			{
				if (tilesetName == m_pTilesets[tsIndex].name)
				{
					imageID = m_pTilesets[tsIndex].id;
					break;
				}
			}
			ifs.read(reinterpret_cast<char*>(&sourceID), 4);
			ifs.read(reinterpret_cast<char*>(&xID), 4);
			ifs.read(reinterpret_cast<char*>(&yID), 4);
			ifs.read(reinterpret_cast<char*>(&flag), 4);
			ifs.read(reinterpret_cast<char*>(&width), 4);
			ifs.read(reinterpret_cast<char*>(&height), 4);
			ZeroMemory(buff, 256);
			ifs.read(reinterpret_cast<char*>(&size), 1);
			ifs.read(buff, size);
			trigger = buff;

			// setting up each tile of the first layer
			destID = yID * m_nNumCols + xID;	// the id of the tile in the 1-d array
			m_pTilesL1[destID] = CTile(sourceID, imageID, m_nNumCols, xID, yID, width, height, flag, trigger);
			// skip ahead to determine if we have more tiles
			// or if we move on to layer 2 ("L") or 3 ("F")
			sPos = ifs.tellg();
			ifs.seekg(sPos+1);
			eat = ifs.peek();
			count++;
			if (count == 400)
				int i = 0;
			if (eat == "L" || eat == "F")
				break;
			else
				ifs.seekg(sPos);
		}
	}
	catch(ios_base::failure &)
	{
		if (!ifs.eof())
			throw;
		else
			ifs.clear();ifs.close(); return;
	}
			
	try
	{
		if (m_pTilesL1[0].DestXID() != -1)
		{
			ifs.seekg(sPos);	// set the input stream position back to where it needs to be
			//////////////////////////////////////////////////////////////////////////
			// Layer TWO
			ZeroMemory(buff, 256);
			ifs.read(reinterpret_cast<char*>(&size), 1);
			ifs.read(buff, size);
			name = buff;
 		}
		while (name == "Layer2")
		{
			// read in tile's tileset name (which tileset it came from)
			ZeroMemory(buff, 256);
			ifs.read(reinterpret_cast<char*>(&size), 1);
			ifs.read(buff, size);
			tilesetName = buff;
			// determine which image id this tile should have
			for (int tsIndex = 0; tsIndex < tilesetCount; ++tsIndex)
			{
				if (tilesetName == m_pTilesets[tsIndex].name)
				{
					imageID = m_pTilesets[tsIndex].id;
					break;
				}
			}
			ifs.read(reinterpret_cast<char*>(&sourceID), 4);
			ifs.read(reinterpret_cast<char*>(&xID), 4);
			ifs.read(reinterpret_cast<char*>(&yID), 4);
			ifs.read(reinterpret_cast<char*>(&flag), 4);
			ifs.read(reinterpret_cast<char*>(&width), 4);
			ifs.read(reinterpret_cast<char*>(&height), 4);
			ZeroMemory(buff, 256);
			ifs.read(reinterpret_cast<char*>(&size), 1);
			ifs.read(buff, size);
			trigger = buff;

			// setting up each tile of the first layer
			destID = yID * m_nNumCols + xID;
			m_pTilesL2[destID] = CTile(sourceID, imageID, m_nNumCols, xID, yID, width, height, flag, trigger);
			sPos = ifs.tellg();
			ifs.seekg(sPos+1);
			eat = ifs.peek();
			if (eat == "F")
				break;
			else
				ifs.seekg(sPos);
		}

	}
	catch (ios_base::failure &)
	{
		if (!ifs.eof())
			throw;
		else
			ifs.clear();ifs.close(); return;	
	}

	try
	{
		if (m_pTilesL2[0].DestXID() != -1 && m_pTilesL1[0].DestXID() != -1 || eat[0] == 'F')
		{
			ifs.seekg(sPos);
			ZeroMemory(buff, 256);
			ifs.read(reinterpret_cast<char*>(&size), 1);
			ifs.read(buff, size);
			name = buff;
		}
		//////////////////////////////////////////////////////////////////////////
		// the Free Placed layer 
		int srcPosX, srcPosY, destX, destY, count = 0;	// unique variables to load in for CFreeTiles
		// this is the last layer, so check for end of file
		while (!ifs.eof())
		{
			// read in tile's tileset name (which tileset it came from)
			ZeroMemory(buff, 256);
			ifs.read(reinterpret_cast<char*>(&size), 1);
			ifs.read(buff, size);
			tilesetName = buff;

			// determine which image id this tile should have
			for (int tsIndex = 0; tsIndex < tilesetCount; ++tsIndex)
			{
				if (tilesetName == m_pTilesets[tsIndex].name)
				{
					imageID = m_pTilesets[tsIndex].id;
					break;
				}
			}

			ifs.read(reinterpret_cast<char*>(&srcPosX), 4);
			ifs.read(reinterpret_cast<char*>(&srcPosY), 4);
			ifs.read(reinterpret_cast<char*>(&flag), 4);
			ifs.read(reinterpret_cast<char*>(&destX), 4);
			ifs.read(reinterpret_cast<char*>(&destY), 4);
			ifs.read(reinterpret_cast<char*>(&width), 4);
			ifs.read(reinterpret_cast<char*>(&height), 4);
			ZeroMemory(buff, 256);
			ifs.read(reinterpret_cast<char*>(&size), 1);
			ifs.read(buff, size);
			trigger = buff;

			m_pFreeTiles[count++] = CFreeTile(srcPosX, srcPosY, imageID, destX, destY, width, height, flag, trigger);
			m_nFreeTileCount = count;
		}
	}
	catch(ios_base::failure &)
	{
		if (!ifs.eof())
			throw;
		else
			ifs.close(); return;
	}
}

float CBattleMap::GetZdepthDraw(int xAnchor, int yAnchor, int currTileID)
{
	if (CSGD_DirectInput::GetInstance()->KeyPressed(DIK_D))	// if need to do debugging
		int i = 0;

	//  for the character to be ahead of the object,
	//	it must be directly below, to the southeast, or
	//	to the southwest of an edge tile...
	//	otherwise it is behind the object.
	if (currTileID != -1)
	{
		if (m_pTilesL1[currTileID-(m_nNumCols+1)].Flag() == FLAG_OBJECT_EDGE ||
				m_pTilesL1[currTileID-m_nNumCols].Flag() == FLAG_OBJECT_EDGE ||
				m_pTilesL1[currTileID-1].Flag() == FLAG_OBJECT_EDGE ||
				m_pTilesL1[currTileID-((m_nNumCols+1)<<1)].Flag() == FLAG_OBJECT_EDGE ||
				m_pTilesL1[currTileID-(m_nNumCols+2)].Flag() == FLAG_OBJECT_EDGE ||
				m_pTilesL1[currTileID-(((m_nNumCols+1)<<1)-1)].Flag() == FLAG_OBJECT_EDGE )
		{
			return depth.CHARACTER_AHEAD;
		}
		else
			return depth.CHARACTER_BEHIND;
	} 
	else // shouldn't let it get here...
		return 0;
}

void CBattleMap::DrawDebugInfo()
{
	//char szAnchorPt[64];
	//sprintf_s(szAnchorPt, "A-PT X:%i, Y:%i", turtleX, turtleY);
	//CSGD_Direct3D::GetInstance()->DrawText(szAnchorPt, 5, 5);	

	char szMousePt[64];
	sprintf_s(szMousePt, "M-PT X:%i, Y:%i", m_ptMouseScreenCoord.x, m_ptMouseScreenCoord.y);
	CSGD_Direct3D::GetInstance()->DrawText(szMousePt, 10, 720, 0, 0, 0);	
}

MY_POINT CBattleMap::IsoTilePlot(MY_POINT pt, int xOffset, int yOffset)
{
	MY_POINT newPt;
	newPt.x = (pt.x - pt.y) * (m_nTileWidth >> 1) + xOffset;
	newPt.y = (pt.x + pt.y) * (m_nTileHeight >> 1) + yOffset;
	return newPt;
}

int CBattleMap::IsMousePosValid(POINT mousePt)
{
	int size = m_vCharacters.size();
	for (int i = 0; i < size; ++i)
	{
		RECT currRect = m_vCharacters[i].GetRect();
		if (mousePt.x >= currRect.left - (int)m_fScrollX &&
			mousePt.x <= currRect.right - (int)m_fScrollX &&
			mousePt.y >= currRect.top - (int)m_fScrollY &&
			mousePt.y <= currRect.bottom - (int)m_fScrollY)
		{
			return i;
		}
	}
	// determine if the mouse is over an invalid tile for movement
	if (m_pTilesL1[m_nCurrSelectedTile].Flag() != FLAG_COLLISION)
	{
		m_nCurrMouseTileTarget = m_nCurrSelectedTile;
	}
	if (m_pTilesL1[m_nCurrSelectedTile].Flag() != FLAG_OBJECT_EDGE)
	{
		m_nCurrMouseTileTarget = m_nCurrSelectedTile;
	}
	if (m_pTilesL1[m_nCurrSelectedTile].Flag() == FLAG_COLLISION ||
			 m_pTilesL1[m_nCurrSelectedTile].Flag() == FLAG_OBJECT_EDGE)
	{
		m_nCurrMouseTileTarget = m_nCurrSelectedTile = -1;
	}
	return -1;
}

void CBattleMap::CalculateRanges()
{
	POINT ptGridLocation = m_vCharacters[m_nCurrCharacter].GetMapCoord();

	// reset alphas
	for(int nx = 2; nx < m_nNumCols; ++nx)
		for(int ny = 2; ny < m_nNumRows; ++ny)
		{
			int id = ny*m_nNumCols+nx;
			if (m_pTilesL1[id].Alpha() != 255)
				m_pTilesL1[id].SetAlpha(255);
		}
	// mark tiles to be drawn with range color
	// scan the neighbors
	int range = m_vCharacters[m_nCurrCharacter].GetCurrAP();
	for(int nx = ptGridLocation.x - range; nx <= ptGridLocation.x + range; ++nx)
	{
		for(int ny = ptGridLocation.y - range; ny <= ptGridLocation.y + range; ++ny)
		{
			//make sure the neighbor is on the map
			if(nx >= 2 && ny >= 2 && nx < m_nNumCols && ny < m_nNumRows
				&& !(nx == ptGridLocation.x && ny == ptGridLocation.y))
			{
				int distance = ((abs(nx - ptGridLocation.x) + abs( ny - ptGridLocation.y) ) << 1);
				int ap		 = m_vCharacters[m_nCurrCharacter].GetCurrAP();
				int id		 = ny*m_nNumCols+nx;
				if ( distance <= ap && m_pTilesL1[id].Flag() != FLAG_OBJECT_EDGE && m_pTilesL1[id].Flag() != FLAG_COLLISION)
				{
					m_pTilesL1[id].SetAlpha(200);
				}
			}
		}
	}
	if (m_nCurrTarget > -1)
	{
		m_nDistanceToTarget = (abs(m_vCharacters[m_nCurrTarget].GetMapCoord().x - m_vCharacters[m_nCurrCharacter].GetMapCoord().x) + 
			abs(m_vCharacters[m_nCurrTarget].GetMapCoord().y - m_vCharacters[m_nCurrCharacter].GetMapCoord().y));
	}
}

void CBattleMap::DrawHover()
{
	RECT hoverRect;
	if (m_nHoverCharacter < 4)
	{
		hoverRect = m_vCharacters[m_nHoverCharacter].GetRect();
		m_pD3D->DrawLine(hoverRect.left, hoverRect.top, hoverRect.right, hoverRect.top, 0,0,255);	// top line
		m_pD3D->DrawLine(hoverRect.left, hoverRect.top, hoverRect.left, hoverRect.bottom, 0,0,255);	// left line
		m_pD3D->DrawLine(hoverRect.right, hoverRect.top, hoverRect.right, hoverRect.bottom, 0,0,255);	// right line
		m_pD3D->DrawLine(hoverRect.left, hoverRect.bottom, hoverRect.right, hoverRect.bottom, 0,0,255);	// bottom line
	}
	RECT targetRect;
	if (m_nHoverCharacter > 3)
	{
		targetRect= m_vCharacters[m_nHoverCharacter].GetRect();
		m_pD3D->DrawLine(targetRect.left, targetRect.top, targetRect.right, targetRect.top, 255,0,0);	// top line
		m_pD3D->DrawLine(targetRect.left, targetRect.top, targetRect.left, targetRect.bottom, 255,0,0);	// left line
		m_pD3D->DrawLine(targetRect.right, targetRect.top, targetRect.right, targetRect.bottom, 255,0,0);	// right line
		m_pD3D->DrawLine(targetRect.left, targetRect.bottom, targetRect.right, targetRect.bottom, 255,0,0);	// bottom line
	}

	if (m_nHoverCharacter == m_nCurrTarget && m_bIsMouseAttack)
		SetMousePtr(m_pAssets->aMouseAttackID);
	else if (m_nHoverCharacter > 3)
		SetMousePtr(m_pAssets->aMouseMagGlassID);
	else
		SetMousePtr(m_pAssets->aMousePointerID);
}

void CBattleMap::SetStartPositions()
{
	// mapCoordinates represents the grid x=column, y=row
	POINT mapCoordinate;
	mapCoordinate.x = 10;
	mapCoordinate.y = 18;
	m_pPlayer->GetTurtles()[LEONARDO]->SetCurrTile(mapCoordinate, GetOffsetX(), GetOffsetY(), m_nTileWidth, m_nTileHeight, m_nNumCols);

	mapCoordinate.x = 4;
	mapCoordinate.y = 18;
	m_pPlayer->GetTurtles()[DONATELLO]->SetCurrTile(mapCoordinate, GetOffsetX(), GetOffsetY(), m_nTileWidth, m_nTileHeight, m_nNumCols);

	mapCoordinate.x = 3;
	mapCoordinate.y = 17;
	m_pPlayer->GetTurtles()[RAPHAEL]->SetCurrTile(mapCoordinate, GetOffsetX(), GetOffsetY(), m_nTileWidth, m_nTileHeight, m_nNumCols);

	mapCoordinate.x = 4;
	mapCoordinate.y = 17;
	m_pPlayer->GetTurtles()[MIKEY]->SetCurrTile(mapCoordinate, GetOffsetX(), GetOffsetY(), m_nTileWidth, m_nTileHeight, m_nNumCols);
	m_vCharacters.clear();
	for (int i = 0; i < 4; ++i)
		m_vCharacters.push_back((CBase)(*m_pPlayer->GetTurtles()[i]));

	for (int i = 0; i < m_nNumEnemiesLeft; ++i)
	{
		mapCoordinate.x = rand() % (18 - 3) + 2;
		mapCoordinate.y = rand() % (18 - 3) + 2;
		m_vEnemies[i]->SetCurrTile(mapCoordinate, GetOffsetX(), GetOffsetY(), m_nTileWidth, m_nTileHeight, m_nNumCols);
		m_vCharacters.push_back(*m_vEnemies[i]);
	}
}

void CBattleMap::UpdatePositions()	// updates the CPlayer's turtles and the enemy characters
{
	for (int i = 0; i < 4; ++i)
		m_pPlayer->GetTurtles()[i]->SetCurrTile(m_vCharacters[i].GetMapCoord(), GetOffsetX(), GetOffsetY(), m_nTileWidth, m_nTileHeight, m_nNumCols);
	for (int i = 0; i < m_nNumEnemiesLeft; ++i)
		m_vEnemies[i]->SetCurrTile(m_vEnemies[i]->GetMapCoord(), GetOffsetX(), GetOffsetY(), m_nTileWidth, m_nTileHeight, m_nNumCols);
}

bool CBattleMap::HandleKeyBoardInput(float fElapsedTime)
{
	if (m_bIsPlayersTurn)
	{
		// Keyboard
		// camera movement
		if (m_pDI->KeyDown(DIK_A))
		{
			MoveCamLeft(fElapsedTime);
		}
		if (m_pDI->KeyDown(DIK_D))
		{
			MoveCamRight(fElapsedTime);
		}
		if (m_pDI->KeyDown(DIK_S))
		{
			MoveCamDown(fElapsedTime);
		}
		if (m_pDI->KeyDown(DIK_W))
		{
			MoveCamUp(fElapsedTime);
		}
		// character movement
		if (m_nCurrCharacter != -1)
		{
			if (m_pDI->KeyPressed(DIK_NUMPAD7))
			{
				m_nMoveDirection = MOVE_MINUS_X;
			}
			else if (m_pDI->KeyPressed(DIK_NUMPAD3))
			{
				m_nMoveDirection = MOVE_ADD_X;
			}
			else if (m_pDI->KeyPressed(DIK_NUMPAD9))
			{
				m_nMoveDirection = MOVE_MINUS_Y;	
			}
			else if (m_pDI->KeyPressed(DIK_NUMPAD1))
			{
				m_nMoveDirection = MOVE_ADD_Y;
			}
		}
	}
	if (m_pDI->KeyPressed(DIK_SPACE))
	{
		m_bIsPlayersTurn = !m_bIsPlayersTurn;
		// reset APs
		for (int i = 0; i < 4; ++i)
		{
			m_vCharacters[i].SetCurrAP(m_vCharacters[i].GetBaseAP());
			if (m_nCurrCharacter > -1)
				CalculateRanges();
		}
	}
	return true;
}
void CBattleMap::HandleMouseInput(float fElapsedTime, POINT mouse, int xID, int yID)
{
	int index = IsMousePosValid(mouse);
	if (index != -1)
	{
		m_nHoverCharacter = index;
	}
	else
		m_nHoverCharacter = -1;

	// is the mouse over an action box button?
	if (m_bIsPlayersTurn)
	{
		m_nCurrBtnSelected = m_bxActionBox->Input(m_ptMouseScreenCoord);
		if (m_nCurrBtnSelected > -1)
			SetMousePtr(m_pAssets->aMousePointerID);
	}

	// when the left mouse button is clicked
	if (m_pDI->MouseButtonPressed(MOUSE_LEFT))
	{
		// for movement, see if the tile is open
		if (m_nCurrMouseTileTarget != -1 && m_nCurrCharacter > -1)
		{
			// it's an open tile...

			// is tile occupied?
			bool bOccupied = false;	
			int tileID = yID * m_nNumCols + xID;
			for (int i = 0; i < m_nNumCharacters; ++i)
				if (m_vCharacters[i].GetCurrTile() == tileID)
				{bOccupied = true; break; }

			// now check if the character has enough Action Points
			int dist = (abs(xID - m_vCharacters[m_nCurrCharacter].GetMapCoord().x) + abs(yID - m_vCharacters[m_nCurrCharacter].GetMapCoord().y)) * 2;
			if (m_vCharacters[m_nCurrCharacter].GetCurrAP() >= dist && !bOccupied)
			{
				POINT mPoint; mPoint.x = xID, mPoint.y = yID;
				m_vCharacters[m_nCurrCharacter].SetCurrTile(mPoint, GetOffsetX(), GetOffsetY(), m_nTileWidth, m_nTileHeight, m_nNumCols);
				m_nCurrCharacterTile = m_vCharacters[m_nCurrCharacter].GetCurrTile();
				m_vCharacters[m_nCurrCharacter].DecrementCurrAP(dist);
				UpdatePositions();
				m_pPlayer->GetTurtles()[m_nCurrCharacter].SetCurrAP(m_vCharacters[m_nCurrCharacter].GetCurrAP());
				CalculateRanges();
			}
		}
		switch (m_nCurrBtnSelected)
		{
		case ACTION_SPECIAL:
			m_bDisplaySpecialBox = true;
			break;
		case ACTION_ITEM:
			break;
		case ACTION_ENDTURN:
			m_bIsPlayersTurn = false;
			break;
		default:
			break;
		}
	}
	else if (m_pDI->MouseButtonPressed(MOUSE_RIGHT))
	{
		if (index > -1 && index < 4)		// see if we're clicking on a character (turtle)
		{
			m_nCurrCharacter	 = index;
			m_nCurrCharacterTile = m_vCharacters[index].GetCurrTile();
			CalculateRanges();
		}
		else if (index > 3 && m_nCurrTarget != m_nHoverCharacter && m_nCurrCharacter > -1) // or if we're clicking on an enemy to view their stats
		{
			m_bIsMouseAttack = true;
			m_nCurrTarget = index;
			m_ncurrTargetTile = m_vCharacters[m_nCurrTarget].GetCurrTile();
			m_nDistanceToTarget = (abs(m_vCharacters[m_nCurrTarget].GetMapCoord().x - m_vCharacters[m_nCurrCharacter].GetMapCoord().x) + 
								   abs(m_vCharacters[m_nCurrTarget].GetMapCoord().y - m_vCharacters[m_nCurrCharacter].GetMapCoord().y));
		}
		else if (index > 3 && m_nCurrTarget == m_nHoverCharacter)	// otherwise, attempting to attack
		{
			// check if in range
			if (m_nDistanceToTarget > m_vCharacters[m_nCurrCharacter].GetRange())
			{
				m_bOutOfRange = true;
				return;
			}
			// check if enough AP
			if (m_vCharacters[m_nCurrCharacter].GetCurrAP() < 4)
			{
				m_bNotEnoughAP = true;
				return;
			}
			else
				PerformAttack();
		}
	}
}
void CBattleMap::PerformAttack()
{
	int damage = ( (m_vCharacters[m_nCurrCharacter].GetStrength() - m_vCharacters[m_nCurrTarget].GetDefense()) + m_vCharacters[m_nCurrCharacter].GetAccuracy()) * 2;
	damage += rand() % (5 - (-4)) -5;

	m_vCharacters[m_nCurrCharacter].DecrementCurrAP(4);
	m_vCharacters[m_nCurrTarget].SetHealth(m_vCharacters[m_nCurrTarget].GetHealth() - damage);
	m_vEnemies[m_nCurrTarget-4]->SetHealth(m_vCharacters[m_nCurrTarget].GetHealth() - damage);

	CalculateRanges();
}
void CBattleMap::FindPathToTarget()
{
	POINT ptGridLocation = m_vCharacters[m_nCurrCharacter].GetMapCoord();
	int range = m_vCharacters[m_nCurrCharacter].GetCurrAP();
	int currTile = m_nCurrCharacterTile;
	int adjTiles[4]; int count = 0;

	while (currTile != m_ncurrTargetTile)
	{
		for(int nx = ptGridLocation.x - 1; nx <= ptGridLocation.x + 1; ++nx)
		{
			if (count == 4)
				count = 0;
			for(int ny = ptGridLocation.y - 1; ny <= ptGridLocation.y + 1; ++ny)
			{
				// skip tiles that are diagonal from the currTile
				if ((nx == ptGridLocation.x - 1 && ny == ptGridLocation.y - 1) ||
					(nx == ptGridLocation.x + 1 && ny == ptGridLocation.y - 1) ||
					(nx == ptGridLocation.x - 1 && ny == ptGridLocation.y + 1) ||
					(nx == ptGridLocation.x + 1 && ny == ptGridLocation.y + 1) )
				{
					continue;
					++count;
				}
				//make sure the neighbor is on the map
				if(nx >= 2 && ny >= 2 && nx < m_nNumCols && ny < m_nNumRows
					&& !(nx == ptGridLocation.x && ny == ptGridLocation.y))
				{

					++count;
				}
			}
		}
	}
}

void CBattleMap::MoveCamUp(float fElapsedTime)
{
	m_fScrollY += SCROLLSPEED * fElapsedTime;
	// tile offsets
	SetOffsetY((int)m_fScrollY + m_nIsoCenterLeftY - (m_nTileHeight >> 1));
	SetFTosY((int)m_fScrollY - m_nTileHeight);
	for (int i = 0; i < m_nNumCharacters; ++i)
		m_vCharacters[i].SetCurrTile(m_vCharacters[i].GetMapCoord(), GetOffsetX(), GetOffsetY(), m_nTileWidth, m_nTileHeight, m_nNumCols);
	UpdatePositions();
}
void CBattleMap::MoveCamDown(float fElapsedTime)
{
	m_fScrollY -= SCROLLSPEED * fElapsedTime;
	// tile offsets
	SetOffsetY((int)m_fScrollY + m_nIsoCenterLeftY - (m_nTileHeight >> 1));
	SetFTosY((int)m_fScrollY - m_nTileHeight);
	for (int i = 0; i < m_nNumCharacters; ++i)
		m_vCharacters[i].SetCurrTile(m_vCharacters[i].GetMapCoord(), GetOffsetX(), GetOffsetY(), m_nTileWidth, m_nTileHeight, m_nNumCols);
	UpdatePositions();
}
void CBattleMap::MoveCamLeft(float fElapsedTime)
{
	m_fScrollX += SCROLLSPEED * fElapsedTime;
	// tile offsets
	SetOffsetX((int)m_fScrollX + m_nIsoCenterTopX - (m_nTileWidth >> 1));
	SetFTosX((int)m_fScrollX - ((m_nMapWidth >> 1) - (m_nScrenWidth >> 1)) - (m_nTileWidth >> 1));
	for (int i = 0; i < m_nNumCharacters; ++i)
		m_vCharacters[i].SetCurrTile(m_vCharacters[i].GetMapCoord(), GetOffsetX(), GetOffsetY(), m_nTileWidth, m_nTileHeight, m_nNumCols);
	UpdatePositions();
}
void CBattleMap::MoveCamRight(float fElapsedTime)
{
	m_fScrollX -= SCROLLSPEED * fElapsedTime;
	// tile offsets
	SetOffsetX((int)m_fScrollX + m_nIsoCenterTopX - (m_nTileWidth >> 1));
	SetFTosX((int)m_fScrollX - ((m_nMapWidth >> 1) - (m_nScrenWidth >> 1)) - (m_nTileWidth >> 1));
	for (int i = 0; i < m_nNumCharacters; ++i)
		m_vCharacters[i].SetCurrTile(m_vCharacters[i].GetMapCoord(), GetOffsetX(), GetOffsetY(), m_nTileWidth, m_nTileHeight, m_nNumCols);
	UpdatePositions();
}
void CBattleMap::CenterCam(float fElapsedTime)
{

}

void CBattleMap::DrawBoxes()
{
	//m_bxActionBox->Render();
}