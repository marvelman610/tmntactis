////////////////////////////////////////////////////////
//   File Name	:	"Boss.h"
//
//   Author		:	Matthew Di Matteo (MD)
//
//   Purpose	:	To provide a game object to be used as a Boss emeny
//					against the user during the game.
////////////////////////////////////////////////////////

#ifndef BOSS_H
#define BOSS_H

#include "Base.h"
#include <vector>
#include "Tile.h"
using std::vector;


class CPlayer;


class CBoss : public CBase
{
private:
	int m_nInRange;				//range of turtle from enemy
	int m_nTurtle;				//store the turtle number id
	int m_nXChange;				//difference in x between turtle and shredder
	int m_nYChange;				//difference in y between turtle and shredder
	bool m_bMoving;				//moving bool
	int m_nDistance;			//distance from turtle
	
	CPlayer* m_pPlayer;			//pointer to player
	CTile* m_pTilesL1;			//pointer to tile
	vector<POINT> m_vPath;		//path to take
	MY_POINT_FLOAT m_ptStartXY; //point for movement
	MY_POINT_FLOAT m_ptCurrPos; //current position to check against change
	vector<POINT>m_vClosedList; //list of the closed tiles
	vector<POINT>m_vMoveList;	//list of tiles to move through(path)
	


public:
	///////////////////////////////////////////////////////////////////
	//	Function:	"CBoss(Constructor)"
	///////////////////////////////////////////////////////////////////
	CBoss(void);

	///////////////////////////////////////////////////////////////////
	//	Function:	"~CBoss(Destructor)"
	///////////////////////////////////////////////////////////////////
	~CBoss(void);

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

	void FindPathX(POINT endPt, vector<POINT>Closed);

	void FindPathNew(POINT begin, POINT end);

	bool IsOnClose(CTile* tile, vector<CTile>& closed);

	////////////////////////////////////////////////////////////////////
	// Function: “Update”
	//
	// Purpose: Updates game objects based on time.
	////////////////////////////////////////////////////////////////////
	virtual void Update(float fElapsedTime);
	
	////////////////////////////////////////////////////////////////////
	// Function: “Render”
	//
	// Purpose: Draws game objects to the screen.
	////////////////////////////////////////////////////////////////////
	virtual void Render();
};
#endif