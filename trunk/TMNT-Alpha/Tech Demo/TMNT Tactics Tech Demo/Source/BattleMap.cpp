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
#include "Animation.h"
//#include "fmod.hpp"
#include "BitmapFont.h"
#include "ParticleSystem.h"
#include "HUD.h"
#include <fstream>
#include <exception>

#define BOUNDING_BOXES 0
#define SCROLLSPEED 150.0f
// default constructor may be used for
// blank maps...a cool animation sequence,
// or needing to render while not in a real battle, etc...
CBattleMap::CBattleMap(void)
{
	m_pAssets		= CAssets::GetInstance();
	m_pTM			= CSGD_TextureManager::GetInstance();
	m_pGame			= CGame::GetInstance();
	m_pDI			= CSGD_DirectInput::GetInstance();
//	m_pFMOD			= m_pGame->GetFMODSystem();
//	m_pBitmapFont   = CBitmapFont::GetInstance();
	m_pHUD			= CHUD::GetInstance();

	m_pTilesL1 = NULL;
	m_pTilesL2 = NULL;
	m_pFreeTiles = NULL;
	//m_pMoveableTiles = NULL;

	m_nNumEnemiesLeft = 0;
	m_nNumCols = 0;
	m_nNumRows = 0;
	m_nTotalNumTiles = 0;
	m_nTileWidth = 0;
	m_nTileHeight = 0;
	m_nBGImageID = -1;
	m_nDotImageID = -1;
	m_nCursorImageID = -1;
	m_nCurrSelectedTile = -1;
	m_nMapHeight = 0;
	m_nMapWidth = 0;
	m_fScrollX = 0.0f;
	m_fScrollY = 0.0f;
	m_nScrenWidth = m_pGame->GetScreenWidth();
	m_nScreenHeight = m_pGame->GetScreenHeight();
	m_szFileName = "Blank.xml";
	m_szMapName  = "Blank";

	m_strCurrVersion = "TED-Version-1.0";

	//m_pHUD->LoadHUD();
}

CBattleMap::CBattleMap(char* szFileName, char* szMapName, int nNumEnemies)
{
	m_pAssets		= CAssets::GetInstance();
	m_pTM			= CSGD_TextureManager::GetInstance();
	m_pGame			= CGame::GetInstance();
	m_pDI			= CSGD_DirectInput::GetInstance();

	m_pBitmapFont   = CBitmapFont::GetInstance();
	//  Sm_pFMOD		= m_pGame->GetFMODSystem();
	m_pHUD			= CHUD::GetInstance();

	m_pTilesL1 = NULL;
	m_pTilesL2 = NULL;
	m_pFreeTiles = NULL;
	//m_pMoveableTiles = NULL;

	m_nNumEnemiesLeft = nNumEnemies;
	m_szFileName = szFileName;
	m_szMapName  = szMapName;
	m_nNumCols = 0;
	m_nNumRows = 0;
	m_nTotalNumTiles = 0;
	m_nTileWidth = 0;
	m_nTileHeight = 0;
	m_nBGImageID = -1;
	m_nDotImageID = -1;
	m_nCurrSelectedTile = -1;
	m_nMapHeight = 0;
	m_nMapWidth = 0;
	m_nFreeTileCount = 0;
	m_fScrollX = 0.0f;
	m_fScrollY = 0.0f;
	m_nScrenWidth = m_pGame->GetScreenWidth();
	m_nScreenHeight = m_pGame->GetScreenHeight();

	m_nCursorImageID = m_pAssets->aBMcursorID;
	m_strCurrVersion = "TED-Version-1.0";
	LoadMapInfo();
	m_pAnimation = new CAnimation();
	m_pAnimation->Load("Resources/AnimationInfo/VG_Michelangelo.dat");
	m_pParticleSys = new CParticleSystem();
	
	//m_pParticleSys->InitGeometry();
	//m_pParticleSys->InitParticle();
	m_pParticleSys->Load("Resources/ParticleInfo/VG_Test.dat");
	//m_pAnimation->Play();
	//CreateEnemies();
}

