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
#include "CSGD_Direct3D.h"
#include "Turtle.h"
#include "Assets.h"
#include "Battlemap.h"

CHUD::CHUD(void)
{
	m_pPlayer = CPlayer::GetInstance();
	m_pBitmapFont = CBitmapFont::GetInstance();
	m_pTM = CSGD_TextureManager::GetInstance();
	m_pD3D = CSGD_Direct3D::GetInstance();

	m_rMikeyHP.top = 0;
	m_rMikeyHP.bottom = 16;
	m_rMikeyHP.right = 104;
	m_rMikeyHP.left = 0;

	m_rMikeyAP.top = 0;
	m_rMikeyAP.bottom = 16;
	m_rMikeyAP.right = 104;
	m_rMikeyAP.left = 0;

	m_rLeoHP.top = 0;
	m_rLeoHP.bottom = 16;
	m_rLeoHP.right = 104;
	m_rLeoHP.left = 0;

	m_rLeoAP.top = 0;
	m_rLeoAP.bottom = 16;
	m_rLeoAP.right = 104;
	m_rLeoAP.left = 0;

	m_rDonHP.top = 0;
	m_rDonHP.bottom = 16;
	m_rDonHP.right = 104;
	m_rDonHP.left = 0;

	m_rDonAP.top = 0;
	m_rDonAP.bottom = 16;
	m_rDonAP.right = 104;
	m_rDonAP.left = 0;

	m_rRaphHP.top = 0;
	m_rRaphHP.bottom = 16;
	m_rRaphHP.right = 104;
	m_rRaphHP.left = 0;

	m_rRaphAP.top = 0;
	m_rRaphAP.bottom = 16;
	m_rRaphAP.right = 104;
	m_rRaphAP.left = 0;

	m_rEnemyHP.bottom = 16;
	m_rEnemyHP.left = 0;
	m_rEnemyHP.right = 104;
	m_rEnemyHP.top = 0;

	m_rEnemyAP.bottom = 16;
	m_rEnemyAP.left = 0;
	m_rEnemyAP.right = 104;
	m_rEnemyAP.top = 0;

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
		//******************************************
		//MODIFIED CODE TO USE ARRAY OF TURTLE POINTER INSTEAD OF INDIVIDUAL TURTLES
		if(m_pPlayer->GetTurtles()[LEONARDO] != NULL && m_pPlayer->GetTurtles()[DONATELLO] != NULL && 
			m_pPlayer->GetTurtles()[RAPHAEL] != NULL && m_pPlayer->GetTurtles()[MIKEY] != NULL)
		{
			float healthwidth = 104.0f *((float)m_pPlayer->GetTurtles()[MIKEY]->GetHealth() / (float)m_pPlayer->GetTurtles()[MIKEY]->GetMaxHealth());
			float  apwidth = 104.0f * ((float)m_pPlayer->GetTurtles()[MIKEY]->GetCurrAP() / (float)m_pPlayer->GetTurtles()[MIKEY]->GetBaseAP()); 
			m_rMikeyHP.right = (LONG)healthwidth;
			m_rMikeyAP.right = (LONG)apwidth;

			healthwidth = 104.0f * ( (float)m_pPlayer->GetTurtles()[LEONARDO]->GetHealth() / (float)m_pPlayer->GetTurtles()[LEONARDO]->GetMaxHealth() );
			apwidth = 104.0f * ( (float)m_pPlayer->GetTurtles()[LEONARDO]->GetCurrAP() / (float)m_pPlayer->GetTurtles()[LEONARDO]->GetBaseAP() );
			m_rLeoHP.right = (LONG)healthwidth;
			m_rLeoAP.right = (LONG)apwidth;

			healthwidth = 104.0f * ( (float)m_pPlayer->GetTurtles()[DONATELLO]->GetHealth() / (float)m_pPlayer->GetTurtles()[DONATELLO]->GetMaxHealth() );
			apwidth = 104.0f * ( (float)m_pPlayer->GetTurtles()[DONATELLO]->GetCurrAP() / (float)m_pPlayer->GetTurtles()[DONATELLO]->GetBaseAP() );
			m_rDonHP.right = (LONG)healthwidth;
			m_rDonAP.right = (LONG)apwidth;

			healthwidth = 104.0f * ( (float)m_pPlayer->GetTurtles()[RAPHAEL]->GetHealth() / (float)m_pPlayer->GetTurtles()[RAPHAEL]->GetMaxHealth() );
			apwidth = 104.0f * ( (float)m_pPlayer->GetTurtles()[RAPHAEL]->GetCurrAP() / (float)m_pPlayer->GetTurtles()[RAPHAEL]->GetBaseAP() );
			m_rRaphHP.right = (LONG)healthwidth;
			m_rRaphAP.right = (LONG)apwidth;
		}
		if(CBattleMap::GetInstance()->GetCurrTarget() > -1 &&CBattleMap::GetInstance()->GetCurrEnemyTarget()->GetHealth() > 0)
		{
			float hpWidth = 104.0f * ( (float)CBattleMap::GetInstance()->GetCurrEnemyTarget()->GetHealth() / (float)CBattleMap::GetInstance()->GetCurrEnemyTarget()->GetMaxHealth() );
			float apWidth = 104.0f * ( (float)CBattleMap::GetInstance()->GetCurrEnemyTarget()->GetCurrAP() / (float)CBattleMap::GetInstance()->GetCurrEnemyTarget()->GetBaseAP() );		
			m_rEnemyHP.left = (float)(104 - hpWidth);
			m_rEnemyAP.left = (float)(104 - apWidth);
		}

}
void CHUD::Render()
{
	//change alpha to lower when over objects
	if(m_pPlayer->GetTurtles()[MIKEY] != NULL && (CBattleMap::GetInstance()->GetCurrActive() == MIKEY))
	{
		m_pTM->Draw(CAssets::GetInstance()->aMikeyHUDFaceID, 0,0,1.0f,1.0f,NULL, 0.0f,0.0f,0.0f, D3DCOLOR_ARGB(100,255,255,255));
		m_pTM->Draw(CAssets::GetInstance()->aGreenHealthBarID, 144,49,1.0f,1.0f,&m_rMikeyHP,0.0f,0.0f,0.0f,D3DCOLOR_ARGB(100,255,255,255));
		m_pTM->Draw(CAssets::GetInstance()->aBlueHealthBarID, 144, 98, 1.0f,1.0f,&m_rMikeyAP,0.0f,0.0f,0.0f,D3DCOLOR_ARGB(100,255,255,255));
	}
	else if(m_pPlayer->GetTurtles()[LEONARDO] != NULL && (CBattleMap::GetInstance()->GetCurrActive() == LEONARDO))
	{
		m_pTM->Draw(CAssets::GetInstance()->aLeonardoHUDID, 0,0,1.0f,1.0f,NULL, 0.0f,0.0f, 0.0f, D3DCOLOR_ARGB(100,255,255,255));
		m_pTM->Draw(CAssets::GetInstance()->aGreenHealthBarID, 170, 80, 1.0f,1.0f,&m_rLeoHP,0.0f,0.0f,0.0f,D3DCOLOR_ARGB(100,255,255,255));
		m_pTM->Draw(CAssets::GetInstance()->aBlueHealthBarID, 170, 140, 1.0f, 1.0f,&m_rLeoAP, 0.0f,0.0f,0.0f, D3DCOLOR_ARGB(100,255,255,255));	
	}
	else if(m_pPlayer->GetTurtles()[DONATELLO] != NULL && (CBattleMap::GetInstance()->GetCurrActive() == DONATELLO))
	{
		m_pTM->Draw(CAssets::GetInstance()->aDonatelloHUDID, 0,0,1.0f,1.0f,NULL,0.0f,0.0f,0.0f,D3DCOLOR_ARGB(100,255,255,255));
		m_pTM->Draw(CAssets::GetInstance()->aGreenHealthBarID, 175, 72, 1.0f,1.0f,&m_rDonHP,0.0f,0.0f,0.0f,D3DCOLOR_ARGB(100,255,255,255));
		m_pTM->Draw(CAssets::GetInstance()->aBlueHealthBarID, 175,118,1.0f,1.0f,&m_rDonAP,0.0f,0.0f,0.0f,D3DCOLOR_ARGB(100,255,255,255));
	}
	else if(m_pPlayer->GetTurtles()[RAPHAEL] != NULL && (CBattleMap::GetInstance()->GetCurrActive() == RAPHAEL))
	{
		m_pTM->Draw(CAssets::GetInstance()->aRaphaelHUDID, 0,0,1.0f,1.0f,NULL,0.0f,0.0f,0.0f,D3DCOLOR_ARGB(100,255,255,255));
		m_pTM->Draw(CAssets::GetInstance()->aGreenHealthBarID, 156, 40, 1.0f, 1.0f,&m_rRaphHP, 0.0f,0.0f,0.0f,D3DCOLOR_ARGB(100,255,255,255));
		m_pTM->Draw(CAssets::GetInstance()->aBlueHealthBarID, 156,83,1.0f,1.0f,&m_rRaphAP,0.0f,0.0f,0.0f,D3DCOLOR_ARGB(100,255,255,255));
	}

	//draw hud menu if player turn active


	//enemy hud
	if( CBattleMap::GetInstance()->GetCurrTarget() > -1 )
	{
		m_pTM->Draw(CAssets::GetInstance()->aFootClanHUDID, 540,0,1.0f,1.0f, NULL, 0.0f, 0.0f, 0.0f,D3DCOLOR_ARGB(150,255,255,255));
		//get current target 
		m_pTM->Draw(CAssets::GetInstance()->aGreenHealthBarID, 795, 61, 1.0f,1.0f,&m_rEnemyHP, 0.0f,0.0f,0.0f, D3DCOLOR_ARGB(100, 255,255,255));
		m_pTM->Draw(CAssets::GetInstance()->aBlueHealthBarID, 795, 105,1.0f,1.0f,&m_rEnemyAP, 0.0f,0.0f,0.0f, D3DCOLOR_ARGB(100,255,255,255));
	}
	
}

