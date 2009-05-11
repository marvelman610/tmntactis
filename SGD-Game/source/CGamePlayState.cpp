////////////////////////////////////////////////////////////////////////////
//	File Name	:	"CGamePlayState.cpp"
//
//	Author		:	Matt DiMatteo (MD)
//
//	Purpose		:	The Gameplay state derived from the base state
//					to hold all data needed to play the game
////////////////////////////////////////////////////////////////////////////
#include "CGamePlayState.h"
#include "CBase.h"
#include "CSGD_EventSystem.h"
#include "CSGD_MessageSystem.h"
#include "SGD_Math.h"

CGamePlayState* CGamePlayState::GetInstance()
{
	static CGamePlayState instance;
	return &instance;
}
void CGamePlayState::Enter()
{
	m_pDI = CSGD_DirectInput::GetInstance();
	m_pD3D = CSGD_Direct3D::GetInstance();
	m_pTM = CSGD_TextureManager::GetInstance();
	m_pWM = CSGD_WaveManager::GetInstance();
	m_pDS = CSGD_DirectSound::GetInstance();
	m_pOF = CSGD_ObjectFactory<string, CBase>::GetInstance();
	m_pOM = CSGD_ObjectManager::GetInstance();

	m_pOF->RegisterClassType<CBase>("CBase");

	m_nBackground = m_pTM->LoadTexture("resource/graphics/MaD_Background.png");
	blank = m_pTM->LoadTexture("resource/graphics/MaD_PunisherBK.png");
	m_nCursor = m_pTM->LoadTexture("resource/graphics/MaD_HandGun.bmp",D3DCOLOR_XRGB(0,0,0));
	m_nGameOver = m_pTM->LoadTexture("resource/graphics/MaD_GameOver.png");

	
	m_nSoundID1 = m_pWM->LoadWave("resource/sound/MaD_BGM.wav");
	m_nCHEATID = m_pWM->LoadWave("resource/sound/MaD_coin.wav");
	fireball = m_pWM->LoadWave("resource/sound/MaD_fireball.wav");
	gunsound = m_pWM->LoadWave("resource/sound/MaD_Tankshot.wav");

	m_pWM->Play(m_nSoundID1, DSBPLAY_LOOPING);
	

	font.Load("resource/graphics/MaD_Font.png",20,20,20);
	
	m_nTime = 2000;

	timer = 0;
	count = 0;
	points = 0;
	pos = 0;
	boss = 0;
	pointscheat = 0;
	m_fElapsedTime = 0.0f;
	m_fGameTime = 0.0f;
	

	bIsPaused = false;
	gameover = false;

	m_nCurrentMenuSelection = 0;
	playercount = 1;
	numenemies = 2;
	lives = 3;

	CSGD_MessageSystem::GetInstance()->SendMsg(new CCreatePlayerMessage());
	
	CSGD_MessageSystem::GetInstance()->SendMsg(new CCreateEnemyMessage());
	CSGD_MessageSystem::GetInstance()->SendMsg(new CCreateEnemyMessage());

	enemiesKilled = 0;
	totalScore = 0;
	accuracy = 0;
	bulletsFired = 0;

	tally =0;

	m_dwPreviousTime = GetTickCount();
}
bool CGamePlayState::Input()
{
	if(boss == 5 && m_pDI->KeyPressed(DIK_RETURN))
	{
		boss == 10;
		CGame::GetInstance()->ChangeState(CHighScoreState::GetInstance());
	}

	if(m_pDI->KeyPressed(DIK_P))
	{
		bIsPaused = !bIsPaused;
		m_fElapsedTime = 0;
		m_dwPreviousTime = GetTickCount();
	}
	if(gameover)
	{
		if(m_pDI->KeyPressed(DIK_RETURN))
		{
			CGame::GetInstance()->ChangeState(CHighScoreState::GetInstance());
		}

	}
	if(bIsPaused)
	{
		if(m_pDI->KeyPressed(DIK_DOWN))
		{
			m_pWM->Play(gunsound);
				m_pWM->SetVolume(gunsound,CGame::GetInstance()->GetSFXVolume());
				m_pWM->SetPan(gunsound, (CGame::GetInstance()->GetPanning()-50)*2);
			
			if(m_nCurrentMenuSelection<1)
				m_nCurrentMenuSelection++;
			else
				m_nCurrentMenuSelection = 0;
		}
		if(m_pDI->KeyPressed(DIK_UP))
		{
			m_pWM->Play(gunsound);
			m_pWM->SetVolume(gunsound,CGame::GetInstance()->GetSFXVolume());
				m_pWM->SetPan(gunsound, (CGame::GetInstance()->GetPanning()-50)*2);

			if(m_nCurrentMenuSelection>0)
				m_nCurrentMenuSelection--;
			else
				m_nCurrentMenuSelection = 1;
		}	
		if(m_pDI->KeyPressed(DIK_RETURN))
		{
			switch(m_nCurrentMenuSelection)
			{
			case 0:
				bIsPaused = !bIsPaused;
				break;
			case 1:
				CGame::GetInstance()->ChangeState(CMainMenuState::GetInstance());
				break;
			}
		}
	}
	if(m_pDI->KeyPressed(DIK_E))
	{
		if(numenemies<10)
		{
			CSGD_MessageSystem::GetInstance()->SendMsg(new CCreateEnemyMessage());
			numenemies++;
		}

	}
	if(m_pDI->KeyPressed(DIK_R))
	{
		if(playercount == 0)
		{
			lives--;
			if(lives >= 0)
			{

				//CSGD_MessageSystem::GetInstance()->SendMsg(new CCreatePlayerMessage());
				playercount = 1;
			}
			else
				gameover = true;
		}

	}
	return true;
}
void CGamePlayState::Update()
{
	m_pWM->SetVolume(m_nSoundID1,CGame::GetInstance()->GetBGMVolume());
	m_pWM->SetPan(m_nSoundID1, (CGame::GetInstance()->GetPanning()-50)*2);

	cheat();
	
	if(boss == 4)
	{
  		accuracy = ((enemiesKilled*2) / bulletsFired)*100.0f;
		totalScore = points + ((lives+1)*1000) + (m_nTime*10) + (accuracy*100);

	}

		if(!bIsPaused)
		{
			
			DWORD dwStartTime = GetTickCount();
			m_fElapsedTime = (float)(dwStartTime - m_dwPreviousTime)/1000.0f;

			m_dwPreviousTime = dwStartTime;

			//if(boss !=3)
			m_fGameTime += m_fElapsedTime;
			timer += m_fElapsedTime;
			if(boss == 5)
				tally += m_fElapsedTime;
			if(boss==1)
			{
				CSGD_MessageSystem::GetInstance()->SendMsg(new CCreateBossMessage());
				boss = 3;
			}
			else if(timer > 6 && playercount != 0 && numenemies<10)
			{
				if(boss < 4)
				{
					CSGD_MessageSystem::GetInstance()->SendMsg(new CCreateEnemyMessage2());
					numenemies++;
				}
			
				if( boss == 0)
				{
					CSGD_MessageSystem::GetInstance()->SendMsg(new CCreateEnemyMessage());

					CSGD_MessageSystem::GetInstance()->SendMsg(new CCreateEnemyMessage());
					CSGD_MessageSystem::GetInstance()->SendMsg(new CCreateEnemyMessage());

					numenemies+=3;
				}
				timer = 0.0f;
			}
			
			if(FloatEquals(m_fGameTime ,1.0f))
			{
				//m_nScore += 10;
				m_fGameTime = 0.0f;


				if(boss < 4)
					m_nTime -= 10;
			}

			

			m_pOM->UpdateObjects(m_fElapsedTime);
			m_pOM->CheckCollisions();  


		
	}



}
void CGamePlayState::Render()
{


	m_pTM->Draw(m_nBackground,pos, 0);

	m_pOM->RenderObjects();
	m_pD3D->GetSprite()->Flush();

	char buffer[64];
	
	if(boss != 4)
	{
		
		sprintf_s(buffer, _countof(buffer), "PTS: %d", points);
		font.DrawString(buffer, 5,60,1.35f);
			
		sprintf_s(buffer, _countof(buffer), "TIME: %d", m_nTime);
		font.DrawString(buffer, 300,10,.9f);
	}

	if(lives < 0)
		font.DrawString("LIVES X0", 50, 580,1.1f);
	else
	{
		sprintf_s(buffer, _countof(buffer), "LIVES X%d", lives);
		font.DrawString(buffer, 50, 580,1.1f);
	}

	if(playercount == 0 && lives >= 0)
		font.DrawString("PRESS 'R' TO RESPAWN", 200, 200,1);
		

	if(bIsPaused)
	{
		
		RECT r = {0,0,800,3000};
		m_pTM->Draw(blank,0,0,1,1, &r,0,0,0, D3DCOLOR_ARGB(128,255,255,255));
		m_pTM->Draw(m_nCursor,300, 250+(m_nCurrentMenuSelection*25),0.25f,0.25f);
		
		font.DrawString("PAUSE", 300, 200,2);
		font.DrawString("RESUME", 350, 250);
		font.DrawString("EXIT", 350,275);
	
	}

	if(gameover)
	{
		bIsPaused = false;
		m_pTM->Draw(m_nGameOver,150,100);
		font.DrawString("YOU RAN OUT OF LIVES!", 200, 450,1);
		font.DrawString("PRESS ENTER TO CONTINUE...", 200, 500,1);


	}
	if(boss == 4)
	{
		font.DrawString("YOU WIN!", 100, 200,4);
		
	}
	if(boss == 5)
	{
		RECT r = {0,0,800,3000};
		m_pTM->Draw(blank,0,0,1,1, &r,0,0,0, D3DCOLOR_ARGB(85,255,255,255));
		font.DrawString("LEVEL COMPLETE",200,175,1.5f);

		char buffer[64];
		sprintf_s(buffer, _countof(buffer), "POINTS     : %d         -- %d", points, points);
		font.DrawString(buffer, 50,300,1);
		sprintf_s(buffer, _countof(buffer), "TIME LEFT  : %d   X 10   -- %d", m_nTime, (m_nTime*10));
		font.DrawString(buffer, 50,350,1);
		sprintf_s(buffer, _countof(buffer), "LIVES LEFT :    %d   X 1000 -- %d", lives+1, (lives+1)*1000);
		font.DrawString(buffer, 50,400,1);
		sprintf_s(buffer, _countof(buffer), "ACCURACY   : %.02f  X 100  -- %.0f", accuracy, (accuracy*100));
		font.DrawString(buffer, 50,450,1);
		
		if(tally >.05f)
		{
			tally = 0;
			if(count+100 < totalScore)
			{
				count += 100;
				m_pWM->Play(fireball);
			}
			else 
			{
				count += totalScore-count;
				//m_pWM->Play(fireball);
			}
				
			
		}
		sprintf_s(buffer, _countof(buffer), "TOTAL      :                 %d", count);
		font.DrawString(buffer, 50,500,1);
		font.DrawString("PRESS ENTER TO CONTINUE...", 320 ,580, 0.9f);

	}

}
void CGamePlayState::Exit()
{


	CSGD_EventSystem::GetInstance()->ClearEvents();
	CSGD_MessageSystem::GetInstance()->ClearMessages();

	if(m_pOM)
	{
		m_pOM->DeleteInstance();
	}

	font.Unload();

	m_pWM->UnloadWave(m_nSoundID1);
	m_pWM->UnloadWave(gunsound);
	m_pWM->UnloadWave(fireball);
	m_pWM->UnloadWave(m_nCHEATID);

	if(m_pTM)
	{
		m_pTM->UnloadTexture(m_nCursor);
		m_pTM->UnloadTexture(blank);
		m_pTM->UnloadTexture(m_nBackground);
		m_pTM->UnloadTexture(m_nGameOver);
	}
}
void CGamePlayState::cheat()
{
	switch(pointscheat)
	{
	case 0:
		if (m_pDI->KeyPressed(DIK_C))
			pointscheat++;
	else if (m_pDI->CheckBufferedKeysEx() != 0)
		pointscheat = 0;
		break;
	case 1:
	if (m_pDI->KeyPressed(DIK_H))
		pointscheat++;
	else if (m_pDI->CheckBufferedKeysEx() != 0)
		pointscheat = 0;
		break;
	case 2:
		if (m_pDI->KeyPressed(DIK_E))
		pointscheat++;
	else if (m_pDI->CheckBufferedKeysEx() != 0)
		pointscheat = 0;
		break;
	case 3:
	if (m_pDI->KeyPressed(DIK_A))
		pointscheat++;
	else if (m_pDI->CheckBufferedKeysEx() != 0)
		pointscheat = 0;
		break;
	case 4:
	if (m_pDI->KeyPressed(DIK_T))
		pointscheat++;
	else if (m_pDI->CheckBufferedKeysEx() != 0)
		pointscheat = 0;
		break;
	}
	if( pointscheat == 5)
	{
		m_pWM->SetVolume(m_nCHEATID,CGame::GetInstance()->GetSFXVolume());
		m_pWM->SetPan(m_nCHEATID, (CGame::GetInstance()->GetPanning()-50)*2);
		m_pWM->Play(m_nCHEATID);points += 8000;
		pointscheat = 0;
	}
}