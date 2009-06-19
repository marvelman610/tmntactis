////////////////////////////////////////////////////////
//   File Name	:	"CPlayer.h"
//
//   Author		:	Matthew Di Matteo (MD)
//
//   Purpose	:	To provide a game object to be used as the player
//					being controlled by user during the game.
////////////////////////////////////////////////////////

#ifndef PLAYER_H
#define PLAYER_H

enum { LEONARDO, DONATELLO, RAPHAEL, MIKEY, };
enum { WORLD_MAP, BATTLE_MAP, };
#include "Turtle.h"
#include "Base.h"
#include "BattleItem.h"
#include <vector>
using std::vector;

class CAchievements;

class CPlayer
{
private:
	CTurtle*  m_pTurtles[4];
	CAchievements* m_pAcheivements;

	vector<CBattleItem> m_nInventory;
	int		m_nCurrStage;
	string  m_sProfileName;
	string  m_sFileName;

	///////////////////////////////////////////////////////////////////
	//	Function:	"CPlayer(Constructor)"
	///////////////////////////////////////////////////////////////////
	CPlayer(void);
	CPlayer(const CPlayer&);
	CPlayer& operator=(const CPlayer&);

	///////////////////////////////////////////////////////////////////
	//	Function:	"~CPlayer(Destructor)"
	///////////////////////////////////////////////////////////////////
	~CPlayer(void);

	///////////////////////////////////////////////////////////////////
	//  Function: "LoadNewSkills"
	//
	//  Purpose: Loads all skills for a new game.
	///////////////////////////////////////////////////////////////////
	void LoadNewSkills(const char* filename);

	///////////////////////////////////////////////////////////////////
	//  Function: "LoadTurtleStats"
	//
	//  Purpose: Load stats for turtles
	///////////////////////////////////////////////////////////////////
	bool LoadTurtleStats(const char* szXMLFileName);

	///////////////////////////////////////////////////////////////////
	//  Function: "LoadAnimations"
	//
	//  Purpose: Create the new game, turtles, skills, everything.
	///////////////////////////////////////////////////////////////////
	void LoadAnimations();

public:
	///////////////////////////////////////////////////////////////////
	//  Function: "GetInstance"
	//
	//  Purpose: Gets instance to the only instance of the class.
	///////////////////////////////////////////////////////////////////
	static CPlayer* GetInstance();

	///////////////////////////////////////////////////////////////////
	//  Function: "NewGame"
	//
	//  Purpose: Create the new game, turtles, skills, everything.
	///////////////////////////////////////////////////////////////////
	void NewGame();

	//////////////////////////////////////////////////////////////////////////
	//	Accessors / Mutators
	//////////////////////////////////////////////////////////////////////////
	inline int GetCurrStage(void)		{ return m_nCurrStage;		}
	inline CTurtle** GetTurtles(void)	{ return m_pTurtles;		}
	inline CAchievements* GetAch()		{ return m_pAcheivements;}
	inline string GetProfName()			{ return m_sProfileName;}
	inline string GetFileName()			{ return m_sFileName;}
	void AddItem(CBattleItem a)			{ m_nInventory.push_back(a);}

	inline int GetNumItems()			{ return m_nInventory.size();}
	vector<CBattleItem>* GetItems()		{ return &m_nInventory;		}

	void SetProfileName(string name)	{ m_sProfileName = name;}
	void SetFileName(string name)		{ m_sFileName = name;}
	void SetAch(CAchievements* ach)		{ m_pAcheivements = ach;}
	void SetStage(int stage)			{ m_nCurrStage = stage;}

	
	bool RemoveItem(int index);


};
#endif