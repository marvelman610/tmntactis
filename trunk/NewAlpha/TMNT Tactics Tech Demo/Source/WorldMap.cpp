#include "WorldMap.h"
#include "Assets.h"
#include "BitmapFont.h"
#include "CSGD_TextureManager.h"

CWorldMap::CWorldMap()
{
	m_pTM = CSGD_TextureManager::GetInstance();
	m_pBitmapFont = CBitmapFont::GetInstance();
	m_pAssets	= CAssets::GetInstance();
}

CWorldMap::~CWorldMap()
{

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