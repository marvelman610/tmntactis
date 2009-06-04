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

void CBase::SetCurrTile(POINT mapPt, int xOffset, int yOffset, int tileWidth, int tileHeight, int numCols)
{
	m_ptMapCoord = mapPt;
	m_nCurrTileID = mapPt.y * numCols + mapPt.x;
	SetPosX((float)(mapPt.x - mapPt.y) * (tileWidth >> 1) + xOffset + (tileWidth>>1));
	SetPosY((float)(mapPt.x + mapPt.y) * (tileHeight >> 1) + yOffset + (tileHeight>>1));
}