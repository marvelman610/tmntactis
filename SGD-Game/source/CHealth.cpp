////////////////////////////////////////////////////////
//   File Name	:	"CHealth.cpp"
//
//   Author		:	Matthew Di Matteo (MD)
//
//   Purpose	:	To provide a game object to be used as an health powerup
////////////////////////////////////////////////////////
#include"CHealth.h"
#include"SGD Wrappers/CSGD_TextureManager.h"
#include"SGD Wrappers/CSGD_WaveManager.h"
#include "CSGD_EventSystem.h"
#include "CSGD_MessageSystem.h"
#include"CGame.h"


CHealth::CHealth()
{
	SetImageID(CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/MaD_health.png"));

	SetPosX(400);
	SetPosY(500);

	CSGD_EventSystem::GetInstance()->RegisterClient("MovePowerUp", this);
	CSGD_EventSystem::GetInstance()->RegisterClient("Collision", this);

	m_nType = OBJECT_HEALTH;

}
CHealth::~CHealth()
{
	CSGD_EventSystem::GetInstance()->UnregisterClient("MovePowerUp", this);
	CSGD_EventSystem::GetInstance()->UnregisterClient("Collision", this);

}
void CHealth::Update(float fElapsedTime)
{
	if(GetPosX() < 0)
		CSGD_MessageSystem::GetInstance()->SendMsg(new CDestroyPowerUp(this));
}

void CHealth::Render()
{
	CSGD_TextureManager::GetInstance()->Draw(GetImageID(),GetPosX(), GetPosY());
}
void CHealth::HandleEvent(CEvent *pEvent)
{
	if(pEvent->GetEventID() == "MovePowerUp")
	{
		SetPosX(GetPosX()-0.25f);
	}
	if(pEvent->GetEventID() == "Collision")
	{
		if(pEvent->GetParam() == this)
			CSGD_MessageSystem::GetInstance()->SendMsg(new CDestroyPowerUp(this));	
	}
}

CPoints::CPoints()
{
	SetImageID(CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/MaD_Points.png"));

	SetPosX(400);
	SetPosY(500);



	CSGD_EventSystem::GetInstance()->RegisterClient("MovePowerUp", this);
	CSGD_EventSystem::GetInstance()->RegisterClient("Collision", this);

	m_nType = OBJECT_POINTS;

}
CPoints::~CPoints()
{
	CSGD_TextureManager::GetInstance()->UnloadTexture(GetImageID());

	CSGD_EventSystem::GetInstance()->UnregisterClient("MovePowerUp", this);
	CSGD_EventSystem::GetInstance()->UnregisterClient("Collision", this);

}
void CPoints::Update(float fElapsedTime)
{
	if(GetPosX() < 0)
		CSGD_MessageSystem::GetInstance()->SendMsg(new CDestroyPowerUp2(this));
}

void CPoints::Render()
{
	CSGD_TextureManager::GetInstance()->Draw(GetImageID(),GetPosX(), GetPosY());
}
void CPoints::HandleEvent(CEvent *pEvent)
{
	if(pEvent->GetEventID() == "MovePowerUp")
	{
		SetPosX(GetPosX()-0.25f);
	}
	if(pEvent->GetEventID() == "Collision")
	{
		if(pEvent->GetParam() == this)
			CSGD_MessageSystem::GetInstance()->SendMsg(new CDestroyPowerUp2(this));		
	}
}