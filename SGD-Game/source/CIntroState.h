////////////////////////////////////////////////////////////////////////////
//	File Name	:	"CIntroState.h"
//
//	Author		:	Matt DiMatteo (MD)
//
//	Purpose		:	Provides an intro to the game
////////////////////////////////////////////////////////////////////////////
#pragma once
#include "IGameState.h"
//#include "SGD Wrappers/CSGD_Direct3D.h"
#include "SGD Wrappers/CSGD_TextureManager.h"
//#include "SGD Wrappers/CSGD_DirectSound.h"
#include "SGD Wrappers/CSGD_WaveManager.h"
#include "SGD Wrappers/CSGD_DirectInput.h"
#include"CBitmapFont.h"

class CIntroState : public IGameState
{
private:
	//int m_nImageID;
	//int m_nCurrentMenuSelection;
	//int m_nSoundLoopID;
	int m_nSoundEffectID;
	int themeID;
	int storyID;
	int startID;

	int a;
	float time;

	int id[95];

	int j;
	int z;

	float scale;
	float offset;


	float timer;
	float m_fElapsedTime;
	DWORD m_dwPreviousTime;

	bool intro;
	bool space;
	bool render;
	bool start;
	bool draw;

	char* buffer;
	char line1[50];
	char line2[50];
	char line3[50];
	char line4[50];
	char line5[50];
	char line6[50];


	

	//CSGD_Direct3D*	m_pD3D;
	CSGD_TextureManager*  m_pTM;
	CSGD_DirectInput*  m_pDI;
	CSGD_WaveManager*  m_pWM;
	//CSGD_DirectSound*  m_pDS;

	CBitmapFont pfont;
	CBitmapFont font;
	
	///////////////////////////////////////////////////////////////////
	//	Function:	"CIntroState(Constructor)"
	///////////////////////////////////////////////////////////////////
	CIntroState(void) {}
	CIntroState(const CIntroState&);
	CIntroState& operator=(const CIntroState&);

	///////////////////////////////////////////////////////////////////
	//	Function:	"~COptionsState(Destructor)"
	///////////////////////////////////////////////////////////////////
	~CIntroState(void) {}

public:
	///////////////////////////////////////////////////////////////////
	// Function: "GetInstance"
	//
	//  Purpose: Gets instance to the only instance of the class.
	///////////////////////////////////////////////////////////////////
	static CIntroState* GetInstance();

	///////////////////////////////////////////////////////////////////
	// Function: "Enter"
	//
	//  Purpose: Initializes variables and pointers upon entering the state.
	///////////////////////////////////////////////////////////////////
	void Enter();
		
	///////////////////////////////////////////////////////////////////
	// Function: "Input"
	//
	//  Purpose: Receives input from player.
	///////////////////////////////////////////////////////////////////
	bool Input();

	///////////////////////////////////////////////////////////////////
	// Function: "Update"
	//
	// Purpose: Updates game objects based on time.
	////////////////////////////////////////////////////////////////////
	void Update();

	////////////////////////////////////////////////////////////////////
	// Function: “Render”
	//
	// Purpose: Draws game objects to the screen.
	////////////////////////////////////////////////////////////////////
	void Render();

	////////////////////////////////////////////////////////////////////
	// Function: “Exit”
	//
	// Purpose: Unloads any assets aquired since entering. It does this 
	//			and then exits the state .
	////////////////////////////////////////////////////////////////////
	void Exit();

};
