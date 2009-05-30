//////////////////////////////////////////////////////////////////////////
//	Filename	:	CBattleMap.cpp
//
//	Author		:	Ramon Johannessen (RJ)
//
//	Purpose		:	To execute the individual battles that take place,
//					drawing the isometric tile map, updating, getting input, etc...
//////////////////////////////////////////////////////////////////////////
#include "BattleMap.h"
//#include "fmod.hpp"
#include "CSGD_TextureManager.h"
#include "Assets.h"
#include "CGame.h"
//#include "BitmapFont.h"
#include "CSGD_Direct3D.h"
#include "CSGD_DirectInput.h"
#include "Tile.h"
#include <fstream>
#include <exception>

#define SCROLLSPEED 150.0f
// default constructor may be used for
// blank maps...a cool animation sequence,
// or needing to render while not in a real battle, etc...
CBattleMap::CBattleMap(void)
{
	m_pAssets		= CAssets::GetInstance();
//	m_pBitmapFont   = CBitmapFont::GetInstance();
	m_pTM			= CSGD_TextureManager::GetInstance();
	m_pGame			= CGame::GetInstance();
	//m_pFMOD			= m_pGame->GetFMODSystem();
	m_pDI			= CSGD_DirectInput::GetInstance();
//	m_pHUD			= CHUD::GetInstance();

	m_pTilesL1 = NULL;
	m_pTilesL2 = NULL;
	m_pFreeTiles = NULL;
	m_pMoveableTiles = NULL;

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
	m_szFileName = "Blank.xml";
	m_szMapName  = "Blank";

	m_strCurrVersion = "TED-Version-1.0";
}

CBattleMap::CBattleMap(char* szFileName, char* szMapName, int nNumEnemies)
{
	m_pAssets		= CAssets::GetInstance();
	m_pTM			= CSGD_TextureManager::GetInstance();
	m_pGame			= CGame::GetInstance();
	m_pDI			= CSGD_DirectInput::GetInstance();
	//m_pDI->MouseSetPosX(0);
	//m_pDI->MouseSetPosY(0);
//	m_pBitmapFont   = CBitmapFont::GetInstance();
	//m_pFMOD			= m_pGame->GetFMODSystem();
//	m_pHUD			= CHUD::GetInstance();

	m_pTilesL1 = NULL;
	m_pTilesL2 = NULL;
	m_pFreeTiles = NULL;
	m_pMoveableTiles = NULL;

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

	m_nCursorImageID = m_pAssets->aBMcursorID;
	m_strCurrVersion = "TED-Version-1.0";
	LoadMapInfo();
	CreateEnemies();
}

CBattleMap::~CBattleMap(void)
{
	delete[] m_pTilesL1;
	delete[] m_pTilesL2;
	delete[] m_pFreeTiles;

	if (m_pAssets)
		m_pAssets = NULL;
// 	if (m_pBitmapFont)
// 		m_pBitmapFont = NULL;
	if (m_pTM)
		m_pTM = NULL;
// 	if (m_pFMOD)
// 		m_pFMOD = NULL;
// 	if (m_pDI)
// 		m_pDI = NULL;
	if (m_pGame)
		m_pGame = NULL;
// 	if (m_pHUD)
// 		m_pHUD = NULL;
}

