////////////////////////////////////////////////////////
//   File Name	:	"Ninja.h"
//
//   Author		:	Matthew Di Matteo (MD)
//
//   Purpose	:	To provide a game object to be used as an emeny
//					against the user during the game.
////////////////////////////////////////////////////////

#ifndef NINJA_H
#define NINJA_H

#include "Base.h"
#include "CSGD_TextureManager.h"
#include "Player.h"
#include "Tile.h"

class CNinja : public CBase
{
private:
	//set up integers to change state
	int m_nAttack;
	int m_nDefend;
	int m_nLowHealth;
	int m_nInRange;

	int m_nTurtle; //store the turtle number id

	int m_nXChange; //difference in x between turtle and ninja
	int m_nYChange; //difference in y between turtle and ninja

	CPlayer* m_pPlayer; //pointer to player
	CTile* m_pTile; //pointer to tile
	vector<POINT> m_vPath; //path to take
	MY_POINT_FLOAT m_ptStartXY; //point for movement
	bool m_bMoving;


public:
	///////////////////////////////////////////////////////////////////
	//	Function:	"CNinja(Constructor)"
	///////////////////////////////////////////////////////////////////
	CNinja(void);

	///////////////////////////////////////////////////////////////////
	//	Function:	"~CNinja(Destructor)"
	///////////////////////////////////////////////////////////////////
	~CNinja(void);

	////////////////////////////////////////////////////////////////////
	// Function: “AI”
	//
	// Purpose: Function to handle artificial intelligence of the object.
	////////////////////////////////////////////////////////////////////
	void AI();

	/////////////////////////////////////////////////////////////////////
	// Function : "FindPath"
	//
	// Purpose : Function to handle the path finding for the ninja
	/////////////////////////////////////////////////////////////////////
	void FindPath(POINT begin, POINT end);

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

	////////////////////////////////////////////////////////////////////
	// Function: Accessor and Mutator
	////////////////////////////////////////////////////////////////////
	inline int GetInRange(void)			{ return m_nInRange;	}
	inline void SetInRange(int set)		{ m_nInRange = set;		}
	inline int GetXDifference(void)		{ return m_nXChange;	}
	inline void SetXDifference(int set)	{ m_nXChange = set;		}
	inline int GetYDifference(void)		{ return m_nYChange;	}
	inline void SetYDifference(int set)	{ m_nYChange = set;		}
	inline int GetTurtle(void)			{ return m_nTurtle;		}

};
#endif