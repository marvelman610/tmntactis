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
#include "Ninja.h"
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

	m_rShredderAP.bottom = 16;
	m_rShredderAP.left = 0;
	m_rShredderAP.right = 104;
	m_rShredderAP.top = 0;

	m_rShredderHP.bottom = 16;
	m_rShredderHP.left = 0;
	m_rShredderHP.right = 104;
	m_rShredderHP.top = 0;

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
			m_rEnemyHP.right = (LONG)hpWidth;
			m_rEnemyAP.right = (LONG)apWidth;
		}
		if(CBattleMap::GetInstance()->GetHasBoss() == true)
		{
			if(CBattleMap::GetInstance()->GetBoss() != NULL)
			{
				float hpWidth = 104.0f * ( (float)CBattleMap::GetInstance()->GetBoss()->GetHealth() / (float)CBattleMap::GetInstance()->GetBoss()->GetMaxHealth() );
				float apWidth = 104.0f * ( (float)CBattleMap::GetInstance()->GetBoss()->GetCurrAP() / (float)CBattleMap::GetInstance()->GetBoss()->GetBaseAP());
				m_rShredderHP.right = (LONG)hpWidth;
				m_rShredderAP.right = (LONG)apWidth;
			}
		}

}

void CHUD::DrawFootClanHUD()
{
	m_pTM->DrawWithZSort(CAssets::GetInstance()->aFootClanHUDID,	540, 0, 0.4f, 1.0f,1.0f, NULL, 0.0f, 0.0f, 0.0f,D3DCOLOR_ARGB(150,255,255,255));
	//get current target 
	m_pTM->DrawWithZSort(CAssets::GetInstance()->aGreenHealthBarID, 895, 61, 0.4f, -1.0f,1.0f,&m_rEnemyHP, 0.0f,0.0f,0.0f, D3DCOLOR_ARGB(100, 255,255,255));
	m_pTM->DrawWithZSort(CAssets::GetInstance()->aBlueHealthBarID,	895, 105,0.4f, -1.0f,1.0f,&m_rEnemyAP, 0.0f,0.0f,0.0f, D3DCOLOR_ARGB(100,255,255,255));
	char szLVL[3]; sprintf_s(szLVL, "%i", CBattleMap::GetInstance()->GetCurrEnemyTarget()->GetLevel());
	CBitmapFont::GetInstance()->DrawString(szLVL, 923, 140, 0.05f, 0.5f, D3DCOLOR_XRGB(0, 255, 0));
	char szEXP[8]; sprintf_s(szEXP, "%i", CBattleMap::GetInstance()->GetCurrEnemyTarget()->GetExperience());
	CBitmapFont::GetInstance()->DrawString(szEXP, 999, 140, 0.05f, 0.5f, D3DCOLOR_XRGB(0, 255, 0));
	char szHP[8]; sprintf_s(szHP, "%i", CBattleMap::GetInstance()->GetCurrEnemyTarget()->GetHealth());
	CBitmapFont::GetInstance()->DrawString(szHP, 782, 78, 0.05f, 0.5f, D3DCOLOR_XRGB(0, 255, 0));
	char szAP[3]; sprintf_s(szAP, "%i", CBattleMap::GetInstance()->GetCurrEnemyTarget()->GetCurrAP());
	CBitmapFont::GetInstance()->DrawString(szAP, 782, 125, 0.05f, 0.5f, D3DCOLOR_XRGB(0, 255, 0));
}
void CHUD::DrawMikeyHUD()
{
	m_pTM->DrawWithZSort(CAssets::GetInstance()->aMikeyHUDFaceID,	0,	0, 0.4f, 1.0f,1.0f,NULL, 0.0f,0.0f,0.0f, D3DCOLOR_ARGB(150,255,255,255));
	m_pTM->DrawWithZSort(CAssets::GetInstance()->aGreenHealthBarID, 156, 59, 0.4f,1.0f,1.0f,&m_rMikeyHP,0.0f,0.0f,0.0f,D3DCOLOR_ARGB(100,255,255,255));
	m_pTM->DrawWithZSort(CAssets::GetInstance()->aBlueHealthBarID,	156, 113, 0.4f, 1.0f,1.0f,&m_rMikeyAP,0.0f,0.0f,0.0f,D3DCOLOR_ARGB(100,255,255,255));
	char szLVL[3]; sprintf_s(szLVL, "%i", m_pPlayer->GetTurtles()[MIKEY]->GetLevel());
	CBitmapFont::GetInstance()->DrawString(szLVL, 52,145,0.05f,0.5f, D3DCOLOR_XRGB(0, 255, 0));
	char szEXP[8]; sprintf_s(szEXP, "%i", m_pPlayer->GetTurtles()[MIKEY]->GetExperience());
	CBitmapFont::GetInstance()->DrawString(szEXP, 138,145,0.05f,0.5f, D3DCOLOR_XRGB(0, 255, 0));
	char szHP[8]; sprintf_s(szHP, "%i", m_pPlayer->GetTurtles()[MIKEY]->GetHealth());
	CBitmapFont::GetInstance()->DrawString(szHP, 211,79,0.05f, 0.5f, D3DCOLOR_XRGB(0, 255, 0));
	char szAP[3]; sprintf_s(szAP, "%i", m_pPlayer->GetTurtles()[MIKEY]->GetCurrAP());
	CBitmapFont::GetInstance()->DrawString(szAP, 211,135,0.05f, 0.5f, D3DCOLOR_XRGB(0, 255, 0));
}

