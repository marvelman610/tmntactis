//////////////////////////////////////////////////////////////////////////
//	Filename	:	Assets.h
//
//	Author		:	Ramon Johannessen (RJ)
//
//	Purpose		:	To define the CAssets class. Will load all assets used
//					within the game and hold the IDs. A singleton.
//////////////////////////////////////////////////////////////////////////
#ifndef ASSETS_H
#define ASSETS_H
//////////////////////////////////////////////////////////////////////////
// all assets will follow a naming convention to easily identify.
// For a background image used in the BattleMap class for instance, 
// the variable name would be aBM_bgID.
//
//		the template is a<all capital letters of class name minus the "C">_<what the image is>ID
//////////////////////////////////////////////////////////////////////////

class CSGD_FModManager;
class CSGD_TextureManager;
typedef int ASSET;

class CAssets
{
private:
	CSGD_TextureManager* m_pTM;
	CSGD_FModManager*	 m_pFMOD;

	bool m_bLoading;

	CAssets(void);
	~CAssets(void);
	CAssets(const CAssets&);
	CAssets& operator=(const CAssets&);
public:

	//////////////////////////////////////////////////////////////////////////
	//	Function	:	GetInstance
	//
	//	Purpose		:	Return the only instance of this object
	//////////////////////////////////////////////////////////////////////////
	static CAssets* GetInstance();

	//////////////////////////////////////////////////////////////////////////
	//	Function	:	LoadAssets()
	//
	//	Purpose		:	Load in all assets used throughout the entirety of the game
	//////////////////////////////////////////////////////////////////////////
	void LoadAssets();

	bool IsLoading()	const {return m_bLoading;}

	// all assets are public...no need to write accessors/mutators for all of them

	// Images common to multiple classes
	ASSET aMenuCursorImageID;
	ASSET aBitmapFontID;
	ASSET aBitmapFont2ID;
	ASSET aBitmapFont3ID;
	ASSET aMousePointerID;
	ASSET aMouseGrabID;
	ASSET aMouseAttackID;
	ASSET aMouseMagGlassID;
	ASSET aMouseMoveID;

	// box images
	ASSET aBpointerID;

	// BattleMap IDs
	ASSET aBMbgID;
	ASSET aBMgroundTilesID, aBMtreeTilesID;
	ASSET aBMcursorID;
	ASSET aBMgreenSquareID;
	ASSET aBMcurrSelectedArrowID;
	ASSET aBMcurrTargetArrowID;
	ASSET aBMactionBoxID;
	ASSET aBMskillBoxID;
	ASSET aBMarcadeMusicID;

	// WorldMap IDs
	ASSET aWMmapID;

	// Main Menu IDs
	ASSET aMMBGimageID;
	ASSET aMMmenuClickSnd;
	ASSET aMMmenuMoveSnd;
	ASSET aMMmusicID;

	// Options Menu IDs
	ASSET aOMbgID;
	

	// Credits Menu IDs
	ASSET aCMmusicID;
	ASSET aCMbgID;
	ASSET aCMbgID2;
	ASSET aCMbgID3;
	ASSET aCMbgID4;
	ASSET aCMbgID5;
	ASSET aCMbgID6;

	// How To Play Menu IDs
	ASSET aHTPMBGimageID;
	ASSET aHTPMSelectID;
	ASSET aHTPMMoveID;
	ASSET aHTPMAttackID;
	ASSET aHTPMSpecialID;
	ASSET aHTPMItemID;

	//HUD Image ID's
	ASSET aHUDBoxID;
	ASSET aRedHealthBarID;
	ASSET aGreenHealthBarID;
	ASSET aBlueHealthBarID;

	//HUD Character Faces
	ASSET aMikeyHUDFaceID;
	ASSET aLeonardoHUDID;
	ASSET aDonatelloHUDID;
	ASSET aRaphaelHUDID;

	//temp ninja
	ASSET aFootClanHUDID;
	ASSET aShredderHUDID;

	ASSET aItemID;

	//particles
	ASSET aFireParticle;
	ASSET aBloodParticle;
	ASSET aSmokeParticle;
	ASSET aGlowParticle;
};

#endif