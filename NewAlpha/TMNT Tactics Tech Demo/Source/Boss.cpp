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

enum {MINUS_X, MINUS_Y, ADD_X, ADD_Y, };

int Random(int min, int max)
{
	int number = abs(max) - abs(min);
	return ((rand()%number) + abs(min));
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
	m_pTile = CBattleMap::GetInstance()->GetTiles();
	m_nType = OBJECT_BOSS;
	m_vClosedList.clear();
	m_vMoveList.clear();
	m_nDistance = 0;
	//m_vPath = 0;
	//m_ptStartXY =0;	
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
			m_pTile[ptCurr.y * CBattleMap::GetInstance()->GetNumCols() + (ptCurr.x+1)].Flag() == FLAG_NONE &&
			!CBattleMap::GetInstance()->CheckTileOccupied(ptCurr.y * CBattleMap::GetInstance()->GetNumCols() + (ptCurr.x+1)))
		{ ++ptCurr.x; continue; }

		if (ptCurr.x > ptTarget.x && (oldX != ptCurr.x - 1) && 
			m_pTile[ptCurr.y * CBattleMap::GetInstance()->GetNumCols() + (ptCurr.x-1)].Flag() == FLAG_NONE &&
			!CBattleMap::GetInstance()->CheckTileOccupied(ptCurr.y * CBattleMap::GetInstance()->GetNumCols() + (ptCurr.x-1)))
		{ --ptCurr.x; continue; }

		if (ptCurr.y < ptTarget.y && (oldY != ptCurr.y + 1) && 
			m_pTile[(ptCurr.y+1) * CBattleMap::GetInstance()->GetNumCols() + ptCurr.x].Flag() == FLAG_NONE &&
			!CBattleMap::GetInstance()->CheckTileOccupied((ptCurr.y+1) * CBattleMap::GetInstance()->GetNumCols() + ptCurr.x))
		{ ++ptCurr.y; continue; }

		if (ptCurr.y > ptTarget.y && (oldY != ptCurr.y - 1) && 
			m_pTile[(ptCurr.y-1) * CBattleMap::GetInstance()->GetNumCols() + ptCurr.x].Flag() == FLAG_NONE &&
			!CBattleMap::GetInstance()->CheckTileOccupied((ptCurr.y-1) * CBattleMap::GetInstance()->GetNumCols() + ptCurr.x))
		{ --ptCurr.y; continue; }

		if (ptCurr.x == ptTarget.x)
		{
			if (oldX != ptCurr.x+1 && m_pTile[ptCurr.y * CBattleMap::GetInstance()->GetNumCols() + ptCurr.x+1].Flag() == FLAG_NONE &&
				!CBattleMap::GetInstance()->CheckTileOccupied(ptCurr.y * CBattleMap::GetInstance()->GetNumCols() + ptCurr.x+1) )
			{ ++ptCurr.x; continue; }
			if (oldX != ptCurr.x-1 && m_pTile[ptCurr.y * CBattleMap::GetInstance()->GetNumCols() + ptCurr.x-1].Flag() == FLAG_NONE &&
				!CBattleMap::GetInstance()->CheckTileOccupied(ptCurr.y * CBattleMap::GetInstance()->GetNumCols() + ptCurr.x-1))
			{ --ptCurr.x; continue; }
		}
		else if (ptCurr.y == ptTarget.y)
		{
			if (oldY != ptCurr.y+1 && m_pTile[(ptCurr.y+1) * CBattleMap::GetInstance()->GetNumCols() + ptCurr.x].Flag() == FLAG_NONE &&
				!CBattleMap::GetInstance()->CheckTileOccupied((ptCurr.y+1) * CBattleMap::GetInstance()->GetNumCols() + ptCurr.x))
			{ ++ptCurr.y; continue; }
			if (oldY != ptCurr.y-1 && m_pTile[(ptCurr.y-1) * CBattleMap::GetInstance()->GetNumCols() + ptCurr.x].Flag() == FLAG_NONE &&
				!CBattleMap::GetInstance()->CheckTileOccupied((ptCurr.y-1) * CBattleMap::GetInstance()->GetNumCols() + ptCurr.x))
			{ --ptCurr.y; continue; }
		}

		if ( (ptCurr.x > ptTarget.x) && (oldX != ptCurr.x+1) && m_pTile[ptCurr.y * CBattleMap::GetInstance()->GetNumCols() + (ptCurr.x+1)].Flag() == FLAG_NONE &&
			!CBattleMap::GetInstance()->CheckTileOccupied(ptCurr.y * CBattleMap::GetInstance()->GetNumCols() + (ptCurr.x+1)))
		{++ptCurr.x; continue;}
		else if (oldX != ptCurr.x-1 && m_pTile[ptCurr.y * CBattleMap::GetInstance()->GetNumCols() + (ptCurr.x-1)].Flag() == FLAG_NONE &&
			!CBattleMap::GetInstance()->CheckTileOccupied(ptCurr.y * CBattleMap::GetInstance()->GetNumCols() + (ptCurr.x-1)))
		{--ptCurr.x; continue;}

		if ( (ptCurr.y > ptTarget.y) && (oldY != ptCurr.y+1) && m_pTile[(ptCurr.y+1) * CBattleMap::GetInstance()->GetNumCols() + ptCurr.x].Flag() == FLAG_NONE &&
			!CBattleMap::GetInstance()->CheckTileOccupied((ptCurr.y+1) * CBattleMap::GetInstance()->GetNumCols() + ptCurr.x))
		{++ptCurr.y; continue;}
		else if (oldY != ptCurr.y-1 && m_pTile[(ptCurr.y-1) * CBattleMap::GetInstance()->GetNumCols() + ptCurr.x].Flag() == FLAG_NONE &&
			!CBattleMap::GetInstance()->CheckTileOccupied((ptCurr.y-1) * CBattleMap::GetInstance()->GetNumCols() + ptCurr.x))
		{--ptCurr.y; continue;}
		pathWeight = 10000; break;
	}
	if (m_vPath.size() > 0)
	{
		m_ptStartXY.x = GetPosX();
		m_ptStartXY.y = GetPosY();
		for (unsigned int i = 0; i < m_vPath.size(); ++i)
			m_pTile[m_vPath[i].y * CBattleMap::GetInstance()->GetNumCols() + m_vPath[i].x].SetAlpha(199);
	}
}

