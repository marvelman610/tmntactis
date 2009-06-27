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

int Random(int min, int max, bool bRandNeg = false)
{
	int number = 0;
	number = abs(max) - abs(min);
	number = ((rand()%number) + abs(min));
	if(bRandNeg && rand() % 2 == 0)
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
}

CBoss::~CBoss(void)
{	
	m_pPlayer = NULL;
	m_pTilesL1 = NULL;
	m_pBitmapFont = NULL;
	m_pBattleMap = NULL;
}

void CBoss::AI()
{
	//////////////////////////////////////////////////////////////////////////
	// reset everything for a new turn
	for (int i =0; i < 4; ++i)
	{m_pComb[i].damage = 0; m_pComb[i].attackID = -1;}
	m_nCurrAttackInd = m_nCurrAnimInd = -1;
	m_nTurtle = 0;
	m_bMoving = m_bAttacking = m_bAttackDone = m_bDoDmg = m_bRenderCombText = false;
	m_nInRange = 20; // if no turtles are found in range, this signals a random move
	SetCurrAP(16);
	//////////////////////////////////////////////////////////////////////////
	
	//set temporary ranges
	//check distance
	CTurtle** turtles = m_pPlayer->GetTurtles();
	CBattleMap* pBM = CBattleMap::GetInstance();
	int distance[4];
	for (int i = 0; i < 4; ++i)
	{
		//set range to 100 if turtle is dead
		if (!turtles[i]->GetAlive())
			distance[i] = 100;
		else
			distance[i] = abs(turtles[i]->GetMapCoord().x - GetMapCoord().x) + 
				abs( turtles[i]->GetMapCoord().y - GetMapCoord().y);
	}
	// determine who to move towards (who is closest), start with [0]
	int lowest = distance[0]; m_nInRange = lowest; 
	m_nXChange = turtles[0]->GetMapCoord().x - GetMapCoord().x;
	m_nYChange = turtles[0]->GetMapCoord().y - GetMapCoord().y;
	for (int t = 1; t < 4; ++t)	// iterate through the turtles
	{
		if (distance[t] < lowest)
		{
			lowest = distance[t];
			m_nInRange = lowest;
			m_nXChange = turtles[t]->GetMapCoord().x - GetMapCoord().x;
			m_nYChange = turtles[t]->GetMapCoord().y - GetMapCoord().y; 
			m_nTurtle = t;
		}
	}

	POINT begin = GetMapCoord();
	if (m_nInRange > 8)
	{m_nInRange = 20; m_nTurtle = -1;}

	DetermineMove(begin, pBM);
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
		PerformMove(fElapsedTime);
	}
	if (m_bAttacking)
	{
		// execute the attacks, keeps track of which attacks are to happen
		// only grab the next one if the currAnim is done playing
		if (!m_vAnimations[m_nCurrAnimInd].IsAnimationPlaying())
		{
			++m_nCurrAttackInd;
			if (m_pComb[m_nCurrAttackInd].attackID > -1)
			{
				m_nCurrAnimInd	= m_pComb[m_nCurrAttackInd].attackID;
				m_nDamage		= m_pComb[m_nCurrAttackInd].damage;
				DecrementCurrAP(m_pComb[m_nCurrAttackInd].apCost);
				SetCurrAnim(m_nCurrAnimInd);
				m_bRenderCombText = true; m_bDoDmg = true;	
				m_Timer.StartTimer(1.0f);	// render combat text for a bit
			}
			else // we hit the end
			{
				m_bAttacking = m_bMoving = false;
				m_bAttackDone = true; 
				m_Timer.StartTimer(0.5f); /*slight delay before player's turn*/
				m_nCurrAnimInd = 1; // back to idle
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

		//boss xp gain
		int nXP = 10;
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
		m_bAttackDone = m_bDoDmg = m_bRenderCombText = false;
	}
	if (m_bRenderCombText)
	{
		m_bRenderCombText = false;
	}
	if (m_bDoDmg)
	{
		m_pPlayer->GetTurtles()[m_nTurtle]->DecrementHealth(m_nDamage);
	}
}

bool CBoss::IsOnCloseList(CTile* tile, vector<CTile>& closed)
{
	for (unsigned int i = 0; i < closed.size(); ++i)
		if (closed[i].DestXID() == tile->DestXID() && closed[i].DestYID() == tile->DestYID())
			return true;
	return false;
}

void CBoss::DetermineAttack()
{
	// this will get the AP when boss first starts attacking and determine which attacks to execute
	switch(GetCurrAP())
	{
	case 4:
		{
			// one normal high attack
			m_nCurrAnimInd = ATK_HIGH;
			m_pComb[0].attackID = ATK_HIGH;
			m_pComb[0].damage = Random(35, 45);	
			m_pComb[0].apCost = 4;
		}
		break;
	case 6:
		{
			// one normal low attack
			m_nCurrAnimInd = ATK_LOW;
			m_pComb[0].attackID = ATK_LOW;
			m_pComb[0].damage = Random(50, 60);	
			m_pComb[0].apCost = 4;
		}
		break;
	case 8:
		{
			// a low and a high
			m_nCurrAnimInd = ATK_LOW;
			m_pComb[0].attackID = ATK_LOW;
			m_pComb[0].damage = Random(50, 60);	
			m_pComb[0].apCost = 4;
			m_pComb[1].attackID = ATK_HIGH;
			m_pComb[1].damage = Random(50, 60);	
			m_pComb[1].apCost = 4;
		}
		break;
	case 10:
		{
			// a low and a high
			m_nCurrAnimInd = ATK_LOW;
			m_pComb[0].attackID = ATK_LOW;
			m_pComb[0].damage = Random(50, 60);	
			m_pComb[0].apCost = 4;
			m_pComb[1].attackID = ATK_HIGH;
			m_pComb[1].damage = Random(50, 60);	
			m_pComb[1].apCost = 6;
		}
		break;
	case 12:
		{
			// a ground pound
			m_nCurrAnimInd = ATK_SPECIAL;
			m_pComb[0].attackID = ATK_SPECIAL;
			m_pComb[0].damage = Random(70, 85);	
			m_pComb[0].apCost = 12;
		}
		break;
	case 14:
		{
			// a low and a ground pound
			m_nCurrAnimInd = ATK_LOW;
			m_pComb[0].attackID = ATK_LOW;
			m_pComb[0].damage = Random(30, 40);	
			m_pComb[0].apCost = 2;
			m_pComb[1].attackID = ATK_SPECIAL;
			m_pComb[1].damage = Random(70, 80);	
			m_pComb[1].apCost = 12;
		}
		break;
	case 16:
		{
			// a low and a ground pound
			m_nCurrAnimInd = ATK_LOW;
			m_pComb[0].attackID = ATK_LOW;
			m_pComb[0].damage = Random(30, 40);	
			m_pComb[0].apCost = 4;
			m_pComb[1].attackID = ATK_SPECIAL;
			m_pComb[1].damage = Random(70, 80);	
			m_pComb[1].apCost = 12;
		}
		break;
	default:
		{
			int debug = 0;
		}
		break;
	}
}

void CBoss::DetermineMove( POINT &begin, CBattleMap* pBM )
{
	POINT end;
	POINT mapPt;
	//switch case for distance 0 - 8
	switch(m_nInRange)
	{
		//on turtle, should never occur
	case 0:
		{
			int debug = 0; 
		}
		break;
		//one tile away from  turtle
	case 1:
		{
			m_bAttacking = true;
			DetermineAttack();
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
				int change = Random(1, 6, true);
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

void CBoss::PerformMove( float fElapsedTime )
{
	m_ptCurrPos.x = GetPosX();
	m_ptCurrPos.y = GetPosY();

	//get start position and next tile
	if(GetMapCoord().x > m_vMoveList[m_nMoveListIndex].x) //move left one x tile(on screen)
	{
		//32 pixels left 16 pixels up
		if( abs(m_ptStartXY.x - m_ptCurrPos.x) < 32.0f && abs(m_ptStartXY.y - m_ptCurrPos.y) < 16.0f)
		{
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
			m_ptStartXY.x = GetPosX();
			m_ptStartXY.y = GetPosY();
		}
		CBattleMap::GetInstance()->UpdatePositions();
	}

	// movement is done
	if(m_nMoveListIndex == -1 && m_nTurtle > -1)	// attack the targeted turtle (should be in range if the m_nTurtle is > -1)
	{
		m_vMoveList.clear();
		m_bMoving = false;
		CTurtle* turtle = m_pPlayer->GetTurtles()[m_nTurtle];
		int distance = abs(turtle->GetMapCoord().x - GetMapCoord().x) + 
			abs( turtle->GetMapCoord().y - GetMapCoord().y);
		// if in range and enough AP..attack
		if (distance == 1 && GetCurrAP() > 3)
		{
			m_bAttacking = true;
			DetermineAttack();
		}
		else // no AP left, turn is done
		{
			m_nCurrAnimInd = 1;
			m_bAttacking = m_bMoving = false;
			m_bAttackDone = true;
			m_Timer.StartTimer(0.5f); /*slight delay before player's turn*/
		}
	}
	else if (m_nMoveListIndex == -1)	// out of range or not enough AP, turn is done
	{
		//end shredder turn
		m_bAttacking = m_bMoving = false;
		m_bAttackDone = true;
		m_Timer.StartTimer(0.5f); /*slight delay before player's turn*/
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
		unsigned int oInd = 1;	// if the open vector has more than 1, start at open[1], otherwise, we keep open[0] as currTile
		vector<CTile>::iterator iter = open.begin();	// start at the beginning (currTile[0]), iter to erase = index of currTile
		int iterCount = 0;
		if (open.size() > 1)
		{
			for ( ; ; )
			{
				if (open[oInd].F() <= currTile.F())
				{ currTile = open[oInd]; iterCount = oInd;}
				++oInd;
				if (oInd == open.size())
					break;
			}
		}
		while (iterCount > 0)
		{
			++iter;
			--iterCount;
		}
		adjTiles = bMap->GetAdjTiles(currTile.DestXID(), currTile.DestYID(), tiles);
		open.erase(iter); closed.push_back(currTile);

		//////////////////////////////////////////////////////////////////////////

		// check the adjacent tiles (adjacent to the current), determine which one's best - based on distance? 
		for (int ind = 0; ind < 4; ++ind)
		{
			if (adjTiles[ind] == NULL)
				continue;
			int tileID = adjTiles[ind]->DestYID() * numCols + adjTiles[ind]->DestXID();

			if (tiles[tileID].Flag() != FLAG_COLLISION && tiles[tileID].Flag() != FLAG_OBJECT_EDGE &&
				!IsOnCloseList(adjTiles[ind], closed))
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