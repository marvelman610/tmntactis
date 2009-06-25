////////////////////////////////////////////////////////
//   File Name	:	"BitmapFont.cpp"
//
//   Author		:	Matthew Di Matteo (MD)
//
//   Purpose	:	To provide a way for the user to draw text
//					using bitmap fonts
////////////////////////////////////////////////////////

#include "BitmapFont.h"
#include "Assets.h"
#include "CSGD_TextureManager.h"

CBitmapFont* CBitmapFont::GetInstance()
{
	static CBitmapFont instance;
	return &instance;
}

CBitmapFont::CBitmapFont(void)
{
	m_nNumCols = 10;
	m_cStartChar = 32;
	m_nCharHeight = 32;
	m_nCharWidth = 32;
	m_nImageID = CSGD_TextureManager::GetInstance()->LoadTexture("Resources/Images/VG_BitmapFont.png", D3DCOLOR_XRGB(0,0,0));
}

CBitmapFont::~CBitmapFont(void)
{
}
RECT CBitmapFont::CellAlgorithm(int ID)
{
	RECT rcell;
	rcell.left = (ID % m_nNumCols) * m_nCharWidth;
	rcell.top = (ID / m_nNumCols) * m_nCharHeight;
	rcell.right = rcell.left + m_nCharWidth;
	rcell.bottom = rcell.top + m_nCharHeight;

	return rcell;
}
void CBitmapFont::Load()
{

}
void CBitmapFont::DrawString(const char* szString, int posX, int posY, float posZ, float scale, DWORD dwcolor) //if u want to color text, make fonts white (desaturate)
{
	int offsetx = posX;
	int offsety = posY;

	//find length of string 
	int len = (int)strlen(szString);

	//loop through string
	for(int i = 0; i < len; i++)
	{
		//do cell algorithm for each character
		char ch = szString[i];
		ch = toupper(ch);
		if(ch == ' ')
		{
			offsetx += int(m_nCharWidth * scale);
			continue;
		}
		else if( ch == '\n')
		{
			offsetx = posX;
			offsety += int(m_nCharHeight );
			continue;
		}
		//convert ascii char to id off sheet
		int id = (int)(ch - m_cStartChar);
		RECT rcell = CellAlgorithm(id);

		CSGD_TextureManager::GetInstance()->DrawWithZSort(m_nImageID,offsetx,offsety, posZ, scale, scale, &rcell,0,0,0, dwcolor);
		offsetx += int(m_nCharWidth*scale+5);
	}
}

void CBitmapFont::DrawChar(char c, int posX, int posY, float scale, DWORD dwcolor) 
{
	int offsetx = posX;
	int offsety = posY;

		//do cell algorithm for each character
	char ch = c; //!!toUpper()
	if(ch == ' ')
	{
		offsetx += int(m_nCharWidth * scale);
		return;
	}
	else if( ch == '\n')
	{
		offsetx = posX;
		offsety += int(m_nCharHeight * scale);
		return;

	}
	//convert ascii char to id off sheet
	int id = (int)(ch - m_cStartChar);
	RECT rcell = CellAlgorithm(id);

	//CSGD_TextureManager::GetInstance()->Draw(imageID,offsetx,offsety,scale,scale,&rcell,0,0,0);
	offsetx += int(m_nCharWidth*scale);
}


void CBitmapFont::DrawStringAutoCenter (const char* szString, int ScreenWidth, int yPos, float zPos, float fScale, DWORD dwColor)
{
	m_cStartChar = 32;
	CSGD_TextureManager* pTM = CSGD_TextureManager::GetInstance ();
	int nOffsetX = (ScreenWidth >> 1);
	int nOffsetY = yPos;
	char ch;

	//	find length of string
	int len = strlen(szString);
	nOffsetX -= ((int)(len * (m_nCharWidth + 10) * fScale) >> 1);
	// 	loop through string	
	for (int i = 0; i < len; i++)
	{
		//	do cell algorithm for each character
		ch = szString[i];
		ch = toupper(ch);
		if (ch == ' ')
		{
			nOffsetX += (int)((m_nCharWidth + 10) * fScale);
			continue;
		}
		else if(ch == '\n')
		{
			//nPosX = nPosX;
			yPos += m_nCharHeight + 15;	
			continue;
		}
		//	convert ascii value into an id off the sheet
		int nID = (int)(ch  - m_cStartChar);
		RECT rCell = CellAlgorithm(nID);

		pTM->DrawWithZSort(m_nImageID, nOffsetX, nOffsetY, zPos, fScale, fScale, &rCell, 0.0f, 0.0f, 0.0f, dwColor);

		nOffsetX += (int)((m_nCharWidth + 10) * fScale);
	}
}

void CBitmapFont::DrawStringAutoCenterBox (const char* szString, int width, int startX, int yPos, float zPos, float fScale, DWORD dwColor)
{
	m_cStartChar = 32;
	CSGD_TextureManager* pTM = CSGD_TextureManager::GetInstance ();
	int nOffsetX = startX + (width >> 1);
	int nOffsetY = yPos;
	char ch;

	//	find length of string
	int len = strlen(szString);
	nOffsetX -= ((int)(len * (m_nCharWidth + 10) * fScale) >> 1);
	// 	loop through string	
	for (int i = 0; i < len; i++)
	{
		//	do cell algorithm for each character
		ch = szString[i];
		ch = toupper(ch);
		if (ch == ' ')
		{
			nOffsetX += (int)((m_nCharWidth + 10) * fScale);
			continue;
		}
		else if(ch == '\n')
		{
			//nPosX = nPosX;
			yPos += m_nCharHeight + 15;	
			continue;
		}
		//	convert ascii value into an id off the sheet
		int nID = (int)(ch  - m_cStartChar);
		RECT rCell = CellAlgorithm(nID);

		pTM->DrawWithZSort(m_nImageID, nOffsetX, nOffsetY, zPos, fScale, fScale, &rCell, 0.0f, 0.0f, 0.0f, dwColor);

		nOffsetX += (int)((m_nCharWidth + 10) * fScale);
	}
}

void CBitmapFont::Reset() // back to original BM font
{
	m_nCharWidth = 32;
	m_nCharHeight = 32;
	m_nNumCols = 10;
	m_nImageID = CAssets::GetInstance()->aBitmapFontID;
}
void CBitmapFont::ChangeBMFont(int imageID, int width, int height, int cols)
{
	m_nCharWidth = width;
	m_nCharHeight = height;
	m_nNumCols = cols;
	m_nImageID = imageID;
	// set new start char?
}