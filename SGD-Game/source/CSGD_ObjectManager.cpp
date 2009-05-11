////////////////////////////////////////////////
//	File	:	"CSGD_ObjectManager.cpp"
//
//	Author	:	David Brown (DB)
//
//	Purpose	:	To contain and manage all of our game objects.
/////////////////////////////////////////////////
#include "CSGD_ObjectManager.h"
#include "CBase.h"
#include"CEnemy.h"
#include "SGD Wrappers\SGD_Util.h"
#include"CSGD_EventSystem.h"
#include <queue>
using namespace std;

CSGD_ObjectManager * CSGD_ObjectManager::s_mInstance = NULL;
CSGD_ObjectManager::CSGD_ObjectManager(void)
{
	//s_mInstance = this;
}

CSGD_ObjectManager::~CSGD_ObjectManager(void)
{
}

void CSGD_ObjectManager::UpdateObjects(float fElapsedTime)
{
	vector<CBase*>::iterator iter = m_vObjectList.begin();

	while(iter != m_vObjectList.end())
	{
		(*iter)->Update(fElapsedTime);
		iter++;
	}
}

struct cmp
{
	bool operator() (const CBase * a, const CBase * b)
	{
		return (a->GetPosY()+a->GetHeight() > b->GetPosY()+b->GetHeight());
	}
};

void CSGD_ObjectManager::RenderObjects(void)
{
	priority_queue<CBase *, vector<CBase *>, cmp> objects;
	for (unsigned int i=0; i < m_vObjectList.size(); i++)
	{
		objects.push(m_vObjectList[i]);
		//m_vObjectList[i]->Render();
	}
	while (objects.size())
	{
		objects.top()->Render();
		objects.pop();
	}
}

void CSGD_ObjectManager::AddObject(CBase* pObject)
{
	//	Check for valid object pointer
	if (pObject == NULL)
		return;

	//	Add object to object list
	m_vObjectList.push_back(pObject);

	//	Add my reference to it.
	pObject->AddRef();
}

void CSGD_ObjectManager::RemoveObject(CBase* pObject)
{
	//	Check for valid object pointer
	if (pObject == NULL)
		return;

	for (vector<CBase*>::iterator iter = m_vObjectList.begin();
		 iter != m_vObjectList.end();
		 iter++)
	{
		// if this is the object we are looking for.
		if ((*iter) == pObject)
		{
			// Remove my reference to this object.
			(*iter)->Release();

			// Remove the object from the list
			iter = m_vObjectList.erase(iter);
			break;
		}
	}
}

