#include "Game.h"
#include "Assets.h"
#include "CSGD_TextureManager.h"
#include "CSGD_Direct3D.h"
#include "CSGD_DirectInput.h"
#include "MainMenuState.h"
#include "Player.h"
//#include "ParticleSystem.h"

//#include "MessageSystem.h"
//#include "ObjectFactory.h"
//#include "FMod.hpp"
//#pragma comment(lib, "FModEx.dll");


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
// 	m_pFMODSystem = NULL;
// 	m_FMChannel1 = NULL;
// 	m_FMChannel2 = NULL;

	// variables
	m_bIsRunning = false;
	m_nSFXVolume = 0;
	m_nMusicVolume = 0;
/*	m_PlayerInfo = 0;*/
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
	
	//m_pFMODSystem = ;
	//m_FMChannel1 = NULL;
	//m_FMChannel2 = NULL;
	//m_pMessageSystem = MessageSysterm::GetInstance();
	//m_pObjectFactory = ObjectFactory::GetInstance();
	//m_pParticleSystem = ParticleSystem::GetInstance();

	//  Initialize the singletons
	m_pD3D->InitDirect3D(hWnd, nScreenWidth, nScreenHeight, bIsWindowed, false);
	m_pTM->InitTextureManager(m_pD3D->GetDirect3DDevice(), m_pD3D->GetSprite());
	

	// assets class requires texture manager to be initialized
	m_pAssets = CAssets::GetInstance();
	m_pAssets->LoadAssets();
	//////////////////////////////////////////////////////////////////////////

	m_pDI->InitDirectInput(hWnd, hInstance, DI_KEYBOARD, 0);
	m_pDI->InitDirectInput(hWnd, hInstance, DI_MOUSE, 0);
	m_pDI->AcquireAll();

	SetIsRunning(true);

	m_nScreenWidth = nScreenWidth;
	m_nScreenHeight = nScreenHeight;

	m_pPlayer = CPlayer::GetInstance();

	ChangeState(CMainMenuState::GetInstance());
}


void CGame::Shutdown()
{
	ChangeState(NULL);

// 	if(m_pParticleSystem)
// 	{
// 		m_pParticleSystem->Shutdown();
// 		m_pParticleSystem = NULL;
// 	}
// 	if(m_pObjectFactory)
// 	{
// 		m_pObjectFactory->Shutdown();
// 		m_pObjectFactory = NULL;
// 	}
// 	if(m_pMessageSystem)
// 	{
// 		m_pMessageSystem->Shutdown();
// 		m_pMessageSystem = NULL;
// 	}
	if(m_pDI)
	{
		m_pDI->ShutdownDirectInput();
		m_pDI = NULL;
	}
// 	if(m_pFMODSystem)
// 	{
// 		m_pFMODSystem->Shutdown();
// 		m_pFMODSystem = NULL;
// 	}
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
}


bool CGame::Main(POINT mouse)
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
	if ( ( m_pDI->KeyDown(DIK_RETURN) && m_pDI->KeyDown(DIK_LMENU) ) || ( m_pDI->KeyDown(DIK_RETURN) && m_pDI->KeyDown(DIK_RMENU) ) )
	{
		m_pD3D->ChangeDisplayParam(m_pD3D->GetPresentParams()->BackBufferWidth, m_pD3D->GetPresentParams()->BackBufferHeight, m_pD3D->GetPresentParams()->Windowed);
		m_pDI->ReadDevices();
	}
	
	if(m_pCurrentState->Input(m_fElapsedTime, mouse) == false)
		return false;

	m_pCurrentState->Update(m_fElapsedTime);

	m_pD3D->Clear(0,0,128);

	m_pD3D->DeviceBegin();
	m_pD3D->SpriteBegin();
	m_pCurrentState->Render();
	m_pD3D->SpriteEnd();
	m_pD3D->DeviceEnd();

	m_pD3D->Present();

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
	m_nSFXVolume = 0;
	m_nMusicVolume = 0;
//	m_PlayerInfo = 0;
}

//FMOD::System* CGame::GetFMODSystem(void)
//{
//	return &m_pFMODSystem;
//}

void CGame::SetMusicVolume(int _nMusicVolume)
{

}

void CGame::SetSFXVolume(int _nSFXVolume)
{

}