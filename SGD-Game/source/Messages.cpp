////////////////////////////////////////////////////////////////////////////
//	File Name	:	"Messages.cpp"
//
//	Author		:	Matt DiMatteo (MD)
//
//	Purpose		:	This file contains all messages to be used throught the game
//
////////////////////////////////////////////////////////////////////////////
#include "Messages.h"
#include "CPlayer.h"
#include "CBullet.h"
#include"CEnemy.h"
#include"CHealth.h"
#include"CBoss.h"


CBaseMessage::CBaseMessage(MSGID msgID)
{
	m_msgID = msgID;
}
CBaseMessage::~CBaseMessage(void) {}

CCreateEnemyMessage::CCreateEnemyMessage(): CBaseMessage(MSG_CREATE_ENEMY) {}
CCreateEnemyMessage::~CCreateEnemyMessage() {}

CCreateEnemyMessage2::CCreateEnemyMessage2(): CBaseMessage(MSG_CREATE_ENEMY2) {}
CCreateEnemyMessage2::~CCreateEnemyMessage2() {}

CCreateBossMessage::CCreateBossMessage(): CBaseMessage(MSG_CREATE_BOSS) {}
CCreateBossMessage::~CCreateBossMessage() {}

CDestroyBossMessage::CDestroyBossMessage(CBoss* pBoss) : CBaseMessage(MSG_DESTROY_BOSS) 
{
	m_pBoss = pBoss;
	m_pBoss->AddRef();
}
CDestroyBossMessage::~CDestroyBossMessage() 
{
	m_pBoss->Release();
}


CDestroyEnemyMessage::CDestroyEnemyMessage(CEnemy* Enemy) : CBaseMessage(MSG_DESTROY_ENEMY) 
{
	m_pEnemy = Enemy;
	m_pEnemy->AddRef();
}
CDestroyEnemyMessage::~CDestroyEnemyMessage() 
{
	m_pEnemy->Release();
}

CCreateBulletMessage::CCreateBulletMessage(CPlayer* pPlayer): CBaseMessage(MSG_CREATE_BULLET1)
{
	m_pPlayer = pPlayer;
	m_pPlayer->AddRef();
}
CCreateBulletMessage::~CCreateBulletMessage()
{
	m_pPlayer->Release();
}
CCreateBulletMessage2::CCreateBulletMessage2(CBoss* pBoss): CBaseMessage(MSG_CREATE_BULLET2)
{
	m_pBoss = pBoss;
	m_pBoss->AddRef();
}
CCreateBulletMessage2::~CCreateBulletMessage2()
{
	m_pBoss->Release();
}

CDestroyBulletMessage::CDestroyBulletMessage(CBullet* pBullet): CBaseMessage(MSG_DESTROY_BULLET)
{
	m_pBullet = pBullet;
	m_pBullet->AddRef();
	
}
CDestroyBulletMessage::~CDestroyBulletMessage()
{
	m_pBullet->Release();
}

CDestroyBulletMessage2::CDestroyBulletMessage2(WebBullet* pBullet): CBaseMessage(MSG_DESTROY_BULLET2)
{
	m_pBullet = pBullet;
	m_pBullet->AddRef();
	
}
CDestroyBulletMessage2::~CDestroyBulletMessage2()
{
	m_pBullet->Release();
}

CDestroyPlayerMessage::CDestroyPlayerMessage(CPlayer* player): CBaseMessage(MSG_DESTROY_PLAYER)
{
	m_pPlayer = player;
	m_pPlayer->AddRef();

}
CDestroyPlayerMessage::~CDestroyPlayerMessage()
{
	m_pPlayer->Release();
}

CCreatePlayerMessage::CCreatePlayerMessage(): CBaseMessage(MSG_CREATE_PLAYER) {}

CDestroyPowerUp::CDestroyPowerUp(CHealth* pHealth) : CBaseMessage(MSG_DESTROY_PWRUP)
{
	m_pHealth = pHealth;
	m_pHealth->AddRef();

}
CCreatePowerUp::CCreatePowerUp(CEnemy* pEnemy):CBaseMessage(MSG_CREATE_PWRUP) 
{
	m_pEnemy = pEnemy;
	m_pEnemy->AddRef();
}
CCreatePowerUp::~CCreatePowerUp()
{
	m_pEnemy->Release();
}
CDestroyPowerUp::~CDestroyPowerUp()
{
	m_pHealth->Release();

}

CDestroyPowerUp2::CDestroyPowerUp2(CPoints* pPoints) : CBaseMessage(MSG_DESTROY_PWRUP2)
{
	m_pPoints = pPoints;
	m_pPoints->AddRef();

}
CDestroyPowerUp2::~CDestroyPowerUp2()
{
	m_pPoints->Release();

}
CCreatePowerUp2::CCreatePowerUp2(CEnemy* pEnemy):CBaseMessage(MSG_CREATE_PWRUP2) 
{
	m_pEnemy = pEnemy;
	m_pEnemy->AddRef();
}
CCreatePowerUp2::~CCreatePowerUp2()
{
	m_pEnemy->Release();
}