void CHUD::DrawLeoHUD()
{
	m_pTM->DrawWithZSort(CAssets::GetInstance()->aLeonardoHUDID, 0, 0, 0.4f,1.0f,1.0f,NULL, 0.0f,0.0f, 0.0f, D3DCOLOR_ARGB(150,255,255,255));
	m_pTM->DrawWithZSort(CAssets::GetInstance()->aGreenHealthBarID, 180, 70, 0.4f, 1.0f,1.0f,&m_rLeoHP,0.0f,0.0f,0.0f,D3DCOLOR_ARGB(100,255,255,255));
	m_pTM->DrawWithZSort(CAssets::GetInstance()->aBlueHealthBarID, 180, 125, 0.4f, 1.0f, 1.0f,&m_rLeoAP, 0.0f,0.0f,0.0f, D3DCOLOR_ARGB(100,255,255,255));
	char szLVL[3]; sprintf_s(szLVL, "%i", m_pPlayer->GetTurtles()[LEONARDO]->GetLevel());
	CBitmapFont::GetInstance()->DrawString(szLVL, 47,155,0.05f,0.5f, D3DCOLOR_XRGB(0, 255, 0));
	char szEXP[8]; sprintf_s(szEXP, "%i", m_pPlayer->GetTurtles()[LEONARDO]->GetExperience());
	CBitmapFont::GetInstance()->DrawString(szEXP, 162,155,0.05f,0.5f, D3DCOLOR_XRGB(0, 255, 0));
	char szHP[8]; sprintf_s(szHP, "%i", m_pPlayer->GetTurtles()[LEONARDO]->GetHealth());
	CBitmapFont::GetInstance()->DrawString(szHP, 235,95,0.05f, 0.5f, D3DCOLOR_XRGB(0, 255, 0));
	char szAP[3]; sprintf_s(szAP, "%i", m_pPlayer->GetTurtles()[LEONARDO]->GetCurrAP());
	CBitmapFont::GetInstance()->DrawString(szAP, 235,155,0.05f, 0.5f, D3DCOLOR_XRGB(0, 255, 0));
}