CBattleMap::~CBattleMap(void)
{
	delete[] m_pTilesL1;
	delete[] m_pTilesL2;
	delete[] m_pFreeTiles;
	delete m_pParticleSys;
	delete m_pAnimation;

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
	//m_pTM->Draw(m_nBGImageID, 0, 0);
	m_pHUD->Render();

	// tile offsets
	SetOffsetX((int)m_fScrollX + m_nIsoCenterTopX - (m_nTileWidth >> 1));
	SetOffsetY((int)m_fScrollY + m_nIsoCenterLeftY - (m_nTileHeight >> 1));
	SetFTosX((int)m_fScrollX - ((m_nMapWidth >> 1) - (m_nScrenWidth >> 1)) + m_nTileWidth);
	SetFTosY((int)m_fScrollY - m_nTileHeight);
	
	// draw layer one & two
	MY_POINT mapPT;
	for (int x = 0; x < m_nNumCols; ++x)
	{
		for (int y = 0; y < m_nNumRows; ++y)
		{
			int tileID = y*m_nNumCols+x;
			if (m_pTilesL1[tileID].DestXID() == -1)
				continue;
			if (m_pTilesL1[tileID].DestXID() != -1)
			{
				mapPT.x = x; mapPT.y = y;
				mapPT = IsoTilePlot(mapPT, GetOffsetX(), GetOffsetY());
				m_pTM->DrawWithZSort(m_pTilesL1[tileID].ImageID(), mapPT.x, mapPT.y, depth.GROUND, 1.0f, 1.0f, 
					m_pTilesL1[tileID].SourceRect(), 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(m_pTilesL1[tileID].Alpha(),255,255,255));
			}
			if (m_pTilesL2[tileID].DestXID() != -1)
			{
				mapPT.x = x; mapPT.y = y;
				mapPT = IsoTilePlot(mapPT, GetOffsetX(), GetOffsetY());
				m_pTM->DrawWithZSort(m_pTilesL2[tileID].ImageID(), mapPT.x, mapPT.y, depth.GROUNDL2, 0.9f, 1.0f, 
					m_pTilesL2[tileID].SourceRect(), 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(m_pTilesL2[tileID].Alpha(),255,255,255));
			}
			
			// draws selection rectangle on the currently selected tile
			if (m_nCurrSelectedTile == tileID)
			{
				m_pTM->DrawWithZSort(m_nCursorImageID, mapPT.x, mapPT.y, depth.SELECTION, 1.0f, 1.0f);
			//////////////////////////////////////////////////////////////////////////
			// characters would be rendered here:
				// the turtleX and turtleY represent the anchor point of the turtle in map space
				turtleX = m_pAnimation->GetFrames()[0].nAnchorX + mapPT.x;
				turtleY = m_pAnimation->GetFrames()[0].nAnchorY + mapPT.y;
				m_pAnimation->Render(mapPT.x + (m_nTileWidth >> 1), mapPT.y + (m_nTileHeight >> 1), 
									GetZdepthDraw(turtleX, turtleY, tileID)  );
			//////////////////////////////////////////////////////////////////////////
			}
		}
	}
	// draw the free placed layer
		//		RECT srcRect = {0, 0, 64, 50};
	for (int i = 0; i < m_nFreeTileCount; ++i)
	{
		// if the character is inside a map object, alpha that object out
		if (ObjectManager::GetInstance()->CheckObjectsToAlpha(m_pFreeTiles[i].SourceRect()))
		{
			m_pFreeTiles[i].SetAlpha(100);
		}
		else
			m_pFreeTiles[i].SetAlpha(255);
		m_pTM->DrawWithZSort(m_pFreeTiles[i].ImageID(), m_pFreeTiles[i].DestX()+GetFreeTileXos()-96, m_pFreeTiles[i].DestY()+GetFreeTileYos(),
			0.6f, .0f, 1.0f, m_pFreeTiles[i].SourceRect(), 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(m_pFreeTiles[i].Alpha(),255,255,255));
#if BOUNDING_BOXES
		// drawing bounding boxes
		CSGD_Direct3D::GetInstance()->DrawLine(m_pFreeTiles[i].DestX(), m_pFreeTiles[i].DestY(),
			m_pFreeTiles[i].DestX()+m_pFreeTiles[i].Width(), m_pFreeTiles[i].DestY(),
			255, 0, 0);
		CSGD_Direct3D::GetInstance()->DrawLine(m_pFreeTiles[i].DestX(), m_pFreeTiles[i].DestY()+m_pFreeTiles[i].Height(),
			m_pFreeTiles[i].DestX()+m_pFreeTiles[i].Width(), m_pFreeTiles[i].DestY()+m_pFreeTiles[i].Height(),
			255, 0, 0);
		CSGD_Direct3D::GetInstance()->DrawLine(m_pFreeTiles[i].DestX(), m_pFreeTiles[i].DestY(),
			m_pFreeTiles[i].DestX(), m_pFreeTiles[i].DestY()+m_pFreeTiles[i].Height(),
			255, 0, 0);
		CSGD_Direct3D::GetInstance()->DrawLine(m_pFreeTiles[i].DestX()+m_pFreeTiles[i].Width(), m_pFreeTiles[i].DestY(),
			m_pFreeTiles[i].DestX()+m_pFreeTiles[i].Width(), m_pFreeTiles[i].DestY()+m_pFreeTiles[i].Height(),
			255, 0, 0);
#endif
// 		m_pTM->Draw(m_pFreeTiles[i].ImageID(), m_pFreeTiles[i].DestX(), 
// 			m_pFreeTiles[i].DestY()+yOffset, 1.0f, 1.0f, &srcRect, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(m_pFreeTiles[i].Alpha(),255,255,255));
	}
	m_pParticleSys->DrawParticle();
	DrawDebugInfo();
}

