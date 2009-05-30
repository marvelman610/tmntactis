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
//#include "Animation.h"
//#include "HUD.h"
//#include "WorldMap.h"

// Constructor
CGamePlayState::CGamePlayState(void)
{
	m_pCurrentMenuState = NULL;
	m_pBattleMap = NULL;
	//m_pHUD = NULL;
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
	//m_pHUD = HUD::GetInstance();
	//m_pWorldMap = WorldMap::GetInstance();

	m_pBattleMap = new CBattleMap("Resources/MapInfo/VG_MyMap.dat");
	//m_nCurrentMap = ;
}

// Exit
void CGamePlayState::Exit(void)
{
	if(m_pBattleMap)
	{
		delete m_pBattleMap;
	}
// 	if(m_pWorldMap)
// 	{
// 		m_pWorldMap->Release();
// 		m_pWorldMap = NULL;
// 	}
// 	if(m_pHUD)
// 	{
// 		m_pHUD->Release();
// 		m_pHUD = NULL;
// 	}
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
}
// Render
void CGamePlayState::Render(void)
{
	CSGD_TextureManager* pTM = CSGD_TextureManager::GetInstance();
	CSGD_Direct3D* pD3D = CSGD_Direct3D::GetInstance();

	m_pBattleMap->Render();
}