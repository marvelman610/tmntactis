////////////////////////////////////////////////////////
//   File Name	:	"CGame.h"
//
//   Author		:	Ramon Johannessen (RJ)
//
//   Purpose	:	To encapsulate all game related code
////////////////////////////////////////////////////////
#pragma once

//#ifndef _CGAME_H_
//#define _CGAME_H_
//
//// define game class here
//
//#endif
#include <windows.h>
#include <ctime>

//class CBitmapFont;
class CSGD_DirectInput;
class CBattleMap;
class CSGD_Direct3D;
class CSGD_TextureManager;
class CAssets;

class CGame
{
private:
	// Keeps track of the current game state
	//IGameState*  m_pCurrentState;

	// pointers to wrapper singletons
	CSGD_Direct3D*			m_pD3D;
	CSGD_TextureManager*	m_pTM;
	CSGD_DirectInput*		m_pDI;

	//CSGD_EventSystem*		m_pES;
	//CSGD_MessageSystem*	m_pMS;
	//CSGD_ObjectManager*	m_pOM2;
	//CBitmapFont*			m_pBitmapFont;

	CBattleMap*	m_pBattleMap;
	CAssets*	m_pAssets;

	// pause/running variables
	float m_fElapsedTime;
	DWORD m_dwPrevTime;

	bool m_bIsRunning;
// 	bool m_bIsPaused;
// 
// 	// sound-related variables
// 	int m_nSFXVolume;
// 	int m_nMusicVolume;
// 	int m_nPanning;
// 	// saved sound info
// 	int	m_nSoundInfo[3];
// 
// 	// keep hold of score, in case it is a high score
// 	int		m_nLastScore;
// 
// 	// for size of image
// 	//RECT m_rectBackGroundSize;
// 
// 	//Asset IDS
// 	int			m_nImageID;
// 	int			m_nSoundID;

	// screen dimensions
	int m_nScreenWidth;
	int	m_nScreenHeight;

	// Make this a proper singleton
	CGame();
	~CGame();
	CGame(const CGame&);
	CGame& operator=(const CGame&);

public:
	//////////////////////////////////////////////////////////////////////////
	//	Function	:	GetInstance
	//
	//	Purpose		:	return the singleton instance of this class
	//////////////////////////////////////////////////////////////////////////
	static CGame* GetInstance();
	//////////////////////////////////////////////////////////////////////////
	//	Function	:	Initialize
	//
	//	Purpose		:	To set up the game singletons used throughout the game
	//////////////////////////////////////////////////////////////////////////
	void Initialize(HWND hWnd, HINSTANCE hInstance, int nScreenWidth, int nScreenHeight, bool bIsWindowed);
	//////////////////////////////////////////////////////////////////////////
	//	Function	:	Main
	//
	//	Purpose		:	Where the game begins from and runs throughout its life
	//////////////////////////////////////////////////////////////////////////
	bool Main(POINT mouse);
	//////////////////////////////////////////////////////////////////////////
	//	Function	:	Shutdown
	//
	//	Purpose		:	To clean up memory before closing program
	//////////////////////////////////////////////////////////////////////////
	void Shutdown();
	//////////////////////////////////////////////////////////////////////////
	//	Function	:	ChangeState
	//
	//	Purpose		:	Handles changing from one game state to another, e.g.
	//					MainMenuState to GamePlayState
	//////////////////////////////////////////////////////////////////////////
	//void ChangeState(IGameState* pNewState);
	//	Function	:	LoadSettings
	//
	//	Purpose		:	Load in any saved sound info
	//////////////////////////////////////////////////////////////////////////
//	void LoadSettings(const char* filename);
	//////////////////////////////////////////////////////////////////////////
	// Accessors
	//////////////////////////////////////////////////////////////////////////
	//RECT GetBackGroundSize(void){ return m_rectBackGroundSize; }
// 	int  GetScreenWidth ()		{return m_nScreenWidth;}
// 	int  GetScreenHeight ()		{return m_nScreenHeight;}
// 	bool GetIsPaused()			{return m_bIsPaused;}
	bool GetIsRunning()			{return m_bIsRunning;}
// 	int  GetSFXVolume()			{return m_nSFXVolume;}
// 	int  GetMusicVolume()		{return m_nMusicVolume;}
// 	int  GetPanning()			{return m_nPanning;}

	//////////////////////////////////////////////////////////////////////////
	// Mutators
	//////////////////////////////////////////////////////////////////////////
//	void SetIsPaused	()					{m_bIsPaused = !m_bIsPaused;}
	void SetIsRunning	(bool _bIsRunning)		{m_bIsRunning = _bIsRunning;}
// 	void SetLastScore	(int _nLastScore)		{m_nLastScore = _nLastScore;}
// 	void SetSFXVolume	(int _nSFXVolume);
// 	void SetMusicVolume	(int _nMusicVolume);
// 	void SetPanning		(int _nPanning);


	void Render();
};