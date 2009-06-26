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
enum {BG_SELECT, BG_MOVE, BG_ATTACK, BG_SPECIAL, BG_ITEM, NUM_BGS, };

CHowToPlayMenuState* CHowToPlayMenuState::GetInstance()
{
	static CHowToPlayMenuState menuState;
	return &menuState;
}
void CHowToPlayMenuState::Enter()
{
	CBaseMenuState::Enter();

	// setup the bgs 
	m_pBGids = new int[NUM_BGS];
	m_pBGids[BG_SELECT] = GetAssets()->aHTPMSelectID;
	m_pBGids[BG_MOVE] = GetAssets()->aHTPMMoveID;
	m_pBGids[BG_ATTACK] = GetAssets()->aHTPMAttackID;
	m_pBGids[BG_SPECIAL] = GetAssets()->aHTPMSpecialID;
	m_pBGids[BG_ITEM] = GetAssets()->aHTPMItemID;

	m_bMouseOverEsc = false;
	SetBGImageID(m_pBGids[BG_SELECT]);	// starts out on select bg
 	SetBGWidth(GetTM()->GetTextureWidth(m_pBGids[BG_SELECT]));
 	SetBGHeight(GetTM()->GetTextureHeight(m_pBGids[BG_SELECT]));

	SetCurrMenuSelection( BACK );
	SetCursorX(350);
	SetCursorY(335);

	GetFMOD()->PlaySound(GetAssets()->aHTPsound);
	GetFMOD()->SetVolume(GetAssets()->aHTPsound, GetGame()->GetMusicVolume());
}

void CHowToPlayMenuState::Exit()
{
	delete[] m_pBGids;
	m_pBGids = NULL;
	GetFMOD()->StopSound(GetAssets()->aHTPsound);
	GetFMOD()->ResetSound(GetAssets()->aHTPsound);
	CBaseMenuState::Exit();
}

bool CHowToPlayMenuState::Input(float fElapsedTime, POINT mousePT)
{
	m_nMouseX = mousePT.x;
	m_nMouseY = mousePT.y;
	if (mousePT.x > 500 && mousePT.x < 625 && mousePT.y > 720 && mousePT.y < 768)
		m_bMouseOverEsc = true;	// get out now!
	else
		m_bMouseOverEsc = false;
	if (GetDI()->KeyPressed(DIK_ESCAPE) || (m_bMouseOverEsc && GetDI()->MouseButtonPressed(MOUSE_LEFT)) || GetDI()->JoystickButtonPressed(1,0))
		CGame::GetInstance()->ChangeState(CMainMenuState::GetInstance());

	if(GetDI()->KeyPressed(DIK_RETURN) || GetDI()->MouseButtonPressed(MOUSE_RIGHT) || GetDI()->JoystickButtonPressed(0,0))
	{
		m_nCurrBG++;
		if (m_nCurrBG == NUM_BGS)	//wrap around to first one
			m_nCurrBG = 0;
		SetBGImageID(m_pBGids[m_nCurrBG]);
	}
	else if (GetDI()->MouseButtonPressed(MOUSE_LEFT) /*TODO::what is joystick btn for this?*/)
	{
		m_nCurrBG--;
		if (m_nCurrBG < 0)			// wrap around to last one
			m_nCurrBG = NUM_BGS-1;
		SetBGImageID(m_pBGids[m_nCurrBG]);
	}

	return true;
}

void CHowToPlayMenuState::Render()
{
	CBaseMenuState::Render();
	GetTM()->DrawWithZSort(GetAssets()->aMousePointerID, m_nMouseX-10, m_nMouseY-3, 0.0f);

	CBitmapFont::GetInstance()->DrawString("NEXT PAGE R-click", 720, 720, 0.05f, 0.4f);
	CBitmapFont::GetInstance()->DrawString("PREV PAGE L-click", 720, 740, 0.05f, 0.4f);

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