void CHUD::DrawDonHUD()
{
	m_pTM->DrawWithZSort(CAssets::GetInstance()->aDonatelloHUDID, 0,0, 0.4f,1.0f,1.0f,NULL,0.0f,0.0f,0.0f,D3DCOLOR_ARGB(150,255,255,255));
	m_pTM->DrawWithZSort(CAssets::GetInstance()->aGreenHealthBarID, 185, 77, 0.4f, 1.0f,1.0f,&m_rDonHP,0.0f,0.0f,0.0f,D3DCOLOR_ARGB(100,255,255,255));
	m_pTM->DrawWithZSort(CAssets::GetInstance()->aBlueHealthBarID, 185, 123, 0.4f,1.0f,1.0f,&m_rDonAP,0.0f,0.0f,0.0f,D3DCOLOR_ARGB(100,255,255,255));
	char szLVL[3]; sprintf_s(szLVL, "%i", m_pPlayer->GetTurtles()[DONATELLO]->GetLevel());
	CBitmapFont::GetInstance()->DrawString(szLVL, 56,145,0.05f,0.5f, D3DCOLOR_XRGB(0, 255, 0));
	char szEXP[8]; sprintf_s(szEXP, "%i", m_pPlayer->GetTurtles()[DONATELLO]->GetExperience());
	CBitmapFont::GetInstance()->DrawString(szEXP, 154,145,0.05f,0.5f, D3DCOLOR_XRGB(0, 255, 0));
	char szHP[8]; sprintf_s(szHP, "%i", m_pPlayer->GetTurtles()[DONATELLO]->GetHealth());
	CBitmapFont::GetInstance()->DrawString(szHP, 240,95,0.05f, 0.5f, D3DCOLOR_XRGB(0, 255, 0));
	char szAP[3]; sprintf_s(szAP, "%i", m_pPlayer->GetTurtles()[DONATELLO]->GetCurrAP());
	CBitmapFont::GetInstance()->DrawString(szAP, 240,142,0.05f, 0.5f, D3DCOLOR_XRGB(0, 255, 0));
}

void CHUD::DrawRaphHUD()
{
	m_pTM->DrawWithZSort(CAssets::GetInstance()->aRaphaelHUDID, 0,0, 0.4f,1.0f,1.0f,NULL,0.0f,0.0f,0.0f,D3DCOLOR_ARGB(150,255,255,255));
	m_pTM->DrawWithZSort(CAssets::GetInstance()->aGreenHealthBarID, 176, 60, 0.4f, 1.0f, 1.0f,&m_rRaphHP, 0.0f,0.0f,0.0f,D3DCOLOR_ARGB(100,255,255,255));
	m_pTM->DrawWithZSort(CAssets::GetInstance()->aBlueHealthBarID,  176, 103, 0.4f, 1.0f, 1.0f,&m_rRaphAP,0.0f,0.0f,0.0f,D3DCOLOR_ARGB(100,255,255,255));
	char szLVL[3]; sprintf_s(szLVL, "%i", m_pPlayer->GetTurtles()[RAPHAEL]->GetLevel());
	CBitmapFont::GetInstance()->DrawString(szLVL, 60, 131,0.05f,0.5f, D3DCOLOR_XRGB(0, 255, 0));
	char szEXP[8]; sprintf_s(szEXP, "%i",m_pPlayer->GetTurtles()[RAPHAEL]->GetExperience());
	CBitmapFont::GetInstance()->DrawString(szEXP, 147,131,0.05f,0.5f, D3DCOLOR_XRGB(0, 255, 0));
	char szHP[8]; sprintf_s(szHP, "%i", m_pPlayer->GetTurtles()[RAPHAEL]->GetHealth());
	CBitmapFont::GetInstance()->DrawString(szHP, 222,85,0.05f, 0.5f, D3DCOLOR_XRGB(0, 255, 0));
	char szAP[3]; sprintf_s(szAP, "%i", m_pPlayer->GetTurtles()[RAPHAEL]->GetCurrAP());
	CBitmapFont::GetInstance()->DrawString(szAP, 222,125,0.05f, 0.5f, D3DCOLOR_XRGB(0, 255, 0));
}

