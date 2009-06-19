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
	m_nYVelocity = -1;
	m_nYPos = 300;
	m_dwCounter = 0;
	//GetMenuItemSpacing() == offset between each line printed
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
	
	m_dwTimer = GetTickCount();
	SetBGImageID(GetAssets()->aCMbgID);
	SetBGHeight(GetTM()->GetTextureHeight(GetAssets()->aCMbgID));
	SetBGWidth(GetTM()->GetTextureWidth(GetAssets()->aCMbgID));
	CenterBGImage();
	m_nYPos = 300;
	m_nYVelocity = -1;
	m_nAlpha = 150;
	GetFMOD()->PlaySound(GetAssets()->aBMninjarap);
	GetFMOD()->SetVolume(GetAssets()->aBMninjarap, GetGame()->GetMusicVolume());
}

bool CCreditState::Input(float fElapsedTimes, POINT mousePT)
{
	if(GetDI()->JoystickDPadPressed(2,0) || GetDI()->KeyPressed(DIK_UP))//up
	{
		//move text slower
		m_nYVelocity--;
	}
	else if(GetDI()->JoystickDPadPressed(3,0) || GetDI()->KeyPressed(DIK_DOWN))//down
	{
		//move text faster
		m_nYVelocity++;
	}
	if(GetDI()->KeyPressed(DIK_ESCAPE) || GetDI()->JoystickButtonPressed(1,0))
	{
		GetGame()->ChangeState(CMainMenuState::GetInstance());
	}
	return true;
}

void CCreditState::Render()
{
	//CBaseMenuState::Render();
	RECT imageRect = { 0, 0, GetBGImageWidth(), GetBGImageHeight()};
	GetTM()->DrawWithZSort(GetBGImageID(), 0,0,0.1f,1.0f,1.0f, &imageRect, 0.0f,0.0f,0.0f,D3DCOLOR_ARGB(m_nAlpha, 255,255,255));
	

	GetBitmapFont()->DrawString("CREDITS", 100, m_nYPos );

	GetBitmapFont()->DrawString("EXECUTIVE PRODUCER", 100, m_nYPos + GetMenuItemSpacing());
	GetBitmapFont()->DrawString("DUSTIN CLINGMAN", 100, m_nYPos + GetMenuItemSpacing()*2);

	GetBitmapFont()->DrawString("ASSOCIATE PRODUCER", 100, m_nYPos + GetMenuItemSpacing()*3);
	GetBitmapFont()->DrawString("ROB MARTINEZ", 150, m_nYPos + GetMenuItemSpacing()*4);

	GetBitmapFont()->DrawString("ENGINEER", 100, m_nYPos + GetMenuItemSpacing()*5);

	GetBitmapFont()->DrawString("ART", 100, m_nYPos + GetMenuItemSpacing()*6);

	GetBitmapFont()->DrawString("ANIMATION", 100, m_nYPos + GetMenuItemSpacing()*7);
	GetBitmapFont()->DrawString("MATT DI MATTEO", 100, m_nYPos + GetMenuItemSpacing()*8);

	GetBitmapFont()->DrawString("LEVEL DESIGN", 100, m_nYPos + GetMenuItemSpacing()*9);
	GetBitmapFont()->DrawString("RAMON JOHANNESSEN", 100, m_nYPos + GetMenuItemSpacing()*10);

	GetBitmapFont()->DrawString("PROJECT LEAD", 100, m_nYPos + GetMenuItemSpacing()*11);

	GetBitmapFont()->DrawString("TECHNICAL DIRECTOR", 100, m_nYPos + GetMenuItemSpacing()*12);

	GetBitmapFont()->DrawString("AUDIO DIRECTOR", 100, m_nYPos + GetMenuItemSpacing()*13);

	GetBitmapFont()->DrawString("LEAD SOUND DESIGNER", 100, m_nYPos + GetMenuItemSpacing()*14);

	GetBitmapFont()->DrawString("SOUND DESIGN", 100, m_nYPos + GetMenuItemSpacing()*15);

	GetBitmapFont()->DrawString("SOUND ASSISTANT", 100, m_nYPos + GetMenuItemSpacing()*16);

	GetBitmapFont()->DrawString("VOICE DIRECTION", 100, m_nYPos + GetMenuItemSpacing()*17);

	GetBitmapFont()->DrawString("VOICE TALENT", 100, m_nYPos + GetMenuItemSpacing()*18);

	GetBitmapFont()->DrawString("ORIGINAL MUSIC BY", 100, m_nYPos + GetMenuItemSpacing()*19);
	GetBitmapFont()->DrawString("VANILLA ICE", 100, m_nYPos + GetMenuItemSpacing()*20);

	GetBitmapFont()->DrawString("PROJECT MANAGEMENT", 100, m_nYPos + GetMenuItemSpacing()*21);

	GetBitmapFont()->DrawString("LEAD TESTER", 100, m_nYPos + GetMenuItemSpacing()*22);

	GetBitmapFont()->DrawString("ART SUPPORT", 100, m_nYPos + GetMenuItemSpacing()*23);

	GetBitmapFont()->DrawString("GENERAL MANAGER", 100, m_nYPos + GetMenuItemSpacing()*24);

	GetBitmapFont()->DrawString("DOCUMENTATION", 100, m_nYPos + GetMenuItemSpacing()*25);

	GetBitmapFont()->DrawString("PACKAGE DESIGN", 100, m_nYPos + GetMenuItemSpacing()*26);

	GetBitmapFont()->DrawString("TECHNICAL OFFICER", 100, m_nYPos + GetMenuItemSpacing()*27);

	GetBitmapFont()->DrawString("GAMEPLAY OFFICER", 100, m_nYPos + GetMenuItemSpacing()*28);

	GetBitmapFont()->DrawString("INTERFACE OFFICER", 100, m_nYPos + GetMenuItemSpacing() * 29);

	GetBitmapFont()->DrawString("PROJECT OFFICER", 100, m_nYPos + GetMenuItemSpacing() * 30);

	GetBitmapFont()->DrawString("SPECIAL THANKS", 100, m_nYPos + GetMenuItemSpacing()*31 );
}

void CCreditState::Exit()
{
	GetFMOD()->StopSound(GetAssets()->aBMninjarap);
	GetFMOD()->ResetSound(GetAssets()->aBMninjarap);
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

	m_dwCounter++;
	if(m_dwCounter > 25)
	{
		m_nYPos += m_nYVelocity;
		m_dwCounter = 0;
	}
}