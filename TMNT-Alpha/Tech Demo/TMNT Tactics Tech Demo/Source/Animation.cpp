////////////////////////////////////////////////////////
//   File Name	:	"CAnimation.cpp"
//
//   Author		:	Matthew Di Matteo (MD)
//
//   Purpose	:	This file provides a way to set and control
//					the animations of the game objects 
////////////////////////////////////////////////////////

#include "Animation.h"


#include<fstream>
#include <exception>
using namespace std;
#include "CSGD_TextureManager.h"


CAnimation::CAnimation(void)
{
	m_nTotalFrames = 0;
	m_nCurrFrame = 0;

	m_fTimeWaited = 0;

	m_bIsPlaying = false;
	m_bIsLooping = false;

	m_fSpeed = 1.5f;

	m_nImageID = -1;

}

CAnimation::~CAnimation(void)
{
	delete[] m_pFrames;
}
void CAnimation::Play()
{ 
	Reset(); 
	m_bIsPlaying = true;

	//////////////////////////////////////////////////////////////////////////
	// TEMP
	m_bIsLooping = false;
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
	m_nCurrFrame = 0;
}
void CAnimation::Load(const char* FileName)
{
	int nDuration;
	char bIsLooping[128] = "true ";
	char filebuff[128];

	ifstream ifs;
	ifs.exceptions(~ios_base::goodbit);
	try
	{
		ifs.open(FileName, ios_base::in| ios_base::binary);
		if (ifs.is_open())
		{
			unsigned char size;
			ifs.read(reinterpret_cast<char *>(&m_nTotalFrames),4);

			ifs.read(reinterpret_cast<char *>(&size),1);
			ZeroMemory(filebuff,128);
			ifs.read(filebuff,size);

			m_nAnimName = filebuff; //2

			ifs.read(reinterpret_cast<char *>(&size),1);
			ZeroMemory(filebuff,128);
			ifs.read(filebuff,size);
			
			char temp[128];
			sprintf_s(temp, _countof(temp),"resources/images/%s", filebuff);

			m_nImageID = CSGD_TextureManager::GetInstance()->LoadTexture(temp ,D3DCOLOR_XRGB(255,255,255));

			ifs.read(reinterpret_cast<char *>(&size),1);
			ZeroMemory(filebuff,128);
			ifs.read(filebuff,size);

			m_nTriggerName = filebuff; // flying

			ifs.read(reinterpret_cast<char *>(&size),1);
			ZeroMemory(filebuff,128);
			ifs.read(filebuff,size);

			m_nTriggerType = filebuff; //particle

			ifs.read(reinterpret_cast<char *>(&size),1);
			ZeroMemory(filebuff,128);
			ifs.read(filebuff,size);

			if(strcmp(filebuff,bIsLooping))
				m_bIsLooping = true;
			else
				m_bIsLooping = false;

			ifs.read(reinterpret_cast<char *>(&nDuration),4);  //0.35
			m_fDuration = nDuration /1000.0f;

			m_pFrames = new sFrame[m_nTotalFrames];

			for(int i = 0; i < m_nTotalFrames; i++)
			{
				ifs.read(reinterpret_cast<char *>(&(m_pFrames[i].nFrameX)),4);
				ifs.read(reinterpret_cast<char *>(&m_pFrames[i].nFrameY),4);
				ifs.read(reinterpret_cast<char *>(&m_pFrames[i].nFrameWidth),4);
				ifs.read(reinterpret_cast<char *>(&m_pFrames[i].nFrameHeight),4);
				ifs.read(reinterpret_cast<char *>(&m_pFrames[i].nAnchorX),4);
				ifs.read(reinterpret_cast<char *>(&m_pFrames[i].nAnchorY),4);
				m_pFrames[i].nAnchorX = m_pFrames[i].nAnchorX - m_pFrames[i].nFrameX;
				m_pFrames[i].nAnchorY = m_pFrames[i].nAnchorY - m_pFrames[i].nFrameY;
			}
		}
	}
	catch (ios_base::failure &) 
	{
		if (!ifs.eof()) 
			throw; 
	}
	ifs.close();
}
void CAnimation::Update(float fElapsedtime)
{
	if(!m_bIsPlaying)
		return;

	m_fTimeWaited += fElapsedtime * m_fSpeed;

	if(m_fTimeWaited > m_fDuration)
	{
		m_nCurrFrame++;
		//m_fTimeWaited = 0;
		m_fTimeWaited -= m_fDuration;
		
		if(m_nCurrFrame >= m_nTotalFrames)
		{
			if(m_bIsLooping)
				m_nCurrFrame = 0;
			else
			{
				Stop();
				m_nCurrFrame = m_nTotalFrames-1;
			}
		}

	}
}
void CAnimation::Render(int posx, int posy, float posZ, float scale, bool bFacingLeft, DWORD dwColor)
{
	if(m_nImageID !=-1)
	{
		RECT frame = {m_pFrames[m_nCurrFrame].nFrameX, m_pFrames[m_nCurrFrame].nFrameY, 
			m_pFrames[m_nCurrFrame].nFrameX + m_pFrames[m_nCurrFrame].nFrameWidth, m_pFrames[m_nCurrFrame].nFrameY+m_pFrames[m_nCurrFrame].nFrameHeight};
		float fScaleX = scale;
		if(bFacingLeft) // flip
		{
			//manipulate scale and pos
			fScaleX *=-1;
			posx += int(m_pFrames[m_nCurrFrame].nFrameWidth*scale);

		}
		//draw stuff to screen
				/*CSGD_TextureManager::GetInstance()->Draw(m_nImageID,posx,posy,fScaleX,scale,&frame, 0, 0, 0);*/
		CSGD_TextureManager::GetInstance()->DrawWithZSort(m_nImageID,posx-m_pFrames[m_nCurrFrame].nAnchorX,
			posy-m_pFrames[m_nCurrFrame].nAnchorY, 
			posZ,
			fScaleX,
			scale,
			&frame,
			0, 0, 0);
	}
}