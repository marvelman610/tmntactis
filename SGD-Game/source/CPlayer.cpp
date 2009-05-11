////////////////////////////////////////////////////////
//   File Name	:	"CPlayer.cpp"
//
//   Author		:	Matthew Di Matteo (MD)
//
//   Purpose	:	To provide a game object to be used as the player
//					being controlled by user during the game
////////////////////////////////////////////////////////
#include "CPlayer.h"
#include "SGD Wrappers/CSGD_TextureManager.h"
#include "SGD Wrappers/CSGD_WaveManager.h"
#include "SGD Wrappers/CSGD_DirectInput.h"
#include "CSGD_MessageSystem.h"
#include "CSGD_EventSystem.h"
#include"CGamePlayState.h"
#include "SGD_Math.h"

CPlayer::CPlayer()
{
	m_fShotTime = 0.0f;

	m_nSoundID = CSGD_WaveManager::GetInstance()->LoadWave("resource/sound/MaD_GunShot.wav");
	m_nDeathID = CSGD_WaveManager::GetInstance()->LoadWave("resource/sound/MaD_Death.wav");
	m_nGODID = CSGD_WaveManager::GetInstance()->LoadWave("resource/sound/MaD_Powerup.wav");
	m_nGUNID = CSGD_WaveManager::GetInstance()->LoadWave("resource/sound/MaD_guncheat.wav");


	playerhpbar = CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/MaD_playerhpbar.bmp");
	playerhp = CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/MaD_playerhp.bmp");
	deadID = CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/MaD_deadplayer.png");
 
	m_fRotation = (270.0f*3.14159f)/180;

	SetWidth(75);
	SetHeight(100);

	FaceLeft = false;

	curAnimation = 0;

	timer = 0;

	render = true;

	CSingleAnimation walking;
	walking.Load("resource/graphics/MaD_PunisherWalking.png",75,100,12,0.06f,12);
	animations.push_back(walking);

	CSingleAnimation shooting;
	shooting.Load("resource/graphics/MaD_PunisherShooting.png",140,100,6, 0.025f,12);
	animations.push_back(shooting);

	CSingleAnimation spin;
	spin.Load("resource/graphics/MaD_victory2.png",105,70,6, 0.25f,6);
	animations.push_back(spin);

	CSingleAnimation victory;
	victory.Load("resource/graphics/MaD_victory.png",56,106,4, 0.25f,4);
	animations.push_back(victory);

	CSGD_EventSystem::GetInstance()->RegisterClient("Fire", this);
	CSGD_EventSystem::GetInstance()->RegisterClient("Collision", this);
	CSGD_EventSystem::GetInstance()->RegisterClient("GoodCollision", this);
	CSGD_EventSystem::GetInstance()->RegisterClient("Up", this);
	CSGD_EventSystem::GetInstance()->RegisterClient("Down", this);
	CSGD_EventSystem::GetInstance()->RegisterClient("Left", this);
	CSGD_EventSystem::GetInstance()->RegisterClient("Right", this);
	CSGD_EventSystem::GetInstance()->RegisterClient("TurnAround", this);
	CSGD_EventSystem::GetInstance()->RegisterClient("Pos", this);
	CSGD_EventSystem::GetInstance()->RegisterClient("MovePowerUp", this);
	CSGD_EventSystem::GetInstance()->RegisterClient("BadCollision", this);
	CSGD_EventSystem::GetInstance()->RegisterClient("Points", this);

	pickup = CSGD_WaveManager::GetInstance()->LoadWave("resource/sound/MaD_fireball.wav");


	hp = 1000;

	scale = 1;
	guncheat = 0;
	gunbool = false;

	godcheat = 0;
	godbool = false;

	died = 0;
	m_nType = OBJECT_PLAYER;
}
CPlayer::~CPlayer()
{


	for(unsigned int i = 0; i < animations.size(); i++)
		animations[i].Unload(animations[i].GetImageID());

	CSGD_WaveManager::GetInstance()->UnloadWave(pickup);



	CSGD_EventSystem::GetInstance()->UnregisterClient("Fire", this);
	CSGD_EventSystem::GetInstance()->UnregisterClient("Collision", this);
	CSGD_EventSystem::GetInstance()->UnregisterClient("GoodCollision", this);
	CSGD_EventSystem::GetInstance()->UnregisterClient("Right", this);
	CSGD_EventSystem::GetInstance()->UnregisterClient("Left", this);
	CSGD_EventSystem::GetInstance()->UnregisterClient("Down", this);
	CSGD_EventSystem::GetInstance()->UnregisterClient("Up", this);
	CSGD_EventSystem::GetInstance()->UnregisterClient("TurnAround", this);
	CSGD_EventSystem::GetInstance()->UnregisterClient("Pos", this);
	CSGD_EventSystem::GetInstance()->UnregisterClient("MovePowerUp", this);
	CSGD_EventSystem::GetInstance()->UnregisterClient("BadCollision", this);
	CSGD_EventSystem::GetInstance()->UnregisterClient("Points", this);


}
void CPlayer::Update(float fElapsedTime)
{
	cheat();
	static int counter = 0;

	animations[curAnimation].Update(fElapsedTime);

	if(CGamePlayState::GetInstance()->GetBoss() == 10)
		CSGD_MessageSystem::GetInstance()->SendMsg(new CDestroyPlayerMessage(this));

	if(	CGamePlayState::GetInstance()->GetPlayercount() == 1)
	{


		if(CGamePlayState::GetInstance()->GetBoss() == 4)
		{
			timer +=fElapsedTime;
			hp = 1000;
			SetPosX(300);
			SetPosY(400);
			animations[curAnimation].Stop();
			curAnimation = 2;
			animations[curAnimation].Resume();

			if(timer > 5)
				CGamePlayState::GetInstance()->SetBoss(5);

		}
		else if(CGamePlayState::GetInstance()->GetBoss() == 5)
		{
			timer +=fElapsedTime;
			hp = 1000;

			animations[curAnimation].Stop();
			curAnimation = 3;
			animations[curAnimation].Resume();

		}
		else
		{

			if(godbool)
				hp=1000;

			if(hp < 1000)
				hp+=10.0f*fElapsedTime;
			if(hp >=1000)
				hp = 1000;

			CSGD_EventSystem::GetInstance()->SendEvent("TurnAround",this);

			CSGD_EventSystem::GetInstance()->SendEvent("Pos",this);

			//Shot timer and delay
			m_fShotTime += fElapsedTime;
			if(CSGD_DirectInput::GetInstance()->KeyDown(DIK_SPACE))
			{
				if(FaceLeft && counter == 0)
				{
					SetPosX(GetPosX()-50);
					counter++;
				}
				curAnimation = 1;
				animations[curAnimation].Resume();

				float SHOTDELAY;
				if(gunbool == false)
					SHOTDELAY = 0.3f;
				else if(gunbool == true)
					SHOTDELAY = 0.1f;


				if(m_fShotTime >= SHOTDELAY)
				{
					CSGD_WaveManager::GetInstance()->Play(m_nSoundID);
					CSGD_WaveManager::GetInstance()->SetVolume(m_nSoundID, CGame::GetInstance()->GetSFXVolume());
					CSGD_WaveManager::GetInstance()->SetPan(m_nSoundID, (CGame::GetInstance()->GetPanning()-50)*2);

					CSGD_EventSystem::GetInstance()->SendEvent("Fire",this);

					m_fShotTime = 0.0f;
				}
			}
			else 
			{
				if(counter >0)
				{
					SetPosX(GetPosX()+50);
					counter = 0;
				}

				curAnimation = 0;

				CBase::Update(fElapsedTime);
				animations[curAnimation].Resume();


				if(CSGD_DirectInput::GetInstance()->KeyDown(DIK_UP))
					CSGD_EventSystem::GetInstance()->SendEvent("Up",this);
				else if(CSGD_DirectInput::GetInstance()->KeyDown(DIK_DOWN))
					CSGD_EventSystem::GetInstance()->SendEvent("Down",this);
				else if(CSGD_DirectInput::GetInstance()->KeyDown(DIK_LEFT))
					CSGD_EventSystem::GetInstance()->SendEvent("Left",this);
				else if(CSGD_DirectInput::GetInstance()->KeyDown(DIK_RIGHT))
					CSGD_EventSystem::GetInstance()->SendEvent("Right",this);
				else
					animations[curAnimation].Stop();

				//Collision with walls
				if((GetPosX()*scale) <= 0)
				{
					SetPosX(0);
					SetVelX(0);
				}

				if(GetPosX() + GetWidth() >= 800)
				{
					SetPosX(float(800-GetWidth()));
					SetVelX(0);
				}

				if(GetPosY() + (GetHeight()*scale) <= 400)
				{
					SetPosY( float(400-GetHeight()) );
					SetVelY(0);
				}
				if(GetPosY() + (GetHeight()*scale) >= 580)
				{
					SetPosY(float(580 - GetHeight()) );
					SetVelY(0);
				}
			}
		}
	}
	died += fElapsedTime;

	x = (1000 - hp)/5;


}
void CPlayer::Render()
{
	RECT r;
	r.left = 43;
	r.right = r.left+202-x;
	r.top = 17;
	r.bottom = 44;
	if(CGamePlayState::GetInstance()->GetPlayercount() == 0)
	{
		CSGD_TextureManager::GetInstance()->Draw(deadID, GetPosX(), GetPosY()+50);
		hp=1000;
	}
	else
	{

			render = !render;
		if(died  < 5 && render  && CGamePlayState::GetInstance()->GetPaused() == false)
		{
			hp = 1000;
			animations[curAnimation].Render((int)GetPosX(),(int)GetPosY(),scale,GetFacing());
			//died =0;
		}
		else if( died > 5 || CGamePlayState::GetInstance()->GetPaused() == true)
		{
			animations[curAnimation].Render((int)GetPosX(),(int)GetPosY(),scale,GetFacing());
			//died += fElapsedTime;
			
		}



	}	

	CSGD_TextureManager::GetInstance()->Draw(playerhpbar,0,0);
	CSGD_TextureManager::GetInstance()->Draw(playerhp,43,17,1,1,&r);
}
void CPlayer::HandleEvent(CEvent* pEvent)
{
	if(pEvent->GetEventID() == "Fire")
	{
 		if(pEvent->GetParam() == this)
		{
			CSGD_MessageSystem::GetInstance()->SendMsg(new CCreateBulletMessage(this));
			if(CGamePlayState::GetInstance()->GetBoss() < 1)
				CGamePlayState::GetInstance()->SetbulletsFired(CGamePlayState::GetInstance()->GetbulletsFired()+1);
		}
	}
	else if(pEvent->GetEventID() == "Collision")
	{
		if(pEvent->GetParam() == this)
		{
			this->hp--;
			if(hp <= 0)
			{
				CSGD_WaveManager::GetInstance()->Play(m_nDeathID);
				CSGD_WaveManager::GetInstance()->SetVolume(m_nDeathID, CGame::GetInstance()->GetSFXVolume());
				CSGD_WaveManager::GetInstance()->SetPan(m_nDeathID, (CGame::GetInstance()->GetPanning()-50)*2);

				//CSGD_MessageSystem::GetInstance()->SendMsg(new CDestroyPlayerMessage(this));
				died = 0;
				CGamePlayState::GetInstance()->SetPlayerCount(0);
			}
		}
	}
	else if(pEvent->GetEventID() == "GoodCollision")
	{
		if(pEvent->GetParam() == this)
		{
			CSGD_WaveManager::GetInstance()->Play(pickup);
			CSGD_WaveManager::GetInstance()->SetVolume(pickup, CGame::GetInstance()->GetSFXVolume());
			CSGD_WaveManager::GetInstance()->SetPan(pickup, (CGame::GetInstance()->GetPanning()-50)*2);
			hp+=100;
			if(hp>=1000)
				hp = 1000;
		}
	}
	else if(pEvent->GetEventID() == "Points")
	{
		if(pEvent->GetParam() == this)
		{
			CSGD_WaveManager::GetInstance()->Play(pickup);
			CSGD_WaveManager::GetInstance()->SetVolume(pickup, CGame::GetInstance()->GetSFXVolume());
			CSGD_WaveManager::GetInstance()->SetPan(pickup, (CGame::GetInstance()->GetPanning()-50)*2);
			CGamePlayState::GetInstance()->SetPoints(CGamePlayState::GetInstance()->GetPoints()+25);
		}
	}
	else if(pEvent->GetEventID() == "Up")
	{
 		if(pEvent->GetParam() == this)
		{
			SetPosY( (GetPosY()-0.35f) );
		}
	}
	else if(pEvent->GetEventID() == "Down")
	{
 		if(pEvent->GetParam() == this)
		{
			SetPosY( (GetPosY()+0.35f) );
		}
	}
	else if(pEvent->GetEventID() == "Left")
	{
 		if(pEvent->GetParam() == this)
		{
			FaceLeft = true;
			m_fRotation = (90.0f*3.14159f) / 180;
			SetPosX( (GetPosX()-0.5f) );
		}

	}
	else if(pEvent->GetEventID() == "Right")
	{
 		if(pEvent->GetParam() == this)
		{
			FaceLeft = false;
			m_fRotation = (270.0f*3.14159f)/180;
			if(GetPosX() <600)
				SetPosX( (GetPosX()+0.5f) );

			if(GetPosX() > 500)
			{
				if(CGamePlayState::GetInstance()->GetPos() > -2670)
				{
					CGamePlayState::GetInstance()->SetPos(CGamePlayState::GetInstance()->GetPos()-0.25f);
					CSGD_EventSystem::GetInstance()->SendEvent("Fast",this);
					CSGD_EventSystem::GetInstance()->SendEvent("MovePowerUp");
				}
				else
				{
					CSGD_EventSystem::GetInstance()->SendEvent("Slow",this);

					SetPosX( (GetPosX()+0.25f) );
					
					if(CGamePlayState::GetInstance()->GetBoss() == 0)
					{
						CGamePlayState::GetInstance()->SetBoss( 1);
					}

				}
				

			}
			
		}
	}
	else if(pEvent->GetEventID() == "GoToPlayer")
			CSGD_EventSystem::GetInstance()->SendEvent("Pos",this);

	else if(pEvent->GetEventID() == "BadCollision")
	{
		hp-=50;
		if(hp <= 0)
		{
			CSGD_WaveManager::GetInstance()->Play(m_nDeathID);
			CSGD_MessageSystem::GetInstance()->SendMsg(new CDestroyPlayerMessage(this));
			CGamePlayState::GetInstance()->SetPlayerCount(0);
		}
	}
}

