//////////////////////////////////////////////////////////////////////////
//	Filename	:	CBattleMap.h
//
//	Author		:	Ramon Johannessen (RJ)
//
//	Purpose		:	To execute the individual battles that take place,
//					drawing the isometric tile map, updating, getting input, etc...
//////////////////////////////////////////////////////////////////////////
#ifndef BATTLEMAP_H
#define BATTLEMAP_H

#include <windows.h>
#include <iostream>
using namespace std;

 class CSGD_TextureManager;
 class CSGD_DirectInput;
 class CSGD_Direct3D;
 class CGame;
 class CAssets;
//class CBitmapFont;
 class CTile;
 class CFreeTile;
//class CHUD;

 struct MY_POINT 
 {
	 int x;
	 int y;
 };
class CBattleMap
{
	struct sTileset 
	{
		string name;
		int id;
	};
	string m_strCurrVersion;

	// IDs
	int m_nBGImageID;
	int m_nDotImageID;
	int m_nCursorImageID;

	// Map variables
	int m_nMapWidth;
	int m_nMapHeight;
	int m_nNumCols;
	int m_nNumRows;
	int m_nTileWidth;
	int m_nTileHeight;
	int m_nTotalNumTiles;
	int m_nCurrSelectedTile;

	int m_nIsoCenterLeftY;
	int m_nIsoCenterTopX; 
	int m_nFreeTileCount;
	float m_fScrollX;
	float m_fScrollY;

	char* m_szMapName;
	char* m_szFileName;

	CTile* m_pTilesL1;			// all tiles on the map, used for drawing first layer
	CTile* m_pTilesL2;			// all tiles on the map, used for drawing first layer
	CTile* m_pMoveableTiles;	// the open tiles, used for collision checking
	CFreeTile* m_pFreeTiles;
	sTileset m_pTilesets[4];

	// battle variables
	int m_nNumEnemiesLeft;

	//////////////////////////////////////////////////////////////////////////
	// Pointers to singletons
	//CBitmapFont*		m_pBitmapFont;
	CAssets*			m_pAssets;
	CSGD_TextureManager* m_pTM;
	//FMOD::System*		m_pFMODsys;
	CSGD_Direct3D*		m_pD3D;
	CSGD_DirectInput*	m_pDI;
	CGame*				m_pGame;
	//CHUD*				m_pHUD;
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	//	Function	:	CreateEnemies
	//
	//	Purpose		:	Make and place the m_nNumEnemiesLeft amount of enemies
	//					to the battle map
	//////////////////////////////////////////////////////////////////////////
	void CreateEnemies();
	//////////////////////////////////////////////////////////////////////////
	//	Function	:	LoadMapInfo
	//
	//	Purpose		:	Load in the new map, set up all of the members
	//////////////////////////////////////////////////////////////////////////
	void LoadMapInfo();
	//////////////////////////////////////////////////////////////////////////
	//	Function	:	IsoTilePlot
	//
	//	Purpose		:	Determines at which pixel on the screen the given map
	//					ID (xID, yID = pt.x, pt.y) should be drawn
	//
	//	Return		:	The pixel point to draw at
	//////////////////////////////////////////////////////////////////////////
	MY_POINT IsoTilePlot(MY_POINT pt, int xOffset, int yOffset);

public:
	CBattleMap(void);
	CBattleMap(char* szFileName, char* szMapName = "Test", int nNumEnemies = 0);
	~CBattleMap(void);

	//////////////////////////////////////////////////////////////////////////
	//	Function	:	Update
	//
	//	Purpose		:	Update the Battle Map
	//////////////////////////////////////////////////////////////////////////
	void Update(float fElapsedTime);
	//////////////////////////////////////////////////////////////////////////
	//	Function	:	Render
	//
	//	Purpose		:	Render the Map and all objects on it
	//////////////////////////////////////////////////////////////////////////
	void Render();
	//////////////////////////////////////////////////////////////////////////
	//	Function	:	Input
	//
	//	Purpose		:	Handle any user input for the battle, mouse or keyboard 
	//
	//	Return		:	true/false, false if we are exiting the game
	//////////////////////////////////////////////////////////////////////////
	bool Input(float, POINT);

	//////////////////////////////////////////////////////////////////////////
	int GetCurrSelectedTile() const {return m_nCurrSelectedTile;}

};

#endif