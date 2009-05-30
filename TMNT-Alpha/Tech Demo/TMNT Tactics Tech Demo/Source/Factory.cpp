////////////////////////////////////////////////////////
//   File Name	:	"Factory.cpp"
//
//   Author		:	Matthew Di Matteo (MD)
//
//   Purpose	:	Creates base objects
////////////////////////////////////////////////////////

#include"Factory.h"
#include"ObjectManager.h"

#include"Ninja.h"
#include"Boss.h"
#include"Turtle.h"
#include"BattleItem.h"

Factory::Factory(void)
{
}

Factory::~Factory(void)
{
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
	ObjectManager::GetInstance()->Add(ninja);
	return ninja;
}
CTurtle* Factory::CreateTurtle()
{
	CTurtle* turtle = new CTurtle();
	ObjectManager::GetInstance()->Add(turtle);
	return turtle;
}
CBattleItem* Factory::CreateBattleItem()
{
	CBattleItem* item = new CBattleItem();
	ObjectManager::GetInstance()->Add(item);
	return item;
}