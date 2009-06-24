////////////////////////////////////////////////////////
//   File Name	:	"CPlayer.cpp"
//
//   Author		:	Matthew Di Matteo (MD)
//
//   Purpose	:	To provide a game object to be used as the player
//					being controlled by user during the game.
////////////////////////////////////////////////////////
#include "Player.h"
#include "Factory.h"
#include "tinyxml/tinyxml.h"
#include "CSGD_TextureManager.h"
#include "ObjectManager.h"
#include "Skill.h"
#include "Achievements.h"
#include "Game.h"
#include <string>
#include <fstream>
using namespace std;

#include "Animation.h"

CPlayer::CPlayer(void)
{
	m_pTurtles[LEONARDO] = Factory::GetInstance()->CreateTurtle("Leonardo");
	m_pTurtles[DONATELLO]= Factory::GetInstance()->CreateTurtle("Donatello");
	m_pTurtles[RAPHAEL]	 = Factory::GetInstance()->CreateTurtle("Raphael");
	m_pTurtles[MIKEY]	 = Factory::GetInstance()->CreateTurtle("Michelangelo");
	LoadAnimations();
	m_sProfileName = "NONE"; m_sFileName = "NONE";

	m_pTurtles[LEONARDO]->GetCurrAnim()->Play();
	m_pTurtles[DONATELLO]->GetCurrAnim()->Play();
	m_pTurtles[RAPHAEL]->GetCurrAnim()->Play();
	m_pTurtles[MIKEY]->GetCurrAnim()->Play();	 

	m_nCurrStage = 0; m_nKillCount = 0;
}

void CPlayer::Exit()
{
	delete m_pAcheivements; m_pAcheivements = NULL;
}

CPlayer::~CPlayer(void)
{
	for(int i = 0; i < 4; i++)
	{
		for(unsigned int j = 0; j < m_pTurtles[i]->GetAnimations().size(); j++)
			m_pTurtles[i]->GetAnimations()[j].Unload();
	}
}

CPlayer* CPlayer::GetInstance()
{
	static CPlayer instance;
	return &instance;
}

void CPlayer::NewGame()
{
	m_sProfileName = "NONE"; m_sFileName = "NONE";
	ObjectManager::GetInstance()->RemoveAll();
	GetItems()->clear();

	m_pAcheivements = new CAchievements();
	for (int i = 1; i < NUM_MAPS; ++i)		// lock all maps, except first one
		m_bMapsUnlocked[i] = false;
	m_bMapsUnlocked[0] = true;

	//////////////////////////////////////////////////////////////////////////
	// create and add starting weapons
	for (int i = 0; i < 4; ++i)
	{
		m_pTurtles[i]->ClearWeapons();
		m_pTurtles[i]->ClearActiveSkills();
		m_pTurtles[i]->ClearInactiveSkills();
	}
	CGame::GetInstance()->LoadNewSkills("Resources/XML/VG_TurtleSkills.xml");
	CBase weapon;
	weapon.SetWeapon("Bokken",6,0,-1,0);
	m_pTurtles[LEONARDO]->AddWeapon( weapon );

	weapon.SetWeapon("Oak Bo Staff",5,1,-1,7);
	m_pTurtles[DONATELLO]->AddWeapon( weapon );

	weapon.SetWeapon("Rusty Sais",6,0,-1,10);
	m_pTurtles[RAPHAEL]->AddWeapon( weapon );

	weapon.SetWeapon("Wooden Nunchaku",3,0,-1,4);
	m_pTurtles[MIKEY]->AddWeapon( weapon ); 	
	//////////////////////////////////////////////////////////////////////////

	m_nCurrStage = 0;	// current stage starts at the first battle map

	// set stats to starting values
	LoadTurtleStats("Resources/XML/VG_TurtleStats.xml");

	//////////////////////////////////////////////////////////////////////////
	// TODO:: remove temp items before final build
	POINT pt; pt.x = 3; pt.y = 17;
	Factory::GetInstance()->CreateBattleItem(BLACK_EGGS, pt);
	Factory::GetInstance()->CreateBattleItem(GRENADO, pt);
	Factory::GetInstance()->CreateBattleItem(PIZZA, pt);
}

