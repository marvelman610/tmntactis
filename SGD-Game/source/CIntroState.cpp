////////////////////////////////////////////////////////////////////////////
//	File Name	:	"CIntroState.cpp"
//
//	Author		:	Matt DiMatteo (MD)
//
//	Purpose		:	Provides an intro to the game
////////////////////////////////////////////////////////////////////////////
#include"CIntroState.h"
#include "CGame.h"

CIntroState* CIntroState::GetInstance()
{
	static CIntroState instance;
	return &instance;
}
void CIntroState::Enter() 
{
	m_pTM = CSGD_TextureManager::GetInstance();
	m_pWM = CSGD_WaveManager::GetInstance();
	m_pDI = CSGD_DirectInput::GetInstance();
	

	
	pfont.Load("resource/graphics/MaD_Font.png",20,20,20);

	m_nSoundEffectID = m_pWM->LoadWave("resource/sound/MaD_TankShot.wav");
	themeID = m_pWM->LoadWave("resource/sound/MaD_punisher.wav");

	buffer = "ON A SUNDAY IN CENTRAL PARK, FRANK CASTLE'S FAMILY\nWAS KILLED IN A RAIN OF MOB BULLETS.FROM THAT DAY\nFORTH, CASTLE WAS REBORN AS THE PUNISHER, A     \nRELENTLESS VIGILANTE DEDICATED TO ONE THING:     \nSENDING EVIL MEN TO THEIR GRAVES. THIS IS FRANK'S\nWAR...AND IN WAR, THERE ARE NO RULES.";
	
	j =0;	

	char buff[45];
	sprintf_s(buff, "resource/graphics/snap/MaD_snap (%d).png",j);
	id[j] = m_pTM->LoadTexture(buff);

	storyID = m_pTM->LoadTexture("resource/graphics/MaD_punisherstory.png");
	startID = m_pTM->LoadTexture("resource/graphics/MaD_punishertitle1.png");

	m_fElapsedTime = 0.0f;
	m_dwPreviousTime = GetTickCount();

	intro = true;
	timer = 0;

	time = 0;

	a=0;

	z = 0;
	scale = 0.78f;
	offset = 0;
	
	render = false;
	space = false;
	start = false;
	draw = true;

	m_pWM->Play(themeID, DSBPLAY_LOOPING);


}
bool CIntroState::Input()
{
	
	if(m_pDI->KeyPressed(DIK_SPACE))
	{
		if(render)
		{
			space = true;
		}
		else if(intro)
		{
			intro = false;
		}
		if(intro == false && a == 17)
		{
			render = false;
			j =100;
			timer = 2.3f;

			start = true;
		}
	}
	if(start)
	{
		if(m_pDI->KeyPressed(DIK_RETURN))
			CGame::GetInstance()->ChangeState(CMainMenuState::GetInstance());

	}

	return true;
}
void CIntroState::Update()
{
	m_pWM->SetVolume(themeID,CGame::GetInstance()->GetBGMVolume());
	m_pWM->SetPan(themeID, (CGame::GetInstance()->GetPanning()-50)*2);
	DWORD dwStartTime = GetTickCount();
	m_fElapsedTime = (float)(dwStartTime - m_dwPreviousTime)/1000.0f;

	m_dwPreviousTime = dwStartTime;

	if(j == 94)
	{
		timer += m_fElapsedTime;
		scale -= 0.0001f;
		offset+= 0.05f;
			
		if(timer > 2.2f)
		{
			intro = false;
			timer = 0;
		}	
	}
	if(intro == false)
	{
		timer += m_fElapsedTime;
		time += m_fElapsedTime;

		if(start == false)
			render = true;

		if(space == false)
		{
			if(time > 1.2f)
			{
				
				if(a <17)
					a++;
				time = 0;
			}
			if(timer > 0.15f)
			{
				timer = 0;
				if(z>287)
					z = 287;

				if(z<50)
					line1[z] = buffer[z];
				else if(z < 100)
					line2[z-50] = buffer[z];
				else if(z<150)
					line3[z-100] = buffer[z];
				else if(z<200)
					line4[z-150] = buffer[z];
				else if(z<250)
					line5[z-200] = buffer[z];
				else if(z<300)
					line6[z-250] = buffer[z];
				
				z++;
			
			}	
			
			
		}


	}

	timer+=m_fElapsedTime;
	if(timer > 2.2f)
	{
		timer=0;
		draw = !draw;
	}
	
	j++;
	if(j <95)
	{
		
		char buff[64];
		sprintf_s(buff, "resource/graphics/snap/MaD_snap (%d).png",j);
		id[j] = m_pTM->LoadTexture(buff);
	}


}

void CIntroState::Render()
{
	if(intro)
	{
		if(j == 88 || j == 91)
		{
			m_pWM->Play(m_nSoundEffectID);
			m_pWM->SetVolume(m_nSoundEffectID,CGame::GetInstance()->GetSFXVolume());
			m_pWM->SetPan(m_nSoundEffectID, (CGame::GetInstance()->GetPanning()-50)*2);
		}

		if(j > 94)
			j = 94;
		
		m_pTM->Draw(id[j],0+(int)offset,100+(int)offset,scale,scale);

	}
	if(j == 94)
		render = true;

	else if(render)
	{

		if(space == true)
			a = 17;

		RECT r = {0,0+(a*218),700,218+(a*218)};
		m_pTM->Draw(storyID,50,50,1,1,&r);
		pfont.DrawString("PRESS SPACE TO SKIP...",500,275,0.5f);

		if(space ==false)
		{
			for(int i = 0; i < 50; i++)
				pfont.DrawChar(line1[i], 25+(i*15),400,0.75f,0.75f);
			for(int i = 0; i < 50; i++)
				pfont.DrawChar(line2[i], 10+(i*15),420,0.75f,0.75f);
			for(int i = 0; i < 50; i++)
				pfont.DrawChar(line3[i], 10+(i*15),440,0.75f,0.75f);
			for(int i = 0; i < 50; i++)
				pfont.DrawChar(line4[i], 25+(i*15),460,0.75f,0.75f);
			for(int i = 0; i < 50; i++)
				pfont.DrawChar(line5[i], 25+(i*15),480,0.75f,0.75f);
			for(int i = 0; i < 50; i++)
				pfont.DrawChar(line6[i], 25+(i*15),500,0.75f,0.75f);
		}
		else
			pfont.DrawString(buffer,25,400,0.75f);

	}
	else if(start)
	{
		m_pTM->Draw(startID,0,0,1.6f,1.6f);
		
		pfont.DrawString("THE PUNISHER: WAR ZONE",100,50,1.5f);
		
		
		if(draw)
			pfont.DrawString("PRESS 'ENTER ' TO PROCEED",150,550); 

	}
}

void CIntroState::Exit()
{
	for(int i = 0; i <95; i++)
		m_pTM->UnloadTexture(id[i]);

	m_pTM->UnloadTexture(startID);
	m_pTM->UnloadTexture(storyID);

	m_pWM->UnloadWave(m_nSoundEffectID);
	m_pWM->UnloadWave(themeID);

	font.Unload();
	pfont.Unload();


}