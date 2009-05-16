////////////////////////////////////////////////////////
//   File Name	:	"BitmapFont.cpp"
//
//   Author		:	Matthew Di Matteo (MD)
//
//   Purpose	:	To provide a way for the user to draw text
//					using bitmap fonts
////////////////////////////////////////////////////////

#include "BitmapFont.h"

CBitmapFont* CBitmapFont::GetInstance()
{
	static CBitmapFont instance;
	return &instance;
}

CBitmapFont::CBitmapFont(void)
{
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
void CBitmapFont::DrawString(char* szString, int posX, int posY, float scale, DWORD dwcolor) //if u want to to color text, make fonts white (desaturate
{
	int offsetx = posX;
	int offsety = posY;

	//find length of string 
	int len = (int)strlen(szString);

	//loop throught string
	for(int i = 0; i < len; i++)
	{
		//do cell algorithm for each charater
		char ch = szString[i]; //!!toUpper()
		if(ch == ' ')
		{
			offsetx += int(m_nCharWidth * scale);
			continue;
		}
		else if( ch == '\n')
		{
			offsetx = posX;
			offsety += int(m_nCharHeight );

			/*posY += m_nCharHeight;
			posX = posX - i*m_nCharWidth;*/

			continue;
		}
		//convert asccii char to id off sheet
		int id = (int)(ch - m_cStartChar);
		RECT rcell = CellAlgorithm(id);

		//CSGD_TextureManager::GetInstance()->Draw(imageID,offsetx,offsety,scale,scale,&rcell,0,0,0);
		offsetx += int(m_nCharWidth*scale);
	}
	
}

void CBitmapFont::DrawChar(char c, int posX, int posY, float scale, DWORD dwcolor) 
{
	int offsetx = posX;
	int offsety = posY;

		//do cell algorithm for each charater
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
	//convert asccii char to id off sheet
	int id = (int)(ch - m_cStartChar);
	RECT rcell = CellAlgorithm(id);

	//CSGD_TextureManager::GetInstance()->Draw(imageID,offsetx,offsety,scale,scale,&rcell,0,0,0);
	offsetx += int(m_nCharWidth*scale);

	
}