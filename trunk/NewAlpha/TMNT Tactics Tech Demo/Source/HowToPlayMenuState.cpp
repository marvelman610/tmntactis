//////////////////////////////////////////////////////////////////////////
//	Filename	:	HowToPlayMenuState.cpp
//
//	Author		:	Ramon Johannessen (RJ)
//
//	Purpose		:	The How to Play Menu will display information on how to
//					play the game. 
//////////////////////////////////////////////////////////////////////////
#include "HowToPlayMenuState.h"
#include "BaseMenuState.h"
#include "MainMenuState.h"
#include "CSGD_TextureManager.h"
#include "CSGD_DirectInput.h"
#include "BitmapFont.h"
#include "Assets.h"
#include "Game.h"
#include "CSGD_FModManager.h"

// if we have any menu options in this menu, 
//	add to this enum
enum {BACK, NULL_END};

CHowToPlayMenuState* CHowToPlayMenuState::GetInstance()
{
	static CHowToPlayMenuState menuState;
	return &menuState;
}
void CHowToPlayMenuState::Enter()
{
	CBaseMenuState::Enter();
	m_bMouseOverEsc = false;
	SetBGImageID(GetAssets()->aHTPMSelectID);
 	SetBGWidth(GetTM()->GetTextureWidth(GetAssets()->aHTPMSelectID));
 	SetBGHeight(GetTM()->GetTextureHeight(GetAssets()->aHTPMSelectID));
	//CenterBGImage();

	SetCurrMenuSelection( BACK );
	SetCursorX(350);
	SetCursorY(335);

	GetFMOD()->PlaySound(GetAssets()->aHTPsound);
	GetFMOD()->SetVolume(GetAssets()->aHTPsound, GetGame()->GetMusicVolume());
}

void CHowToPlayMenuState::Exit()
{
	GetFMOD()->StopSound(GetAssets()->aHTPsound);
	GetFMOD()->ResetSound(GetAssets()->aHTPsound);
	CBaseMenuState::Exit();
}

bool CHowToPlayMenuState::Input(float fElapsedTime, POINT mousePT)
{
	m_nMouseX = mousePT.x;
	m_nMouseY = mousePT.y;
	if(GetDI()->KeyPressed(DIK_RETURN) || GetDI()->MouseButtonPressed(MOUSE_LEFT) || GetDI()->JoystickButtonPressed(0,0))
	{
		if(GetBGImageID() == GetAssets()->aHTPMSelectID)
		{
			SetBGImageID(GetAssets()->aHTPMMoveID);
		}
		else if(GetBGImageID() == GetAssets()->aHTPMMoveID)
		{
			SetBGImageID(GetAssets()->aHTPMAttackID);
		}
		else if(GetBGImageID() == GetAssets()->aHTPMAttackID)
		{
			SetBGImageID(GetAssets()->aHTPMSpecialID);
		}
		else if(GetBGImageID() == GetAssets()->aHTPMSpecialID)
		{
			SetBGImageID(GetAssets()->aHTPMItemID);
		}
		else if(GetBGImageID() == GetAssets()->aHTPMItemID)
		{
			SetBGImageID(GetAssets()->aHTPMSelectID);
		}
	}
	if (mousePT.x > 500 && mousePT.x < 600 && mousePT.y > 720 && mousePT.y < 750)
		m_bMouseOverEsc = true;
	else
		m_bMouseOverEsc = false;
	if (GetDI()->KeyPressed(DIK_ESCAPE) || (m_bMouseOverEsc && GetDI()->MouseButtonPressed(MOUSE_LEFT)) || GetDI()->JoystickButtonPressed(1,0))
	{
		CGame::GetInstance()->ChangeState(CMainMenuState::GetInstance());
	}
	return true;
}

void CHowToPlayMenuState::Render()
{
	CBaseMenuState::Render();
	GetTM()->DrawWithZSort(GetAssets()->aMousePointerID, m_nMouseX-10, m_nMouseY-3, 0.0f);

	CBitmapFont::GetInstance()->DrawString("CLICK TO MOVE ON", 350, 680);

	// TODO:: add the how to play info here
	if (m_bMouseOverEsc)
		CBitmapFont::GetInstance()->DrawString("ESC", 512, 730, 0.001f, 1.0f, D3DCOLOR_XRGB(255, 0, 0));
	else
		CBitmapFont::GetInstance()->DrawString("ESC", 512, 730);
}

void CHowToPlayMenuState::Update(float fElapsedTime)
{
	CBaseMenuState::Update(fElapsedTime);
	// TODO:: add update code here

}
