////////////////////////////////////////////////////////
//   File Name	:	"ObjectManager.h"
//
//   Author		:	Matthew Di Matteo (MD)
//
//   Purpose	:	Manages all objects in the game
////////////////////////////////////////////////////////

#define OBJECTMANAGER_H
#ifndef OBJECTMANGER_H

#include <vector>
using std::vector;

class CBase;

class ObjectManager
{
private:
	vector<CBase*> m_vObjects;

	///////////////////////////////////////////////////////////////////
	//	Function:	"ObjectManager(Constructor)"
	///////////////////////////////////////////////////////////////////
	ObjectManager(void);
	ObjectManager& operator=(const ObjectManager&);
	ObjectManager(const ObjectManager&);

	///////////////////////////////////////////////////////////////////
	//	Function:	"~ObjectManager(Destructor)"
	///////////////////////////////////////////////////////////////////
	~ObjectManager(void);
	

public:

	///////////////////////////////////////////////////////////////////
	// Function: "GetInstance"
	//
	//  Purpose: Gets instance to the only instance of the class.
	///////////////////////////////////////////////////////////////////
	static ObjectManager* GetInstance();

	////////////////////////////////////////////////////////////////////
	// Function: �UpdateObjects�
	//
	// Purpose: Calls all game objects update based on time.
	////////////////////////////////////////////////////////////////////
	void UpdateObjects(float fElapsedTime);

	////////////////////////////////////////////////////////////////////
	// Function: �RenderObjects�
	//
	// Purpose: Calls all game objects render functions.
	////////////////////////////////////////////////////////////////////
	void RenderObjects(void);

	////////////////////////////////////////////////////////////////////
	// Function: �Add�
	//
	// Purpose: Adds a pointer to an object to the vector of objects
	////////////////////////////////////////////////////////////////////
	void Add(CBase* pObj);
	////////////////////////////////////////////////////////////////////
	// Function: �Remove�
	//
	// Purpose: Remove object from vector of objects
	////////////////////////////////////////////////////////////////////
	void Remove(CBase* pObj);

	////////////////////////////////////////////////////////////////////
	// Function: �RemoveAll�
	//
	// Purpose: Clears the vector of objects
	////////////////////////////////////////////////////////////////////
	void RemoveAll(void);



};

#endif