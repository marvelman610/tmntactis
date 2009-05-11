////////////////////////////////////////////////////////
//   File Name	:	"CBitmapFont.cpp"
//
//   Author		:	Matthew Di Matteo (MD)
//
//   Purpose	:	To provide a way for the user to draw text
//					using bitmap fonts
////////////////////////////////////////////////////////
#include"CBitmapFont.h"
#include "SGD Wrappers/CSGD_TextureManager.h"

RECT CBitmapFont::cellalgorithm(int id)
{
	RECT rcell;
	rcell.left = (id % numCols) * charWidth;
	rcell.top = (id / numCols) * charHeight;
	rcell.right = rcell.left + charWidth;
	rcell.bottom = rcell.top + charHeight;

	return rcell;
}
void CBitmapFont::Load(const char* file, int width, int height, int numcols)
{
	SetID(CSGD_TextureManager::GetInstance()->LoadTexture(file));
	SetcharWidth(width);
	SetcharHeight(height);
	SetNumCols(numcols);
	startChar = 32;
}
void CBitmapFont::Unload()
{
	CSGD_TextureManager::GetInstance()->UnloadTexture(imageID);	
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
			offsetx += int(charWidth * scale);
			continue;
		}
		else if( ch == '\n')
		{
			offsetx = posX;
			offsety += int(charHeight );

			/*posY += charHeight;
			posX = posX - i*charWidth;*/

			continue;
		}
		//convert asccii char to id off sheet
		int id = (int)(ch - startChar);
		RECT rcell = cellalgorithm(id);

		CSGD_TextureManager::GetInstance()->Draw(imageID,offsetx,offsety,scale,scale,&rcell,0,0,0);
		offsetx += int(charWidth*scale);
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
		offsetx += int(charWidth * scale);
		return;
	}
	else if( ch == '\n')
	{
		offsetx = posX;
		offsety += int(charHeight * scale);
		return;

	}
	//convert asccii char to id off sheet
	int id = (int)(ch - startChar);
	RECT rcell = cellalgorithm(id);

	CSGD_TextureManager::GetInstance()->Draw(imageID,offsetx,offsety,scale,scale,&rcell,0,0,0);
	offsetx += int(charWidth*scale);

	
}