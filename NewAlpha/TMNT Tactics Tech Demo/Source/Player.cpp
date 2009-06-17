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

	CBase weapon;
	weapon.SetWeapon("Bokken",6,0,-1,0);
	m_pTurtles[LEONARDO]->AddWeapon( weapon);

	weapon.SetWeapon("Oak Bo Staff",5,1,-1,7);
	m_pTurtles[DONATELLO]->AddWeapon( weapon);

	weapon.SetWeapon("Rusty Sais",6,0,-1,10);
	m_pTurtles[RAPHAEL]->AddWeapon( weapon);

	weapon.SetWeapon("Wooden Nunchaku",3,0,-1,4);
	m_pTurtles[MIKEY]->AddWeapon( weapon); 	

	LoadAnimations();
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
	LoadNewSkills("Resources/XML/VG_TurtleSkills.xml");
	LoadTurtleStats("Resources/XML/VG_TurtleStats.xml");
	POINT pt; pt.x = 15; pt.y = 18;
	Factory::GetInstance()->CreateBattleItem(GRENADO, pt);
}

void CPlayer::LoadSavedGame(const char* fileName)
{
	ifstream ifs;
	ifs.open(fileName, ios_base::in | ios_base::binary);

	if (!ifs.good())
		{MessageBox(0, "Failed to load saved game.", "Error", MB_OK); return;}
	for (int i = 0; i < 4; ++i)
	{
		if (m_pTurtles[i])
		{
			char* name = m_pTurtles[i]->GetName();
			ObjectManager::GetInstance()->Remove(m_pTurtles[i]);
			delete m_pTurtles[i];
			Factory::GetInstance()->CreateTurtle(name);
		}
		ifs.read(reinterpret_cast<char*>(m_pTurtles[i]), sizeof(CTurtle));
	}
}
void CPlayer::SaveGame(const char* fileName)
{
	ofstream ofs;
	ofs.open(fileName, ios_base::binary | ios_base::out);
	CTurtle** turtles = CPlayer::GetInstance()->GetTurtles();

	if (ofs.is_open())
	{
		for (int i = 0; i < 4; ++i)
		{
			ofs.write((char*)(&turtles[i]), sizeof(CTurtle));
		}
	}
	else
		ofs.close();
}

void CPlayer::LoadNewSkills(const char* filename)
{
	TiXmlDocument doc;

	if (!doc.LoadFile(filename))
		{MessageBox(0, "Failed to load new skills.", "Error", MB_OK); return;}

	int type, dmg, range, cost, combAmt, numSkills, turtleID, skillID; string name; double duration;
	vector<CSkill> inactiveSkills, activeSkill;

	TiXmlElement* pRoot = doc.RootElement();
	TiXmlElement* pTurtle = pRoot->FirstChildElement("TURTLE");
	while (pTurtle)
	{
		pTurtle->Attribute("NumberOfSkills", &numSkills);
		pTurtle->Attribute("TurtleName", &turtleID);
		TiXmlElement* pSkill;
		pSkill = pTurtle->FirstChildElement("SKILL");
		for (int i = 0; i < numSkills; ++i)
		{
			char* tempName = (char*)pSkill->Attribute("Name");
			name = tempName;
			pSkill->Attribute("ID", &skillID);
			pSkill->Attribute("Type", &type);
			pSkill->Attribute("Dmg", &dmg);
			pSkill->Attribute("Range", &range);
			pSkill->Attribute("Cost", &cost);
			pSkill->Attribute("CombAmt", & combAmt);
			pSkill->Attribute("Duration", &duration);
			CSkill* Skill = new CSkill(name, type, skillID, dmg, range, cost, combAmt, duration);
			if (i > 0)
				inactiveSkills.push_back(*Skill);
			else
				activeSkill.push_back(*Skill);
			pSkill = pSkill->NextSiblingElement();
			delete Skill;
		}
		m_pTurtles[turtleID]->SetSkillsActive(activeSkill);
		m_pTurtles[turtleID]->SetSkillsInactive(inactiveSkills);
		inactiveSkills.clear();
		activeSkill.clear();
		pTurtle = pTurtle->NextSiblingElement("TURTLE");
	}
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
	anim.Load("Resources/AnimationInfo/VG_leonardo.dat", 1,0.15f);
	m_pTurtles[LEONARDO]->AddAnim(anim);
	anim.Load("Resources/AnimationInfo/VG_leonardo.dat", 2,0.15f);
	m_pTurtles[LEONARDO]->AddAnim(anim);

	anim.Load("Resources/AnimationInfo/VG_donatello.dat", 1,0.15f);
	m_pTurtles[DONATELLO]->AddAnim(anim);
	anim.Load("Resources/AnimationInfo/VG_donatello.dat", 2,0.15f);
	m_pTurtles[DONATELLO]->AddAnim(anim);

	anim.Load("Resources/AnimationInfo/VG_raphael.dat", 1,0.15f);
	m_pTurtles[RAPHAEL]->AddAnim(anim);
	anim.Load("Resources/AnimationInfo/VG_raphael.dat", 2,0.15f);
	m_pTurtles[RAPHAEL]->AddAnim(anim);

	anim.Load("Resources/AnimationInfo/VG_michelangelo1.dat", 1,0.15f);
	m_pTurtles[MIKEY]->AddAnim(anim);
	anim.Load("Resources/AnimationInfo/VG_michelangelo1.dat", 2,0.15f);
	m_pTurtles[MIKEY]->AddAnim(anim);
}
bool CPlayer::RemoveItem(int index)
{
	vector<CBattleItem>::iterator iter = m_nInventory.begin();
	int count = 0;
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
