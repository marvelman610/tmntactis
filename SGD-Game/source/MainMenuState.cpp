////////////////////////////////////////////////////////////////////////////
//	File Name	:	"CMainMenuState.cpp"
//
//	Author		:	Matt DiMatteo (MD)
//
//	Purpose		:	menu state derived from the base state
//					used to access other states
////////////////////////////////////////////////////////////////////////////
#include "MainMenuState.h"
#include"SGD Wrappers\CSGD_WaveManager.h"
#include"CCreditsState.h"

void CMainMenuState::Enter() 
{
	m_pDI = CSGD_DirectInput::GetInstance();
	m_pD3D = CSGD_Direct3D::GetInstance();
	m_pTM = CSGD_TextureManager::GetInstance();

	//create texture
	m_nImageID = m_pTM->LoadTexture("resource/graphics/MaD_HandGun.bmp");

	howto1 = m_pTM->LoadTexture("resource/graphics/MaD_howto1.png");
	howto2 = m_pTM->LoadTexture("resource/graphics/MaD_howto2.png");
	page = 0;

	gunsound = CSGD_WaveManager::GetInstance()->LoadWave("resource/sound/MaD_Tankshot.wav");

	font.Load("resource/graphics/MaD_Font.png",20,20,20);

	background = m_pTM->LoadTexture("resource/graphics/MaD_PunisherBK.png");

	m_nCurrentMenuSelection = 0;
}
bool CMainMenuState::Input() 
{

	if(page == 0)
	{
		if(m_pDI->KeyPressed(DIK_DOWN))
		{
			CSGD_WaveManager::GetInstance()->Play(gunsound);
			CSGD_WaveManager::GetInstance()->SetVolume(gunsound, CGame::GetInstance()->GetSFXVolume());
			CSGD_WaveManager::GetInstance()->SetPan(gunsound, (CGame::GetInstance()->GetPanning()-50)*2);
			if(m_nCurrentMenuSelection<5)
				m_nCurrentMenuSelection++;
			else
				m_nCurrentMenuSelection = 0;
		}
		if(m_pDI->KeyPressed(DIK_UP))
		{
			CSGD_WaveManager::GetInstance()->Play(gunsound);
			CSGD_WaveManager::GetInstance()->SetVolume(gunsound, CGame::GetInstance()->GetSFXVolume());
			CSGD_WaveManager::GetInstance()->SetPan(gunsound, (CGame::GetInstance()->GetPanning()-50)*2);

			if(m_nCurrentMenuSelection>0)
				m_nCurrentMenuSelection--;
			else
				m_nCurrentMenuSelection = 5;
		}
	}
	if(m_pDI->KeyPressed(DIK_RETURN))
	{
		switch(m_nCurrentMenuSelection)
		{
		case 0:
			CGame::GetInstance()->ChangeState(CGamePlayState::GetInstance());
			break;
		case 1:
			CGame::GetInstance()->ChangeState(COptionsState::GetInstance());
			break;
		case 2:
			page++;
			if(page > 2)
			{
				page = 0;
				m_nCurrentMenuSelection = 0;
			}
			break;
		case 3:
			CGame::GetInstance()->ChangeState(CHighScoreState::GetInstance());
			break;
		case 4:
			CGame::GetInstance()->ChangeState(CCreditsState::GetInstance());
			break;
		case 5:
			CGame::GetInstance()->SetIsRunning(false);
			break;
		}
	}
	return true;
}
void CMainMenuState::Update()
{
}
void CMainMenuState::Render()
{
	if(page == 0)
	{
		m_pTM->Draw(background,0,75);
		m_pTM->Draw(m_nImageID,475, 250+(m_nCurrentMenuSelection*50),0.5f,0.5f);

		font.DrawString("THE PUNISHER: WAR ZONE",100,50,1.5f);

		font.DrawString("PLAY",550,250);
		font.DrawString("OPTIONS",550,300);
		font.DrawString("HOW TO PLAY",550,350);
		font.DrawString("HIGH SCORES",550,400);
		font.DrawString("CREDITS",550,450);
		font.DrawString("EXIT",550,500);
	}
	else if(page == 1)
		m_pTM->Draw(howto1,0,0);
	else if(page == 2)
		m_pTM->Draw(howto2,0,0);

}
void CMainMenuState::Exit()
{
	if(m_pTM){m_pTM->UnloadTexture(m_nImageID);}
	CSGD_WaveManager::GetInstance()->UnloadWave(gunsound);

	font.Unload();
}
CMainMenuState* CMainMenuState::GetInstance()
{
	static CMainMenuState instance;
	return &instance;
}

