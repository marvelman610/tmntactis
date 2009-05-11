////////////////////////////////////////////////////////
//   File Name	:	"CBase.cpp"
//
//   Author		:	Matthew Di Matteo (MD)
//
//   Purpose	:	To encapsulate all shared data and functionality 
//					for our game objects
////////////////////////////////////////////////////////
#include "CBase.h"
#include "SGD Wrappers/CSGD_TextureManager.h"

CBase::CBase()
{
	SetImageID(-1);
	SetPosX(0);
	SetPosY(0);
	SetVelX(0);
	SetVelY(0);
	SetWidth(0);
	SetHeight(0);
	m_uiRefCount = 1;

	//m_nType = OBJECT_BASE;
}
CBase::~CBase()
{
}

void CBase::Update(float fElapsedTime)
{
	SetPosX(GetPosX() + GetVelX()*fElapsedTime);
	SetPosY(GetPosY() + GetVelY()*fElapsedTime);

}
void CBase::Render()
{
	CSGD_TextureManager::GetInstance()->Draw(GetImageID(), (int)GetPosX(), (int)GetPosY());
}
RECT CBase::GetRect()
{
	RECT rCollisionRect;
	rCollisionRect.left = long(GetPosX());
	rCollisionRect.right = long(GetWidth() + GetPosX());
	rCollisionRect.top = long(GetPosY());
	rCollisionRect.bottom = long(GetHeight() + GetPosY());

	return rCollisionRect;
}
bool CBase::CheckCollision(CBase * pBase)
{	
	RECT rCollision;
	RECT rMine = GetRect();
	RECT rHis = pBase->GetRect();
	if (IntersectRect(&rCollision, &rMine, &rHis)) 		
		return true;

	return false;
}
bool CBase::CheckCollisionA(CBase * pBase)
{	
	RECT rCollision;
	RECT rMine = GetRectt();
	RECT rHis = pBase->GetRectt();
	if (IntersectRect(&rCollision, &rMine, &rHis)) 		
		return true;

	return false;
}

RECT CBase::GetRectt()
{
	RECT rCollisionRect;
	rCollisionRect.left = long(GetPosX());
	rCollisionRect.right = long(GetWidth() + GetPosX());
	rCollisionRect.top = long(GetPosY()+GetHeight()-15);
	rCollisionRect.bottom = long(GetHeight() + GetPosY());

	return rCollisionRect;
}