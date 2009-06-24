////////////////////////////////////////////////////////
//   File Name	:	"Boss.cpp"
//
//   Author		:	Matthew Di Matteo (MD)
//
//   Purpose	:	To provide a game object to be used as a Boss emeny
//					against the user during the game.
////////////////////////////////////////////////////////

#include "Boss.h"
#include <math.h>
#include <ctime>
#include "Player.h"
#include "BattleMap.h"
#include "ObjectManager.h"
#include "Achievements.h"
#include "BitmapFont.h"
#include "Tile.h"
#include "Assets.h"

enum { IDLE_1, IDLE_2, ATK_HIGH, ATK_LOW, };

int Random(int min, int max)
{
	int number = 0;
	number = abs(max) - abs(min);
	number = ((rand()%number) + abs(min));
	if(rand() % 2 == 0)
		number = -number;
	return number;
}

CBoss::CBoss(void)
{
	srand(unsigned int(time(0)));
	m_nInRange = 0;
	m_nTurtle = 0;	
	m_nXChange = 0;
	m_nYChange = 0;
	m_bMoving = false;
	m_pPlayer = CPlayer::GetInstance();
	m_pTilesL1 = CBattleMap::GetInstance()->GetTiles();
	m_nType = OBJECT_BOSS;
	m_vClosedList.clear();
	m_vMoveList.clear();
	m_nDistance = 0;
	m_pBitmapFont = CBitmapFont::GetInstance();
	m_bAttacking = m_bMoving = false;
}

CBoss::~CBoss(void)
{
}

void CBoss::FindPath(POINT begin, POINT end)
{
	m_vPath.clear();
	POINT ptCurr = begin; // begin point
	POINT ptTarget = end;	// end point
	int range = GetCurrAP();	// max distance moveable
	int pathWeight = 0;
	vector<int> pathX;
	vector<int> pathY;
	int oldX = ptCurr.x;
	int oldY = ptCurr.y;

	while (true)
	{
		if (pathWeight-4 > -1)
		{
			if ( (ptCurr.x == pathX[pathWeight-4]) && (ptCurr.y == pathY[pathWeight-4]) )
			{ pathWeight = 10000; break; }
		}
		if (pathWeight-1 > -1)
		{
			pathX.push_back(ptCurr.x);
			pathY.push_back(ptCurr.y);
			m_vPath.push_back(ptCurr);
			oldX = pathX[pathWeight-1];
			oldY = pathY[pathWeight-1];
		}
		++pathWeight;
		if (pathWeight*2 > GetCurrAP())
			break;
		if (ptTarget.x == ptCurr.x && ptTarget.y == ptCurr.y)
			break;
		if (pathWeight > CBattleMap::GetInstance()->GetNumCols())
		{pathWeight = 10000; break;}

		if (ptCurr.x < ptTarget.x && (oldX != ptCurr.x + 1) && 
			m_pTilesL1[ptCurr.y * CBattleMap::GetInstance()->GetNumCols() + (ptCurr.x+1)].Flag() == FLAG_NONE &&
			!CBattleMap::GetInstance()->CheckTileOccupied(ptCurr.y * CBattleMap::GetInstance()->GetNumCols() + (ptCurr.x+1)))
		{ ++ptCurr.x; continue; }

		if (ptCurr.x > ptTarget.x && (oldX != ptCurr.x - 1) && 
			m_pTilesL1[ptCurr.y * CBattleMap::GetInstance()->GetNumCols() + (ptCurr.x-1)].Flag() == FLAG_NONE &&
			!CBattleMap::GetInstance()->CheckTileOccupied(ptCurr.y * CBattleMap::GetInstance()->GetNumCols() + (ptCurr.x-1)))
		{ --ptCurr.x; continue; }

		if (ptCurr.y < ptTarget.y && (oldY != ptCurr.y + 1) && 
			m_pTilesL1[(ptCurr.y+1) * CBattleMap::GetInstance()->GetNumCols() + ptCurr.x].Flag() == FLAG_NONE &&
			!CBattleMap::GetInstance()->CheckTileOccupied((ptCurr.y+1) * CBattleMap::GetInstance()->GetNumCols() + ptCurr.x))
		{ ++ptCurr.y; continue; }

		if (ptCurr.y > ptTarget.y && (oldY != ptCurr.y - 1) && 
			m_pTilesL1[(ptCurr.y-1) * CBattleMap::GetInstance()->GetNumCols() + ptCurr.x].Flag() == FLAG_NONE &&
			!CBattleMap::GetInstance()->CheckTileOccupied((ptCurr.y-1) * CBattleMap::GetInstance()->GetNumCols() + ptCurr.x))
		{ --ptCurr.y; continue; }

		if (ptCurr.x == ptTarget.x)
		{
			if (oldX != ptCurr.x+1 && m_pTilesL1[ptCurr.y * CBattleMap::GetInstance()->GetNumCols() + ptCurr.x+1].Flag() == FLAG_NONE &&
				!CBattleMap::GetInstance()->CheckTileOccupied(ptCurr.y * CBattleMap::GetInstance()->GetNumCols() + ptCurr.x+1) )
			{ ++ptCurr.x; continue; }
			if (oldX != ptCurr.x-1 && m_pTilesL1[ptCurr.y * CBattleMap::GetInstance()->GetNumCols() + ptCurr.x-1].Flag() == FLAG_NONE &&
				!CBattleMap::GetInstance()->CheckTileOccupied(ptCurr.y * CBattleMap::GetInstance()->GetNumCols() + ptCurr.x-1))
			{ --ptCurr.x; continue; }
		}
		else if (ptCurr.y == ptTarget.y)
		{
			if (oldY != ptCurr.y+1 && m_pTilesL1[(ptCurr.y+1) * CBattleMap::GetInstance()->GetNumCols() + ptCurr.x].Flag() == FLAG_NONE &&
				!CBattleMap::GetInstance()->CheckTileOccupied((ptCurr.y+1) * CBattleMap::GetInstance()->GetNumCols() + ptCurr.x))
			{ ++ptCurr.y; continue; }
			if (oldY != ptCurr.y-1 && m_pTilesL1[(ptCurr.y-1) * CBattleMap::GetInstance()->GetNumCols() + ptCurr.x].Flag() == FLAG_NONE &&
				!CBattleMap::GetInstance()->CheckTileOccupied((ptCurr.y-1) * CBattleMap::GetInstance()->GetNumCols() + ptCurr.x))
			{ --ptCurr.y; continue; }
		}

		if ( (ptCurr.x > ptTarget.x) && (oldX != ptCurr.x+1) && m_pTilesL1[ptCurr.y * CBattleMap::GetInstance()->GetNumCols() + (ptCurr.x+1)].Flag() == FLAG_NONE &&
			!CBattleMap::GetInstance()->CheckTileOccupied(ptCurr.y * CBattleMap::GetInstance()->GetNumCols() + (ptCurr.x+1)))
		{++ptCurr.x; continue;}
		else if (oldX != ptCurr.x-1 && m_pTilesL1[ptCurr.y * CBattleMap::GetInstance()->GetNumCols() + (ptCurr.x-1)].Flag() == FLAG_NONE &&
			!CBattleMap::GetInstance()->CheckTileOccupied(ptCurr.y * CBattleMap::GetInstance()->GetNumCols() + (ptCurr.x-1)))
		{--ptCurr.x; continue;}

		if ( (ptCurr.y > ptTarget.y) && (oldY != ptCurr.y+1) && m_pTilesL1[(ptCurr.y+1) * CBattleMap::GetInstance()->GetNumCols() + ptCurr.x].Flag() == FLAG_NONE &&
			!CBattleMap::GetInstance()->CheckTileOccupied((ptCurr.y+1) * CBattleMap::GetInstance()->GetNumCols() + ptCurr.x))
		{++ptCurr.y; continue;}
		else if (oldY != ptCurr.y-1 && m_pTilesL1[(ptCurr.y-1) * CBattleMap::GetInstance()->GetNumCols() + ptCurr.x].Flag() == FLAG_NONE &&
			!CBattleMap::GetInstance()->CheckTileOccupied((ptCurr.y-1) * CBattleMap::GetInstance()->GetNumCols() + ptCurr.x))
		{--ptCurr.y; continue;}
		pathWeight = 10000; break;
	}
	if (m_vPath.size() > 0)
	{
		m_ptStartXY.x = GetPosX();
		m_ptStartXY.y = GetPosY();
		for (unsigned int i = 0; i < m_vPath.size(); ++i)
			m_pTilesL1[m_vPath[i].y * CBattleMap::GetInstance()->GetNumCols() + m_vPath[i].x].SetAlpha(199);
	}
}