void CBattleMap::Render()
{
	//m_pTM->Draw(m_nBGImageID, 0, 0);
	//m_pHUD->Draw();

	int xOffset = (int)m_fScrollX + m_nIsoCenterTopX - (m_nTileWidth >> 1);
	int yOffset = (int)m_fScrollY;
	
	// draw layer one & two
	MY_POINT mapPT;
	for (int x = 0; x < m_nNumCols; ++x)
	{
		for (int y = 0; y < m_nNumRows; ++y)
		{
			int tileID = y*m_nNumCols+x;
			if (m_pTilesL1[tileID].DestXID() != -1)
			{
				mapPT.x = x; mapPT.y = y;
				mapPT = IsoTilePlot(mapPT, xOffset, yOffset);
				m_pTM->Draw(m_pTilesL1[tileID].ImageID(), mapPT.x, mapPT.y, 1.0f, 1.0f, 
					m_pTilesL1[tileID].SourceRect(), 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(m_pTilesL1[tileID].Alpha(),255,255,255));
			}
			if (m_pTilesL2[tileID].DestXID() != -1)
			{
				mapPT.x = x; mapPT.y = y;
				mapPT = IsoTilePlot(mapPT, xOffset, yOffset);
				m_pTM->Draw(m_pTilesL2[tileID].ImageID(), mapPT.x, mapPT.y, 1.0f, 1.0f, 
					m_pTilesL2[tileID].SourceRect(), 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(m_pTilesL2[tileID].Alpha(),255,255,255));
			}
			if (m_nCurrSelectedTile == tileID)
			{
				m_pTM->Draw(m_nCursorImageID, mapPT.x, mapPT.y, 1.0f, 1.0f);
			}
		}
	}
	// draw the free placed layer

//		RECT srcRect = {0, 0, 64, 50};
	for (int i = 0; i < m_nFreeTileCount; ++i)
	{
		m_pTM->Draw(m_pFreeTiles[i].ImageID(), m_pFreeTiles[i].DestX()+(int)m_fScrollX, m_pFreeTiles[i].DestY()+yOffset,
			1.0f, 1.0f, m_pFreeTiles[i].SourceRect(), 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(m_pFreeTiles[i].Alpha(),255,255,255));

// 		m_pTM->Draw(m_pFreeTiles[i].ImageID(), m_pFreeTiles[i].DestX(), 
// 			m_pFreeTiles[i].DestY()+yOffset, 1.0f, 1.0f, &srcRect, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(m_pFreeTiles[i].Alpha(),255,255,255));
	}
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
	//Input(fElapsedTime);
}

bool CBattleMap::Input(float fElapsedTime, POINT mouse)
{
	mouse.x -= m_fScrollX;
	mouse.y -= m_fScrollY;
	if (m_pDI->KeyPressed(DIK_ESCAPE))
		return false;

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

	//int mouseX = m_pDI->MouseGetPosX() + 176;
	//int mouseY = m_pDI->MouseGetPosY() + 97;
	int xID, yID;
	xID = ((m_nTileWidth * mouse.y) + (m_nTileHeight * (mouse.x - m_nIsoCenterTopX))) / (m_nTileWidth * m_nTileHeight);
	yID = ((m_nTileWidth * mouse.y) - (m_nTileHeight * (mouse.x - m_nIsoCenterTopX))) / (m_nTileWidth * m_nTileHeight);
	if (xID < 0)
	{
		xID = 0;
	}
	else if (xID >= m_nNumCols)
	{
		xID = m_nNumCols-1;
	}
	else if (yID < 0)
	{
		yID = 0;
	}
	else if (yID >= m_nNumRows)
	{
		yID = m_nNumRows-1;
	}

	m_nCurrSelectedTile = yID * m_nNumCols + xID;
	if (m_pDI->KeyPressed(DIK_D))
		int i = 0;

	return true;
}

void CBattleMap::CreateEnemies()
{
	for (int i = 0; i < m_nNumEnemiesLeft; ++i)
	{
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
			m_nIsoCenterLeftY = (int)((((float)(m_nNumRows)) / 2.0f) * m_nTileHeight/* - (m_nTileHeight >>1)*/);
			m_nIsoCenterTopX = (int)((((float)(m_nNumCols)) / 2.0f) * m_nTileWidth/* - (m_nTileWidth >> 1)*/);
			m_nIsoCenterLeftY = 500;
			// allocate memory for layer 1, 2, and 3(free placed tiles)
			m_pTilesL1 = new CTile[m_nNumRows*m_nNumCols];
			m_pTilesL2 = new CTile[m_nNumRows*m_nNumCols];
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
			ifs.close();
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
			ifs.close();	
	}

	try
	{
		if (m_pTilesL2[0].DestXID() != -1 && m_pTilesL1[0].DestXID() != -1)
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
			ifs.close();
	}
}
