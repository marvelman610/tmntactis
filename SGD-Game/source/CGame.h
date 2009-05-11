////////////////////////////////////////////////////////
//   File Name	:	"CGame.h"
//
//   Author		:	Matthew Di Matteo (MD)
//
//   Purpose	:	To encapsulate all game related code.
////////////////////////////////////////////////////////
#pragma once

#include "SGD Wrappers/CSGD_Direct3D.h"
#include "SGD Wrappers/CSGD_TextureManager.h"
#include "SGD Wrappers/CSGD_DirectSound.h"
#include "SGD Wrappers/CSGD_WaveManager.h"
#include "SGD Wrappers/CSGD_DirectInput.h"

#include "IGameState.h"
#include "MainMenuState.h"
#include "OptionsState.h"
#include "CGamePlayState.h"
#include "CHighScoreState.h"
#include "CIntroState.h"

#include "CSGD_MessageSystem.h"
#include "CSGD_ObjectFactory.h"
#include "CBase.h"
#include <string>
using std::string;

class CGame
{
private:
	IGameState*  m_pCurrentState;

	CSGD_Direct3D*			m_pD3D;
	CSGD_TextureManager*	m_pTM;
	CSGD_DirectSound*		m_pDS;
	CSGD_WaveManager*		m_pWM;
	CSGD_DirectInput*		m_pDI;

	CSGD_MessageSystem*     m_pMS;

	CSGD_ObjectFactory<string, CBase>*     m_pOF;

	bool m_bIsRunning;

	int SFXvolume;
	int BGMvolume;

	int panning;

	//Asset IDS
	int			m_nImageID;
	int			m_nSoundID;

	///////////////////////////////////////////////////////////////////
	//	Function:	"CGame(Constructor)"
	///////////////////////////////////////////////////////////////////
	CGame();
	CGame(const CGame&);
	CGame& operator=(const CGame&);

	///////////////////////////////////////////////////////////////////
	//	Function:	"~CGame(Destructor)"
	///////////////////////////////////////////////////////////////////
	~CGame() {}

public:
	///////////////////////////////////////////////////////////////////
	//  Function: Accessors
	//
	//  Purpose : Returns the specified type.
	///////////////////////////////////////////////////////////////////
	inline bool GetIsRunning() {return m_bIsRunning;}
	inline int GetSFXVolume()	{ return SFXvolume; }
	inline int GetBGMVolume()	{ return BGMvolume; }
	inline int GetPanning()	{ return panning; }



	///////////////////////////////////////////////////////////////////
	// Function: Modifiers
	//
	//  Purpose: Modifies the specified type.
	///////////////////////////////////////////////////////////////////
	inline void SetIsRunning(bool _bIsRunning) {m_bIsRunning = _bIsRunning;}
	inline void SetSFXVolume(int vol)	{ SFXvolume = vol; }
	inline void SetBGMVolume(int vol)	{ BGMvolume = vol; }
	inline void SetPanning(int pan)	{ panning = pan; }
	
	///////////////////////////////////////////////////////////////////
	// Function: "GetInstance"
	//
	//  Purpose: Gets instance to the only instance of the class.
	///////////////////////////////////////////////////////////////////
	static CGame* GetInstance();
		
	///////////////////////////////////////////////////////////////////
	// Function: "Initialization"
	//
	//  Purpose: Initializes the game variables and sets the pointers 
	//			of the wrapper classes.
	///////////////////////////////////////////////////////////////////
	void Initialize(HWND hWnd, HINSTANCE hInstance, int nScreenWidth, int nScreenHeight, bool bIsWindowed);
	
	///////////////////////////////////////////////////////////////////
	// Function: "Main"
	//
	//  Purpose: Manages the Input, Update, and Rendering of the game and
	//			keeps it running.
	///////////////////////////////////////////////////////////////////
	bool Main();

	///////////////////////////////////////////////////////////////////
	// Function: "Shutdown"
	//
	//  Purpose: Shutsdown all the wrappers, factory, and systems to
	//			avoid memory leaks.
	///////////////////////////////////////////////////////////////////
	void Shutdown();
		
	///////////////////////////////////////////////////////////////////
	// Function: "ChangeState"
	//
	//  Purpose: Changes the state of the game to the specified state.
	///////////////////////////////////////////////////////////////////
	void ChangeState(IGameState* pNewState);
		
	///////////////////////////////////////////////////////////////////
	// Function: "MessageProc"
	//
	//  Purpose: Processes messages using the message system.
	///////////////////////////////////////////////////////////////////
	static friend void MessageProc(CBaseMessage* pMsg);
};