//////////////////////////////////////////////////////////////////////////
//	Filename	:	GamePlayState.h
//
//	Author		:	Ramon Johannessen (RJ)
//
//	Purpose		:	To define the CGamePlayState class. This state handles
//					all gameplay code.
//////////////////////////////////////////////////////////////////////////
#pragma once

#include "IGameState.h"

enum {MAP_WORLD, MAP_BATTLE, MAP_NULL };

class CBaseMenuState;
class CBattleMap;
class CBaseMenuState;
class CAnimation;
class CPlayer;
class CHUD;
//class WorldMap;

class CGamePlayState : public IGameState
{
private:
	CBaseMenuState* m_pCurrentMenuState;
	CBattleMap*		m_pBattleMap;
	CPlayer*		m_pPlayer;
	CHUD* m_pHUD;
	//WorldMap* m_pWorldMap;

	bool m_bIsPaused;

	int m_nCurrentMap;

	// Constructor
	CGamePlayState(void);
	// Copy Constructor
	CGamePlayState(const CGamePlayState&);
	// Assignment Operator
	CGamePlayState& operator=(const CGamePlayState&);
	//Destructor
	~CGamePlayState(void);
public:
	///////////////////////////////////////////////////////////////////
	//	Function: GetInstance
	//
	//	Purpose: Init the instance and return a pointer to the instance
	///////////////////////////////////////////////////////////////////
	static CGamePlayState* GetInstance(void);
  
	inline bool GetPaused(){return m_bIsPaused;}
	inline void SetPaused(bool p){m_bIsPaused = p;}

	///////////////////////////////////////////////////////////////////
	//	Function:	Enter
	//
	//	Purpose:	Enter and initialize the current state
	//////////////////////////////////////////////////////////////////
	void Enter(void);
	//////////////////////////////////////////////////////////////////
	//	Function: Exit
	//
	//	Purpose: Cleans up all the state info
	///////////////////////////////////////////////////////////////////
	void Exit(void);
	///////////////////////////////////////////////////////////////////
	//	Function: Input
	//
	//	Purpose: Read the input from the user
	/////////////////////////////////////////////////////////////////
	bool Input(float, POINT);
	/////////////////////////////////////////////////////////////////
	//	Function:	Update
	//
	//	Purpose:	Update the state objects
	////////////////////////////////////////////////////////////////
	void Update(float fElapsedTime);
	////////////////////////////////////////////////////////////////
	//	Function:	Render
	//
	//	Purpose:	Render the objects of the state
	////////////////////////////////////////////////////////////////
	void Render(void);
	////////////////////////////////////////////////////////////////
	//	Function:	MessageProc
	//
	//	Purpose:	Retrieves and handles the messages
	////////////////////////////////////////////////////////////////
	//static friend void MessageProc(CBaseMessage* pMsg);
};