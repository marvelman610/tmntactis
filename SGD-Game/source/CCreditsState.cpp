////////////////////////////////////////////////////////////////////////////
//	File Name	:	"CCreditsState.cpp"
//
//	Author		:	Matt DiMatteo (MD)
//
//	Purpose		:	The Credits state derived from the base state
//					to hold all data needed to display the credits.
////////////////////////////////////////////////////////////////////////////
#include"CCreditsState.h"
#include"SGD Wrappers/CSGD_WaveManager.h"
#include"SGD Wrappers/CSGD_DirectInput.h"
#include"CGame.h"

CCreditsState* CCreditsState::GetInstance()
{
	static CCreditsState instance;
	return &instance;
}
void CCreditsState::Enter()
{
	music = CSGD_WaveManager::GetInstance()->LoadWave("resource/sound/MaD_punisher.wav");
	
	for(int i = 0; i < 6; i++)
	{
		credit[i].posX = 150;
		credit[i].posY = 800;
		credit[i].velY = -0.1f;
		credit[i].name = "MATTHEW DIMATTEO";
	}

	credit[0].title = "LEAD DESIGNER";
	credit[1].title = "LEAD PROGRAMMER";
	credit[2].title = "CREATIVE DIRECTOR";
	credit[3].title = "CHARACTER CREATORS";
	credit[3].name = "GERRY CONWAY\n\nJOHN ROMITA SR.\n\nROSS ANDRU\n\nMARVEL COMICS";
	credit[4].title = "SPECIAL THANKS";
	credit[4].name = "DAVID BROWN\n\nJOHN, ROBERT, AND RONALD";
	credit[5].title = "SPRITES RIPPED FROM:";
	credit[5].name = "HTTP://SPRITE-\n\n     DATABASE.NET";


	font.Load("resource/graphics/MaD_Font.png",20,20,20);

	/*m_fElapsedTime = 0.0f;
	m_dwPreviousTime = GetTickCount();*/

	c.title = " CREDITS";
	c.name = "---------";
	c.posX = 200;
	c.posY = 500;
	c.velY = -0.1f;

	CSGD_WaveManager::GetInstance()->Play(music);

}
bool CCreditsState::Input()
{
	if(CSGD_DirectInput::GetInstance()->KeyPressed(DIK_RETURN))
	{
		CGame::GetInstance()->ChangeState(CMainMenuState::GetInstance());
	}


	return true;
}
void CCreditsState::Update()
{
	CSGD_WaveManager::GetInstance()->SetVolume(music, CGame::GetInstance()->GetBGMVolume());
	CSGD_WaveManager::GetInstance()->SetPan(music, (CGame::GetInstance()->GetPanning()-50)*2);

	c.posY += c.velY;
	for(int i = 0; i < 6; i++)
	{
		credit[i].posY += credit[i].velY;
	}

	if(credit[5].posY < -80)
		CGame::GetInstance()->ChangeState(CMainMenuState::GetInstance());
		
}
void CCreditsState::Render()
{

	font.DrawString(c.title,c.posX,c.posY-80,2.5f);
	font.DrawString(c.name,c.posX,c.posY,2.5f);
	for(int i = 0; i < 6; i++)
	{
		font.DrawString(credit[i].title,100,credit[i].posY+(i*450)-65,1.8f);
		font.DrawString(credit[i].name,credit[i].posX,credit[i].posY+(i*450),1.3f);
	}

}
void CCreditsState::Exit()
{
	CSGD_WaveManager::GetInstance()->UnloadWave(music);


}