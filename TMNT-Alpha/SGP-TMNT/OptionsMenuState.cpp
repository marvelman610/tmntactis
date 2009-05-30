//////////////////////////////////////////////////////////////////////////
//	Filename	:	COptionsMenuState.cpp
//
//	Author		:	Ramon Johannessen (RJ)
//
//	Purpose		:	The Options Menu will allow the user to customize the
//					game settings, such as music volume, sound effects, etc. 
//////////////////////////////////////////////////////////////////////////
#include "OptionsMenuState.h"
#include <fstream>

COptionsMenuState* COptionsMenuState::GetInstance()
{
	static COptionsMenuState menuState;
	return &menuState;
}


bool COptionsMenuState::Input()
{
	if (m_pDI->KeyPressed(DIK_DOWN))
	{
		m_nCurrMenuSelection++;
		if (m_nCurrMenuSelection == NULL_END)
			m_nCurrMenuSelection = SFX_VOLUME;
	}
	else if (m_pDI->KeyPressed(DIK_UP))
	{
		m_nCurrMenuSelection--;
		if (m_nCurrMenuSelection < MUSIC_VOLUME)
			m_nCurrMenuSelection = NULL_END-1;
	}
	else if (m_pDI->KeyPressed(DIK_LEFT))
	{
		switch(m_nCurrMenuSelection)
		{
		case MUSIC_VOLUME:
			{

			}
			break;
		case SFX_VOLUME:
			{

			}
			break;
		}
	}
	else if (m_pDI->KeyPressed(DIK_RIGHT))
	{
		switch(m_nCurrMenuSelection)
		{
		case MUSIC_VOLUME:
			{

			}
			break;
		case SFX_VOLUME:
			{

			}
			break;
		}
	}
	else if (m_pDI->KeyPressed(DIK_RETURN))
	{
		switch(m_nCurrMenuSelection)
		{
		case BACK:
			CGame::GetInstance()->ChangeState(CMainMenuState::GetInstance());
		}
	}
}

void COptionsMenuState::Render()
{
	CBaseMenuState::Render();
	// TODO:: add the how to play info here
}

void COptionsMenuState::SaveSettings()
{
	ofstream ofs("SoundSettings.dat", ios_base::out | ios_base::binary);

}

void COptionsMenuState::Exit()
{
	CGame::GetInstance()->SetMusicVolume(m_nMusicVolume);
	CGame::GetInstance()->SetSFXVolume(m_nSFXVolume);
	SaveSettings();
	CBaseMenuState::Exit();
}