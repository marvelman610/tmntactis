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
#include "Tile.h"

#include <vector>
using std::vector;

enum TYPE{ OBJECT_WEAPON, OBJECT_BATTLEITEM, OBJECT_BOSS, OBJECT_NINJA, OBJECT_TURTLE};

class CAnimation;
class CBase : public IBaseInterface
{
private:
	int m_nImageID;
	int m_nState;

	char* m_szName;

	//vector<CAnimation> animations;

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
	
	CTile m_pTileArray;


protected:
	int m_nType;
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
	inline float GetPosX()const {return m_fPosX;}
	inline float GetPosY()const {return m_fPosY;}
	inline float GetVelX()const {return m_fVelX;}
	inline float GetVelY()const {return m_fVelY;}
	inline int GetWidth() const {return m_nWidth;}
	inline int GetHeight() const {return m_nHeight;}
	inline int GetType(){return m_nType;}

	//////////////////////////////////////
	//	Function:	Mutators
	//
	//	Purpose	:	To set the specified type
	///////////////////////////////////////
	inline void SetImageID(int nImageID){m_nImageID = nImageID;}
	inline void SetPosX(float nPosX){m_fPosX = nPosX;}
	inline void SetPosY(float nPosY){m_fPosY = nPosY;}
	inline void SetVelX(float nVelX){m_fVelX = nVelX;}
	inline void SetVelY(float nVelY){m_fVelY = nVelY;}
	inline void SetWidth(int nWidth){m_nWidth = nWidth;}
	inline void SetHeight(int nHeight){m_nHeight = nHeight;}

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
	vector<CTile> Pathfind(void);

};

#endif