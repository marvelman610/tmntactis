////////////////////////////////////////////////////////
//   File Name	:	"Ninja.cpp"
//
//   Author		:	Matthew Di Matteo (MD)
//
//   Purpose	:	To provide a game object to be used as an emeny
//					against the user during the game.
////////////////////////////////////////////////////////

#include "Ninja.h"
#include "Assets.h"
#include <cmath>
#include "MessageSystem.h"
#include "ObjectManager.h"
#include "BattleMap.h"

CNinja::CNinja(void)
{
	SetType( OBJECT_NINJA );
	m_nAttack = 0;
	m_nDefend = 0;
	m_nLowHealth = 0;
	m_nInRange = 0;

	SetBaseAP(16);
	SetCurrAP(16);
	
	SetHealthMax(100);
	SetHealth(100);

	SetLevel(1);
	SetStrength(10);
	SetDefense(5);
	SetAccuracy(5);
	SetSpeed(5);
	SetRange(2);

	SetExperience(0);

	CAnimation anim;
	anim.Load("Resources/AnimationInfo/VG_WhiteNinja.dat", 1);
	AddAnim(anim);
	anim.Load("Resources/AnimationInfo/VG_WhiteNinja.dat", 2);
	AddAnim(anim);
	m_vAnimations[m_nCurrAnimation].Play();
}

CNinja::~CNinja(void)
{
	for(int j = 0; j < (int)this->GetAnimations().size(); j++)
			this->GetAnimations()[j].Unload();
}

