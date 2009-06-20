////////////////////////////////////////////////////////
//   File Name	:	"Turtle.h"
//
//   Author		:	Matthew Di Matteo (MD)
//
//   Purpose	:	To provide a game object to be used by the player
//					as a part of their party in the game
////////////////////////////////////////////////////////

#ifndef TURTLE_H
#define TURTLE_H

#include "Base.h"
#include "Skill.h"
#include <vector>
using std::vector;

class CSkill;

class CTurtle : public CBase
{
private:
	vector<CSkill> m_vInactiveSkills;
	vector<CSkill> m_vActiveSkills;
	int m_nCurrSelectedSkill;
	bool m_bIsAlive;
	bool m_bIsActive; // bool to check for hud 

	vector<CBase> m_vWeapons;
	int m_nCurrWeapon;

public:
	///////////////////////////////////////////////////////////////////
	//	Function:	"CTurtle(Constructor)"
	///////////////////////////////////////////////////////////////////
	CTurtle(void);

	///////////////////////////////////////////////////////////////////
	//	Function:	"~CTurtle(Destructor)"
	///////////////////////////////////////////////////////////////////
	~CTurtle(void);

	////////////////////////////////////////////////////////////////////
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

	////////////////////////////////////////////////////////////////////
	// Function: “SetSkillsInactive”
	//
	// Purpose: Populates the vector of inactive skills.
	////////////////////////////////////////////////////////////////////
	void SetSkillsInactive(vector<CSkill> &inactiveSkills) {m_vInactiveSkills = inactiveSkills;}

	////////////////////////////////////////////////////////////////////
	// Function: “SetSkillsActive”
	//
	// Purpose: Populates the vector of active skills.
	////////////////////////////////////////////////////////////////////
	void SetSkillsActive(vector<CSkill> &activeSkills) {m_vActiveSkills = activeSkills;}

	////////////////////////////////////////////////////////////////////
	// Function: “SetAttribute”
	//
	// Purpose: Sets all of the turtle's attributes.
	////////////////////////////////////////////////////////////////////
	void SetAttributes(int ap = 10,int hp = 100,int strength = 15,int defense = 5,int accuracy = 5,
					   int speed = 5,int level = 1, int experience = 0,int range = 1);
	/////////////////////////////////////////////////////////////////////////////
	//Function:	Accessors / Mutators
	//////////////////////////////////////////////////////////////////////////////
	inline bool GetActive(void) { return m_bIsActive; }
	inline vector<CSkill>* GetSkills()  { return &m_vActiveSkills;}
	inline vector<CSkill>* GetInactiveSkills() {return &m_vInactiveSkills;}
	inline int GetNumSkills(void) {return m_vActiveSkills.size();}
	inline int GetCurrSelectedSkillIndex() {return m_nCurrSelectedSkill;}
	inline CSkill* GetCurrSelectedSkill() {return &m_vActiveSkills[m_nCurrSelectedSkill];}
	inline void SetSelectedSkill(int skill)	{m_nCurrSelectedSkill = skill;}
	inline bool GetAlive(void) { return m_bIsAlive; }
	inline void SetAlive(bool set) { m_bIsAlive = set; }

	inline int GetCurrWeaponIndex() {return m_nCurrWeapon;}
	inline void SetCurrWeaponIndex(int i) {m_nCurrWeapon = i;}

	vector<CBase>* GetWeapons()			{ return &m_vWeapons;		}
	void SetWeapons(vector<CBase>& weapons) {m_vWeapons = weapons;}
	void AddWeapon(CBase a)				{ m_vWeapons.push_back(a);	}
};

#endif