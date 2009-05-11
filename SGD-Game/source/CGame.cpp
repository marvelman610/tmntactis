////////////////////////////////////////////////////////
//   File Name	:	"CGame.cpp"
//
//   Author		:	Matthew Di Matteo (MD)
//
//   Purpose	:	To encapsulate all game related code
////////////////////////////////////////////////////////
#include "CGame.h"

#include "CBullet.h"
#include "SGD_Math.h"
#include "CSGD_ObjectManager.h"
#include "CEnemy.h"
#include"CSGD_EventSystem.h"

#include"CHealth.h"
#include"CBoss.h"

#include"tinyxml/tinyxml.h"


CGame::CGame()
{
		m_pD3D = NULL;
		m_pTM = NULL;
		m_pDS = NULL;
		m_pWM = NULL;
		m_pDI = NULL;

		m_pMS = NULL;

		m_pOF = NULL;

		m_nImageID = -1;
		m_nSoundID = -1;

}
CGame* CGame::GetInstance()
{
	static CGame instance;
	return &instance;
}
void CGame::Initialize(HWND hWnd, HINSTANCE hInstance, int nScreenWidth, int nScreenHeight, bool bIsWindowed)
{
	//Get pointers to singletons:
	m_pD3D = CSGD_Direct3D::GetInstance();
	m_pTM = CSGD_TextureManager::GetInstance();
	m_pDS = CSGD_DirectSound::GetInstance();
	m_pWM = CSGD_WaveManager::GetInstance();
	m_pDI = CSGD_DirectInput::GetInstance();
	m_pMS = CSGD_MessageSystem::GetInstance();
	m_pOF = CSGD_ObjectFactory<string, CBase>::GetInstance();

	//  Initialize the singletons
	m_pD3D->InitDirect3D(hWnd, nScreenWidth, nScreenHeight, bIsWindowed, false);
	m_pTM->InitTextureManager(m_pD3D->GetDirect3DDevice(), m_pD3D->GetSprite());
	m_pDS->InitDirectSound(hWnd);
	m_pWM->InitWaveManager(hWnd, m_pDS->GetDSObject());
	m_pDI->InitDirectInput(hWnd, hInstance, DI_KEYBOARD, 0);

	m_pMS->InitMessageSystem(MessageProc);

	m_pOF->RegisterClassType<CBullet>("CBullet");
	m_pOF->RegisterClassType<WebBullet>("WebBullet");
	m_pOF->RegisterClassType<CEnemy>("CEnemy");
	m_pOF->RegisterClassType<CPlayer>("CPlayer");
	m_pOF->RegisterClassType<CHealth>("CHealth");
	m_pOF->RegisterClassType<CBoss>("CBoss");
	m_pOF->RegisterClassType<CPoints>("CPoints");


	//ChangeState(CMainMenuState::GetInstance());
	ChangeState(CIntroState::GetInstance());

	SetIsRunning(true);

	TiXmlDocument doc;
	if(!doc.LoadFile("resource/data/options.xml"))
	{
		SetSFXVolume(50);
		SetBGMVolume(50);
		SetPanning(50);
	}
	else
	{
		TiXmlElement* pRoot = doc.RootElement();
		TiXmlElement* pBGM = pRoot->FirstChildElement("bgm");
		
		if(pBGM)						
			pBGM->Attribute("volume", &BGMvolume);
	
		TiXmlElement* pSFX = pRoot->FirstChildElement("sfx");
		if(pSFX)
			pSFX->Attribute("volume", &SFXvolume);
		TiXmlElement* pPan = pRoot->FirstChildElement("panning");
		if(pPan)
			pPan->Attribute("p", &panning);
		

	}
	//Load common assets


}
void CGame::Shutdown()
{
	TiXmlDocument doc;

	TiXmlDeclaration* pDec = new TiXmlDeclaration("1.0", "utf-8", "yes");
	doc.LinkEndChild(pDec);

	TiXmlElement* pRoot = new TiXmlElement("options");
	doc.LinkEndChild(pRoot);

	TiXmlElement* pBGM = new TiXmlElement("bgm");
	pBGM->SetAttribute("volume", BGMvolume);
	pRoot->LinkEndChild(pBGM);

	TiXmlElement* pSFX = new TiXmlElement("sfx");
	pSFX->SetAttribute("volume", SFXvolume);
	pRoot->LinkEndChild(pSFX);

	TiXmlElement* pPan = new TiXmlElement("panning");
	pPan->SetAttribute("p", panning);
	pRoot->LinkEndChild(pPan);

	doc.SaveFile("resource/data/options.xml");

	
	if(m_pOF){m_pOF->ShutdownObjectFactory(); m_pOF=NULL;}
	if(m_pMS){m_pMS->ShutdownMessageSystem(); m_pMS=NULL;}
	CSGD_EventSystem::GetInstance()->ShutdownEventSystem();
	ChangeState(NULL);

	if(m_pDI)
	{
		m_pDI->ShutdownDirectInput();
		m_pDI = NULL;
	}
	if(m_pWM)
	{
		m_pWM->ShutdownWaveManager();
		m_pWM = NULL;
	}
	if(m_pDS)
	{
		m_pDS->ShutdownDirectSound();
		m_pDS = NULL;
	}
	if(m_pTM)
	{
		m_pTM->ShutdownTextureManager();
		m_pTM=NULL;
	}
	if(m_pD3D)
	{
		m_pD3D->ShutdownDirect3D();
		m_pD3D=NULL;
	}
}
bool CGame::Main()
{
	////////////////
	//Input:
	////////////////
	m_pDI->ReadDevices(); // once a frame!!
	m_pCurrentState->Input();
	
	if ((m_pDI->KeyDown(DIK_RMENU) && m_pDI->KeyDown(DIK_RETURN)) || (m_pDI->KeyDown(DIK_LMENU) && m_pDI->KeyDown(DIK_RETURN)) )
		m_pD3D->ChangeDisplayParam(m_pD3D->GetPresentParams()->BackBufferWidth, m_pD3D->GetPresentParams()->BackBufferHeight, !m_pD3D->GetPresentParams()->Windowed);

	////////////////
	//Update:                              
	////////////////
	m_pWM->Update();
	m_pCurrentState->Update();
	m_pMS->ProcessMessages();
	CSGD_EventSystem::GetInstance()->ProcessEvents();

	////////////////
	//Draw:
	////////////////
	m_pD3D->Clear(0,0,0);
	m_pD3D->DeviceBegin();
	m_pD3D->SpriteBegin();

	//Do rendering here
	m_pCurrentState->Render();

	m_pD3D->SpriteEnd();
	m_pD3D->DeviceEnd();
	/////////////////////////////////
	//do text rendering after this to get it right
	//////////////////

	m_pD3D->Present();

	return GetIsRunning();	//keep running, if false, stop game
}
void CGame::ChangeState(IGameState* pNewState)
{
	if(m_pCurrentState)
	m_pCurrentState->Exit();

	m_pCurrentState = pNewState;
	if(m_pCurrentState)
		m_pCurrentState->Enter();


}
void MessageProc(CBaseMessage* pMsg)
{
	CGame * pGame = CGame::GetInstance();

	switch(pMsg->GetMsgID())
	{
	case MSG_CREATE_BULLET1:
		{
			CCreateBulletMessage * pCBM = (CCreateBulletMessage*)pMsg;
			CBullet * pBullet = (CBullet*)pGame->m_pOF->CreateObject("CBullet");
			pBullet->SetOwner(pCBM->GetPlayer());
			//pBullet->SetImageID(CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/MaD_Bullet.png"));

			pBullet->SetPosX(pCBM->GetPlayer()->GetPosX()+90);
			pBullet->SetPosY(pCBM->GetPlayer()->GetPosY()+20);
			if(pCBM->GetPlayer()->GetFacing())
				pBullet->SetRotation((270*SGD_PI) / 180);	

			tVector2D vOrientation;
			vOrientation.fX = 0.0f;
			vOrientation.fY = 1.0f;
			vOrientation = vOrientation * (200);
			vOrientation = Vector2DRotate(vOrientation, pBullet->GetOwner()->GetRotation());
			pBullet->SetVelX(vOrientation.fX);
			pBullet->SetVelY(vOrientation.fY);

			CSGD_ObjectManager::GetInstance()->AddObject(pBullet);
			pBullet->Release();		
		}
		break;
	case MSG_CREATE_BULLET2:
		{
			CCreateBulletMessage2 * pCBM = (CCreateBulletMessage2*)pMsg;
			WebBullet * pBullet = (WebBullet*)pGame->m_pOF->CreateObject("WebBullet");
			pBullet->SetBoss(pCBM->GetBoss());
			//pBullet->SetImageID(CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/MaD_web.png"));

			pBullet->SetPosX(690);
			pBullet->SetPosY(pCBM->GetBoss()->GetPosY()+20);
			/*if(pCBM->GetPlayer()->GetFacing())
				pBullet->SetRotation((270*SGD_PI) / 180);*/	

			pBullet->SetVelX(-70);
			//pBullet->SetVelY(vOrientation.fY);

			CSGD_ObjectManager::GetInstance()->AddObject(pBullet);
			pBullet->Release();		
		}
		break;
	case MSG_CREATE_ENEMY:
		{
			CCreateEnemyMessage * pCTM = (CCreateEnemyMessage*)pMsg;
			CEnemy* pEnemy = (CEnemy*)pGame->m_pOF->CreateObject("CEnemy");
			pEnemy->SetPosX(800.0f);
			pEnemy->SetPosY(float(rand()% 180 + 300));
			pEnemy->SetWidth(30);
			pEnemy->SetHeight(96);
			pEnemy->SetVelX(-30);
			CSGD_ObjectManager::GetInstance()->AddObject(pEnemy);
			pEnemy->Release();
		}
		break;
	case MSG_CREATE_ENEMY2:
			{
				CCreateEnemyMessage2 * pCTM = (CCreateEnemyMessage2*)pMsg;
				CEnemy* pEnemy = (CEnemy*)pGame->m_pOF->CreateObject("CEnemy");
				pEnemy->SetFlip(false);
				pEnemy->SetPosX(0.0f);
				pEnemy->SetPosY(float(rand()% 180 + 300));
				pEnemy->SetWidth(30);
				pEnemy->SetHeight(96);
				pEnemy->SetVelX(30);
				CSGD_ObjectManager::GetInstance()->AddObject(pEnemy);
				pEnemy->Release();
			}
		break;
	case MSG_CREATE_BOSS:
			{
			 	CCreateBossMessage * pCBM = (CCreateBossMessage*)pMsg;
				CBoss* pBoss = (CBoss*)pGame->m_pOF->CreateObject("CBoss");
				pBoss->SetFlip(true);
				pBoss->SetPosX(750);
				pBoss->SetPosY(400);
				pBoss->SetWidth(30);
				pBoss->SetHeight(96);
				pBoss->SetVelX(-45);
				CSGD_ObjectManager::GetInstance()->AddObject(pBoss);
				pBoss->Release();
			}
		break;
	case MSG_DESTROY_BULLET:
		{
			CDestroyBulletMessage * pDB = (CDestroyBulletMessage*)pMsg;
			CSGD_ObjectManager::GetInstance()->RemoveObject(pDB->GetBullet());
		}
		break;
	case MSG_DESTROY_BULLET2:
		{
			CDestroyBulletMessage2 * pDB = (CDestroyBulletMessage2*)pMsg;
			CSGD_ObjectManager::GetInstance()->RemoveObject(pDB->GetBullet());
		}
		break;
	case MSG_DESTROY_ENEMY:
		{
			CDestroyEnemyMessage * pDE = (CDestroyEnemyMessage*)pMsg;
			CSGD_ObjectManager::GetInstance()->RemoveObject(pDE->GetEnemy());
		}
		break;
	case MSG_DESTROY_PLAYER:
		{
			CDestroyPlayerMessage * pDP = (CDestroyPlayerMessage*)pMsg;
			CSGD_ObjectManager::GetInstance()->RemoveObject(pDP->GetPlayer());
		}
		break;
	case MSG_CREATE_PWRUP:
		{
			CCreatePowerUp* pCP = (CCreatePowerUp*)pMsg;
			CHealth* pHealth = (CHealth*)pGame->m_pOF->CreateObject("CHealth");
  			pHealth->SetOwner(pCP->GetEnemy());
			pHealth->SetPosX(pHealth->GetOwner()->GetPosX());
			pHealth->SetPosY(pHealth->GetOwner()->GetPosY()+pHealth->GetOwner()->GetHeight()-40);
			pHealth->SetHeight(40);
			pHealth->SetWidth(64);
			CSGD_ObjectManager::GetInstance()->AddObject(pHealth);
			pHealth->Release();
		}
		break;
	case MSG_CREATE_PWRUP2:
		{
			CCreatePowerUp2* pCP = (CCreatePowerUp2*)pMsg;
			CPoints* pPoints = (CPoints*)pGame->m_pOF->CreateObject("CPoints");
  			pPoints->SetOwner(pCP->GetEnemy());
			pPoints->SetPosX(pPoints->GetOwner()->GetPosX());
			pPoints->SetPosY(pPoints->GetOwner()->GetPosY()+pPoints->GetOwner()->GetHeight()-40);
			pPoints->SetHeight(40);
			pPoints->SetWidth(64);
			CSGD_ObjectManager::GetInstance()->AddObject(pPoints);
			pPoints->Release();
		}
		break;
	case MSG_CREATE_PLAYER:
		{
			CCreatePlayerMessage * pCP = (CCreatePlayerMessage*)pMsg;
			CPlayer* pPlayer = (CPlayer*)pGame->m_pOF->CreateObject("CPlayer");
			pPlayer->SetPosX(100);
			pPlayer->SetPosY((float)(rand()% 200 + 400));
			CSGD_ObjectManager::GetInstance()->AddObject(pPlayer);
			pPlayer->Release();
			
		}
		break;
	case MSG_DESTROY_PWRUP:
		{
			CDestroyPowerUp * pDP = (CDestroyPowerUp*)pMsg;
			CSGD_ObjectManager::GetInstance()->RemoveObject(pDP->GetPwrup());
		}
		break;
	case MSG_DESTROY_PWRUP2:
		{
			CDestroyPowerUp2 * pDP = (CDestroyPowerUp2*)pMsg;
			CSGD_ObjectManager::GetInstance()->RemoveObject(pDP->GetPwrup());
		}
		break;
	case MSG_DESTROY_BOSS:
		{
			CDestroyBossMessage * pDB = (CDestroyBossMessage*)pMsg;
			CSGD_ObjectManager::GetInstance()->RemoveObject(pDB->GetBoss());
		}
		break;	
	}
}

