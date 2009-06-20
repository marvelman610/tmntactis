#ifndef GAME_H
#define GAME_H

#include "windows.h"
#include "IGameState.h"

//class ParticleSystem;
class CBitmapFont;
class CSGD_DirectInput;
class CBattleMap;
class CSGD_Direct3D;
class CSGD_TextureManager;
class CSGD_FModManager;
class CAssets;
class CPlayer;
class MessageSystem;
class CBaseMessage;
class Factory;
#include <string>
using std::string;

class CGame
{
private:
	//pointers to other classes
	IGameState*			m_pCurrentState;
	CSGD_Direct3D*		m_pD3D;
	CSGD_TextureManager* m_pTM;
	CSGD_DirectInput*   m_pDI;
	CAssets*			m_pAssets;
	CPlayer*			m_pPlayer;
	MessageSystem*		m_pMS;
	CSGD_FModManager*	m_pFMOD;
	CBitmapFont*		m_pBitMapFont;
	//ParticleSystem* m_pParticleSystem;
	//MessageSystem* m_pMessageSystem;
	//Factory* m_pObjectFactory;
	string m_sCurrProfName;

	// variables
	bool	m_bIsRunning;
	float	m_fSFXVolume;
	float	m_fMusicVolume;
	float	m_fElapsedTime;
	int		m_nScreenWidth;
	int		m_nScreenHeight;
	//PLAYERINFO m_PlayerInfo;

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
	bool Main(POINT mouse);
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
	//FMOD::System* GetFMODSystem(void);

	static friend void MessageProc(CBaseMessage* pMsg);

	//////////////////////////////////////////////////////////////////////////
	// Accessors
	//////////////////////////////////////////////////////////////////////////
	//RECT GetBackGroundSize(void){ return m_rectBackGroundSize; }
 	int  GetScreenWidth ()		{return m_nScreenWidth;}
 	int  GetScreenHeight ()		{return m_nScreenHeight;}
	bool GetIsRunning()			{return m_bIsRunning;}
	float  GetSFXVolume()		{return m_fSFXVolume;}
	float  GetMusicVolume()		{return m_fMusicVolume;}
	string GetProfName()		{return m_sCurrProfName;}
	// 	bool GetIsPaused()			{return m_bIsPaused;}
	// 	int  GetPanning()			{return m_nPanning;}

	//////////////////////////////////////////////////////////////////////////
	// Mutators
	//////////////////////////////////////////////////////////////////////////
	void SetSFXVolume	(float _nSFXVolume);
	void SetMusicVolume	(float _nMusicVolume);
	void SetIsRunning	(bool _bIsRunning)		{m_bIsRunning = _bIsRunning;}
	void SetProfName	(string profName)		{m_sCurrProfName = profName;}
	//	void SetIsPaused	()					{m_bIsPaused = !m_bIsPaused;}
	// 	void SetLastScore	(int _nLastScore)		{m_nLastScore = _nLastScore;}
	// 	void SetPanning		(int _nPanning);
};

#endif