void CBoss::AI()
{
	//check distance
	SetCurrAP(16);
	
	//set temporary ranges
	int tempRange1 = abs(GetMapCoord().x - m_pPlayer->GetTurtles()[MIKEY]->GetMapCoord().x) + 
						abs(GetMapCoord().y - m_pPlayer->GetTurtles()[MIKEY]->GetMapCoord().y);
	int tempRange2 = abs(GetMapCoord().x - m_pPlayer->GetTurtles()[LEONARDO]->GetMapCoord().x) +
						abs(GetMapCoord().y - m_pPlayer->GetTurtles()[LEONARDO]->GetMapCoord().y );
	int tempRange3 = abs(GetMapCoord().x - m_pPlayer->GetTurtles()[DONATELLO]->GetMapCoord().x) +
						abs(GetMapCoord().y - m_pPlayer->GetTurtles()[DONATELLO]->GetMapCoord().y);
	int tempRange4 = abs(GetMapCoord().x - m_pPlayer->GetTurtles()[RAPHAEL]->GetMapCoord().x) +
						abs(GetMapCoord().y - m_pPlayer->GetTurtles()[RAPHAEL]->GetMapCoord().y);


	//set range to 100 if turtle is dead
	if( m_pPlayer->GetTurtles()[MIKEY]->GetAlive() == false)
	{
		tempRange1 = 100;
	}
	if(m_pPlayer->GetTurtles()[LEONARDO]->GetAlive() == false)
	{
		tempRange2 = 100;
	}
	if( m_pPlayer->GetTurtles()[DONATELLO]->GetAlive() == false)
	{
		tempRange3 = 100;
	}
	if( m_pPlayer->GetTurtles()[RAPHAEL]->GetAlive() == false)
	{
		tempRange4 = 100;
	}

	//checking if mikey is closest
	if( m_pPlayer->GetTurtles()[MIKEY]->GetAlive() == true)
	{
		if(tempRange1 < tempRange2)
		{
			if(tempRange1 < tempRange3)
			{
				if(tempRange1 < tempRange4)
				{
					m_nInRange = tempRange1;
					m_nXChange = m_pPlayer->GetTurtles()[MIKEY]->GetMapCoord().x - GetMapCoord().x;
					m_nYChange = m_pPlayer->GetTurtles()[MIKEY]->GetMapCoord().y - GetMapCoord().y ; 
					m_nTurtle = MIKEY;
				}
			}
		}
	}
	
	//check if leo is closest
	if( m_pPlayer->GetTurtles()[LEONARDO]->GetAlive() == true)
	{
		if(tempRange2 < tempRange1)
		{
			if(tempRange2 < tempRange3)
			{
				if(tempRange2 < tempRange4)
				{
					m_nInRange = tempRange2;
					m_nXChange =  m_pPlayer->GetTurtles()[LEONARDO]->GetMapCoord().x - GetMapCoord().x;
					m_nYChange =  m_pPlayer->GetTurtles()[LEONARDO]->GetMapCoord().y - GetMapCoord().y;
					m_nTurtle = LEONARDO;
				}
			}
		}
	}

	//check if donny is closest
	if( m_pPlayer->GetTurtles()[DONATELLO]->GetAlive() == true)
	{
		if(tempRange3 < tempRange1)
		{
			if(tempRange3 < tempRange2)
			{
				if(tempRange3 < tempRange4)
				{
					m_nInRange = tempRange3;
					m_nXChange =  m_pPlayer->GetTurtles()[DONATELLO]->GetMapCoord().x - GetMapCoord().x;
					m_nYChange =  m_pPlayer->GetTurtles()[DONATELLO]->GetMapCoord().y - GetMapCoord().y; 
					m_nTurtle = DONATELLO;
				}
			}
		}
	}

	//check if raph is closest
	if( m_pPlayer->GetTurtles()[RAPHAEL]->GetAlive() == true)
	{
		if(tempRange4 < tempRange1)
		{
			if(tempRange4 < tempRange2)
			{
				if(tempRange4 < tempRange3)
				{
					m_nInRange = tempRange4;
					m_nXChange = m_pPlayer->GetTurtles()[RAPHAEL]->GetMapCoord().x - GetMapCoord().x ;
					m_nYChange =  m_pPlayer->GetTurtles()[RAPHAEL]->GetMapCoord().y - GetMapCoord().y; 
					m_nTurtle = RAPHAEL;
				}
			}
		}
	}

	POINT begin = GetMapCoord();
	//safe check to see if turtle is chosen if all turtles are == distance
	if(m_nTurtle <= -1)
	{
		m_nTurtle = 0;
	}
	POINT end;
	POINT mapPt;
	//switch case for distance 0 - 8

	switch(m_nInRange)
	{
		//on turtle
	case 0:
		{
		}
		break;
		//one tile away from  turtle
	case 1:
		{
			//attack four times(4ap * 4 = 16)
			//m_pPlayer->GetTurtles()[m_nTurtle]->SetHealth(m_pPlayer->GetTurtles()[m_nTurtle]->GetHealth() - (20 * 4));
			//end turn
			//SetCurrAP(0);
			//TODO::wait till attack is done to end the turn? would require actually decrementing AP when the attack animation was played
			/*CBattleMap::GetInstance()->UpdatePositions();
			CBattleMap::GetInstance()->NinjaMoveComplete();
			CBattleMap::GetInstance()->SetTurn(true);*/
			switch(GetCurrAP())
				{
					SetCurrAnim(2);
				case 4:
					{
						m_pPlayer->GetTurtles()[m_nTurtle]->SetHealth(m_pPlayer->GetTurtles()[m_nTurtle]->GetHealth() - (20));	
					}
					break;
				case 6:
					{
						//use skill low punch , sweep
						//do 40+ damage 
						int damage = Random(30, 50);//damage
						m_pPlayer->GetTurtles()[m_nTurtle]->SetHealth(m_pPlayer->GetTurtles()[m_nTurtle]->GetHealth() - damage);

						

					}
					break;
				case 8:
					{
						m_pPlayer->GetTurtles()[m_nTurtle]->SetHealth(m_pPlayer->GetTurtles()[m_nTurtle]->GetHealth() - (20 * 2));
					}
					break;
				case 10:
					{
						//use skill punch, kick
						//do 60+ damage
						int damage = Random(45, 65);//damage
						m_pPlayer->GetTurtles()[m_nTurtle]->SetHealth(m_pPlayer->GetTurtles()[m_nTurtle]->GetHealth() - damage);
					}
					break;
				case 12:
					{
						m_pPlayer->GetTurtles()[m_nTurtle]->SetHealth(m_pPlayer->GetTurtles()[m_nTurtle]->GetHealth() - (20 * 3));
					}
					break;
				case 16:
					{
						//use skill low punch, sweep then use skill punck, kick
						int damage = Random(80, 100);
						m_pPlayer->GetTurtles()[m_nTurtle]->SetHealth(m_pPlayer->GetTurtles()[m_nTurtle]->GetHealth() - damage);
					}
					break;
				default:
					{
					}
					break;
				}
				//end shredder turn
				CBattleMap::GetInstance()->UpdatePositions();
				CBattleMap::GetInstance()->NinjaMoveComplete();
				CBattleMap::GetInstance()->SetTurn(true);

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
			/*SetCurrTile(mapPt, CBattleMap::GetInstance()->GetOffsetX(), CBattleMap::GetInstance()->GetOffsetY(), 
				CBattleMap::GetInstance()->GetTileWidth(), CBattleMap::GetInstance()->GetTileHeight(), 
				CBattleMap::GetInstance()->GetNumCols(), true);*/

			//attack 3 times (4ap * 3 = 12)
			//m_pPlayer->GetTurtles()[m_nTurtle]->SetHealth(m_pPlayer->GetTurtles()[m_nTurtle]->GetHealth() - (20 * 3));
			//end turn
			
			//CBattleMap::GetInstance()->SetTurn(true);
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
			
			/*SetCurrTile(mapPt, CBattleMap::GetInstance()->GetOffsetX(), CBattleMap::GetInstance()->GetOffsetY(), 
				CBattleMap::GetInstance()->GetTileWidth(), CBattleMap::GetInstance()->GetTileHeight(), 
				CBattleMap::GetInstance()->GetNumCols(), true);*/		

			//attack three times(12ap)= 16ap
			//m_pPlayer->GetTurtles()[m_nTurtle]->SetHealth(m_pPlayer->GetTurtles()[m_nTurtle]->GetHealth() - ( 20 * 3));
			//end turn
			
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
			
			/*SetCurrTile(mapPt, CBattleMap::GetInstance()->GetOffsetX(), CBattleMap::GetInstance()->GetOffsetY(), 
				CBattleMap::GetInstance()->GetTileWidth(), CBattleMap::GetInstance()->GetTileHeight(), 
				CBattleMap::GetInstance()->GetNumCols(), true);*/
	

			//attack twice(8ap) = 14ap
			//m_pPlayer->GetTurtles()[m_nTurtle]->SetHealth(m_pPlayer->GetTurtles()[m_nTurtle]->GetHealth() - (20 * 2));
			//end turn
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
			
			/*SetCurrTile(mapPt, CBattleMap::GetInstance()->GetOffsetX(), CBattleMap::GetInstance()->GetOffsetY(), 
				CBattleMap::GetInstance()->GetTileWidth(), CBattleMap::GetInstance()->GetTileHeight(), 
				CBattleMap::GetInstance()->GetNumCols(), true);*/

			//attack twice(8ap) = 16ap
			//m_pPlayer->GetTurtles()[m_nTurtle]->SetHealth(m_pPlayer->GetTurtles()[m_nTurtle]->GetHealth() - (20 * 2));
			//end turn
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
			
			/*SetCurrTile(mapPt, CBattleMap::GetInstance()->GetOffsetX(), CBattleMap::GetInstance()->GetOffsetY(), 
				CBattleMap::GetInstance()->GetTileWidth(), CBattleMap::GetInstance()->GetTileHeight(), 
				CBattleMap::GetInstance()->GetNumCols(), true);*/

			//attack one(4ap) = 14ap
			//m_pPlayer->GetTurtles()[m_nTurtle]->SetHealth(m_pPlayer->GetTurtles()[m_nTurtle]->GetHealth() - (20));
			//end turn
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
			
			/*SetCurrTile(mapPt, CBattleMap::GetInstance()->GetOffsetX(), CBattleMap::GetInstance()->GetOffsetY(), 
				CBattleMap::GetInstance()->GetTileWidth(), CBattleMap::GetInstance()->GetTileHeight(), 
				CBattleMap::GetInstance()->GetNumCols(), true);*/

			//attack once(4ap) = 16ap
			//m_pPlayer->GetTurtles()[m_nTurtle]->SetHealth(m_pPlayer->GetTurtles()[m_nTurtle]->GetHealth() - (20));
			//end turn
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
			

			/*SetCurrTile(mapPt, CBattleMap::GetInstance()->GetOffsetX(), CBattleMap::GetInstance()->GetOffsetY(), 
				CBattleMap::GetInstance()->GetTileWidth(), CBattleMap::GetInstance()->GetTileHeight(), 
				CBattleMap::GetInstance()->GetNumCols(), true);*/


		}
		break;
	default:
		{
			//end turn
			//SetCurrAP(0);
			////TODO::wait till attack is done to end the turn? would require actually decrementing AP when the attack animation was played
			//CBattleMap::GetInstance()->UpdatePositions();
			//CBattleMap::GetInstance()->NinjaMoveComplete();
			CBattleMap::GetInstance()->SetTurn(true);
		}
		break;
	}


}
void CBoss::Update(float fElapsedTime)
{ 

	if(m_bMoving == true)
	{
		m_ptStartXY.x = GetPosX();
		m_ptStartXY.y = GetPosY();

		//get start position and next tile
		if(GetMapCoord().x > m_vMoveList[0].x) //move left one x tile(on screen)
		{
			//32 pixels left 16 pixels up
			m_ptCurrPos.x = GetPosX();
			m_ptCurrPos.y = GetPosY();
			if( abs(m_ptStartXY.x - m_ptCurrPos.x) < 32.0f && abs(m_ptStartXY.y - m_ptCurrPos.y) < 16.0f)
			{
				m_ptCurrPos.x  -= GetVelX() * fElapsedTime;
				m_ptCurrPos.y -= GetVelY() * fElapsedTime;
				SetPosPtF(m_ptCurrPos);
			}
		}
		else if(GetMapCoord().x < m_vMoveList[0].x)//move right one x tile(on screen)
		{
			//32 pixels right  16 pixels down
			m_ptCurrPos.x = GetPosX();
			m_ptCurrPos.y = GetPosY();
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
			m_ptCurrPos.x = GetPosX();
			m_ptCurrPos.y = GetPosY();
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
			m_ptCurrPos.x = GetPosX();
			m_ptCurrPos.y = GetPosY();
			if( abs(m_ptStartXY.x - m_ptCurrPos.x) < 32.0f && abs(m_ptStartXY.y - m_ptCurrPos.y) < 16.0f)
			{
				m_ptCurrPos.x -= GetVelX() * fElapsedTime;
				m_ptCurrPos.y += GetVelY() * fElapsedTime;
				SetPosPtF(m_ptCurrPos);
			}
		}


		if( abs(m_ptStartXY.x - m_ptCurrPos.x) >= 32.0f && abs(m_ptStartXY.y - m_ptCurrPos.y) > 16.0f)
		{
			SetCurrTile(m_vMoveList[0], CBattleMap::GetInstance()->GetOffsetX(), CBattleMap::GetInstance()->GetOffsetY(),
			CBattleMap::GetInstance()->GetTileWidth(), CBattleMap::GetInstance()->GetTileHeight(), CBattleMap::GetInstance()->GetNumCols());
			SetCurrAP(GetCurrAP()-2);
			vector<POINT>::iterator first = m_vMoveList.begin();
			m_vMoveList.erase(first);
			CBattleMap::GetInstance()->UpdatePositions();
		}

		if(m_vMoveList.size() <= 0)
		{
			m_vMoveList.clear();
			m_bMoving = false;
			CBattleMap::GetInstance()->SetTurn(true);
		}
	}

	//CBase::Update(fElapsedTime);
	if(!GetCurrAnim()->IsAnimationPlaying())
		SetCurrAnim(1);
	m_vAnimations[m_nCurrAnimation].Update(fElapsedTime);
	// a ninja has been moved...execute the animation and position change over time
//	if (m_bMoving == true)
//	{
//		if (m_vPath.size() > 0)
//		{
//			// grab the next move and take it out of the vector..if the previous move is complete
//			POINT newPoint = m_vPath[0];
//			// set up variables
//			POINT currPoint= GetMapCoord();
//			MY_POINT_FLOAT currPos; 
//			currPos.x = GetPosX(); currPos.y = GetPosY();
//
//			// NORTHWEST
//			if ( newPoint.x < currPoint.x && abs(m_ptStartXY.x - currPos.x) < 32 && abs(m_ptStartXY.y - currPos.y) < 16)
//			{
//				SetCurrAnimFacing(true);
//
//				currPos.x -= GetVelX() * fElapsedTime;
//				currPos.y -= GetVelY() * fElapsedTime;
//				SetPosPtF(currPos);
//			}
//			// SOUTHEAST
//			else if ( newPoint.x > currPoint.x && abs(m_ptStartXY.x - currPos.x) < 32 && abs(m_ptStartXY.y - currPos.y) < 16)
//			{
//				SetCurrAnimFacing(false);
//
//				currPos.x += GetVelX() * fElapsedTime;
//				currPos.y += GetVelY() * fElapsedTime;
//				SetPosPtF(currPos);
//			}
//			// NORTHEAST
//			if ( newPoint.y < currPoint.y && abs(m_ptStartXY.x - currPos.x) < 32 && abs(m_ptStartXY.y - currPos.y) < 16)
//			{
//				SetCurrAnimFacing(false);
//
//				currPos.y -= GetVelY() * fElapsedTime;
//				currPos.x += GetVelX() * fElapsedTime;
//				SetPosPtF(currPos);
//			}
//			// SOUTHWEST
//			else if ( newPoint.y > currPoint.y && abs(m_ptStartXY.x - currPos.x) < 32 && abs(m_ptStartXY.y - currPos.y) < 16)
//			{
//				SetCurrAnimFacing(true);
//
//				currPos.y += GetVelY() * fElapsedTime;
//				currPos.x -= GetVelX() * fElapsedTime;
//				SetPosPtF(currPos);
//			}
//			// check to see if this current tile move is complete
//			if ( abs(m_ptStartXY.x - currPos.x) >= 32 && abs(m_ptStartXY.y - currPos.y) >= 16)
//			{
//				vector<POINT>::iterator first = m_vPath.begin();
//				m_vPath.erase(first);
//				SetCurrTile(newPoint, CBattleMap::GetInstance()->GetOffsetX(), CBattleMap::GetInstance()->GetOffsetY(), CBattleMap::GetInstance()->GetTileWidth(),
//					CBattleMap::GetInstance()->GetTileHeight(), CBattleMap::GetInstance()->GetNumCols());
//// 				SetCurrTile(newPoint, CBattleMap::GetInstance()->GetOffsetX(), CBattleMap::GetInstance()->GetOffsetY(), CBattleMap::GetInstance()->GetTileWidth(),
//// 					CBattleMap::GetInstance()->GetTileHeight(), CBattleMap::GetInstance()->GetNumCols());
//				
// 				DecrementCurrAP(2);
//				m_ptStartXY.x = GetPosX();
//				m_ptStartXY.y = GetPosY();
//			}
//		}
//		else // movement is done
//		{
//			SetCurrAP(GetCurrAP());
//			m_bMoving = false;
//			//SetCurrAnim(3);
//
//			if(m_nInRange == 1)
//			{
//				//skills punch, kick
//				//skills low punch, sweep
//				switch(GetCurrAP())
//				{
//				case 4:
//					{
//						m_pPlayer->GetTurtles()[m_nTurtle]->SetHealth(m_pPlayer->GetTurtles()[m_nTurtle]->GetHealth() - (20));	
//					}
//					break;
//				case 6:
//					{
//						//use skill low punch , sweep
//						//do 40+ damage 
//						int damage = Random(30, 50);//damage
//						m_pPlayer->GetTurtles()[m_nTurtle]->SetHealth(m_pPlayer->GetTurtles()[m_nTurtle]->GetHealth() - damage);
//					}
//					break;
//				case 8:
//					{
//						m_pPlayer->GetTurtles()[m_nTurtle]->SetHealth(m_pPlayer->GetTurtles()[m_nTurtle]->GetHealth() - (20 * 2));
//					}
//					break;
//				case 10:
//					{
//						//use skill punch, kick
//						//do 60+ damage
//						int damage = Random(45, 65);//damage
//						m_pPlayer->GetTurtles()[m_nTurtle]->SetHealth(m_pPlayer->GetTurtles()[m_nTurtle]->GetHealth() - damage);
//					}
//					break;
//				case 12:
//					{
//						m_pPlayer->GetTurtles()[m_nTurtle]->SetHealth(m_pPlayer->GetTurtles()[m_nTurtle]->GetHealth() - (20 * 3));
//					}
//					break;
//				case 16:
//					{
//						//use skill low punch, sweep then use skill punck, kick
//						int damage = Random(80, 100);
//						m_pPlayer->GetTurtles()[m_nTurtle]->SetHealth(m_pPlayer->GetTurtles()[m_nTurtle]->GetHealth() - damage);
//					}
//					break;
//				default:
//					{
//					}
//					break;
//				}
//				//end shredder turn
//				CBattleMap::GetInstance()->SetTurn(true);
//			}
//			//TODO::wait till attack is done to end the turn? would require actually decrementing AP when the attack animation was played
//			CBattleMap::GetInstance()->UpdatePositions();
//			CBattleMap::GetInstance()->NinjaMoveComplete();
//			CBattleMap::GetInstance()->SetTurn(true);
//		}
//	}

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

}


