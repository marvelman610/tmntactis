////////////////////////////////////////////////////////
//   File Name	:	"HUD.h"
//
//   Author		:	Matthew Di Matteo (MD)
//
//   Purpose	:	This class encompasses all menus to 
//					appear on the screen during gameplay.
////////////////////////////////////////////////////////

#ifndef HUD_H
#define HUD_H

#include <windows.h>

class CPlayer;
class CBitmapFont;
class CSGD_TextureManager;
class CSGD_Direct3D;

class CHUD
{
private:
	int m_nImageID;		//character face image

	RECT m_rMikeyHP;	//HP bar rect
	RECT m_rMikeyAP;	//AP bar rect

	RECT m_rLeoHP;		//HP bar rect
	RECT m_rLeoAP;		//AP bar rect

	RECT m_rDonHP;		//HP bar rect
	RECT m_rDonAP;		//AP bar rect

	RECT m_rRaphHP;		//HP bar rect
	RECT m_rRaphAP;		//AP bar rect

	RECT m_rEnemyHP;	//HP bar rect
	RECT m_rEnemyAP;	//AP bar rect

	RECT m_rShredderHP;	//HP bar rect
	RECT m_rShredderAP;	//AP bar rect

	CPlayer* m_pPlayer;
	CBitmapFont* m_pBitmapFont;
	CSGD_TextureManager* m_pTM;
	CSGD_Direct3D* m_pD3D;

public:
	///////////////////////////////////////////////////////////////////
	//	Function:	"CHUD(Constructor)"
	///////////////////////////////////////////////////////////////////
	CHUD(void);

	///////////////////////////////////////////////////////////////////
	//	Function:	"~CHUD(Destructor)"
	///////////////////////////////////////////////////////////////////
	~CHUD(void);
	////////////////////////////////////////////////////////////////////
	// Function : "LoadHUD"
	//
	// Purpose : To load the textures used for the HUD
	////////////////////////////////////////////////////////////////////
	//void LoadHUD(void);

	///////////////////////////////////////////////////////////////////
	// Function: "GetInstance"
	//
	//  Purpose: Gets instance to the only instance of the class.
	///////////////////////////////////////////////////////////////////
	static CHUD* GetInstance();
	
	////////////////////////////////////////////////////////////////////
	// Function: “Update”
	//
	// Purpose: Updates game objects based on time.
	////////////////////////////////////////////////////////////////////
	void Update(float fElapsedTime);
	
	////////////////////////////////////////////////////////////////////
	// Function: “Render”
	//
	// Purpose: Draws game objects to the screen.
	////////////////////////////////////////////////////////////////////
	void Render();

	void DrawRaphHUD();
	void DrawDonHUD();
	void DrawLeoHUD();
	void DrawMikeyHUD();
	void DrawFootClanHUD();
};

#endif