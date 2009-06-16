#include "Game.h"
#include "Assets.h"
#include "CSGD_TextureManager.h"
#include "CSGD_Direct3D.h"
#include "CSGD_DirectInput.h"
#include "MainMenuState.h"
#include "Player.h"
#include "MessageSystem.h"
#include "Factory.h"
#include "Ninja.h"
#include "ObjectManager.h"
#include "Battlemap.h"
#include "CSGD_FModManager.h"
#include "BitmapFont.h"
#include <ctime>
//#include "ParticleSystem.h"
//#include "MessageSystem.h"
//#include "ObjectFactory.h"

CGame::CGame()
{
	m_pCurrentState = NULL;
	m_pD3D = NULL;
	m_pTM = NULL;
	m_pDI = NULL;
	m_pPlayer = NULL;

// 	m_pMessageSystem = NULL;
// 	m_pObjectFactory = NULL;
// 	m_pParticleSystem = NULL;

	// variables
	m_bIsRunning = false;
	m_fSFXVolume = 0.2f;
	m_fMusicVolume = 0.2f;
/*	m_PlayerInfo = 0;*/

	srand((unsigned int)(time(0)));
}

CGame::~CGame()
{
}

CGame* CGame::GetInstance(void)
{
	static CGame instance;
	return &instance;
}

void CGame::Initialize(HWND hWnd, HINSTANCE hInstance, int nScreenWidth, int nScreenHeight, bool bIsWindowed)
{
	//Get pointers to singletons:
	m_pTM = CSGD_TextureManager::GetInstance();
	m_pDI = CSGD_DirectInput::GetInstance();
	m_pD3D = CSGD_Direct3D::GetInstance();
	m_pMS = MessageSystem::GetInstance();
	m_pFMOD = CSGD_FModManager::GetInstance();
	m_nScreenWidth = nScreenWidth;
	m_nScreenHeight = nScreenHeight;
	
	//m_pMessageSystem = MessageSysterm::GetInstance();
	//m_pObjectFactory = Factory::GetInstance();
	//m_pParticleSystem = ParticleSystem::GetInstance();

	//  Initialize the singletons
	m_pD3D->InitDirect3D(hWnd, nScreenWidth, nScreenHeight, bIsWindowed, false);
	m_pTM->InitTextureManager(m_pD3D->GetDirect3DDevice(), m_pD3D->GetSprite());
	m_pMS->InitMessageSystem(MessageProc);

	m_pBitMapFont = CBitmapFont::GetInstance();
	// NOTE::assets class requires texture manager, bitmapFont, and FMOD to be initialized properly
	if(!m_pFMOD->InitFModManager(hWnd))
		exit(0);
	m_pFMOD->Update();
	m_pAssets = CAssets::GetInstance();

	//////////////////////////////////////////////////////////////////////////

	m_pDI->InitDirectInput(hWnd, hInstance, DI_KEYBOARD, 0);
	m_pDI->InitDirectInput(hWnd, hInstance, DI_MOUSE, 0);
	m_pDI->InitDirectInput(hWnd, hInstance, DI_JOYSTICKS, 0);
	m_pDI->AcquireAll();

	SetIsRunning(true);

	m_pPlayer = CPlayer::GetInstance();

	ChangeState(CMainMenuState::GetInstance());
}

void CGame::Shutdown()
{
	ChangeState(NULL);
	if(m_pMS){m_pMS->ShutdownMessageSystem(); m_pMS=NULL;}

// 	if(m_pParticleSystem)
// 	{
// 		m_pParticleSystem->Shutdown();
// 		m_pParticleSystem = NULL;
// 	}
 	//if(m_pObjectFactory)
 	//{
 	//	m_pObjectFactory->Shutdown();
 	//	m_pObjectFactory = NULL;
 	//}
// 	if(m_pMessageSystem)
// 	{
// 		m_pMessageSystem->Shutdown();
// 		m_pMessageSystem = NULL;
// 	}
	if (m_pFMOD)
	{
		m_pFMOD->ShutdownFModManager();
		m_pFMOD = NULL;
	}
	if(m_pDI)
	{
		m_pDI->ShutdownDirectInput();
		m_pDI = NULL;
	}
	if(m_pTM)
	{
		m_pTM->ShutdownTextureManager();
		m_pTM = NULL;
	}
	if(m_pD3D)
	{
		m_pD3D->ShutdownDirect3D();
		m_pD3D = NULL;
	}
	if (m_pBitMapFont)
		m_pBitMapFont = NULL;
}

