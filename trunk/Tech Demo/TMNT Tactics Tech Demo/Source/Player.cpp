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

#include"tinyxml/tinyxml.h"

CPlayer::CPlayer(void)
{
	m_pLeonardo = Factory::CreateTurtle();
	m_pDonatello = Factory::CreateTurtle();
	m_pRaphael = Factory::CreateTurtle();
	m_pMikey = Factory::CreateTurtle();

}

CPlayer::~CPlayer(void)
{
}
CPlayer* CPlayer::GetInstance()
{
	static CPlayer instance;
	return &instance;
}
bool CPlayer::Load(const char* szXmlFileName)
{
	TiXmlDocument doc;
	int ap = 0, hp = 0, strength = 0, defense = 0, accuracy = 0, speed = 0, level = 0, experience = 0, range = 0;

	if(!doc.LoadFile(szXmlFileName))
		return false;

	TiXmlElement* pRoot = doc.RootElement();
	TiXmlElement* pLeo = pRoot->FirstChildElement("Leonardo");

	if(pLeo)
	{
		
		
		/*const char* pText = pEntry->GetText();
		strcpy_s(hs[i].buffer, _countof(hs[i].buffer), pText);*/

		
		
		pLeo->Attribute("AP", &ap);
		pLeo->Attribute("HP", &hp);
		pLeo->Attribute("strength", &strength);
		pLeo->Attribute("defense", &defense);
		pLeo->Attribute("accuracy", &accuracy);
		pLeo->Attribute("level", &level);
		pLeo->Attribute("exp", &experience);
		pLeo->Attribute("range", &range);

		m_pLeonardo->SetAttribute(ap,hp,strength,defense,accuracy,speed,level,experience,range);
		
		
		//pLeo = pLeo->NextSiblingElement("entry");
		
		

	}
	
}
