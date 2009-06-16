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
#include "CSGD_FModManager.h"
#include "BitmapFont.h"
#include "Assets.h"
#include "Game.h"

CCreditState::CCreditState()
{
	m_dwTimer = 0;
	m_nAlpha = 0;
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
	CBaseMenuState::Enter();
	GetFMOD()->PlaySound(GetAssets()->aCMmusicID);
	GetFMOD()->SetVolume(GetAssets()->aCMmusicID, GetGame()->GetMusicVolume());
	m_dwTimer = GetTickCount();
	SetBGImageID(GetAssets()->aCMbgID);
	SetBGHeight(GetTM()->GetTextureHeight(GetAssets()->aCMbgID));
	SetBGWidth(GetTM()->GetTextureWidth(GetAssets()->aCMbgID));
	CenterBGImage();
	m_nAlpha = 150;
}

bool CCreditState::Input(float fElapsedTimes, POINT mousePT)
{
	
	if(GetDI()->KeyPressed(DIK_ESCAPE))
	{
		GetGame()->ChangeState(CMainMenuState::GetInstance());
	}
	return true;
}

void CCreditState::Render()
{
	//CBaseMenuState::Render();
	RECT imageRect = { 0, 0, GetBGImageWidth(), GetBGImageHeight()};
	GetTM()->Draw(GetBGImageID(), 0,0,1.0f,1.0f, &imageRect, 0.0f,0.0f,0.0f,D3DCOLOR_ARGB(m_nAlpha, 255,255,255));
	
	GetBitmapFont()->DrawString("ESC", 440, 384);
}

void CCreditState::Exit()
{
	GetFMOD()->StopSound(GetAssets()->aCMmusicID);
	GetFMOD()->ResetSound(GetAssets()->aCMmusicID);
	CBaseMenuState::Exit();
}

void CCreditState::Update(float fElapsedtime)
{
	CBaseMenuState::Update(fElapsedtime);

	if(GetTickCount()- m_dwTimer > 3000)
	{
		//change the image
		if(GetBGImageID() == GetAssets()->aCMbgID)
		{
			SetBGImageID(GetAssets()->aCMbgID2);
			SetBGHeight(270);
			SetBGWidth(480);
			CenterBGImage();
		}
		else if(GetBGImageID() == GetAssets()->aCMbgID2)
		{
			SetBGImageID(GetAssets()->aCMbgID3);
			SetBGHeight(450);
			SetBGWidth(608);
			CenterBGImage();
		}
		else if(GetBGImageID() == GetAssets()->aCMbgID3)
		{
			SetBGImageID(GetAssets()->aCMbgID4);
			SetBGHeight(480);
			SetBGWidth(640);
			CenterBGImage();
		}
		else if(GetBGImageID() == GetAssets()->aCMbgID4)
		{
			SetBGImageID(GetAssets()->aCMbgID5);
			SetBGHeight(300);
			SetBGWidth(400);
			CenterBGImage();
		}
		else if(GetBGImageID() == GetAssets()->aCMbgID5)
		{
			SetBGImageID(GetAssets()->aCMbgID6);
			SetBGHeight(280);
			SetBGWidth(400);
			CenterBGImage();
		}
		m_dwTimer = GetTickCount();
	}
	
	if(GetTickCount() - m_dwTimer < 1500)
	{
		//fade in
		if(m_nAlpha < 255)
			m_nAlpha++;
	}
	else
	{
		//fade out
		if(m_nAlpha > 0)
			m_nAlpha--;
	}
}