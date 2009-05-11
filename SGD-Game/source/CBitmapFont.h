////////////////////////////////////////////////////////
//   File Name	:	"CBitmapFont.h"
//
//   Author		:	Matthew Di Matteo (MD)
//
//   Purpose	:	To provide a way for the user to draw text
//					using bitmap fonts
////////////////////////////////////////////////////////
#pragma once

#include <windows.h>

class CBitmapFont
{
private:
	int charWidth;
	int charHeight;
	int numCols;
	char startChar;
	int imageID;

	////////////////////////////////////////////////////////////////////
	// Function: “cellalgorithm”
	//
	// Purpose: Algorithm function that finds location of the id you want
	//			on the sprite sheet.
	////////////////////////////////////////////////////////////////////
	RECT cellalgorithm(int id);

public:
	//////////////////////////////////////
	//	Function:	Mutators
	//
	//	Purpose	:	To set the specified type
	///////////////////////////////////////
	inline void SetID(int id) { imageID = id;}
	inline void SetcharWidth(int width) { charWidth = width;}
	inline void SetcharHeight(int height) { charHeight = height;}
	inline void SetNumCols(int num) {numCols = num;}
	
	///////////////////////////////////////////////////////////////////
	// Function: “Load”
	//
	// Purpose: Sets all data needed for the font.
	////////////////////////////////////////////////////////////////////
	void Load(const char* file, int width, int height, int numcols); 

	///////////////////////////////////////////////////////////////////
	// Function: “Unload”
	//
	// Purpose: Releases font texture from the texture manager.
	////////////////////////////////////////////////////////////////////
	void Unload(); 

	///////////////////////////////////////////////////////////////////
	// Function: “drawstring”
	//
	// Purpose: Draws the string passed in to the screen at the specified location.
	////////////////////////////////////////////////////////////////////
	void DrawString( char* szstring, int posX, int posY, float scale = 1, DWORD color = -1); 

	///////////////////////////////////////////////////////////////////
	// Function: “DrawChar”
	//
	// Purpose: Draws the character passed in to the screen at the specified location.
	////////////////////////////////////////////////////////////////////
	void DrawChar(char c, int posX, int posY, float scale = 1, DWORD dwcolor = NULL);

};