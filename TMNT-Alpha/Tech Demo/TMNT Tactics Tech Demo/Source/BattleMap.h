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
#include "Base.h"
#include <vector>
using std::vector;
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
class CHUD;
class CPlayer;

 // enum for tile flags
 enum {FLAG_NONE, FLAG_COLLISION, FLAG_OBJECT_EDGE, };
#include "Base.h"

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
	// characters, by default, render behind the OBJECTS layer (CHARACTER_BEHIND)
	struct DEPTH
	{
		float GROUND;
		float GROUNDL2;
		float SELECTION;
		float CHARACTER_BEHIND;
		float CHARACTER_AHEAD;
		float OBJECTS;
		float ARROW;
		DEPTH()
		{
			GROUND = 1.0f;
			GROUNDL2 = 0.9f;
			SELECTION = 0.8f;
			CHARACTER_BEHIND = 0.7f;
			OBJECTS = 0.6f;
			CHARACTER_AHEAD = 0.5f;
			ARROW = 0.49f;
		}
	};
	POINT ms;					// the mouse location, updated each Update() call
	DEPTH depth;				// struct that holds all the z-depth draw locations
	string m_strCurrVersion;

	// Map variables
	int m_nMapWidth;
	int m_nMapHeight;
	int m_nNumCols;
	int m_nNumRows;
	int m_nTileWidth;
	int m_nTileHeight;
	int m_nTotalNumTiles;
	int m_nCurrSelectedTile;

	int m_nScrenWidth;
	int m_nScreenHeight;
	int m_nIsoCenterLeftY;
	int m_nIsoCenterTopX; 
	int m_nFreeTileCount;
	int m_nOffsetX;
	int m_nOffsetY;
	int m_nFreeTileOSx;
	int m_nfreeTileOSy;
	float m_fScrollX;
	float m_fScrollY;

	char* m_szMapName;
	char* m_szFileName;

	CTile* m_pTilesL1;			// all tiles on the map, used for drawing first layer
	CTile* m_pTilesL2;			// all tiles on the map, used for drawing first layer
	//CTile* m_pMoveableTiles;	// the open tiles, used for collision checking
	CFreeTile* m_pFreeTiles;
	sTileset m_pTilesets[4];	// keep track of the tileset info (struct)

	// Character variables
	CPlayer*	m_pPlayer;	// the player singleton pointer
	int m_nNumEnemiesLeft;
	int m_nNumCharacters;
	int m_nHoverCharacter;
	int m_nCurrCharacter;
	int m_nCurrCharacterTile;
	vector<CBase> m_vCharacters; // all characters that are on the current battle map

	// temp particles
	CParticleSystem* m_pParticleSys;
	//int turtleX;
	//int turtleY;

	//////////////////////////////////////////////////////////////////////////
	// Pointers to singletons
	CAssets*			m_pAssets;
	CSGD_TextureManager* m_pTM;
	CSGD_Direct3D*		m_pD3D;
	CSGD_DirectInput*	m_pDI;
	CGame*				m_pGame;
	CCamera*			m_pCamera;
	CBitmapFont*		m_pBitmapFont;
	CHUD*				m_pHUD;
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

	//////////////////////////////////////////////////////////////////////////
	//	Function	:	"IsMousePosValid"
	//
	//	Purpose		:	Determines if the user has clicked on a valid object
	//					to select
	//
	//	Return		:	The current index into the character array, if not a 
	//					valid click, returns -1
	//////////////////////////////////////////////////////////////////////////
	int IsMousePosValid(POINT mousePt);

	//////////////////////////////////////////////////////////////////////////
	//	Function	:	"CalculateRanges"
	//
	//	Purpose		:	Determines the attack and move range of the current character
	//////////////////////////////////////////////////////////////////////////
	void CalculateRanges();

	//////////////////////////////////////////////////////////////////////////
	//	Function	:	"DisplayRanges"
	//
	//	Purpose		:	Shows the attack and move ranges visually on the map
	//////////////////////////////////////////////////////////////////////////
	void DisplayRanges();

	//////////////////////////////////////////////////////////////////////////
	//	Function	:	"DrawHover"
	//
	//	Purpose		:	Draws the rectangle showing which character is currently
	//					being hovered over
	//////////////////////////////////////////////////////////////////////////
	void DrawHover();

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
	//	Purpose		:	Handle any user input for the battle, mouse and/or keyboard 
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
	//	Return		:	returns the z depth(float) for the character to be drawn at
	//////////////////////////////////////////////////////////////////////////
	float GetZdepthDraw(int x, int y, int currTileID = -1);

	//////////////////////////////////////////////////////////////////////////
	//	Accessors
	//////////////////////////////////////////////////////////////////////////
	int GetCurrSelectedTile() const {return m_nCurrSelectedTile;}
	int GetNumCols()	const		{return m_nNumCols;}
	int GetTileWidth()  const		{return m_nTileWidth;}
	int GetOffsetX()	const		{return m_nOffsetX;}
	int GetOffsetY()	const		{return m_nOffsetY;}
	int GetFreeTileXos()const		{return m_nFreeTileOSx;}
	int GetFreeTileYos()const		{return m_nfreeTileOSy;}

	//////////////////////////////////////////////////////////////////////////
	//	Mutators
	//////////////////////////////////////////////////////////////////////////
	void SetCurrSelectedTile(const int selectedTile){m_nCurrSelectedTile = selectedTile;}
	void SetOffsetX(const int offsetX)				{m_nOffsetX = offsetX;}
	void SetOffsetY(const int offsetY)				{m_nOffsetY = offsetY;}
	void SetFTosX(const int osX)					{m_nFreeTileOSx = osX;}
	void SetFTosY(const int osY)					{m_nfreeTileOSy = osY;}
	void SetTurtlePos();

	//////////////////////////////////////////////////////////////////////////
	//  Function	:	DrawDebugInfo
	//
	//	Purpose		:	Draws any useful information for debugging to the screen
	//////////////////////////////////////////////////////////////////////////
	void DrawDebugInfo();
};

#endif