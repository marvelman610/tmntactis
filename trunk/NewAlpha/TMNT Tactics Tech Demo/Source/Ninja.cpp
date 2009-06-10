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


CNinja::CNinja(void)
{
	m_nType = OBJECT_NINJA;
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
	for(int j = 0; j < this->GetAnimations().size(); j++)
			this->GetAnimations()[j].Unload();
}

void CNinja::AI()
{
	//get current x and y tile position then get the x and y tile positions
	//of each turtle then set the m_nInRange to however many tiles apart the closest turtle is
	for(int i = 0; i < 4; i++)
	{

		if( abs(GetMapCoord().x - m_pPlayer->GetTurtles()[MIKEY]->GetMapCoord().x ) + 
			abs(GetMapCoord().y - m_pPlayer->GetTurtles()[MIKEY]->GetMapCoord().y) >
			abs(GetMapCoord().x - m_pPlayer->GetTurtles()[LEONARDO]->GetMapCoord().x) +
			abs(GetMapCoord().y - m_pPlayer->GetTurtles()[LEONARDO]->GetMapCoord().y)  
			)
		{
			if(	abs(GetMapCoord().x - m_pPlayer->GetTurtles()[MIKEY]->GetMapCoord().x ) + 
				abs(GetMapCoord().y - m_pPlayer->GetTurtles()[MIKEY]->GetMapCoord().y) >
				abs(GetMapCoord().x - m_pPlayer->GetTurtles()[RAPHAEL]->GetMapCoord().x) +
				abs(GetMapCoord().y - m_pPlayer->GetTurtles()[RAPHAEL]->GetMapCoord().y)  
				)
			{
				if(	abs(GetMapCoord().x - m_pPlayer->GetTurtles()[MIKEY]->GetMapCoord().x ) + 
					abs(GetMapCoord().y - m_pPlayer->GetTurtles()[MIKEY]->GetMapCoord().y) >
					abs(GetMapCoord().x - m_pPlayer->GetTurtles()[DONATELLO]->GetMapCoord().x) +
					abs(GetMapCoord().y - m_pPlayer->GetTurtles()[DONATELLO]->GetMapCoord().y)   
					)
				{
					//set distance to mikey
					m_nInRange = abs(GetMapCoord().x - m_pPlayer->GetTurtles()[MIKEY]->GetMapCoord().x) + 
						abs(GetMapCoord().y - m_pPlayer->GetTurtles()[MIKEY]->GetMapCoord().y);
				}
			}
		}
	}


	switch(m_nInRange)
	{
		//next to turtle
	case 0:
		switch(GetCurrAP())
		{
			case 0: //end turn
				break;
			case 1: 
				{
					if(( (float)GetHealth() / (float)GetMaxHealth() )*.1f > .9f)
					{
						//throw rock 1 ap 
					}
					else {}
					//defend (ends turn)
				}
				break;
			case 2:
				{
					if( ( (float)GetHealth() / (float)GetMaxHealth() )* .1f > .9f )
					{
						//check for ninja star(2ap)
						//else throw rock and then defend
					}
					else 
					{//defend
					}
				}
				break;
			case 3:
				{
					if( (float)GetHealth() / (float)GetMaxHealth()  *.1f > .9f )
					{
						//throw star (2ap) throw rock(1ap)
					}
					else
					{
						//throw star and defend
					}
				}
				break;
			case 4:
				{
					if( (float)GetHealth() / (float)GetMaxHealth() * .1f > .9f )
					{
						//attack
					}
					else
					{
						//throw star, rock then defend
					}
				}
				break;
			case 5:
				{
					if( (float)GetHealth() / (float)GetMaxHealth() * .1f > .9f )
					{
						//attack then defend
					}
					else
					{
						//throw 2 stars then defend
					}
				}
				break;
			case 6:
				{
					if( (float)GetHealth() / (float)GetMaxHealth()* .1f > .9f  )
					{
						//attack and throw star
					}
					else
					{
						//throw 3 stars if not then throw 5 rocks and defend
					}
				}
				break;
			case 7:
				{
					if( (float)GetHealth() / (float)GetMaxHealth()* .1f > .9f  )
					{
						//attack, throw star and defend
					}
					else
					{
						//throw 3 stars then defend else throw 6 rocks and defend
					}
				}
				break;
			case 8:
				{
					if( (float)GetHealth() / (float)GetMaxHealth()* .1f > .9f  )
					{
						//attack twice
					}
					else
					{
						//throw 3 stars and defend or move back attack once and move back a space and defend
					}
				}
				break;
			case 9:
				{
					if( (float)GetHealth() / (float)GetMaxHealth()* .1f > .9f  )
					{
						//attack twice and defend
					}
					else
					{
						//attack and move back two spaces then defend
					}
				}
				break;
			case 10:
				{
					if( (float)GetHealth() / (float)GetMaxHealth() * .1f > .9f )
					{
						// attack twice throw rock and defend
					}
					else
					{
						//attack once move back two spaces and defend
					}
				}
				break;
			case 11:
				{
					if( (float)GetHealth() / (float)GetMaxHealth() * .1f > .9f )
					{
						//attack twice throw star and defend
					}
					else
					{
						//attack once move back three spaces throw rock or defend
					}
				}
				break;
			case 12:
				{
					if( (float)GetHealth() / (float)GetMaxHealth()* .1f > .9f  )
					{
						//attack three times
					}
					else
					{
						//attack once move back four spaces
					}
				}
				break;
			case 13:
				{
					if( (float)GetHealth() / (float)GetMaxHealth() * .1f > .9f )
					{
						//attack three times and defend
					}
					else
					{
						//attack once move back four spaces and defend
					}
				}
				break;
			case 14:
				{
					if( (float)GetHealth() / (float)GetMaxHealth()* .1f > .9f  )
					{
						//attack three time, throw rock and defend
					}
					else
					{
						//attack once move back four spaces throw rock and defend
					}
				}
				break;
			case 15:
				{
					if( (float)GetHealth() / (float)GetMaxHealth()* .1f > .9f  )
					{
						//attack three times, throw star and defend
					}
					else
					{
						//attack twice move back two space and defend
					}
				}
				break;
			case 16:
				{
					if( (float)GetHealth() / (float)GetMaxHealth() * .1f > .9f  )
					{
						//attack four times
					}
					else
					{
						//attack twice move back four spaces
					}
				}
				break;
		}
		break;
		//one space away
		case 1:
			{
				switch(GetCurrAP())
				{
					case 0:
						{
							//end turn
						}
						break;
					case 1:
						{
							if( (float)GetHealth() / (float)GetMaxHealth() * .1f > .9f )
							{
								//throw rock
							}
							else
							{
								//defend
							}
						}
						break;
					case 2:
						{
							if( (float)GetHealth() / (float)GetMaxHealth() * .1f > .9f )
							{
								//throw star or two rocks
							}
							else
							{
								//throw rock and defend
							}
						}
						break;
					case 3:
						{
							if( (float)GetHealth() / (float)GetMaxHealth() * .1f > .9f )
							{
								//throw star and rock or three rocks
							}
							else
							{
								//throw two rocks and defend
							}
						}
						break;
					case 4:
						{
							if( (float)GetHealth() / (float)GetMaxHealth() * .1f > .9f )
							{
								//attack
							}
							else
							{
								//throw star, rock then defend
							}
						}
						break;
					case 5:
						{
							if( (float)GetHealth() / (float)GetMaxHealth() * .1f > .9f )
							{
								//attack and defend
							}
							else
							{
								//attack and defend
							}
						}
						break;
					case 6:
						{
							if( (float)GetHealth() / (float)GetMaxHealth() * .1f > .9f )
							{
								//attack throw star( or throw rock then defend)
							}
							else
							{
								//attack throw rock then defend
							}
						}
						break;
					case 7:
						{
							if( (float)GetHealth() / (float)GetMaxHealth() * .1f > .9f )
							{
								//attack throw star, rock then defend
							}
							else
							{
								//attack throw star, rock then defend
							}
						}
						break;
					case 8:
						{
							if( (float)GetHealth() / (float)GetMaxHealth() * .1f > .9f )
							{
								//attack twice
							}
							else
							{
								//attack once throw star, rock then defend
							}
						}
						break;
					case 9:
						{
							if( (float)GetHealth() / (float)GetMaxHealth() * .1f > .9f )
							{
								//attack twice then throw rock 
							}
							else
							{
								//attack twice and defend
							}
						}
						break;
					case 10:
						{
							if( (float)GetHealth() / (float)GetMaxHealth() * .1f > .9f )
							{
								//attack twice and throw star
							}
							else
							{
								//attack twice throw rock then defend
							}
						}
						break;
					case 11:
						{
							if( (float)GetHealth() / (float)GetMaxHealth() * .1f > .9f )
							{
								//attack twice, throw star, throw rock
							}
							else
							{
								//attack twice throw star and defend
							}
						}
						break;
					case 12:
						{
							if( (float)GetHealth() / (float)GetMaxHealth() * .1f > .9f )
							{
								//attack three times
							}
							else
							{
								//attack twice throw star throw rock and defend
							}
						}
						break;
					case 13:
						{
							if( (float)GetHealth() / (float)GetMaxHealth() * .1f > .9f )
							{
								//attack three times and throw rock
							}
							else
							{
								//attack three time and defend
							}
						}
						break;
					case 14:
						{
							if( (float)GetHealth() / (float)GetMaxHealth() * .1f > .9f )
							{
								//attack 3 times throw star
							}
							else
							{
								//attack 3 times throw rock and defend
							}
						}
						break;
					case 15:
						{
						if( (float)GetHealth() / (float)GetMaxHealth() * .1f > .9f )
							{
								//attack 3 times throw star and throw rock
							}
							else
							{
								//attack 3 times throw star and defend
							}
						}
						break;
					case 16:
						{
							if( (float)GetHealth() / (float)GetMaxHealth() * .1f > .9f )
							{
								//attack 4 times
							}
							else
							{
								//attack 4 times
							}
						}
						break;
					}
			}
			break;
			//two space away (one out of range)
		case 2:
			{
				switch(GetCurrAP())
				{
					case 0:
						{
							//end turn
						}
						break;
					case 1:
						{
							if( (float)GetHealth() / (float)GetMaxHealth() * .1f > .9f )
							{
								//throw rock
							}
							else
							{
								//defend
							}
						}
						break;
					case 2:
						{
							if( (float)GetHealth() / (float)GetMaxHealth() * .1f > .9f )
							{
								//throw star or two rocks
							}
							else
							{
								//throw rock and defend
							}
						}
						break;
					case 3:
						{
							if( (float)GetHealth() / (float)GetMaxHealth() * .1f > .9f )
							{
								//throw star and a rock
							}
							else
							{
								//throw star and defend
							}
						}
						break;
					case 4:
						{
							if( (float)GetHealth() / (float)GetMaxHealth() * .1f > .9f )
							{
								//throw star x2 
							}
							else
							{
								//throw star, rock and defend
							}
						}
						break;
					case 5:
						{
							if( (float)GetHealth() / (float)GetMaxHealth() * .1f > .9f )
							{
								//throw star x2 and rock
							}
							else
							{
								//throw star x2 and defend
							}
						}
						break;
					case 6:
						{
							if( (float)GetHealth() / (float)GetMaxHealth() * .1f > .9f )
							{
								//move one space forward then attack
							}
							else
							{
								//move one space forward then attack
							}
						}
						break;
					case 7:
						{
							if( (float)GetHealth() / (float)GetMaxHealth() * .1f > .9f )
							{
								//move one space forward then attack and throw rock
							}
							else
							{
								//move one space forward then attack and defend
							}
						}
						break;
					case 8:
						{
							if( (float)GetHealth() / (float)GetMaxHealth() * .1f > .9f )
							{
								//move one space forward attack, throw star
							}
							else
							{
								//move one space forward attack, throw rock and defend
							}
						}
						break;
					case 9:
						{
							if( (float)GetHealth() / (float)GetMaxHealth() * .1f > .9f )
							{
								//move one space forward attack, throw star, throw rock
							}
							else
							{
								//move one space forward attack, throw star, defend
							}
						}
						break;
					case 10:
						{
							if( (float)GetHealth() / (float)GetMaxHealth() * .1f > .9f )
							{
								//move one space forward attack x2
							}
							else
							{
								//move one space forward attack x2
							}
						}
						break;
					case 11:
						{
							if( (float)GetHealth() / (float)GetMaxHealth() * .1f > .9f )
							{
								//move one space forward attack x2, throw rock
							}
							else
							{
								//move one space forward attack x2, defend
							}
						}
						break;
					case 12:
						{
							if( (float)GetHealth() / (float)GetMaxHealth() * .1f > .9f )
							{
								//move one space forward attack x2, throw star
							}
							else
							{
								//move one space forward attack x2, throw rock, defend
							}
						}
						break;
					case 13:
						{
							if( (float)GetHealth() / (float)GetMaxHealth() * .1f > .9f )
							{
								//move one space forward attack x2, throw star, throw rock
							}
							else
							{
								//move one space forward attack x2, throw star, defend
							}
						}
						break;
					case 14:
						{
							if( (float)GetHealth() / (float)GetMaxHealth() * .1f > .9f )
							{
								//move forward one space attack x3
							}
							else
							{
								//move one space forward attack x3
							}
						}
						break;
					case 15:
						{
							if( (float)GetHealth() / (float)GetMaxHealth() * .1f > .9f )
							{
								//move one space forward attack x3, throw rock
							}
							else
							{
								//move one space forward attack x3, defend
							}
						}
						break;
					case 16:
						{
							if( (float)GetHealth() / (float)GetMaxHealth() * .1f > .9f )
							{
								//move one space forward attack x3 throw star
							}
							else
							{
								//move one space forward attack x3, throw rock, def
							}
						}
						break;
					}
			}
			break;
			
		case 3:
			{
				switch(GetCurrAP())
				{
					case 0:
						{
							//end turn
						}
						break;
					case 1:
						{
							if( (float)GetHealth() / (float)GetMaxHealth() * .1f > .9f )
							{
								//throw rock
							}
							else
							{
								//defend
							}
						}
						break;
					case 2:
						{
							if( (float)GetHealth() / (float)GetMaxHealth() * .1f > .9f )
							{
								//throw star or two rocks
							}
							else
							{
								//throw rock and defend
							}
						}
						break;
					case 3:
						{
							if( (float)GetHealth() / (float)GetMaxHealth() * .1f > .9f )
							{
								//throw star and rock
							}
							else
							{
								//throw star and defend
							}
						}
						break;
					case 4:
						{
							//attack
						}
						break;
					case 5:
						{
							if( (float)GetHealth() / (float)GetMaxHealth() * .1f > .9f )
							{
								//attack throw rock
							}
							else
							{
								//attack and defend
							}
						}
						break;
					case 6:
						{
							if( (float)GetHealth() / (float)GetMaxHealth() * .1f > .9f )
							{
								//attack throw star
							}
							else
							{
								//attack throw rock and defend
							}
						}
						break;
					case 7:
						{
							if( (float)GetHealth() / (float)GetMaxHealth() * .1f > .9f )
							{
								//attack throw star throw rock
							}
							else
							{
								//attack throw star and defend
							}
						}
						break;
					case 8:
						{
							if( (float)GetHealth() / (float)GetMaxHealth() * .1f > .9f )
							{
								//attack 2
							}
							else
							{
								//attack 2
							}
						}
						break;
					case 9:
						{
							if( (float)GetHealth() / (float)GetMaxHealth() * .1f > .9f )
							{
								//attack 2 throw rock
							}
							else
							{
								//attack 2 defend
							}
						}
						break;
					case 10:
						{
							if( (float)GetHealth() / (float)GetMaxHealth() * .1f > .9f )
							{
								//attack 2 throw star
							}
							else
							{
								//attack 2 throw rock, defend
							}
						}
						break;
					case 11:
						{
							if( (float)GetHealth() / (float)GetMaxHealth() * .1f > .9f )
							{
								//attack 2 throw star throw rock
							}
							else
							{
								//attack 2 throw star defend
							}
						}
						break;
					case 12:
						{
							if( (float)GetHealth() / (float)GetMaxHealth() * .1f > .9f )
							{
								//attack 3
							}
							else
							{
								//attack 3
							}
						}
						break;
					case 13:
						{
							if( (float)GetHealth() / (float)GetMaxHealth() * .1f > .9f )
							{
								//attack 3 throw rock
							}
							else
							{
								//attack 3 defend
							}
						}
						break;
					case 14:
						{
							if( (float)GetHealth() / (float)GetMaxHealth() * .1f > .9f )
							{
								//attack 3 throw star
							}
							else
							{
								//attack 3 throw rock defend
							}
						}
						break;
					case 15:
						{
							if( (float)GetHealth() / (float)GetMaxHealth() * .1f > .9f )
							{
								//attack 3 throw star throw rock
							}
							else
							{
								//attack 3 throw star defend
							}
						}
						break;
					case 16:
						{
							if( (float)GetHealth() / (float)GetMaxHealth() * .1f > .9f )
							{
								//attack 4
							}
							else
							{
								//attack 4
							}
						}
						break;
					}
			}
			break;
		case 4:
			{
				switch(GetCurrAP())
				{
					case 0:
						{
							//end turn
						}
						break;
					case 1:
						{
							if( (float)GetHealth() / (float)GetMaxHealth() * .1f > .9f )
							{
								//throw rock
							}
							else
							{
								//throw rock
							}
						}
						break;
					case 2:
						{
							if( (float)GetHealth() / (float)GetMaxHealth() * .1f > .9f )
							{
								//throw star or rock x2
							}
							else
							{
								//throw rock defend
							}
						}
						break;
					case 3:
						{
							if( (float)GetHealth() / (float)GetMaxHealth() * .1f > .9f )
							{
								//throw star and rock
							}
							else
							{
								//throw star, defend
							}
						}
						break;
					case 4:
						{
							if( (float)GetHealth() / (float)GetMaxHealth() * .1f > .9f )
							{
								//throw star x2
							}
							else
							{
								//throw star, rock, defend
							}
						}
						break;
					case 5:
						{
							if( (float)GetHealth() / (float)GetMaxHealth() * .1f > .9f )
							{
								//throw star x2, throw rock
							}
							else
							{
								//throw star x2, defend
							}
						}
						break;
					case 6:
						{
							if( (float)GetHealth() / (float)GetMaxHealth() * .1f > .9f )
							{
								//throw star x3
							}
							else
							{
								//throw star x2, defend
							}
						}
						break;
					case 7:
						{
							if( (float)GetHealth() / (float)GetMaxHealth() * 0.1f > 0.9f )
							{
								//throw star x3, throw rock 
							}
							else
							{
								//throw star x3, defend
							}
						}
						break;
					case 8:
						{
							if( (float)GetHealth() / (float)GetMaxHealth()* 0.1f > 0.9f )
							{
								//throw star x4
							}
							else
							{
								//throw star x3 throw rock, defend
							}
						}
						break;
					case 9:
						{
							if( (float)GetHealth() / (float)GetMaxHealth() * 0.1f > 0.9f )
							{
								//throw star x 4, throw rock
							}
							else
							{
								//throw star x4, defend
							}
						}
						break;
					case 10:
						{
							if( (float)GetHealth() / (float)GetMaxHealth() *0.1f > 0.9f )
							{
								//move forward 3 spaces and attack
							}
							else
							{
								// move forward 3 spaces and attack 
							}
						}
						break;
					case 11:
						{
							if( (float)GetHealth() / (float)GetMaxHealth() * 0.1f >0.9f)
							{
								//move forward 3 spaces, attack and throw rock
							}
							else
							{
								//move forward 3 spaces, attack and defend
							}
						}
						break;
					case 12:
						{
							if( (float)GetHealth() / (float)GetMaxHealth() * 0.1f > 0.9f )
							{
								//move forward 3 spaces, attack x2 
							}
							else
							{
								//move forward 3 spaces, attack x2
							}
						}
						break;
					case 13:
						{
							if( (float)GetHealth() / (float)GetMaxHealth() * 0.1f > 0.9f )
							{
								//move forward 3 spaces, attack x2 and throw rock
							}
							else
							{
								//move forward 3 spaces, attack x2 and defend
							}
						}
						break;
					case 14:
						{
							if( (float)GetHealth() / (float)GetMaxHealth() * 0.1f > 0.9f )
							{
								//move forward 3 spaces, attack x2, throw star
							}
							else
							{
								//move forward 3 spaces, attack x2, throw rock
							}
						}
						break;
					case 15:
						{
							if( (float)GetHealth() / (float)GetMaxHealth() * 0.1f > 0.9f )
							{
								//move forward 3 spaces, attack x2, throw star, throw rock
							}
							else
							{
								//move forward 3 spaces, attack x2, throw star, defend
							}
						}
						break;
					case 16:
						{
							if( (float)GetHealth() / (float)GetMaxHealth() * 0.1f > 0.9f )
							{
								//move forward 3 spaces, attack x3
							}
							else
							{
								//move forward 3 spaces, attack x3
							}
						}
						break;
				
				}
			}
			break;
			//out of range by 4 spaces
		case 5:
			{
				switch(GetCurrAP())
				{
					case 0:
						{
							//end turn
						}
						break;
					case 1:
						{
							//end turn
						}
						break;
					case 2:
						{
							//defend
						}
						break;
					case 3:
						{
							//defend
						}
						break;
					case 4:
						{
							//move one space and defend
						}
						break;
					case 5:
						{
							//move one space and defend
						}
						break;
					case 6:
						{
							//move two spaces and defend
						}
						break;
					case 7:
						{
							//move two spaces, throw rock, defend
						}
						break;
					case 8:
						{
							if( (float)GetHealth() / (float)GetMaxHealth() * 0.1f > 0.9f )
							{
								//move three spaces, throw star
							}
							else
							{
								//move two spaces, throw rock, defend
							}
						}
						break;
					case 9:
						{
							if( (float)GetHealth() / (float)GetMaxHealth() * 0.1f > 0.9f )
							{
								//move three spaces, throw star, throw rock
							}
							else
							{
								//move three spaces, throw rock, defend
							}
						}
						break;
					case 10:
						{
							if( (float)GetHealth() / (float)GetMaxHealth() * 0.1f > 0.9f )
							{
								//move three spaces,throw star x2
							}
							else
							{
								//move three spaces, throw star, defend
							}
						}
						break;
					case 11:
						{
							if( (float)GetHealth() / (float)GetMaxHealth() * 0.1f > 0.9f )
							{
								//move three spaces, throw star x2, throw rock
							}
							else
							{
								//move three spaces, throw star, throw rock, defend
							}
						}
						break;
					case 12:
						{
							if( (float)GetHealth() / (float)GetMaxHealth() * 0.1f > 0.9f )
							{
								//move four spaces, attack
							}
							else
							{
								//move three spaces, throw star x2, defend
							}
						}
						break;
					case 13:
						{
							if( (float)GetHealth() / (float)GetMaxHealth() * 0.1f > 0.9f )
							{
								//move four spaces, attack, throw rock
							}
							else
							{
								//move four spaces, attack, throw rocck
							}
						}
						break;
					case 14:
						{
							if( (float)GetHealth() / (float)GetMaxHealth() * 0.1f > 0.9f )
							{
								//move four spaces, attack, throw star
							}
							else
							{
								//move four spaces, attack, defend
							}
						}
						break;
					case 15:
						{
							if( (float)GetHealth() / (float)GetMaxHealth() * 0.1f > 0.9f )
							{
								//move four spaces, attack, throw star, throw rock
							}
							else
							{
								//move four spaces, attack, throw rock, defend
							}
						}
						break;
					case 16:
						{
							if( (float)GetHealth() / (float)GetMaxHealth() * 0.1f > 0.9f )
							{
								//move four spaces, attack x2
							}
							else
							{
								//move four spaces, attack x2
							}
						}
						break;
				}
			}
			break;
			//out of range by 5 units
		case 6:
			{
				switch( GetCurrAP() )
				{
					case 0:
						{
							//end turn
						}
						break;
					case 1:
						{
							//end turn
						}
						break;
					case 2:
						{
							//defend
						}
						break;
					case 3:
						{
							//defend
						}
						break;
					case 4:
						{
							//move one space, defend
						}
						break;
					case 5:
						{
							//move one space, defend
						}
						break;
					case 6:
						{
							//move two spaces, defend
						}
						break;
					case 7:
						{
							//move two spaces, defend
						}
						break;
					case 8:
						{
							if( (float)GetHealth() / (float)GetMaxHealth() * 0.1f > 0.9f )
							{
								//move three spaces, throw star
							}
							else
							{
								//move three spaces, defend
							}
						}
						break;
					case 9:
						{
							if( (float)GetHealth() / (float)GetMaxHealth() * 0.1f > 0.9f )
							{
								//move three spaces, throw star, throw rock
							}
							else
							{
								//move three spaces, throw rock, defend
							}
						}
						break;
					case 10:
						{
							if( (float)GetHealth() / (float)GetMaxHealth() * 0.1f > 0.9f )
							{
								//move three spaces, throw star x2
							}
							else
							{
								//move three spaces, throw star, defend
							}
						}
						break;
					case 11:
						{
							if( (float)GetHealth() / (float)GetMaxHealth() * 0.1f > 0.9f )
							{
								//move three spaces, throw star x2, throw rock
							}
							else
							{
								//move three spaces, throw star, throw rock, defend
							}
						}
						break;
					case 12:
						{
							if( (float)GetHealth() / (float)GetMaxHealth() * 0.1f > 0.9f )
							{
								//move three spaces, throw star x3
							}
							else
							{
								//move three spaces, throw star x2, defend
							}
						}
						break;
					case 13:
						{
							if( (float)GetHealth() / (float)GetMaxHealth() * 0.1f > 0.9f )
							{
								//move three spaces, throw star x3, throw rock
							}
							else
							{
								//move three spaces, throw star x2, throw rock, defend
							}
						}
						break;
					case 14:
						{
							if( (float)GetHealth() / (float)GetMaxHealth() * 0.1f > 0.9f )
							{
								//move three spaces, throw star x4
							}
							else
							{
								//move three spaces, throw star x3, defend
							}
						}
						break;
					case 15:
						{
							if( (float)GetHealth() / (float)GetMaxHealth() * 0.1f > 0.9f )
							{
								//move three spaces, throw star x4, throw rock
							}
							else
							{
								//move three spaces, throw star x3, throw rock, defend
							}
						}
						break;
					case 16:
						{
							if( (float)GetHealth() / (float)GetMaxHealth() * 0.1f > 0.9f )
							{
								//move three spaces, throw star x5
							}
							else
							{
								//move three spaces, throw star x4, defend
							}
						}
						break;
				}
			}
			break;
			//out of range by 6 spaces
		case 7:
			{
				switch( GetCurrAP() )
				{
					case 0:
						{
							//end turn
						}
						break;
					case 1:
						{
							//end turn
						}
						break;
					case 2:
						{
							//move one space
						}
						break;
					case 3:
						{
							//move one space
						}
						break;
					case 4:
						{
							//move two spaces
						}
						break;
					case 5:
						{
							//move two spaces
						}
						break;
					case 6:
						{
							//move three spaces
						}
						break;
					case 7:
						{
							//move three spaces
						}
						break;
					case 8:
						{
							//move four spaces
						}
						break;
					case 9:
						{
							if( (float)GetHealth() / (float)GetMaxHealth() * 0.1f > 0.9f )
							{
								//move four spaces, throw rock
							}
							else
							{
								//move four spaces throw rock
							}
						}
						break;
					case 10:
						{
							if( (float)GetHealth() / (float)GetMaxHealth() * 0.1f > 0.9f )
							{
								//move four spaces, throw star
							}
							else
							{
								//move four spaces, defend
							}
						}
						break;
					case 11:
						{
							if( (float)GetHealth() / (float)GetMaxHealth() * 0.1f > 0.9f )
							{
								//move four spaces, throw star, throw rock
							}
							else
							{
								//move four spaces, throw rock, defend
							}
						}
						break;
					case 12:
						{
							if( (float)GetHealth() / (float)GetMaxHealth() * 0.1f > 0.9f )
							{
								//move four spaces, throw star x2
							}
							else
							{
								//move four spaces, throw star, defend
							}
						}
						break;
					case 13:
						{
							if( (float)GetHealth() / (float)GetMaxHealth() * 0.1f > 0.9f )
							{
								//move four spaces, throw star x2, throw rock
							}
							else
							{
								//move four spaces, throw star, throw rock, defend
							}
						}
						break;
					case 14:
						{
							if( (float)GetHealth() / (float)GetMaxHealth() * 0.1f > 0.9f )
							{
								//move four spaces, throw star x3
							}
							else
							{
								// move four spaces, throw star x2, defend
							}
						}
						break;
					case 15:
						{
							if( (float)GetHealth() / (float)GetMaxHealth() * 0.1f > 0.9f )
							{
								//move four spaces, throw star x3, throw rock
							}
							else
							{
								//move four spaces, throw star x2, throw rock, defend
							}
						}
						break;
					case 16:
						{
							if( (float)GetHealth() / (float)GetMaxHealth() * 0.1f > 0.9f )
							{
								//move four spaces, throw star x4
							}
							else
							{
								//move four spaces, throw star x3, defend
							}
						}
						break;
				}
			}
			break;

			//out 7 spaces
		case 8:
			{
				switch(GetCurrAP())
				{
					case 0:
						{
							//end turn
						}
						break;
					case 1:
						{
							//end turn
						}
						break;
					case 2:
						{
							//defend
						}
						break;
					case 3:
						{
							//defend
						}
						break;
					case 4:
						{
							//move one space, defend
						}
						break;
					case 5:
						{
							//move one space, defend
						}
						break;
					case 6:
						{
							//move two spaces, defend
						}
						break;
					case 7:
						{
							//move two spaces, defend
						}
						break;
					case 8:
						{
							//move three spaces, defend
						}
						break;
					case 9:
						{
							//move three spaces, defend
						}
						break;
					case 10:
						{
							//move four spaces, defend
						}
						break;
					case 11:
						{
							if( (float)GetHealth() / (float)GetMaxHealth() * 0.1f > 0.9f )
							{
								//move five spaces, throw rock
							}
							else
							{
								//move four spaces, defend
							}
						}
						break;
					case 12:
						{
							if( (float)GetHealth() / (float)GetMaxHealth() * 0.1f > 0.9f )
							{
								//move five spaces, throw star
							}
							else
							{
								//move five spaces, defend
							}
						}
						break;
					case 13:
						{
							if( (float)GetHealth() / (float)GetMaxHealth() * 0.1f > 0.9f )
							{
								//move five spaces, throw star, throw rock
							}
							else
							{
								//move five spaces, throw rock, defend
							}
						}
						break;
					case 14:
						{
							if( (float)GetHealth() / (float)GetMaxHealth() * 0.1f > 0.9f )
							{
								//move five spaces, throw star x2
							}
							else
							{
								//move five spaces, throw star, defend
							}
						}
						break;
					case 15:
						{
							if( (float)GetHealth() / (float)GetMaxHealth() * 0.1f > 0.9f )
							{
								//move five spaces, throw star x2, throw rock
							}
							else
							{
								//move five spaces, throw star, throw rock, defend
							}
						}
						break;
					case 16:
						{
							if( (float)GetHealth() / (float)GetMaxHealth() * 0.1f > 0.9f )
							{
								//move five spaces, throw star x3
							}
							else
							{
								//move five spaces, throw star x2, defend
							}
						}
						break;
				}
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

	if( ( (float)GetHealth()/(float)GetMaxHealth()) < ( ( (float)GetMaxHealth() * 0.3f ) / (float)GetMaxHealth() ) )
	{
		m_nLowHealth = 1;
	}
	else m_nLowHealth = 0;

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
	m_vAnimations[m_nCurrAnimation].Render((int)GetPosX(), (int)GetPosY(), GetPosZ(), 1.2f);
}