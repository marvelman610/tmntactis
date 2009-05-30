#include "BaseMenuState.h"

void CBaseMenuState::Render()
{
	m_pD3D->Draw(m_nBGImageID, 0, 0);
}

void CBaseMenuState::Update(float fElapsedTime)
{

}

void CBaseMenuState::Enter()
{
	m_pAssets		= CAsset::GetInstance();
	m_pBitmapFont   = CBitmapFont::GetInstance();
	m_pTM			= CSGD_TextureManager::GetInstance();
	m_pFMOD			= CGame::GetInstance()->GetFMODSystem();
	m_pDI			= CSGD_DirectInput::GetInstance();
}

void CBaseMenuState::Exit()
{
	if (m_pAssets)
		m_pAssets = NULL;
	if (m_pBitmapFont)
		m_pBitmapFont = NULL;
	if (m_pTM)
		m_pTM = NULL;
	if (m_pFMOD)
		m_pFMOD = NULL;
	if (m_pDI)
		m_pDI = NULL;
}
