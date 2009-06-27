////////////////////////////////////////////////////////
//   File Name	:	"BattleItem.cpp"
//
//   Author		:	Matthew Di Matteo (MD)
//
//   Purpose	:	Class derived from item that will act as
//					a special item to be used in battle 
////////////////////////////////////////////////////////
#include "BattleItem.h"
#include "BattleMap.h"
#include "Assets.h"
#include "CSGD_TextureManager.h"

CBattleItem::CBattleItem(void):CBase()
{
	m_nHeal = 0;
	m_nDamage = 0;
	m_nDamageRadius = 1;

	m_nType = OBJECT_BATTLEITEM;

	SetWidth(32);
	SetHeight(32);
}
CBattleItem::~CBattleItem(void)
{
}
void CBattleItem::Update(float fElapsedTime)
{
	CBattleMap* pBM =  CBattleMap::GetInstance();
	SetCurrTile(GetMapCoord(),pBM->GetOffsetX(), pBM->GetOffsetY(), pBM->GetTileWidth(), pBM->GetTileHeight(), pBM->GetNumCols(), false);
}
void CBattleItem::Render()
{
	CSGD_TextureManager::GetInstance()->DrawWithZSort(GetImageID(), (int)GetPosX(), (int)GetPosY(), GetPosZ());
}