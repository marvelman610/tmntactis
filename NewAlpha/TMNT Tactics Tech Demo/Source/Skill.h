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
class CSkill;
class CParticleSystem;
class CBase;
class CSGD_DirectInput;
class CSGD_Direct3D;
class CSGD_TextureManager;
class CBox;

enum {SKILL_DAMAGE, SKILL_HEAL, SKILL_CREATE, SKILL_DEFENSE, };	//types
enum {LEFT, RIGHT, UP, DOWN, INCORRECT = -1};
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
typedef void (*rendPtr)( CSkill*, CParticleSystem* );
typedef void (*updPtr)(float, CSkill*, CParticleSystem* );

class CSkill
{
private:
	//int m_nSkillID;
	 string m_strName;		// The name of the skill
	 int	m_nType;		// what type of skill: e.g. Damage, Healing, Creation
	 int	m_nDamage;
	 int	m_nRange;
	 int	m_nSkillCost;
	 int	m_nCurrAmountSuccessful;	// quick-time event, how many have they got right so far?
	 int	m_nMaxCombinationAmount;	// how many buttons need to be pressed for a perfect?
	 int	m_nSkillID;					// enum for which skill this is
	 int*	m_pCombination;				// the combination of random directions
	 bool	m_bComplete;				// is execution of the skill complete?
	 

	 float m_fTimer;
	 float m_fDuration;


	 CPlayer*			m_pPlayer;
	 CBattleMap*		m_pBattleMap;
	 CSGD_DirectInput*  m_pDI;
	 CSGD_Direct3D*		m_pD3D;
	 CSGD_TextureManager* m_pTM;

	rendPtr m_pRenderPtr;			// the function pointer to the appropriate Render()
	updPtr  m_pUpdatePtr;			// the function pointer to the appropriate Update()

public:
	//////////////////////////////////////////////////////////////////////////
	//	Function	:	Update
	//
	//	Purpose		:	Update the main menu, calls the render function as well
	//////////////////////////////////////////////////////////////////////////
	void Update(float fElapsedTime, CSkill* skill, CParticleSystem* ps);

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
	string GetSkillName()			{return m_strName;}
	int GetSkillCost()				{return m_nSkillCost;}
	int GetDmg()					{return m_nDamage;}
	int GetRange()					{return m_nRange;}
	int GetCurrAmtSuccessful()		{return m_nCurrAmountSuccessful;}
	int GetID()						{return m_nSkillID;}
	void SetCurrAmtSuccessful(int amt){m_nCurrAmountSuccessful = amt;}
	int GetMaxCombAmt()				{return m_nMaxCombinationAmount;}
	void SetComb(int num);
	int* GetComb()					{return m_pCombination;}
	void ClearComb()				{if (m_pCombination) delete[] m_pCombination; m_pCombination = NULL;}
	float GetDur()					{return m_fDuration;}
	float GetTimer()				{return m_fTimer;}
	CPlayer* GetPlayer()			{return m_pPlayer;}
	CBattleMap* GetBattleMap()		{return m_pBattleMap;}

	bool IsComplete()				{return m_bComplete;}
	void IsComplete(bool bComplete)	{m_bComplete = bComplete;}

	~CSkill();
	CSkill();
	CSkill(string name, int type, int skillID, int dmg, int range, int cost, int combAmt, float duration);
};


#endif