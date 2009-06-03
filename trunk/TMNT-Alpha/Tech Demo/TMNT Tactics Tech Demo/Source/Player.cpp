////////////////////////////////////////////////////////
//   File Name	:	"CPlayer.cpp"
//
//   Author		:	Matthew Di Matteo (MD)
//
//   Purpose	:	To provide a game object to be used as the player
//					being controlled by user during the game.
////////////////////////////////////////////////////////

#include "Player.h"
#include "Turtle.h"
#include "Factory.h"
#include "tinyxml/tinyxml.h"
#include "CSGD_TextureManager.h"
#include "Skill.h"
#include <string>
#include <fstream>
using namespace std;

#include "Animation.h"

CPlayer::CPlayer(void)
{
	m_pLeonardo = Factory::CreateTurtle();
	m_pDonatello = Factory::CreateTurtle();
	m_pRaphael = Factory::CreateTurtle();
	m_pMikey = Factory::CreateTurtle();

	CAnimation anim;
	anim.Load("Resources/AnimationInfo/VG_leonardo.dat", 1);
	m_pLeonardo->AddAnim(anim);
	anim.Load("Resources/AnimationInfo/VG_leonardo.dat", 2);
	m_pLeonardo->AddAnim(anim);

	m_pLeonardo->SetPosX(64);
	m_pLeonardo->SetPosY(364);

	anim.Load("Resources/AnimationInfo/VG_donatello.dat", 1);
	m_pDonatello->AddAnim(anim);
	anim.Load("Resources/AnimationInfo/VG_donatello.dat", 2);
	m_pDonatello->AddAnim(anim);

	m_pDonatello->SetPosX(96);
	m_pDonatello->SetPosY(380);

	anim.Load("Resources/AnimationInfo/VG_raphael.dat", 1);
	m_pRaphael->AddAnim(anim);
	anim.Load("Resources/AnimationInfo/VG_raphael.dat", 2);
	m_pRaphael->AddAnim(anim);

	m_pRaphael->SetPosX(96);
	m_pRaphael->SetPosY(348);

	anim.Load("Resources/AnimationInfo/VG_michelangelo1.dat", 1);
	m_pMikey->AddAnim(anim);
	anim.Load("Resources/AnimationInfo/VG_michelangelo1.dat", 2);
	m_pMikey->AddAnim(anim);
	
	m_pMikey->SetPosX(128);
	m_pMikey->SetPosY(364);

}

CPlayer::~CPlayer(void)
{
	//for(int i = 0; i < m_pLeonardo->animations.size; i++)
	//{
		//m_pLeonardo->animations[i].Unload();
	//}
	//for(int i = 0; i < m_pDonatello->animations.size; i++)
	//{
		//m_pDonatello->animations[i].Unload();
	//}
	/*for(int i = 0; i < m_pRaphael->animations.size; i++)
	{
		m_pRaphael->animations[i].Unload();
	}
	for(int i = 0; i < m_pMikey->animations.size; i++)
	{
		m_pMikey->animations[i].Unload();
	}*/
	
	
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
}

void CPlayer::LoadSavedGame(const char* fileName)
{
	ifstream ifs;
	ifs.open(fileName, ios_base::in | ios_base::binary);

	if (!ifs.good())
		{MessageBox(0, "Failed to load saved game.", "Error", MB_OK); return;}
}

void CPlayer::LoadNewSkills(const char* filename)
{
	TiXmlDocument doc;

	if (!doc.LoadFile(filename))
		{MessageBox(0, "Failed to load new skills.", "Error", MB_OK); return;}

	int type, dmg, range, cost, combAmt, numSkills, turtleID, skillID; string name;
	vector<CSkill> inactiveSkills;

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
			CSkill Skill = CSkill(name, type, skillID, dmg, range, cost, combAmt);
			inactiveSkills.push_back(Skill);
			pSkill = pSkill->NextSiblingElement();
		}

		switch (turtleID)
		{
		case LEONARDO:
			m_pLeonardo->SetSkillsInactive(inactiveSkills);
			break;
		case DONATELLO:
			m_pDonatello->SetSkillsInactive(inactiveSkills);
			break;
		case RAPHAEL:
			m_pRaphael->SetSkillsInactive(inactiveSkills);
			break;
		case MIKEY:
			m_pMikey->SetSkillsInactive(inactiveSkills);
			break;
		}
		inactiveSkills.clear();
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

		m_pLeonardo->SetAttributes(ap,hp,strength,defense,accuracy,speed,level,experience,range);
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

		m_pRaphael->SetAttributes(ap,hp,strength,defense,accuracy,speed,level,experience,range);
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

		m_pDonatello->SetAttributes(ap,hp,strength,defense,accuracy,speed,level,experience,range);
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

		m_pMikey->SetAttributes(ap,hp,strength,defense,accuracy,speed,level,experience,range);
	}
	return true;
}
//void CPlayer::Render()
//{
//	m_pLeonardo->Render();
//	m_pDonatello->Render();
//	m_pRaphael->Render(); 
//	m_pMikey->Render();
//
//
//}
//void CPlayer::Update(float fElapsedTime)
//{
//	m_pLeonardo->Update(fElapsedTime);
//	m_pDonatello->Update(fElapsedTime);
//	m_pRaphael->Update(fElapsedTime); 
//	m_pMikey->Update(fElapsedTime);
//}