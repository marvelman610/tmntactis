//////////////////////////////////////////////////////////////////////////
//	Filename	:	BaseMenuState.h
//
//	Author		:	Ramon Johannessen (RJ)
//
//	Purpose		:	Base Class for all menu states
//////////////////////////////////////////////////////////////////////////
#ifndef CBASEMENUSTATE_H
#define CBASEMENUSTATE_H
#include "IGameState.h"
#include "fmod.hpp"
#include "CSGD_TextureManager.h"
#include "Assets.h"
#include "Game.h"
#include "BitmapFont.h"
#include "Direct3D.h"
#include "CSGD_DirectInput.h"

class CBitmapFont;
class CAsset;
class CSGD_TextureManager;
class Direct3D;
class CSGD_DirectInput;

class CBaseMenuState : IGameState
{
private:
	int	m_nBGImageID;					// bg image ID
	int	m_nImageWidth;				// bg image width	
	int m_nImageHeight;				// bg image height
	int m_nCursorImageID;			// the menu selection cursor image
	int	m_nCurrMenuSelection;		// where is the menu cursor at?
	int m_nCursorX;					// screen x for cursor
	int m_nCursorY;					// screen y for cursor
	FMOD::Sound m_sBGMusicID;		// bg music track

	CBitmapFont*		m_pBitmapFont;	// a pointer to the bitmap font singleton
	CAsset*				m_pAssets;
	CSGD_TextureManager m_pTM;
	FMOD::System*		m_pFMOD;
	Direct3D*			m_pD3D;
	CSGD_DirectInput*	m_pDI;


	//CBaseMenuState* m_pCurrentState;// a pointer to the current menu state


	CBaseMenuState(const CBaseMenuState&);
	CBaseMenuState& operator= (const CBaseMenuState&);
public:
	CBaseMenuState();
	~CBaseMenuState();

	//////////////////////////////////////////////////////////////////////////
	//	Function	:	Update
	//
	//	Purpose		:	Any update code goes here, pure virtual, virtual, to be overwritten
	//////////////////////////////////////////////////////////////////////////
	virtual void Update(float fElapsedTime);
	//////////////////////////////////////////////////////////////////////////
	//	Function	:	Render
	//
	//	Purpose		:	draw everything to the screen, virtual, to be overwritten
	//////////////////////////////////////////////////////////////////////////
	virtual void Render();
	//////////////////////////////////////////////////////////////////////////
	//	Function	:	Input
	//
	//	Purpose		:	Handle any user input for all menu states, mouse or keyboard, virtual, to be overwritten
	//
	//	Return		:	true/false, false if we are exiting the game
	//////////////////////////////////////////////////////////////////////////
	virtual bool Input();
	//////////////////////////////////////////////////////////////////////////
	//	Function	:	Enter
	//
	//	Purpose		:	When the state is first entered, execute this code, virtual, to be overwritten
	//////////////////////////////////////////////////////////////////////////
	virtual void Enter();
	//////////////////////////////////////////////////////////////////////////
	//	Function	:	Exit
	//
	//	Purpose		:	When the state exits, execute this code, virtual, to be overwritten
	//////////////////////////////////////////////////////////////////////////
	virtual void Exit();

};

#endif