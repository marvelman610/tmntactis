//////////////////////////////////////////////////////////////////////////
//	Filename	:	MainMenuState.h
//
//	Author		:	Ramon Johannessen (RJ)
//
//	Purpose		:	The Main Menu will be the first menu loaded when the game
//					starts. It will display all menu selections available.
//////////////////////////////////////////////////////////////////////////
#ifndef CMAINMENU_H
#define CMAINMENU_H

#include "BaseMenuState.h"
#include "Timer.h"
#include <string>
using std::string;

class CBox;

class CMainMenuState : public CBaseMenuState
{
private:
	CBox*	m_bxProfile;
	CBox*	m_bxMsg;
	bool	m_bDisplayLoadBox;
	bool	m_bNewGamePressed;
	bool	m_bGameLoaded;
	bool    m_bProfilesSaved;
	bool	m_bProfileSelected;
	int		m_nNumProfiles;
	int		m_nCurrProfileInd;
	string*  m_sProfiles;
	float	m_fTimer;
	CTimer  m_Timer;

	CMainMenuState();
	~CMainMenuState();
	CMainMenuState(const CMainMenuState&);
	CMainMenuState& operator= (const CMainMenuState&);

public:
	//////////////////////////////////////////////////////////////////////////
	//	Function	:	GetInstance
	//
	//	Purpose		:	Return the only instance of this object
	//////////////////////////////////////////////////////////////////////////
	static CMainMenuState* GetInstance();
	//////////////////////////////////////////////////////////////////////////
	//	Function	:	Update
	//
	//	Purpose		:	Update the main menu
	//////////////////////////////////////////////////////////////////////////
	void Update(float fElapsedTime);
	//////////////////////////////////////////////////////////////////////////
	//	Function	:	Render
	//
	//	Purpose		:	Render the main menu
	//////////////////////////////////////////////////////////////////////////
	void Render();
	//////////////////////////////////////////////////////////////////////////
	//	Function	:	Input
	//
	//	Purpose		:	Handle any user input for this menu state, mouse or keyboard 
	//
	//	Return		:	true/false, false if we are exiting the game
	//////////////////////////////////////////////////////////////////////////
	bool Input(float elapsedTime, POINT mouse);
	//////////////////////////////////////////////////////////////////////////
	//	Function	:	Enter
	//
	//	Purpose		:	Load the bg image, set up the sound, and all singletons used
	//////////////////////////////////////////////////////////////////////////
	void Enter();
	//////////////////////////////////////////////////////////////////////////
	//	Function	:	Exit
	//
	//	Purpose		:	Clean up any dynamic memory, release textures, etc...
	//////////////////////////////////////////////////////////////////////////
	void Exit();

	void SetIsLoaded(bool bLoaded) {m_bGameLoaded = bLoaded;}

};

#endif	

