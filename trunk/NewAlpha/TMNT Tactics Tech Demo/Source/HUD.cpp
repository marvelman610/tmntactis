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
#include "Assets.h"
#include "Battlemap.h"

CHUD::CHUD(void)
{
	m_pPlayer = CPlayer::GetInstance();
	m_pBitmapFont = CBitmapFont::GetInstance();
	m_pTM = CSGD_TextureManager::GetInstance();

	m_nImageID = -1;
}

CHUD::~CHUD(void)
{
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
		//******************************************
		//MODIFIED CODE TO USE ARRAY OF TURTLE POINTER INSTEAD OF INDIVIDUAL TURTLES
		if(m_pPlayer->GetTurtles()[LEONARDO] != NULL && m_pPlayer->GetTurtles()[DONATELLO] != NULL && 
			m_pPlayer->GetTurtles()[RAPHAEL] != NULL && m_pPlayer->GetTurtles()[MIKEY] != NULL)
		{

			//if(m_pPlayer->GetTurtles()[LEONARDO]->GetActive() == true && (CBattleMap::GetInstance()->GetCurrActive() == LEONARDO))
				//do the lerp to draw the health bar at set position and ap bar 

			//else if(m_pPlayer->GetTurtles()[DONATELLO]->GetActive() == true && (CBattleMap::GetInstance()->GetCurrActive() == DONATELLO))
				//do the lerp to draw the health bar at the set position and ap bar

			//else if(m_pPlayer->GetTurtles()[RAPHAEL]->GetActive() == true && (CBattleMap::GetInstance()->GetCurrActive() == RAPHAEL))
				//do the lerp to draw the health bar at the set position and ap bar
		
			//else if(m_pPlayer->GetTurtles()[MIKEY]->GetActive() == true && (CBattleMap::GetInstance()->GetCurrActive() == MIKEY))
				//do the lerp to draw the health bar at the set position and ap bar

		}
		break;
		/*if(m_pPlayer->GetLeonardo() != NULL && m_pPlayer->GetDonatello() != NULL && m_pPlayer->GetRaphael() != NULL && m_pPlayer->GetMikey() != NULL)
		{
			if(m_pPlayer->GetLeonardo()->GetActive() == true)
				m_nImageID = m_pTM->LoadTexture("Resources/Images/VG_Leonardo.png", D3DCOLOR_XRGB(255,255,255));	
			else if(m_pPlayer->GetDonatello()->GetActive() == true)
				m_nImageID = m_pTM->LoadTexture("Resources/Images/VG_Donatello.png", D3DCOLOR_XRGB(255,255,255));
			else if(m_pPlayer->GetRaphael()->GetActive() == true)
				m_nImageID = m_pTM->LoadTexture("Resources/Images/VG_Raphael.png", D3DCOLOR_XRGB(255,255,255));
			else if(m_pPlayer->GetMikey()->GetActive() == true)
				m_nImageID = m_pTM->LoadTexture("Resources/Images/VG_Michelangelo.png", D3DCOLOR_XRGB(255,255,255));
		}
		break;*/
	}
}
void CHUD::Render()
{
	//change alpha to lower when over objects
	if(m_pPlayer->GetTurtles()[MIKEY] != NULL && (CBattleMap::GetInstance()->GetCurrActive() == MIKEY))
	{
		m_pTM->Draw(CAssets::GetInstance()->aMikeyHUDFaceID, 0,0,1.0f,1.0f,NULL, 0.0f,0.0f,0.0f, D3DCOLOR_ARGB(100,255,255,255));
	}
	else if(m_pPlayer->GetTurtles()[LEONARDO] != NULL && (CBattleMap::GetInstance()->GetCurrActive() == LEONARDO))
	{
		m_pTM->Draw(CAssets::GetInstance()->aLeonardoHUDID, 0,0,1.0f,1.0f,NULL, 0.0f,0.0f, 0.0f, D3DCOLOR_ARGB(100,255,255,255));
	}
	else if(m_pPlayer->GetTurtles()[DONATELLO] != NULL && (CBattleMap::GetInstance()->GetCurrActive() == DONATELLO))
	{
		m_pTM->Draw(CAssets::GetInstance()->aDonatelloHUDID, 0,0,1.0f,1.0f,NULL,0.0f,0.0f,0.0f,D3DCOLOR_ARGB(100,255,255,255));
	}
	else if(m_pPlayer->GetTurtles()[RAPHAEL] != NULL && (CBattleMap::GetInstance()->GetCurrActive() == RAPHAEL))
	{
		m_pTM->Draw(CAssets::GetInstance()->aRaphaelHUDID, 0,0,1.0f,1.0f,NULL,0.0f,0.0f,0.0f,D3DCOLOR_ARGB(100,255,255,255));
	}

	//draw hud menu if player turn active
	
}

