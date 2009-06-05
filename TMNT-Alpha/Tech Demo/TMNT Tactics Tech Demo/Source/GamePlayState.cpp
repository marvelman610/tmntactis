//////////////////////////////////////////////////////////////////////////
//	Filename	:	GamePlayState.cpp
//
//	Author		:	Ramon Johannessen (RJ)
//
//	Purpose		:	To define the CGamePlayState class. This state handles
//					all gameplay code.
//////////////////////////////////////////////////////////////////////////
#include "GamePlayState.h"
#include "CSGD_DirectInput.h"
#include "CSGD_Direct3D.h"
#include "CSGD_TextureManager.h"
#include "BaseMenuState.h"
#include "BattleMap.h"
#include "MainMenuState.h"
#include "Game.h"
#include "Player.h"
//#include "Animation.h"
#include "HUD.h"
//#include "WorldMap.h"
#include"ObjectManager.h"

// Constructor
CGamePlayState::CGamePlayState(void)
{
	m_pCurrentMenuState = NULL;
	m_pBattleMap = NULL;
	m_pPlayer = NULL;
	m_pHUD = NULL;
	//m_pWorldMap  = NULL;

	m_nCurrentMap = 0;
}

// Destructor
CGamePlayState::~CGamePlayState(void)
{
}

// Enter
void CGamePlayState::Enter(void)
{
	//m_pCurrentMenuState = BaseMenuState::GetInstance();
	m_pHUD = CHUD::GetInstance();
	//m_pWorldMap = WorldMap::GetInstance();
	m_pPlayer = CPlayer::GetInstance();
	m_pBattleMap = CBattleMap::GetInstance();

	// TODO:: will enter battle map once player goes into a battle
	//			from the world map
	m_pBattleMap->Enter("Resources/MapInfo/VG_ZSortTest.dat");

	//m_nCurrentMap = ;
}

// Exit
void CGamePlayState::Exit(void)
{
	if(m_pBattleMap)
	{
		m_pBattleMap->Exit();
	}
	if (m_pPlayer)
		m_pPlayer = NULL;
// 	if(m_pWorldMap)
// 	{
// 		m_pWorldMap->Release();
// 		m_pWorldMap = NULL;
// 	}
 	if(m_pHUD)
 	{
 		//m_pHUD->Release();
 		m_pHUD = NULL;
 	}
	if(m_pCurrentMenuState)
	{
	//	m_pCurrentMenuState->Release();
		m_pCurrentMenuState = NULL;
	}
}

// Instance
CGamePlayState* CGamePlayState::GetInstance(void)
{
	static CGamePlayState instance;
	return &instance;
}

// Messages
// void MessageProc(CBaseMessage* pMsg)
// {
// 	switch( pMsg->GetMsgID() )
// 	{
// 	default:
// 		break;
// 	}
// }


// Input
bool CGamePlayState::Input(float fElapsedTime, POINT mousePt)
{
	CSGD_DirectInput* pDI = CSGD_DirectInput::GetInstance();

	if (pDI->KeyPressed(DIK_ESCAPE))
	{
		CGame::GetInstance()->ChangeState(CMainMenuState::GetInstance());
		return true;
	}

	m_pBattleMap->Input(fElapsedTime, mousePt);

	return true;
}
// Update
void CGamePlayState::Update(float fElapsedTime)
{
	m_pBattleMap->Update(fElapsedTime);
	ObjectManager::GetInstance()->UpdateObjects(fElapsedTime);
}
// Render
void CGamePlayState::Render(void)
{
// 	CSGD_TextureManager* pTM = CSGD_TextureManager::GetInstance();
// 	CSGD_Direct3D* pD3D = CSGD_Direct3D::GetInstance();

	ObjectManager::GetInstance()->RenderObjects();
	m_pBattleMap->Render();

}