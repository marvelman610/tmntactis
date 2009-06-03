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

	left = (LONG)(m_fPosX-m_vAnimations[0].GetFrames()[0].nAnchorX);
	top = (LONG)(m_fPosY-m_vAnimations[0].GetFrames()[0].nAnchorY);
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