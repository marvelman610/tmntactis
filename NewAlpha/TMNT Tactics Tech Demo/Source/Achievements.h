//////////////////////////////////////////////////////////////////////////
//	Filename	:	CAchievements.h
//
//	Author		:	Ramon Johannessen (RJ)
//
//	Purpose		:	Encapsulates all acheivement code..
//////////////////////////////////////////////////////////////////////////
#ifndef ACHIEVEMENTS_H
#define ACHIEVEMENTS_H

enum {
	ACH_NEWGAME, 
	ACH_LVLUP, 
	ACH_TURTLESDEAD, 
	ACH_ALLUNLOCKED, 
	ACH_FIRSTMAPCOMPLETE,
	ACH_KILL30, 
	ACH_MEGAKILL, 
	ACH_KILL_SHREDDER,
	ACH_UNLOCKSKILLS, 
	ACH_QUICKFINGERS, 
	};
#include <string>
using std::string;

class CSGD_TextureManager;
class CAssets;
class CSGD_Direct3D;

struct Acheivement 
{
	bool bUnlocked;
	int imageID;
	string title;
	char* description;

	Acheivement()
	{
		bUnlocked = false;
		imageID = -1;
		title = "";
		description = "";
	}
};
class CAchievements
{
	CSGD_TextureManager* m_pTM;
	CAssets*			 m_pAssets;
	CSGD_Direct3D*		 m_pD3D;
	float m_fTimer;
	int m_nCurrUnlocked;	// used to know which image to display

	int m_nUnlockCount;
	Acheivement m_pAcheivements[10];

public:
	// CTOR
	CAchievements();

	~CAchievements(void);

	/////////////////////////////////////////////////////////////////
	//	Function:	Unlock
	//
	//	Purpose:	Unlocks the given achievement...
	////////////////////////////////////////////////////////////////
	void Unlock(int id);

	/////////////////////////////////////////////////////////////////
	//	Function:	SetToLocked
	//
	//	Purpose:	sets all achievements to locked
	////////////////////////////////////////////////////////////////
	void SetToLocked();
	void LoadUnlock(int id);

	/////////////////////////////////////////////////////////////////
	//	Function:	Update
	//
	//	Purpose:	Updates the current achievement
	////////////////////////////////////////////////////////////////
	void Update(float fElapsedTime);

	/////////////////////////////////////////////////////////////////
	//	Function:	Render
	//
	//	Purpose:	Renders currently unlocked achievement
	//				returns: true if there's something to render
	////////////////////////////////////////////////////////////////
	bool Render();

	//////////////////////////////////////////////////////////////////////////
	//	accessors
	//////////////////////////////////////////////////////////////////////////
	bool GetLocked(int id) {return m_pAcheivements[id].bUnlocked;}
	void SetSingleAch(Acheivement ach, int index)	{ m_pAcheivements[index] = ach;}
};


#endif