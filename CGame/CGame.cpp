#include "CGame.h"


CGame::CGame()
{
	m_pCurrentState = NULL;
	m_pDirect3D = NULL;
	m_pTextureManager = NULL;
	m_pFMODSystem = NULL;
	m_FMChannel1 = NULL;
	m_FMChannel2 = NULL;
	m_pDirectInput = NULL;
	m_pMessageSystem = NULL;
	m_pObjectFactory = NULL;
	m_pParticleSystem = NULL;

	// variables
	m_bIsRunning = false;
	m_nSFXVolume = 0;
	m_nMusicVolume = 0;
	m_PlayerInfo = 0;
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
	m_pDirect3D = Direct3D::GetInstance();
	m_pTextureManager = TextureManager::GetInstance();
	//m_pFMODSystem = ;
	//m_FMChannel1 = NULL;
	//m_FMChannel2 = NULL;
	m_pDirectInput = DirectInput::GetInstance();
	m_pMessageSystem = MessageSysterm::GetInstance();
	m_pObjectFactory = ObjectFactory::GetInstance();
	m_pParticleSystem = ParticleSystem::GetInstance();

	m_bIsRunning = true;

	ChangeStater(CMainMenuSystem::GetInstance());

}


void CGame::Shutdown()
{
	ChangeState(NULL);

	if(m_pParticleSystem)
	{
		m_pParticleSystem->Shutdown();
		m_pParticleSystem = NULL;
	}
	if(m_pObjectFactory)
	{
		m_pObjectFactory->Shutdown();
		m_pObjectFactory = NULL;
	}
	if(m_pMessageSystem)
	{
		m_pMessageSystem->Shutdown();
		m_pMessageSystem = NULL;
	}
	if(m_pDirectInput)
	{
		m_pDirectInput->Shutdown();
		m_pDirectInput = NULL;
	}
	if(m_pFMODSystem)
	{
		m_pFMODSystem->Shutdown();
		m_pFMODSystem = NULL;
	}
	if(m_pTextureManager)
	{
		m_pTextureManager->Shutdown();
		m_pTextureManager = NULL;
	}
	if(m_pDirect3D)
	{
		m_pDirect3D->Shutdown();
		m_pDirect3D = NULL;
	}
}


bool CGame::Main(void)
{
	DWORD dwStartTime = GetTickCount();
	static DWORD dwLastTime = GetTickCount();
	m_fElapsedTime = (float)(dwStartTime - dwLastTime)/1000.0f;
	dwLastTime = dwStartTime;

	static float fSecondTick = 0.0f;
	fSecondTick += m_fElapsedTime;
	if(fSecondTick >= 1.0f) { fSecondTick = 0.0f; }
	
	// input, update, render
	m_pDirectInput->ReadDevices();
	
	if(m_pCurrentState->Input() == false)
		return false;

	m_pCurrentState->Update(m_fElapsedTime);

	m_pCurrentState->Render();
	return true;
}

void CGame::ChangeState(IGameState *pGameState)
{
	if(m_pCurrentState) { m_pCurrentState->Exit(); }

	m_pCurrentState = pGameState;

	if(m_pCurrentState) { m_pCurrentState->Enter(); }
	
}

void LoadSettings(void)
{
	// get settings
	m_nSFXVolume = 0;
	m_nMusicVolume = 0;
	m_PlayerInfo = 0;
}

FMOD::System* CGame::GetFMODSystem(void)
{
	return &m_pFMODSystem;
}