void CNinja::AI()
{
	//get current x and y tile position then get the x and y tile positions
	//of each turtle then set the m_nInRange to however many tiles apart the closest turtle is

	SetCurrAP(16);
	for(int i = 0; i < 4; i++)
	{

		if( abs(GetMapCoord().x - m_pPlayer->GetTurtles()[MIKEY]->GetMapCoord().x ) + 
			abs(GetMapCoord().y - m_pPlayer->GetTurtles()[MIKEY]->GetMapCoord().y) <
			abs(GetMapCoord().x - m_pPlayer->GetTurtles()[LEONARDO]->GetMapCoord().x) +
			abs(GetMapCoord().y - m_pPlayer->GetTurtles()[LEONARDO]->GetMapCoord().y)  
			)
		{
			if(	abs(GetMapCoord().x - m_pPlayer->GetTurtles()[MIKEY]->GetMapCoord().x ) + 
				abs(GetMapCoord().y - m_pPlayer->GetTurtles()[MIKEY]->GetMapCoord().y) <
				abs(GetMapCoord().x - m_pPlayer->GetTurtles()[RAPHAEL]->GetMapCoord().x) +
				abs(GetMapCoord().y - m_pPlayer->GetTurtles()[RAPHAEL]->GetMapCoord().y)  
				)
			{
				if(	abs(GetMapCoord().x - m_pPlayer->GetTurtles()[MIKEY]->GetMapCoord().x ) + 
					abs(GetMapCoord().y - m_pPlayer->GetTurtles()[MIKEY]->GetMapCoord().y) <
					abs(GetMapCoord().x - m_pPlayer->GetTurtles()[DONATELLO]->GetMapCoord().x) +
					abs(GetMapCoord().y - m_pPlayer->GetTurtles()[DONATELLO]->GetMapCoord().y)   
					)
				{
					//set distance to mikey
					m_nInRange = abs(GetMapCoord().x - m_pPlayer->GetTurtles()[MIKEY]->GetMapCoord().x) + 
						abs(GetMapCoord().y - m_pPlayer->GetTurtles()[MIKEY]->GetMapCoord().y);
					
					m_nXChange = GetMapCoord().x - m_pPlayer->GetTurtles()[MIKEY]->GetMapCoord().x;
					m_nYChange = GetMapCoord().y - m_pPlayer->GetTurtles()[MIKEY]->GetMapCoord().y; 
					m_nTurtle = MIKEY;
				}
				else
				{
					m_nInRange = abs(GetMapCoord().x - m_pPlayer->GetTurtles()[DONATELLO]->GetMapCoord().x) +
						abs(GetMapCoord().y - m_pPlayer->GetTurtles()[DONATELLO]->GetMapCoord().y);

					m_nXChange = GetMapCoord().x - m_pPlayer->GetTurtles()[DONATELLO]->GetMapCoord().x;
					m_nYChange = GetMapCoord().y - m_pPlayer->GetTurtles()[DONATELLO]->GetMapCoord().y; 
					m_nTurtle = DONATELLO;
				}
			}
			else
			{
				if(	abs(GetMapCoord().x - m_pPlayer->GetTurtles()[RAPHAEL]->GetMapCoord().x ) + 
					abs(GetMapCoord().y - m_pPlayer->GetTurtles()[RAPHAEL]->GetMapCoord().y) <
					abs(GetMapCoord().x - m_pPlayer->GetTurtles()[DONATELLO]->GetMapCoord().x) +
					abs(GetMapCoord().y - m_pPlayer->GetTurtles()[DONATELLO]->GetMapCoord().y)   
				    )
				{
					m_nInRange = abs(GetMapCoord().x - m_pPlayer->GetTurtles()[RAPHAEL]->GetMapCoord().x) +
						abs(GetMapCoord().y - m_pPlayer->GetTurtles()[RAPHAEL]->GetMapCoord().y);

					m_nXChange = GetMapCoord().x - m_pPlayer->GetTurtles()[RAPHAEL]->GetMapCoord().x;
					m_nYChange = GetMapCoord().y - m_pPlayer->GetTurtles()[RAPHAEL]->GetMapCoord().y; 
					m_nTurtle = RAPHAEL;
				}
				else
				{
					m_nInRange = abs(GetMapCoord().x - m_pPlayer->GetTurtles()[DONATELLO]->GetMapCoord().x) +
						abs(GetMapCoord().y - m_pPlayer->GetTurtles()[DONATELLO]->GetMapCoord().y);

					m_nXChange = GetMapCoord().x - m_pPlayer->GetTurtles()[DONATELLO]->GetMapCoord().x;
					m_nYChange = GetMapCoord().y - m_pPlayer->GetTurtles()[DONATELLO]->GetMapCoord().y; 
					m_nTurtle = DONATELLO;
				}
			}
		}
		else
		{
			if(	abs(GetMapCoord().x - m_pPlayer->GetTurtles()[LEONARDO]->GetMapCoord().x ) + 
				abs(GetMapCoord().y - m_pPlayer->GetTurtles()[LEONARDO]->GetMapCoord().y) <
				abs(GetMapCoord().x - m_pPlayer->GetTurtles()[DONATELLO]->GetMapCoord().x) +
				abs(GetMapCoord().y - m_pPlayer->GetTurtles()[DONATELLO]->GetMapCoord().y)  
			  )
			{
				if(	abs(GetMapCoord().x - m_pPlayer->GetTurtles()[LEONARDO]->GetMapCoord().x ) + 
					abs(GetMapCoord().y - m_pPlayer->GetTurtles()[LEONARDO]->GetMapCoord().y) <
					abs(GetMapCoord().x - m_pPlayer->GetTurtles()[RAPHAEL]->GetMapCoord().x) +
					abs(GetMapCoord().y - m_pPlayer->GetTurtles()[RAPHAEL]->GetMapCoord().y)  
				  )
				{
					m_nInRange = abs(GetMapCoord().x - m_pPlayer->GetTurtles()[LEONARDO]->GetMapCoord().x) +
						abs(GetMapCoord().y - m_pPlayer->GetTurtles()[LEONARDO]->GetMapCoord().y );

					m_nXChange = GetMapCoord().x - m_pPlayer->GetTurtles()[LEONARDO]->GetMapCoord().x;
					m_nYChange = GetMapCoord().y - m_pPlayer->GetTurtles()[LEONARDO]->GetMapCoord().y;
					m_nTurtle = LEONARDO;
				}
				else
				{
					m_nInRange = abs(GetMapCoord().x - m_pPlayer->GetTurtles()[RAPHAEL]->GetMapCoord().x) +
						abs(GetMapCoord().y - m_pPlayer->GetTurtles()[RAPHAEL]->GetMapCoord().y);

					m_nXChange = GetMapCoord().x - m_pPlayer->GetTurtles()[RAPHAEL]->GetMapCoord().x;
					m_nYChange = GetMapCoord().y - m_pPlayer->GetTurtles()[RAPHAEL]->GetMapCoord().y; 
					m_nTurtle = RAPHAEL;
				}
			}
			else
			{
				if( abs(GetMapCoord().x - m_pPlayer->GetTurtles()[DONATELLO]->GetMapCoord().x ) + 
					abs(GetMapCoord().y - m_pPlayer->GetTurtles()[DONATELLO]->GetMapCoord().y) <
					abs(GetMapCoord().x - m_pPlayer->GetTurtles()[RAPHAEL]->GetMapCoord().x) +
					abs(GetMapCoord().y - m_pPlayer->GetTurtles()[RAPHAEL]->GetMapCoord().y)  
				  )
				{
					m_nInRange = abs(GetMapCoord().x - m_pPlayer->GetTurtles()[DONATELLO]->GetMapCoord().x) +
						abs(GetMapCoord().y - m_pPlayer->GetTurtles()[DONATELLO]->GetMapCoord().y);

					m_nXChange = GetMapCoord().x - m_pPlayer->GetTurtles()[DONATELLO]->GetMapCoord().x;
					m_nYChange = GetMapCoord().y - m_pPlayer->GetTurtles()[DONATELLO]->GetMapCoord().y; 
					m_nTurtle = DONATELLO;
				}
				else
				{
					m_nInRange = abs(GetMapCoord().x - m_pPlayer->GetTurtles()[RAPHAEL]->GetMapCoord().x) +
						abs(GetMapCoord().y - m_pPlayer->GetTurtles()[RAPHAEL]->GetMapCoord().y);

					m_nXChange = GetMapCoord().x - m_pPlayer->GetTurtles()[RAPHAEL]->GetMapCoord().x;
					m_nYChange = GetMapCoord().y - m_pPlayer->GetTurtles()[RAPHAEL]->GetMapCoord().y;
					m_nTurtle = RAPHAEL;
				}
			}
		}
	}

	//map point to set
	POINT mapPt;
	switch(m_nInRange)
	{
		//next to turtle
		
	case 0:
		{
			//16 ap always
			// attack four times (4ap * 4 = 16)
			m_pPlayer->GetTurtles()[m_nTurtle]->SetHealth(m_pPlayer->GetTurtles()[m_nTurtle]->GetHealth() - (20 * 4)); 
			//end turn
			SetCurrAP(0);
		}
		break;
		//one tile away from  turtle
	case 1:
		{
			//attack four times(4ap * 4 = 16)
			m_pPlayer->GetTurtles()[m_nTurtle]->SetHealth(m_pPlayer->GetTurtles()[m_nTurtle]->GetHealth() - (20 * 4));
			//end turn
			SetCurrAP(0);
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
			
			
			SetCurrTile(mapPt, CBattleMap::GetInstance()->GetOffsetX(), CBattleMap::GetInstance()->GetOffsetY(), 
				CBattleMap::GetInstance()->GetTileWidth(), CBattleMap::GetInstance()->GetTileHeight(), 
				CBattleMap::GetInstance()->GetNumCols(), true);

			//attack 3 times (4ap * 3 = 12)
			m_pPlayer->GetTurtles()[m_nTurtle]->SetHealth(m_pPlayer->GetTurtles()[m_nTurtle]->GetHealth() - (20 * 3));
			//end turn
			SetCurrAP(0);
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
			SetCurrTile(mapPt, CBattleMap::GetInstance()->GetOffsetX(), CBattleMap::GetInstance()->GetOffsetY(), 
				CBattleMap::GetInstance()->GetTileWidth(), CBattleMap::GetInstance()->GetTileHeight(), 
				CBattleMap::GetInstance()->GetNumCols(), true);

			//attack three times(12ap)= 16ap
			m_pPlayer->GetTurtles()[m_nTurtle]->SetHealth(m_pPlayer->GetTurtles()[m_nTurtle]->GetHealth() - ( 20 * 3));
			//end turn
			SetCurrAP(0);
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
			SetCurrTile(mapPt, CBattleMap::GetInstance()->GetOffsetX(), CBattleMap::GetInstance()->GetOffsetY(), 
				CBattleMap::GetInstance()->GetTileWidth(), CBattleMap::GetInstance()->GetTileHeight(), 
				CBattleMap::GetInstance()->GetNumCols(), true);

			//attack twice(8ap) = 14ap
			m_pPlayer->GetTurtles()[m_nTurtle]->SetHealth(m_pPlayer->GetTurtles()[m_nTurtle]->GetHealth() - (20 * 2));
			//end turn
			SetCurrAP(0);
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
			SetCurrTile(mapPt, CBattleMap::GetInstance()->GetOffsetX(), CBattleMap::GetInstance()->GetOffsetY(), 
				CBattleMap::GetInstance()->GetTileWidth(), CBattleMap::GetInstance()->GetTileHeight(), 
				CBattleMap::GetInstance()->GetNumCols(), true);

			//attack twice(8ap) = 16ap
			m_pPlayer->GetTurtles()[m_nTurtle]->SetHealth(m_pPlayer->GetTurtles()[m_nTurtle]->GetHealth() - (20 * 2));
			//end turn
			SetCurrAP(0);
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
			SetCurrTile(mapPt, CBattleMap::GetInstance()->GetOffsetX(), CBattleMap::GetInstance()->GetOffsetY(), 
				CBattleMap::GetInstance()->GetTileWidth(), CBattleMap::GetInstance()->GetTileHeight(), 
				CBattleMap::GetInstance()->GetNumCols(), true);

			//attack one(4ap) = 14ap
			m_pPlayer->GetTurtles()[m_nTurtle]->SetHealth(m_pPlayer->GetTurtles()[m_nTurtle]->GetHealth() - (20));
			//end turn
			SetCurrAP(0);
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
			SetCurrTile(mapPt, CBattleMap::GetInstance()->GetOffsetX(), CBattleMap::GetInstance()->GetOffsetY(), 
				CBattleMap::GetInstance()->GetTileWidth(), CBattleMap::GetInstance()->GetTileHeight(), 
				CBattleMap::GetInstance()->GetNumCols(), true);

			//attack once(4ap) = 16ap
			m_pPlayer->GetTurtles()[m_nTurtle]->SetHealth(m_pPlayer->GetTurtles()[m_nTurtle]->GetHealth() - (20));
			//end turn
			SetCurrAP(0);
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
			SetCurrTile(mapPt, CBattleMap::GetInstance()->GetOffsetX(), CBattleMap::GetInstance()->GetOffsetY(), 
				CBattleMap::GetInstance()->GetTileWidth(), CBattleMap::GetInstance()->GetTileHeight(), 
				CBattleMap::GetInstance()->GetNumCols(), true);

			//end turn
			SetCurrAP(0);
		}
		break;
	default:
		{
			//end turn
			SetCurrAP(0);
		}
		break;
	}
}
void CNinja::Update(float fElapsedTime)
{
	if(GetHealth() <= 0)
	{
		/*int type = rand()% 1;

		if(type == 0)	MessageSystem::GetInstance()->SendMsg(new CCreateItem(this));
		else			MessageSystem::GetInstance()->SendMsg(new CCreateWeapon(this));*/

		
		MessageSystem::GetInstance()->SendMsg(new CCreateItem(this));
		return;

	}

	//if( ( (float)GetHealth()/(float)GetMaxHealth()) < ( ( (float)GetMaxHealth() * 0.3f ) / (float)GetMaxHealth() ) )
	//{
	//	m_nLowHealth = 1;
	//}
	//else m_nLowHealth = 0;

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
	m_vAnimations[m_nCurrAnimation].Update(fElapsedTime);

	
}
void CNinja::Render()
{
	m_vAnimations[m_nCurrAnimation].Render((int)GetPosX(), (int)GetPosY(), GetPosZ(), 1.2f, false, m_dwColor);
}