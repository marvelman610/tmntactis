////////////////////////////////////////////////////////
//   File Name	:	"Skill.h"
//
//   Author		:	Matthew Di Matteo (MD)
//
//   Purpose	:	To provide the player with a skill 
//					to deal more damage to the opponent
////////////////////////////////////////////////////////

#ifndef SKILL_H
#define SKILL_H

class CSkill
{
private:
	char* m_szName;
	int m_nType;
	int m_nDamage;
	int m_nRange;
	int	m_nCurrAmountSuccessful;
	int m_nMaxCombinationAmount;
};

#endif