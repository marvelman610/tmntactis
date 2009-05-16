////////////////////////////////////////////////////////
//   File Name	:	"Item.h"
//
//   Author		:	Matthew Di Matteo (MD)
//
//   Purpose	:	To create a base class for different types
//					of items to be used inside the game
//					
////////////////////////////////////////////////////////

#ifndef ITEM_H
#define ITEM_H

#include "IBaseInterface.h"
#include "Base.h" 

class CItem : public IBaseInterface
{
private:
	CBase* m_pOwner;

	int m_nImageID;
	
	float m_fPosX;
	float m_fPosY;

	float m_nSourceX;
	float m_nSourceY;

	int m_nWidth;
	int m_nHeight;

	int m_nType;

public:
	CItem(void);
	virtual ~CItem(void){}
	////////////////////////////////////////////////////////////////////
	// Function: “Update”
	//
	// Purpose: Updates game objects based on time.
	////////////////////////////////////////////////////////////////////
	virtual void Update(float fElapsedTime);
	
	////////////////////////////////////////////////////////////////////
	// Function: “Render”
	//
	// Purpose: Draws game objects to the screen.
	////////////////////////////////////////////////////////////////////
	virtual void Render();
};

#endif