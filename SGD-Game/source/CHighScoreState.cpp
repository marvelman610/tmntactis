#include"CHighScoreState.h"
#include<stdio.h>
#include"tinyxml/tinyxml.h"
#include "CGamePlayState.h"
#include"CGame.h"
#include"MainMenuState.h"



#define MAX 26

CHighScoreState* CHighScoreState::GetInstance()
{
	static CHighScoreState instance;
	return &instance;
}



void CHighScoreState::Enter()
{
	m_pDI = CSGD_DirectInput::GetInstance();

	font.Load("resource/graphics/MaD_Font.png",20,20,20);

	me.score1 = 0;
	scored = false;

	Load("resource/data/highscores.xml");

	curLetter =0;

	me.place = 0;

	m_fElapsedTime = 0;
	m_dwPreviousTime = GetTickCount();
	timer = 0;


	count =0;
}
bool CHighScoreState::Input()
{
	if(m_pDI->KeyPressed(DIK_BACK))
	{
		count--;
		curLetter = 0;
	}
	if(m_pDI->KeyPressed(DIK_UP))
	{
		curLetter--;
		if(curLetter <0)
			curLetter  = MAX;
	}
	if(m_pDI->KeyPressed(DIK_DOWN))
	{
		curLetter++;
		if(curLetter > MAX)
			curLetter  = 0;
	}
	if(m_pDI->KeyPressed(DIK_RETURN))
	{
		if(count <= 2)
		{
			count++;
			curLetter = 0;
		}
		if(count ==4 && me.score1 != 0)
		{
			Save("resource/data/highscores.xml");
			Load("resource/data/highscores.xml");
			CGamePlayState::GetInstance()->SetPoints(0);
			me.score1 = 0;
			CGame::GetInstance()->ChangeState(CMainMenuState::GetInstance());
		}

		if(scored == false)
			CGame::GetInstance()->ChangeState(CMainMenuState::GetInstance());
		}


	return true;
}
void CHighScoreState::Update()
{		

	DWORD dwStartTime = GetTickCount();
	m_fElapsedTime = (float)(dwStartTime - m_dwPreviousTime)/1000.0f;

	m_dwPreviousTime = dwStartTime;

	if(timer > 375)
	{
		timer =0;

	}
	timer += m_fElapsedTime;
	char buff[64];
	sprintf_s(buff, _countof(buff), "%d", me.score1);
	strcpy_s(me.score, _countof(me.score),buff);

	if(count == 3)
	{
		you = me;
		Highscore tmp;
		int n = 10;

		for (int j=0; j<n; j++)
		{
			if (hs[j].score1 <= me.score1) 
			{ 
				tmp = me;     
				me = hs[j];

				hs[j] = tmp;
			}
		}

		count++;
	}


}
void CHighScoreState::Render()
{
	font.DrawString("HIGH SCORE", 200, 100,2);
	if(count <=2 && me.score1 != 0)
	{
		char text;
		text = 'A'+curLetter;

		me.buffer[count] = text;

		if(timer > 0.25f)
		{
			timer++;
			for(int i = 0; i<3; i++)
				font.DrawChar(me.buffer[i],180+(i*20),500);			
		}
		for(int i = 0; i<10; i++)
				font.DrawChar(me.score[i],380+(i*20),500);


	}

	font.DrawString("1:  ",100,200);
	font.DrawString("2:  ",100,225);
	font.DrawString("3:  ",100,250);
	font.DrawString("4:  ",100,275);
	font.DrawString("5:  ",100,300);
	font.DrawString("6:  ",100,325);
	font.DrawString("7:  ",100,350);
	font.DrawString("8:  ",100,375);
	font.DrawString("9:  ",100,400);
	font.DrawString("10: ",100,425);

	

	for(int i = 0; i < 10; i++)
	{
		
		font.DrawString(hs[i].buffer,180,200+(i*25));
		font.DrawString(hs[i].score,380,200+(i*25));
	}

}
void CHighScoreState::Exit()
{
	font.Unload();


}
bool CHighScoreState::Load(const char* szXmlFileName)
{
	TiXmlDocument doc;

	if(!doc.LoadFile(szXmlFileName))
		return false;

	TiXmlElement* pRoot = doc.RootElement();
	TiXmlElement* pEntry = pRoot->FirstChildElement("entry");

	if(pEntry)
	{
		int i = 0;
		while(pEntry)
		{
			const char* pText = pEntry->GetText();

			strcpy_s(hs[i].buffer, _countof(hs[i].buffer), pText);
			const char* pScore = pEntry->Attribute("score");
			
			if(pScore)
			{
				strcpy_s(hs[i].score, _countof(hs[i].score),pScore);
				pEntry->Attribute("score", &hs[i].score1);
			}
			i++;
			pEntry = pEntry->NextSiblingElement("entry");
			hs[i].place = i+1;
		}
	}

	for(int i = 0; i < 10; i++)
	{
		if(CGamePlayState::GetInstance()->GetPoints() > hs[i].score1)
		{
			me.score1 = CGamePlayState::GetInstance()->Gettscore();
			scored = true;
		}
		if(me.score1 >0)
			break;
	}

	return true;

}
bool CHighScoreState::Save(const char* szXmlFileName)
{
	TiXmlDocument doc;

	TiXmlDeclaration* pDec = new TiXmlDeclaration("1.0", "utf-8", "yes");
	
	//put decl in doc
	doc.LinkEndChild(pDec);

	TiXmlElement* pRoot = new TiXmlElement("highscores");
	doc.LinkEndChild(pRoot);

	for(int i = 0; i< 10; i++)
	{
		TiXmlElement* pEntry = new TiXmlElement("entry");
		pEntry->SetAttribute("score", hs[i].score1);
		TiXmlText *pText = new TiXmlText(hs[i].buffer);
		pEntry->LinkEndChild(pText);

		pRoot->LinkEndChild(pEntry);

	}

	if(!doc.SaveFile(szXmlFileName))
		return false;

	return true;
}