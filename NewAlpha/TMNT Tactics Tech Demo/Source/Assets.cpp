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
#include "CSGD_FModManager.h"
#include "CSGD_Direct3D.h"
#include "BitmapFont.h"
#include "Game.h"

CAssets* CAssets::GetInstance()
{
	static CAssets instance;
	return &instance;
}

CAssets::CAssets()
{
	m_pTM = CSGD_TextureManager::GetInstance();
	m_pFMOD = CSGD_FModManager::GetInstance();
	LoadAssets();
}
CAssets::~CAssets()
{
	if(m_pFMOD)
		m_pFMOD = NULL;
}

void CAssets::LoadAssets()
{
	m_bLoading = true;
	CSGD_Direct3D *d3d = CSGD_Direct3D::GetInstance();
	d3d->Clear(0,0,0);
	d3d->DeviceBegin();
	d3d->SpriteBegin();
	CBitmapFont::GetInstance()->DrawStringAutoCenter("LOADING...", CGame::GetInstance()->GetScreenWidth(), 650, 0.0f, 1.5f, D3DCOLOR_XRGB(255, 0, 0));
	d3d->SpriteEnd();
	d3d->DeviceEnd();
	d3d->Present();
	//////////////////////////////////////////////////////////////////////////
	// globally used IDs
	aBitmapFontID		= m_pTM->LoadTexture("Resources/Images/VG_BitmapFont.png", D3DCOLOR_XRGB(0,0,0));
	aBitmapFont2ID		= m_pTM->LoadTexture("Resources/Images/VG_BitmapFont2.png", D3DCOLOR_XRGB(0,0,0));
	aBitmapFont3ID		= m_pTM->LoadTexture("Resources/Images/VG_BitmapFont3.png", D3DCOLOR_XRGB(0,0,0));
	aMenuCursorImageID	= m_pTM->LoadTexture("Resources/Images/VG_menuCursor.bmp", D3DCOLOR_XRGB(255,255,255));
	aMouseMagGlassID	= m_pTM->LoadTexture("Resources/Images/VG_cursorMagGlass.png", D3DCOLOR_XRGB(255,255,255));
	aMouseAttackID		= m_pTM->LoadTexture("Resources/Images/VG_cursorAttack.png", D3DCOLOR_XRGB(255,255,255));
	aMousePointerID		= m_pTM->LoadTexture("Resources/Images/VG_cursorPointer.png", D3DCOLOR_XRGB(255,255,255));
	aMouseMoveID		= m_pTM->LoadTexture("Resources/Images/VG_cursorMove.png", D3DCOLOR_XRGB(255,255,255));
	//aMouseGrabID		= m_pTM->LoadTexture("Resources/Images/VG_cursorGrab.png", D3DCOLOR_XRGB(255,255,255));
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	//	box IDs
	aBpointerID			= m_pTM->LoadTexture("Resources/Images/VG_boxPointer.png", D3DCOLOR_XRGB(255,255,255));
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// Battle Map IDs
	//aBMbgID				= m_pTM->LoadTexture("Resources/Images/VG_battleMapBG.png");
	aBMcursorID			= m_pTM->LoadTexture("Resources/Images/VG_SquareCursor.png", D3DCOLOR_XRGB(0,0,0));
	aBMgreenSquareID	= m_pTM->LoadTexture("Resources/Images/VG_SquareSelectedGreen.png", D3DCOLOR_XRGB(0,0,0));
	aBMcurrSelectedArrowID=m_pTM->LoadTexture("Resources/Images/VG_currArrow.png", D3DCOLOR_XRGB(255,255,255));
	aBMcurrTargetArrowID= m_pTM->LoadTexture("Resources/Images/VG_currTargetArrow.png", D3DCOLOR_XRGB(255,255,255));
	aBMactionBoxID		= m_pTM->LoadTexture("Resources/Images/VG_actionBox.png", D3DCOLOR_XRGB(255,255,255));
	aBMskillBoxID		= m_pTM->LoadTexture("Resources/Images/VG_skillBoxBG.png", D3DCOLOR_XRGB(255,255,255));

	aBMarcadeMusicID	= m_pFMOD->LoadSound("Resources/Sounds/VG_musicArcade.mp3", FMOD_LOOP_NORMAL);
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// Main Menu IDs
	aMMBGimageID		= m_pTM->LoadTexture("Resources/Images/VG_MainMenuBG.png", D3DCOLOR_XRGB(255,255,255));

	aMMmenuClickSnd		= m_pFMOD->LoadSound("Resources/Sounds/VG_menuClick.mp3");
	aMMmenuMoveSnd		= m_pFMOD->LoadSound("Resources/Sounds/VG_menuSelectMove.mp3");
	aMMmusicID			= m_pFMOD->LoadSound("Resources/Sounds/VG_themeSong.mp3", FMOD_LOOP_NORMAL);
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// Options Menu IDs
	aOMbgID			= m_pTM->LoadTexture("Resources/Images/VG_OptionsMenuBG.png");
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// Tutorial IDs
	aTutorialID		= m_pTM->LoadTexture("Resources/Images/VG_TutorialMenuBG.png");
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// Credits IDs
	aCMmusicID		= m_pFMOD->LoadSound("Resources/Sounds/VG_musicCredits.mp3", FMOD_LOOP_NORMAL);
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
	aFootClanHUDID		= m_pTM->LoadTexture("Resources/Images/VG_FootClanHUD.png", D3DCOLOR_XRGB(255,255,255));
	aShredderHUDID		= m_pTM->LoadTexture("Resources/Images/VG_ShredderHUD.png", D3DCOLOR_XRGB(255,255,255));

	///////////////////////////////////////////////////////////////////////
	//Items and weapons
	//temp item
	aItemID = m_pTM->LoadTexture("Resources/Images/VG_tempItem.png");

	////////////////////////////////////////////////////////////////////////
	//Particles
	aGlowParticle = aFireParticle = m_pTM->LoadTexture("Resources/Images/VG_Particle.png");
	aBloodParticle = m_pTM->LoadTexture("Resources/Images/VG_Blood.png");
	aSmokeParticle = m_pTM->LoadTexture("Resources/Images/VG_Cloud.png");

	m_bLoading = false;
}