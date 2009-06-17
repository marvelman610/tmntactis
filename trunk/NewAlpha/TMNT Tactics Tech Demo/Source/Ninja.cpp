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

	SetVelX(100.0f);
	SetVelY(50.0f);

	SetLevel(1);
	SetStrength(10);
	SetDefense(5);
	SetAccuracy(5);
	SetSpeed(5);
	SetRange(2);

	SetExperience(0);

	m_pPlayer = CPlayer::GetInstance();
	m_pTile = CBattleMap::GetInstance()->GetTiles();

	CAnimation anim;
	anim.Load("Resources/AnimationInfo/VG_WhiteNinja1.dat", 1);
	AddAnim(anim);
	anim.Load("Resources/AnimationInfo/VG_WhiteNinja1.dat", 2);
	AddAnim(anim);
	anim.Load("Resources/AnimationInfo/VG_WhiteNinja1.dat", 3);
	AddAnim(anim);
	anim.Load("Resources/AnimationInfo/VG_WhiteNinja1.dat", 4);
	AddAnim(anim);
	anim.Load("Resources/AnimationInfo/VG_WhiteNinja1.dat", 5);
	AddAnim(anim);
	anim.Load("Resources/AnimationInfo/VG_WhiteNinja1.dat", 6);
	AddAnim(anim);
	anim.Load("Resources/AnimationInfo/VG_WhiteNinja1.dat", 7);
	AddAnim(anim);
	anim.Load("Resources/AnimationInfo/VG_WhiteNinja1.dat", 8);
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
	
	int tempRange1 = abs(GetMapCoord().x - m_pPlayer->GetTurtles()[MIKEY]->GetMapCoord().x) + 
						abs(GetMapCoord().y - m_pPlayer->GetTurtles()[MIKEY]->GetMapCoord().y);
	int tempRange2 = abs(GetMapCoord().x - m_pPlayer->GetTurtles()[LEONARDO]->GetMapCoord().x) +
						abs(GetMapCoord().y - m_pPlayer->GetTurtles()[LEONARDO]->GetMapCoord().y );
	int tempRange3 = abs(GetMapCoord().x - m_pPlayer->GetTurtles()[DONATELLO]->GetMapCoord().x) +
						abs(GetMapCoord().y - m_pPlayer->GetTurtles()[DONATELLO]->GetMapCoord().y);
	int tempRange4 = abs(GetMapCoord().x - m_pPlayer->GetTurtles()[RAPHAEL]->GetMapCoord().x) +
						abs(GetMapCoord().y - m_pPlayer->GetTurtles()[RAPHAEL]->GetMapCoord().y);


	
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
	if(m_nTurtle <= -1)
	{
		m_nTurtle = 0;
	}
	POINT end;
	
	//for(int i = 0; i < 4; i++)
	//{

	//	if( abs(GetMapCoord().x - m_pPlayer->GetTurtles()[MIKEY]->GetMapCoord().x ) + 
	//		abs(GetMapCoord().y - m_pPlayer->GetTurtles()[MIKEY]->GetMapCoord().y) <
	//		abs(GetMapCoord().x - m_pPlayer->GetTurtles()[LEONARDO]->GetMapCoord().x) +
	//		abs(GetMapCoord().y - m_pPlayer->GetTurtles()[LEONARDO]->GetMapCoord().y)  
	//		)
	//	{
	//		if(	abs(GetMapCoord().x - m_pPlayer->GetTurtles()[MIKEY]->GetMapCoord().x ) + 
	//			abs(GetMapCoord().y - m_pPlayer->GetTurtles()[MIKEY]->GetMapCoord().y) <
	//			abs(GetMapCoord().x - m_pPlayer->GetTurtles()[RAPHAEL]->GetMapCoord().x) +
	//			abs(GetMapCoord().y - m_pPlayer->GetTurtles()[RAPHAEL]->GetMapCoord().y)  
	//			)
	//		{
	//			if(	abs(GetMapCoord().x - m_pPlayer->GetTurtles()[MIKEY]->GetMapCoord().x ) + 
	//				abs(GetMapCoord().y - m_pPlayer->GetTurtles()[MIKEY]->GetMapCoord().y) <
	//				abs(GetMapCoord().x - m_pPlayer->GetTurtles()[DONATELLO]->GetMapCoord().x) +
	//				abs(GetMapCoord().y - m_pPlayer->GetTurtles()[DONATELLO]->GetMapCoord().y)   
	//				)
	//			{
	//				//set distance to mikey
	//				m_nInRange = abs(GetMapCoord().x - m_pPlayer->GetTurtles()[MIKEY]->GetMapCoord().x) + 
	//					abs(GetMapCoord().y - m_pPlayer->GetTurtles()[MIKEY]->GetMapCoord().y);
	//				
	//				m_nXChange =  m_pPlayer->GetTurtles()[MIKEY]->GetMapCoord().x - GetMapCoord().x ;
	//				m_nYChange =  m_pPlayer->GetTurtles()[MIKEY]->GetMapCoord().y - GetMapCoord().y ; 
	//				m_nTurtle = MIKEY;
	//			}
	//			else
	//			{
	//				m_nInRange = abs(GetMapCoord().x - m_pPlayer->GetTurtles()[DONATELLO]->GetMapCoord().x) +
	//					abs(GetMapCoord().y - m_pPlayer->GetTurtles()[DONATELLO]->GetMapCoord().y);

	//				m_nXChange =  m_pPlayer->GetTurtles()[DONATELLO]->GetMapCoord().x - GetMapCoord().x;
	//				m_nYChange =  m_pPlayer->GetTurtles()[DONATELLO]->GetMapCoord().y - GetMapCoord().y; 
	//				m_nTurtle = DONATELLO;
	//			}
	//		}
	//		else
	//		{
	//			if(	abs(GetMapCoord().x - m_pPlayer->GetTurtles()[RAPHAEL]->GetMapCoord().x ) + 
	//				abs(GetMapCoord().y - m_pPlayer->GetTurtles()[RAPHAEL]->GetMapCoord().y) <
	//				abs(GetMapCoord().x - m_pPlayer->GetTurtles()[DONATELLO]->GetMapCoord().x) +
	//				abs(GetMapCoord().y - m_pPlayer->GetTurtles()[DONATELLO]->GetMapCoord().y)   
	//			    )
	//			{
	//				m_nInRange = abs(GetMapCoord().x - m_pPlayer->GetTurtles()[RAPHAEL]->GetMapCoord().x) +
	//					abs(GetMapCoord().y - m_pPlayer->GetTurtles()[RAPHAEL]->GetMapCoord().y);

	//				m_nXChange =  m_pPlayer->GetTurtles()[RAPHAEL]->GetMapCoord().x - GetMapCoord().x;
	//				m_nYChange =  m_pPlayer->GetTurtles()[RAPHAEL]->GetMapCoord().y - GetMapCoord().y; 
	//				m_nTurtle = RAPHAEL;
	//			}
	//			else
	//			{
	//				m_nInRange = abs(GetMapCoord().x - m_pPlayer->GetTurtles()[DONATELLO]->GetMapCoord().x) +
	//					abs(GetMapCoord().y - m_pPlayer->GetTurtles()[DONATELLO]->GetMapCoord().y);

	//				m_nXChange = m_pPlayer->GetTurtles()[DONATELLO]->GetMapCoord().x - GetMapCoord().x;
	//				m_nYChange = m_pPlayer->GetTurtles()[DONATELLO]->GetMapCoord().y - GetMapCoord().y; 
	//				m_nTurtle = DONATELLO;
	//			}
	//		}
	//	}
	//	else
	//	{
	//		if(	abs(GetMapCoord().x - m_pPlayer->GetTurtles()[LEONARDO]->GetMapCoord().x ) + 
	//			abs(GetMapCoord().y - m_pPlayer->GetTurtles()[LEONARDO]->GetMapCoord().y) <
	//			abs(GetMapCoord().x - m_pPlayer->GetTurtles()[DONATELLO]->GetMapCoord().x) +
	//			abs(GetMapCoord().y - m_pPlayer->GetTurtles()[DONATELLO]->GetMapCoord().y)  
	//		  )
	//		{
	//			if(	abs(GetMapCoord().x - m_pPlayer->GetTurtles()[LEONARDO]->GetMapCoord().x ) + 
	//				abs(GetMapCoord().y - m_pPlayer->GetTurtles()[LEONARDO]->GetMapCoord().y) <
	//				abs(GetMapCoord().x - m_pPlayer->GetTurtles()[RAPHAEL]->GetMapCoord().x) +
	//				abs(GetMapCoord().y - m_pPlayer->GetTurtles()[RAPHAEL]->GetMapCoord().y)  
	//			  )
	//			{
	//				m_nInRange = abs(GetMapCoord().x - m_pPlayer->GetTurtles()[LEONARDO]->GetMapCoord().x) +
	//					abs(GetMapCoord().y - m_pPlayer->GetTurtles()[LEONARDO]->GetMapCoord().y );

	//				m_nXChange =  m_pPlayer->GetTurtles()[LEONARDO]->GetMapCoord().x - GetMapCoord().x;
	//				m_nYChange =  m_pPlayer->GetTurtles()[LEONARDO]->GetMapCoord().y - GetMapCoord().y;
	//				m_nTurtle = LEONARDO;
	//			}
	//			else
	//			{
	//				m_nInRange = abs(GetMapCoord().x - m_pPlayer->GetTurtles()[RAPHAEL]->GetMapCoord().x) +
	//					abs(GetMapCoord().y - m_pPlayer->GetTurtles()[RAPHAEL]->GetMapCoord().y);

	//				m_nXChange = m_pPlayer->GetTurtles()[RAPHAEL]->GetMapCoord().x - GetMapCoord().x ;
	//				m_nYChange =  m_pPlayer->GetTurtles()[RAPHAEL]->GetMapCoord().y - GetMapCoord().y; 
	//				m_nTurtle = RAPHAEL;
	//			}
	//		}
	//		else
	//		{
	//			if( abs(GetMapCoord().x - m_pPlayer->GetTurtles()[DONATELLO]->GetMapCoord().x ) + 
	//				abs(GetMapCoord().y - m_pPlayer->GetTurtles()[DONATELLO]->GetMapCoord().y) <
	//				abs(GetMapCoord().x - m_pPlayer->GetTurtles()[RAPHAEL]->GetMapCoord().x) +
	//				abs(GetMapCoord().y - m_pPlayer->GetTurtles()[RAPHAEL]->GetMapCoord().y)  
	//			  )
	//			{
	//				m_nInRange = abs(GetMapCoord().x - m_pPlayer->GetTurtles()[DONATELLO]->GetMapCoord().x) +
	//					abs(GetMapCoord().y - m_pPlayer->GetTurtles()[DONATELLO]->GetMapCoord().y);

	//				m_nXChange =  m_pPlayer->GetTurtles()[DONATELLO]->GetMapCoord().x - GetMapCoord().x ;
	//				m_nYChange =  m_pPlayer->GetTurtles()[DONATELLO]->GetMapCoord().y -	GetMapCoord().y ;
	//				m_nTurtle = DONATELLO;
	//			}
	//			else
	//			{
	//				m_nInRange = abs(GetMapCoord().x - m_pPlayer->GetTurtles()[RAPHAEL]->GetMapCoord().x) +
	//					abs(GetMapCoord().y - m_pPlayer->GetTurtles()[RAPHAEL]->GetMapCoord().y);

	//				m_nXChange =  m_pPlayer->GetTurtles()[RAPHAEL]->GetMapCoord().x - GetMapCoord().x ;
	//				m_nYChange =  m_pPlayer->GetTurtles()[RAPHAEL]->GetMapCoord().y - GetMapCoord().y ;
	//				m_nTurtle = RAPHAEL;
	//			}
	//		}
	//	}
	//}

	//map point to set
	POINT mapPt;
	switch(m_nInRange)
	{
		//next to turtle
	case 0:
		{
			SetCurrAnim(4);

			//16 ap always
			// attack four times (4ap * 4 = 16)
			m_pPlayer->GetTurtles()[m_nTurtle]->SetHealth(m_pPlayer->GetTurtles()[m_nTurtle]->GetHealth() - (20 * 4)); 
			//end turn
			SetCurrAP(0);
			//TODO::wait till attack is done to end the turn? would require actually decrementing AP when the attack animation was played
			CBattleMap::GetInstance()->UpdatePositions();
			CBattleMap::GetInstance()->NinjaMoveComplete();
			
			//CBattleMap::GetInstance()->SetTurn(true);

		}
		break;
		//one tile away from  turtle
	case 1:
		{
 			SetCurrAnim(4);

			//attack four times(4ap * 4 = 16)
			m_pPlayer->GetTurtles()[m_nTurtle]->SetHealth(m_pPlayer->GetTurtles()[m_nTurtle]->GetHealth() - (20 * 4));
			//end turn
			SetCurrAP(0);
			//TODO::wait till attack is done to end the turn? would require actually decrementing AP when the attack animation was played
			CBattleMap::GetInstance()->UpdatePositions();
			CBattleMap::GetInstance()->NinjaMoveComplete();
			
			//SetCurrAnim(0);

			//CBattleMap::GetInstance()->SetTurn(true);

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
			FindPath(begin, end);
			/*SetCurrTile(mapPt, CBattleMap::GetInstance()->GetOffsetX(), CBattleMap::GetInstance()->GetOffsetY(), 
				CBattleMap::GetInstance()->GetTileWidth(), CBattleMap::GetInstance()->GetTileHeight(), 
				nNumCols, true);*/

			//attack 3 times (4ap * 3 = 12)
			//m_pPlayer->GetTurtles()[m_nTurtle]->SetHealth(m_pPlayer->GetTurtles()[m_nTurtle]->GetHealth() - (20 * 3));
			//end turn
			
			CBattleMap::GetInstance()->SetTurn(true);
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
			/*SetCurrTile(mapPt, CBattleMap::GetInstance()->GetOffsetX(), CBattleMap::GetInstance()->GetOffsetY(), 
				CBattleMap::GetInstance()->GetTileWidth(), CBattleMap::GetInstance()->GetTileHeight(), 
				nNumCols, true);*/

			m_bMoving = true;
			FindPath(begin, end);

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
			/*SetCurrTile(mapPt, CBattleMap::GetInstance()->GetOffsetX(), CBattleMap::GetInstance()->GetOffsetY(), 
				CBattleMap::GetInstance()->GetTileWidth(), CBattleMap::GetInstance()->GetTileHeight(), 
				nNumCols, true);*/
			
			m_bMoving = true;
			FindPath(begin, end);

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
			/*SetCurrTile(mapPt, CBattleMap::GetInstance()->GetOffsetX(), CBattleMap::GetInstance()->GetOffsetY(), 
				CBattleMap::GetInstance()->GetTileWidth(), CBattleMap::GetInstance()->GetTileHeight(), 
				nNumCols, true);*/

			m_bMoving = true;
			FindPath(begin, end);

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
			/*SetCurrTile(mapPt, CBattleMap::GetInstance()->GetOffsetX(), CBattleMap::GetInstance()->GetOffsetY(), 
				CBattleMap::GetInstance()->GetTileWidth(), CBattleMap::GetInstance()->GetTileHeight(), 
				nNumCols, true);*/
			
			m_bMoving = true;
			FindPath(begin, end);

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
			/*SetCurrTile(mapPt, CBattleMap::GetInstance()->GetOffsetX(), CBattleMap::GetInstance()->GetOffsetY(), 
				CBattleMap::GetInstance()->GetTileWidth(), CBattleMap::GetInstance()->GetTileHeight(), 
				nNumCols, true);*/

			m_bMoving = true;
			FindPath(begin, end);

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

			/*SetCurrTile(mapPt, CBattleMap::GetInstance()->GetOffsetX(), CBattleMap::GetInstance()->GetOffsetY(), 
				CBattleMap::GetInstance()->GetTileWidth(), CBattleMap::GetInstance()->GetTileHeight(), 
				nNumCols, true);*/

			m_bMoving = true;
			
			FindPath(begin, end);

		}
		break;
	default:
		{
			//end turn

			/*if(rand()%2 > 1)
			{
				mapPt.x  = GetMapCoord().x + 2;
				mapPt.y = GetMapCoord().y + 2;
			}
			else 
			{
				mapPt.x = GetMapCoord().x - 2;
				mapPt.y = GetMapCoord().y - 2;
			}*/

			switch(abs(m_nXChange))
			{
			case 0: 
				mapPt.x = GetMapCoord().x;
				if(m_nYChange > 0)
				{
					mapPt.y = GetMapCoord().y + 8;
				}
				else
				{
					mapPt.y = GetMapCoord().y - 8;
				}
				break;
			case 1: 
				mapPt.x = GetMapCoord().x + m_nXChange;
				if(m_nYChange > 0)
				{
					mapPt.y = GetMapCoord().y + 7;
				}
				else
				{
					mapPt.y = GetMapCoord().y - 7;
				}
				break;
			case 2: 
				mapPt.x = GetMapCoord().x + m_nXChange;
				if(m_nYChange > 0)
				{
					mapPt.y = GetMapCoord().y + 6;
				}
				else
				{
					mapPt.y = GetMapCoord().y - 6;
				}
				break;
			case 3:
				mapPt.x = GetMapCoord().x + m_nXChange;
				if(m_nYChange > 0)
				{
					mapPt.y = GetMapCoord().y + 5;
				}
				else
				{
					mapPt.y = GetMapCoord().y - 5;
				}
				break;
			case 4:
				mapPt.x = GetMapCoord().x + m_nXChange;
				if(m_nYChange > 0)
				{
					mapPt.y = GetMapCoord().y + 4;
				}
				else
				{
					mapPt.y = GetMapCoord().y - 4;
				}
				break;
			case 5:
				mapPt.x = GetMapCoord().x + m_nXChange;
				if(m_nYChange > 0)
				{
					mapPt.y = GetMapCoord().y + 3;
				}
				else
				{
					mapPt.y = GetMapCoord().y - 3;
				}
				break;
			case 6:
				mapPt.x = GetMapCoord().x + m_nXChange;
				if(m_nYChange > 0)
				{
					mapPt.y = GetMapCoord().y + 2;
				}
				else
				{
					mapPt.y = GetMapCoord().y - 2;
				}
				break;
			case 7:
				mapPt.x = GetMapCoord().x + m_nXChange;
				if(m_nYChange > 0)
				{
					mapPt.y = GetMapCoord().y + 1;
				}
				else
				{
					mapPt.y = GetMapCoord().y - 1;
				}
				break;
			case 8:
				mapPt.y = GetMapCoord().y;
				if(m_nXChange > 0)
				{
					mapPt.x = GetMapCoord().x + 8;
				}
				else
				{
					mapPt.x = GetMapCoord().x - 8;
				}
				break;
			}
			if(mapPt.x > -1 && mapPt.y > -1)
			{
				end = mapPt;
				m_bMoving = true;
				FindPath(begin, end);
			}
			else
			{
				CBattleMap::GetInstance()->SetTurn(true);
			}

			
			//TODO::wait till attack is done to end the turn? would require actually decrementing AP when the attack animation was played
			/*CBattleMap::GetInstance()->UpdatePositions();
			CBattleMap::GetInstance()->NinjaMoveComplete();
			CBattleMap::GetInstance()->SetTurn(true);*/
		}
		break;
	}
	
}
///////////////////////////////////////////////////////////////////////////////
// Function: FindPath
//
// Purpose: To find a path to get to the turtles
///////////////////////////////////////////////////////////////////////////////
void CNinja::FindPath(POINT begin, POINT end)
{
	SetCurrAnim(0);

	m_vPath.clear();
	POINT ptCurr = begin; // begin point
	POINT ptTarget = end;	// end point
	int range = GetCurrAP();	// max distance moveable
	int pathWeight = 0;
	vector<int> pathX;
	vector<int> pathY;
	int oldX = ptCurr.x;
	int oldY = ptCurr.y;
	int nNumCols = CBattleMap::GetInstance()->GetNumCols();

	while (true)
	{
		if (pathX.size() > 0 && ptCurr.x == pathX[pathX.size()-1] && ptCurr.y == pathY[pathX.size()-1])
		{ pathWeight = 10000; break; }
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
		if (pathWeight > nNumCols)
		{pathWeight = 10000; break;}

		if (pathX.size() > 1)
		{
			if (ptCurr.x < ptTarget.x && (oldX != ptCurr.x + 1) && ptCurr.x+1 != pathX[pathX.size()-2] &&
				m_pTile[ptCurr.y * nNumCols + (ptCurr.x+1)].Flag() == FLAG_NONE &&
				!CBattleMap::GetInstance()->CheckTileOccupied(ptCurr.y * nNumCols + (ptCurr.x+1)))
			{ ++ptCurr.x; continue; }
			if (ptCurr.x > ptTarget.x && (oldX != ptCurr.x - 1) && ptCurr.x-1 != pathX[pathX.size()-2] &&
				m_pTile[ptCurr.y * nNumCols + (ptCurr.x-1)].Flag() == FLAG_NONE &&
				!CBattleMap::GetInstance()->CheckTileOccupied(ptCurr.y * nNumCols + (ptCurr.x-1)))
			{ --ptCurr.x; continue; }
			if (ptCurr.y < ptTarget.y && (oldY != ptCurr.y + 1) && ptCurr.y+1 != pathY[pathX.size()-2] &&
				m_pTile[(ptCurr.y+1) * nNumCols + ptCurr.x].Flag() == FLAG_NONE &&
				!CBattleMap::GetInstance()->CheckTileOccupied((ptCurr.y+1) * nNumCols + ptCurr.x))
			{ ++ptCurr.y; continue; }
			if (ptCurr.y > ptTarget.y && (oldY != ptCurr.y - 1) && ptCurr.y-1 != pathY[pathX.size()-2] &&
				m_pTile[(ptCurr.y-1) * nNumCols + ptCurr.x].Flag() == FLAG_NONE &&
				!CBattleMap::GetInstance()->CheckTileOccupied((ptCurr.y-1) * nNumCols + ptCurr.x))
			{ --ptCurr.y; continue; }
		}
		else
		{
			if (ptCurr.x < ptTarget.x && (oldX != ptCurr.x + 1) && 
				m_pTile[ptCurr.y * nNumCols + (ptCurr.x+1)].Flag() == FLAG_NONE &&
				!CBattleMap::GetInstance()->CheckTileOccupied(ptCurr.y * nNumCols + (ptCurr.x+1)))
			{ ++ptCurr.x; continue; }
			if (ptCurr.x > ptTarget.x && (oldX != ptCurr.x - 1) && 
				m_pTile[ptCurr.y * nNumCols + (ptCurr.x-1)].Flag() == FLAG_NONE &&
				!CBattleMap::GetInstance()->CheckTileOccupied(ptCurr.y * nNumCols + (ptCurr.x-1)))
			{ --ptCurr.x; continue; }
			if (ptCurr.y < ptTarget.y && (oldY != ptCurr.y + 1) && 
				m_pTile[(ptCurr.y+1) * nNumCols + ptCurr.x].Flag() == FLAG_NONE &&
				!CBattleMap::GetInstance()->CheckTileOccupied((ptCurr.y+1) * nNumCols + ptCurr.x))
			{ ++ptCurr.y; continue; }
			if (ptCurr.y > ptTarget.y && (oldY != ptCurr.y - 1) && 
				m_pTile[(ptCurr.y-1) * nNumCols + ptCurr.x].Flag() == FLAG_NONE &&
				!CBattleMap::GetInstance()->CheckTileOccupied((ptCurr.y-1) * nNumCols + ptCurr.x))
			{ --ptCurr.y; continue; }
		}

		if (ptCurr.x == ptTarget.x)
		{
			if (oldX != ptCurr.x+1 && m_pTile[ptCurr.y * nNumCols + ptCurr.x+1].Flag() == FLAG_NONE &&
				!CBattleMap::GetInstance()->CheckTileOccupied(ptCurr.y * nNumCols + ptCurr.x+1) )
			{ ++ptCurr.x; continue; }
			if (oldX != ptCurr.x-1 && m_pTile[ptCurr.y * nNumCols + ptCurr.x-1].Flag() == FLAG_NONE &&
				!CBattleMap::GetInstance()->CheckTileOccupied(ptCurr.y * nNumCols + ptCurr.x-1))
			{ --ptCurr.x; continue; }
		}
		else if (ptCurr.y == ptTarget.y)
		{
			if (oldY != ptCurr.y+1 && m_pTile[(ptCurr.y+1) * nNumCols + ptCurr.x].Flag() == FLAG_NONE &&
				!CBattleMap::GetInstance()->CheckTileOccupied((ptCurr.y+1) * nNumCols + ptCurr.x))
			{ ++ptCurr.y; continue; }
			if (oldY != ptCurr.y-1 && m_pTile[(ptCurr.y-1) * nNumCols + ptCurr.x].Flag() == FLAG_NONE &&
				!CBattleMap::GetInstance()->CheckTileOccupied((ptCurr.y-1) * nNumCols + ptCurr.x))
			{ --ptCurr.y; continue; }
		}

		if ( (ptCurr.x > ptTarget.x) && (oldX != ptCurr.x+1) && m_pTile[ptCurr.y * nNumCols + (ptCurr.x+1)].Flag() == FLAG_NONE &&
			!CBattleMap::GetInstance()->CheckTileOccupied(ptCurr.y * nNumCols + (ptCurr.x+1)))
		{++ptCurr.x; continue;}
		else if (oldX != ptCurr.x-1 && m_pTile[ptCurr.y * nNumCols + (ptCurr.x-1)].Flag() == FLAG_NONE &&
			!CBattleMap::GetInstance()->CheckTileOccupied(ptCurr.y * nNumCols + (ptCurr.x-1)))
		{--ptCurr.x; continue;}

		if ( (ptCurr.y > ptTarget.y) && (oldY != ptCurr.y+1) && m_pTile[(ptCurr.y+1) * nNumCols + ptCurr.x].Flag() == FLAG_NONE &&
			!CBattleMap::GetInstance()->CheckTileOccupied((ptCurr.y+1) * nNumCols + ptCurr.x))
		{++ptCurr.y; continue;}
		else if (oldY != ptCurr.y-1 && m_pTile[(ptCurr.y-1) * nNumCols + ptCurr.x].Flag() == FLAG_NONE &&
			!CBattleMap::GetInstance()->CheckTileOccupied((ptCurr.y-1) * nNumCols + ptCurr.x))
		{--ptCurr.y; continue;}
		pathWeight = 10000; break;
	}
	if (m_vPath.size() > 0)
	{
		m_ptStartXY.x = GetPosX();
		m_ptStartXY.y = GetPosY();
		for (unsigned int i = 0; i < m_vPath.size(); ++i)
			m_pTile[m_vPath[i].y * nNumCols + m_vPath[i].x].SetAlpha(199);
	}
}
///////////////////////////////////////////////////////////////////////////////
// Function: Update
//
// Purpose: To update the ninja information
///////////////////////////////////////////////////////////////////////////////
void CNinja::Update(float fElapsedTime)
{
	m_vAnimations[m_nCurrAnimation].Update(fElapsedTime);

	// a ninja has been moved...execute the animation and position change over time
	if (m_bMoving)
	{
		if(GetCurrAnim()->IsAnimationPlaying() && GetCurrAnim()->GetCurrAnimFrame() != 1 &&  GetCurrAnim()->GetCurrAnimFrame() != 2)
			GetCurrAnim()->Stop();

		if (m_vPath.size() > 0)
		{
			// grab the next move and take it out of the vector..if the previous move is complete
			POINT newPoint = m_vPath[0];
			// set up variables
			POINT currPoint= GetMapCoord();
			MY_POINT_FLOAT currPos; 
			currPos.x = GetPosX(); currPos.y = GetPosY();

			// NORTHWEST
			if ( newPoint.x < currPoint.x && abs(m_ptStartXY.x - currPos.x) < 32 && abs(m_ptStartXY.y - currPos.y) < 16)
			{
				if(!GetCurrAnim()->IsAnimationPlaying())
					SetCurrAnim(2);
				SetCurrAnimFacing(false);

				currPos.x -= GetVelX() * fElapsedTime;
				currPos.y -= GetVelY() * fElapsedTime;
				SetPosPtF(currPos);
			}
			// SOUTHEAST
			else if ( newPoint.x > currPoint.x && abs(m_ptStartXY.x - currPos.x) < 32 && abs(m_ptStartXY.y - currPos.y) < 16)
			{
				if(!GetCurrAnim()->IsAnimationPlaying())
					SetCurrAnim(1);
				SetCurrAnimFacing(true);
				currPos.x += GetVelX() * fElapsedTime;
				currPos.y += GetVelY() * fElapsedTime;
				SetPosPtF(currPos);
			}
			// NORTHEAST
			if ( newPoint.y < currPoint.y && abs(m_ptStartXY.x - currPos.x) < 32 && abs(m_ptStartXY.y - currPos.y) < 16)
			{
				if(!GetCurrAnim()->IsAnimationPlaying())
					SetCurrAnim(2);
				SetCurrAnimFacing(true);

				currPos.y -= GetVelY() * fElapsedTime;
				currPos.x += GetVelX() * fElapsedTime;
				SetPosPtF(currPos);
			}
			// SOUTHWEST
			else if ( newPoint.y > currPoint.y && abs(m_ptStartXY.x - currPos.x) < 32 && abs(m_ptStartXY.y - currPos.y) < 16)
			{
				if(!GetCurrAnim()->IsAnimationPlaying())
					SetCurrAnim(1);
				SetCurrAnimFacing(false);

				currPos.y += GetVelY() * fElapsedTime;
				currPos.x -= GetVelX() * fElapsedTime;
				SetPosPtF(currPos);
			}
			// check to see if this current tile move is complete
			if ( abs(m_ptStartXY.x - currPos.x) >= 32 && abs(m_ptStartXY.y - currPos.y) >= 16)
			{
				vector<POINT>::iterator first = m_vPath.begin();
				m_vPath.erase(first);
				SetCurrTile(newPoint, CBattleMap::GetInstance()->GetOffsetX(), CBattleMap::GetInstance()->GetOffsetY(), CBattleMap::GetInstance()->GetTileWidth(),
					CBattleMap::GetInstance()->GetTileHeight(), CBattleMap::GetInstance()->GetNumCols());
// 				SetCurrTile(newPoint, CBattleMap::GetInstance()->GetOffsetX(), CBattleMap::GetInstance()->GetOffsetY(), CBattleMap::GetInstance()->GetTileWidth(),
// 					CBattleMap::GetInstance()->GetTileHeight(), nNumCols);
				
 				DecrementCurrAP(2);
				m_ptStartXY.x = GetPosX();
				m_ptStartXY.y = GetPosY();
			}
		}
		else // movement is done
		{
			if(GetCurrAnim()->GetCurrAnimFrame() == GetCurrAnim()->GetTotalFrames())
			{
				SetCurrAnim(0);
				CBattleMap::GetInstance()->SetTurn(true);
			}
				
			if(GetCurrAnim()->IsAnimationPlaying() && GetCurrAnim()->GetCurrAnimFrame() != 4)
				SetCurrAnim(0);
			

			SetCurrAP(GetCurrAP());
			m_bMoving = false;

			if(m_nInRange == 1)
			{
				SetCurrAnim(4);

				switch(GetCurrAP())
				{
				case 4:
					{

						m_pPlayer->GetTurtles()[m_nTurtle]->SetHealth(m_pPlayer->GetTurtles()[m_nTurtle]->GetHealth() - (20));	

					}
					break;
				case 8:
					{
						m_pPlayer->GetTurtles()[m_nTurtle]->SetHealth(m_pPlayer->GetTurtles()[m_nTurtle]->GetHealth() - (20 * 2));
					}
					break;
				case 12:
					{
						m_pPlayer->GetTurtles()[m_nTurtle]->SetHealth(m_pPlayer->GetTurtles()[m_nTurtle]->GetHealth() - (20 * 3));
					}
					break;
				default:
					{
					}
					break;
				}
				

			}
			//TODO::wait till attack is done to end the turn? would require actually decrementing AP when the attack animation was played
			CBattleMap::GetInstance()->UpdatePositions();
			CBattleMap::GetInstance()->NinjaMoveComplete();
			//SetCurrAnim(0);
			CBattleMap::GetInstance()->SetTurn(true);
		}
	}


	if(GetHealth() <= 0)
	{
		int type = rand()% 4;

		if(type < 3)	MessageSystem::GetInstance()->SendMsg(new CCreateItem(this));
		else			MessageSystem::GetInstance()->SendMsg(new CCreateWeapon(this));

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
	
}
void CNinja::Render()
{
	m_vAnimations[m_nCurrAnimation].Render((int)GetPosX(), (int)GetPosY(), GetPosZ(), 1, m_dwColor);
}