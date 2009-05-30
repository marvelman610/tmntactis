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
 class CBitmapFont;
 class CTile;
 class CFreeTile;
 class CAnimation;
 class CParticleSystem;
 class CCamera;
//class CHUD;

 // characters, by default, render behind the OBJECTS layer
 enum { 
		ZSORT_BACKGROUND = 0,
		ZSORT_GROUND,
		ZSORT_GROUND_LAYER2, 
		ZSORT_CHARACTERS_BEHIND,	 
		ZSORT_OBJECTS,
		ZSORT_CHARACTERS_AHEAD,

	  };

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
	struct DEPTH
	{
		float GROUND;
		float GROUNDL2;
		float SELECTION;
		float CHARACTER_BEHIND;
		float CHARACTER_AHEAD;
		float OBJECTS;
		DEPTH()
		{
			GROUND = 1.0f;
			GROUNDL2 = 0.9f;
			SELECTION = 0.8f;
			CHARACTER_BEHIND = 0.7f;
			OBJECTS = 0.6f;
			CHARACTER_AHEAD = 0.5;
		}
	};
	POINT ms;
	DEPTH depth;
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
	int m_nOffsetX;
	int m_nOffsetY;
	float m_fScrollX;
	float m_fScrollY;

	char* m_szMapName;
	char* m_szFileName;

	CTile* m_pTilesL1;			// all tiles on the map, used for drawing first layer
	CTile* m_pTilesL2;			// all tiles on the map, used for drawing first layer
	//CTile* m_pMoveableTiles;	// the open tiles, used for collision checking
	CFreeTile* m_pFreeTiles;
	sTileset m_pTilesets[4];	// keep track of the tileset info (struct)

	// battle variables
	int m_nNumEnemiesLeft;
	// temp animations
	CAnimation*		m_pAnimation;
	// temp particles
	CParticleSystem* m_pParticleSys;
	int turtleX;
	int turtleY;

	//////////////////////////////////////////////////////////////////////////
	// Pointers to singletons
	CAssets*			m_pAssets;
	CSGD_TextureManager* m_pTM;
	CSGD_Direct3D*		m_pD3D;
	CSGD_DirectInput*	m_pDI;
	CGame*				m_pGame;
	CCamera*			m_pCamera;
	CBitmapFont*		m_pBitmapFont;
	//CHUD*				m_pHUD;
	//FMOD::System*		m_pFMODsys;
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
	//  Function	:	GetZdepthDraw
	//
	//	Purpose		:	To determine if a character object should be rendered
	//					in front of a map object (free placed object)
	//
	//	Return		:	returns the z depth(enum) for the character to be drawn at
	//////////////////////////////////////////////////////////////////////////
	float GetZdepthDraw(int x, int y);

	//////////////////////////////////////////////////////////////////////////
	//	Accessors
	//////////////////////////////////////////////////////////////////////////
	int GetCurrSelectedTile() const {return m_nCurrSelectedTile;}
	int GetNumCols()	const		{return m_nNumCols;}
	int GetTileWidth()  const		{return m_nTileWidth;}
	int GetOffsetX()	const		{return m_nOffsetX;}
	int GetOffsetY()	const		{return m_nOffsetY;}

	//////////////////////////////////////////////////////////////////////////
	//	Mutators
	//////////////////////////////////////////////////////////////////////////
	void SetCurrSelectedTile(const int selectedTile){m_nCurrSelectedTile = selectedTile;}
	void SetOffsetX(const int offsetX)				{m_nOffsetX = offsetX;}
	void SetOffsetY(const int offsetY)				{m_nOffsetY = offsetY;}


	void DrawDebugInfo();

};

#endif