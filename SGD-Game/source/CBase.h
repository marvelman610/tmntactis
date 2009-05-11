////////////////////////////////////////////////////////
//   File Name	:	"CBase.h"
//
//   Author		:	Matthew Di Matteo (MD)
//
//   Purpose	:	To encapsulate all shared data and functionality 
//					for our game objects
////////////////////////////////////////////////////////
#pragma once

#include <windows.h>
#include "IBaseInterface.h"

enum TYPE{ OBJECT_PLAYER, OBJECT_Enemy, OBJECT_BULLET, OBJECT_WEB, 
			OBJECT_HEALTH, OBJECT_BOSS, OBJECT_POINTS};

class CBase : public IBaseInterface
{
private:
	unsigned int m_uiRefCount;
	int m_nImageID;

	float m_nPosX;
	float m_nPosY;

	float m_nVelX;
	float m_nVelY;

	int m_nWidth;
	int m_nHeight;

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
	virtual ~CBase();
	//////////////////////////////////////
	//	Function:	Accessors
	//
	//	Purpose	:	To get the specified type
	///////////////////////////////////////
	inline int GetImageID()const {return m_nImageID;}
	inline float GetPosX()const {return m_nPosX;}
	inline float GetPosY()const {return m_nPosY;}
	inline float GetVelX()const {return m_nVelX;}
	inline float GetVelY()const {return m_nVelY;}
	inline int GetWidth() const {return m_nWidth;}
	inline int GetHeight() const {return m_nHeight;}
	inline int GetType(){return m_nType;}

	//////////////////////////////////////
	//	Function:	Mutators
	//
	//	Purpose	:	To set the specified type
	///////////////////////////////////////
	inline void SetImageID(int nImageID){m_nImageID = nImageID;}
	inline void SetPosX(float nPosX){m_nPosX = nPosX;}
	inline void SetPosY(float nPosY){m_nPosY = nPosY;}
	inline void SetVelX(float nVelX){m_nVelX = nVelX;}
	inline void SetVelY(float nVelY){m_nVelY = nVelY;}
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
	// Function: “AddRef”
	//
	// Purpose: Increments the ref count variable. The variable lets you 
	//			know how many pointers are in use.
	////////////////////////////////////////////////////////////////////
	void AddRef()
	{
		m_uiRefCount++;
	}

	////////////////////////////////////////////////////////////////////
	// Function: “Release”
	//
	// Purpose: Decrements the count varible. If the variable reaches zero,
	//			the pointer is no longer needed and is deleted.
	////////////////////////////////////////////////////////////////////
	void Release()
	{
		m_uiRefCount--;
		if(m_uiRefCount <= 0)
			delete this;

	}

	////////////////////////////////////////////////////////////////////
	// Function: “GetRect”
	//
	// Purpose: Returns a rectangle of the object used for detecting collision.
	////////////////////////////////////////////////////////////////////
	RECT GetRect();
	RECT GetRectt();

	
	////////////////////////////////////////////////////////////////////
	// Function: “CheckCollision”
	//
	// Purpose: Checks to see if the passed int object's rect intersects
	//			this rect to see if the two objects collide.
	////////////////////////////////////////////////////////////////////
	virtual bool CheckCollision(CBase * pBase);
	virtual bool CheckCollisionA(CBase * pBase);

};