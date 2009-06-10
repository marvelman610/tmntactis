//////////////////////////////////////////////////////////////////////////
//	Filename	:	COptionsMenuState.h
//
//	Author		:	Ramon Johannessen (RJ)
//
//	Purpose		:	The Options Menu will allow the user to customize the
//					game settings, such as music volume, sound effects, etc. 
//////////////////////////////////////////////////////////////////////////
#ifndef OPTIONSMENUSTATE_H
#define OPTIONSMENUSTATE_H

#include "BaseMenuState.h"
#include <windows.h>
#include <iostream>
using namespace std;


class COptionsMenuState : public CBaseMenuState
{
private:
	bool m_bHasASettingChanged;	// has the user changed any settings? if so, save them on exit
	int  m_nSFXVolume;
	int  m_nMusicVolume;
	//fmod::Sound m_fmsSFX;		// the sound played when the user changes the SFX volume
	
	COptionsMenuState(void);
	~COptionsMenuState(void);
	COptionsMenuState(const COptionsMenuState&);
	COptionsMenuState& operator= (const COptionsMenuState&);

public:
	//////////////////////////////////////////////////////////////////////////
	//	Function	:	GetInstance
	//
	//	Purpose		:	Return the only instance of this object
	//////////////////////////////////////////////////////////////////////////
	static COptionsMenuState* GetInstance();
	//////////////////////////////////////////////////////////////////////////
	//	Function	:	Render
	//
	//	Purpose		:	Render the Options menu
	//////////////////////////////////////////////////////////////////////////
	void Render();
	//////////////////////////////////////////////////////////////////////////
	//	Function	:	Input
	//
	//	Purpose		:	Handle any user input for this menu state, mouse or keyboard 
	//
	//	Return		:	true/false, false if we are exiting the game
	//////////////////////////////////////////////////////////////////////////
	bool Input(float fElapsedTime, POINT mousePt);
	//////////////////////////////////////////////////////////////////////////
	//	Function	:	Enter
	//
	//	Purpose		:	Load the current settings
	//////////////////////////////////////////////////////////////////////////
	void Enter();
	//////////////////////////////////////////////////////////////////////////
	//	Function	:	Exit
	//
	//	Purpose		:	Call SaveSettings
	//////////////////////////////////////////////////////////////////////////
	void Exit();
	//////////////////////////////////////////////////////////////////////////
	//	Function	:	SaveSettings
	//
	//	Purpose		:	to save any changed options
	//////////////////////////////////////////////////////////////////////////
	void SaveSettings();
};

#endif