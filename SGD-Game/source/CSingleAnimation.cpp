////////////////////////////////////////////////////////
//   File Name	:	"CSingleAnimation.cpp"
//
//   Author		:	Matthew Di Matteo (MD)
//
//   Purpose	:	This file provides a way to set and control
//					the animations of game objects 
////////////////////////////////////////////////////////
#include"CSingleAnimation.h"
#include "SGD Wrappers/CSGD_TextureManager.h"

CSingleAnimation::CSingleAnimation()
{	
	bIsPlaying = false;
	bIsLooping = true;
	curFrame = 0;
	reset();
	
}
CSingleAnimation::~CSingleAnimation()
{

}
void CSingleAnimation::Unload(int id)
{
	CSGD_TextureManager::GetInstance()->UnloadTexture(id);	
}

RECT CSingleAnimation::cellalgorithm(int id)
{
	RECT rcell;
	rcell.left = (id % numCols) * frameWidth;
	rcell.top = (id / numCols) * frameHeight;
	rcell.right = rcell.left + frameWidth;
	rcell.bottom = rcell.top + frameHeight;

	return rcell;
}

void CSingleAnimation::reset()
{
	speed = 1;
	timeWaited = 0;
}

void CSingleAnimation::Load(const char* file, int width, int height, int numcols, float time, int totalframe)
{
	SetID(CSGD_TextureManager::GetInstance()->LoadTexture(file));
	SetFrameWidth(width);
	SetFrameHeight(height);
	SetNumCols(numcols);
	timePerFrame = time;
	totalFrames = totalframe;
}

void CSingleAnimation::Update(float elapsedtime)
{
	if(!bIsPlaying)
		return;
	//increment time waited
	timeWaited += elapsedtime * speed;

	//check to see if i waited long enough to change frames
	if(timeWaited > timePerFrame)
	{
		timeWaited -= timePerFrame;//0.0f
		curFrame++;
		if(curFrame>= totalFrames)
		{
			if(bIsLooping)
				curFrame = 0;
			else
			{
				Stop();
				curFrame =totalFrames-1;//freeze on last frame
			}	
		}
	}	
}
void CSingleAnimation::Render(int posx, int posy, float scale, bool bFacingLeft,DWORD dwColor)
{
	RECT frame = cellalgorithm(curFrame);
	float fScaleX = scale;
	if(bFacingLeft) // flip
	{
		//manipulate sclae and pos
		fScaleX *=-1;
		posx += int(frameWidth*scale);

	}
	//draw stuff to screen
	//ptm->draw(imageid,posx,posy,fscaleX,scale,&frame, 0 0 0, color) //pos - anchor
	CSGD_TextureManager::GetInstance()->Draw(imageID,posx,posy,fScaleX,scale,&frame, 0, 0, 0);

}


void CSingleAnimation::Play()
{ 
	reset(); 
	bIsPlaying = true;
}
void CSingleAnimation::Stop() 
{ 
	bIsPlaying = false;
}
void CSingleAnimation::Resume() 
{
	bIsPlaying = true;
}
