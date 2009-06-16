#include "WorldMap.h"
#include "Assets.h"
#include "BitmapFont.h"
#include "CSGD_TextureManager.h"
#include "CSGD_DirectInput.h"
#include "Game.h"
#include "MainMenuState.h"

CWorldMap::CWorldMap()
{
	m_pTM = CSGD_TextureManager::GetInstance();
	m_pBitmapFont = CBitmapFont::GetInstance();
	m_pDI	= CSGD_DirectInput::GetInstance();
	m_pAssets	= CAssets::GetInstance();
	m_nMapImageID = m_pAssets->aWMmapID;
}

CWorldMap::~CWorldMap()
{
	m_pTM = NULL;
	m_pBitmapFont = NULL;
	m_pAssets = NULL;
}

CWorldMap* CWorldMap::GetInstance()
{
	static CWorldMap instance;
	return &instance;
}

void CWorldMap::Enter()
{

}

void CWorldMap::Exit()
{

}

void CWorldMap::Render()
{
	m_pTM->DrawWithZSort(m_nMapImageID, 0, 0, 1.0f, 1.0f, 1.0f);
	m_pTM->DrawWithZSort(m_pAssets->aMousePointerID, m_ptMouse.x-10, m_ptMouse.y-3, 0.0f);
}

void CWorldMap::Update(float fElapsedTime)
{

}

bool CWorldMap::Input(float fElapsedTime, POINT mouse)
{
	if (m_pDI->KeyPressed(DIK_ESCAPE))
		CGame::GetInstance()->ChangeState(CMainMenuState::GetInstance());

	m_ptMouse = mouse;

	return true;
}