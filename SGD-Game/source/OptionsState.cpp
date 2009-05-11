////////////////////////////////////////////////////////////////////////////
//	File Name	:	"COptionsState.cpp"
//
//	Author		:	Matt DiMatteo (MD)
//
//	Purpose		:	options state derived from the base state
//					used to set game options 
////////////////////////////////////////////////////////////////////////////
#include "OptionsState.h"

COptionsState* COptionsState::GetInstance()
{
	static COptionsState instance;
	return &instance;
}
void COptionsState::Enter() 
{
	m_pDI = CSGD_DirectInput::GetInstance();
	m_pD3D = CSGD_Direct3D::GetInstance();
	m_pTM = CSGD_TextureManager::GetInstance();
	m_pWM = CSGD_WaveManager::GetInstance();
	m_pDS = CSGD_DirectSound::GetInstance();

	//create texture
	m_nImageID = m_pTM->LoadTexture("resource/graphics/MaD_HandGun.bmp");
	m_nSoundLoopID = m_pWM->LoadWave("resource/sound/MaD_BGM.wav");
	m_nSoundEffectID = m_pWM->LoadWave("resource/sound/MaD_Death.wav");

	gunsound = CSGD_WaveManager::GetInstance()->LoadWave("resource/sound/MaD_Tankshot.wav");

	//CSGD_WaveManager* pWM = CSGD_WaveManager::GetInstance();

	CGame* pGame = CGame::GetInstance();


	m_pWM->Play(m_nSoundLoopID, DSBPLAY_LOOPING);

	m_pWM->SetVolume(m_nSoundLoopID, pGame->GetBGMVolume());
	m_pWM->SetVolume(m_nSoundEffectID, pGame->GetSFXVolume());
	m_pWM->SetPan(m_nSoundLoopID, (pGame->GetPanning()-50)*2);
	m_pWM->SetPan(m_nSoundEffectID, (pGame->GetPanning()-50)*2);



	font.Load("resource/graphics/MaD_Font.png",20,20,20);
	//font.Load("resource/graphics/MaD_Font.png",20,21,13);

	m_nCurrentMenuSelection = 0;

	
}
bool COptionsState::Input() 
{
	//if (CSGD_DirectInput::GetInstance()->KeyPressed(DIK_UP))
	//	m_nCurrentMenuSelection--;

	//if (CSGD_DirectInput::GetInstance()->KeyPressed(DIK_DOWN))
	//	m_nCurrentMenuSelection++;

	CGame *pGame = CGame::GetInstance();

	switch (m_nCurrentMenuSelection)
	{
		case 0:
			if (CSGD_DirectInput::GetInstance()->KeyPressed(DIK_LEFT)) 
			{

				pGame->SetSFXVolume(pGame->GetSFXVolume() - 5);
				CSGD_WaveManager::GetInstance()->SetVolume(m_nSoundEffectID, pGame->GetSFXVolume());
				CSGD_WaveManager::GetInstance()->Play(m_nSoundEffectID);
			}

			if (CSGD_DirectInput::GetInstance()->KeyPressed(DIK_RIGHT)) {
				pGame->SetSFXVolume(pGame->GetSFXVolume() + 5);
				CSGD_WaveManager::GetInstance()->SetVolume(m_nSoundEffectID, pGame->GetSFXVolume());
				CSGD_WaveManager::GetInstance()->Play(m_nSoundEffectID);
			}


			if(pGame->GetSFXVolume() > 100)
				pGame->SetSFXVolume(100);
			if(CGame::GetInstance()->GetSFXVolume() < 0)
				pGame->SetSFXVolume(0);

			break;
		case 1:
			if (CSGD_DirectInput::GetInstance()->KeyPressed(DIK_LEFT)){
				pGame->SetBGMVolume(pGame->GetBGMVolume() - 5);
				CSGD_WaveManager::GetInstance()->SetVolume(m_nSoundLoopID, pGame->GetBGMVolume());
			}

			if (CSGD_DirectInput::GetInstance()->KeyPressed(DIK_RIGHT)){
				pGame->SetBGMVolume(pGame->GetBGMVolume() + 5);
				CSGD_WaveManager::GetInstance()->SetVolume(m_nSoundLoopID, pGame->GetBGMVolume());
			}

			if(CGame::GetInstance()->GetBGMVolume() > 100)
				CGame::GetInstance()->SetBGMVolume(100);
			if(CGame::GetInstance()->GetBGMVolume() < 0)
				CGame::GetInstance()->SetBGMVolume(0);

			break;
		case 2:
			if (CSGD_DirectInput::GetInstance()->KeyPressed(DIK_LEFT)) 
			{
				CGame::GetInstance()->SetPanning(CGame::GetInstance()->GetPanning()-5);
				if(CGame::GetInstance()->GetPanning() < 0)
					CGame::GetInstance()->SetPanning(0);
				m_pWM->SetPan(m_nSoundLoopID, (CGame::GetInstance()->GetPanning()-50)*2);
				m_pWM->SetPan(m_nSoundEffectID, (CGame::GetInstance()->GetPanning()-50)*2);
			}
			if (CSGD_DirectInput::GetInstance()->KeyPressed(DIK_RIGHT)) 
			{
				CGame::GetInstance()->SetPanning(CGame::GetInstance()->GetPanning()+5);
				if(CGame::GetInstance()->GetPanning() > 100)
					CGame::GetInstance()->SetPanning(100);
				m_pWM->SetPan(m_nSoundLoopID, (CGame::GetInstance()->GetPanning()-50)*2);
				m_pWM->SetPan(m_nSoundEffectID, (CGame::GetInstance()->GetPanning()-50)*2);				
			}
			break;
		case 3:
			if (CSGD_DirectInput::GetInstance()->KeyPressed(DIK_RETURN))
				CGame::GetInstance()->ChangeState(CMainMenuState::GetInstance());
			break;
	}
	
	if(m_pDI->KeyPressed(DIK_DOWN))
	{
		CSGD_WaveManager::GetInstance()->Play(gunsound);
		CSGD_WaveManager::GetInstance()->SetVolume(gunsound, CGame::GetInstance()->GetSFXVolume());
		CSGD_WaveManager::GetInstance()->SetPan(gunsound, (CGame::GetInstance()->GetPanning()-50)*2);
		if(m_nCurrentMenuSelection<3)
			m_nCurrentMenuSelection++;
		else
			m_nCurrentMenuSelection = 0;
	}
	if(m_pDI->KeyPressed(DIK_UP))
	{
		CSGD_WaveManager::GetInstance()->Play(gunsound);
		CSGD_WaveManager::GetInstance()->SetVolume(gunsound, CGame::GetInstance()->GetSFXVolume());
		CSGD_WaveManager::GetInstance()->SetPan(gunsound, (CGame::GetInstance()->GetPanning()-50)*2);
		if(m_nCurrentMenuSelection>0)
			m_nCurrentMenuSelection--;
		else
			m_nCurrentMenuSelection = 3;
	}
	if(m_pDI->KeyPressed(DIK_RETURN) && (m_nCurrentMenuSelection == 3))
	{
		CGame::GetInstance()->ChangeState(CMainMenuState::GetInstance());
	}
	return true;
}
void COptionsState::Update()
{
}
void COptionsState::Render()
{
	m_pTM->Draw(m_nImageID,50, 180+(m_nCurrentMenuSelection*30),0.25f,0.25f);

	font.DrawString("OPTIONS MENU", 100, 30,2);
	font.DrawString("CHANGE SFX VOLUME", 100, 180);
	font.DrawString("CHANGE MUSIC VOLUME", 100, 210);
	font.DrawString("MUSIC PAN L:R", 100, 240);
	font.DrawString("BACK", 100, 270);


	char *line = new char[64];
	wsprintf(line, "<%i>", CGame::GetInstance()->GetSFXVolume());
	font.DrawString(line, 500, 180);
	wsprintf(line, "<%i>", CGame::GetInstance()->GetBGMVolume());
	font.DrawString(line, 500, 210);
	wsprintf(line, "L<%i>-R<%i>", 100-CGame::GetInstance()->GetPanning(), CGame::GetInstance()->GetPanning() );
	font.DrawString(line, 500, 240);
	delete[] line;
}
void COptionsState::Exit()
{
	m_pWM->Stop(m_nSoundLoopID);
	m_pWM->UnloadWave(m_nSoundEffectID);
	m_pWM->UnloadWave(m_nSoundLoopID);
	if(m_pTM){m_pTM->UnloadTexture(m_nImageID);}

	font.Unload();
}
