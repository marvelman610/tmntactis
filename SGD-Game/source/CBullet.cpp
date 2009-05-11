////////////////////////////////////////////////////////
//   File Name	:	"CBullet.cpp"
//
//   Author		:	Matthew Di Matteo (MD)
//
//   Purpose	:	To provide a game object used as a projectile
//					in the game
////////////////////////////////////////////////////////
#include "CBullet.h"
#include "SGD Wrappers\CSGD_TextureManager.h"
#include "CSGD_MessageSystem.h"
#include "CSGD_EventSystem.h"
#include"CGamePlayState.h"
#include"SGD_Math.h"

CBullet::CBullet()
{
	m_nType = OBJECT_BULLET;
	m_fRotation = NULL;
	m_pOwner = NULL;
	SetImageID(CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/MaD_Bullet.png"));
	SetWidth(8);
	SetHeight(8);

	SetRotation( (90*SGD_PI) / 180 );

	tVector2D vOrientation;
	vOrientation.fX = 0.0f;
	vOrientation.fY = 1.0f;
	vOrientation = vOrientation * 200;
	vOrientation = Vector2DRotate(vOrientation, -GetRotation());
	SetVelX(vOrientation.fX);
	SetVelY(vOrientation.fY);

	CSGD_EventSystem::GetInstance()->RegisterClient("Collision", this);
	
}
CBullet::~CBullet()
{
	CSGD_EventSystem::GetInstance()->UnregisterClient("Collision",this);
	
	CSGD_TextureManager::GetInstance()->UnloadTexture(GetImageID());
}
void CBullet::Update(float fElapsedTime)
{
	CBase::Update(fElapsedTime);
	if(GetPosX() > 800 || GetPosX() < 0 )
		CSGD_MessageSystem::GetInstance()->SendMsg(new CDestroyBulletMessage(this));
}
void CBullet::Render()
{
	CSGD_TextureManager::GetInstance()->Draw(GetImageID(), int(GetPosX()-(GetWidth()/2)), int(GetPosY() - (GetHeight()/2)), 1, 1, 0,(GetWidth()/2.0f),(GetHeight()/2.0f),m_fRotation);
	
}
void CBullet::HandleEvent(CEvent* pEvent)
{
	if(pEvent->GetEventID() == "Collision")
	{	
		if(pEvent->GetParam() == this)
			CSGD_MessageSystem::GetInstance()->SendMsg(new CDestroyBulletMessage(this));
	}
}

WebBullet::WebBullet()
{
	SetImageID(CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/MaD_web.png"));
	m_nType = OBJECT_WEB;
	SetWidth(8);
	SetHeight(8);
	CSGD_EventSystem::GetInstance()->RegisterClient("Collision", this);

}
WebBullet::~WebBullet()
{
	CSGD_EventSystem::GetInstance()->UnregisterClient("Collision", this);

	CSGD_TextureManager::GetInstance()->UnloadTexture(GetImageID());
}

void WebBullet::Update(float fElapsedTime)
{
	CBase::Update(fElapsedTime);
	if(GetPosX() > 800 || GetPosX() < 0 )
		CSGD_MessageSystem::GetInstance()->SendMsg(new CDestroyBulletMessage2(this));

}
void WebBullet::Render()
{
	CSGD_TextureManager::GetInstance()->Draw(GetImageID(), int(GetPosX()-(GetWidth()/2)), int(GetPosY() - (GetHeight()/2)), 2, 2, 0,(GetWidth()/2.0f),(GetHeight()/2.0f));
	
}
void WebBullet::HandleEvent(CEvent* pEvent)
{
	if(pEvent->GetEventID() == "Collision")
	{	
		if(pEvent->GetParam() == this)
		{
			CSGD_MessageSystem::GetInstance()->SendMsg(new CDestroyBulletMessage2(this));
		}
	}
}