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

enum { IDLE_1, IDLE_2, ATK_HIGH, ATK_LOW, ATK_SPECIAL = 7};

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
	m_nMoveListIndex = 0;
	m_bMoving = false;
	m_pPlayer = CPlayer::GetInstance();
	m_pTilesL1 = CBattleMap::GetInstance()->GetTiles();
	m_nType = OBJECT_BOSS;
	m_nDistance = 0;
	m_pBitmapFont = CBitmapFont::GetInstance();
	m_pBattleMap = CBattleMap::GetInstance();
	m_bAttacking = m_bMoving = m_bRenderCombText = m_bAttackDone = m_bDoDmg = false;
	changeX = changeY = 0;
}

CBoss::~CBoss(void)
{	
}


void CBoss::AI()
{
	changeX = changeY = 0;
	for (int i =0; i < 4; ++i)
	{m_pComb[i].damage = 0; m_pComb[i].attackID = -1;}
	
	m_nInRange = 20; // if no turtles are found in range, this signals a random move
	m_bMoving = m_bAttacking = m_bAttackDone = m_bDoDmg = m_bRenderCombText = false;
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
	if (m_nInRange > 8)
		m_nInRange = 20;
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
			int debug = 0;
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
	vector<CTile*> references;
	//POINT current; current.x = begin.x; current.y = begin.y;
	int currTileID = begin.y * numCols + begin.x;
	CTile currTile;
	currTile = tiles[currTileID];
	open.push_back(currTile);

	// NOTE: end == 1 tile away from target
	// while ap > 2 and the distance > 0
	// GetCurrAP() >=2 && (abs(end.x - current.x)+abs(end.y - current.y)) > 0;
	bool pathFound = false;
	while( !pathFound )	
	{
		for (unsigned int oInd = 1; oInd < open.size(); ++oInd)
			if (open[oInd].F() <= currTile.F() /*&& open[oInd].DestXID() != tempTile.DestXID() && open[oInd].DestYID() != tempTile.DestYID()*/)
				currTile = open[oInd];
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
					CTile* parent = new CTile();
					*parent = currTile;
					references.push_back(parent);
					adjTiles[ind]->SetCost(currTile.Cost()+adjTiles[ind]->TerrainCost());
					adjTiles[ind]->SetH(2*(abs(end.x - adjTiles[ind]->DestXID())+abs(end.y - adjTiles[ind]->DestYID())));
					adjTiles[ind]->SetF(adjTiles[ind]->Cost()+adjTiles[ind]->H()); 
					adjTiles[ind]->SetParent(parent);
					open.push_back(*adjTiles[ind]);
				}
				else // on open list, see if this path is better
				{
					if (currTile.Cost() + adjTiles[ind]->TerrainCost() < adjTiles[ind]->Cost())
					{
						CTile* parent = new CTile();
						*parent = currTile;
						references.push_back(parent);
						adjTiles[ind]->SetParent(parent);
						adjTiles[ind]->SetCost(currTile.Cost()+adjTiles[ind]->TerrainCost());
						adjTiles[ind]->SetF(currTile.Cost()+adjTiles[ind]->H());
					}
				}
			}
		}
		if(open.size() == 0)
			break;
		else if (currTile.DestXID() == end.x && currTile.DestYID() == end.y)
		{pathFound = true; delete[] adjTiles; break;}
		delete[] adjTiles;
		currTile = open[0];
	}
	while (true)
	{
		POINT pt; pt.x = currTile.DestXID(); pt.y = currTile.DestYID();
		if (currTile.DestXID() == begin.x && currTile.DestYID() == begin.y)
			break;
		if (m_vMoveList.size() < 8)
			m_vMoveList.push_back(pt);
		else if (m_vMoveList.size() == 8)
			break;
		if (currTile.Parent() == NULL)
			break;
		currTile = *(currTile.Parent());
	}
	int count = 0;
	while (count < (int)references.size())
	{
		CTile* t = references[count];
		delete t; count++;
	}

	if(m_vMoveList.size() > 0)
		m_bMoving = true;
	else m_bMoving = false;

	delete[] tiles;
	m_ptStartXY.x = GetPosX(); m_ptStartXY.y = GetPosY();
	m_nMoveListIndex = m_vMoveList.size()-1;
}

