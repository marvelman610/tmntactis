#include "CGamePlayState.h"
#include "DirectInput.h"
#include "TextureManager.h"
// Constructor
CGamePlayState::CGamePlayState(void)
{
	m_pCurrentMenuState = NULL;
	m_pHUD = NULL;
	m_pWorldMap  = NULL;
	m_pBattleMap = NULL;
	m_nCurrentMap = 0;
}

// Destructor
CGamePlayState::~CGamePlayState(void)
{
}

// Enter
void CGamePlayState::Enter(void)
{
	m_pCurrentMenuState = BaseMenuState::GetInstance();
	m_pHUD = HUD::GetInstance();
	m_pWorldMap = WorldMap::GetInstance();
	m_pBattleMap = BattleMap::GetInstance();
	//m_nCurrentMap = ;

}

// Exit
void CGamePlayState::Exit(void)
{
	if(m_pBattleMap)
	{
		m_pBattleMap->Release();
		m_pBattleMap = NULL;
	}
	if(m_pWorldMap)
	{
		m_pWorldMap->Release();
		m_pWorldMap = NULL;
	}
	if(m_pHUD)
	{
		m_pHUD->Release();
		m_pHUD = NULL;
	}
	if(m_pCurrentMenuState)
	{
		m_pCurrentMenuState->Release();
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
void MessageProc(CBaseMessage* pMsg)
{
	switch( pMsg->GetMsgID() )
	{
	default:
		break;
	}
}


// Input
bool CGamePlayState::Input(void)
{
	DirectInput* pDI = DirectInput::GetInstance();
	return true;
}
// Update
void CGamePlayState::Update(float fElapsedTime)
{

}
// Render
void CGamePlayState::Render(void)
{
	TextureManager* pTM = TextureManager::GetInstance();
	Direct3D* pD3D = Direct3D::GetInstance();

	pD3D->Clear(0,0,128);
	
	pD3D->DeviceBegin();
	pD3D->SpriteBegin();
	pD3D->LineBegin();

	//draw

	pD3D->LineEnd();
	pD3D->SpriteEnd();
	pD3D->DeviceEnd();

	pD3D->Present();
}