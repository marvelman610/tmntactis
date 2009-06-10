////////////////////////////////////////////////////////
//   File Name	:	"Skill.h"
//
//   Author		:	Matthew Di Matteo (MD), Ramon Johannessen (RJ)
//
//   Purpose	:	To provide the player with skills 
//					to deal more damage to the opponent,
//					handles the rendering and updating of
//					each specific skill
////////////////////////////////////////////////////////

#ifndef SKILL_H
#define SKILL_H

#include "tinyxml/tinyxml.h"
#include "CSGD_TextureManager.h"
#include <string>
using std::string;

class CBattleMap;
class CPlayer;

enum {SKILL_DAMAGE, SKILL_HEAL, SKILL_CREATE, SKILL_DEFENSE, };	//types
enum eSkillIDs // IDs used to set the appropriate function pointers when info is loaded in
{
SWORD_SPIN				= 0,
SWORD_JAB				= 1,
FLIP_BACKSTAB			= 2,
COUNTER_ATTACK			= 3,
KNOCK_BACK				= 4,
STAFF_SPIN				= 5,
STAFF_UPPERCUT			= 6,
BACKFLIP_AWAY			= 7,
CREATE_BOMB				= 8,
FLYING_SAI_STAB			= 9,
SAI_FURY				= 10,
NUNCHUCK_SKULL_SPLITTER = 11,
NUNCHUCK_SPIN			= 12,
ROLL_AWAY				= 13,

};
// member function pointer typedefs
typedef void (*rendPtr)();
typedef void (*updPtr)(float);

class CSkill
{
private:
	 string m_strName;		// The name of the skill
	 int m_nType;		// what type of skill: e.g. Damage, Healing, Creation
	//int m_nSkillID;
	 bool m_bComplete;
	 int m_nDamage;
	 int m_nRange;
	 int m_nSkillCost;
	 int	m_nCurrAmountSuccessful;	// quick-time event, how many have they got right so far?
	 int m_nMaxCombinationAmount;	// how many buttons need to be pressed for a perfect?

	 float m_fTimer;

	 CPlayer*	m_pPlayer;
	 CBattleMap* m_pBattleMap;

	rendPtr m_pRenderPtr;			// the function pointer to the appropriate Render()
	updPtr  m_pUpdatePtr;			// the function pointer to the appropriate Update()

public:
	//////////////////////////////////////////////////////////////////////////
	//	Function	:	Update
	//
	//	Purpose		:	Update the main menu
	//////////////////////////////////////////////////////////////////////////
	void Update(float fElapsedTime);
	//////////////////////////////////////////////////////////////////////////
	//	Function	:	Render
	//
	//	Purpose		:	Render the main menu
	//////////////////////////////////////////////////////////////////////////
	void Render();

	//////////////////////////////////////////////////////////////////////////
	//	Function	:	SetFunctions
	//
	//	Purpose		:	Sets the render and update function pointer to the 
	//					appropriate functions for the given skill ID
	//////////////////////////////////////////////////////////////////////////
	void SetFunctions(int skillID);

	//////////////////////////////////////////////////////////////////////////
	//	Accessors / Mutators
	//////////////////////////////////////////////////////////////////////////
	string GetSkillName()	{return m_strName;}
	int GetSkillCost()		{return m_nSkillCost;}
	int GetDmg()				{return m_nDamage;}
	int GetRange()			{return m_nRange;}
	int GetCurrAmtSuccessful(){return m_nCurrAmountSuccessful;}
	int GetMaxCombAmt()		{return m_nMaxCombinationAmount;}
	CPlayer* GetPlayer()		{return m_pPlayer;}
	CBattleMap* GetBattleMap(){return m_pBattleMap;}

	bool IsComplete()				{return m_bComplete;}
	void IsComplete(bool bComplete)	{m_bComplete = bComplete;}

// 	void RenderSwordJab();
// 	void UpdateSwordJab(float elapsedTime);

	~CSkill();
	CSkill();
	CSkill(string name, int type, int skillID, int dmg, int range, int cost, int combAmt);
};


#endif