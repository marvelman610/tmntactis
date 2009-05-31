//////////////////////////////////////////////////////////////////////////
//	Filename	:	BaseMenuState.h
//
//	Author		:	Ramon Johannessen (RJ)
//
//	Purpose		:	Base Class for all menu states
//////////////////////////////////////////////////////////////////////////
#ifndef CBASEMENUSTATE_H
#define CBASEMENUSTATE_H
#include "IGameState.h"
#include "Player.h"

class CBitmapFont;
class CAssets;
class CSGD_TextureManager;
class CSGD_Direct3D;
class CSGD_DirectInput;
class CGame;

class CBaseMenuState : public IGameState
{
private:
	int	m_nBGImageID;				// bg image ID
	int m_nScreenWidth;				// the program's width and height
	int m_nScreenHeight;
	int m_nBGX;						// where the background x is
	int m_nBGY;						// where the background y is
	int	m_nImageWidth;				// bg image width	
	int m_nImageHeight;				// bg image height
	int m_nCursorImageID;			// the menu selection cursor image
	int	m_nCurrMenuSelection;		// where is the menu cursor at?
	int m_nCursorX;					// screen x for cursor
	int m_nCursorY;					// screen y for cursor
	int m_nMenuItemSpacing;			// the distance between one menu item and the next
	//FMOD::Sound m_sBGMusicID;		// bg music track

	CBitmapFont*		m_pBitmapFont;	// a pointer to the bitmap font singleton
	CAssets*			m_pAssets;
	CSGD_TextureManager* m_pTM;
	CSGD_Direct3D*		m_pD3D;
	CSGD_DirectInput*	m_pDI;
	CGame*				m_pGame;
	//FMOD::System*		m_pFMOD;

	//CBaseMenuState* m_pCurrentState;// a pointer to the current menu state


	CBaseMenuState(const CBaseMenuState&);
	CBaseMenuState& operator= (const CBaseMenuState&);
public:
	CBaseMenuState();
	~CBaseMenuState();

	//////////////////////////////////////////////////////////////////////////
	//	Function	:	Update
	//
	//	Purpose		:	Any update code goes here, pure virtual, virtual, to be overwritten
	//////////////////////////////////////////////////////////////////////////
	virtual void Update(float fElapsedTime);
	//////////////////////////////////////////////////////////////////////////
	//	Function	:	Render
	//
	//	Purpose		:	draw everything to the screen, virtual, to be overwritten
	//////////////////////////////////////////////////////////////////////////
	virtual void Render();
	//////////////////////////////////////////////////////////////////////////
	//	Function	:	Input
	//
	//	Purpose		:	Handle any user input for all menu states, mouse or keyboard, virtual, to be overwritten
	//
	//	Return		:	true/false, false if we are exiting the game
	//////////////////////////////////////////////////////////////////////////
	virtual bool Input(float elapsedTime, POINT mousePt);
	//////////////////////////////////////////////////////////////////////////
	//	Function	:	Enter
	//
	//	Purpose		:	When the state is first entered, execute this code, virtual, to be overwritten
	//////////////////////////////////////////////////////////////////////////
	virtual void Enter();
	//////////////////////////////////////////////////////////////////////////
	//	Function	:	Exit
	//
	//	Purpose		:	When the state exits, execute this code, virtual, to be overwritten
	//////////////////////////////////////////////////////////////////////////
	virtual void Exit();

	//////////////////////////////////////////////////////////////////////////
	//	Function	:	CenterBGImage
	//
	//	Purpose		:	Automatically centers the background image
	//////////////////////////////////////////////////////////////////////////
	void CenterBGImage();

	//////////////////////////////////////////////////////////////////////////
	//	Accessors
	//////////////////////////////////////////////////////////////////////////
	CAssets* GetAssets()			{return m_pAssets;}
	CSGD_DirectInput* GetDI()		{return m_pDI;}
	CSGD_TextureManager* GetTM()	{return m_pTM;}
	CSGD_Direct3D* GetD3D()			{return m_pD3D;}
	CBitmapFont* GetBitmapFont()	{return m_pBitmapFont;}
	CGame* GetGame()				{return m_pGame;}
	int GetCurrMenuSelection()		{return m_nCurrMenuSelection;}
	int GetMenuItemSpacing()		{return m_nMenuItemSpacing;}
	int GetCursorX()				{return m_nCursorX;}
	int GetCursorY()				{return m_nCursorY;}
	int GetScreenWidth()			{return m_nScreenWidth;}
	int GetScreenHeight()			{return m_nScreenHeight;}

	//////////////////////////////////////////////////////////////////////////
	//	Mutators
	//////////////////////////////////////////////////////////////////////////
	void SetCursorX(int x)						{m_nCursorX = x;}
	void SetCursorY(int y)						{m_nCursorY = y;}
	void SetCurrMenuSelection(int selection)	{m_nCurrMenuSelection = selection;}
	void SetBGImageID(int bgImageID)			{m_nBGImageID = bgImageID;}
	void SetBGWidth(int width)					{m_nImageWidth = width;}
	void SetBGHeight(int height)				{m_nImageHeight = height;}
	void SetCursorImageID(int cursorID) 		{m_nCursorImageID = cursorID;}

};

#endif