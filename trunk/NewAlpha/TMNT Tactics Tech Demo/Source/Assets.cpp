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

#ifdef _DEBUG
	const bool bLoadMusic = false;
#else
	const bool bLoadMusic = true;
#endif

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
// 	int timeTravel = m_pFMOD->LoadSound("Resources/Sounds/VG_timeTravel.mp3");
// 	m_pFMOD->PlaySound(timeTravel);

	m_bLoading = true;
	CSGD_Direct3D *d3d = CSGD_Direct3D::GetInstance();
	d3d->Clear(0,0,0);
	d3d->DeviceBegin();
	d3d->SpriteBegin();
	CBitmapFont::GetInstance()->DrawStringAutoCenter("TMNT",		CGame::GetInstance()->GetScreenWidth(), 20, 0.09f, 1.5f, D3DCOLOR_XRGB(0, 255, 0));
	CBitmapFont::GetInstance()->DrawStringAutoCenter("TACTICS",		CGame::GetInstance()->GetScreenWidth(), 100, 0.09f, 1.5f, D3DCOLOR_XRGB(0, 255, 0));
	CBitmapFont::GetInstance()->DrawStringAutoCenter("LOADING...",  CGame::GetInstance()->GetScreenWidth(), 650, 0.0f, 1.5f, D3DCOLOR_XRGB(0, 255, 0));
	d3d->SpriteEnd();
	d3d->DeviceEnd();
	d3d->Present();
	//////////////////////////////////////////////////////////////////////////
	// globally used IDs
	aBitmapFontID		= m_pTM->LoadTexture("Resources/Images/VG_BitmapFont.png", D3DCOLOR_XRGB(0,0,0));
	//aBitmapFont2ID		= m_pTM->LoadTexture("Resources/Images/VG_BitmapFont2.png", D3DCOLOR_XRGB(0,0,0));
	//aBitmapFont3ID		= m_pTM->LoadTexture("Resources/Images/VG_BitmapFont3.png", D3DCOLOR_XRGB(0,0,0));
	aMenuCursorImageID	= m_pTM->LoadTexture("Resources/Images/VG_menuCursor.bmp", D3DCOLOR_XRGB(255,255,255));
	aMouseMagGlassID	= m_pTM->LoadTexture("Resources/Images/VG_cursorMagGlass.png", D3DCOLOR_XRGB(255,255,255));
	aMouseAttackID		= m_pTM->LoadTexture("Resources/Images/VG_cursorAttack.png", D3DCOLOR_XRGB(255,255,255));
	aMousePointerID		= m_pTM->LoadTexture("Resources/Images/VG_cursorPointer.png", D3DCOLOR_XRGB(255,255,255));
	aMouseMoveID		= m_pTM->LoadTexture("Resources/Images/VG_cursorMove.png", D3DCOLOR_XRGB(255,255,255));
	//aMouseGrabID		= m_pTM->LoadTexture("Resources/Images/VG_cursorGrab.png", D3DCOLOR_XRGB(255,255,255));
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	//	box IDs
	//aBpointerID			= m_pTM->LoadTexture("Resources/Images/VG_boxPointer.png", D3DCOLOR_XRGB(255,255,255));
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// Battle Map IDs
	//aBMbgID				= m_pTM->LoadTexture("Resources/Images/VG_MountainBG.png", D3DCOLOR_XRGB(255,255,255));
	aBMcursorID			= m_pTM->LoadTexture("Resources/Images/VG_SquareCursor.png", D3DCOLOR_XRGB(0,0,0));
	aBMgreenSquareID	= m_pTM->LoadTexture("Resources/Images/VG_SquareSelectedGreen.png", D3DCOLOR_XRGB(0,0,0));
	aBMcurrSelectedArrowID=m_pTM->LoadTexture("Resources/Images/VG_currArrow.png", D3DCOLOR_XRGB(255,255,255));
	aBMcurrTargetArrowID= m_pTM->LoadTexture("Resources/Images/VG_currTargetArrow.png", D3DCOLOR_XRGB(255,255,255));
	aBMactionBoxID		= m_pTM->LoadTexture("Resources/Images/VG_actionBox.png", D3DCOLOR_XRGB(255,255,255));
	aBMskillBoxID		= m_pTM->LoadTexture("Resources/Images/VG_skillBoxBG.png", D3DCOLOR_XRGB(255,255,255));
	aBMvictoryID		= m_pTM->LoadTexture("Resources/Images/VG_victory.png", D3DCOLOR_XRGB(255,255,255));
	aBMdefeatID			= m_pTM->LoadTexture("Resources/Images/VG_defeat.png", D3DCOLOR_XRGB(255,255,255));
	//////////////////////////////////////////////////////////////////////////
	aBMqteLeftID		= m_pTM->LoadTexture("Resources/Images/VG_qteLEFT.png", D3DCOLOR_XRGB(1,1,1));
	aBMqteRightID		= m_pTM->LoadTexture("Resources/Images/VG_qteRIGHT.png", D3DCOLOR_XRGB(1,1,1));
	aBMqteDownID		= m_pTM->LoadTexture("Resources/Images/VG_qteDOWN.png", D3DCOLOR_XRGB(1,1,1));
	aBMqteUpID			= m_pTM->LoadTexture("Resources/Images/VG_qteUP.png", D3DCOLOR_XRGB(1,1,1));
	//////////////////////////////////////////////////////////////////////////
	aBMMikeBGID			= m_pTM->LoadTexture("Resources/Images/VG_bgMike.png");
	aBMLeoBGID			= m_pTM->LoadTexture("Resources/Images/VG_bgLeo.png");
	aBMRaphBGID			= m_pTM->LoadTexture("Resources/Images/VG_bgRaph.png");
	aBMDonBGID			= m_pTM->LoadTexture("Resources/Images/VG_bgDon.png");

	if(bLoadMusic)
	{
		aBMeggSmackSnd		= m_pFMOD->LoadSound("Resources/Sounds/VG_eggSmack.mp3");
		aBMpainSnd			= m_pFMOD->LoadSound("Resources/Sounds/VG_painScream.mp3");
		aBMdeathSnd			= m_pFMOD->LoadSound("Resources/Sounds/VG_dying.mp3");
		aBMpunchSnd1		= m_pFMOD->LoadSound("Resources/Sounds/VG_punch.mp3");
		aBMpunchSnd2		= m_pFMOD->LoadSound("Resources/Sounds/VG_punchKick.mp3");
		aBMgrenadeSnd		= m_pFMOD->LoadSound("Resources/Sounds/VG_grenadeExplosion.mp3");
		aBMpickupSnd		= m_pFMOD->LoadSound("Resources/Sounds/VG_pickup.mp3");
		aBMninjaPizzaSnd	= m_pFMOD->LoadSound("Resources/Sounds/VG_ninjaPizza.mp3");
		aBMcowabungaSnd		= m_pFMOD->LoadSound("Resources/Sounds/VG_cowabunga.mp3");
		aBMninjaAttackSnd	= m_pFMOD->LoadSound("Resources/Sounds/VG_ninjaAttack.mp3");
		aBMwillBeOthersSnd	= m_pFMOD->LoadSound("Resources/Sounds/VG_willBeOthers.mp3");
		aBMfootstepsSnd		= m_pFMOD->LoadSound("Resources/Sounds/VG_footsteps.mp3", FMOD_LOOP_NORMAL);
		//////////////////////////////////////////////////////////////////////////
	}

	//////////////////////////////////////////////////////////////////////////
	// WorldMap IDs
	aWMmapID			= m_pTM->LoadTexture("Resources/Images/VG_worldMap.png", D3DCOLOR_XRGB(255,255,255));
	aWMtempleID			= m_pTM->LoadTexture("Resources/Images/VG_templeIcon.png", D3DCOLOR_XRGB(255,255,255));
	aWMlockID			= m_pTM->LoadTexture("Resources/Images/VG_lock.png", D3DCOLOR_XRGB(255,255,255));
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// Main Menu IDs
	aMMBGimageID		= m_pTM->LoadTexture("Resources/Images/VG_MainMenuBG.png", D3DCOLOR_XRGB(255,255,255));

	aMMmenuClickSnd		= m_pFMOD->LoadSound("Resources/Sounds/VG_menuClick.mp3");
	aMMmenuMoveSnd		= m_pFMOD->LoadSound("Resources/Sounds/VG_menuSelectMove.mp3");
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// Options Menu IDs
	aOMbgID			= m_pTM->LoadTexture("Resources/Images/VG_OptionsMenuBG.png");
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// Credits IDs
	//////////////////////////////////////////////////////////////////////////
	aCMbgID		= m_pTM->LoadTexture("Resources/Images/VG_TutorialMenuBG.png");
	aCMbgID2	= m_pTM->LoadTexture("Resources/Images/VG_Game1.png");
	aCMbgID3	= m_pTM->LoadTexture("Resources/Images/VG_Game2.png");
	aCMbgID4	= m_pTM->LoadTexture("Resources/Images/VG_Game3.png");
	aCMbgID5	= m_pTM->LoadTexture("Resources/Images/VG_Game4.png");
	aCMbgID6	= m_pTM->LoadTexture("Resources/Images/VG_Game5.png");

	//////////////////////////////////////////////////////////////////////////
	// How To Play Menu IDs
	//aHTPMBGimageID		= m_pTM->LoadTexture("Resources/Images/VG_HowToPlayMenuBG.png");
	aHTPMAttackID	= m_pTM->LoadTexture("Resources/Images/VG_HowToAttack.png");
	aHTPMItemID		= m_pTM->LoadTexture("Resources/Images/VG_HowToItem.png");
	aHTPMMoveID		= m_pTM->LoadTexture("Resources/Images/VG_HowToMove.png");
	aHTPMSelectID	= m_pTM->LoadTexture("Resources/Images/VG_HowToSelect.png");
	aHTPMSpecialID	= m_pTM->LoadTexture("Resources/Images/VG_HowToSpecial.png");

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
	aShredderHUDID		= m_pTM->LoadTexture("Resources/Images/VG_ShredderHUD2.png", D3DCOLOR_XRGB(255,255,255));

	///////////////////////////////////////////////////////////////////////
	//Items and weapons
	aEggID = m_pTM->LoadTexture("Resources/Images/VG_blackEgg.png");
	aGrenadoID = m_pTM->LoadTexture("Resources/Images/VG_grenado.png");
	aPizzaID = m_pTM->LoadTexture("Resources/Images/VG_fullPizza.png");

	//atempID = m_pTM->LoadTexture("Resources/Images/VG_tempweapon.png");
	aSwordID = m_pTM->LoadTexture("Resources/Images/VG_Katana.png");
	aSaiID= m_pTM->LoadTexture("Resources/Images/VG_Sais.png");
	aNunchakuID= m_pTM->LoadTexture("Resources/Images/VG_nunchaku.png");
	aStaffID= m_pTM->LoadTexture("Resources/Images/VG_staff.png");
	

	////////////////////////////////////////////////////////////////////////
	//Particles
	aGlowParticle = aFireParticle = m_pTM->LoadTexture("Resources/Images/VG_Particle.png");
	aBloodParticle = m_pTM->LoadTexture("Resources/Images/VG_Blood.png");
	aSmokeParticle = m_pTM->LoadTexture("Resources/Images/VG_Cloud.png");
	aGlassParticle = m_pTM->LoadTexture("Resources/Images/VG_Glass.png");
	
	////////////////////////////////////////////////////////////////////////
	// Achievements
	aAvenged = m_pTM->LoadTexture("Resources/Images/VG_Avenged.png", D3DCOLOR_XRGB(255,0,0));
	aBreakGame = m_pTM->LoadTexture("Resources/Images/VG_BreakGame.png", D3DCOLOR_XRGB(255,0,0));
	aCowabunga = m_pTM->LoadTexture("Resources/Images/VG_Cowabunga.png", D3DCOLOR_XRGB(255,0,0));
	aFlipNow = m_pTM->LoadTexture("Resources/Images/VG_FlipsNow.png", D3DCOLOR_XRGB(255,0,0));
	aLessonInPain = m_pTM->LoadTexture("Resources/Images/VG_LessonInPain.png", D3DCOLOR_XRGB(255,0,0));
	aMegaKill = m_pTM->LoadTexture("Resources/Images/VG_MegaKill.png", D3DCOLOR_XRGB(255,0,0));
	aPartyTime = m_pTM->LoadTexture("Resources/Images/VG_PartyTime.png", D3DCOLOR_XRGB(255,0,0));
	aQuickFingers = m_pTM->LoadTexture("Resources/Images/VG_QuickFingers.png", D3DCOLOR_XRGB(255,0,0));
	aStrikeHard = m_pTM->LoadTexture("Resources/Images/VG_StrikeHard.png", D3DCOLOR_XRGB(255,0,0));
	aTrueNinja = m_pTM->LoadTexture("Resources/Images/VG_TrueNinja.png", D3DCOLOR_XRGB(255,0,0));

	//bg
	aAchievementBGID = m_pTM->LoadTexture("Resources/Images/VG_AchievementBG.png");

	if (bLoadMusic)
	{
		aStrikeHardSnd		= m_pFMOD->LoadSound("Resources/Sounds/VG_strikeHard.mp3");
		aLessonInPainSnd	= m_pFMOD->LoadSound("Resources/Sounds/VG_pain101.mp3");
		//////////////////////////////////////////////////////////////////////////
		
		aBMarcadeMusicID	= m_pFMOD->LoadSound("Resources/Sounds/VG_musicArcade.mp3", FMOD_LOOP_NORMAL);
// 		aBMsimusaMusicID;
// 		aBMsinaroMusicID;
// 		aBMiwamiMusicID;
// 		aBMyamatoMusicID;
		aMMmusicID			= m_pFMOD->LoadSound("Resources/Sounds/VG_themeSong.mp3", FMOD_LOOP_NORMAL);
		aWMworldMapMusicID	= m_pFMOD->LoadSound("Resources/Sounds/VG_worldMapMusic.mp3", FMOD_LOOP_NORMAL);
		aOMmusicSnd			= m_pFMOD->LoadSound("Resources/Sounds/VG_musicCredits.mp3", FMOD_LOOP_NORMAL);
		aBMninjarap			= m_pFMOD->LoadSound("Resources/Sounds/VG_NINJARAP.mp3", FMOD_LOOP_NORMAL);
		aHTPsound			= m_pFMOD->LoadSound("Resources/Sounds/VG_ZombieOutbreak.mp3", FMOD_LOOP_NORMAL);
		aMinimalKesoID		= m_pFMOD->LoadSound("Resources/Sounds/VG_Minimal.mp3", FMOD_LOOP_NORMAL);
	}
	m_bLoading = false;
}