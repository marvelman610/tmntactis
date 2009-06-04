////////////////////////////////////////////////////////
//   File Name	:	"Ninja.cpp"
//
//   Author		:	Matthew Di Matteo (MD)
//
//   Purpose	:	To provide a game object to be used as an emeny
//					against the user during the game.
////////////////////////////////////////////////////////

#include "Ninja.h"

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

}

CNinja::~CNinja(void)
{
}

void CNinja::AI()
{
	switch(m_nInRange)
	{
		case 0:
			{
				if(m_nLowHealth == 0)
				{
					// range 0 (turtle next to them)
					if(GetCurrAP() >= 4)
					{
						m_nAttack = 1;
					}
					else m_nAttack = 0;

					if(GetCurrAP() == 2)
					{
						m_nDefend = 1;
						//end turn when defend is chosen
					}
				}
				else //if low health == 1
				{
					//if( HaveHealingItem == true)
					//{
						//use item
					//		if(GetCurrAP() > 1)
					//			{ move away }
					//}

					if(GetCurrAP() > 1)
					{
						//move away towards your team
					}
				}
				if( GetCurrAP() == 0 )
				{
					//end turn
				}
			}
			break;
		case 1:
			{
				if(m_nLowHealth == 0)
				{
					if(GetCurrAP() >= 4)
					{
						m_nAttack = 1;
					}
					//else if ap < 4 { move back a tile or two & defend}
				}
				if( GetCurrAP() == 0 )
				{
					//end turn
				}
			}
			break;
		case 2:
			{
				if(m_nLowHealth == 0)
				{
					if(GetCurrAP() >=5)
					{//move one space closer
						//attack until ap runs out
					}
					if(GetCurrAP() < 5)
					{
						//move back and defend
					}
				}
			}
			break;
		case 3:
			{
				if(m_nLowHealth == 0)
				{
					if(GetCurrAP() >=6)
					{
						//move two spaces closer
						//attack
					}
					if(GetCurrAP() < 6)
					{
						//move away and defend
					}
				}
			}
			break;
		case 4:
			{
				if(m_nLowHealth == 0)
				{
					if(GetCurrAP() >= 7)
					{
						// move in three spaces and attack
					}
					if(GetCurrAP() < 7)
					{
						//run and defend
					}
				}
			}
			break;
		case 5:
			{
				if(m_nLowHealth == 0)
				{
					if(GetCurrAP() >=8 )
					{
						//move four tiles and attack
					}
					if(GetCurrAP() < 8)
					{
						//move away and defend
					}
				}
			}
			break;
		case 6:
			{
				if(m_nLowHealth == 0)
				{
					if(GetCurrAP() >= 9)
					{
						//move five tiles and attack
					}
					if(GetCurrAP() < 9)
					{
						//move away and defend
					}
				}
			}
			break;
		case 7:
			{
				if(m_nLowHealth == 0)
				{
					if(GetCurrAP() >= 10)
					{
						// move 6 tiles and attack
					}
					if(GetCurrAP() < 10)
					{
						// move in 2 tiles and defend
					}
				}
			}
			break;
		case 8:
			{
				if(m_nLowHealth == 0)
				{
					if(GetCurrAP() >= 11)
					{
						//move 7 tiles and attack
					}
					if(GetCurrAP() < 11)
					{
						//move in 3 tiles and defend
					}
				}
			}
			break;

		default:
			break;
	}
}
void CNinja::Update(float fElapsedTime)
{
	if( ( (float)GetHealth()/(float)GetMaxHealth()) < (3.0f/10.0f))
	{
		m_nLowHealth = 1;
	}
	else m_nLowHealth = 0;

	if( GetExperience() >= 100)
	{
		SetExperience( GetExperience() - 100 );
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

}