void CSGD_ObjectManager::RemoveAllObjects(void)
{
	//	Call Release() on all objects.
	for (unsigned int i=0; i < m_vObjectList.size(); i++)
	{
		m_vObjectList[i]->Release();
	}

	//	Collapse the vector
	m_vObjectList.clear();
}
void CSGD_ObjectManager::DeleteInstance()
{
	s_mInstance->RemoveAllObjects();
	delete s_mInstance;
	s_mInstance = NULL;
	//SAFE_DELETE(s_mInstance);
}
void CSGD_ObjectManager::CheckCollisions(void)
{
	for(unsigned int i = 0; i < m_vObjectList.size(); i++)
	{
		for(unsigned int j = 0; j < m_vObjectList.size(); j++)
		{
			if(m_vObjectList[i]->GetType() != m_vObjectList[j]->GetType())
			{
				if(m_vObjectList[i]->GetType() == OBJECT_BULLET && m_vObjectList[j]->GetType() != OBJECT_PLAYER)
				{
					if(m_vObjectList[i]->CheckCollision(m_vObjectList[j]) )
					{
						CSGD_EventSystem::GetInstance()->SendEvent("Collision",m_vObjectList[i]);
						CSGD_EventSystem::GetInstance()->SendEvent("Collision", m_vObjectList[j]);
						break;
					}
				}
				else if(m_vObjectList[j]->GetType() == OBJECT_BULLET && m_vObjectList[i]->GetType() != OBJECT_PLAYER)
				{
					if(m_vObjectList[i]->CheckCollision(m_vObjectList[j]) )
					{
						CSGD_EventSystem::GetInstance()->SendEvent("Collision", m_vObjectList[j]);
						CSGD_EventSystem::GetInstance()->SendEvent("Collision", m_vObjectList[i]);
						break;

					}
				}	
				else if(m_vObjectList[j]->GetType() == OBJECT_WEB &&m_vObjectList[i]->GetType() == OBJECT_PLAYER)
				{
					if(m_vObjectList[i]->CheckCollision(m_vObjectList[j]) )
					{
						CSGD_EventSystem::GetInstance()->SendEvent("Collision", m_vObjectList[j]);
						CSGD_EventSystem::GetInstance()->SendEvent("BadCollision", m_vObjectList[i]);
						break;

					}
				}
				else if(m_vObjectList[i]->GetType() == OBJECT_WEB &&m_vObjectList[i]->GetType() == OBJECT_PLAYER)
				{
					if(m_vObjectList[i]->CheckCollision(m_vObjectList[j]) )
					{
						CSGD_EventSystem::GetInstance()->SendEvent("BadCollision", m_vObjectList[j]);
						CSGD_EventSystem::GetInstance()->SendEvent("Collision", m_vObjectList[i]);
						break;

					}
				}
				else if(m_vObjectList[j]->GetType() == OBJECT_Enemy || m_vObjectList[j]->GetType() == OBJECT_BOSS 
					&& m_vObjectList[i]->GetType() == OBJECT_PLAYER)
				{
					if(m_vObjectList[i]->CheckCollisionA(m_vObjectList[j]) )
					{
						CSGD_EventSystem::GetInstance()->SendEvent("Collision", m_vObjectList[i]);
						break;

					}

				}
				else if(m_vObjectList[i]->GetType() == OBJECT_Enemy || m_vObjectList[i]->GetType() == OBJECT_BOSS
					&& m_vObjectList[j]->GetType() == OBJECT_PLAYER)
				{
					if(m_vObjectList[i]->CheckCollisionA(m_vObjectList[j]) )
					{
						CSGD_EventSystem::GetInstance()->SendEvent("Collision", m_vObjectList[j]);
						break;
					}
				}
				else if(m_vObjectList[i]->GetType() == OBJECT_HEALTH && m_vObjectList[j]->GetType() == OBJECT_PLAYER)
				{
					if(m_vObjectList[i]->CheckCollisionA(m_vObjectList[j]) )
					{
						CSGD_EventSystem::GetInstance()->SendEvent("Collision", m_vObjectList[i]);
						CSGD_EventSystem::GetInstance()->SendEvent("GoodCollision", m_vObjectList[j]);
						break;
					}
				}
				else if(m_vObjectList[j]->GetType() == OBJECT_HEALTH && m_vObjectList[i]->GetType() == OBJECT_PLAYER)
				{
					if(m_vObjectList[i]->CheckCollisionA(m_vObjectList[j]) )
					{
						CSGD_EventSystem::GetInstance()->SendEvent("Collision", m_vObjectList[j]);
						CSGD_EventSystem::GetInstance()->SendEvent("GoodCollision", m_vObjectList[i]);
						break;
					}

				}
				else if(m_vObjectList[i]->GetType() == OBJECT_POINTS && m_vObjectList[j]->GetType() == OBJECT_PLAYER)
				{
					if(m_vObjectList[i]->CheckCollisionA(m_vObjectList[j]) )
					{
						CSGD_EventSystem::GetInstance()->SendEvent("Collision", m_vObjectList[i]);
						CSGD_EventSystem::GetInstance()->SendEvent("Points", m_vObjectList[j]);
						break;
					}
				}
				else if(m_vObjectList[j]->GetType() == OBJECT_POINTS && m_vObjectList[i]->GetType() == OBJECT_PLAYER)
				{
					if(m_vObjectList[i]->CheckCollisionA(m_vObjectList[j]) )
					{
						CSGD_EventSystem::GetInstance()->SendEvent("Collision", m_vObjectList[j]);
						CSGD_EventSystem::GetInstance()->SendEvent("Points", m_vObjectList[i]);
						break;
					}

				}
			}
		}
	}
}


