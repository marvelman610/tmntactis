////////////////////////////////////////////////////////
//   File Name	:	"WorldMap.h"
//
//   Author		:	Matthew Di Matteo (MD)
//
//   Purpose	:	This file provides a world map select screen 
//					so the player can pick where he/she goes to battle. 
////////////////////////////////////////////////////////

#include "WorldMap.h"

CWorldMap* CWorldMap::GetInstance()
{
	static CWorldMap instance;
	return &instance;
}
CWorldMap::CWorldMap(void)
{
}

CWorldMap::~CWorldMap(void)
{
}
void CWorldMap::Update(float fElapsedTime)
{
}
void CWorldMap::Render()
{

}