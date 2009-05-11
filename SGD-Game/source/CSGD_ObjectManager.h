////////////////////////////////////////////////
//	File	:	"CSGD_ObjectManager.h"
//
//	Author	:	David Brown (DB)
//
//	Purpose	:	To contain and manage all of our game objects.
/////////////////////////////////////////////////
#pragma once
#include <vector>
using std::vector;

class CBase;

class CSGD_ObjectManager
{
private:
	vector<CBase*> m_vObjectList;

		CSGD_ObjectManager(void);
	~CSGD_ObjectManager(void);
	CSGD_ObjectManager& operator=(const CSGD_ObjectManager& pOM);
	CSGD_ObjectManager(const CSGD_ObjectManager& pOM);

	static CSGD_ObjectManager * s_mInstance;

public:

	static CSGD_ObjectManager * GetInstance(){if (!s_mInstance) {s_mInstance = new CSGD_ObjectManager;} return s_mInstance;}
	static void DeleteInstance();
	void CheckCollisions(void);

	void UpdateObjects(float fElapsedTime);
	void RenderObjects(void);

	void AddObject(CBase* pObject);
	void RemoveObject(CBase* pObject);

	void RemoveAllObjects(void);


};