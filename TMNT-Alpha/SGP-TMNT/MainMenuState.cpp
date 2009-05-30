//////////////////////////////////////////////////////////////////////////
//	Filename	:	MainMenuState.cpp
//
//	Author		:	Ramon Johannessen (RJ)
//
//	Purpose		:	The Main Menu will be the first menu loaded when the game
//					starts. It will display all menu selections available.
//////////////////////////////////////////////////////////////////////////

#include "MainMenuState.h"

CMainMenuState* GetInstance()
{
	static CMainMenuState menuState;
	return &menuState;
}

bool CMainMenuState::Input()
{
	if (m_pDI->KeyPressed(DIK_DOWN))
	{
		m_nCurrMenuSelection++;
		if (m_nCurrMenuSelection == NULL_END)
			m_nCurrMenuSelection = PLAY;
	}
	else if (m_pDI->KeyPressed(DIK_UP))
	{
		m_nCurrMenuSelection--;
		if (m_nCurrMenuSelection < PLAY)
			m_nCurrMenuSelection = NULL_END-1;
	}
	else if (m_pDI->KeyPressed(DIK_RETURN))
	{
		switch(m_nCurrMenuSelection)
		{
		case PLAY:
			CGame::GetInstance()->ChangeState(CGamePlayState::GetInstance());
			break;
		case OPTIONS:
			CGame::GetInstance()->ChangeState(COptionsMenuState::GetInstance());
			break;
		case HOWTOPLAY:
			CGame::GetInstance ()->ChangeState(CHowToPlayMenuState::GetInstance ());
			break;
		case CREDITS:
			CGame::GetInstance ()->ChangeState(CCreditsMenuState::GetInstance ());
			break;
		case EXIT:
			CGame::GetInstance()->SetIsRunning(false);
			break;
		case LOAD:
			CGame::GetInstance()->ChangeState(CLoadMenuState::GetInstance());
		}
	}
}

void CMainMenuState::Render()
{
	CBaseMenuState::Render();
	m_pD3D->Draw(m_nCursorImageID, m_nCursorX, m_nCursorY);
}

void CMainMenuState::Update(float fElapsedTime)
{

}

void CMainMenuState::Enter()
{
	CBaseMenuState::Enter();
	m_nBGImageID		= m_pAssets->m_nMMBGimageID;
	m_nImageWidth		= m_pTM->GetImageWidth(m_nBGImageID);
	m_nImageHeight		= m_pTM->GetImageHeight(m_nBGImageID);
	m_nCursorImageID	= m_pAssets->m_nMMCursorImageID;
	m_sBGMusicID		= m_pAssets->m_sMMBGmusic;

	m_nCurrMenuSelection = PLAY;
	// TODO:: setup starting cursor position
	m_nCursorX = 0;
	m_nCursorY = 0;
}

void CMainMenuState::Exit()
{
	CBaseMenuState::Exit();
}