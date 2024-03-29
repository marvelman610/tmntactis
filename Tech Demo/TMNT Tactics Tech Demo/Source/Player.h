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

class CTurtle;

class CPlayer
{
private:
	//CTurtle* m_pTurtles[4];
	CTurtle* m_pLeonardo;
	CTurtle* m_pDonatello;
	CTurtle* m_pRaphael;
	CTurtle* m_pMikey;

	int m_pItemsArr[50];
	int CurrStage;

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

public:
	///////////////////////////////////////////////////////////////////
	// Function: "GetInstance"
	//
	//  Purpose: Gets instance to the only instance of the class.
	///////////////////////////////////////////////////////////////////
	static CPlayer* GetInstance();

	bool Load(const char* szXMLFileName);

};

#endif