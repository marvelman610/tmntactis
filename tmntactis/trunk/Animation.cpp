////////////////////////////////////////////////////////
//   File Name	:	"CAnimation.cpp"
//
//   Author		:	Matthew Di Matteo (MD)
//
//   Purpose	:	This file provides a way to set and control
//					the animations of the game objects 
////////////////////////////////////////////////////////

#include "Animation.h"

CAnimation::CAnimation(void)
{
}

CAnimation::~CAnimation(void)
{
}
void CAnimation::Play()
{ 
	Reset(); 
	m_bIsPlaying = true;
}
void CAnimation::Stop() 
{ 
	m_bIsPlaying = false;
}
void CAnimation::Resume() 
{
	m_bIsPlaying = true;
}
void CAnimation::Reset()
{

}
void Load(const char* FileName)
{

}
void Update(float fElapsedtime)
{

}
void Render(int posx, int posy, float scale, bool bFacingleft, DWORD dwColor)
{

}