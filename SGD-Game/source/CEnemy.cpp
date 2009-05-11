////////////////////////////////////////////////////////
//   File Name	:	"CEnemy.cpp"
//
//   Author		:	Matthew Di Matteo (MD)
//
//   Purpose	:	To provide a game object to be used as an emeny
//					against the user during the game
////////////////////////////////////////////////////////
#include "CEnemy.h"
#include "CPlayer.h"
#include "SGD Wrappers/CSGD_TextureManager.h"
#include "CSGD_EventSystem.h"
#include "CSGD_MessageSystem.h"
#include "CHealth.h"
#include "CGame.h"

CEnemy::CEnemy()
{
	m_nType = OBJECT_Enemy;

	curAnimation = 0;

	bloodID = CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/MaD_blood.png");

	CSingleAnimation walking;
	walking.Load("resource/graphics/MaD_ninjawalking.png",42,64,3,0.5f,3);
	//walking.Load("resource/graphics/MaD_HawkeyeWalking.png",45,64,3,0.5f,3);
	animations.push_back(walking);

	CSGD_EventSystem::GetInstance()->RegisterClient("Collision", this);
	CSGD_EventSystem::GetInstance()->RegisterClient("Fast", this);
	CSGD_EventSystem::GetInstance()->RegisterClient("Slow", this);
	CSGD_EventSystem::GetInstance()->RegisterClient("TurnAround", this);
	CSGD_EventSystem::GetInstance()->RegisterClient("GoToPlayer", this);
	CSGD_EventSystem::GetInstance()->RegisterClient("Pos", this);

	hp = 24;

	timer = 0;
	bloodbool = false;

	flip = true;
}
CEnemy::~CEnemy()
{
	for(unsigned int i = 0; i < animations.size(); i++)
		animations[i].Unload(animations[i].GetImageID());

	CSGD_TextureManager::GetInstance()->UnloadTexture(bloodID);

	CSGD_EventSystem::GetInstance()->UnregisterClient("Collision", this);
	CSGD_EventSystem::GetInstance()->UnregisterClient("Fast", this);
	CSGD_EventSystem::GetInstance()->UnregisterClient("Slow", this);
	CSGD_EventSystem::GetInstance()->UnregisterClient("TurnAround", this);
	CSGD_EventSystem::GetInstance()->UnregisterClient("GoToPlayer", this);
	CSGD_EventSystem::GetInstance()->UnregisterClient("Pos", this);
}
void CEnemy::Update(float fElapsedTime)
{
	CBase::Update(fElapsedTime);
	CSGD_EventSystem::GetInstance()->SendEvent("GoToPlayer");

	timer+=fElapsedTime;

	if(GetPosX() < 0 || GetPosX() > 805)
	{
		CSGD_MessageSystem::GetInstance()->SendMsg(new CDestroyEnemyMessage(this));
		CGamePlayState::GetInstance()->SetNumEnemies(CGamePlayState::GetInstance()->GetNumEnemies()-1);
	}

	animations[curAnimation].Update(fElapsedTime);
	animations[curAnimation].Resume();
}
void CEnemy::Render()
{
	animations[curAnimation].Render((int)GetPosX(),(int)GetPosY(),1.5f,flip);

	if(bloodbool)
	{
		if(flip == false)
			CSGD_TextureManager::GetInstance()->Draw(bloodID,GetPosX()+50,GetPosY()+15,-1);
		else
			CSGD_TextureManager::GetInstance()->Draw(bloodID,GetPosX()+15,GetPosY()+15);
		if(timer>0.5f)
		{
			bloodbool = false;
			timer = 0;
		}
	}
		
}
void CEnemy::HandleEvent(CEvent* pEvent)
{
	if(pEvent->GetEventID() == "Collision")
	{
        if(pEvent->GetParam() == this)
		{
			this->hp -= 3;
			bloodbool = true;
			if(hp <= 0)
			{
				int h =rand()%11;
				if(h ==5)
					CSGD_MessageSystem::GetInstance()->SendMsg(new CCreatePowerUp(this));
				else if(h == 6 || h==7 || h==8)
					CSGD_MessageSystem::GetInstance()->SendMsg(new CCreatePowerUp2(this));
										
				CSGD_MessageSystem::GetInstance()->SendMsg(new CDestroyEnemyMessage(this));
				CGamePlayState::GetInstance()->SetNumEnemies(CGamePlayState::GetInstance()->GetNumEnemies()-1);

				if(CGamePlayState::GetInstance()->GetBoss() <1)
					CGamePlayState::GetInstance()->SetenemiesKilled(CGamePlayState::GetInstance()->GetenemiesKilled()+1);
				
				CGamePlayState::GetInstance()->SetPoints(CGamePlayState::GetInstance()->GetPoints()+10);
				
			}
		}
	}

	else if(pEvent->GetEventID() == "TurnAround")
	{
		CPlayer* p = (CPlayer*)pEvent->GetParam();
		if(p->GetPosX()+10 < GetPosX())
		{
			if (flip == false)
				SetVelX(GetVelX()*-1);

			flip = true;
		}
		else if(p->GetPosX() > GetPosX())
		{
			if(flip == true)
				SetVelX(GetVelX()*-1);
			flip = false;
			
		}
	}

	else if(pEvent->GetEventID() == "Fast")
	{
			SetVelX(GetVelX()*1.001f);
		
	}
	else if(pEvent->GetEventID() == "Slow")
	{
			SetVelX(GetVelX()/1.001f);
		
	}
	else if(pEvent->GetEventID() == "Pos")
	{
		CPlayer* p = (CPlayer*)pEvent->GetParam();

		if(GetPosY() > p->GetPosY())
			SetPosY(GetPosY()-0.02f);
		else if(GetPosY() < p->GetPosY())
			SetPosY(GetPosY()+0.02f);
			
	}


}

