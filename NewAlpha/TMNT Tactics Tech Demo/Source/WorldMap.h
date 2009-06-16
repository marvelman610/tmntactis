//////////////////////////////////////////////////////////////////////////
//	Filename	:	CBattleMap.h
//
//	Author		:	Ramon Johannessen (RJ)
//
//	Purpose		:	To execute the individual battles that take place,
//					drawing the isometric tile map, updating, getting input, etc...
//////////////////////////////////////////////////////////////////////////
#ifndef WORLDMAP_H
#define WORLDMAP_H

#include <Windows.h>
#include "CSGD_Direct3D.h"
#include <string>
using std::string;

class CSGD_TextureManager;
class CBitmapFont;
class CAssets;
class CSGD_DirectInput;
class CBox;
class CPlayer;

enum {LOC_SIMUSA, LOC_SINARO, LOC_YAMATO, LOC_IWAMI, NUM_LOCATIONS, };

class CWorldMap
{
	int m_nMapImageID;
	int m_nWorldPos;	// which location are they at?
	int m_nMapOSx;		// scrolling offsets
	int m_nMapOSy;
	int m_nScreenWidth;
	int m_nScreenHeight;
	int m_nMapWidth;
	int m_nMapHeight;
	float m_fPosX;		// positions of the player's icon
	float m_fPosY;
	POINT m_ptMouse;

	CPlayer* m_pPlayer;

	// boxes
	CBox* m_bxMenu;
	CBox* m_bxChooseTurtle;
	CBox* m_bxTrainSkills;
	CBox* m_bxHelp;

	struct MAP_POINT
	{
		int x;
		int y;
		MAP_POINT(){}
		MAP_POINT(int X, int Y)
		{ x = X; y = Y;}
	};

	struct sLocation 
	{
		bool bSelected;
		bool bHovering;
		int imageID;
		string name;
		MAP_POINT mapXY;
		DWORD color;

		sLocation()
		{ color = D3DCOLOR_XRGB(255,255,255); bSelected = false; bHovering = false;}
		bool MouseInRect(POINT mouse, int osx, int osy)
		{
			if (mouse.x >= mapXY.x-osx && mouse.x <= mapXY.x+32-osx &&
				mouse.y >= mapXY.y-osy && mouse.y <= mapXY.y+32-osy)
			{
				return true;
			}
			else
				return false;
		}
	};
	sLocation m_Locations[4];
	sLocation* m_pCurrLoc;

	CAssets*				m_pAssets;
	CBitmapFont*			m_pBitmapFont;
	CSGD_TextureManager*	m_pTM;
	CSGD_DirectInput*		m_pDI;
	
	CWorldMap();
	~CWorldMap();
	CWorldMap(const CWorldMap&);
	CWorldMap& operator= (const CWorldMap&);

public:

	///////////////////////////////////////////////////////////////////
	//	Function: GetInstance
	//
	//	Purpose: Init the instance and return a pointer to the instance
	///////////////////////////////////////////////////////////////////
	static CWorldMap* GetInstance();

	void Enter();
	void Exit();

	void Render();
	void Update(float fElapsedTime);

	bool Input(float, POINT);
};


#endif