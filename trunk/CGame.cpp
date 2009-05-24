////////////////////////////////////////////////////////
//   File Name	:	"CGame.cpp"
//
//   Author		:	Ramon Johannessen (RJ)
//
//   Purpose	:	To encapsulate all game related code
////////////////////////////////////////////////////////

#include "CGame.h"
#include "CSGD_Direct3D.h"
#include "CSGD_TextureManager.h"
#include "BattleMap.h"
//#include <fstream>
#include "CSGD_DirectInput.h"
#include "Assets.h"

CGame::CGame()
{
		//m_bIsPaused = false;
		m_pD3D = NULL;
		m_pTM = NULL;
		m_pDI = NULL;

		m_fElapsedTime = 0.0f;
		m_dwPrevTime = GetTickCount();

}
CGame::~CGame()
{
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
		m_pDI = CSGD_DirectInput::GetInstance();

		//  Initialize the singletons
		m_pD3D->InitDirect3D(hWnd, nScreenWidth, nScreenHeight, bIsWindowed, false);
		m_pTM->InitTextureManager(m_pD3D->GetDirect3DDevice(), m_pD3D->GetSprite());
		m_pAssets = CAssets::GetInstance();
		m_pDI->InitDirectInput(hWnd, hInstance, DI_KEYBOARD, 0);
		m_pDI->InitDirectInput(hWnd, hInstance, DI_MOUSE, 1);
		m_pDI->AcquireAll();

		SetIsRunning(true);

		m_nScreenWidth = nScreenWidth;
		m_nScreenHeight = nScreenHeight;
		
		//m_pAssets->LoadAssets();
		m_pBattleMap = new CBattleMap("MapInfo/MyMap.dat");
}
void CGame::Shutdown()
{
	delete m_pBattleMap;
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
	if (m_pDI)
	{
		m_pDI->ShutdownDirectInput();
		m_pDI = NULL;
	}
}
bool CGame::Main(POINT mouse)
{
	DWORD dwStartTime = GetTickCount();
	m_fElapsedTime = (float)(dwStartTime - m_dwPrevTime) / 1000.0f;
	m_dwPrevTime = dwStartTime;
	////////////////
	//Input:
	////////////////
	m_pDI->ReadDevices(); // once a frame!!

	if(!m_pBattleMap->Input(m_fElapsedTime, mouse))
		return false;
	//if (!m_pCurrentState->Input ())
	//	return false;
	m_pBattleMap->Update(m_fElapsedTime);

	////////////////
	//Update:
	////////////////
	
	////////////////
	//Draw:
	////////////////
	m_pD3D->Clear(0,0,0);
	m_pD3D->DeviceBegin();
	m_pD3D->SpriteBegin();

	//Do rendering here
	//m_pCurrentState->Render();
	m_pBattleMap->Render();

	m_pD3D->SpriteEnd();
	m_pD3D->DeviceEnd();
	//////////////////////////////////////////////
	//do text rendering after this to get it right
	//////////////////////////////////////////////
	char szCurrTile[32];
	sprintf_s(szCurrTile, "Current Tile: %d", m_pBattleMap->GetCurrSelectedTile());
	m_pD3D->DrawText(szCurrTile, 5, 5);
	sprintf_s(szCurrTile, "Mouse X: %d, Mouse Y: %d", mouse.x, mouse.y);
	m_pD3D->DrawText(szCurrTile, 5, 20);

	m_pD3D->Present();

	return GetIsRunning();	//keep running, if false, stop game
}
// void CGame::ChangeState(IGameState* pNewState)
// {
// 	if(m_pCurrentState)
// 	m_pCurrentState->Exit();
// 
// 	m_pCurrentState = pNewState;
// 	if(m_pCurrentState)
// 		m_pCurrentState->Enter();
// }

// void CGame::SetMusicVolume (int _nMusicVolume)
// {
// 	m_nMusicVolume += _nMusicVolume;
// 	if (m_nMusicVolume > 100)
// 		m_nMusicVolume = 100;
// 	else if (m_nMusicVolume < 0)
// 		m_nMusicVolume = 0;
// }
// 
// void CGame::SetSFXVolume (int _nSFXVolume)
// {
// 	m_nSFXVolume += _nSFXVolume;
// 	if (m_nSFXVolume > 100)
// 		m_nSFXVolume = 100;
// 	else if (m_nSFXVolume < 0)
// 		m_nSFXVolume = 0;
// }
// 
// void CGame::SetPanning (int _nPanning)
// {
// 	m_nPanning += _nPanning;
// 	if (m_nPanning < -50)
// 		m_nPanning = -50;
// 	else if (m_nPanning > 50)
// 		m_nPanning = 50;
// }

//void CGame::LoadSettings(const char* filename)
//{
// 	ifstream ifs(filename);
// 	if (ifs.good ())
// 	{
// 		int info = 0;
// 	
// 		ifs >> info;
// 		SetSFXVolume(info);
// 	
// 		ifs >> info;
// 		SetMusicVolume(info);
// 	
// 		ifs >> info;
// 		SetPanning(info);
// 		ifs.close ();
// 	}
// 	else
// 	{
// 		SetSFXVolume(50);
// 		SetMusicVolume(50);
// 		SetPanning(0);
// 	}
//}

void CGame::Render()
{

}