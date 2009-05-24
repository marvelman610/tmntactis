//////////////////////////////////////////////////////////////////////////
//	Filename	:	Assets.cpp
//
//	Author		:	Ramon Johannessen (RJ)
//
//	Purpose		:	To define the CAssets class. Will load all assets used
//					within the game and hold the IDs. A singleton.
//////////////////////////////////////////////////////////////////////////
#include "Assets.h"
#include "CSGD_TextureManager.h"
#include "CSGD_Direct3D.h"

CAssets* CAssets::GetInstance()
{
	static CAssets instance;
	return &instance;
}

CAssets::CAssets()
{
	m_pTM = CSGD_TextureManager::GetInstance();
	LoadAssets();
}
CAssets::~CAssets()
{

}


void CAssets::LoadAssets()
{
	//aBMgroundTilesID	= m_pTM->LoadTexture("groundTiles.bmp", D3DCOLOR_XRGB(255,255,255));
	//aBMtreeTilesID		= m_pTM->LoadTexture("trees.bmp", D3DCOLOR_XRGB(255,255,255));
	aBMcursorID			= m_pTM->LoadTexture("Images/Cursor.png", D3DCOLOR_XRGB(0,0,0));
}