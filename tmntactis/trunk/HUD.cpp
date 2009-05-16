////////////////////////////////////////////////////////
//   File Name	:	"HUD.cpp"
//
//   Author		:	Matthew Di Matteo (MD)
//
//   Purpose	:	This class encompasses all menus to 
//					appear on the screen during gameplay.
////////////////////////////////////////////////////////

#include "HUD.h"
#include "BitmapFont.h"
#include "Player.h"

CHUD::CHUD(void)
{
	m_pPlayer = CPlayer::GetInstance();
	m_pBitmapFont = CBitmapFont::GetInstance();
}

CHUD::~CHUD(void)
{
}
CHUD* CHUD::GetInstance()
{
	static CHUD instance;
	return &instance;
}
void CHUD::Update(float fElapsedTime)
{

}
void CHUD::Render()
{

}
