////////////////////////////////////////////////////////
//   File Name	:	"Factory.cpp"
//
//   Author		:	Matthew Di Matteo (MD)
//
//   Purpose	:	Creates base objects
////////////////////////////////////////////////////////

#include"Factory.h"
#include"ObjectManager.h"
#include "tinyxml/tinyxml.h"
#include"Ninja.h"
#include"Boss.h"
#include"Turtle.h"
#include"BattleItem.h" 
#include"BattleMap.h"

#define TOTAL_NUM_WEAPONS 12

Factory::Factory(void)
{
	for(int i = 0; i < TOTAL_NUM_WEAPONS; i++)
	{
		weapons[i] = new CBase;
	}
	LoadItems("Resources/XML/VG_Items.xml");
}

Factory::~Factory(void)
{
	for(int i = 0; i < TOTAL_NUM_WEAPONS; i++)
	{
		delete weapons[i];
	}
}
Factory* Factory::GetInstance()
{
	static Factory instance;
	return &instance;
}
bool Factory::LoadItems(const char* fileName)
{
	TiXmlDocument doc;

	if(!doc.LoadFile(fileName))
	{
		MessageBox(0, "Failed to load items.", "Error",  MB_OK);
		return false;
	}

	TiXmlElement* pRoot = doc.RootElement();
	TiXmlElement* pBattleItem = pRoot->FirstChildElement("BattleItem");
	if(pBattleItem)
	{
		char* name[3];
		int dmg[3], range[3], radius[3];
		TiXmlElement* pItem = pBattleItem->FirstChildElement("Item");
		
		for(int i = 0; i < 3; i++)
		{
			
			name[i] = (char*)pItem->Attribute("name");
			pItem->Attribute("Dmg", &dmg[i]);
			pItem->Attribute("range", &range[i]);
			pItem->Attribute("radius", &radius[i]);

			pItem = pItem->NextSiblingElement("Item");
		}

		blackeggs.SetName(name[0]);
		blackeggs.SetDamage(dmg[0]);
		blackeggs.SetRange(range[0]);
		blackeggs.SetRadius(radius[0]);

		grenado.SetName(name[1]);
		grenado.SetDamage(dmg[1]);
		grenado.SetRange(range[1]);
		grenado.SetRadius(radius[1]);

		pizza.SetName(name[2]);
		pizza.SetDamage(dmg[2]);
		pizza.SetRange(range[2]);
		pizza.SetRadius(radius[2]);
	}
	TiXmlElement* pWeapon = pBattleItem->NextSiblingElement("Weapon");
	if(pWeapon)
	{
		char* Name[12];
		int atk[12], def[12];
		TiXmlElement* pItem = pWeapon->FirstChildElement("Item");
		for(int i = 0; i < 12; i++)
		{
			Name[i] = (char*)pItem->Attribute("name");
			pItem->Attribute("atk", &atk[i]);
			pItem->Attribute("def", &def[i]);

			weapons[i]->SetName(Name[i]);
			weapons[i]->SetStrength(atk[i]);
			weapons[i]->SetDefense(def[i]);

			pItem = pItem->NextSiblingElement("Item");
		}


	}

	return true;
}
CBoss* Factory::CreateBoss()
{
	CBoss* boss = new CBoss();
	ObjectManager::GetInstance()->Add(boss);
	return boss;
}

CNinja* Factory::CreateNinja()
{
	CNinja* ninja = new CNinja();

	CAnimation anim;
	anim.Load("Resources/AnimationInfo/VG_whiteninja.dat", 1);
	ninja->AddAnim(anim);
	anim.Load("Resources/AnimationInfo/VG_whiteninja.dat", 2);
	ninja->AddAnim(anim);

	ObjectManager::GetInstance()->Add(ninja);

	return ninja;
}
CTurtle* Factory::CreateTurtle(char* name)
{
	CTurtle* turtle = new CTurtle();
	turtle->SetName(name);
	ObjectManager::GetInstance()->Add(turtle);
	return turtle;
}
CBattleItem* Factory::CreateBattleItem(int type, POINT mapPoint)
{
	CBattleItem* item = new CBattleItem();

	switch(type)
	{
	case BLACK_EGGS:
		item->SetName(blackeggs.GetName());
		item->SetRange(blackeggs.GetRange());
		item->SetDamage(blackeggs.GetDamage());
		break;
	case GRENADO:
		item->SetName(grenado.GetName());
		item->SetRange(grenado.GetRange());
		item->SetDamage(grenado.GetDamage());
		break;
	case PIZZA:
		item->SetName(pizza.GetName());
		item->SetHeal(pizza.GetDamage());
		break;
	}
	CBattleMap* pBM =  CBattleMap::GetInstance();
	item->SetCurrTile(mapPoint, pBM->GetOffsetX(),pBM->GetOffsetY(),pBM->GetTileWidth(), pBM->GetTileHeight(),pBM->GetNumCols(),false);

	ObjectManager::GetInstance()->Add(item);
	return item;
}
CBase* Factory::CreateWeapon(int type, POINT mapPoint)
{
	CBase* weapon = new CBase();
	switch(type)
	{
	case BOKKEN:
		weapon = weapons[BOKKEN];
		break;
	case TACHI:
		weapon = weapons[TACHI];
		break;
	case KATANA:
		weapon = weapons[KATANA];
		break;
	case NINJATO:
		weapon = weapons[NINJATO];
		break;
	case WOODNUN:
		weapon = weapons[WOODNUN];
		break;
	case GLASSNUN:
		weapon = weapons[GLASSNUN];
		break;
	case STEELNUN:
		weapon = weapons[STEELNUN];
		break;
	case OAKSTAFF:
		weapon = weapons[OAKSTAFF];
		break;
	case BAMBOOSTAFF:
		weapon = weapons[BAMBOOSTAFF];
		break;
	case STEELSTAFF:
		weapon = weapons[STEELSTAFF];
		break;
	case RUSTYSAI:
		weapon = weapons[RUSTYSAI];
		break;
	case POLISHEDSAI:
		weapon = weapons[POLISHEDSAI];
		break;
	}

	return weapon;
}