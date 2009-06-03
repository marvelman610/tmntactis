////////////////////////////////////////////////////////
//   File Name	:	"Base.h"
//
//   Author		:	Matthew Di Matteo (MD)
//
//   Purpose	:	To encapsulate all shared data and functionality 
//					for our game objects
////////////////////////////////////////////////////////

#ifndef BASE_H
#define BASE_H

#include "IBaseInterface.h"


#include <windows.h>
#include "Animation.h"
#include <vector>
using std::vector;

enum TYPE{ OBJECT_WEAPON, OBJECT_BATTLEITEM, OBJECT_BOSS, OBJECT_NINJA, OBJECT_TURTLE};

class CBase : public IBaseInterface
{
private:
	int m_nImageID;
	int m_nState;

	char* m_szName;

	

	POINT m_ptAnchor;

	float m_fPosX;
	float m_fPosY;

	float m_fVelX;
	float m_fVelY;

	int m_nWidth;
	int m_nHeight;

	int m_nHealthMax;
	int m_nHealth;

	int m_nLevel;
	int m_nStrength;
	int m_nDefense;
	int m_nAccuracy;
	int m_nSpeed;
	int m_nRange;

	int m_nBasePoints;
	int m_nCurrPoints;

	int m_nExperience;
	
	//CTile m_pTileArray;


	CBase* m_pOwner;
	

protected:
	int m_nType;
	vector<CAnimation> animations;
	int m_ncurrFrame;
public:
	///////////////////////////////////////////////////////////////////
	//	Function:	"Base(Constructor)"
	///////////////////////////////////////////////////////////////////
	CBase();

	///////////////////////////////////////////////////////////////////
	//	Function:	"~Base(Destructor)"
	///////////////////////////////////////////////////////////////////
	virtual ~CBase() {}

	//////////////////////////////////////
	//	Function:	Accessors
	//
	//	Purpose	:	To get the specified type
	///////////////////////////////////////
	inline int GetImageID()const {return m_nImageID;}
	inline float GetPosX()const  {return m_fPosX;}
	inline float GetPosY()const  {return m_fPosY;}
	inline float GetVelX()const  {return m_fVelX;}
	inline float GetVelY()const  {return m_fVelY;}
	inline int GetWidth() const  {return m_nWidth;}
	inline int GetHeight() const {return m_nHeight;}
	inline int GetType()		 {return m_nType;}
	inline POINT GetAnchor()	 {return m_ptAnchor;}

	//////////////////////////////////////
	//	Function:	Mutators
	//
	//	Purpose	:	To set the specified type
	///////////////////////////////////////
	inline void SetImageID(int nImageID){m_nImageID = nImageID;}
	inline void SetAnchor(POINT pt) {m_ptAnchor = pt;}
	inline void SetPosX(float nPosX){m_fPosX = nPosX;}
	inline void SetPosY(float nPosY){m_fPosY = nPosY;}
	inline void SetVelX(float nVelX){m_fVelX = nVelX;}
	inline void SetVelY(float nVelY){m_fVelY = nVelY;}
	inline void SetWidth(int nWidth){m_nWidth = nWidth;}
	inline void SetHeight(int nHeight){m_nHeight = nHeight;}
	inline void SetBaseAP(int nAP){m_nBasePoints = nAP;}
	inline void SetCurrAP(int nAP){m_nCurrPoints = nAP;}
	inline void SetHealthMax(int nHealthMax){m_nHealthMax = nHealthMax;}
	inline void SetHealth(int nHealth){m_nHealth = nHealth;}
	inline void SetStrength(int nStrength){m_nStrength = nStrength;}
	inline void SetDefense(int nDefense){m_nDefense = nDefense;}
	inline void SetAccuracy(int nAccuracy){m_nAccuracy = nAccuracy;}
	inline void SetSpeed(int nSpeed){m_nSpeed = nSpeed;}
	inline void SetLevel(int nLevel){m_nLevel = nLevel;}
	inline void SetRange(int nRange){m_nRange = nRange;}
	inline void SetExperience(int nExperience){m_nExperience = nExperience;}


	void AddAnim(CAnimation& a)
	{
		animations.push_back(a);
	}

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

	////////////////////////////////////////////////////////////////////
	// Function: “Release”
	//
	// Purpose: Decrements the count varible. If the variable reaches zero,
	//			the pointer is no longer needed and is deleted.
	////////////////////////////////////////////////////////////////////
	void Release()
	{

	}

	////////////////////////////////////////////////////////////////////
	// Function: “Pathfind”
	//
	// Purpose: Will be used by all derived objects to do path finding.
	//
	////////////////////////////////////////////////////////////////////
	//vector<CTile> Pathfind(void);

};

#endif