MY_POINT CBattleMap::IsoTilePlot(MY_POINT pt, int xOffset, int yOffset)
{
	MY_POINT newPt;
	newPt.x = (pt.x - pt.y) * (m_nTileWidth >> 1) + xOffset;
	newPt.y = (pt.x + pt.y) * (m_nTileHeight >> 1) + yOffset;
	return newPt;
}

void CBattleMap::Update(float fElapsedTime)
{
	m_pAnimation->Update(fElapsedTime);
	m_pParticleSys->UpdateParticle(fElapsedTime, ms);
}

bool CBattleMap::Input(float fElapsedTime, POINT mouse)
{
	mouse.x -= (LONG)m_fScrollX;
	mouse.y -= (LONG)m_fScrollY;
	ms = mouse;
	//if (m_pDI->KeyPressed(DIK_ESCAPE))
	//	return false;

	// Keyboard
	if (m_pDI->KeyDown(DIK_LEFT))
	{
		m_fScrollX += SCROLLSPEED * fElapsedTime;
	}
	if (m_pDI->KeyDown(DIK_RIGHT))
	{
		m_fScrollX -= SCROLLSPEED * fElapsedTime;
	}
	if (m_pDI->KeyDown(DIK_DOWN))
	{
		m_fScrollY -= SCROLLSPEED * fElapsedTime;
	}
	if (m_pDI->KeyDown(DIK_UP))
	{
		m_fScrollY += SCROLLSPEED * fElapsedTime;
	}
	// Mouse
	if (m_pDI->MouseButtonPressed(MOUSE_LEFT))
	{
		m_pAnimation->Play();
	}

	int xID, yID;
	xID = ((m_nTileWidth * (mouse.y )) + (m_nTileHeight * (mouse.x - m_nIsoCenterTopX ))) / (m_nTileWidth * m_nTileHeight);
	yID = ((m_nTileWidth * (mouse.y )) - (m_nTileHeight * (mouse.x - m_nIsoCenterTopX ))) / (m_nTileWidth * m_nTileHeight);
	if (xID < 0)
	{
		xID = 0;
	}
	else if (xID >= m_nNumCols && yID >= m_nNumRows)
	{
		yID = m_nNumRows-1;
		xID = m_nNumCols-1;
	}
	else if (yID < 0)
	{
		yID = 0;
	}
	else if (xID >= m_nNumCols)
	{
		xID = m_nNumCols-1;
	}
	else if (yID >= m_nNumRows)
	{
		yID = m_nNumRows-1;
	}

	m_nCurrSelectedTile = yID * m_nNumCols + xID;

	// debugging
	if (m_pDI->KeyPressed(DIK_D))
		int i = 0;

	return true;
}

void CBattleMap::CreateEnemies()
{
// 	for (int i = 0; i < m_nNumEnemiesLeft; ++i)
// 	{
// 	}
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
			PostQuitMessage(1);
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
			m_pTilesL1  =new CTile[m_nNumRows*m_nNumCols];
			m_pTilesL2  =new CTile[m_nNumRows*m_nNumCols];
			m_pFreeTiles=new CFreeTile[m_nNumRows*m_nNumCols];
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
	if (m_pDI->KeyPressed(DIK_D))	// if need to do debugging
		int i = 0;

	//  for the character to be ahead of the object,
	//	it must be directly below, to the southeast, or
	//	to the southwest of an edge tile...
	//	otherwise it is behind the object.
	//	TODO::make sure to check for edge of map, so that tiles across the map aren't being looked at
	//			edge will be accounted for by not using the top two rows of the map...
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
	char szAnchorPt[64];
	sprintf_s(szAnchorPt, "A-PT X:%i, Y:%i", turtleX, turtleY);
	CSGD_Direct3D::GetInstance()->DrawText(szAnchorPt, 5, 5);	

	char szMousePt[64];
	sprintf_s(szMousePt, "M-PT X:%i, Y:%i", ms.x, ms.y);
	CSGD_Direct3D::GetInstance()->DrawText(szMousePt, 5, 25);	
}