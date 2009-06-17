////////////////////////////////////////////////////////
//   File Name	:	"Base.cpp"
//
//   Author		:	Matthew Di Matteo (MD)
//
//   Purpose	:	To encapsulate all shared data and functionality 
//					for our game objects
////////////////////////////////////////////////////////

#include "Base.h"
#include "CSGD_Direct3D.h"
#include "CSGD_TextureManager.h"

CBase::CBase(void)
{
	m_szName = NULL;
	m_nImageID = -1;

	m_nHealthMax = 100;
	m_nHealth = 0;

	m_nLevel = 1;
	m_nStrength = 0;
	m_nDefense = 0;
	m_nAccuracy = 0;
	m_nSpeed = 0;
	m_nRange = 0;

	m_fPosZ = 0.0f;	// draw in front of everything by default
	m_nCurrAnimation = 0;
	m_nSkillXP = m_nExperience = 0;
	m_dwColor = D3DCOLOR_XRGB(255,255,255);
}

void CBase::Update(float fElapsedTime)
{

}

void CBase::Render()
{
	if(GetType() == OBJECT_WEAPON)
		CSGD_TextureManager::GetInstance()->DrawWithZSort(GetImageID(), (int)GetPosX(), (int)GetPosY(), GetPosZ());


}

RECT CBase::GetRect()
{
	LONG left, top, right, bottom;

	left = (LONG)(m_fPosX);
	top = (LONG)(m_fPosY);
	right = (LONG)(left+m_vAnimations[m_nCurrAnimation].GetFrames()[m_nCurrAnimation].nFrameWidth);
	bottom = (LONG)(top+m_vAnimations[m_nCurrAnimation].GetFrames()[m_nCurrAnimation].nFrameHeight);

	RECT rect = {left,top,right,bottom};
	return rect;
}

//vector<CTile> CBase::Pathfind(void)
//{
//	//vector<CTile> tiles;
//	return tiles;
//}

void CBase::SetCurrTile(POINT mapPt, int xOffset, int yOffset, int tileWidth, int tileHeight, int numCols, bool anchor)
{
	m_ptMapCoord = mapPt;
	m_nCurrTileID = mapPt.y * numCols + mapPt.x;
	if(anchor)
	{
		SetPosX((float)(mapPt.x - mapPt.y) * (tileWidth >> 1) + xOffset + (tileWidth>>1));
		SetPosY((float)(mapPt.x + mapPt.y) * (tileHeight >> 1) + yOffset + (tileHeight>>1));
		POINT anchorPt;
		anchorPt.x = (LONG)(GetPosX() + m_vAnimations[m_nCurrAnimation].GetFrames()[m_nCurrAnimation].nAnchorX);
		anchorPt.y = (LONG)(GetPosY() + m_vAnimations[m_nCurrAnimation].GetFrames()[m_nCurrAnimation].nAnchorY);
		SetAnchor(anchorPt);
	}
	else
	{
		mapPt.x = (LONG)(m_ptMapCoord.x - m_ptMapCoord.y) * (tileWidth >> 1) + xOffset + (tileWidth>>1);
		mapPt.y = (LONG)(m_ptMapCoord.x + m_ptMapCoord.y) * (tileHeight >> 1) + yOffset + (tileHeight>>1);
		SetPosPt(mapPt);
	}
}

void CBase::Colorize(bool bColorize)
{
	if (bColorize)
		m_dwColor = D3DCOLOR_XRGB(255, 50, 50);
	else
		m_dwColor = D3DCOLOR_XRGB(255,255,255);
}

void CBase::SetCurrAnim(int animID)
{
	m_vAnimations[m_nCurrAnimation].Stop();
	m_nCurrAnimation = animID;
	//if (animID == 0)
		m_vAnimations[m_nCurrAnimation].Play();

}

void CBase::SetWeapon(char* name, int attack, int defense, int imageID, int numType)
{
	SetName(name);
	SetStrength(attack);
	SetDefense(defense);
	SetImageID(imageID);
	SetNumType(numType);
}
