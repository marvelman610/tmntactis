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

class CSGD_TextureManager;
class CBitmapFont;
class CAssets;
class CSGD_DirectInput;

class CWorldMap
{
	int m_nMapImageID;
	int m_nWorldPos;	// which location are they at?
	float m_fPosX;
	float m_fPosY;
	POINT m_ptMouse;

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