////////////////////////////////////////////////////////
//   File Name	:	"ObjectManager.cpp"
//
//   Author		:	Matthew Di Matteo (MD)
//
//   Purpose	:	Manages all objects in the game
////////////////////////////////////////////////////////

#include "ObjectManager.h"
#include "Base.h"

#include <queue>
using namespace std;

struct cmp
{
	bool operator() (const CBase * a, const CBase * b)
	{
		return (a->GetPosY()+a->GetHeight() > b->GetPosY()+b->GetHeight());
	}
};

ObjectManager::ObjectManager(void)
{

}

ObjectManager::~ObjectManager(void)
{
	RemoveAll();
}
ObjectManager* ObjectManager::GetInstance()
{
	static ObjectManager instance;
	return &instance;
}
void ObjectManager::UpdateObjects(float fElapsedTime)
{
	for (unsigned int i=0; i < m_vObjects.size(); i++)
	{
		m_vObjects[i]->Update(fElapsedTime);
	}
}
void ObjectManager::RenderObjects(void)
{
	//Z-Sorted
	priority_queue<CBase *, vector<CBase *>, cmp> objects;
	for (unsigned int i=0; i < m_vObjects.size(); i++)
	{
		objects.push(m_vObjects[i]);
	}
	while (objects.size())
	{
		objects.top()->Render();
		objects.pop();
	}
}

void ObjectManager::Add(CBase* pObj)
{
	if (pObj != NULL)
		m_vObjects.push_back(pObj);
}
void ObjectManager::Remove(CBase* pObj)
{
	if (pObj != NULL)
	{
		vector<CBase*>::iterator iter = m_vObjects.begin();
		while(iter != m_vObjects.end())
		{
			if ((*iter) == pObj)
			{
				iter = m_vObjects.erase(iter);
				break;
			}
			iter++;
		}
	}
}

void ObjectManager::RemoveAll(void)
{
	m_vObjects.clear();
}

bool ObjectManager::CheckObjectsToAlpha(RECT* mapObjectRect)
{
	for (unsigned int i=0; i < m_vObjects.size(); i++)
	{
		if (m_vObjects[i]->GetAnchor().x >= mapObjectRect->left &&
			m_vObjects[i]->GetAnchor().x <= mapObjectRect->left + mapObjectRect->right &&
			m_vObjects[i]->GetAnchor().y >= mapObjectRect->top &&
			m_vObjects[i]->GetAnchor().y <= mapObjectRect->top + mapObjectRect->bottom )
		{
			return true;	
		}
	}
	return false;
}