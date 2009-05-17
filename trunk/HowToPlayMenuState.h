//////////////////////////////////////////////////////////////////////////
//	Filename	:	HowToPlayMenuState.h
//
//	Author		:	Ramon Johannessen (RJ)
//
//	Purpose		:	The How to Play Menu will display information on how to
//					play the game. 
//////////////////////////////////////////////////////////////////////////
#ifndef HOWTOPLAYMENUSTATE_H
#define HOWTOPLAYMENUSTATE_H

#include "basemenustate.h"

class CHowToPlayMenuState : public CBaseMenuState
{
private:

	// proper singleton
	CHowToPlayMenuState(void);
	~CHowToPlayMenuState(void);
	CHowToPlayMenuState(const CHowToPlayMenuState &);
	CHowToPlayMenuState& operator= (const CHowToPlayMenuState&);

public:
	//////////////////////////////////////////////////////////////////////////
	//	Function	:	GetInstance
	//
	//	Purpose		:	Return the only instance of this object
	//////////////////////////////////////////////////////////////////////////
	CHowToPlayMenuState* GetInstance();
	//////////////////////////////////////////////////////////////////////////
	//	Function	:	Render
	//
	//	Purpose		:	Render the How To Play menu
	//////////////////////////////////////////////////////////////////////////
	void Render();
	//////////////////////////////////////////////////////////////////////////
	//	Function	:	Input
	//
	//	Purpose		:	Handle any user input for this menu state, mouse or keyboard 
	//
	//	Return		:	true/false, false if we are exiting the game
	//////////////////////////////////////////////////////////////////////////
	bool Input();

};

#endif