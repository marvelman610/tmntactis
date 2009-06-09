//////////////////////////////////////////////////////////////////////////
//	Filename	:	Assets.h
//
//	Author		:	Ramon Johannessen (RJ)
//
//	Purpose		:	To define the CAssets class. Will load all assets used
//					within the game and hold the IDs. A singleton.
//////////////////////////////////////////////////////////////////////////
#pragma once
//////////////////////////////////////////////////////////////////////////
// all assets will follow a naming convention to easily identify.
// For a background image used in the BattleMap class for instance, 
// the variable name would be aBM_bgID.
//
//		the template is a<all capital letters of class name minus the "C">_<what the image is>ID
//////////////////////////////////////////////////////////////////////////

class CSGD_TextureManager;

class CAssets
{
private:
	CSGD_TextureManager* m_pTM;

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

	// all assets are public...no need to write accessors/mutators for all of them

	// Images common to multiple classes
	int aMenuCursorImageID;
	int aBitmapFontID;
	int aMousePointerID;
	int aMouseGrabID;
	int aMouseAttackID;
	int aMouseMagGlassID;
	int aMouseMoveID;
	int aBitmapFont2ID;
	int aBitmapFont3ID;

	// BattleMap IDs
	int aBMbgID;
	int aBMgroundTilesID, aBMtreeTilesID;
	int aBMcursorID;
	int aBMgreenSquareID;
	int aBMcurrSelectedArrowID;
	int aBMcurrTargetArrowID;
	int aBMactionBoxID;

	// Main Menu IDs
	int aMMBGimageID;

	// Options Menu IDs
	int aOMbgID;

	// How To Play Menu IDs
	int aHTPMBGimageID;

	//HUD Image ID's
	int aHUDBoxID;
	int aRedHealthBarID;
	int aGreenHealthBarID;
	int aBlueHealthBarID;
	//HUD Character Faces
	int aMikeyHUDFaceID;
	int aLeonardoHUDID;
	int aDonatelloHUDID;
	int aRaphaelHUDID;

	//temp ninja
	int aFootClanHUDID;
	int aShredderHUDID;
};
