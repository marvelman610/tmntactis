////////////////////////////////////////////////////////
//   File Name	:	"Base.cpp"
//
//   Author		:	Matthew Di Matteo (MD)
//
//   Purpose	:	To encapsulate all shared data and functionality 
//					for our game objects
////////////////////////////////////////////////////////

#include "Base.h"

CBase::CBase(void)
{
	m_szName = NULL;
	m_fPosZ = 0.0f;	// draw in front of everything by default
	m_nCurrAnimation = 0;
}

void CBase::Update(float fElapsedTime)
{

}
void CBase::Render()
{

}

RECT CBase::GetRect()
{
	LONG left, top, right, bottom;

	left = (LONG)(m_fPosX);
	top = (LONG)(m_fPosY);
	right = (LONG)(left+m_vAnimations[0].GetFrames()[0].nFrameWidth);
	bottom = (LONG)(top+m_vAnimations[0].GetFrames()[0].nFrameHeight);

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
		anchorPt.x = (LONG)(GetPosX() + m_vAnimations[0].GetFrames()[0].nAnchorX);
		anchorPt.y = (LONG)(GetPosY() + m_vAnimations[0].GetFrames()[0].nAnchorY);
		SetAnchor(anchorPt);
	}
	else
	{
		mapPt.x = (float)(m_ptMapCoord.x - m_ptMapCoord.y) * (tileWidth >> 1) + xOffset + (tileWidth>>1);
		mapPt.y = (float)(m_ptMapCoord.x + m_ptMapCoord.y) * (tileHeight >> 1) + yOffset + (tileHeight>>1);
		SetPosPt(mapPt);
	}
}