void CBoss::AI()
{
	m_nInRange = 20; // if no turtles are found in range, this signals a random move
	m_bAttacking = false;
	m_nCurrAnimInd = -1;
	//check distance
	SetCurrAP(16);
	
	CTurtle** turtles = m_pPlayer->GetTurtles();
	CBattleMap* pBM = CBattleMap::GetInstance();
	//set temporary ranges
	int distToMikey = abs(turtles[MIKEY]->GetMapCoord().x - GetMapCoord().x) + 
		abs( turtles[MIKEY]->GetMapCoord().y - GetMapCoord().y);
	int distToLeo = abs( turtles[LEONARDO]->GetMapCoord().x - GetMapCoord().x) +
		abs(turtles[LEONARDO]->GetMapCoord().y - GetMapCoord().y );
	int distToDon = abs(turtles[DONATELLO]->GetMapCoord().x - GetMapCoord().x ) +
		abs(turtles[DONATELLO]->GetMapCoord().y - GetMapCoord().y );
	int distToRaph = abs(turtles[RAPHAEL]->GetMapCoord().x - GetMapCoord().x ) +
		abs(turtles[RAPHAEL]->GetMapCoord().y - GetMapCoord().y );

	//set range to 100 if turtle is dead
	if( turtles[MIKEY]->GetAlive() == false)
		distToMikey = 100;
	if(turtles[LEONARDO]->GetAlive() == false)
		distToLeo = 100;
	if( turtles[DONATELLO]->GetAlive() == false)
		distToDon = 100;
	if( turtles[RAPHAEL]->GetAlive() == false)
		distToRaph = 100;

	//checking if mikey is closest
	if( turtles[MIKEY]->GetAlive() == true)
	{
		if(distToMikey < distToLeo)
		{
			if(distToMikey < distToDon)
			{
				if(distToMikey < distToRaph)
				{
					m_nInRange = distToMikey;
					m_nXChange = turtles[MIKEY]->GetMapCoord().x - GetMapCoord().x;
					m_nYChange = turtles[MIKEY]->GetMapCoord().y - GetMapCoord().y ; 
					m_nTurtle = MIKEY;
				}
			}
		}
	}
	
	//check if leo is closest
	if( turtles[LEONARDO]->GetAlive() == true)
	{
		if(distToLeo < distToMikey)
		{
			if(distToLeo < distToDon)
			{
				if(distToLeo < distToRaph)
				{
					m_nInRange = distToLeo;
					m_nXChange =  turtles[LEONARDO]->GetMapCoord().x - GetMapCoord().x;
					m_nYChange =  turtles[LEONARDO]->GetMapCoord().y - GetMapCoord().y;
					m_nTurtle = LEONARDO;
				}
			}
		}
	}

	//check if donny is closest
	if( turtles[DONATELLO]->GetAlive() == true)
	{
		if(distToDon < distToMikey)
		{
			if(distToDon < distToLeo)
			{
				if(distToDon < distToRaph)
				{
					m_nInRange = distToDon;
					m_nXChange =  turtles[DONATELLO]->GetMapCoord().x - GetMapCoord().x;
					m_nYChange =  turtles[DONATELLO]->GetMapCoord().y - GetMapCoord().y; 
					m_nTurtle = DONATELLO;
				}
			}
		}
	}

	//check if raph is closest
	if( turtles[RAPHAEL]->GetAlive() == true)
	{
		if(distToRaph < distToMikey)
		{
			if(distToRaph < distToLeo)
			{
				if(distToRaph < distToDon)
				{
					m_nInRange = distToRaph;
					m_nXChange = turtles[RAPHAEL]->GetMapCoord().x - GetMapCoord().x ;
					m_nYChange =  turtles[RAPHAEL]->GetMapCoord().y - GetMapCoord().y; 
					m_nTurtle = RAPHAEL;
				}
			}
		}
	}

	POINT begin = GetMapCoord();
	//safe check to see if turtle is chosen if all turtles are == distance
	if(m_nTurtle <= -1)
		m_nTurtle = 0;
	POINT end;
	POINT mapPt;
	//switch case for distance 0 - 8

	switch(m_nInRange)
	{
		//on turtle
	case 0:
		{
			int debug = 0;
		}
		break;
		//one tile away from  turtle
	case 1:
		{
			m_bAttacking = true;
		}
		break;
		//two tiles away from turtle 1 out of range
	case 2:
		{
			//move in 1 tile (2ap)
			switch(abs(m_nXChange))
			{
			case 0:
				mapPt.x = GetMapCoord().x;
				//if(x == 0) y == 2
				if(m_nYChange > 0)
				{
					mapPt.y = GetMapCoord().y + (m_nYChange -1);
				}
				else 
				{
					mapPt.y = GetMapCoord().y + (m_nYChange +1);
				}
				break;
			case 1:
				mapPt.x = GetMapCoord().x + m_nXChange;
				mapPt.y = GetMapCoord().y;
				break;
			case 2:
				if(m_nXChange > 0)
				{
					mapPt.x = GetMapCoord().x + (m_nXChange -1);
					mapPt.y = GetMapCoord().y;
				}
				else
				{
					mapPt.x = GetMapCoord().x + (m_nXChange +1);
					mapPt.y = GetMapCoord().y;
				}
				break;
			}
			end = mapPt;
			m_bMoving = true;
			FindPathNew(begin ,end);
		}
		break;
		//three tiles, move in two tiles
	case 3:
		{
			
			//move in two tiles(4ap)
			switch(abs(m_nXChange))
			{
			case 0://if x = 0, y = 3
				if(m_nYChange > 0)
				{
					mapPt.x = GetMapCoord().x;
					mapPt.y = GetMapCoord().y + (m_nYChange -1);
				}
				else
				{
					mapPt.x = GetMapCoord().x;
					mapPt.y = GetMapCoord().y + (m_nYChange +1);
				}
				break;
			case 1://if x = 1, y = 2
				mapPt.x = GetMapCoord().x;
				mapPt.y = GetMapCoord().y + m_nYChange;
				break;
			case 2://if x = 2, y = 1
				mapPt.x = GetMapCoord().x + m_nXChange;
				mapPt.y = GetMapCoord().y;
				break;
			case 3://if x = 3, y = 0
				if(m_nXChange > 0)
				{
					mapPt.x = GetMapCoord().x + (m_nXChange -1);
					mapPt.y = GetMapCoord().y;
				}
				else
				{
					mapPt.x = GetMapCoord().x + (m_nXChange +1);
					mapPt.y = GetMapCoord().y;
				}
				break;
			}
			end = mapPt;
			m_bMoving = true;
			FindPathNew(begin,end);
		}
		break;
		//four tiles, move in three tiles
	case 4:
		{
			//move in three tiles(6ap)
			switch(abs(m_nXChange))
			{
			case 0://if x = 0, y = 4
				if(m_nYChange > 0)
				{
					mapPt.x = GetMapCoord().x;
					mapPt.y = GetMapCoord().y + (m_nYChange -1);
				}
				else
				{
					mapPt.x = GetMapCoord().x;
					mapPt.y = GetMapCoord().y + (m_nYChange +1);
				}
				break;
			case 1://if x1 y3
				mapPt.x = GetMapCoord().x;
				mapPt.y = GetMapCoord().y + m_nYChange;
				break;
			case 2://if x2 y2
				mapPt.x = GetMapCoord().x + m_nXChange;
				if(m_nYChange >0)
				{
					mapPt.y = GetMapCoord().y + (m_nYChange -1);
				}
				else 
				{
					mapPt.y = GetMapCoord().y + (m_nYChange +1);
				}
				break;
			case 3://if x3 y1
				mapPt.x = GetMapCoord().x + m_nXChange;
				mapPt.y = GetMapCoord().y;
				break;
			case 4://if x4 y0
				if(m_nXChange > 0)
				{
					mapPt.x = GetMapCoord().x + (m_nXChange -1);
				}
				else
				{
					mapPt.x = GetMapCoord().x + (m_nXChange +1);
				}
				mapPt.y = GetMapCoord().y;
				break;
			}
			end = mapPt;
			m_bMoving = true;
			FindPathNew(begin,end);
		}
		break;
		//five tiles, move four
	case 5:
		{
			//move four tiles(8ap)
			switch(abs(m_nXChange))
			{
			case 0://if x0 y5
				if(m_nYChange > 0)
				{
					mapPt.y = GetMapCoord().y + (m_nYChange -1);
				}
				else
				{
					mapPt.y = GetMapCoord().y + (m_nYChange +1);
				}
				mapPt.x = GetMapCoord().x;
				break;
			case 1://if x1 y4
				mapPt.x = GetMapCoord().x;
				mapPt.y = GetMapCoord().y + m_nYChange;
				break;
			case 2://if x2 y3
				mapPt.x = GetMapCoord().x + m_nXChange;
				if(m_nYChange > 0)
				{
					mapPt.y = GetMapCoord().y + (m_nYChange -1);
				}
				else
				{
					mapPt.y = GetMapCoord().y + (m_nYChange +1);
				}
				break;
			case 3://if x3 y2
				mapPt.y = GetMapCoord().y + m_nYChange;
				if(m_nXChange > 0)
				{
					mapPt.x = GetMapCoord().x + (m_nXChange -1);
				}
				else
				{
					mapPt.x = GetMapCoord().x + (m_nXChange +1);
				}
				break;
			case 4://if x4 y1
				mapPt.y = GetMapCoord().y;
				mapPt.x = GetMapCoord().x + m_nXChange;
				break;
			case 5://if x5 y0
				if(m_nXChange > 0)
				{
					mapPt.x = GetMapCoord().x + (m_nXChange -1);
				}
				else
				{
					mapPt.x = GetMapCoord().x + (m_nXChange +1);
				}
				mapPt.y = GetMapCoord().y;
				break;
			}
			end = mapPt;
			m_bMoving = true;
			FindPathNew(begin,end);
		}
		break;
		//six tiles, move five
	case 6:
		{
			//move 5 tiles(10ap)
			switch(abs(m_nXChange))
			{
			case 0://if x0 y6
				if(m_nYChange > 0)
				{
					mapPt.y = GetMapCoord().y + (m_nYChange -1);
				}
				else
				{
					mapPt.y = GetMapCoord().y + (m_nYChange -1);
				}
				mapPt.x = GetMapCoord().x;
				break;
			case 1://if x1 y5
				mapPt.x = GetMapCoord().x;
				mapPt.y = GetMapCoord().y + m_nYChange;
				break;
			case 2://if x2 y4
				mapPt.x = GetMapCoord().x + m_nXChange;
				if(m_nYChange > 0)
				{
					mapPt.y = GetMapCoord().y + (m_nYChange -1);
				}
				else
				{
					mapPt.y = GetMapCoord().y + (m_nYChange +1);
				}
				break;
			case 3://if x3 y3
				mapPt.x = GetMapCoord().x + m_nXChange;
				if(m_nYChange > 0)
				{
					mapPt.y = GetMapCoord().y + (m_nYChange -1);
				}
				else
				{
					mapPt.y = GetMapCoord().y + (m_nYChange +1);
				}
				break;
			case 4://if x4 y2
				mapPt.y = GetMapCoord().y + m_nYChange;
				if(m_nXChange > 0)
				{
					mapPt.x = GetMapCoord().x + (m_nXChange -1);
				}
				else
				{
					mapPt.x = GetMapCoord().x + (m_nXChange +1);
				}
				break;
			case 5://if x5 y1
				mapPt.y = GetMapCoord().y;
				mapPt.x = GetMapCoord().x + m_nXChange;
				break;
			case 6://if x6 y0
				mapPt.y = GetMapCoord().y;
				if(m_nXChange > 0)
				{
					mapPt.x = GetMapCoord().x + (m_nXChange -1);
				}
				else
				{
					mapPt.x = GetMapCoord().x + (m_nXChange +1);
				}
				break;
			}
			end = mapPt;
			m_bMoving = true;
			FindPathNew(begin,end);
		}
		break;
		//7 tiles, move 6
	case 7:
		{
			//move 6 tiles(12ap)
			switch(abs(m_nXChange))
			{
			case 0://if x0 y7
				if(m_nYChange > 0)
				{
					mapPt.y = GetMapCoord().y + (m_nYChange -1);
				}
				else
				{
					mapPt.y = GetMapCoord().y + (m_nYChange +1);
				}
				mapPt.x = GetMapCoord().x;
				break;
			case 1://if x1 y6
				mapPt.x = GetMapCoord().x;
				mapPt.y = GetMapCoord().y + m_nYChange;
				break;
			case 2://if x2 y5
				mapPt.x = GetMapCoord().x + m_nXChange;
				if(m_nYChange > 0)
				{
					mapPt.y = GetMapCoord().y + (m_nYChange -1);
				}
				else
				{
					mapPt.y = GetMapCoord().y + (m_nYChange +1);
				}
				break;
			case 3://if x3 y4
				mapPt.x = GetMapCoord().x + m_nXChange;
				if(m_nYChange > 0)
				{
					mapPt.y = GetMapCoord().y + (m_nYChange -1);
				}
				else
				{
					mapPt.y = GetMapCoord().y + (m_nYChange +1);
				}
				break;
			case 4://if x4 y3
				mapPt.y = GetMapCoord().y + m_nYChange;
				if(m_nXChange > 0)
				{
					mapPt.x = GetMapCoord().x + (m_nXChange -1);
				}
				else
				{
					mapPt.x = GetMapCoord().x + (m_nXChange +1);
				}
				break;
			case 5://if x5 y2
				mapPt.y = GetMapCoord().y + m_nYChange;
				if(m_nXChange > 0)
				{
					mapPt.x = GetMapCoord().x + (m_nXChange -1);
				}
				else
				{
					mapPt.x = GetMapCoord().x + (m_nXChange +1);
				}
				break;
			case 6://if x6 y1
				mapPt.y = GetMapCoord().y;
				mapPt.x = GetMapCoord().x + m_nXChange;
				break;
			case 7://if x7 y0
				mapPt.y = GetMapCoord().y;
				if(m_nXChange > 0)
				{
					mapPt.x = GetMapCoord().x + (m_nXChange -1);
				}
				else
				{
					mapPt.x = GetMapCoord().x + (m_nXChange +1);
				}
				break;
			}
			end = mapPt;
			m_bMoving = true;
			FindPathNew(begin,end);
		}
		break;
		//8 tiles, move 7
	case 8:
		{
			//move 7 tiles(14ap)
			switch(abs(m_nXChange))
			{
			case 0://if x0 y8
				if(m_nYChange > 0)
				{
					mapPt.y = GetMapCoord().y + (m_nYChange -1);
				}
				else
				{
					mapPt.y = GetMapCoord().y + (m_nYChange +1);
				}
				mapPt.x = GetMapCoord().x;
				break;
			case 1://if x1 y7
				mapPt.x = GetMapCoord().x;
				mapPt.y = GetMapCoord().y + m_nYChange;
				break;
			case 2://if x2 y6
				mapPt.x = GetMapCoord().x + m_nXChange;
				if(m_nYChange > 0)
				{
					mapPt.y = GetMapCoord().y + (m_nYChange -1);
				}
				else
				{
					mapPt.y = GetMapCoord().y + (m_nYChange +1);
				}
				break;
			case 3://if x3 y5
				mapPt.x = GetMapCoord().x + m_nXChange;
				if(m_nYChange > 0)
				{
					mapPt.y = GetMapCoord().y + (m_nYChange -1);
				}
				else
				{
					mapPt.y = GetMapCoord().y + (m_nYChange +1);
				}
				break;
			case 4://if x4 y4
				mapPt.x = GetMapCoord().x + m_nXChange;
				if(m_nYChange > 0)
				{
					mapPt.y = GetMapCoord().y + (m_nYChange -1);
				}
				else
				{
					mapPt.y = GetMapCoord().y + (m_nYChange +1);
				}
				break;
			case 5://if x5 y3
				mapPt.y = GetMapCoord().y + m_nYChange;
				if(m_nXChange > 0)
				{
					mapPt.x = GetMapCoord().x + (m_nXChange -1);
				}
				else
				{
					mapPt.x = GetMapCoord().x + (m_nXChange +1);
				}
				break;
			case 6://if x6 y2
				mapPt.y = GetMapCoord().y + m_nYChange;
				if(m_nXChange > 0)
				{
					mapPt.x = GetMapCoord().x + (m_nXChange -1);
				}
				else
				{
					mapPt.x = GetMapCoord().x + (m_nXChange +1);
				}
				break;
			case 7://if x7 y1
				mapPt.x = GetMapCoord().x + m_nXChange;
				mapPt.y = GetMapCoord().y;
				break;
			case 8://if x8 y0
				if(m_nXChange > 0)
				{
					mapPt.x = GetMapCoord().x + (m_nXChange -1);
				}
				else
				{
					mapPt.x = GetMapCoord().x + (m_nXChange +1);
				}
				mapPt.y = GetMapCoord().y;
				break;
			}

			end = mapPt;
			m_bMoving = true;
			FindPathNew(begin,end);
		}
		break;
	case 20:
		{
			// need to do something..like move instead of ending turn
			// find a tile to move to that's valid
			CTile* tiles = pBM->GetTiles();
			// loop until a valid tile is found
			// find a valid map coord
			while (true)
			{
				int change = Random(1, 4);//rand() % 4 - (-4+1) - 4;
				end.x = begin.x + change;
				change = Random(1, 4);//rand() % (4 - (-4+1) - 4);
				end.y = begin.y + change;
				if (end.x > 1 && end.y > 1 && end.x < pBM->GetNumCols() && end.y < pBM->GetNumRows() && 
						tiles[end.y*pBM->GetNumCols()+end.x].Flag() == FLAG_NONE
						&& end.x != begin.x && end.y != begin.y)
					break;	// found a valid tile
			}
			m_bMoving = true;
			FindPathNew(begin,end);
		}
		break;
	default:
		{

		}
	}
}
void CBoss::Update(float fElapsedTime)
{ 
	if(m_nCurrAnimation > -1)
		m_vAnimations[m_nCurrAnimation].Update(fElapsedTime);

	if(m_bMoving && m_vMoveList.size() > 0)
	{
		m_ptCurrPos.x = GetPosX();
		m_ptCurrPos.y = GetPosY();

		//get start position and next tile
		if(GetMapCoord().x > m_vMoveList[0].x) //move left one x tile(on screen)
		{
			//32 pixels left 16 pixels up
			if( abs(m_ptStartXY.x - m_ptCurrPos.x) < 32.0f && abs(m_ptStartXY.y - m_ptCurrPos.y) < 16.0f)
			{
				m_ptCurrPos.x -= GetVelX() * fElapsedTime;
				m_ptCurrPos.y -= GetVelY() * fElapsedTime;
				SetPosPtF(m_ptCurrPos);
			}
		}
		else if(GetMapCoord().x < m_vMoveList[0].x)//move right one x tile(on screen)
		{
			//32 pixels right  16 pixels down
			if( abs(m_ptStartXY.x - m_ptCurrPos.x) < 32.0f && abs(m_ptStartXY.y - m_ptCurrPos.y) < 16.0f)
			{
				m_ptCurrPos.x += GetVelX() * fElapsedTime;
				m_ptCurrPos.y += GetVelY() * fElapsedTime;
				SetPosPtF(m_ptCurrPos);
			}
		}
		else if(GetMapCoord().y > m_vMoveList[0].y)//move up one tile y(on screen)
		{
			//32 pixels up and 16 pixels right 
			if( abs(m_ptStartXY.x - m_ptCurrPos.x) < 32.0f && abs(m_ptStartXY.y - m_ptCurrPos.y) < 16.0f)
			{
				m_ptCurrPos.x += GetVelX() * fElapsedTime;
				m_ptCurrPos.y -= GetVelY() * fElapsedTime;
				SetPosPtF(m_ptCurrPos);
			}
		}
		else if(GetMapCoord().y < m_vMoveList[0].y)//move down one tile y(on screen)
		{
			//32 pixels down and 16 pixels left
			if( abs(m_ptStartXY.x - m_ptCurrPos.x) < 32.0f && abs(m_ptStartXY.y - m_ptCurrPos.y) < 16.0f)
			{
				m_ptCurrPos.x -= GetVelX() * fElapsedTime;
				m_ptCurrPos.y += GetVelY() * fElapsedTime;
				SetPosPtF(m_ptCurrPos);
			}
		}
		if( abs(m_ptStartXY.x - m_ptCurrPos.x) >= 32.0f && abs(m_ptStartXY.y - m_ptCurrPos.y) >= 16.0f)
		{
			SetCurrTile(m_vMoveList[0], CBattleMap::GetInstance()->GetOffsetX(), CBattleMap::GetInstance()->GetOffsetY(),
			CBattleMap::GetInstance()->GetTileWidth(), CBattleMap::GetInstance()->GetTileHeight(), CBattleMap::GetInstance()->GetNumCols());
			SetCurrAP(GetCurrAP()-2);
			vector<POINT>::iterator first = m_vMoveList.begin();
			m_vMoveList.erase(first);
			if (m_vMoveList.size() > 0)
			{m_ptCurrPos.x = m_vMoveList[0].x; m_ptCurrPos.y = m_vMoveList[0].y;}
			CBattleMap::GetInstance()->UpdatePositions();
		}

		// movement is done
		if(m_vMoveList.size() <= 0)
		{
			//m_vMoveList.clear();
			m_bMoving = false;
			// if in range and enough AP..attack
			if (m_nInRange == 1 && GetCurrAP() >= 4)
			{
				m_bAttacking = true;
			}
			else	// out or range or not enough AP
			{
				//end shredder turn
				m_bAttacking = m_bMoving = false;
				SetCurrAnim(rand() % 2);
				CBattleMap::GetInstance()->UpdatePositions();
				CBattleMap::GetInstance()->NinjaMoveComplete();
				CBattleMap::GetInstance()->SetTurn(true);
			}
		}
	}
	else if (m_bAttacking == true)
	{
		CTurtle** turtles = m_pPlayer->GetTurtles();
		if (m_nCurrAnimInd == -1 || !m_vAnimations[m_nCurrAnimInd].IsAnimationPlaying())
		{
			switch(GetCurrAP())
			{
			case 4:
				{
					m_nCurrAnimInd = ATK_HIGH;
					SetCurrAnim(m_nCurrAnimInd);
					int damage = Random(35, 45);
					turtles[m_nTurtle]->SetHealth(turtles[m_nTurtle]->GetHealth() - damage);	
				}
				break;
			case 6:
				{
					//use skill low punch , sweep
					//do 40+ damage 
					m_nCurrAnimInd = ATK_LOW;
					SetCurrAnim(m_nCurrAnimInd);
					m_nDamage = Random(50, 60);
				}
				break;
			case 8:
				{
					m_nCurrAnimInd = ATK_LOW;
					SetCurrAnim(m_nCurrAnimInd);
					m_nDamage = Random(80, 100);
				}
				break;
			case 10:
				{
					//use skill punch, kick
					//do 60+ damage
					m_nCurrAnimInd = ATK_LOW;
					SetCurrAnim(m_nCurrAnimInd);
					m_nDamage = Random(65, 85);
				}
				break;
			case 12:
				{
					m_nCurrAnimInd = ATK_HIGH;
					SetCurrAnim(m_nCurrAnimInd);
					m_nDamage = Random(35, 45);
				}
				break;
			case 16:
				{
					//use skill low punch, sweep then use skill punch, kick
					m_nCurrAnimInd = ATK_LOW;
					SetCurrAnim(m_nCurrAnimInd);
					m_nDamage = Random(80, 100);
				}
				break;
			default:
				{
					int debug = 0;
				}
				break;
			}
			turtles[m_nTurtle]->SetHealth(turtles[m_nTurtle]->GetHealth() - m_nDamage);
		}
	}
	//CBase::Update(fElapsedTime);
	if(!m_bAttacking && m_nCurrAnimInd > -1 && !GetCurrAnim()->IsAnimationPlaying())
		SetCurrAnim(1);

	if( GetExperience() >= 100)
	{
		SetExperience( GetExperience() - (100* GetLevel()) );
		SetLevel(GetLevel()+1);
		SetHealthMax((int)((float)GetMaxHealth() * 1.25f));
		SetBaseAP(GetBaseAP()+2);
		SetStrength( (int)( (float)GetStrength() * 1.2f ) );
		SetDefense( (int) ( (float)GetDefense() * 1.2f ) );
		SetAccuracy( (int) ( (float)GetAccuracy() * 1.2f ) );
		SetSpeed( (int) ( (float)GetSpeed() * 1.2f ) );
	}
	if (GetHealth() <= 0)
	{
		CPlayer::GetInstance()->GetAch()->Unlock(ACH_KILL_SHREDDER);
		ObjectManager::GetInstance()->Remove(this);
	}
}
void CBoss::Render()
{
	m_vAnimations[m_nCurrAnimation].Render((int)GetPosX()+m_vAnimations[0].GetFrames()[0].nAnchorX, (int)GetPosY()+m_vAnimations[0].GetFrames()[0].nAnchorY, GetPosZ(), 1, m_dwColor);
	if(m_bAttacking)
	{
		int offset = (int)(m_Timer.GetElapsed()*20.0f )-15;

		m_pBitmapFont->ChangeBMFont(CAssets::GetInstance()->aBitmapFontBubblyID,16,15,20);

		//temp
		//TODO:: implement xp gain
		int nXP = 10;

		//ninja xp
		char tempXP[16];
		sprintf_s(tempXP, "+%i", nXP);
		m_pBitmapFont->DrawString(tempXP,(int)GetPosX()+5,(int)GetPosY()-offset,0.4f,0.9f,D3DCOLOR_XRGB(255,255,0));

		//player damage
		char tempDmg[16];
		sprintf_s(tempDmg, "-%i", m_nDamage);
		m_pBitmapFont->DrawString(tempDmg,(int)m_pPlayer->GetTurtles()[m_nTurtle]->GetPosX()+5, (int)m_pPlayer->GetTurtles()[m_nTurtle]->GetPosY()-offset,0.3f,0.9f,D3DCOLOR_XRGB(255,0,0));

		m_pBitmapFont->Reset();
	}
}


