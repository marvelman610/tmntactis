////////////////////////////////////////////////////////
//   File Name	:	"BattleItem.cpp"
//
//   Author		:	Matthew Di Matteo (MD)
//
//   Purpose	:	Class derived from item that will act as
//					a special item to be used in battle 
////////////////////////////////////////////////////////
#include "BattleItem.h"

CBattleItem::CBattleItem(void)
{
	m_nHeal = 0;
	m_nDamage = 0;
	m_nDamageRadius = 1;
}
CBattleItem::~CBattleItem(void)
{
}
void CBattleItem::Update(float fElapsedTime)
{

}
void CBattleItem::Render()
{

}