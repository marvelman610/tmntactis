#include "BaseMenuState.h"
#include "Assets.h"
#include "Game.h"
#include "CSGD_TextureManager.h"
#include "CSGD_FModManager.h"
#include "CSGD_Direct3D.h"
#include "CSGD_DirectInput.h"
#include "BitmapFont.h"

CBaseMenuState::CBaseMenuState()
{

}
CBaseMenuState::~CBaseMenuState()
{

}
void CBaseMenuState::Render()
{
	m_pTM->DrawWithZSort(m_nBGImageID, m_nBGX, m_nBGY, 1.0f, 1.0f, 1.0f, NULL, 0.0f, 0.0f, 0.0f, D3DCOLOR_XRGB(150, 150, 150));
}

void CBaseMenuState::Update(float fElapsedTime)
{

}

void CBaseMenuState::Enter()
{
	m_pAssets		= CAssets::GetInstance();
	m_pDI			= CSGD_DirectInput::GetInstance();
	m_pTM			= CSGD_TextureManager::GetInstance();
	m_nScreenWidth	= CGame::GetInstance()->GetScreenWidth();
	m_nScreenHeight = CGame::GetInstance()->GetScreenHeight();
	m_pBitmapFont   = CBitmapFont::GetInstance();
	m_pGame			= CGame::GetInstance();
	m_pFMOD			= CSGD_FModManager::GetInstance();

	m_nMenuClick	= m_pAssets->aMMmenuMoveSnd;
	m_nMenuItemSpacing = 45;
}

bool CBaseMenuState::Input(float elapsedTime, POINT mousePt)
{
	if (m_pDI->KeyPressed(DIK_DOWN) || m_pDI->KeyPressed(DIK_UP))
	{
		if (m_pFMOD->IsSoundPlaying(m_nMenuClick))
		{
			m_pFMOD->StopSound(m_nMenuClick);
			m_pFMOD->ResetSound(m_nMenuClick);
		}
		m_pFMOD->PlaySound(m_nMenuClick);
		if(!m_pFMOD->SetVolume(m_nMenuClick, m_pGame->GetSFXVolume()))
			MessageBox(0, "VOLUME NOT SET", "ERROR", MB_OK);
	}
	return true;
}

void CBaseMenuState::Exit()
{
	if (m_pAssets)
		m_pAssets = NULL;
	if (m_pTM)
		m_pTM = NULL;
	if (m_pDI)
		m_pDI = NULL;
	if (m_pBitmapFont)
		m_pBitmapFont = NULL;
	if (m_pFMOD)
		m_pFMOD = NULL;
	if(m_pGame)
		m_pGame = NULL;
}

void CBaseMenuState::CenterBGImage()
{
	m_nBGX = (m_nScreenWidth >> 1) - (m_nImageWidth >> 1);
	m_nBGY = (m_nScreenHeight >> 1) - (m_nImageHeight >> 1);
}