bool CPlayer::LoadTurtleStats(const char* szXmlFileName)
{
	TiXmlDocument doc;
	int ap = 0, hp = 0, strength = 0, defense = 0, accuracy = 0, speed = 0, level = 0, experience = 0, range = 0;

	if(!doc.LoadFile(szXmlFileName))
		{MessageBox(0, "Failed to load turtle stats.", "Error",  MB_OK);return false;}

	TiXmlElement* pRoot = doc.RootElement();
	TiXmlElement* pLeo = pRoot->FirstChildElement("Leonardo");

	if(pLeo)
	{
		pLeo->Attribute("AP", &ap);
		pLeo->Attribute("HP", &hp);
		pLeo->Attribute("strength", &strength);
		pLeo->Attribute("defense", &defense);
		pLeo->Attribute("accuracy", &accuracy);
		pLeo->Attribute("speed", &speed);
		pLeo->Attribute("level", &level);
		pLeo->Attribute("exp", &experience);
		pLeo->Attribute("range", &range);

		m_pTurtles[LEONARDO]->SetAttributes(ap,hp,strength,defense,accuracy,speed,level,experience,range);
	}
	TiXmlElement* pRaph = pLeo->NextSiblingElement("Raphael");
	if(pRaph)
	{
		pRaph->Attribute("AP", &ap);
		pRaph->Attribute("HP", &hp);
		pRaph->Attribute("strength", &strength);
		pRaph->Attribute("defense", &defense);
		pRaph->Attribute("accuracy", &accuracy);
		pRaph->Attribute("speed", &speed);
		pRaph->Attribute("level", &level);
		pRaph->Attribute("exp", &experience);
		pRaph->Attribute("range", &range);

		m_pTurtles[RAPHAEL]->SetAttributes(ap,hp,strength,defense,accuracy,speed,level,experience,range);
	}
	TiXmlElement* pDon = pRaph->NextSiblingElement("Donatello");
	if(pDon)
	{
		pDon->Attribute("AP", &ap);
		pDon->Attribute("HP", &hp);
		pDon->Attribute("strength", &strength);
		pDon->Attribute("defense", &defense);
		pDon->Attribute("accuracy", &accuracy);
		pDon->Attribute("speed", &speed);
		pDon->Attribute("level", &level);
		pDon->Attribute("exp", &experience);
		pDon->Attribute("range", &range);

		m_pTurtles[DONATELLO]->SetAttributes(ap,hp,strength,defense,accuracy,speed,level,experience,range);
	}
	
	TiXmlElement* pMikey = pDon->NextSiblingElement("Michelangelo");
	if(pMikey)
	{
		pMikey->Attribute("AP", &ap);
		pMikey->Attribute("HP", &hp);
		pMikey->Attribute("strength", &strength);
		pMikey->Attribute("defense", &defense);
		pMikey->Attribute("accuracy", &accuracy);
		pMikey->Attribute("speed", &speed);
		pMikey->Attribute("level", &level);
		pMikey->Attribute("exp", &experience);
		pMikey->Attribute("range", &range);

		m_pTurtles[MIKEY]->SetAttributes(ap,hp,strength,defense,accuracy,speed,level,experience,range);
	}
	
	return true;
}

