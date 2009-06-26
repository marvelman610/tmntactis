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
	m_fYVelocity = -80.0f;
	m_nYPos = 500;
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
	m_nYPos = 700;
	m_fYVelocity = -70.0f;
	m_nAlpha = 150;
	m_fTimer = 0.0f;
	SetMenuX(100);
	GetFMOD()->PlaySound(GetAssets()->aBMninjarap);
	GetFMOD()->SetVolume(GetAssets()->aBMninjarap, GetGame()->GetMusicVolume());
}

bool CCreditState::Input(float fElapsedTimes, POINT mousePT)
{
	if(GetDI()->JoystickDPadPressed(2,0) || GetDI()->KeyDown(DIK_UP))//up
	{
		//move text slower
		m_fYVelocity -= (100.0f * fElapsedTimes);
		if (m_fYVelocity < -250.0f)
			m_fYVelocity = -250.0f;
	}
	else if(GetDI()->JoystickDPadPressed(3,0) || GetDI()->KeyDown(DIK_DOWN))//down
	{
		//move text faster
		m_fYVelocity += (100.0f * fElapsedTimes);
		if (m_fYVelocity > 250.0f)
			m_fYVelocity = 250.0f;
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
	CenterBGImage();
	GetTM()->DrawWithZSort(GetBGImageID(), GetBGx(), GetBGy(), 0.1f,1.0f,1.0f, &imageRect, 0.0f,0.0f,0.0f,D3DCOLOR_ARGB(m_nAlpha, 150,150,150));
	
	GetBitmapFont()->DrawStringAutoCenter("CREDITS", GetGame()->GetScreenWidth(), m_nYPos-GetMenuItemSpacing() );

	GetBitmapFont()->DrawString("EXECUTIVE PRODUCER", GetMenuX(), m_nYPos + GetMenuItemSpacing());
	GetBitmapFont()->DrawString("DUSTIN CLINGMAN", GetMenuX(), m_nYPos + GetMenuItemSpacing()*2);

	GetBitmapFont()->DrawString("ASSOCIATE PRODUCER", GetMenuX(), m_nYPos + GetMenuItemSpacing()*4);
	GetBitmapFont()->DrawString("ROB MARTINEZ", GetMenuX(), m_nYPos + GetMenuItemSpacing()*5);

	GetBitmapFont()->DrawString("ENGINEER", GetMenuX(), m_nYPos + GetMenuItemSpacing()*7);
	GetBitmapFont()->DrawString("MATT DI MATTEO", GetMenuX(), m_nYPos + GetMenuItemSpacing()* 8);
	GetBitmapFont()->DrawString("RAMON JOHANNESSEN", GetMenuX(), m_nYPos + GetMenuItemSpacing()* 9);
	GetBitmapFont()->DrawString("JOSE SIERRA", GetMenuX(), m_nYPos + GetMenuItemSpacing()* 10);

	GetBitmapFont()->DrawString("ART", GetMenuX(), m_nYPos + GetMenuItemSpacing()*12);
	GetBitmapFont()->DrawString("CHRIS JAHOSKY", GetMenuX(), m_nYPos + GetMenuItemSpacing()*13);

	GetBitmapFont()->DrawString("ANIMATION", GetMenuX(), m_nYPos + GetMenuItemSpacing()*15);
	GetBitmapFont()->DrawString("MATT DI MATTEO", GetMenuX(), m_nYPos + GetMenuItemSpacing()*16);

	GetBitmapFont()->DrawString("LEVEL DESIGN", GetMenuX(), m_nYPos + GetMenuItemSpacing()*18);
	GetBitmapFont()->DrawString("RAMON JOHANNESSEN", GetMenuX(), m_nYPos + GetMenuItemSpacing()*19);

	GetBitmapFont()->DrawString("PROJECT LEAD", GetMenuX(), m_nYPos + GetMenuItemSpacing()*21);
	GetBitmapFont()->DrawString("JOSE SIERRA", GetMenuX(), m_nYPos + GetMenuItemSpacing()*22);

	GetBitmapFont()->DrawString("TECHNICAL DIRECTOR", GetMenuX(), m_nYPos + GetMenuItemSpacing()*24);
	GetBitmapFont()->DrawString("MATT DI MATTEO", GetMenuX(), m_nYPos + GetMenuItemSpacing()*25);

	GetBitmapFont()->DrawString("AUDIO DIRECTOR", GetMenuX(), m_nYPos + GetMenuItemSpacing()*27);
	GetBitmapFont()->DrawString("MATT DI MATTEO", GetMenuX(), m_nYPos + GetMenuItemSpacing()*28);
	GetBitmapFont()->DrawString("RAMON JOHANNESSEN", GetMenuX(), m_nYPos + GetMenuItemSpacing()*29);
	GetBitmapFont()->DrawString("JOSE SIERRA", GetMenuX(), m_nYPos + GetMenuItemSpacing()*30);

	GetBitmapFont()->DrawString("LEAD SOUND DESIGNER", GetMenuX(), m_nYPos + GetMenuItemSpacing()*32);
	GetBitmapFont()->DrawString("RAMON JOHANNESSEN", GetMenuX(), m_nYPos + GetMenuItemSpacing()*33);

	GetBitmapFont()->DrawString("SOUND DESIGN", GetMenuX(), m_nYPos + GetMenuItemSpacing()*35);
	GetBitmapFont()->DrawString("MATT DI MATTEO", GetMenuX(), m_nYPos + GetMenuItemSpacing()*36);

	GetBitmapFont()->DrawString("SOUND ASSISTANT", GetMenuX(), m_nYPos + GetMenuItemSpacing()*38);
	GetBitmapFont()->DrawString("JOSE SIERRA", GetMenuX(), m_nYPos + GetMenuItemSpacing()*39);

	GetBitmapFont()->DrawString("VOICE DIRECTION", GetMenuX(), m_nYPos + GetMenuItemSpacing()*41);
	GetBitmapFont()->DrawString("RAMON JOHANNESSEN", GetMenuX(), m_nYPos + GetMenuItemSpacing()*42);

	GetBitmapFont()->DrawString("VOICE TALENT", GetMenuX(), m_nYPos + GetMenuItemSpacing()*44);
	GetBitmapFont()->DrawString("TMNT", GetMenuX(), m_nYPos + GetMenuItemSpacing()*45);

	GetBitmapFont()->DrawString("ORIGINAL MUSIC BY", GetMenuX(), m_nYPos + GetMenuItemSpacing()*47);
	GetBitmapFont()->DrawString("VANILLA ICE", GetMenuX(), m_nYPos + GetMenuItemSpacing()*48);
	GetBitmapFont()->DrawString("DAVID ROCK", GetMenuX(), m_nYPos + GetMenuItemSpacing()*49);
	GetBitmapFont()->DrawString("ROBERTO ESPINOSA", GetMenuX(), m_nYPos + GetMenuItemSpacing()*50);

	GetBitmapFont()->DrawString("PROJECT MANAGEMENT", GetMenuX(), m_nYPos + GetMenuItemSpacing()*52);
	GetBitmapFont()->DrawString("ROBERT MARTINEZ", GetMenuX(), m_nYPos + GetMenuItemSpacing()*53);

	GetBitmapFont()->DrawString("LEAD TESTER", GetMenuX(), m_nYPos + GetMenuItemSpacing()*55);
	GetBitmapFont()->DrawString("BARBECUE STUDIOS", GetMenuX(), m_nYPos + GetMenuItemSpacing()*56);

	GetBitmapFont()->DrawString("ART SUPPORT", GetMenuX(), m_nYPos + GetMenuItemSpacing()*58);
	GetBitmapFont()->DrawString("CHRIS JAHOSKY", GetMenuX(), m_nYPos + GetMenuItemSpacing()*59);

	GetBitmapFont()->DrawString("GENERAL MANAGER", GetMenuX(), m_nYPos + GetMenuItemSpacing()*61);
	GetBitmapFont()->DrawString("ROBERT MARTINEZ", GetMenuX(), m_nYPos + GetMenuItemSpacing()*62);

	GetBitmapFont()->DrawString("DOCUMENTATION", GetMenuX(), m_nYPos + GetMenuItemSpacing()*64);
	GetBitmapFont()->DrawString("MATT DI MATTEO", GetMenuX(), m_nYPos + GetMenuItemSpacing()*65);
	GetBitmapFont()->DrawString("RAMON JOHANNESSEN", GetMenuX(), m_nYPos + GetMenuItemSpacing()*66);
	GetBitmapFont()->DrawString("JOSE SIERRA", GetMenuX(), m_nYPos + GetMenuItemSpacing()*67);

	GetBitmapFont()->DrawString("PACKAGE DESIGN", GetMenuX(), m_nYPos + GetMenuItemSpacing()*69);
	GetBitmapFont()->DrawString("MATT DI MATTEO", GetMenuX(), m_nYPos + GetMenuItemSpacing()*70);
	GetBitmapFont()->DrawString("RAMON JOHANNESSEN", GetMenuX(), m_nYPos + GetMenuItemSpacing()*71);
	GetBitmapFont()->DrawString("JOSE SIERRA", GetMenuX(), m_nYPos + GetMenuItemSpacing()*72);

	GetBitmapFont()->DrawString("TECHNICAL OFFICER", GetMenuX(), m_nYPos + GetMenuItemSpacing()*74);
	GetBitmapFont()->DrawString("MATT DI MATTEO", GetMenuX(), m_nYPos + GetMenuItemSpacing()*75);

	GetBitmapFont()->DrawString("GAMEPLAY OFFICER", GetMenuX(), m_nYPos + GetMenuItemSpacing()*77);
	GetBitmapFont()->DrawString("RAMON JOHANNESSEN", GetMenuX(), m_nYPos + GetMenuItemSpacing()*78);

	GetBitmapFont()->DrawString("INTERFACE OFFICER", GetMenuX(), m_nYPos + GetMenuItemSpacing() * 80);
	GetBitmapFont()->DrawString("JOSE SIERRA", GetMenuX(), m_nYPos + GetMenuItemSpacing() * 81);

	GetBitmapFont()->DrawString("PROJECT OFFICER", GetMenuX(), m_nYPos + GetMenuItemSpacing() * 83);
	GetBitmapFont()->DrawString("JOSE SIERRA", GetMenuX(), m_nYPos + GetMenuItemSpacing() * 84);

	GetBitmapFont()->DrawString("SPECIAL THANKS", GetMenuX(), m_nYPos + GetMenuItemSpacing() * 86);
	GetBitmapFont()->DrawString("TO EVERYONE WHO WAITED", GetMenuX(), m_nYPos + GetMenuItemSpacing() * 87);
	GetBitmapFont()->DrawString("AND THOSE WHO HELPED", GetMenuX(), m_nYPos + GetMenuItemSpacing() * 88);
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

	m_fTimer += fElapsedtime;

	if(m_fTimer > 3.5f)
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
		else if(GetBGImageID() == GetAssets()->aCMbgID6)
		{
			SetBGImageID(GetAssets()->aCMbgID);
			SetBGHeight(207);
			SetBGWidth(480);
			CenterBGImage();
		}
		m_fTimer = 0.0f;
	}
	
	if(m_fTimer < 2.0f)
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

	m_nYPos += (int)(m_fYVelocity * fElapsedtime);
}