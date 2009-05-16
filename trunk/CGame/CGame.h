#pragma once

#include "IGameState.h"
#include "MessageSystem.h"
#include "TextureManager.h"
#include "Direct3D.h"
#include "DirectInput.h"
#include "ObjectFactory.h"
#include "ParticleSystem.h"

#include "FMod.hpp"
#pragma comment(lib, "FModEx.dll");


class CGame
{
private:
	//pointers to other classes
	IGameState* m_pCurrentState;
	Direct3D* m_pDirect3D;
	TextureManager* m_pTextureManager;
	FMOD::System* m_pFMODSystem;
	FMOD::Channel m_FMChannel1;
	FMOD::Channel m_FMChannel2;
	DirectInput* m_pDirectInput;
	MessageSystem* m_pMessageSystem;
	ObjectFactory* m_pObjectFactory;
	ParticleSystem* m_pParticleSystem;

	// variables
	bool m_bIsRunning;
	int m_nSFXVolume;
	int m_nMusicVolume;
	float m_fElapsedTime;
	PLAYERINFO m_PlayerInfo;

	//Constructor
	CGame(void);
	//Copy Constructor
	CGame(const CGame&);
	//Assignment Operator
	CGame& operator=(const CGame&);
	//Destructor
	~CGame(void);

public:
	///////////////////////////////////////////////
	//	Function:	ChangeState
	//
	//	Purpose:	Change the State of the Game
	///////////////////////////////////////////////
	void ChangeState(IGameState* pGameState);
	/////////////////////////////////////////////////////////////////
	//	Function:	Main
	//
	//	Purpose:	To run while the game runs from start to finish
	////////////////////////////////////////////////////////////////
	bool Main(void);
	////////////////////////////////////////////////////////////////
	//	Function:	Shutdown
	//
	//	Purpose:	Clean up all memory 
	////////////////////////////////////////////////////////////////
	void Shutdown(void);
	////////////////////////////////////////////////////////////
	//	Function:	Initialize
	//
	//	Purpose:	Initialize the game
	////////////////////////////////////////////////////////////
	void Initialize(HWND hWnd, HINSTANCE hInstance, int nScreenWidth, int nScreenHeight, bool bIsWindowed);
	///////////////////////////////////////////////////////
	//	Function:	GetInstance
	//
	//	Purpose:	Gets the instance of CGame
	///////////////////////////////////////////////////////
	static CGame* GetInstance(void);
	///////////////////////////////////////////////////////
	//	Function:	LoadSettings
	//
	//	Purpose:	To load the game settings
	///////////////////////////////////////////////////////
	void LoadSettings(void);
	////////////////////////////////////////////////////////
	//	Function: GetFMODSystem
	//
	//	Purpose: Return the FMODSystem pointer
	////////////////////////////////////////////////////////
	FMOD::System* GetFMODSystem(void);
};