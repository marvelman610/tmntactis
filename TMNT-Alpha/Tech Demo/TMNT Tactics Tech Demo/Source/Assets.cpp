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
	aBitmapFont2ID		= m_pTM->LoadTexture("Resources/Images/VG_BitmapFont2.png", D3DCOLOR_XRGB(0,0,0));
	aBitmapFont3ID		= m_pTM->LoadTexture("Resources/Images/VG_BitmapFont3.png", D3DCOLOR_XRGB(0,0,0));
	aMenuCursorImageID	= m_pTM->LoadTexture("Resources/Images/VG_menuCursor.bmp", D3DCOLOR_XRGB(255,255,255));
	aMouseMagGlassID	= m_pTM->LoadTexture("Resources/Images/VG_cursorMagGlass.png", D3DCOLOR_XRGB(255,255,255));
	aMousePointerID		= m_pTM->LoadTexture("Resources/Images/VG_cursorPointer.png", D3DCOLOR_XRGB(255,255,255));
	//aMouseGrabID		= m_pTM->LoadTexture("Resources/Images/VG_cursorGrab.png", D3DCOLOR_XRGB(255,255,255));
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// Battle Map IDs
	aBMcursorID			= m_pTM->LoadTexture("Resources/Images/VG_SquareCursor.png", D3DCOLOR_XRGB(0,0,0));
	aBMgreenSquareID	= m_pTM->LoadTexture("Resources/Images/VG_SquareSelectedGreen.png", D3DCOLOR_XRGB(0,0,0));
	aBMcurrSelectedArrowID=m_pTM->LoadTexture("Resources/Images/VG_currArrow.png", D3DCOLOR_XRGB(255,255,255));
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// Main Menu IDs
	aMMBGimageID		= m_pTM->LoadTexture("Resources/Images/VG_MainMenuBG.png", D3DCOLOR_XRGB(255,255,255));

	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// How To Player Menu IDs
	//aHTPMBGimageID		= m_pTM->LoadTexture("Resources/Images/VG_HowToPlayMenuBG.png");

	//////////////////////////////////////////////////////////////////////////
	// HUD Image ID's
	//aHUDBoxID			= m_pTM->LoadTexture("Resources/Images/VG_HUDBox.png", D3DCOLOR_XRGB(255,255,255));
	aGreenHealthBarID	= m_pTM->LoadTexture("Resources/Images/VG_HealthBarGreen.png", D3DCOLOR_XRGB(255,255,255));
	aRedHealthBarID		= m_pTM->LoadTexture("Resources/Images/VG_HealthBarRed.png", D3DCOLOR_XRGB(255,255,255));
	aBlueHealthBarID	= m_pTM->LoadTexture("Resources/Images/VG_HealthBarBlue.png", D3DCOLOR_XRGB(255,255,255));

	//////////////////////////////////////////////////////////////////////////
	// HUD Character Faces IDs
	aMikeyHUDFaceID		= m_pTM->LoadTexture("Resources/Images/VG_MichelangeloHUD.png", D3DCOLOR_XRGB(255,255,255));
	aLeonardoHUDID		= m_pTM->LoadTexture("Resources/Images/VG_LeonardoHUD.png", D3DCOLOR_XRGB(255,255,255));
	aDonatelloHUDID		= m_pTM->LoadTexture("Resources/Images/VG_DonatelloHUD.png", D3DCOLOR_XRGB(255,255,255));
	aRaphaelHUDID		= m_pTM->LoadTexture("Resources/Images/VG_RaphaelHUD.png", D3DCOLOR_XRGB(255,255,255));

	//////////////////////////////////////////////////////////////////////////
	// temp ninja
	aNinjaID			= m_pTM->LoadTexture("Resources/Images/VG_Ninja.png", D3DCOLOR_XRGB(0,0,0));
}