bool CGame::Main(POINT mouse)
{
	if (!m_pAssets->IsLoading())
	{
		DWORD dwStartTime = GetTickCount();
		static DWORD dwLastTime = GetTickCount();
		m_fElapsedTime = (float)(dwStartTime - dwLastTime)/1000.0f;
		dwLastTime = dwStartTime;
	
		static float fSecondTick = 0.0f;
		fSecondTick += m_fElapsedTime;
		if(fSecondTick >= 1.0f) { fSecondTick = 0.0f; }
		
		// input, update, render
		m_pDI->ReadDevices();
		/*if ( ( m_pDI->KeyDown(DIK_RETURN) && m_pDI->KeyDown(DIK_LMENU) ) || ( m_pDI->KeyDown(DIK_RETURN) && m_pDI->KeyDown(DIK_RMENU) ) )
		{
			m_pD3D->ChangeDisplayParam(m_pD3D->GetPresentParams()->BackBufferWidth, m_pD3D->GetPresentParams()->BackBufferHeight, m_pD3D->GetPresentParams()->Windowed);
			m_pDI->ReadDevices();
		}*/
		
		if ((m_pDI->KeyDown(DIK_RMENU) && m_pDI->KeyDown(DIK_RETURN))  || (m_pDI->KeyDown(DIK_LMENU) && m_pDI->KeyDown(DIK_RETURN)) )
		{
			m_pD3D->ChangeDisplayParam(m_pD3D->GetPresentParams()->BackBufferWidth, m_pD3D->GetPresentParams()->BackBufferHeight, !m_pD3D->GetPresentParams()->Windowed);
			m_pDI->ReadDevices();
		}
	
		if(m_pCurrentState->Input(m_fElapsedTime, mouse) == false)
			return false;
	
		m_pCurrentState->Update(m_fElapsedTime);
		m_pFMOD->Update();
	
		m_pMS->ProcessMsgs();
	
		m_pD3D->Clear(255,255,255);
	
		m_pD3D->DeviceBegin();
		m_pD3D->SpriteBegin();
		m_pCurrentState->Render();
		m_pD3D->SpriteEnd();
		m_pD3D->DeviceEnd();
	
		m_pD3D->Present();
	}

	return true;
}

void CGame::ChangeState(IGameState *pGameState)
{
	if(m_pCurrentState) { m_pCurrentState->Exit(); }

	m_pCurrentState = pGameState;

	if(m_pCurrentState) { m_pCurrentState->Enter(); }
}

void CGame::LoadSettings(void)
{
	// get settings
	m_fSFXVolume = 0.2f;
	m_fMusicVolume = 0.2f;
//	m_PlayerInfo = 0;
}

//FMOD::System* CGame::GetFMODSystem(void)
//{
//	return &m_pFMODSystem;
//}

void CGame::SetMusicVolume(float _nMusicVolume)
{

}

void CGame::SetSFXVolume(float _nSFXVolume)
{

}
void MessageProc(CBaseMessage* pMsg)
{
	switch(pMsg->GetMsgID())
	{
	case MSG_CREATE_ITEM:
		{
			int type = rand()% 3;
			CCreateItem * pCP = (CCreateItem*)pMsg;
			Factory::GetInstance()->CreateBattleItem(type,pCP->GetNinja()->GetMapCoord() );
			ObjectManager::GetInstance()->Remove(pCP->GetNinja());
		}
		break;
	case MSG_DESTROY_ITEM:
		{
			CDestroyItem * pDP = (CDestroyItem*)pMsg;
			CBattleItem p = *(pDP->GetItem());
			CPlayer::GetInstance()->AddItem(p);
			//CBattleMap::GetInstance()->GetChars()[CBattleMap::GetInstance()->GetCurrActive()].SetCurrAP(CBattleMap::GetInstance()->GetChars()[CBattleMap::GetInstance()->GetCurrActive()].GetCurrAP()+30);
			ObjectManager::GetInstance()->Remove(pDP->GetItem());
		}
		break;
	case MSG_CREATE_WEAPON:
		{
			int type = rand()% 12;
			CCreateWeapon * pCP = (CCreateWeapon*)pMsg;
			Factory::GetInstance()->CreateWeapon(type,pCP->GetNinja()->GetMapCoord() );
			ObjectManager::GetInstance()->Remove(pCP->GetNinja());
			
		}
		break;
	case MSG_DESTROY_WEAPON:
		{
			CDestroyWeapon * pDW = (CDestroyWeapon*)pMsg;
			CBase p = *(pDW->GetWeapon());
			CPlayer::GetInstance()->AddWeapon(p);
			ObjectManager::GetInstance()->Remove(pDW->GetWeapon());

		}
		break;
	}
}