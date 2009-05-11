////////////////////////////////////////////////////////
//   File Name	:	"CBoss.cpp"
//
//   Author		:	Matthew Di Matteo (MD)
//
//   Purpose	:	To provide a game object to be used as a Boss emeny
//					against the user during the game.
////////////////////////////////////////////////////////

#include"CBoss.h"
#include"SGD Wrappers/CSGD_TextureManager.h"
#include "CSGD_EventSystem.h"
#include "CSGD_MessageSystem.h"
#include "CPlayer.h"
#include"CGamePlayState.h"

CBoss::CBoss()
{
	m_nType = OBJECT_BOSS;

	curAnimation = 0;

	CSGD_EventSystem::GetInstance()->RegisterClient("Collision", this);
	CSGD_EventSystem::GetInstance()->RegisterClient("TurnAround", this);
	CSGD_EventSystem::GetInstance()->RegisterClient("GoToPlayer", this);
	CSGD_EventSystem::GetInstance()->RegisterClient("Pos", this);

	CSingleAnimation walking;
	walking.Load("resource/graphics/MaD_spidermanwalking.png",50,64,7,0.15f,7);
	animations.push_back(walking);

	CSingleAnimation punching;
	punching.Load("resource/graphics/MaD_spidermanpunching.png",68,64,2,0.5f,2);
	animations.push_back(punching);

	CSingleAnimation shooting;
	shooting.Load("resource/graphics/MaD_spidermanshooting.png",72,64,3,0.15f,3);
	animations.push_back(shooting);
   
	CSingleAnimation jumping;
	jumping.Load("resource/graphics/MaD_spidermanjumping.png",50,80,3,0.25f,3);
	animations.push_back(jumping);

	CSingleAnimation swinging;
	swinging.Load("resource/graphics/MaD_spidermanswing.png",155,224,5,0.25f,5);
	animations.push_back(swinging);
	
	hpID = CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/MaD_miniboss_hp.png");
	hpbarID= CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/MaD_miniboss_hpbar.png");

	web = CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/MaD_web.png");
	
	hp = 1000;

	x = 0;
	i = 0;

	timer=0;
	animations[curAnimation].Play();


}
CBoss::~CBoss()
{
	for(unsigned int i = 0; i < animations.size(); i++)
		animations[i].Unload(animations[i].GetImageID());

	CSGD_EventSystem::GetInstance()->UnregisterClient("Collision", this);
	CSGD_EventSystem::GetInstance()->UnregisterClient("TurnAround", this);
	CSGD_EventSystem::GetInstance()->UnregisterClient("GoToPlayer", this);
	CSGD_EventSystem::GetInstance()->UnregisterClient("Pos", this);

	

}

void CBoss::Update(float fElapsedTime)
{
	
	animations[curAnimation].Update(fElapsedTime);
	
	if( hp<650)
	{
		if(hp < 300)
		{
			if(i == 1)
			{
				animations[curAnimation].Stop();
      			curAnimation = 3;
				animations[curAnimation].Resume();
				i=2;
				SetPosX(577);
				SetVelX(-160);
				
			}
			if( curAnimation == 3 && animations[curAnimation].GetCurFrame() == 2)
			{
				if(i == 2)
				{
					SetPosY(400);
					SetHeight(300);

					i = 3;
				}
				else if(i == 3)
				{
					SetPosY(100);

					animations[curAnimation].Stop();
					curAnimation = 4;
					animations[curAnimation].Resume();	
				}
			}   
			if(GetPosX() < 75 ||GetPosX() > 600)
			{
				flip = !flip;
				SetVelX(GetVelX()*-1); 
			}

		}
		else
		{
			flip = false;
			SetVelX(120);
			animations[curAnimation].Stop();
			curAnimation = 0;
			animations[curAnimation].Resume();
			if(i == 0)
				hp = 649;

			if(GetPosX() == 700)
			{
				timer +=fElapsedTime;
				i = 1;
				flip = true;
				animations[curAnimation].Stop();
				curAnimation = 2;
				animations[curAnimation].Resume();
				SetVelX(0);
				if(timer>0.5f)
				{
					CSGD_MessageSystem::GetInstance()->SendMsg(new CCreateBulletMessage2(this));
					timer = 0;
				}
			}
		}
	}


	if(GetPosX() < 0 )
		SetPosX(0);
	if(GetPosX()+64 >= 800 )
		SetPosX(700);

	CBase::Update(fElapsedTime);


	x = (1000 - hp)/2.2f;
	if(hp < 0)
		x = (1100 - hp)/2.2f;


}

void CBoss::Render()
{
	RECT r;
	r.left = 32+x;
	r.right = 512;
	r.top = 0;
	r.bottom = r.top+128;

	animations[curAnimation].Render((int)GetPosX(),(int)GetPosY(),1.5f,flip);

	CSGD_TextureManager::GetInstance()->Draw(hpbarID, 800-512,0);
	CSGD_TextureManager::GetInstance()->Draw(hpID, 800+32-512+x,0,1,1,&r);
}
void CBoss::HandleEvent(CEvent* pEvent)
{
	if(pEvent->GetEventID() == "Collision")
	{
        if(pEvent->GetParam() == this)
		{
			this->hp -= 4;

			if(hp < -12)	
			{				
				CSGD_MessageSystem::GetInstance()->SendMsg(new CDestroyBossMessage(this));
				CGamePlayState::GetInstance()->SetPoints(CGamePlayState::GetInstance()->GetPoints()+5000);
       				CGamePlayState::GetInstance()->SetBoss(4);
				

			}
		}
	}

	else if(pEvent->GetEventID() == "TurnAround")
	{
		if( hp>660)
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
	}
	else if(pEvent->GetEventID() == "Pos")
	{
			CPlayer* p = (CPlayer*)pEvent->GetParam();

			
			if( hp>660 )
			{
				if(GetPosY() > p->GetPosY())
					SetPosY(GetPosY()-0.03f);
				else if(GetPosY() < p->GetPosY())
					SetPosY(GetPosY()+0.03f);

				if(abs(GetPosX() - p->GetPosX()) < 80)
				{
					animations[curAnimation].Stop();
					curAnimation = 1;
					animations[curAnimation].Resume();
				}
				else
				{
					animations[curAnimation].Stop();
					curAnimation = 0;
					animations[curAnimation].Resume();

				}
			}
			else
			{
				if(GetPosX() == 700 )
				{
					if(GetPosY() > p->GetPosY())
						SetPosY(GetPosY()-0.1f);
					else if(GetPosY() < p->GetPosY())
						SetPosY(GetPosY()+0.1f);
				}
			}
		
			
	}
}