void CBoss::FindPathX(POINT endPt, vector<POINT>Closed)
{
	POINT x;
	x.x = endPt.x;
	x.y = endPt.y;

	POINT testTile[4];
	int distance[4];
	int tileNum;
	int minDistance = 100;
	bool onList[4];
	for(int i =0; i < 4; i++)
	{
		onList[i] = false;
	}
	int coordID;
	if(GetCurrAP() >= 2)
	{
		//add curr position in closed list
		Closed.push_back( GetMapCoord() );
		if(GetMapCoord().x - 1 > -1 )//check if (x-1>-1)
		{
			//set those points to a new point called testTile
			testTile[0].x = GetMapCoord().x-1;
			testTile[0].y = GetMapCoord().y;
			if(Closed.size() > 0)//error check to make sure closed list not empty
			{
				for(unsigned int i = 0; i < Closed.size(); i++)//go through the list of points comparing each
				{
					if(testTile[0].x == Closed[i].x && testTile[0].y == Closed[i].y)//if == then already in list and no need to check distance
					{
						onList[i] = true;
					}
					if(onList[i] == false)
					{
						//set coordid to its respective tileID number
						coordID = testTile[0].y * CBattleMap::GetInstance()->GetNumCols() + testTile[0].x;
						if(m_pTilesL1[coordID].Flag() != FLAG_COLLISION)//check if that coorid is collision tile or not 
						{
							//get the distance from this coordinate to the endpt
							distance[0] = abs( (endPt.x - testTile[0].x)+(endPt.y - testTile[0].y) );
							//check if distance is < mindistance
							if(distance[0] < minDistance)
							{
								minDistance = distance[0];
								tileNum = 0;
							}
						}
						else//add the position to the closed list
						{
							Closed.push_back(testTile[0]);
						}
					}
				}
			}
		}

		// (x+1<GetNumRows()) ,(y+1<GetNumRows())
		if(GetMapCoord().x +1 < CBattleMap::GetInstance()->GetNumCols() )//check if (x+1<getnumcols())
		{
			//set those points to a new point called testTile
			testTile[1].x = GetMapCoord().x+1;
			testTile[1].y = GetMapCoord().y;
			if(Closed.size() > 0)//error check to make sure closed list not empty
			{
				for(unsigned int i = 0; i < Closed.size(); i++)//go through the list of points comparing each
				{
					if(testTile[1].x == Closed[i].x && testTile[1].y == Closed[i].y)//if == then already in list and no need to check distance
					{
						onList[i] = true;
					}
					if(onList[i] == false)
					{
						//set coordid to its respective tileID number
						coordID = testTile[1].y * CBattleMap::GetInstance()->GetNumCols() + testTile[1].x;
						if(m_pTilesL1[coordID].Flag() != FLAG_COLLISION)//check if that coorid is collision tile or not 
						{
							//get the distance from this coordinate to the endpt
							distance[1] = abs( (endPt.x - testTile[1].x)+(endPt.y - testTile[1].y) );
							//check if distance is < mindistance
							if(distance[1] < minDistance)
							{
								minDistance = distance[1];
								tileNum = 1;
							}
						}
						else//add the position to the closed list
						{
							Closed.push_back(testTile[1]);
						}
					}
				}
			}
		}

		if(GetMapCoord().y - 1 > -1 )//check if (y-1>-1)
		{
			//set those points to a new point called testTile
			testTile[2].x = GetMapCoord().x;
			testTile[2].y = GetMapCoord().y-1;
			if(Closed.size() > 0)//error check to make sure closed list not empty
			{
				for(unsigned int i = 0; i < Closed.size(); i++)//go through the list of points comparing each
				{
					if(testTile[2].x == Closed[i].x && testTile[2].y == Closed[i].y)//if == then already in list and no need to check distance
					{
						onList[i] = true;
					}
					if(onList[i] == false)
					{
						//set coordid to its respective tileID number
						coordID = testTile[2].y * CBattleMap::GetInstance()->GetNumCols() + testTile[2].x;
						if(m_pTilesL1[coordID].Flag() != FLAG_COLLISION)//check if that coorid is collision tile or not 
						{
							//get the distance from this coordinate to the endpt
							distance[2] = abs( (endPt.x - testTile[2].x)+(endPt.y - testTile[2].y) );
							//check if distance is < mindistance
							if(distance[2] < minDistance)
							{
								minDistance = distance[2];
								tileNum = 2;
							}
						}
						else//add the position to the closed list
						{
							Closed.push_back(testTile[2]);
						}
					}
				}
			}
		}


		if(GetMapCoord().y + 1 < CBattleMap::GetInstance()->GetNumCols() )//check if (y+1<getnumcols())
		{
			//set those points to a new point called testTile
			testTile[3].x = GetMapCoord().x;
			testTile[3].y = GetMapCoord().y+1;
			if(Closed.size() > 0)//error check to make sure closed list not empty
			{
				for(unsigned int i = 0; i < Closed.size(); i++)//go through the list of points comparing each
				{
					if(testTile[3].x == Closed[i].x && testTile[3].y == Closed[i].y)//if == then already in list and no need to check distance
					{
						onList[i] = true;
					}
					if(onList[i] == false)
					{
						//set coordid to its respective tileID number
						coordID = testTile[3].y * CBattleMap::GetInstance()->GetNumCols() + testTile[3].x;
						if(m_pTilesL1[coordID].Flag() != FLAG_COLLISION)//check if that coorid is collision tile or not 
						{
							//get the distance from this coordinate to the endpt
							distance[3] = abs( (endPt.x - testTile[3].x)+(endPt.y - testTile[3].y) );
							//check if distance is < mindistance
							if(distance[3] < minDistance)
							{
								minDistance = distance[3];
								tileNum = 3;
							}
						}
						else//add the position to the closed list
						{
							Closed.push_back(testTile[3]);
						}
					}
				}
			}
		}

	///////////////////////////////////////////////////////////////////////////////////////
	// select tile to move to and what to do next

		int damage = 0;
		switch(tileNum)
		{
		case 0:
			SetCurrTile(testTile[0], CBattleMap::GetInstance()->GetOffsetX(), CBattleMap::GetInstance()->GetOffsetY(),
				CBattleMap::GetInstance()->GetTileWidth(), CBattleMap::GetInstance()->GetTileHeight(), CBattleMap::GetInstance()->GetNumCols());
			SetCurrAP(GetCurrAP()-2);
			
			//animate movement
			//update position
			CBattleMap::GetInstance()->UpdatePositions();

			m_vClosedList = Closed;
			if( distance[0] > 0 && GetCurrAP() >= 2) //more than one space away and 2 ap left
			{
				FindPathX(endPt, Closed);
			}
			else if( distance[0] == 0 && GetCurrAP() >=4)//at one tile and enough ap to attack
			{
				//switch to check how much ap
				switch(GetCurrAP())
				{
				case 4:
					//attack
					damage = Random(20,30);
					m_pPlayer->GetTurtles()[m_nTurtle]->SetHealth(GetHealth()-damage);
					//end turn
					CBattleMap::GetInstance()->SetTurn(true);
					break;
				case 5:
					//attack
					 damage = Random(20,30);
					m_pPlayer->GetTurtles()[m_nTurtle]->SetHealth(GetHealth()-damage);
					//end turn
					CBattleMap::GetInstance()->SetTurn(true);
					break;
				case 6:
					//special attack
					damage = Random(40,50);
					m_pPlayer->GetTurtles()[m_nTurtle]->SetHealth(GetHealth()-damage);
					//end turn
					CBattleMap::GetInstance()->SetTurn(true);
					break;
				case 7:
					//special attack
					damage = Random(40,50);
					m_pPlayer->GetTurtles()[m_nTurtle]->SetHealth(GetHealth()-damage);
					//end turn
					CBattleMap::GetInstance()->SetTurn(true);
					break;
				case 8:
					//special attack
					damage = Random(40,50);
					m_pPlayer->GetTurtles()[m_nTurtle]->SetHealth(GetHealth()-damage);
					//end turn
					CBattleMap::GetInstance()->SetTurn(true);
					break;
				case 9:
					//special attack
					damage = Random(40,50);
					m_pPlayer->GetTurtles()[m_nTurtle]->SetHealth(GetHealth()-damage);
					//end turn
					CBattleMap::GetInstance()->SetTurn(true);
					break;
				case 10:
					//special attack && attack
					damage = Random(40,50);
					damage += Random(20,30);
					m_pPlayer->GetTurtles()[m_nTurtle]->SetHealth(GetHealth()-damage);
					//end turn
					CBattleMap::GetInstance()->SetTurn(true);
					break;
				case 11:
					//special attack && attack
					damage = Random(40,50);
					damage += Random(20,30);
					m_pPlayer->GetTurtles()[m_nTurtle]->SetHealth(GetHealth()-damage);
					//end turn
					CBattleMap::GetInstance()->SetTurn(true);
					break;
				case 12:
					//special attack x2
					damage = Random(40,50);
					damage += Random(40,50);
					m_pPlayer->GetTurtles()[m_nTurtle]->SetHealth(GetHealth()-damage);
					//end turn
					CBattleMap::GetInstance()->SetTurn(true);
					break;
				case 13:
					//special attack x2
					damage = Random(40,50);
					damage += Random(40,50);
					m_pPlayer->GetTurtles()[m_nTurtle]->SetHealth(GetHealth()-damage);
					//end turn
					CBattleMap::GetInstance()->SetTurn(true);
					break;
				case 14:
					//special attack1 then special attack 2
					damage = Random(40,50);
					damage += Random(50,60);
					m_pPlayer->GetTurtles()[m_nTurtle]->SetHealth(GetHealth()-damage);
					//end turn
					CBattleMap::GetInstance()->SetTurn(true);
					break;
				}
			}
			else
			{
				//end turn
				CBattleMap::GetInstance()->SetTurn(true);
			}
			break;
		case 1:
			SetCurrTile(testTile[1], CBattleMap::GetInstance()->GetOffsetX(), CBattleMap::GetInstance()->GetOffsetY(),
				CBattleMap::GetInstance()->GetTileWidth(), CBattleMap::GetInstance()->GetTileHeight(), CBattleMap::GetInstance()->GetNumCols());
			SetCurrAP(GetCurrAP()-2);
			//update position
			CBattleMap::GetInstance()->UpdatePositions();
			//animate movement
			m_vClosedList = Closed;
			if( distance[1] > 0 && GetCurrAP() >= 2) //more than one space away and 2 ap left
			{
				FindPathX(endPt, Closed);
			}
			else if( distance[1] == 0 && GetCurrAP() >=4)//at one tile and enough ap to attack
			{
				//switch to check how much ap
				switch(GetCurrAP())
				{
				case 4:
					//attack
					damage = Random(20,30);
					m_pPlayer->GetTurtles()[m_nTurtle]->SetHealth(GetHealth()-damage);
					//end turn
					CBattleMap::GetInstance()->SetTurn(true);
					break;
				case 5:
					//attack
					damage = Random(20,30);
					m_pPlayer->GetTurtles()[m_nTurtle]->SetHealth(GetHealth()-damage);
					//end turn
					CBattleMap::GetInstance()->SetTurn(true);
					break;
				case 6:
					//special attack
					damage = Random(40,50);
					m_pPlayer->GetTurtles()[m_nTurtle]->SetHealth(GetHealth()-damage);
					//end turn
					CBattleMap::GetInstance()->SetTurn(true);
					break;
				case 7:
					//special attack
					damage = Random(40,50);
					m_pPlayer->GetTurtles()[m_nTurtle]->SetHealth(GetHealth()-damage);
					//end turn
					CBattleMap::GetInstance()->SetTurn(true);
					break;
				case 8:
					//special attack
					damage = Random(40,50);
					m_pPlayer->GetTurtles()[m_nTurtle]->SetHealth(GetHealth()-damage);
					//end turn
					CBattleMap::GetInstance()->SetTurn(true);
					break;
				case 9:
					//special attack
					damage = Random(40,50);
					m_pPlayer->GetTurtles()[m_nTurtle]->SetHealth(GetHealth()-damage);
					//end turn
					CBattleMap::GetInstance()->SetTurn(true);
					break;
				case 10:
					//special attack && attack
					damage = Random(40,50);
					damage += Random(20,30);
					m_pPlayer->GetTurtles()[m_nTurtle]->SetHealth(GetHealth()-damage);
					//end turn
					CBattleMap::GetInstance()->SetTurn(true);
					break;
				case 11:
					//special attack && attack
					damage = Random(40,50);
					damage += Random(20,30);
					m_pPlayer->GetTurtles()[m_nTurtle]->SetHealth(GetHealth()-damage);
					//end turn
					CBattleMap::GetInstance()->SetTurn(true);
					break;
				case 12:
					//special attack x2
					damage = Random(40,50);
					damage += Random(40,50);
					m_pPlayer->GetTurtles()[m_nTurtle]->SetHealth(GetHealth()-damage);
					//end turn
					CBattleMap::GetInstance()->SetTurn(true);
					break;
				case 13:
					//special attack x2
					damage = Random(40,50);
					damage += Random(40,50);
					m_pPlayer->GetTurtles()[m_nTurtle]->SetHealth(GetHealth()-damage);
					//end turn
					CBattleMap::GetInstance()->SetTurn(true);
					break;
				case 14:
					//special attack1 then special attack 2
					damage = Random(40,50);
					damage += Random(50,60);
					m_pPlayer->GetTurtles()[m_nTurtle]->SetHealth(GetHealth()-damage);
					//end turn
					CBattleMap::GetInstance()->SetTurn(true);
					break;
				}
			}
			else
			{
				//end turn
				CBattleMap::GetInstance()->SetTurn(true);
			}
			break;
		case 2:
			SetCurrTile(testTile[2], CBattleMap::GetInstance()->GetOffsetX(), CBattleMap::GetInstance()->GetOffsetY(),
				CBattleMap::GetInstance()->GetTileWidth(), CBattleMap::GetInstance()->GetTileHeight(), CBattleMap::GetInstance()->GetNumCols());
			SetCurrAP(GetCurrAP()-2);
			//update position
			CBattleMap::GetInstance()->UpdatePositions();
			//animate movement
			m_vClosedList = Closed;
			if( distance[2] > 0 && GetCurrAP() >= 2) //more than one space away and 2 ap left
			{
				FindPathX(endPt, Closed);
			}
			else if( (distance[2] == 0) && GetCurrAP() >=4)//at one tile and enough ap to attack
			{
				//switch to check how much ap
				switch(GetCurrAP())
				{
				case 4:
					//attack
					damage = Random(20,30);
					m_pPlayer->GetTurtles()[m_nTurtle]->SetHealth(GetHealth()-damage);
					//end turn
					CBattleMap::GetInstance()->SetTurn(true);
					break;
				case 5:
					//attack
					damage = Random(20,30);
					m_pPlayer->GetTurtles()[m_nTurtle]->SetHealth(GetHealth()-damage);
					//end turn
					CBattleMap::GetInstance()->SetTurn(true);
					break;
				case 6:
					//special attack
					damage = Random(40,50);
					m_pPlayer->GetTurtles()[m_nTurtle]->SetHealth(GetHealth()-damage);
					//end turn
					CBattleMap::GetInstance()->SetTurn(true);
					break;
				case 7:
					//special attack
					damage = Random(40,50);
					m_pPlayer->GetTurtles()[m_nTurtle]->SetHealth(GetHealth()-damage);
					//end turn
					CBattleMap::GetInstance()->SetTurn(true);
					break;
				case 8:
					//special attack
					damage = Random(40,50);
					m_pPlayer->GetTurtles()[m_nTurtle]->SetHealth(GetHealth()-damage);
					//end turn
					CBattleMap::GetInstance()->SetTurn(true);
					break;
				case 9:
					//special attack
					damage = Random(40,50);
					m_pPlayer->GetTurtles()[m_nTurtle]->SetHealth(GetHealth()-damage);
					//end turn
					CBattleMap::GetInstance()->SetTurn(true);
					break;
				case 10:
					//special attack && attack
					damage = Random(40,50);
					damage += Random(20,30);
					m_pPlayer->GetTurtles()[m_nTurtle]->SetHealth(GetHealth()-damage);
					//end turn
					CBattleMap::GetInstance()->SetTurn(true);
					break;
				case 11:
					//special attack && attack
					damage = Random(40,50);
					damage += Random(20,30);
					m_pPlayer->GetTurtles()[m_nTurtle]->SetHealth(GetHealth()-damage);
					//end turn
					CBattleMap::GetInstance()->SetTurn(true);
					break;
				case 12:
					//special attack x2
					damage = Random(40,50);
					damage += Random(40,50);
					m_pPlayer->GetTurtles()[m_nTurtle]->SetHealth(GetHealth()-damage);
					//end turn
					CBattleMap::GetInstance()->SetTurn(true);
					break;
				case 13:
					//special attack x2
					damage = Random(40,50);
					damage += Random(40,50);
					m_pPlayer->GetTurtles()[m_nTurtle]->SetHealth(GetHealth()-damage);
					//end turn
					CBattleMap::GetInstance()->SetTurn(true);
					break;
				case 14:
					//special attack1 then special attack 2
					damage = Random(40,50);
					damage += Random(50,60);
					m_pPlayer->GetTurtles()[m_nTurtle]->SetHealth(GetHealth()-damage);
					//end turn
					CBattleMap::GetInstance()->SetTurn(true);
					break;
				}			
			}
			else
			{
				//end turn
				CBattleMap::GetInstance()->SetTurn(true);
			}
			break;
		case 3:
			SetCurrTile(testTile[3], CBattleMap::GetInstance()->GetOffsetX(), CBattleMap::GetInstance()->GetOffsetY(),
				CBattleMap::GetInstance()->GetTileWidth(), CBattleMap::GetInstance()->GetTileHeight(), CBattleMap::GetInstance()->GetNumCols());
			SetCurrAP(GetCurrAP()-2);
			//update position
			CBattleMap::GetInstance()->UpdatePositions();
			//animate movement
			m_vClosedList = Closed;
			if( distance[3] > 0 && GetCurrAP() >= 2) //more than one space away and 2 ap left
			{
				FindPathX(endPt, Closed);
			}
			else if( distance[3] == 0 && GetCurrAP() >=4)//at one tile and enough ap to attack
			{
				//switch to check how much ap
				switch(GetCurrAP())
				{
				case 4:
					//attack
					damage = Random(20,30);
					m_pPlayer->GetTurtles()[m_nTurtle]->SetHealth(GetHealth()-damage);
					//end turn
					CBattleMap::GetInstance()->SetTurn(true);
					break;
				case 5:
					//attack
					damage = Random(20,30);
					m_pPlayer->GetTurtles()[m_nTurtle]->SetHealth(GetHealth()-damage);
					//end turn
					CBattleMap::GetInstance()->SetTurn(true);
					break;
				case 6:
					//special attack
					damage = Random(40,50);
					m_pPlayer->GetTurtles()[m_nTurtle]->SetHealth(GetHealth()-damage);
					//end turn
					CBattleMap::GetInstance()->SetTurn(true);
					break;
				case 7:
					//special attack
					damage = Random(40,50);
					m_pPlayer->GetTurtles()[m_nTurtle]->SetHealth(GetHealth()-damage);
					//end turn
					CBattleMap::GetInstance()->SetTurn(true);
					break;
				case 8:
					//special attack
					damage = Random(40,50);
					m_pPlayer->GetTurtles()[m_nTurtle]->SetHealth(GetHealth()-damage);
					//end turn
					CBattleMap::GetInstance()->SetTurn(true);
					break;
				case 9:
					//special attack
					damage = Random(40,50);
					m_pPlayer->GetTurtles()[m_nTurtle]->SetHealth(GetHealth()-damage);
					//end turn
					CBattleMap::GetInstance()->SetTurn(true);
					break;
				case 10:
					//special attack && attack
					damage = Random(40,50);
					damage += Random(20,30);
					m_pPlayer->GetTurtles()[m_nTurtle]->SetHealth(GetHealth()-damage);
					//end turn
					CBattleMap::GetInstance()->SetTurn(true);
					break;
				case 11:
					//special attack && attack
					damage = Random(40,50);
					damage += Random(20,30);
					m_pPlayer->GetTurtles()[m_nTurtle]->SetHealth(GetHealth()-damage);
					//end turn
					CBattleMap::GetInstance()->SetTurn(true);
					break;
				case 12:
					//special attack x2
					damage = Random(40,50);
					damage += Random(40,50);
					m_pPlayer->GetTurtles()[m_nTurtle]->SetHealth(GetHealth()-damage);
					//end turn
					CBattleMap::GetInstance()->SetTurn(true);
					break;
				case 13:
					//special attack x2
					damage = Random(40,50);
					damage += Random(40,50);
					m_pPlayer->GetTurtles()[m_nTurtle]->SetHealth(GetHealth()-damage);
					//end turn
					CBattleMap::GetInstance()->SetTurn(true);
					break;
				case 14:
					//special attack1 then special attack 2
					damage = Random(40,50);
					damage += Random(50,60);
					m_pPlayer->GetTurtles()[m_nTurtle]->SetHealth(GetHealth()-damage);
					//end turn
					CBattleMap::GetInstance()->SetTurn(true);
					break;
				}
			}
			else
			{
				//end turn
				CBattleMap::GetInstance()->SetTurn(true);
			}
			break;
		default:
			//end turn
			CBattleMap::GetInstance()->SetTurn(true);
			break;

		}
	}

}

