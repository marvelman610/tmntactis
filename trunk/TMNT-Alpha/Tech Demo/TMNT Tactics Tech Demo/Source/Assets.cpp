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
	//////////////////////////////////////////////////////////////////////////
	// globally used IDs
	aBitmapFontID		= m_pTM->LoadTexture("Resources/Images/VG_BitmapFont.png", D3DCOLOR_XRGB(0,0,0));
	aMenuCursorImageID	= m_pTM->LoadTexture("Resources/Images/VG_menuCursor.bmp", D3DCOLOR_XRGB(255,255,255));
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// Battle Map IDs
	aBMcursorID			= m_pTM->LoadTexture("Resources/Images/VG_Cursor.png", D3DCOLOR_XRGB(0,0,0));

	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// Main Menu IDs
	aMMBGimageID		= m_pTM->LoadTexture("Resources/Images/VG_MainMenuBG.png", D3DCOLOR_XRGB(255,255,255));

	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// How To Player Menu IDs
	//aHTPMBGimageID		= m_pTM->LoadTexture("Resources/Images/VG_HowToPlayMenuBG.png");
}