void CBoss::FindPathX(POINT endPt, vector<POINT>Closed)
{
	POINT x;
	x.x = endPt.x;
	x.y = endPt.y;
	
	int debug = 0;

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
						if(m_pTile[coordID].Flag() != FLAG_COLLISION)//check if that coorid is collision tile or not 
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
						if(m_pTile[coordID].Flag() != FLAG_COLLISION)//check if that coorid is collision tile or not 
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
						if(m_pTile[coordID].Flag() != FLAG_COLLISION)//check if that coorid is collision tile or not 
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
						if(m_pTile[coordID].Flag() != FLAG_COLLISION)//check if that coorid is collision tile or not 
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
	POINT p;
	p.x = end.x;
	p.y = end.y;
	
	POINT o;
	o.x = begin.x;
	o.y = begin.y;

	int debug = 0;

	m_vMoveList.clear();
	m_vClosedList.clear();

	bool SetTile[4];//bool to see if the tile has been set

	POINT tile[4];//tile coordinates

	int distance[4];//distance from next tile to endpt

	int minDistance = 100;//minimum distance from next tile to endpt

	int TileNum;//index for the array

	bool inClosed = false;//bool to see if the tile is in the closed list
	
	////////////////////////////////////////////////////////////////////////////////////////////
	//TODO:add a check to find the difference in x and y to the end point then select the higher one
	////////////////////////////////////////////////////////////////////////////////////////////
	
	POINT current; current.x = begin.x; current.y = begin.y;

 	while(GetCurrAP() >=2 && (abs(end.x - current.x)+abs(end.y - current.y)) > 0) //while ap > 2 and the distance > 0
	{
		for(int i = 0; i < 4; i++)
		{
			SetTile[i] = false;
			//distance[i] = 100;
		}
		minDistance = 100;	// should this be set to the distance between the current tile and the destination?
							// or would that stop it from finding a path if there was an obstacle in the immediate way?
		int numCols = CBattleMap::GetInstance()->GetNumCols();
		/////////////////////////////////////////////////////////////////////////////////////////
		// FIRST CHECK (SPACE ONE/FOUR) (MINUS_X)
		if(current.x - 1 > 1)
		{
			tile[MINUS_X].x = current.x-1;
			tile[MINUS_X].y = current.y;
			
			int iterator = tile[MINUS_X].y * numCols + tile[MINUS_X].x;

			if(m_vClosedList.size() > MINUS_X)
			{
				for(unsigned int i = MINUS_X; i < m_vClosedList.size();i++)
				{
					if((m_pTile[iterator].Flag() != FLAG_COLLISION || m_pTile[iterator].Flag() != FLAG_OBJECT_EDGE)
						&& ( tile[MINUS_X].x != m_vClosedList[i].x && tile[MINUS_X].y != m_vClosedList[i].y) )
					{
						SetTile[MINUS_X] = true;
					}
				}
				if(SetTile[MINUS_X] == true)
				{
					distance[MINUS_X] = (abs(end.x - tile[MINUS_X].x) + abs(end.y - tile[MINUS_X].y));
					if(distance[MINUS_X] < minDistance){minDistance = distance[MINUS_X]; TileNum = MINUS_X;}
				}
			}
			else
			{
				if(m_pTile[iterator].Flag() != FLAG_COLLISION || m_pTile[iterator].Flag() != FLAG_OBJECT_EDGE)
				{
					SetTile[MINUS_X] = true;
					distance[MINUS_X] = (abs(end.x - tile[MINUS_X].x) + abs(end.y - tile[MINUS_X].y));
					if(distance[MINUS_X] < minDistance){minDistance = distance[MINUS_X]; TileNum = MINUS_X;}
				}
			}
		}

		////////////////////////////////////////////////////////////////////////////////////////
		//SECOND CHECK (ADD_X)
		if(current.x + 1 < numCols)
		{
			tile[ADD_X].x = current.x+1;
			tile[ADD_X].y = current.y;
			
			int iterator = tile[ADD_X].y * numCols + tile[ADD_X].x;

			if(m_vClosedList.size() > 0)
			{
				for(unsigned int i = 0; i < m_vClosedList.size();i++)
				{
					if((m_pTile[iterator].Flag() != FLAG_COLLISION || m_pTile[iterator].Flag() != FLAG_OBJECT_EDGE)
						&& ( tile[ADD_X].x != m_vClosedList[i].x && tile[ADD_X].y != m_vClosedList[i].y) )
					{
						SetTile[ADD_X] = true;
					}
				}
				if(SetTile[ADD_X] == true)
				{
					distance[ADD_X] = (abs(end.x - tile[ADD_X].x) + abs(end.y - tile[ADD_X].y));
					if(distance[ADD_X] < minDistance){minDistance = distance[ADD_X]; TileNum = ADD_X;}
				}
			}
			else
			{
				if(m_pTile[iterator].Flag() != FLAG_COLLISION || m_pTile[iterator].Flag() != FLAG_OBJECT_EDGE)
				{
					SetTile[ADD_X] = true;
					distance[ADD_X] = (abs(end.x - tile[ADD_X].x) + abs(end.y - tile[ADD_X].y));
					if(distance[ADD_X] < minDistance){minDistance = distance[ADD_X]; TileNum = ADD_X;}
				}
			}
		}
		////////////////////////////////////////////////////////////////////////////////////////
		//THIRD CHECK (MINUS_Y)
		if(current.y - 1 > 1)
		{
			tile[MINUS_Y].x = current.x;
			tile[MINUS_Y].y = current.y -1;
			
			int iterator = tile[MINUS_Y].y * numCols + tile[MINUS_Y].x;

			if(m_vClosedList.size() > 0)
			{
				for(unsigned int i = 0; i < m_vClosedList.size();i++)
				{
					if((m_pTile[iterator].Flag() != FLAG_COLLISION || m_pTile[iterator].Flag() != FLAG_OBJECT_EDGE)
						&& ( tile[MINUS_Y].x != m_vClosedList[i].x && tile[MINUS_Y].y != m_vClosedList[i].y) )
					{
						SetTile[MINUS_Y] = true;
					}
				}
				if(SetTile[MINUS_Y] == true)
				{
					distance[MINUS_Y] = (abs(end.x - tile[MINUS_Y].x) + abs(end.y - tile[MINUS_Y].y));
					if(distance[MINUS_Y] < minDistance){minDistance = distance[MINUS_Y]; TileNum = MINUS_Y;}
				}
			}
			else
			{
				if(m_pTile[iterator].Flag() != FLAG_COLLISION || m_pTile[iterator].Flag() != FLAG_OBJECT_EDGE)
				{
					SetTile[MINUS_Y] = true;
					distance[MINUS_Y] = (abs(end.x - tile[MINUS_Y].x) + abs(end.y - tile[MINUS_Y].y));
					if(distance[MINUS_Y] < minDistance){minDistance = distance[MINUS_Y]; TileNum = MINUS_Y;}
				}
			}
		}
		////////////////////////////////////////////////////////////////////////////////////
		//FOURTH CHECK (ADD_Y)
		if(current.y + 1 < CBattleMap::GetInstance()->GetNumRows())
		{
			tile[ADD_Y].x = current.x;
			tile[ADD_Y].y = current.y+1;
			
			int iterator = tile[ADD_Y].y * numCols + tile[ADD_Y].x;

			if(m_vClosedList.size() > 0)
			{
				for(unsigned int i = 0; i < m_vClosedList.size();i++)
				{
					if((m_pTile[iterator].Flag() != FLAG_COLLISION || m_pTile[iterator].Flag() != FLAG_OBJECT_EDGE)
						&& ( tile[ADD_Y].x != m_vClosedList[i].x && tile[ADD_Y].y != m_vClosedList[i].y) )
					{
						SetTile[ADD_Y] = true;
					}
				}
				if(SetTile[ADD_Y] == true)
				{
					distance[ADD_Y] = (abs(end.x - tile[ADD_Y].x) + abs(end.y - tile[ADD_Y].y));
					if(distance[ADD_Y] < minDistance){minDistance = distance[ADD_Y]; TileNum = ADD_Y;}
				}
			}
			else
			{
				if(m_pTile[iterator].Flag() != FLAG_COLLISION || m_pTile[iterator].Flag() != FLAG_OBJECT_EDGE)
				{
					SetTile[ADD_Y] = true;
					distance[ADD_Y] = (abs(end.x - tile[ADD_Y].x) + abs(end.y - tile[ADD_Y].y));
					if(distance[ADD_Y] < minDistance){minDistance = distance[ADD_Y]; TileNum = ADD_Y;}
				}
			}
		}


		//////////////////////////////////////////////////////////////////////////////////////////////
		//TODO:check which is greater difference the x or y delta and pick that one for the distance
		/////////////////////////////////////////////////////////////////////////////////////////////
		//if(distance[0] == distance[2] && abs(m_ptStartXY.y - end.y) < abs(m_ptStartXY.x - end.x) ){TileNum = 2;}
		//if(distance[1] == distance[3] && abs(m_ptStartXY.y - end.y) < abs(m_ptStartXY.x - end.x) ){TileNum = 3;}

		/////////////////////////////////////////////////////////////////////////////////
		// once the next tile has been chosen, put the current tile on the closed list
		// add the next tile to the move list and set the current tile to the next tile
		switch(TileNum)
		{
		case MINUS_X:
			m_vMoveList.push_back(tile[MINUS_X]);
			m_vClosedList.push_back(current);
			current.x = tile[MINUS_X].x;
			current.y = tile[MINUS_X].y;
			SetCurrAP(GetCurrAP()-2);	// 2 will be replaced with tileCost (if implemented)
			break;
// i don't think the current AP should actually be updated until the path is found
// since a different path may need to be chosen, if, for example, this path ends up
// being a dead end...or simply not the best path
		case ADD_X: 
			m_vMoveList.push_back(tile[ADD_X]);
			m_vClosedList.push_back(current);
			current.x = tile[ADD_X].x;
			current.y = tile[ADD_X].y;
			SetCurrAP(GetCurrAP()-2);
			break;

		case MINUS_Y:
			m_vMoveList.push_back(tile[MINUS_Y]);
			m_vClosedList.push_back(current);
			current.x = tile[MINUS_Y].x;
			current.y = tile[MINUS_Y].y;
			SetCurrAP(GetCurrAP()-2);
			break;
			
		case ADD_Y:
			m_vMoveList.push_back(tile[ADD_Y]);
			m_vClosedList.push_back(current);
			current.x = tile[ADD_Y].x;
			current.y = tile[ADD_Y].y;
			SetCurrAP(GetCurrAP()-2);
			break;
		}
		continue;

	}
	if(m_vMoveList.size() > 0)
	{
		m_bMoving = true;
	}
	else m_bMoving = false;
}
