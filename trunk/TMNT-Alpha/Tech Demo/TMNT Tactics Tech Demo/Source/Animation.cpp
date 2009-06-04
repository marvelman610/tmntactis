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
#include "Game.h"			// in case of error, throw back to main menu
#include "MainMenuState.h"	// same as above


CAnimation::CAnimation(void)
{
	m_nTotalFrames = 0;
	m_nCurrFrame = 0;

	m_fTimeWaited = 0;

	m_bIsPlaying = false;
	m_bIsLooping = false;

	m_fSpeed = 0.25f;

	m_nImageID = -1;
	m_pFrames = NULL;
}

CAnimation::~CAnimation(void)
{
	if (m_pFrames)
	{
		//delete[] m_pFrames;
	}	
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
void CAnimation::Load(const char* FileName, int numFrame)
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
			int numSize;

			ifs.read(reinterpret_cast<char *>(&numSize),4);

			for(int i = 1; i <= numFrame && i <= numSize;i++)
			{

				ifs.read(reinterpret_cast<char *>(&m_nTotalFrames),4);

				ifs.read(reinterpret_cast<char *>(&size),1);
				ZeroMemory(filebuff,128);
				ifs.read(filebuff,size);

				m_nAnimName = filebuff; //2

				ifs.read(reinterpret_cast<char *>(&size),1);
				ZeroMemory(filebuff,128);
				ifs.read(filebuff,size);

				char temp[128];
				sprintf_s(temp, _countof(temp),"Resources/Images/%s", filebuff);

				CSGD_TextureManager* tm = CSGD_TextureManager::GetInstance();
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

				for(int j = 0; j < m_nTotalFrames; j++)
				{
					ifs.read(reinterpret_cast<char *>(&(m_pFrames[j].nFrameX)),4);
					ifs.read(reinterpret_cast<char *>(&m_pFrames[j].nFrameY),4);
					ifs.read(reinterpret_cast<char *>(&m_pFrames[j].nFrameWidth),4);
					ifs.read(reinterpret_cast<char *>(&m_pFrames[j].nFrameHeight),4);
					ifs.read(reinterpret_cast<char *>(&m_pFrames[j].nAnchorX),4);
					ifs.read(reinterpret_cast<char *>(&m_pFrames[j].nAnchorY),4);
					m_pFrames[j].nAnchorX = m_pFrames[j].nAnchorX - m_pFrames[j].nFrameX;
					m_pFrames[j].nAnchorY = m_pFrames[j].nAnchorY - m_pFrames[j].nFrameY;
				}
				if(i != numFrame)
				{
					delete[] m_pFrames;
					m_pFrames = NULL;
				}
			}
		}
		else
		{
			char szBuffer[128];
			sprintf_s(szBuffer, "Failed to open file: %s", FileName );
			MessageBox(0, szBuffer, "Error.", MB_OK);
			CGame::GetInstance()->ChangeState(CMainMenuState::GetInstance());
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
		CSGD_TextureManager::GetInstance()->DrawWithZSort(m_nImageID,posx,
			posy, 
			posZ,
			fScaleX,
			scale,
			&frame,
			0, 0,0);
		int test = 0;
	}
}
void CAnimation::Unload()
{
	delete[] m_pFrames;
}