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
#include "Timer.h"
using std::vector;

class CPlayer;
class CBitmapFont;
class CBattleMap;

class CBoss : public CBase
{
	struct BossCombo 
	{
		int attackID;
		int damage;
		int apCost;
		BossCombo()
		{
			apCost = 0;
			attackID = -1;
			damage = 0;
		}
	};
private:
	int m_nInRange;				//range of turtle from enemy
	int m_nTurtle;				//store the turtle number id
	int m_nXChange;				//difference in x between turtle and shredder
	int m_nYChange;				//difference in y between turtle and shredder
	int m_nCurrAnimInd;
	bool m_bMoving;				//moving bool
	bool m_bAttacking;
	bool m_bAttackDone;
	bool m_bRenderCombText;
	bool m_bDoDmg;
	int m_nDamage;
	int m_nDistance;			//distance from turtle
	int m_nCurrAttackInd;
	BossCombo m_pComb[4];
	
	CPlayer* m_pPlayer;			//pointer to player
	CBitmapFont* m_pBitmapFont;
	CBattleMap* m_pBattleMap;
	CTile* m_pTilesL1;			//pointer to tile
	MY_POINT_FLOAT m_ptStartXY; //point for movement
	MY_POINT_FLOAT m_ptCurrPos; //current position to check against change
	vector<POINT>m_vMoveList;	//list of tiles to move through(path)
	int m_nMoveListIndex;
	
	CTimer m_Timer;

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
	// Function: �AI�
	//
	// Purpose: Function to handle artificial intelligence of the object.
	////////////////////////////////////////////////////////////////////
	void AI();

	void DetermineMove( POINT &begin, CBattleMap* pBM );
	/////////////////////////////////////////////////////////////////////
	// Function : "FindPath"
	//
	// Purpose : Function to handle the path finding for the ninja
	/////////////////////////////////////////////////////////////////////
	void FindPathNew(POINT begin, POINT end);

	bool IsOnCloseList(CTile* tile, vector<CTile>& closed);

	////////////////////////////////////////////////////////////////////
	// Function: �Update�
	//
	// Purpose: Updates game objects based on time.
	////////////////////////////////////////////////////////////////////
	virtual void Update(float fElapsedTime);

	void PerformMove( float fElapsedTime );
	void DetermineAttack();

	////////////////////////////////////////////////////////////////////
	// Function: �Render�
	//
	// Purpose: Draws game objects to the screen.
	////////////////////////////////////////////////////////////////////
	virtual void Render();

	void HandleTimerEvent();
};
#endif