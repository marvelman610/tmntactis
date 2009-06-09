/////////////////////////////////////////////////////////////////////////////
//	Filename	:	CCreditState.cpp
//
//	Author		:	Jose Sierra (JS)
//
//	Purpose		:	To let the player view the game credits
/////////////////////////////////////////////////////////////////////////////
#include "CreditState.h"
#include "MainMenuState.h"
#include "CSGD_TextureManager.h"
#include "CSGD_DirectInput.h"
#include "BitmapFont.h"
#include "Assets.h"
#include "Game.h"


CCreditState::CCreditState()
{
}

CCreditState::~CCreditState()
{
}

CCreditState* CCreditState::GetInstance()
{
	static CCreditState creditState;
	return &creditState;
}

void CCreditState::Enter()
{
	
}

bool CCreditState::Input(float fElapsedTimes, POINT mousePT)
{
	if(CSGD_DirectInput::GetInstance()->KeyPressed(DIK_ESCAPE))
	{
		CGame::GetInstance()->ChangeState(CMainMenuState::GetInstance());
	}
	return true;
}

void CCreditState::Render()
{
	CBitmapFont::GetInstance()->DrawString("ESC", 440, 384, 1.0f, D3DCOLOR_ARGB(255,255,0,0));
}

void CCreditState::Exit()
{

}