////////////////////////////////////////////////////////
//   File Name	:	"HUD.cpp"
//
//   Author		:	Matthew Di Matteo (MD)
//
//   Purpose	:	This class encompasses all menus to 
//					appear on the screen during gameplay.
////////////////////////////////////////////////////////

#include "HUD.h"
#include "BitmapFont.h"
#include "Player.h"
#include "CSGD_TextureManager.h"
//#include "CSGD_Direct3D.h"
#include "Turtle.h"

CHUD::CHUD(void)
{
	m_pPlayer = CPlayer::GetInstance();
	m_pBitmapFont = CBitmapFont::GetInstance();
	m_pTM = CSGD_TextureManager::GetInstance();

	m_nImageID = -1;
	m_nImageID1 = -1;//m_pTM->LoadTexture("Resources/Images/VG_HealthBarRed.png", D3DCOLOR_XRGB(255,255,255));
	m_nImageID2 = -1;//m_pTM->LoadTexture("Resources/Images/VG_HealthBarGreen.png", D3DCOLOR_XRGB(255,255,255));
	m_nImageID3 = -1;//m_pTM->LoadTexture("Resources/Images/VG_HealthBarBlue.png", D3DCOLOR_XRGB(255,255,255));
	m_nImageID4 = -1;//m_pTM->LoadTexture("Resources/Images/VG_HUDBox.png", D3DCOLOR_XRGB(255,255,255));
}

CHUD::~CHUD(void)
{
	if(m_nImageID4 > -1)
	{
		m_pTM->UnloadTexture(m_nImageID4);
	}
	if(m_nImageID3 > -1)
	{
		m_pTM->UnloadTexture(m_nImageID3);
	}
	if(m_nImageID2 > -1)
	{
		m_pTM->UnloadTexture(m_nImageID2);
	}
	if(m_nImageID1 > -1)
	{
		m_pTM->UnloadTexture(m_nImageID1);
	}
	if(m_nImageID > -1)
	{
		m_pTM->UnloadTexture(m_nImageID);
	}
}
CHUD* CHUD::GetInstance()
{
	static CHUD instance;
	return &instance;
}
void CHUD::Update(float fElapsedTime)
{
	switch(m_pPlayer->GetCurrStage())
	{
	case WORLD_MAP:
		break;
	case BATTLE_MAP:
		if(m_pPlayer->GetLeonardo() != NULL && m_pPlayer->GetDonatello() != NULL && m_pPlayer->GetRaphael() != NULL && m_pPlayer->GetMikey() != NULL)
		{
			if(m_pPlayer->GetLeonardo()->GetActive() == true)
			{
				m_nImageID = m_pTM->LoadTexture("Resources/Images/VG_Leonardo.png", D3DCOLOR_XRGB(255,255,255));	
			}
			else if(m_pPlayer->GetDonatello()->GetActive() == true)
			{
				m_nImageID = m_pTM->LoadTexture("Resources/Images/VG_Donatello.png", D3DCOLOR_XRGB(255,255,255));
			}
			else if(m_pPlayer->GetRaphael()->GetActive() == true)
			{
				m_nImageID = m_pTM->LoadTexture("Resources/Images/VG_Raphael.png", D3DCOLOR_XRGB(255,255,255));
			}
			else if(m_pPlayer->GetMikey()->GetActive() == true)
			{
				m_nImageID = m_pTM->LoadTexture("Resources/Images/VG_Michelangelo.png", D3DCOLOR_XRGB(255,255,255));
			}
		}
		break;
	}
}
void CHUD::Render()
{

}

void CHUD::LoadHUD(void)
{
	m_nImageID1 = m_pTM->LoadTexture("Resources/Images/VG_HealthBarRed.png", D3DCOLOR_XRGB(255,255,255));
	m_nImageID2 = m_pTM->LoadTexture("Resources/Images/VG_HealthBarGreen.png", D3DCOLOR_XRGB(255,255,255));
	m_nImageID3 = m_pTM->LoadTexture("Resources/Images/VG_HealthBarBlue.png", D3DCOLOR_XRGB(255,255,255));
	m_nImageID4 = m_pTM->LoadTexture("Resources/Images/VG_HUDBox.png", D3DCOLOR_XRGB(255,255,255));

}
