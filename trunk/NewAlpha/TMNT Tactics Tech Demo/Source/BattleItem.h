////////////////////////////////////////////////////////
//   File Name	:	"BattleItem.h"
//
//   Author		:	Matthew Di Matteo (MD)
//
//   Purpose	:	Class derived from item that will act as
//					a special item to be used in battle 
////////////////////////////////////////////////////////

#ifndef BATTLEITEM_H
#define BATTLEITEM_H

#include "Base.h"

class CBattleItem : public CBase
{
private:
	//int m_nType;
	int m_nHeal;
	int m_nDamage;
	int m_nDamageRadius;

public:
	///////////////////////////////////////////////////////////////////
	//	Function:	"CBattleItem(Constructor)"
	///////////////////////////////////////////////////////////////////
	CBattleItem(void);

	///////////////////////////////////////////////////////////////////
	//	Function:	"~CBattleItem(Destructor)"
	///////////////////////////////////////////////////////////////////
	~CBattleItem(void);

	void SetDamage(int dmg) {m_nDamage = dmg;}
	void SetHeal(int heal) {m_nHeal = heal;}
	void SetRadius(int radius) {m_nDamageRadius = radius;}

	inline int GetDamage()  {return m_nDamage;}
	inline int GetHeal()	{return m_nHeal;}
	inline int GetRadius()  {return m_nDamageRadius;}

	///////////////////////////////////////////////////////////////////
	// Function: “Update”
	//
	// Purpose: Updates game objects based on time.
	////////////////////////////////////////////////////////////////////
	void Update(float fElapsedTime);
	
	////////////////////////////////////////////////////////////////////
	// Function: “Render”
	//
	// Purpose: Draws game objects to the screen.
	////////////////////////////////////////////////////////////////////
	void Render();
};

#endif