//////////////////////////////////////////////////////////////////////////
//	Filename	:	HowToPlayMenuState.cpp
//
//	Author		:	Ramon Johannessen (RJ)
//
//	Purpose		:	The How to Play Menu will display information on how to
//					play the game. 
//////////////////////////////////////////////////////////////////////////
#include "HowToPlayMenuState.h"

CHowToPlayMenuState* CHowToPlayMenuState::GetInstance()
{
	static CHowToPlayMenuState menuState;
	return &menuState;
}

bool CHowToPlayMenuState::Input()
{
	if (m_pDI->KeyPressed(DIK_ESCAPE))
	{
		CGame::GetInstance()->ChangeState(CMainMenuState::GetInstance());
	}
}

void CHowToPlayMenuState::Render()
{
	CBaseMenuState::Render();
	// TODO:: add the how to play info here
}