void CPlayer::cheat()
{
	switch(guncheat)
	{
	case 0:
		if (CSGD_DirectInput::GetInstance()->KeyPressed(DIK_G))
			guncheat++;
		else if (CSGD_DirectInput::GetInstance()->CheckBufferedKeysEx() != 0)
			guncheat = 0;
		break;
	case 1:
		if (CSGD_DirectInput::GetInstance()->KeyPressed(DIK_U))
			guncheat++;
		else if (CSGD_DirectInput::GetInstance()->CheckBufferedKeysEx() != 0)
			guncheat = 0;
		break;
	case 2:
		if (CSGD_DirectInput::GetInstance()->KeyPressed(DIK_N))
			guncheat++;
		else if (CSGD_DirectInput::GetInstance()->CheckBufferedKeysEx() != 0)
			guncheat = 0;
		break;
	}
	if( guncheat == 3)
	{
		CSGD_WaveManager::GetInstance()->Play(m_nGUNID);
		CSGD_WaveManager::GetInstance()->SetVolume(m_nGUNID, CGame::GetInstance()->GetSFXVolume());
		CSGD_WaveManager::GetInstance()->SetPan(m_nGUNID, (CGame::GetInstance()->GetPanning()-50)*2);
		gunbool = !gunbool;
		guncheat = 0;
	}
	
	switch(godcheat)
	{
	case 0:
		if (CSGD_DirectInput::GetInstance()->KeyPressed(DIK_G))
			godcheat++;
		else if (CSGD_DirectInput::GetInstance()->CheckBufferedKeysEx() != 0)
			godcheat = 0;
		break;
	case 1:
		if (CSGD_DirectInput::GetInstance()->KeyPressed(DIK_O))
			godcheat++;
		else if (CSGD_DirectInput::GetInstance()->CheckBufferedKeysEx() != 0)
			godcheat = 0;
		break;
	case 2:
		if (CSGD_DirectInput::GetInstance()->KeyPressed(DIK_D))
			godcheat++;
		else if (CSGD_DirectInput::GetInstance()->CheckBufferedKeysEx() != 0)
			godcheat = 0;
		break;
	}
	if( godcheat == 3)
	{
		CSGD_WaveManager::GetInstance()->Play(m_nGODID);
		CSGD_WaveManager::GetInstance()->SetVolume(m_nGODID, CGame::GetInstance()->GetSFXVolume());
		CSGD_WaveManager::GetInstance()->SetPan(m_nGODID, (CGame::GetInstance()->GetPanning()-50)*2);
		godbool = !godbool;
		godcheat = 0;
	}
}