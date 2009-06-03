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

class CPlayer
{
private:
	CTurtle  m_pTurtles[4];
	CTurtle* m_pLeonardo;
	CTurtle* m_pDonatello;
	CTurtle* m_pRaphael;
	CTurtle* m_pMikey;

	int m_pItemsArr[50];
	int m_nCurrStage;

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
	//  Function: "NewGame"
	//
	//  Purpose: Create the new game, turtles, skills, everything.
	///////////////////////////////////////////////////////////////////
	bool LoadTurtleStats(const char* szXMLFileName);

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

	///////////////////////////////////////////////////////////////////
	//  Function: "LoadSavedGame"
	//
	//  Purpose: Set up the saved game. The turtle's skills, stats, exp, etc...
	///////////////////////////////////////////////////////////////////
	void LoadSavedGame(const char* fileName);

	inline int GetCurrStage(void)		{ return m_nCurrStage;	}
	inline CTurtle* GetTurtles(void)	{ return m_pTurtles;	}
	inline CTurtle* GetLeonardo(void)	{ return m_pLeonardo;	}
	inline CTurtle* GetDonatello(void)	{ return m_pDonatello;	}
	inline CTurtle* GetRaphael(void)	{ return m_pRaphael;	}
	inline CTurtle* GetMikey(void)		{ return m_pMikey;		}

	/*void Render();
	void Update(float fElapsedTime);*/
};
#endif