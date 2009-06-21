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
#include "CSGD_FModManager.h"
#include "Game.h"
#include "GamePlayState.h"
#include "BitmapFont.h"
#include "Assets.h"
#include <fstream>

#define VOLUME_ADJUST_SPEED 80.0f
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

	SetMenuX(300); SetMenuY(350);
	SetCursorX(GetMenuX()-80); SetCursorY(GetMenuY()-15);
	SetCurrMenuSelection( MUSIC_VOLUME );

	m_nSFXVolume	= (int)(GetGame()->GetSFXVolume() * 100.0f);
	m_nMusicVolume	= (int)(GetGame()->GetMusicVolume() * 100.0f);
	//m_pFMODsys->Play(FMOD_CHANNEL_FREE, m_fmsBGMusicID, false, FMOD_CHANNEL_REUSE);
	
	GetFMOD()->PlaySound(GetAssets()->aOMmusicSnd);
	GetFMOD()->SetVolume(GetAssets()->aOMmusicSnd, GetGame()->GetMusicVolume());
}


bool COptionsMenuState::Input(float fElapsedTime, POINT mousePt)
{

	CBaseMenuState::Input(fElapsedTime, mousePt);
	if (mousePt.y != m_nMouseY)
	{
		int oldSelection = GetCurrMenuSelection(); 
		int newSelection = (mousePt.y - GetMenuY()) / GetMenuItemSpacing();
		if (newSelection < 0)
			newSelection = 0;
		else if (newSelection > NULL_END-1)
			newSelection = NULL_END-1;
		if ( oldSelection != newSelection )
		{
			SetCurrMenuSelection( newSelection );
			if (GetCurrMenuSelection() < 0)
				SetCurrMenuSelection(MUSIC_VOLUME);
			else if (GetCurrMenuSelection() > NULL_END-1)
				SetCurrMenuSelection(NULL_END-1);
			if (GetFMOD()->IsSoundPlaying(GetAssets()->aMMmenuMoveSnd))
			{
				GetFMOD()->StopSound(GetAssets()->aMMmenuMoveSnd);
				GetFMOD()->ResetSound(GetAssets()->aMMmenuMoveSnd);
			}
			GetFMOD()->PlaySound(GetAssets()->aMMmenuMoveSnd);
			if(!GetFMOD()->SetVolume(GetAssets()->aMMmenuMoveSnd, GetGame()->GetSFXVolume()*0.6f))
				MessageBox(0, "VOLUME NOT SET", "ERROR", MB_OK);
		}
	}
	m_nMouseX = mousePt.x; m_nMouseY = mousePt.y;

	if (GetDI()->KeyPressed(DIK_DOWN) || GetDI()->JoystickDPadPressed(3,0))
	{
		SetCurrMenuSelection(GetCurrMenuSelection()+1);
		if (GetCurrMenuSelection() == NULL_END)
			SetCurrMenuSelection(MUSIC_VOLUME);
	}
	else if (GetDI()->KeyPressed(DIK_UP) || GetDI()->JoystickDPadPressed(2,0))
	{
		SetCurrMenuSelection(GetCurrMenuSelection()-1);;
		if (GetCurrMenuSelection() < MUSIC_VOLUME)
			SetCurrMenuSelection(NULL_END-1);
	}
	else if (GetDI()->KeyDown(DIK_LEFT) || GetDI()->JoystickDPadPressed(0,0) || GetDI()->MouseButtonDown(MOUSE_LEFT))
	{
		switch(GetCurrMenuSelection())
		{
		case MUSIC_VOLUME:
			if (m_nMusicVolume > 0)
			{
				m_nMusicVolume -= (int)(VOLUME_ADJUST_SPEED * fElapsedTime);
				GetGame()->SetMusicVolume((float)m_nMusicVolume/100.0f);
				GetFMOD()->SetVolume(GetAssets()->aOMmusicSnd, GetGame()->GetMusicVolume());
				m_bHasASettingChanged = true;
			}
			break;
		case SFX_VOLUME:
			if (m_nSFXVolume > 0)
			{
				m_nSFXVolume -= (int)(VOLUME_ADJUST_SPEED * fElapsedTime);
				GetGame()->SetSFXVolume((float)m_nSFXVolume/100.0f);
				if (!GetFMOD()->IsSoundPlaying(GetAssets()->aMMmenuClickSnd))
				{
					GetFMOD()->PlaySound(GetAssets()->aMMmenuClickSnd);
					GetFMOD()->SetVolume(GetAssets()->aMMmenuClickSnd, GetGame()->GetSFXVolume());
				}
				m_bHasASettingChanged = true;
			}
			break;
		}
	}
	else if (GetDI()->KeyDown(DIK_RIGHT) || GetDI()->JoystickDPadPressed(1,0) || GetDI()->MouseButtonDown(MOUSE_RIGHT))
	{
		switch(GetCurrMenuSelection())
		{
		case MUSIC_VOLUME:
			if (m_nMusicVolume < 100)
			{
				m_nMusicVolume += (int)(VOLUME_ADJUST_SPEED * fElapsedTime);
				GetGame()->SetMusicVolume((float)m_nMusicVolume/100.0f);
				m_bHasASettingChanged = true;
				GetFMOD()->SetVolume(GetAssets()->aOMmusicSnd, GetGame()->GetMusicVolume());
			}
			break;
		case SFX_VOLUME:
			if (m_nSFXVolume < 100)
			{
				m_nSFXVolume += (int)(VOLUME_ADJUST_SPEED * fElapsedTime);
				GetGame()->SetSFXVolume((float)m_nSFXVolume/100.0f);
				if (!GetFMOD()->IsSoundPlaying(GetAssets()->aMMmenuClickSnd))
				{
					GetFMOD()->PlaySound(GetAssets()->aMMmenuClickSnd);
					GetFMOD()->SetVolume(GetAssets()->aMMmenuClickSnd, GetGame()->GetSFXVolume());
				}
				m_bHasASettingChanged = true;
			}
		}
	}
	if (GetDI()->KeyPressed(DIK_RETURN) || GetDI()->JoystickButtonPressed(0,0) || GetDI()->MouseButtonPressed(MOUSE_LEFT) || GetDI()->KeyPressed(DIK_ESCAPE))
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
	GetTM()->DrawWithZSort(GetAssets()->aMousePointerID, m_nMouseX-10, m_nMouseY-3, 0.0f);
	// TODO:: finish options rendering here
	char szText[64];
	sprintf_s(szText, "MUSIC VOLUME (%i)", m_nMusicVolume);
	GetBitmapFont()->DrawString(szText, GetMenuX(), GetMenuY(), 0.05f, 1.0f, D3DCOLOR_ARGB(255,0,255,0));
	GetBitmapFont()->DrawString(szText, GetMenuX()+4, GetMenuY()+4, 0.051f, 1.0f, D3DCOLOR_ARGB(255,255,0,0));
	sprintf_s(szText, "SFX VOLUME (%i)", m_nSFXVolume);
	GetBitmapFont()->DrawString(szText, GetMenuX(), GetMenuY() + GetMenuItemSpacing(), 0.05f, 1.0f, D3DCOLOR_ARGB(255,0,255,0));
	GetBitmapFont()->DrawString(szText, GetMenuX()+4, GetMenuY() + GetMenuItemSpacing()+4, 0.051f, 1.0f, D3DCOLOR_ARGB(255,255,0,0));
	GetBitmapFont()->DrawString("EXIT", GetMenuX(), GetMenuY() + (2*GetMenuItemSpacing()), 0.05f, 1.0f, D3DCOLOR_ARGB(255,0,255,0));
	GetBitmapFont()->DrawString("EXIT", GetMenuX()+4, GetMenuY() + (2*GetMenuItemSpacing())+4, 0.051f, 1.0f, D3DCOLOR_ARGB(255,255,0,0));
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
	GetFMOD()->StopSound(GetAssets()->aOMmusicSnd);
	GetFMOD()->ResetSound(GetAssets()->aOMmusicSnd);
	if (m_bHasASettingChanged)
		SaveSettings();
	CBaseMenuState::Exit();
}