void CHUD::Render()
{
	//change alpha to lower when over objects
	if(CBattleMap::GetInstance()->GetPlayerTurn() == true)
	{
		if(m_pPlayer->GetTurtles()[MIKEY] != NULL && (CBattleMap::GetInstance()->GetCurrActive() == MIKEY))
		{
			DrawMikeyHUD();

		}
		else if(m_pPlayer->GetTurtles()[LEONARDO] != NULL && (CBattleMap::GetInstance()->GetCurrActive() == LEONARDO))
		{
			DrawLeoHUD();

		}
		else if(m_pPlayer->GetTurtles()[DONATELLO] != NULL && (CBattleMap::GetInstance()->GetCurrActive() == DONATELLO))
		{
			DrawDonHUD();

		}
		else if(m_pPlayer->GetTurtles()[RAPHAEL] != NULL && (CBattleMap::GetInstance()->GetCurrActive() == RAPHAEL))
		{
			DrawRaphHUD();

		}

		//draw hud menu if player turn active


		//enemy hud
		if(CBattleMap::GetInstance()->GetHasBoss() == true )
		{

			if( CBattleMap::GetInstance()->GetCurrTarget() > -1 && CBattleMap::GetInstance()->GetCurrEnemyTarget()->GetType() != OBJECT_BOSS)
			{
				DrawFootClanHUD();

			}

			if(CBattleMap::GetInstance()->GetBoss() != NULL && CBattleMap::GetInstance()->GetCurrTarget() > -1 && CBattleMap::GetInstance()->GetCurrEnemyTarget()->GetType() == OBJECT_BOSS)
			{
				m_pTM->DrawWithZSort(CAssets::GetInstance()->aShredderHUDID,	512, 0, 0.6f, 1.0f,1.0f,NULL, 0.0f,0.0f,0.0f,D3DCOLOR_ARGB(150, 255,255,255));
				m_pTM->DrawWithZSort(CAssets::GetInstance()->aGreenHealthBarID, 862, 60, 0.6f, -1.0f, 1.0f, &m_rEnemyHP, 0.0f,0.0f,0.0f,D3DCOLOR_ARGB(100,255,255,255));
				m_pTM->DrawWithZSort(CAssets::GetInstance()->aBlueHealthBarID,	862, 124, 0.6f, -1.0f, 1.0f, &m_rEnemyAP, 0.0f,0.0f,0.0f,D3DCOLOR_ARGB(100,255,255,255));
				//910 , 201 lvl - 982, 201 exp - 	758, 114hp	- 758, 185 ap
				char szLVL[3]; sprintf_s(szLVL, "%i", CBattleMap::GetInstance()->GetBoss()->GetLevel());
				CBitmapFont::GetInstance()->DrawString(szLVL, 910,176,0.05f, 0.5f);
				char szEXP[8]; sprintf_s(szEXP, "%i", CBattleMap::GetInstance()->GetBoss()->GetExperience());
				CBitmapFont::GetInstance()->DrawString(szEXP, 982,176,0.05f, 0.5f);
				char szHP[8]; sprintf_s(szHP, "%i", CBattleMap::GetInstance()->GetBoss()->GetHealth());
				CBitmapFont::GetInstance()->DrawString(szHP, 758,84,0.05f, 0.5f);
				char szAP[3]; sprintf_s(szAP, "%i", CBattleMap::GetInstance()->GetBoss()->GetCurrAP());
				CBitmapFont::GetInstance()->DrawString(szAP, 758,155,0.05f, 0.5f);
			}
		}
		else
		{
			if( CBattleMap::GetInstance()->GetCurrTarget() > -1 && CBattleMap::GetInstance()->GetCurrEnemyTarget()->GetType() != OBJECT_BOSS)
			{
				DrawFootClanHUD();
			}
		}
	}

	else
	{
		if(CBattleMap::GetInstance()->GetCurrMovingNinja() != NULL)
		{
			if(CBattleMap::GetInstance()->GetCurrTarget() > - 1)
			{
				DrawFootClanHUD();
			}

			switch(CBattleMap::GetInstance()->GetCurrMovingNinja()->GetTurtle())
			{
			case LEONARDO:
				{
					DrawLeoHUD();
				}
				break;
			case DONATELLO:
				{
					DrawDonHUD();
				}
				break;
			case RAPHAEL:
				{
					DrawRaphHUD();
				}
				break;
			case MIKEY:
				{
					DrawMikeyHUD();
				}
				break;
			}
		}
	}
	
}
