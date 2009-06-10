//////////////////////////////////////////////////////////////////////////
//	Filename	:	Box.h
//
//	Author		:	Ramon Johannessen (RJ)
//
//	Purpose		:	To dynamically create menu boxes and populate them with
//					specific information.
//////////////////////////////////////////////////////////////////////////
#pragma once

#include <windows.h>
#include <string>
using std::string;

class CBitmapFont;
class CAssets;
class CSGD_TextureManager;
class CSGD_Direct3D;
enum {BOX_NO_BACK, BOX_WITH_BACK, };

class CBox
{
// 	// which image is currently being used for the BG
	int m_nCurrImage;

	// size/position variables
	int m_nLongestString;
	int m_nBoxWidth;
	int m_nBoxHeight;
	int m_nBoxRight;
	int m_nBoxBottom;
	int m_nPosX;
	int m_nPosY;
	float m_fPosZ;
	int m_nSpacing;
	int m_nStartTextY;
	int m_nStartTextX;
	float m_fScaleX;
	float m_fScaleY;
	float m_fTextZ;
	int m_nAlpha;
	int r;
	int g;
	int b;

	// menu items (buttons)
	string* m_sItems;
	float m_fTextScale;
	int m_nNumItems;
	DWORD m_dwColor;

	// mouse selection (buttons being hovered over/pressed)
	int m_nCurrSelectedIndex;
	bool m_bIsActive;

	int m_nType;

	CSGD_TextureManager*	m_pTM;
	CBitmapFont*			m_pBM;
 	CAssets*				m_pAssets;
 	CSGD_Direct3D*			m_pD3D;
 
	//////////////////////////////////////////////////////////////////////////
	//	Function	:	"CheckMouse"
	//
	//	Purpose		:	Determine if the mouse is over the box, if so, which
	//					index (button) is it over?
	//////////////////////////////////////////////////////////////////////////
	void CheckMouse(POINT mousePt);

 public:
	CBox(int numItems, string* sItems, int posX, int posY, float posZ = 0.1f, int width = 256, int height = -1, 
		int spacing = 35, int startX=40, int startY=40, int imageID = -1, float fScaleX = 1.0f, float fScaleY = 1.0f, 
		float fTextScale = 1.0f, int red = 0, int green = 0, int blue = 0);
	~CBox();

	//////////////////////////////////////////////////////////////////////////
	//	Function	:	"Render"
	//
	//	Purpose		:	Draws the drawing of the box and it's contents
	//////////////////////////////////////////////////////////////////////////
	void Render();

	//////////////////////////////////////////////////////////////////////////
	//	Function	:	"Input"
	//
	//	Purpose		:	Handles user input, selections and such
	//////////////////////////////////////////////////////////////////////////
	int Input(POINT mousePt);
 
	//////////////////////////////////////////////////////////////////////////
	//	Accessors / Mutators
	//////////////////////////////////////////////////////////////////////////
	int CurrImage() const { return m_nCurrImage; }
	void CurrImage(int val) { m_nCurrImage = val; }
	float PosZ() const { return m_fPosZ; }
	void PosZ(float val) { m_fPosZ = val; }
	int PosY() const { return m_nPosY; }
	void PosY(int val) { m_nPosY = val; }
	int PosX() const { return m_nPosX; }
	void PosX(int val) { m_nPosX = val; }
	int BoxHeight() const { return m_nBoxHeight; }
	void BoxHeight(int val) { m_nBoxHeight = val; }
	int BoxWidth() const { return m_nBoxWidth; }
	void BoxWidth(int val) { m_nBoxWidth = val; }
	bool IsActiveBox() const { return m_bIsActive; }
	void SetActive(bool IsActive = true);
	void SetType(int type) {m_nType = type;}
};