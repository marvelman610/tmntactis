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

	ObjectManager(void);
	ObjectManager& operator=(const ObjectManager&);
	ObjectManager(const ObjectManager&);

	~ObjectManager(void);
	

public:

	static ObjectManager* GetInstance();

	void UpdateObjects(float fElapsedTime);
	void RenderObjects(void);

	void Add(CBase* pObj);
	void Remove(CBase* pObj);

	void RemoveAll(void);



};

#endif