void CBoss::Update(float fElapsedTime)
{ 
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

	//////////////////////////////////////////////////////////////////////////
	// handle timer events
	bool bTimeEvent = m_Timer.Update(fElapsedTime);
	if (bTimeEvent)
		HandleTimerEvent();

	if(m_nCurrAnimation > -1)
		m_vAnimations[m_nCurrAnimation].Update(fElapsedTime);

	if(m_bMoving && m_nMoveListIndex > -1)
	{
		m_ptCurrPos.x = GetPosX();
		m_ptCurrPos.y = GetPosY();

		//get start position and next tile
		if(GetMapCoord().x > m_vMoveList[m_nMoveListIndex].x) //move left one x tile(on screen)
		{
			//32 pixels left 16 pixels up
			if( abs(m_ptStartXY.x - m_ptCurrPos.x) < 32.0f && abs(m_ptStartXY.y - m_ptCurrPos.y) < 16.0f)
			{
				changeX = MINUS_X; changeY = MINUS_Y;
				m_ptCurrPos.x -= GetVelX() * fElapsedTime;
				m_ptCurrPos.y -= GetVelY() * fElapsedTime;
				SetPosPtF(m_ptCurrPos);
			}
		}
		else if(GetMapCoord().x < m_vMoveList[m_nMoveListIndex].x)//move right one x tile(on screen)
		{
			//32 pixels right  16 pixels down
			if( abs(m_ptStartXY.x - m_ptCurrPos.x) < 32.0f && abs(m_ptStartXY.y - m_ptCurrPos.y) < 16.0f)
			{
				changeX = ADD_X; changeY = ADD_Y;
				m_ptCurrPos.x += GetVelX() * fElapsedTime;
				m_ptCurrPos.y += GetVelY() * fElapsedTime;
				SetPosPtF(m_ptCurrPos);
			}
		}
		else if(GetMapCoord().y > m_vMoveList[m_nMoveListIndex].y)//move up one tile y(on screen)
		{
			//32 pixels up and 16 pixels right 
			if( abs(m_ptStartXY.x - m_ptCurrPos.x) < 32.0f && abs(m_ptStartXY.y - m_ptCurrPos.y) < 16.0f)
			{
				changeX = ADD_X; changeY = MINUS_Y;
				m_ptCurrPos.x += GetVelX() * fElapsedTime;
				m_ptCurrPos.y -= GetVelY() * fElapsedTime;
				SetPosPtF(m_ptCurrPos);
			}
		}
		else if(GetMapCoord().y < m_vMoveList[m_nMoveListIndex].y)//move down one tile y(on screen)
		{
			//32 pixels down and 16 pixels left
			if( abs(m_ptStartXY.x - m_ptCurrPos.x) < 32.0f && abs(m_ptStartXY.y - m_ptCurrPos.y) < 16.0f)
			{
				changeX = MINUS_X; changeY = ADD_Y;
				m_ptCurrPos.x -= GetVelX() * fElapsedTime;
				m_ptCurrPos.y += GetVelY() * fElapsedTime;
				SetPosPtF(m_ptCurrPos);
			}
		}
		if( abs(m_ptStartXY.x - m_ptCurrPos.x) >= 32.0f && abs(m_ptStartXY.y - m_ptCurrPos.y) >= 16.0f)
		{
			SetCurrTile(m_vMoveList[m_nMoveListIndex], CBattleMap::GetInstance()->GetOffsetX(), CBattleMap::GetInstance()->GetOffsetY(),
			CBattleMap::GetInstance()->GetTileWidth(), CBattleMap::GetInstance()->GetTileHeight(), CBattleMap::GetInstance()->GetNumCols());
			SetCurrAP(GetCurrAP()-2);
			--m_nMoveListIndex;
			if ( m_nMoveListIndex > -1 )
			{
				if (changeX == MINUS_X)
					m_ptStartXY.x -= (m_pBattleMap->GetTileWidth()>>1);
				else
					m_ptStartXY.x += (m_pBattleMap->GetTileWidth()>>1); 
				if (changeY == MINUS_Y)
					m_ptStartXY.y -= (m_pBattleMap->GetTileHeight()>>1);
				else
					m_ptStartXY.y += (m_pBattleMap->GetTileHeight()>>1);
				changeX = changeY = 0;
			}
			CBattleMap::GetInstance()->UpdatePositions();
		}

		// movement is done
		if(GetCurrAP() < 2 || m_nMoveListIndex == -1)
		{
			m_vMoveList.clear();
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
	else if (m_bAttacking)
	{
		// execute the attacks, keeps track of which attacks are to happen
		// only grab the next one if there is one, the anim index is > -1, and the currAnim is done playing
		if (m_pComb[0].attackID > -1 && m_nCurrAnimInd > -1 && !m_vAnimations[m_nCurrAnimInd].IsAnimationPlaying())
		{
			for (int i = 0; i < 4; ++i)
			{
				if (m_pComb[i].attackID > -1)
				{
					m_nCurrAnimInd	= m_pComb[i].attackID;
					m_nDamage		= m_pComb[i].damage;
					SetCurrAnim(m_nCurrAnimInd);
					m_bRenderCombText = true; m_bDoDmg = true;	
					m_Timer.StartTimer(1.2f);	// render combat text for a bit
					break;
				}
				else // we hit the end
					{m_bAttacking = false; m_bAttackDone = true; m_Timer.StartTimer(1.2f)/*slight delay before player's turn*/; }
			}
			return;
		}
		if (m_nCurrAnimInd == -1)	// should only get in here once
		{
			// this will get the AP when boss first starts attacking and determine which attacks to execute
			switch(GetCurrAP())
			{
			case 4:
				{
					// one normal high attack
					m_nCurrAnimInd = ATK_HIGH;
					DecrementCurrAP(4);
					m_pComb[0].attackID = ATK_HIGH;
					m_pComb[0].damage = Random(35, 45);	// only one attack, calculate dmg now
				}
				break;
			case 6:
				{
					// one normal low attack
					m_nCurrAnimInd = ATK_LOW;
					DecrementCurrAP(6);
					m_pComb[0].attackID = ATK_LOW;
					m_pComb[0].damage = Random(50, 60);	// only one attack, calculate dmg now
				}
				break;
			case 8:
				{
					// a low and a high
					m_nCurrAnimInd = ATK_LOW;
					DecrementCurrAP(8);
					m_pComb[0].attackID = ATK_LOW;
					m_pComb[0].damage = Random(50, 60);	// only one attack, calculate dmg now
					m_pComb[1].attackID = ATK_HIGH;
					m_pComb[1].damage = Random(50, 60);	// only one attack, calculate dmg now
				}
				break;
			case 10:
				{
					// a low and a high
					m_nCurrAnimInd = ATK_LOW;
					DecrementCurrAP(10);
					m_pComb[0].attackID = ATK_LOW;
					m_pComb[0].damage = Random(50, 60);	// only one attack, calculate dmg now
					m_pComb[1].attackID = ATK_HIGH;
					m_pComb[1].damage = Random(50, 60);	// only one attack, calculate dmg now
				}
				break;
			case 12:
				{
					// a ground pound
					m_nCurrAnimInd = ATK_SPECIAL;
					DecrementCurrAP(12);
					m_pComb[0].attackID = ATK_SPECIAL;
					m_pComb[0].damage = Random(70, 85);	// only one attack, calculate dmg now
				}
				break;
			case 16:
				{
					// a low and a ground pound
					m_nCurrAnimInd = ATK_LOW;
					DecrementCurrAP(16);
					m_pComb[0].attackID = ATK_LOW;
					m_pComb[0].damage = Random(30, 40);	// only one attack, calculate dmg now
					m_pComb[1].attackID = ATK_SPECIAL;
					m_pComb[1].damage = Random(70, 80);	// only one attack, calculate dmg now
				}
				break;
			default:
				{
					int debug = 0;
				}
				break;
			}
		}
	}
}
void CBoss::Render()
{
	m_vAnimations[m_nCurrAnimation].Render((int)GetPosX()+m_vAnimations[0].GetFrames()[0].nAnchorX, (int)GetPosY()+m_vAnimations[0].GetFrames()[0].nAnchorY, GetPosZ(), 1, m_dwColor);
	if(m_bRenderCombText)
	{
		int offset = (int)(m_Timer.GetElapsed()*20.0f )-15;

		m_pBitmapFont->ChangeBMFont(CAssets::GetInstance()->aBitmapFontBubblyID,16,15,20);

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


void CBoss::HandleTimerEvent()
{
	if (m_bAttackDone)
	{
		SetCurrAnim(rand() % 2);
		CBattleMap::GetInstance()->UpdatePositions();
		CBattleMap::GetInstance()->NinjaMoveComplete();
		CBattleMap::GetInstance()->SetTurn(true);
	}
	if (m_bRenderCombText)
	{
		m_bRenderCombText = false;
	}
	if (m_bDoDmg)
	{
		CTurtle** turtles = m_pPlayer->GetTurtles();
		turtles[m_nTurtle]->SetHealth(turtles[m_nTurtle]->GetHealth() - m_nDamage);
	}
}

bool CBoss::IsOnClose(CTile* tile, vector<CTile>& closed)
{
	for (unsigned int i = 0; i < closed.size(); ++i)
		if (closed[i].DestXID() == tile->DestXID() && closed[i].DestYID() == tile->DestYID())
			return true;
	return false;
}