////////////////////////////////////////////////////////
//   File Name	:	"ObjectManager.cpp"
//
//   Author		:	Matthew Di Matteo (MD)
//
//   Purpose	:	Manages all objects in the game
////////////////////////////////////////////////////////

#include "ObjectManager.h"
#include "BattleMap.h"
#include "CSGD_DirectInput.h"
#include "Tile.h"
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
		m_vObjects[i]->SetPosZ(GetZdepthDraw(m_vObjects[i]->GetAnchor().x, m_vObjects[i]->GetAnchor().y, m_vObjects[i]->GetCurrTile() ) );
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
		int count = 0;
		while(iter != m_vObjects.end())
		{
			if ((*iter) == pObj)
			{
				CBase* temp = m_vObjects[count];
				delete temp;
				iter = m_vObjects.erase(iter);
				break;
			}
			++count;
			++iter;
		}
	}
}

void ObjectManager::RemoveAll(void)
{
	for (unsigned int i=0; i < m_vObjects.size(); i++)
	{
		CBase* temp = m_vObjects[i];		
		delete temp;
	}
	m_vObjects.clear();
}

bool ObjectManager::CheckObjectsToAlpha(RECT* mapObjectRect)
{
	if (CSGD_DirectInput::GetInstance()->KeyPressed(DIK_D))
		int i = 0;
	
	for (unsigned int i=0; i < m_vObjects.size(); i++)
	{
		if (m_vObjects[i]->GetAnchor().x >= mapObjectRect->left &&
			m_vObjects[i]->GetAnchor().x <= mapObjectRect->right &&
			m_vObjects[i]->GetAnchor().y >= mapObjectRect->top &&
			m_vObjects[i]->GetAnchor().y <= mapObjectRect->bottom)
		{
			return true;	
		}
	}
	return false;
}
float ObjectManager::GetZdepthDraw(int xAnchor, int yAnchor, int currTileID)
{
// 	if (CSGD_DirectInput::GetInstance()->KeyPressed(DIK_D))	// if need to do debugging
// 		int i = 0;

	//  for the character to be ahead of the object,
	//	it must be directly below, to the southeast, or
	//	to the southwest of an edge tile...
	//	otherwise it is behind the object.
	CBattleMap* bMap = CBattleMap::GetInstance();
	CTile* tiles = bMap->GetTiles();
	int numCols = bMap->GetNumCols();
	if (currTileID != -1)
	{
		if (tiles[currTileID-1].Flag() == FLAG_OBJECT_EDGE || // NW
			tiles[currTileID-numCols].Flag() == FLAG_OBJECT_EDGE || // NE
			tiles[currTileID-(numCols+1)].Flag() == FLAG_OBJECT_EDGE || // N
			tiles[currTileID-((numCols+1)<<1)].Flag() == FLAG_OBJECT_EDGE || // 
			tiles[currTileID-(numCols+2)].Flag() == FLAG_OBJECT_EDGE ||
			tiles[currTileID-(((numCols+1)<<1)-1)].Flag() == FLAG_OBJECT_EDGE )
		{
			return depth.CHARACTER_AHEAD;
		}
		else
			return depth.CHARACTER_BEHIND;
	} 
	else // shouldn't let it get here...
	{
		MessageBox(0, "Current Tile ID = -1", "Incorrect version.", MB_OK);
		return 0;
	}
}