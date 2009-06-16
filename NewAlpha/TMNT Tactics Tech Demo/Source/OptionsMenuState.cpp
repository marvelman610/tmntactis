//////////////////////////////////////////////////////////////////////////
//	Filename	:	COptionsMenuState.cpp
//
//	Author		:	Ramon Johannessen (RJ)
//
//	Purpose		:	The Options Menu will allow the user to customize the
//					game settings, such as music volume, sound effects, etc. 
//////////////////////////////////////////////////////////////////////////
#include "OptionsMenuState.h"
#include "MainMenuState.h"
#include "CSGD_TextureManager.h"
#include "CSGD_DirectInput.h"
#include "Game.h"
#include "GamePlayState.h"
#include "BitmapFont.h"
#include "Assets.h"
#include <fstream>

enum {MUSIC_VOLUME, SFX_VOLUME, BACK, NULL_END};

COptionsMenuState::COptionsMenuState() 
{

}
COptionsMenuState::~COptionsMenuState()
{

}

COptionsMenuState* COptionsMenuState::GetInstance()
{
	static COptionsMenuState menuState;
	return &menuState;
}

void COptionsMenuState::Enter()
{
	CBaseMenuState::Enter();
	m_bHasASettingChanged = false;

	SetBGImageID(GetAssets()->aOMbgID);
	SetBGWidth(GetTM()->GetTextureWidth(GetAssets()->aOMbgID));
	SetBGHeight(GetTM()->GetTextureHeight(GetAssets()->aOMbgID));
	
	
//	m_fmsBGMusicID		= GetAssets()->m_fmsOMBGmusic;

	SetMenuX(400); SetMenuY(350);
	SetCursorX(GetMenuX()-80); SetCursorY(GetMenuY()-15);
	SetCurrMenuSelection( MUSIC_VOLUME );

	m_nSFXVolume	= GetGame()->GetSFXVolume();
	m_nMusicVolume	= GetGame()->GetMusicVolume();
	//m_pFMODsys->Play(FMOD_CHANNEL_FREE, m_fmsBGMusicID, false, FMOD_CHANNEL_REUSE);
}


bool COptionsMenuState::Input(float fElapsedTime, POINT mousePt)
{
	if (GetDI()->KeyPressed(DIK_DOWN) || GetDI()->JoystickDPadPressed(3,0))
	{
		SetCurrMenuSelection(GetCurrMenuSelection()+1);
		if (GetCurrMenuSelection() == NULL_END)
			SetCurrMenuSelection(MUSIC_VOLUME);
	}
	else if (GetDI()->KeyPressed(DIK_UP)|| GetDI()->JoystickDPadPressed(2,0))
	{
		SetCurrMenuSelection(GetCurrMenuSelection()-1);;
		if (GetCurrMenuSelection() < MUSIC_VOLUME)
			SetCurrMenuSelection(NULL_END-1);
	}
	else if (GetDI()->KeyDown(DIK_LEFT)|| GetDI()->JoystickDPadPressed(0,0))
	{
		switch(GetCurrMenuSelection())
		{
		case MUSIC_VOLUME:
			if (m_nMusicVolume > 0)
			{
				m_nMusicVolume--;
				GetGame()->SetMusicVolume((float)m_nMusicVolume/100.0f);
				m_bHasASettingChanged = true;
			}
			break;
		case SFX_VOLUME:
			if (m_nSFXVolume > 0)
			{
				m_nSFXVolume--;
				GetGame()->SetSFXVolume((float)m_nSFXVolume/100.0f);
				m_bHasASettingChanged = true;
			}
			break;
		}
	}
	else if (GetDI()->KeyDown(DIK_RIGHT)|| GetDI()->JoystickDPadPressed(1,0))
	{
		switch(GetCurrMenuSelection())
		{
		case MUSIC_VOLUME:
			if (m_nMusicVolume < 100)
			{
				m_nMusicVolume++;
				GetGame()->SetMusicVolume((float)m_nMusicVolume/100.0f);
				m_bHasASettingChanged = true;
			}
			break;
		case SFX_VOLUME:
			if (m_nSFXVolume < 100)
			{
				m_nSFXVolume++;
				GetGame()->SetSFXVolume((float)m_nSFXVolume/100.0f);
				m_bHasASettingChanged = true;
			}
		}
	}
	else if (GetDI()->KeyPressed(DIK_RETURN) || GetDI()->JoystickButtonPressed(0,0))
	{
		switch(GetCurrMenuSelection())
		{
		case BACK:
			GetGame()->ChangeState(CMainMenuState::GetInstance());
		}
	}
	return true;
}

void COptionsMenuState::Render()
{
	CBaseMenuState::Render();
	// TODO:: finish options rendering here
	char szText[64];
	sprintf_s(szText, "MUSIC VOLUME (%i)", m_nMusicVolume);
	GetBitmapFont()->DrawString(szText, GetMenuX(), GetMenuY());
	sprintf_s(szText, "SFX VOLUME (%i)", m_nSFXVolume);
	GetBitmapFont()->DrawString(szText, GetMenuX(), GetMenuY() + GetMenuItemSpacing(), 1.0f);
	CBitmapFont::GetInstance()->DrawString("EXIT", 400,GetMenuY()+ (2*GetMenuItemSpacing()));
	// Draw menu cursor
	GetTM()->DrawWithZSort(GetAssets()->aMenuCursorImageID, GetCursorX(), GetCursorY() + (GetCurrMenuSelection()*GetMenuItemSpacing()), 0);
}

void COptionsMenuState::SaveSettings()
{
 	ofstream ofs("Resources/SavedGames/SoundSettings.txt", ios_base::trunc);
 
	float x = GetGame()->GetSFXVolume();
	float xx = GetGame()->GetMusicVolume();
	ofs << GetGame()->GetMusicVolume() << '\n';
	ofs << GetGame()->GetSFXVolume() << '\n';
	ofs.close();
}

void COptionsMenuState::Exit()
{
	if (m_bHasASettingChanged)
		SaveSettings();
	CBaseMenuState::Exit();
}