void CBoss::FindPathNew(POINT begin, POINT end)
{
	CBattleMap* bMap = CBattleMap::GetInstance();
	int numCols = bMap->GetNumCols();
	int numRows = bMap->GetNumRows();
	int totalTiles = numCols * numRows;
	m_vMoveList.clear();
	vector<CTile> open;
	vector<CTile> closed;
	bool bFound;

	// setup the grid of all map tiles that will be used during the pathfinding
	CTile* tiles = new CTile[totalTiles];
	for (int i = 0; i < totalTiles; ++i)
		tiles[i] = m_pTilesL1[i];

	CTile** adjTiles;	// ptr to the array of tiles surrounding the current tile to be checked
	
	//POINT current; current.x = begin.x; current.y = begin.y;
	int currTileID = begin.y * numCols + begin.x;
	CTile currTile = tiles[currTileID];
	open.push_back(currTile);

	// NOTE: end == 1 tile away from target
	// while ap > 2 and the distance > 0
	// GetCurrAP() >=2 && (abs(end.x - current.x)+abs(end.y - current.y)) > 0;
 	while(open.size() > 0 && (currTile.DestXID() != end.x || currTile.DestYID() != end.y) )	
	{
		adjTiles = bMap->GetAdjTiles(currTile.DestXID(), currTile.DestYID(), tiles);
		open.pop_back(); closed.push_back(currTile);

		//////////////////////////////////////////////////////////////////////////

		// check the adjacent tiles (adjacent to the current), determine which one's best - based on distance? 
		for (int ind = 0; ind < 4; ++ind)
		{
			if (adjTiles[ind] == NULL)
				continue;
			int tileID = adjTiles[ind]->DestYID() * numCols + adjTiles[ind]->DestXID();

			if (tiles[tileID].Flag() != FLAG_COLLISION && tiles[tileID].Flag() != FLAG_OBJECT_EDGE &&
				!IsOnClose(adjTiles[ind], closed))
			{
				bFound = false;
				for (unsigned int i = 0; i < open.size(); ++i)
				{
					if (open[i].DestXID() == adjTiles[ind]->DestXID() && open[i].DestYID() == adjTiles[ind]->DestYID())
					{bFound = true; break;}
				}
				if (!bFound)
				{
					adjTiles[ind]->SetCost(currTile.Cost()+adjTiles[ind]->TerrainCost());
					adjTiles[ind]->SetH(2*(abs(end.x - adjTiles[ind]->DestXID())+abs(end.y - adjTiles[ind]->DestYID())));
					adjTiles[ind]->SetF(adjTiles[ind]->Cost()+adjTiles[ind]->H()); 
					//adjTiles[ind]->SetParent(currTile);
					open.push_back(*adjTiles[ind]);
				}
				else // on open list, see if this path is better
				{
					if (currTile.Cost() + adjTiles[ind]->TerrainCost() < adjTiles[ind]->Cost())
					{
						//adjTiles[ind]->SetParent(currTile);
						adjTiles[ind]->SetCost(currTile.Cost()+adjTiles[ind]->TerrainCost());
						adjTiles[ind]->SetF(currTile.Cost()+adjTiles[ind]->H());
					}
				}
			}
		}
		// pick the next best tile
		if (open.size() > 0)
		{currTile = open[0];}
		CTile tempTile = currTile;

		for (unsigned int oInd = 1; oInd < open.size(); ++oInd)
			if (open[oInd].F() <= currTile.F() && open[oInd].DestXID() != tempTile.DestXID() && open[oInd].DestYID() != tempTile.DestYID())
				currTile = open[oInd];

		POINT pt; pt.x = currTile.DestXID(); pt.y = currTile.DestYID();
		if (m_vMoveList.size() < 8)
			m_vMoveList.push_back(pt);
		else
			break;
		delete[] adjTiles;
	}
	if(m_vMoveList.size() > 0)
		m_bMoving = true;
	else m_bMoving = false;

	delete[] tiles;
	m_ptCurrPos.x = begin.x; m_ptCurrPos.y = begin.y;
}

bool CBoss::IsOnClose(CTile* tile, vector<CTile>& closed)
{
	for (unsigned int i = 0; i < closed.size(); ++i)
		if (closed[i].DestXID() == tile->DestXID() && closed[i].DestYID() == tile->DestYID())
			return true;
	return false;
}