void CPlayer::LoadAnimations()
{
	CAnimation anim;
	anim.Load("Resources/AnimationInfo/VG_leonardo3.dat", 1,0.25f);
	m_pTurtles[LEONARDO]->AddAnim(anim);
	anim.Load("Resources/AnimationInfo/VG_leonardo3.dat", 2,0.15f, false);
	m_pTurtles[LEONARDO]->AddAnim(anim);
	anim.Load("Resources/AnimationInfo/VG_leonardo3.dat", 3,0.15f);
	m_pTurtles[LEONARDO]->AddAnim(anim);
	anim.Load("Resources/AnimationInfo/VG_leonardo3.dat", 4,0.25f, false);
	m_pTurtles[LEONARDO]->AddAnim(anim);
	anim.Load("Resources/AnimationInfo/VG_leonardo3.dat", 5,0.15f);
	m_pTurtles[LEONARDO]->AddAnim(anim);
	anim.Load("Resources/AnimationInfo/VG_leonardo3.dat", 6,0.15f);
	m_pTurtles[LEONARDO]->AddAnim(anim);
	anim.Load("Resources/AnimationInfo/VG_leonardo3.dat", 7,0.15f);
	m_pTurtles[LEONARDO]->AddAnim(anim);
	anim.Load("Resources/AnimationInfo/VG_leonardo3.dat", 8,0.15f);
	m_pTurtles[LEONARDO]->AddAnim(anim);
	anim.Load("Resources/AnimationInfo/VG_leonardo3.dat", 9,0.15f);
	m_pTurtles[LEONARDO]->AddAnim(anim);
	anim.Load("Resources/AnimationInfo/VG_leonardo3.dat", 10,0.5f,true);
	m_pTurtles[LEONARDO]->AddAnim(anim);
	anim.Load("Resources/AnimationInfo/VG_leonardo3.dat", 11,0.15f);
	m_pTurtles[LEONARDO]->AddAnim(anim);
	anim.Load("Resources/AnimationInfo/VG_leonardo3.dat", 12,0.15f);
	m_pTurtles[LEONARDO]->AddAnim(anim);
	anim.Load("Resources/AnimationInfo/VG_leonardo3.dat", 13,0.15f);
	m_pTurtles[LEONARDO]->AddAnim(anim);

	anim.Load("Resources/AnimationInfo/VG_donatello.dat", 1,0.25f);
	m_pTurtles[DONATELLO]->AddAnim(anim);
	anim.Load("Resources/AnimationInfo/VG_donatello.dat", 2,0.15f, false);
	m_pTurtles[DONATELLO]->AddAnim(anim);
	anim.Load("Resources/AnimationInfo/VG_donatello.dat", 3,0.15f);
	m_pTurtles[DONATELLO]->AddAnim(anim);
	anim.Load("Resources/AnimationInfo/VG_donatello.dat", 4,0.15f, false);
	m_pTurtles[DONATELLO]->AddAnim(anim);
	anim.Load("Resources/AnimationInfo/VG_donatello.dat", 5,0.15f);
	m_pTurtles[DONATELLO]->AddAnim(anim);
	anim.Load("Resources/AnimationInfo/VG_donatello.dat", 6,0.15f);
	m_pTurtles[DONATELLO]->AddAnim(anim);
	anim.Load("Resources/AnimationInfo/VG_donatello.dat", 7,0.15f);
	m_pTurtles[DONATELLO]->AddAnim(anim);
	anim.Load("Resources/AnimationInfo/VG_donatello.dat", 8,0.15f);
	m_pTurtles[DONATELLO]->AddAnim(anim);
	anim.Load("Resources/AnimationInfo/VG_donatello.dat", 9,0.15f);
	m_pTurtles[DONATELLO]->AddAnim(anim);
	anim.Load("Resources/AnimationInfo/VG_donatello.dat", 10,0.5f,false);
	m_pTurtles[DONATELLO]->AddAnim(anim);
	anim.Load("Resources/AnimationInfo/VG_donatello.dat", 11,0.15f);
	m_pTurtles[DONATELLO]->AddAnim(anim);

	anim.Load("Resources/AnimationInfo/VG_raphael.dat", 1,0.25f);
	m_pTurtles[RAPHAEL]->AddAnim(anim);
	anim.Load("Resources/AnimationInfo/VG_raphael.dat", 2,0.15f, false);
	m_pTurtles[RAPHAEL]->AddAnim(anim);
	anim.Load("Resources/AnimationInfo/VG_raphael.dat", 3,0.15f);
	m_pTurtles[RAPHAEL]->AddAnim(anim);
	anim.Load("Resources/AnimationInfo/VG_raphael.dat", 4,0.15f, false);
	m_pTurtles[RAPHAEL]->AddAnim(anim);
	anim.Load("Resources/AnimationInfo/VG_raphael.dat", 5,0.15f);
	m_pTurtles[RAPHAEL]->AddAnim(anim);
	anim.Load("Resources/AnimationInfo/VG_raphael.dat", 6,0.15f);
	m_pTurtles[RAPHAEL]->AddAnim(anim);
	anim.Load("Resources/AnimationInfo/VG_raphael.dat", 7,0.15f);
	m_pTurtles[RAPHAEL]->AddAnim(anim);
	anim.Load("Resources/AnimationInfo/VG_raphael.dat", 8,0.15f);
	m_pTurtles[RAPHAEL]->AddAnim(anim);
	anim.Load("Resources/AnimationInfo/VG_raphael.dat", 9,0.15f);
	m_pTurtles[RAPHAEL]->AddAnim(anim);
	anim.Load("Resources/AnimationInfo/VG_raphael.dat", 10,0.5f,false);
	m_pTurtles[RAPHAEL]->AddAnim(anim);

	anim.Load("Resources/AnimationInfo/VG_michelangelo.dat", 1,0.25f);
	m_pTurtles[MIKEY]->AddAnim(anim);
	anim.Load("Resources/AnimationInfo/VG_michelangelo.dat", 2,0.15f, false);
	m_pTurtles[MIKEY]->AddAnim(anim);
	anim.Load("Resources/AnimationInfo/VG_michelangelo.dat", 3,0.15f);
	m_pTurtles[MIKEY]->AddAnim(anim);
	anim.Load("Resources/AnimationInfo/VG_michelangelo.dat", 4,0.15f, false);
	m_pTurtles[MIKEY]->AddAnim(anim);
	anim.Load("Resources/AnimationInfo/VG_michelangelo.dat", 5,0.15f);
	m_pTurtles[MIKEY]->AddAnim(anim);
	anim.Load("Resources/AnimationInfo/VG_michelangelo.dat", 6,0.15f);
	m_pTurtles[MIKEY]->AddAnim(anim);
	anim.Load("Resources/AnimationInfo/VG_michelangelo.dat", 7,0.15f);
	m_pTurtles[MIKEY]->AddAnim(anim);
	anim.Load("Resources/AnimationInfo/VG_michelangelo.dat", 8,0.15f);
	m_pTurtles[MIKEY]->AddAnim(anim);
	anim.Load("Resources/AnimationInfo/VG_michelangelo.dat", 9,0.15f);
	m_pTurtles[MIKEY]->AddAnim(anim);
	anim.Load("Resources/AnimationInfo/VG_michelangelo.dat", 10,0.5f,false);
	m_pTurtles[MIKEY]->AddAnim(anim);
}
bool CPlayer::RemoveItem(int index)
{
	vector<CBattleItem>::iterator iter = m_nInventory.begin();
	unsigned int count = 0;
	while(count < m_nInventory.size())
	{
		if (count == index)
		{
			iter = m_nInventory.erase(iter);
			return true;
		}
		++count;
		++iter;
	}
	return false;
}
