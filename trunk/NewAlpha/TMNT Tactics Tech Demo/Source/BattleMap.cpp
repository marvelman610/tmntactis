//////////////////////////////////////////////////////////////////////////
//	Filename	:	CBattleMap.cpp
//
//	Author		:	Ramon Johannessen (RJ)
//
//	Purpose		:	To execute the individual battles that take place,
//					drawing the isometric tile map, updating, getting input, etc...
//////////////////////////////////////////////////////////////////////////
#include "BattleMap.h"
#include "Assets.h"
#include "Game.h"
#include "Tile.h"
#include "CSGD_Direct3D.h"
#include "CSGD_DirectInput.h"
#include "CSGD_TextureManager.h"
#include "CSGD_FModManager.h"
#include "ObjectManager.h"
#include "Factory.h"
#include "Animation.h"
#include "MainMenuState.h"
#include "GamePlayState.h"
#include "BitmapFont.h"
#include "ParticleSystem.h"
#include "HUD.h"
#include "Player.h"
#include "Turtle.h"
#include "Ninja.h"
#include "Box.h"
#include <fstream>
#include <exception>

#define BOUNDING_BOXES 0
#define CAM_EDGE_DIST_TO_MOVE 20	// how close to edge of the screen the mouse has to get to start scrolling
#define SCROLLSPEED 150.0f

CBattleMap::CBattleMap(void)
{
	m_pPlayer		= CPlayer::GetInstance();
	m_pAssets		= CAssets::GetInstance();
	m_pTM			= CSGD_TextureManager::GetInstance();
	m_pGame			= CGame::GetInstance();
	m_pDI			= CSGD_DirectInput::GetInstance();
	m_pD3D			= CSGD_Direct3D::GetInstance();
	m_pBitmapFont   = CBitmapFont::GetInstance();
	m_pFMOD 		= CSGD_FModManager::GetInstance();
	m_pHUD			= CHUD::GetInstance();

	godbool = false;
	godcheat = 0;
	APbool = false;
	APcheat = 0;
}

CBattleMap::~CBattleMap(void)
{
	if (m_pAssets)
		m_pAssets = NULL;
	if (m_pTM)
		m_pTM = NULL;
	if (m_pGame)
		m_pGame = NULL;
	if (m_pDI)
		m_pDI = NULL;
	if (m_pHUD)
		m_pHUD = NULL;
	if (m_pPlayer)
		m_pPlayer = NULL;
 	if (m_pBitmapFont)
 		m_pBitmapFont = NULL;
 	if (m_pFMOD)
 		m_pFMOD = NULL;
}

CBattleMap* CBattleMap::GetInstance()
{
	static CBattleMap instance;
	return &instance;
}
void CBattleMap::Enter(char* szFileName, char* szMapName, int nNumEnemies)
{
	SetMousePtr(m_pAssets->aMousePointerID);
	m_bIsMouseAttack = m_bOutOfRange = false;

	m_fTimer = 0.0f;

	string text[3];
	text[0] = "SPECIAL"; text[1] = "ITEM"; text[2] = "END TURN";
	m_bxActionBox = new CBox(3, text, 20, 575, depth.MENUS, false, 35, 25, 15, m_pAssets->aBMactionBoxID, 0.5f);
	m_bxActionBox->SetActive(); 
	m_bxSkillBox = m_bxItemBox = m_bxPauseBox = m_bxLoadBox = m_bxSaveBox = m_bxMessageBox = NULL;
	//m_bxCurrActiveBox = m_bxActionBox; m_bxCurrActiveBox->BoxType(BX_ACTION);

	m_pTilesL1 = NULL;
	m_pTilesL2 = NULL;
	m_pFreeTiles = NULL;
	//m_pMoveableTiles = NULL;
	m_bHaveMoved = false;
	m_sCurrSkillDisplay = m_sCurrSkillName = "NONE"; m_nCurrSkillCost = -1; m_bExecuteSkill = false;
	m_nCurrCharacter = -1;
	m_nHoverCharacter = -1; m_nMoveDirection = -1; m_nCurrMouseTileTarget = -1; m_ncurrTargetTile = -1; m_nCurrTarget = -1;
	m_nCurrSelectedTile = -1;
	m_nNumCols = 0; m_nNumRows = 0;
	m_nTotalNumTiles = 0;
	m_nTileWidth = 0; m_nTileHeight = 0;
	m_nMapHeight = 0; m_nMapWidth = 0;
	m_nFreeTileCount = 0;
	m_fScrollX = 0.0f; m_fScrollY = 0.0f;
	m_nScrenWidth = m_pGame->GetScreenWidth();
	m_nScreenHeight = m_pGame->GetScreenHeight();
	m_nOffsetX = m_nOffsetY = 0;

	m_strCurrVersion = "TED-Version-1.0";	// current tile editor's version number

	//m_pParticleSys = new CParticleSystem();
	//m_pParticleSys->Load("Resources/ParticleInfo/VG_Test.dat");

	//particle test
	m_pParticleSystem = new CParticleSystem[4]();

	m_pParticleSystem[FIRE].Load("Resources/ParticleInfo/VG_Fire.dat");
	m_pParticleSystem[SMOKE].Load("Resources/ParticleInfo/VG_Cloud.dat");
	m_pParticleSystem[GLOW].Load("Resources/ParticleInfo/VG_Test.dat");
	m_pParticleSystem[BLOOD].Load("Resources/ParticleInfo/VG_Blood.dat");

	m_nNumEnemiesKilled = 0;
	m_nNumCharacters = 4+nNumEnemies;
	m_nNumEnemiesLeft = nNumEnemies;
	m_szFileName = szFileName;
	m_szMapName  = szMapName;
	m_bIsPlayersTurn = (bool)(rand() % 2);

	LoadMapInfo();
	// will be used to set ALL the characters' start positions according to
	// the battle map's spawn points
	CreateEnemies();
	SetStartPositions();

	m_pFMOD->PlaySound(m_pAssets->aBMarcadeMusicID);
	m_pFMOD->SetVolume(m_pAssets->aBMarcadeMusicID, m_pGame->GetMusicVolume());
}
void CBattleMap::Exit()
{
	Reset();
	m_pFMOD->StopSound(m_pAssets->aBMarcadeMusicID);
	m_pFMOD->ResetSound(m_pAssets->aBMarcadeMusicID);
}
void CBattleMap::Reset()
{
	m_nNumEnemiesLeft = m_nNumEnemiesKilled = m_nNumCharacters = 0;
	m_nCurrCharacterTile = -1;
	delete[] m_pTilesL1;
	delete[] m_pTilesL2;
	delete[] m_pFreeTiles;
	delete[] m_pParticleSystem;
	if(m_bxItemBox)
	{
		delete m_bxItemBox;
		m_bxItemBox = NULL;
	}
	if (m_bxSkillBox)
	{
		delete m_bxSkillBox;
		m_bxSkillBox = NULL;
	}
	if (m_bxActionBox)
	{
		delete m_bxActionBox;
		m_bxActionBox = NULL;
	}
	if (m_bxPauseBox)
	{
		delete m_bxPauseBox;
		m_bxPauseBox = NULL;
	}
	if (m_bxSaveBox)
	{
		delete m_bxSaveBox;
		m_bxSaveBox = NULL;
	}
	if (m_bxLoadBox)
	{
		delete m_bxLoadBox;
		m_bxLoadBox = NULL;
	}
	if (m_bxMessageBox)
	{
		delete m_bxMessageBox;
		m_bxMessageBox = NULL;
	}
// 	if (m_bxCurrActiveBox)
// 	{
// 		delete m_bxCurrActiveBox;
// 		m_bxCurrActiveBox = NULL;
// 	}
	ObjectManager::GetInstance()->ClearEnemies();
	m_vCharacters.clear();
	m_vEnemies.clear();
	m_bIsPaused = false;
	CGamePlayState::GetInstance()->SetPaused(false);
}
void CBattleMap::Render()
{
	//m_pTM->DrawWithZSort(m_pAssets->aBMbgID, 0, 0, 1.0f, 1.0f, 1.0f, NULL, 0.0f, 0.0f, 0.0f, D3DCOLOR_XRGB(100, 100, 100));
	//////////////////////////////////////////////////////////////////////////
	//TODO::this will be put in the hud eventually
	//////////////////////////////////////////////////////////////////////////

	if (m_nHoverCharacter != -1 && m_bIsPlayersTurn)
		DrawHover();

	else if (m_nHoverCharacter == -1 && m_nCurrCharacter == -1)
		SetMousePtr(m_pAssets->aMousePointerID);
	else if (m_nHoverCharacter == -1 && m_nCurrCharacter > -1)
		SetMousePtr(m_pAssets->aMouseMoveID);
	if (m_bxItemBox || m_bxSkillBox)
		if (m_bxSkillBox->IsMouseInBox() || m_bxItemBox->IsMouseInBox())
			SetMousePtr(m_pAssets->aMousePointerID);
	if (m_bxActionBox->IsActiveBox() && m_bxActionBox->IsMouseInBox())
		SetMousePtr(m_pAssets->aMousePointerID);
	if (m_nHoverCharacter == -1 && m_bIsPlayersTurn)
	{
		m_pPlayer->GetTurtles()[LEONARDO]->Colorize(false);
		m_pPlayer->GetTurtles()[DONATELLO]->Colorize(false);
		m_pPlayer->GetTurtles()[RAPHAEL]->Colorize(false);
		m_pPlayer->GetTurtles()[MIKEY]->Colorize(false);
		if (m_nHoverCharacter == -1)
		{
			for (int i = 0; i < m_nNumEnemiesLeft; ++i)
				m_vEnemies[i]->Colorize(false);
		}
	}

	// draw layer one & two
	MY_POINT mapPT;
	for (int x = 2; x < m_nNumCols; ++x)
	{
		for (int y = 2; y < m_nNumRows; ++y)
		{
			int tileID = y*m_nNumCols+x;
			if (m_pTilesL1[tileID].DestXID() == -1)
				continue;
			mapPT.x = x; mapPT.y = y;
			mapPT = IsoTilePlot(mapPT, GetOffsetX(), GetOffsetY());

			// Layer one
// 			if (m_pTilesL1[tileID].Flag() == FLAG_OBJECT_EDGE)
// 				m_pTilesL1[tileID].SetAlpha(150);
// 			else
// 				m_pTilesL1[tileID].SetAlpha(255);
			if (m_pTilesL1[tileID].Alpha() == 200 && m_bIsPlayersTurn)
			{
				m_pTM->DrawWithZSort(m_pTilesL1[tileID].ImageID(), mapPT.x, mapPT.y, depth.GROUND, 1.0f, 1.0f, 
					m_pTilesL1[tileID].SourceRect(), 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(m_pTilesL1[tileID].Alpha(),0,0,255) );
			} 
			else if (m_pTilesL1[tileID].Alpha() == 199 && m_bIsPlayersTurn)
			{
				m_pTM->DrawWithZSort(m_pTilesL1[tileID].ImageID(), mapPT.x, mapPT.y, depth.GROUND, 1.0f, 1.0f, 
					m_pTilesL1[tileID].SourceRect(), 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(m_pTilesL1[tileID].Alpha(),0,255,0) );
			}
			else
			{
				m_pTM->DrawWithZSort(m_pTilesL1[tileID].ImageID(), mapPT.x, mapPT.y, depth.GROUND, 1.0f, 1.0f, 
					m_pTilesL1[tileID].SourceRect(), 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(255,255,255,255));
			}
			// Layer two
			if (m_pTilesL2[tileID].DestXID() != -1)
			{
				m_pTM->DrawWithZSort(m_pTilesL2[tileID].ImageID(), mapPT.x, mapPT.y, depth.GROUNDL2, 1.0f, 1.0f, 
					m_pTilesL2[tileID].SourceRect(), 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(m_pTilesL2[tileID].Alpha(),255,255,255));
			}
			
			// draws selection rectangle on the currently selected tile, character, and enemy (and draws arrow)
			if (m_bIsPlayersTurn)
			{
				if (m_nCurrSelectedTile == tileID && !m_bxActionBox->IsMouseInBox())
				{
					if (m_bxItemBox)
						if (m_bxItemBox->IsMouseInBox())
							continue;
					else if (m_bxSkillBox)
						if (m_bxSkillBox->IsMouseInBox())
							continue;
					m_pTM->DrawWithZSort(m_pAssets->aBMcursorID, mapPT.x, mapPT.y, depth.SELECTION, 1.0f, 1.0f);
				}
				else if (m_nHoverCharacter > -1 && m_nHoverCharacter < 4 && m_vCharacters[m_nHoverCharacter].GetCurrTile() == tileID)
				{
					m_pTM->DrawWithZSort(m_pAssets->aBMgreenSquareID, mapPT.x, mapPT.y, depth.SELECTION, 1.0f, 1.0f);
				}
				else if (m_nHoverCharacter > 3 && m_vCharacters[m_nHoverCharacter].GetCurrTile() == tileID)
				{
					m_pTM->DrawWithZSort(m_pAssets->aBMcursorID, mapPT.x, mapPT.y, depth.SELECTION, 1.0f, 1.0f);
				}
				if (m_nCurrCharacterTile == tileID && m_nCurrCharacter > -1)
				{
					m_pTM->DrawWithZSort(m_pAssets->aBMgreenSquareID, mapPT.x, mapPT.y, depth.SELECTION, 1.0f, 1.0f);
					m_pTM->DrawWithZSort(m_pAssets->aBMcurrSelectedArrowID, (int)m_vCharacters[m_nCurrCharacter].GetPosX()+8, 
						(int)m_vCharacters[m_nCurrCharacter].GetPosY()-32, depth.ARROW);
				}
				else if (m_ncurrTargetTile == tileID && m_nCurrTarget > -1)
				{
					m_pTM->DrawWithZSort(m_pAssets->aBMcursorID, mapPT.x, mapPT.y, depth.SELECTION, 1.0f, 1.0f);
					m_pTM->DrawWithZSort(m_pAssets->aBMcurrTargetArrowID, (int)m_vCharacters[m_nCurrTarget].GetPosX()+8,
						(int)m_vCharacters[m_nCurrTarget].GetPosY()-32, depth.ARROW);
				}
			}
		}
	}
	// draw the free placed layer
	for (int i = 0; i < m_nFreeTileCount; ++i)
	{
		// if the character is inside a map object, alpha that object out
		// also, see if the mouse is inside a free-placed object
		RECT srcRect = *m_pFreeTiles[i].SourceRect();
		srcRect.left = m_pFreeTiles[i].DestX() + GetFreeTileXos();
		srcRect.top  = m_pFreeTiles[i].DestY() + GetFreeTileYos(); 
		srcRect.right= srcRect.left + m_pFreeTiles[i].Width();
		srcRect.bottom=srcRect.top + m_pFreeTiles[i].Height();
		if (ObjectManager::GetInstance()->CheckObjectsToAlpha(&srcRect) || (
			m_ptMouseScreenCoord.x > srcRect.left && m_ptMouseScreenCoord.x < srcRect.right &&
			m_ptMouseScreenCoord.y > srcRect.top  && m_ptMouseScreenCoord.y < srcRect.bottom)
			)
			m_pFreeTiles[i].SetAlpha(100);
		else
			m_pFreeTiles[i].SetAlpha(255);

		m_pTM->DrawWithZSort(m_pFreeTiles[i].ImageID(), srcRect.left, srcRect.top,
			depth.OBJECTS, 1.0f, 1.0f, m_pFreeTiles[i].SourceRect(), 0.0f, 0.0f, m_pFreeTiles[i].Rotation(), D3DCOLOR_ARGB(m_pFreeTiles[i].Alpha(),255,255,255));
#if BOUNDING_BOXES
		// drawing bounding boxes
		CSGD_Direct3D::GetInstance()->DrawLine(srcRect.left, srcRect.top, srcRect.right, srcRect.top, 255, 0, 0);
		CSGD_Direct3D::GetInstance()->DrawLine(srcRect.left, srcRect.bottom, srcRect.right, srcRect.bottom, 255, 0, 0);
		CSGD_Direct3D::GetInstance()->DrawLine(srcRect.left, srcRect.top, srcRect.left, srcRect.bottom, 255, 0, 0);
		CSGD_Direct3D::GetInstance()->DrawLine(srcRect.right, srcRect.top, srcRect.right, srcRect.bottom, 255, 0, 0);
		if (m_nCurrCharacter!=-1)
		{
			char szCurrTile[64]; sprintf_s(szCurrTile, "CurrTile:%i", m_vCharacters[m_nCurrCharacter].GetCurrTile() );
			CSGD_Direct3D::GetInstance()->DrawText(szCurrTile, 5, 200);
		}
#endif
	}
	if (m_bIsPlayersTurn)
		DrawBoxes();
	if (m_bIsPaused && m_bxPauseBox)
		m_bxPauseBox->Render();
	m_pHUD->Render();
	if (m_bIsPlayersTurn)
		m_pBitmapFont->DrawStringAutoCenter("PLAYER'S TURN", m_nScrenWidth, 10, 0.09f, 0.6f);
	else
		m_pBitmapFont->DrawStringAutoCenter("COMPUTER'S TURN", m_nScrenWidth, 10, 0.09f, 0.6f);
	if (m_bIsPlayersTurn && m_nCurrCharacter > -1)
	{
		m_pBitmapFont->DrawString("SKILL:", 5, 190, 0.05f, 0.5f);
		m_pBitmapFont->DrawString(m_sCurrSkillName.c_str(), 5, 220, 0.05f, 0.5f);
		if (m_nCurrSkillCost > -1)
		{
			char temp[32];
			sprintf_s(temp, "COST: %i", m_nCurrSkillCost);
			string sCost = temp;
			m_pBitmapFont->DrawString(sCost.c_str(), 5, 250, 0.05f, 0.5f);
		}
	}
	if (m_bNotEnoughAP && m_fTimer < 3)
	{
		m_pBitmapFont->DrawStringAutoCenter("-NOT ENOUGH AP-", m_nScrenWidth, 35, 0.09f, 0.6f, D3DCOLOR_XRGB(255, 0, 0));
	}
	else if (m_bOutOfRange && m_fTimer < 3)
	{
		m_pBitmapFont->DrawStringAutoCenter("-OUT OF RANGE-", m_nScrenWidth, 35, 0.09f, 0.6f, D3DCOLOR_XRGB(255, 0, 0));
	}
	if (m_fTimer > 3 && (m_bNotEnoughAP || m_bOutOfRange))
	{
		m_bNotEnoughAP = m_bOutOfRange = false;
		m_fTimer = 0.0f;
	}

	if (m_nCurrCharacter > -1)
	{
		char apText[64];
		sprintf_s(apText, "AP: %i", m_vCharacters[m_nCurrCharacter].GetCurrAP());
		m_pD3D->DrawText(apText, 10, 480, 0,0,0);
		if (m_nCurrTarget > -1)
		{
			char distText[64];
			sprintf_s(distText, "Dist to Target: %i", m_nDistanceToTarget);
			m_pBitmapFont->DrawStringAutoCenter(distText, m_nScrenWidth, 730, 0.09f, 0.5f);
		}
	}
	if (m_nCurrTarget > -1)
	{
		char ninjaHealth[64];
		sprintf_s(ninjaHealth, "Health: %i", m_vCharacters[m_nCurrTarget].GetHealth());
		m_pD3D->DrawText(ninjaHealth, 10, 460, 0, 0, 0);
	}
	// draw the current mouse pointer
	m_pTM->DrawWithZSort(GetMousePtr(), m_ptMouseScreenCoord.x-10, m_ptMouseScreenCoord.y-3, 0.0f);
	/////////////////////////////////////////////////////////////////////////////
	//draw particles when active
	if(m_pParticleSystem[FIRE].m_bActive == true)
	{	m_pParticleSystem[FIRE].DrawParticle(m_pAssets->aFireParticle); }
	if(m_pParticleSystem[GLOW].m_bActive == true)
	{ m_pParticleSystem[GLOW].DrawParticle(m_pAssets->aGlowParticle);	}
	if(m_pParticleSystem[SMOKE].m_bActive == true)
	{ m_pParticleSystem[SMOKE].DrawParticle(m_pAssets->aSmokeParticle); }
	if(m_pParticleSystem[BLOOD].m_bActive == true)
	{ m_pParticleSystem[BLOOD].DrawParticle(m_pAssets->aBloodParticle); }

	DrawDebugInfo();
}
void CBattleMap::SetPaused(bool IsPaused)
{
	m_bIsPaused = IsPaused;
	if (m_bIsPaused)
	{
		if (m_bxPauseBox)
			delete m_bxPauseBox;
		string text[4]; text[1] = "SAVE GAME"; text[2] = "LOAD GAME"; text[3] = "QUIT"; text[0] = "PAUSED";
		m_bxPauseBox = new CBox(4, text, 320, 300, 0.1f, true);
		m_bxPauseBox->SetActive();
		m_bxPauseBox->SetType(BOX_WITH_BACK);
		m_bxActionBox->SetActive(false);
		if (m_bxItemBox)
			m_bxItemBox->SetActive(false);
		if (m_bxSkillBox)
			m_bxSkillBox->SetActive(false);
	}
	else
	{
		if (m_bxItemBox)
			m_bxItemBox->SetActive();
		else if (m_bxSkillBox)
			m_bxSkillBox->SetActive();
		else 
			m_bxActionBox->SetActive();
		if (m_bxPauseBox)
			delete m_bxPauseBox;
		m_bxPauseBox = NULL;
	}
}
void CBattleMap::Update(float fElapsedTime)
{
	// AI's turn
	if(m_bIsPlayersTurn == false && m_vEnemies.size() != 0)
	{
		m_bHaveMoved = false;
		int index = rand()%m_vEnemies.size();
		CNinja* ninja = (CNinja*)m_vEnemies[index];
		ninja->AI();

		m_vCharacters.clear();
		for (int i = 0; i < 4; ++i)
			m_vCharacters.push_back((CBase)(*m_pPlayer->GetTurtles()[i]));
		for (int i = 0; i < m_nNumEnemiesLeft; ++i)
			m_vCharacters.push_back(*m_vEnemies[i]);
		m_nCurrCharacter = -1;
		for(int nx = 2; nx < m_nNumCols; ++nx)
			for(int ny = 2; ny < m_nNumRows; ++ny)
			{
				int id = ny*m_nNumCols+nx;
				if (m_pTilesL1[id].Alpha() != 255)
					m_pTilesL1[id].SetAlpha(255);
			}
	}
	// a turtle has been moved...execute the animation and position change over time
	if (m_bMoving)
	{
		if (m_vPath.size() > 0)
		{
			// grab the next move and take it out of the vector..if the previous move is complete
			POINT newPoint = m_vPath[0];
			bool bMoveComplete = false;
			// set up variables
			POINT currPoint= m_vCharacters[m_nCurrCharacter].GetMapCoord();
			CTurtle* turtle= m_pPlayer->GetTurtles()[m_nCurrCharacter];
			MY_POINT_FLOAT currPos; 
			currPos.x = turtle->GetPosX(); currPos.y = turtle->GetPosY();

			// NORTHWEST
			if ( newPoint.x < currPoint.x && abs(m_ptStartXY.x - currPos.x) < 32 && abs(m_ptStartXY.y - currPos.y) < 16)
			{
				currPos.x -= turtle->GetVelX() * fElapsedTime;
				currPos.y -= turtle->GetVelY() * fElapsedTime;
				turtle->SetPosPtF(currPos);
			}
			// SOUTHEAST
			else if ( newPoint.x > currPoint.x && abs(m_ptStartXY.x - currPos.x) < 32 && abs(m_ptStartXY.y - currPos.y) < 16)
			{
				currPos.x += turtle->GetVelX() * fElapsedTime;
				currPos.y += turtle->GetVelY() * fElapsedTime;
				turtle->SetPosPtF(currPos);
			}
			// NORTHEAST
			if ( newPoint.y < currPoint.y && abs(m_ptStartXY.x - currPos.x) < 32 && abs(m_ptStartXY.y - currPos.y) < 16)
			{
				currPos.y -= turtle->GetVelY() * fElapsedTime;
				currPos.x += turtle->GetVelX() * fElapsedTime;
				turtle->SetPosPtF(currPos);
			}
			// SOUTHWEST
			else if ( newPoint.y > currPoint.y && abs(m_ptStartXY.x - currPos.x) < 32 && abs(m_ptStartXY.y - currPos.y) < 16)
			{
				currPos.y += turtle->GetVelY() * fElapsedTime;
				currPos.x -= turtle->GetVelX() * fElapsedTime;
				turtle->SetPosPtF(currPos);
			}
			// check to see if this current tile move is complete
			if ( abs(m_ptStartXY.x - currPos.x) >= 32 && abs(m_ptStartXY.y - currPos.y) >= 16)
			{
				vector<POINT>::iterator first = m_vPath.begin();
				m_vPath.erase(first);
				m_vCharacters[m_nCurrCharacter].SetCurrTile(newPoint, GetOffsetX(), GetOffsetY(), m_nTileWidth, m_nTileHeight, m_nNumCols);
				m_pPlayer->GetTurtles()[m_nCurrCharacter]->SetCurrTile(newPoint, GetOffsetX(), GetOffsetY(), m_nTileWidth, m_nTileHeight, m_nNumCols);
				m_nCurrCharacterTile = m_pPlayer->GetTurtles()[m_nCurrCharacter]->GetCurrTile();
 				m_vCharacters[m_nCurrCharacter].DecrementCurrAP(2);
				m_ptStartXY.x = m_vCharacters[m_nCurrCharacter].GetPosX();
				m_ptStartXY.y = m_vCharacters[m_nCurrCharacter].GetPosY();
				m_pTilesL1[currPoint.y * m_nNumCols + currPoint.x].SetAlpha(200);
			}
		}
		else // movement is done
		{
			m_bHaveMoved = true;
			CalculateRanges();
			m_pPlayer->GetTurtles()[m_nCurrCharacter]->SetCurrAP(m_vCharacters[m_nCurrCharacter].GetCurrAP());
			m_bMoving = false;
			m_bPathDisplayed = false;
		}
	}
	cheat();
	if(godbool)
		m_vCharacters[m_nCurrCharacter].SetHealth(100);
	if(APbool)
	{
		m_vCharacters[m_nCurrCharacter].SetCurrAP(20);
		APcheat = !APcheat;
		APbool = false;
	}
	//CPlayer::GetInstance()->Update(fElapsedTime);
	CHUD::GetInstance()->Update(fElapsedTime);

	//update the particle system
	m_pParticleSystem[FIRE].UpdateParticle(fElapsedTime);
	m_pParticleSystem[GLOW].UpdateParticle(fElapsedTime);
	m_pParticleSystem[SMOKE].UpdateParticle(fElapsedTime);
	m_pParticleSystem[BLOOD].UpdateParticle(fElapsedTime);
	
	// if a skill is being executed...
	if ( m_bExecuteSkill )
	{
		CSkill*  skill = m_pPlayer->GetTurtles()[m_nCurrCharacter]->GetCurrSelectedSkill();
		skill->Update(fElapsedTime, skill);
		if (skill->IsComplete())
			m_bExecuteSkill = false;
		return;
	}
	else if ( m_bExecuteSkill )
	{
		m_bExecuteSkill = false;
		return;
	}
	// for keyboard movement
	if (m_nMoveDirection != -1)
	{
		POINT newPt = m_vCharacters[m_nCurrCharacter].GetMapCoord();
		switch(m_nMoveDirection)
		{
		case MOVE_ADD_X:
			++newPt.x;
			if (newPt.x > m_nNumCols-1)
				newPt.x = m_nNumCols-1;
			break;
		case MOVE_ADD_Y:
			++newPt.y;
			if (newPt.y > m_nNumRows-1)
				newPt.y = m_nNumRows-1;
			break;
		case MOVE_MINUS_X:
			--newPt.x;
			if (newPt.x < 2)
				newPt.x = 2;
			break;
		case MOVE_MINUS_Y:
			--newPt.y;
			if (newPt.y < 2)
				newPt.y = 2;
			break;
		}
		int id = newPt.y * m_nNumCols + newPt.x;
		if (m_pTilesL1[id].Flag() != FLAG_OBJECT_EDGE && m_pTilesL1[id].Flag() != FLAG_COLLISION && 
			(m_vCharacters[m_nCurrCharacter].GetCurrAP() >= (abs(newPt.x - m_vCharacters[m_nCurrCharacter].GetMapCoord().x) +
				abs(newPt.y - m_vCharacters[m_nCurrCharacter].GetMapCoord().y) ) * 2))
		{
			m_vCharacters[m_nCurrCharacter].SetCurrTile(newPt, GetOffsetX(), GetOffsetY(), m_nTileWidth, m_nTileHeight, m_nNumCols);
			m_pPlayer->GetTurtles()[m_nCurrCharacter]->SetCurrTile(newPt, GetOffsetX(), GetOffsetY(), m_nTileWidth, m_nTileHeight, m_nNumCols);
			m_nCurrCharacterTile = m_pPlayer->GetTurtles()[m_nCurrCharacter]->GetCurrTile();
			m_vCharacters[m_nCurrCharacter].DecrementCurrAP(2);
			CalculateRanges();
	  	}
		m_nMoveDirection = -1;
	}
	if (m_bNotEnoughAP || m_bOutOfRange)
	{
		m_fTimer += fElapsedTime;
	}
}

bool CBattleMap::Input(float fElapsedTime, POINT mouse)
{
	if(CGamePlayState::GetInstance()->GetPaused() && !m_bxLoadBox && !m_bxSaveBox)
		SetPaused(true);
	else if (!CGamePlayState::GetInstance()->GetPaused() && !m_bxLoadBox && !m_bxSaveBox)
		SetPaused(false);
	int xID, yID;
	m_ptMouseScreenCoord = mouse;
	mouse.x -= (LONG)m_fScrollX;
	mouse.y -= (LONG)m_fScrollY;
	if (m_bMoving)
		return true;

	// Keyboard input
	if (!HandleKeyBoardInput(fElapsedTime))
		return false;
	if (m_bExecuteSkill)
		return true;
	// Mouse movement (edge of screen to move camera)
	if (m_bIsPlayersTurn || m_bIsPaused)
	{
		if(!CGamePlayState::GetInstance()->GetPaused())
		{	
			if (m_ptMouseScreenCoord.x < CAM_EDGE_DIST_TO_MOVE)
				MoveCamLeft(fElapsedTime);
			if (m_ptMouseScreenCoord.x > m_nScrenWidth-CAM_EDGE_DIST_TO_MOVE)
				MoveCamRight(fElapsedTime);
			if (m_ptMouseScreenCoord.y < CAM_EDGE_DIST_TO_MOVE)
				MoveCamUp(fElapsedTime);
			if (m_ptMouseScreenCoord.y > m_nScreenHeight-CAM_EDGE_DIST_TO_MOVE)
				MoveCamDown(fElapsedTime);
		}

		// transform the mouse into map coordinates
		xID = ((m_nTileWidth * (mouse.y )) + (m_nTileHeight * (mouse.x - m_nIsoCenterTopX ))) / (m_nTileWidth * m_nTileHeight);
		yID = ((m_nTileWidth * (mouse.y )) - (m_nTileHeight * (mouse.x - m_nIsoCenterTopX ))) / (m_nTileWidth * m_nTileHeight);
		// these check for the mouse being off the map
		// if it is, it is reset to the lowest row/column
		if (xID >= m_nNumCols && yID >= m_nNumRows)
		{
			yID = m_nNumRows-1;
			xID = m_nNumCols-1;
		}	
		else if (xID < 2 && yID < 2)
			yID = xID = 2;
		else if (xID < 2 && yID >= m_nNumRows)
		{
			xID = 2;
			yID = m_nNumRows-1;
		}
		else if (xID >= m_nNumCols && yID < 2)
		{
			yID = 2;
			xID = m_nNumCols-1;
		}
		else if (xID < 2)
			xID = 2;
		else if (yID < 2)
			yID = 2;
		else if (xID >= m_nNumCols)
			xID = m_nNumCols-1;
		else if (yID >= m_nNumRows)
			yID = m_nNumRows-1;

		m_nCurrSelectedTile = yID * m_nNumCols + xID;	// get the tile ID

		if (m_bIsPlayersTurn || m_bIsPaused)
			HandleMouseInput(fElapsedTime, mouse, xID, yID);

		// debugging
		if (m_pDI->KeyPressed(DIK_D))
			int i = 0;
	}
	return true;
}

void CBattleMap::CreateEnemies()
{
	for (int i = 0; i < m_nNumEnemiesLeft; ++i)
	{
		CNinja* ninja = Factory::GetInstance()->CreateNinja();
		m_vCharacters.push_back((CBase)*ninja);
		m_vEnemies.push_back((CBase*)ninja);
	}
}

void CBattleMap::LoadMapInfo()
{
	ifstream ifs;
	ifs.exceptions(~ios_base::goodbit);
	
	try
	{
		ifs.open(m_szFileName, ios_base::in | ios_base::binary);
	}
	catch(ios_base::failure &)
	{
		if (!ifs.is_open())
		{
			char szBuffer[128];
			sprintf_s(szBuffer, "Failed to open file: %s", m_szFileName );
			MessageBox(0, szBuffer, "Incorrect version.", MB_OK);
			m_pGame->ChangeState(CMainMenuState::GetInstance());
		}
		if (ifs.eof())
		{ifs.close();return;}		
	}
	//read input from the given binary file
	string version, eat, name, tilesetName, fileName;
	char file[256];
	char buff[256];
	ZeroMemory(buff, 256);
	byte size;
	ifs.read(reinterpret_cast<char*>(&size), 1);
	ifs.read(buff, size);

	version = buff;
	try
	{
		if (version == m_strCurrVersion)	// make sure we are loading the current version
		{
			// Read in basic map info:
			// Total number of tiles on map
			// number of columns
			// number of rows
			ifs.read(reinterpret_cast<char*>(&m_nTotalNumTiles), 4);
			ifs.read(reinterpret_cast<char*>(&m_nNumCols), 4);
			ifs.read(reinterpret_cast<char*>(&m_nNumRows), 4);

			// the exact center of the map's x and y, offset by half height and width
			m_nTileWidth = 64; m_nTileHeight = 32;	// hard-coded, grid units are 64x32
			//////////////////////////////////////////////////////////////////////////
			//	set up the map so that it centers at the beginning
			m_nMapWidth = m_nTileWidth * m_nNumCols; m_nMapHeight = m_nTileHeight * m_nNumRows;

			m_nIsoCenterLeftY = -(m_nTileHeight<<1)+(m_nTileHeight>>1) - ((m_nMapHeight >> 1) - (m_nScreenHeight >> 1));
			m_nIsoCenterTopX = (int)((((float)(m_nNumCols)) / 2.0f) * m_nTileWidth - (m_nTileWidth<<1));
			m_nFreeTileOSx = (m_nTileWidth<<1);	
			m_nfreeTileOSy = (m_nTileHeight<<1);
			// allocate memory for layer 1, 2, and 3(free placed tiles)
			m_pTilesL1  = new CTile[m_nNumRows*m_nNumCols];
			m_pTilesL2  = new CTile[m_nNumRows*m_nNumCols];
			m_pFreeTiles= new CFreeTile[m_nNumRows*m_nNumCols];
			SetOffsetX((int)m_fScrollX + m_nIsoCenterTopX - (m_nTileWidth >> 1));
			SetOffsetY((int)m_fScrollY + m_nIsoCenterLeftY - (m_nTileHeight >> 1));
			SetFTosX((int)m_fScrollX - ((m_nMapWidth >> 1) - (m_nScrenWidth >> 1)) - (m_nTileWidth >> 1));
			SetFTosY((int)m_fScrollY - m_nTileHeight);
		}
		else // didn't have the correct version number...
		{
			char szBuffer[128];
			sprintf_s(szBuffer, "Current version: %s ...does not match loaded version: %s", m_strCurrVersion.c_str(), version.c_str());
			MessageBox(0, szBuffer, "Incorrect version.", MB_OK);
			m_pGame->ChangeState(CMainMenuState::GetInstance());
			ifs.close();return;
		}
	}
	catch(ios_base::failure &)
	{
		if (!ifs.eof())
			throw;
		else
			ifs.close();
	}

	int tilesetCount = 0;	// how many tilesets are we going to be drawing from?
	int xID, yID, destID, sourceID, flag, width, height; // tile variables
	float rotation;
	string trigger;	// tile trigger string
	byte red, green, blue; // for tileset key color

	try 
	{
		ZeroMemory(buff, 256);
		ifs.read(reinterpret_cast<char*>(&size), 1);
		ifs.read(buff, size);
		name = buff;
		//////////////////////////////////////////////////////////////////////////
		// Loading in tilesets
		// if there's a tileset..load the image with the correct key color
		while (name == "Tileset")
		{
			ZeroMemory(buff, size);
			ifs.read(reinterpret_cast<char*>(&size), 1);
			ifs.read(buff, size);
			fileName = buff;
			strcpy_s(file, fileName.c_str());

			ZeroMemory(buff, 256);
			ifs.read(reinterpret_cast<char*>(&size), 1);
			ifs.read(buff, size);
			name = buff;

			ifs.read(reinterpret_cast<char*>(&red), 1);
			ifs.read(reinterpret_cast<char*>(&green), 1);
			ifs.read(reinterpret_cast<char*>(&blue), 1);
			// store the tileset information so we can determine where each tile comes from
			m_pTilesets[tilesetCount].id = m_pTM->LoadTexture(file, D3DCOLOR_ARGB(255,red, green, blue));
			m_pTilesets[tilesetCount++].name = name;

			ZeroMemory(buff, 256);
			ifs.read(reinterpret_cast<char*>(&size), 1);
			ifs.read(buff, size);
			name = buff;
		}
	}
	catch (ios_base::failure &)
	{
		if (!ifs.eof())
			throw;
		else
			ifs.close();
	}
	int imageID; int sPos;
	sPos = ifs.tellg();
	try
	{
		//////////////////////////////////////////////////////////////////////////
		// Layer ONE
		int count = 0;
		while (name == "Layer1")
		{
			// read in tile's tileset name (which tileset it came from)
			ZeroMemory(buff, 256);
			ifs.read(reinterpret_cast<char*>(&size), 1);
			ifs.read(buff, size);
			tilesetName = buff;

			// determine which image id this tile should have
			for (int tsIndex = 0; tsIndex < tilesetCount; ++tsIndex)
			{
				if (tilesetName == m_pTilesets[tsIndex].name)
				{
					imageID = m_pTilesets[tsIndex].id;
					break;
				}
			}
			ifs.read(reinterpret_cast<char*>(&sourceID), 4);
			ifs.read(reinterpret_cast<char*>(&xID), 4);
			ifs.read(reinterpret_cast<char*>(&yID), 4);
			ifs.read(reinterpret_cast<char*>(&flag), 4);
			ifs.read(reinterpret_cast<char*>(&width), 4);
			ifs.read(reinterpret_cast<char*>(&height), 4);
			ZeroMemory(buff, 256);
			ifs.read(reinterpret_cast<char*>(&size), 1);
			ifs.read(buff, size);
			trigger = buff;

			// setting up each tile of the first layer
			destID = yID * m_nNumCols + xID;	// the id of the tile in the 1-d array
			m_pTilesL1[destID] = CTile(sourceID, imageID, m_nNumCols, xID, yID, width, height, flag, trigger);
			rotation = 0;
			// skip ahead to determine if we have more tiles
			// or if we move on to layer 2 ("L") or 3 ("F")
			sPos = ifs.tellg();
			ifs.seekg(sPos+1);
			eat = ifs.peek();
			count++;
			if (count == 400)
				int i = 0;
			if (eat == "L" || eat == "F")
				break;
			else
				ifs.seekg(sPos);
		}
	}
	catch(ios_base::failure &)
	{
		if (!ifs.eof())
			throw;
		else
			ifs.clear();ifs.close(); return;
	}
			
	try
	{
		if (m_pTilesL1[0].DestXID() != -1)
		{
			ifs.seekg(sPos);	// set the input stream position back to where it needs to be
			//////////////////////////////////////////////////////////////////////////
			// Layer TWO
			ZeroMemory(buff, 256);
			ifs.read(reinterpret_cast<char*>(&size), 1);
			ifs.read(buff, size);
			name = buff;
 		}
		while (name == "Layer2")
		{
			// read in tile's tileset name (which tileset it came from)
			ZeroMemory(buff, 256);
			ifs.read(reinterpret_cast<char*>(&size), 1);
			ifs.read(buff, size);
			tilesetName = buff;
			// determine which image id this tile should have
			for (int tsIndex = 0; tsIndex < tilesetCount; ++tsIndex)
			{
				if (tilesetName == m_pTilesets[tsIndex].name)
				{
					imageID = m_pTilesets[tsIndex].id;
					break;
				}
			}
			ifs.read(reinterpret_cast<char*>(&sourceID), 4);
			ifs.read(reinterpret_cast<char*>(&xID), 4);
			ifs.read(reinterpret_cast<char*>(&yID), 4);
			ifs.read(reinterpret_cast<char*>(&flag), 4);
			ifs.read(reinterpret_cast<char*>(&width), 4);
			ifs.read(reinterpret_cast<char*>(&height), 4);
			ZeroMemory(buff, 256);
			ifs.read(reinterpret_cast<char*>(&size), 1);
			ifs.read(buff, size);
			trigger = buff;

			// setting up each tile of the first layer
			destID = yID * m_nNumCols + xID;
			m_pTilesL2[destID] = CTile(sourceID, imageID, m_nNumCols, xID, yID, width, height, flag, trigger);
			sPos = ifs.tellg();
			ifs.seekg(sPos+1);
			eat = ifs.peek();
			if (eat == "F")
				break;
			else
				ifs.seekg(sPos);
		}

	}
	catch (ios_base::failure &)
	{
		if (!ifs.eof())
			throw;
		else
			ifs.clear();ifs.close(); return;	
	}

	try
	{
		if (m_pTilesL2[0].DestXID() != -1 && m_pTilesL1[0].DestXID() != -1 || eat[0] == 'F')
		{
			ifs.seekg(sPos);
			ZeroMemory(buff, 256);
			ifs.read(reinterpret_cast<char*>(&size), 1);
			ifs.read(buff, size);
			name = buff;
		}
		//////////////////////////////////////////////////////////////////////////
		// the Free Placed layer 
		int srcPosX, srcPosY, destX, destY, count = 0;	// unique variables to load in for CFreeTiles
		// this is the last layer, so check for end of file
		while (!ifs.eof())
		{
			// read in tile's tileset name (which tileset it came from)
			ZeroMemory(buff, 256);
			ifs.read(reinterpret_cast<char*>(&size), 1);
			ifs.read(buff, size);
			tilesetName = buff;

			// determine which image id this tile should have
			for (int tsIndex = 0; tsIndex < tilesetCount; ++tsIndex)
			{
				if (tilesetName == m_pTilesets[tsIndex].name)
				{
					imageID = m_pTilesets[tsIndex].id;
					break;
				}
			}

			ifs.read(reinterpret_cast<char*>(&srcPosX), 4);
			ifs.read(reinterpret_cast<char*>(&srcPosY), 4);
			ifs.read(reinterpret_cast<char*>(&flag), 4);
			ifs.read(reinterpret_cast<char*>(&destX), 4);
			ifs.read(reinterpret_cast<char*>(&destY), 4);
			ifs.read(reinterpret_cast<char*>(&width), 4);
			ifs.read(reinterpret_cast<char*>(&height), 4);
			ZeroMemory(buff, 256);
			ifs.read(reinterpret_cast<char*>(&size), 1);
			ifs.read(buff, size);
			trigger = buff;
			int rot = 0;
			ifs.read(reinterpret_cast<char*>(&rot), sizeof(int));
			rotation = (float)rot / 10.0f;

			m_pFreeTiles[count++] = CFreeTile(srcPosX, srcPosY, imageID, destX, destY, width, height, flag, trigger, rotation);
			m_nFreeTileCount = count;
		}
	}
	catch(ios_base::failure &)
	{
		if (!ifs.eof())
			throw;
		else
			ifs.close(); return;
	}
}

float CBattleMap::GetZdepthDraw(int xAnchor, int yAnchor, int currTileID)
{
	if (CSGD_DirectInput::GetInstance()->KeyPressed(DIK_D))	// if need to do debugging
		int i = 0;

	//  for the character to be ahead of the object,
	//	it must be directly below, to the southeast, or
	//	to the southwest of an edge tile...
	//	otherwise it is behind the object.
	if (currTileID != -1)
	{
		if (m_pTilesL1[currTileID-(m_nNumCols+1)].Flag() == FLAG_OBJECT_EDGE ||
				m_pTilesL1[currTileID-m_nNumCols].Flag() == FLAG_OBJECT_EDGE ||
				m_pTilesL1[currTileID-1].Flag() == FLAG_OBJECT_EDGE ||
				m_pTilesL1[currTileID-((m_nNumCols+1)<<1)].Flag() == FLAG_OBJECT_EDGE ||
				m_pTilesL1[currTileID-(m_nNumCols+2)].Flag() == FLAG_OBJECT_EDGE ||
				m_pTilesL1[currTileID-(((m_nNumCols+1)<<1)-1)].Flag() == FLAG_OBJECT_EDGE )
		{
			return depth.CHARACTER_AHEAD;
		}
		else
			return depth.CHARACTER_BEHIND;
	} 
	else // shouldn't let it get here...
		return 0;
}

void CBattleMap::DrawDebugInfo()
{
	//char szAnchorPt[64];
	//sprintf_s(szAnchorPt, "A-PT X:%i, Y:%i", turtleX, turtleY);
	//CSGD_Direct3D::GetInstance()->DrawText(szAnchorPt, 5, 5);	

	char szMousePt[64];
	sprintf_s(szMousePt, "M-PT X:%i, Y:%i", m_ptMouseScreenCoord.x, m_ptMouseScreenCoord.y);
	CSGD_Direct3D::GetInstance()->DrawText(szMousePt, 10, 540, 0, 0, 0);	
}

MY_POINT CBattleMap::IsoTilePlot(MY_POINT pt, int xOffset, int yOffset)
{
	MY_POINT newPt;
	newPt.x = (pt.x - pt.y) * (m_nTileWidth >> 1) + xOffset;
	newPt.y = (pt.x + pt.y) * (m_nTileHeight >> 1) + yOffset;
	return newPt;
}

int CBattleMap::IsMousePosValid(POINT mousePt)
{
	int size = m_vCharacters.size();
	for (int i = 0; i < size; ++i)
	{
		RECT currRect = m_vCharacters[i].GetRect();
		if (i < 4 && !m_pPlayer->GetTurtles()[i]->GetAlive())
			continue;
		if (mousePt.x >= currRect.left - (int)m_fScrollX &&
			mousePt.x <= currRect.right - (int)m_fScrollX &&
			mousePt.y >= currRect.top - (int)m_fScrollY &&
			mousePt.y <= currRect.bottom - (int)m_fScrollY)
		{
			return i;
		}
	}
	// determine if the mouse is over an invalid tile for movement
	if (m_pTilesL1[m_nCurrSelectedTile].Flag() != FLAG_COLLISION)
	{
		m_nCurrMouseTileTarget = m_nCurrSelectedTile;
	}
	if (m_pTilesL1[m_nCurrSelectedTile].Flag() != FLAG_OBJECT_EDGE)
	{
		m_nCurrMouseTileTarget = m_nCurrSelectedTile;
	}
	if (m_pTilesL1[m_nCurrSelectedTile].Flag() == FLAG_COLLISION ||
			 m_pTilesL1[m_nCurrSelectedTile].Flag() == FLAG_OBJECT_EDGE)
	{
		m_nCurrMouseTileTarget = m_nCurrSelectedTile = -1;
	}
	return -1;
}

bool CBattleMap::CheckTileOccupied(int tileID)
{
	for (int i = 0; i < m_nNumCharacters; ++i)
		if (m_vCharacters[i].GetCurrTile() == tileID)
			return true;
	return false;
}
void CBattleMap::CalculateRanges()
{
	POINT ptGridLocation = m_vCharacters[m_nCurrCharacter].GetMapCoord();

	// reset alphas
	for(int nx = 2; nx < m_nNumCols; ++nx)
		for(int ny = 2; ny < m_nNumRows; ++ny)
		{
			int id = ny*m_nNumCols+nx;
			if (m_pTilesL1[id].Alpha() != 255)
				m_pTilesL1[id].SetAlpha(255);
		}
	// mark tiles to be drawn with range color
	// scan the neighbors
	int range = m_vCharacters[m_nCurrCharacter].GetCurrAP();
	for(int nx = ptGridLocation.x - range; nx <= ptGridLocation.x + range; ++nx)
	{
		for(int ny = ptGridLocation.y - range; ny <= ptGridLocation.y + range; ++ny)
		{
			//make sure the neighbor is on the map
			if(nx >= 2 && ny >= 2 && nx < m_nNumCols && ny < m_nNumRows
				&& !(nx == ptGridLocation.x && ny == ptGridLocation.y))
			{
				int distance = ((abs(nx - ptGridLocation.x) + abs( ny - ptGridLocation.y) ) << 1);
				int ap		 = m_vCharacters[m_nCurrCharacter].GetCurrAP();
				int id		 = ny*m_nNumCols+nx;
				if ( distance <= ap && m_pTilesL1[id].Flag() != FLAG_OBJECT_EDGE && m_pTilesL1[id].Flag() != FLAG_COLLISION)
				{
					m_pTilesL1[id].SetAlpha(200);
				}
			}
		}
	}
	if (m_nCurrTarget > -1)
	{
		m_nDistanceToTarget = (abs(m_vCharacters[m_nCurrTarget].GetMapCoord().x - m_vCharacters[m_nCurrCharacter].GetMapCoord().x) + 
			abs(m_vCharacters[m_nCurrTarget].GetMapCoord().y - m_vCharacters[m_nCurrCharacter].GetMapCoord().y));
	}
}

void CBattleMap::DrawHover()
{
	if ( m_nCurrBtnSelected == -1)
	{
		switch (m_nHoverCharacter)
		{
		case LEONARDO:
			m_pPlayer->GetTurtles()[LEONARDO]->Colorize();
			m_pPlayer->GetTurtles()[DONATELLO]->Colorize(false);
			m_pPlayer->GetTurtles()[RAPHAEL]->Colorize(false);
			m_pPlayer->GetTurtles()[MIKEY]->Colorize(false);
			break;
		case DONATELLO:
			m_pPlayer->GetTurtles()[DONATELLO]->Colorize();
			m_pPlayer->GetTurtles()[RAPHAEL]->Colorize(false);
			m_pPlayer->GetTurtles()[LEONARDO]->Colorize(false);
			m_pPlayer->GetTurtles()[MIKEY]->Colorize(false);
			break;
		case RAPHAEL:
			m_pPlayer->GetTurtles()[RAPHAEL]->Colorize();
			m_pPlayer->GetTurtles()[DONATELLO]->Colorize(false);
			m_pPlayer->GetTurtles()[LEONARDO]->Colorize(false);
			m_pPlayer->GetTurtles()[MIKEY]->Colorize(false);
			break;
		case MIKEY:
			m_pPlayer->GetTurtles()[MIKEY]->Colorize();
			m_pPlayer->GetTurtles()[DONATELLO]->Colorize(false);
			m_pPlayer->GetTurtles()[LEONARDO]->Colorize(false);
			m_pPlayer->GetTurtles()[RAPHAEL]->Colorize(false);
			break;
		default:
			break;
		}

		if (m_nHoverCharacter > 3)
		{
			if (m_nHoverCharacter == 4 && m_nNumEnemiesLeft > 0)
			{
				m_vEnemies[m_nHoverCharacter-(4)]->Colorize();
				for (int i = 0; i < m_nNumEnemiesLeft; ++i)
					if (m_nHoverCharacter-(4) != i)
						m_vEnemies[i]->Colorize(false);
			}
			else if (m_nHoverCharacter != 4 && m_nNumEnemiesLeft > 0)
			{
				m_vEnemies[m_nHoverCharacter-(4+m_nNumEnemiesKilled)]->Colorize();
				for (int i = 0; i < m_nNumEnemiesLeft; ++i)
					if (m_nHoverCharacter-(4+m_nNumEnemiesKilled) != i)
						m_vEnemies[i]->Colorize(false);
			}
		}
	
		if (m_nHoverCharacter == m_nCurrTarget && m_bIsMouseAttack)
			SetMousePtr(m_pAssets->aMouseAttackID);
		else if (m_nHoverCharacter > 3)
			SetMousePtr(m_pAssets->aMouseMagGlassID);
		else
			SetMousePtr(m_pAssets->aMousePointerID);
	}
}

void CBattleMap::SetStartPositions()
{
	// mapCoordinates represents the grid x=column, y=row
	POINT mapCoordinate;
	mapCoordinate.x = 10;
	mapCoordinate.y = 18;
	m_pPlayer->GetTurtles()[LEONARDO]->SetCurrTile(mapCoordinate, GetOffsetX(), GetOffsetY(), m_nTileWidth, m_nTileHeight, m_nNumCols);

	mapCoordinate.x = 4;
	mapCoordinate.y = 18;
	m_pPlayer->GetTurtles()[DONATELLO]->SetCurrTile(mapCoordinate, GetOffsetX(), GetOffsetY(), m_nTileWidth, m_nTileHeight, m_nNumCols);

	mapCoordinate.x = 3;
	mapCoordinate.y = 17;
	m_pPlayer->GetTurtles()[RAPHAEL]->SetCurrTile(mapCoordinate, GetOffsetX(), GetOffsetY(), m_nTileWidth, m_nTileHeight, m_nNumCols);

	mapCoordinate.x = 4;
	mapCoordinate.y = 17;
	m_pPlayer->GetTurtles()[MIKEY]->SetCurrTile(mapCoordinate, GetOffsetX(), GetOffsetY(), m_nTileWidth, m_nTileHeight, m_nNumCols);
	m_vCharacters.clear();
	for (int i = 0; i < 4; ++i)
		m_vCharacters.push_back((CBase)(*m_pPlayer->GetTurtles()[i]));

	for (int i = 0; i < m_nNumEnemiesLeft; ++i)
	{
		mapCoordinate.x = rand() % (18 - 3) + 2;
		mapCoordinate.y = rand() % (18 - 3) + 2;
		m_vEnemies[i]->SetCurrTile(mapCoordinate, GetOffsetX(), GetOffsetY(), m_nTileWidth, m_nTileHeight, m_nNumCols);
		m_vCharacters.push_back(*m_vEnemies[i]);
	}
}

void CBattleMap::UpdatePositions()	// updates the CPlayer's turtles and the enemy characters
{
	for (int i = 0; i < 4; ++i)
		m_pPlayer->GetTurtles()[i]->SetCurrTile(m_vCharacters[i].GetMapCoord(), GetOffsetX(), GetOffsetY(), m_nTileWidth, m_nTileHeight, m_nNumCols);
	for (int i = 0; i < m_nNumEnemiesLeft; ++i)
		m_vEnemies[i]->SetCurrTile(m_vEnemies[i]->GetMapCoord(), GetOffsetX(), GetOffsetY(), m_nTileWidth, m_nTileHeight, m_nNumCols);
}

bool CBattleMap::HandleKeyBoardInput(float fElapsedTime)
{
	// Keyboard
	if (m_bIsPlayersTurn)
	{
		// camera movement
		if (m_pDI->KeyDown(DIK_A))
		{
			MoveCamLeft(fElapsedTime);
		}
		if (m_pDI->KeyDown(DIK_D))
		{
			MoveCamRight(fElapsedTime);
		}
		if (m_pDI->KeyDown(DIK_S))
		{
			MoveCamDown(fElapsedTime);
		}
		if (m_pDI->KeyDown(DIK_W))
		{
			MoveCamUp(fElapsedTime);
		}
		// character movement
		if (m_nCurrCharacter != -1)
		{
			if (m_pDI->KeyPressed(DIK_NUMPAD7))
			{
				m_nMoveDirection = MOVE_MINUS_X;
			}
			else if (m_pDI->KeyPressed(DIK_NUMPAD3))
			{
				m_nMoveDirection = MOVE_ADD_X;
			}
			else if (m_pDI->KeyPressed(DIK_NUMPAD9))
			{
				m_nMoveDirection = MOVE_MINUS_Y;	
			}
			else if (m_pDI->KeyPressed(DIK_NUMPAD1))
			{
				m_nMoveDirection = MOVE_ADD_Y;
			}
		}
	}
	if (m_pDI->KeyPressed(DIK_SPACE))
	{
		m_bIsPlayersTurn = !m_bIsPlayersTurn;
		// reset APs
		for (int i = 0; i < 4; ++i)
		{
			m_vCharacters[i].SetCurrAP(m_vCharacters[i].GetBaseAP());
			m_pPlayer->GetTurtles()[i]->SetCurrAP(m_vCharacters[i].GetBaseAP());
			if (m_nCurrCharacter > -1)
				CalculateRanges();
			m_pPlayer->GetTurtles()[i]->SetCurrAP(m_pPlayer->GetTurtles()[i]->GetBaseAP());
		}
	}
	if (m_pDI->KeyPressed(DIK_ESCAPE) && !m_bxItemBox && !m_bxSkillBox && !m_bIsPaused)
	{
		CGamePlayState::GetInstance()->SetPaused(true);
		m_pFMOD->PlaySound(m_pAssets->aMMmenuClickSnd);
		if(!m_pFMOD->SetVolume(m_pAssets->aMMmenuClickSnd, CGame::GetInstance()->GetSFXVolume()))
			MessageBox(0, "VOLUME NOT SET", "ERROR", MB_OK);
		return true;
	}
	else if (m_pDI->KeyPressed(DIK_ESCAPE) )
	{
		m_nCurrBtnSelected = BTN_BACK;
		m_pFMOD->PlaySound(m_pAssets->aMMmenuClickSnd);
		if(!m_pFMOD->SetVolume(m_pAssets->aMMmenuClickSnd, CGame::GetInstance()->GetSFXVolume()))
			MessageBox(0, "VOLUME NOT SET", "ERROR", MB_OK);
		HandleButton();
	}
	return true;
}
void CBattleMap::HandleMouseInput(float fElapsedTime, POINT mouse, int xID, int yID)
{
	int index = IsMousePosValid(mouse);
	if (index != -1)
	{
		m_nHoverCharacter = index;
	}
	else
		m_nHoverCharacter = -1;

	// is the mouse over a box button?
	if ((m_bIsPlayersTurn || m_bIsPaused) && m_nCurrCharacter > -1)
	{
		m_nCurrBtnSelected = m_bxActionBox->Input(m_ptMouseScreenCoord);
		if (m_bxSkillBox)
			m_nCurrBtnSelected = m_bxSkillBox->Input(m_ptMouseScreenCoord);
		if (m_bxItemBox)
			m_nCurrBtnSelected = m_bxItemBox->Input(m_ptMouseScreenCoord);
		if (m_bxMessageBox)
			m_nCurrBtnSelected = m_bxMessageBox->Input(m_ptMouseScreenCoord);
		if (m_nCurrBtnSelected > -1)
			SetMousePtr(m_pAssets->aMousePointerID);
	}
	if (m_bxPauseBox)
		m_nCurrBtnSelected = m_bxPauseBox->Input(m_ptMouseScreenCoord);
	else if (m_bxLoadBox)
		m_nCurrBtnSelected = m_bxLoadBox->Input(m_ptMouseScreenCoord);
	else if(m_bxSaveBox)
		m_nCurrBtnSelected = m_bxSaveBox->Input(m_ptMouseScreenCoord);

	// when the left mouse button is clicked...handling non-box input only
	if (m_pDI->MouseButtonPressed(MOUSE_LEFT))
	{
		// for movement, see if the tile is open
		if (m_nCurrMouseTileTarget != -1 && m_nCurrCharacter > -1 && 
			!m_bxSkillBox && !m_bxItemBox && !m_bIsPaused && !m_bxMessageBox && !m_bxActionBox->IsMouseInBox())
		{
			// it's an open tile...

			// is tile occupied?
			bool bOccupied = false;	
			int tileID = yID * m_nNumCols + xID;
			for (int i = 0; i < m_nNumCharacters; ++i)
				if (m_vCharacters[i].GetCurrTile() == tileID)
				{bOccupied = true; return; }

			// the user has clicked for the second time on the same location to move to..moving now
			if (m_bPathDisplayed && !bOccupied && m_ptEndCoord.x == xID && m_ptEndCoord.y == yID)
					m_bMoving = true;
			// reset..a new path needs to be evaluated
			else if (m_bPathDisplayed && !bOccupied && (m_ptEndCoord.x != xID || m_ptEndCoord.y != yID) )
			{
				// reset alphas
				for(int nx = 2; nx < m_nNumCols; ++nx)
					for(int ny = 2; ny < m_nNumRows; ++ny)
					{
						int id = ny*m_nNumCols+nx;
						if (m_pTilesL1[id].Alpha() != 255)
							m_pTilesL1[id].SetAlpha(255);
					}
					CalculateRanges();
			}
			// the click was good, find the path
			m_ptEndCoord.x = xID; m_ptEndCoord.y = yID;
			FindPathToTarget();
			m_bPathDisplayed = true;
		}
		if (m_nCurrMouseTileTarget != -1 && m_nCurrCharacter > -1 || m_bIsPaused)
			HandleButton();
	}
	else if (m_pDI->MouseButtonPressed(MOUSE_RIGHT) && !m_bIsPaused )
	{
		if (index > -1 && index < 4 && !m_bHaveMoved)	// see if we're clicking on a character (turtle), and we haven't already moved
		{
			m_nCurrSkillCost	 = -1;
			m_nCurrCharacter	 = index;
			m_nCurrCharacterTile = m_vCharacters[index].GetCurrTile();
			if (m_pPlayer->GetTurtles()[m_nCurrCharacter]->GetCurrSelectedSkillIndex() > -1)
			{
				CSkill* skills = m_pPlayer->GetTurtles()[m_nCurrCharacter]->GetCurrSelectedSkill();
				m_sCurrSkillName = skills->GetSkillName();
				m_nCurrSkillCost = skills->GetSkillCost();
			}
			else
				m_sCurrSkillName = "NONE";
			if (m_bxSkillBox)
			{
				delete m_bxSkillBox;
				int numSkills = m_pPlayer->GetTurtles()[m_nCurrCharacter]->GetNumSkills();
				string* skillNames = new string[numSkills+1];
				for (int i = 0; i < numSkills; ++i)
				{
					vector<CSkill> skills = *(m_pPlayer->GetTurtles()[m_nCurrCharacter]->GetSkills());
					skillNames[i] = skills[i].GetSkillName();
					char temp[16];
					sprintf_s(temp, " - %i", skills[i].GetSkillCost());
					string cost = temp;
					skillNames[i] += cost;
				}
				skillNames[numSkills] = "NONE";
				m_bxSkillBox = new CBox(numSkills+1, skillNames, m_bxActionBox->BoxRight()+20, 470, depth.MENUS-0.04f, false, 35, 35, 15, -1, 0.7f );
				delete[] skillNames;
				m_bxSkillBox->SetActive(true);
				m_bxSkillBox->SetType(BOX_WITH_BACK);
				m_bxActionBox->SetActive(false);
			}
			CalculateRanges();
		}
		else if (index > 3 && m_nCurrTarget != m_nHoverCharacter && m_nCurrCharacter > -1) // or if we're clicking on an enemy to view their stats
		{
			m_bIsMouseAttack = true;
			m_nCurrTarget = index;
			m_ncurrTargetTile = m_vCharacters[m_nCurrTarget].GetCurrTile();
			m_nDistanceToTarget = (abs(m_vCharacters[m_nCurrTarget].GetMapCoord().x - m_vCharacters[m_nCurrCharacter].GetMapCoord().x) + 
								   abs(m_vCharacters[m_nCurrTarget].GetMapCoord().y - m_vCharacters[m_nCurrCharacter].GetMapCoord().y));
		}
		else if (index > 3 && m_nCurrTarget == m_nHoverCharacter && m_nCurrCharacter > -1)	// otherwise, attempting to attack
		{
			// check if in range
			if (m_nDistanceToTarget > m_vCharacters[m_nCurrCharacter].GetRange() && m_sCurrSkillName == "NONE")
			{ m_bOutOfRange = true; return; }
			else if (m_sCurrSkillName != "NONE" && m_nDistanceToTarget > m_pPlayer->GetTurtles()[m_nCurrCharacter]->GetCurrSelectedSkill()->GetRange())
			{ m_bOutOfRange = true; return; }
			// check if enough AP for a regular attack
			if (m_vCharacters[m_nCurrCharacter].GetCurrAP() < 4 && m_sCurrSkillName == "NONE")
			{ m_bNotEnoughAP = true; return; }
			// check if enough AP for the selected skill
			else if (m_sCurrSkillName != "NONE" && m_vCharacters[m_nCurrCharacter].GetCurrAP() < m_nCurrSkillCost)
			{ m_bNotEnoughAP = true; return; }
			else
				PerformAttack();
		}
		else if (index > -1 && index < 4 && m_bHaveMoved)
		{
			string* message = new string[3];
			message[0] = "CAN ONLY MOVE"; message[1] = "ONE TURTLE"; message[2] = "PER TURN";
			m_bxMessageBox = new CBox(3, message, 320, 300, 0.11f, false, 25, 25, 25, -1, 0.7f);
			m_bxMessageBox->SetType(BOX_WITH_BACK); m_bxMessageBox->SetActive();
			m_bxActionBox->SetActive(false);
			m_bxMessageBox->IsMsgBox(true);
			delete[] message;
		}
	}
}
void CBattleMap::HandleButton()
{
	// check to see if a skill was selected
	if (m_bxSkillBox && m_nCurrBtnSelected > -1 && m_nCurrBtnSelected < m_pPlayer->GetTurtles()[m_nCurrCharacter]->GetNumSkills())
	{
		m_pPlayer->GetTurtles()[m_nCurrCharacter]->SetSelectedSkill(m_nCurrBtnSelected);
		vector<CSkill> skills = *(m_pPlayer->GetTurtles()[m_nCurrCharacter]->GetSkills());
		m_sCurrSkillDisplay = m_sCurrSkillName = skills[m_nCurrBtnSelected].GetSkillName();
		m_nCurrSkillCost = skills[m_nCurrBtnSelected].GetSkillCost();
		char temp[16];
		sprintf_s(temp, ": %i", m_nCurrSkillCost);
		string cost = temp;
		m_sCurrSkillDisplay += cost;
		delete m_bxSkillBox;
		m_bxSkillBox = NULL;
		m_bDisplaySpecialBox = false;
		m_bxActionBox->SetActive();
		return;
	}
	// or if they chose "NONE"
	else if (m_bxSkillBox && m_nCurrBtnSelected == m_pPlayer->GetTurtles()[m_nCurrCharacter]->GetNumSkills())
	{
		m_sCurrSkillDisplay = m_sCurrSkillName = "NONE"; 
		delete m_bxSkillBox; m_bxSkillBox = NULL;
		m_bDisplaySpecialBox = false;
		m_bxActionBox->SetActive();
		return;
	}
	else if (m_bxPauseBox)
	{
		if (m_nCurrBtnSelected == 3 /*Quit*/)
		{
			delete m_bxPauseBox; m_bxPauseBox = NULL;
			CGame::GetInstance()->ChangeState(CMainMenuState::GetInstance());
			return;
		}
		else if (m_nCurrBtnSelected == 2 /*Load*/)
		{
			// TODO:: get the current profile's saved game, set up the load game box, handle results elsewhere
			delete m_bxPauseBox; m_bxPauseBox = NULL;
			string* sLoadGame = new string[2];
			sLoadGame[0] = "LOAD GAME"; sLoadGame[1] = "Bob's saved game";
			m_bxLoadBox = new CBox(2, sLoadGame, 230, 300, 0.11f, true);
			m_bxLoadBox->SetType(BOX_WITH_BACK);
			m_bxLoadBox->SetActive();
			delete[] sLoadGame;
			return;
		}
		else if (m_nCurrBtnSelected == 1 /*Save*/)
		{
			delete m_bxPauseBox; m_bxPauseBox = NULL;
			string* sSaveGame = new string[2];
			sSaveGame[0] = "SAVE GAME"; sSaveGame[1] = "MY SAVED GAME...";
			m_bxSaveBox = new CBox(2, sSaveGame, 230, 300, 0.11f, true);
			m_bxSaveBox->SetType(BOX_WITH_BACK);
			m_bxSaveBox->SetActive();
			delete[] sSaveGame;
			return;
		}
	}
	// catches any random invalid input
	else if ((m_bxItemBox || m_bxSkillBox || m_bxItemBox || m_bxSaveBox || m_bxLoadBox) && m_nCurrBtnSelected != BTN_BACK)
		return;

	switch (m_nCurrBtnSelected)
	{
	case BTN_SPECIAL:
		{
			if (m_nCurrCharacter > -1)
			{
				m_bDisplaySpecialBox = true;
				if (m_bxSkillBox)
					delete m_bxSkillBox;
				int numSkills = m_pPlayer->GetTurtles()[m_nCurrCharacter]->GetNumSkills();
				string* skillNames = new string[numSkills+1];
				for (int i = 0; i < numSkills; ++i)
				{
					vector<CSkill> skills = *(m_pPlayer->GetTurtles()[m_nCurrCharacter]->GetSkills());
					skillNames[i] = skills[i].GetSkillName();
					char temp[16];
					sprintf_s(temp, ": %i", skills[i].GetSkillCost());
					string cost = temp;
					skillNames[i] += cost;
				}
				skillNames[numSkills] = "NONE";
				m_bxSkillBox = new CBox(numSkills+1, skillNames, m_bxActionBox->BoxRight()+20, 470, depth.MENUS-0.04f, false, 35, 35, 15, -1, 0.7f );
				delete[] skillNames;
				m_bxSkillBox->SetActive(true);
				m_bxSkillBox->SetType(BOX_WITH_BACK);
				m_bxActionBox->SetActive(false);
// 				m_bxCurrActiveBox = m_bxSkillBox;
// 				m_bxCurrActiveBox->BoxType(BX_SKILL);
			}
		}
		break;
	case BTN_ITEM:
		{
			if (m_nCurrCharacter > -1)
			{
				m_bDisplayItemBox = true;
				if (m_bxItemBox)
					delete m_bxItemBox;

				int numItems = m_pPlayer->GetInstance()->GetNumItems();
				vector<CBase>* temp = m_pPlayer->GetInstance()->GetItems();
				
				string* item = new string[temp->size()];

				for(int i = 0; i < (int)temp->size(); i++)
				{
					item[i] = (*temp)[i].GetName();
				}

				m_bxItemBox = new CBox(m_pPlayer->GetInstance()->GetNumItems(), item, m_bxActionBox->BoxRight()+20, 400, depth.MENUS-0.04f);
				delete[] item;
				m_bxItemBox->SetActive(true);
				m_bxItemBox->SetType(BOX_WITH_BACK);
				m_bxActionBox->SetActive(false);
// 				m_bxCurrActiveBox = m_bxItemBox;
// 				m_bxCurrActiveBox->BoxType(BX_ITEM);
			}
		}
		break;
	case BTN_ENDTURN:
		m_bIsPlayersTurn = false;
		for (int i = 0; i < 4; ++i)
		{
			m_vCharacters[i].SetCurrAP(m_vCharacters[i].GetBaseAP());
			m_pPlayer->GetTurtles()[i]->SetCurrAP(m_vCharacters[i].GetBaseAP());
			if (m_nCurrCharacter > -1)
				CalculateRanges();
			m_pPlayer->GetTurtles()[i]->SetCurrAP(m_pPlayer->GetTurtles()[i]->GetBaseAP());
		}
		break;
	case BTN_BACK:
		if (m_bxSkillBox)
		{
			delete m_bxSkillBox;
			m_bxSkillBox = NULL;
			m_bDisplaySpecialBox = false;
			m_bxActionBox->SetActive();
		}
		if (m_bxItemBox)
		{
			delete m_bxItemBox;
			m_bxItemBox = NULL;
			m_bDisplayItemBox = false;
			m_bxActionBox->SetActive();
		}
		if (m_bxPauseBox && (!m_bxLoadBox && !m_bxSaveBox))
		{
			delete m_bxPauseBox;
			m_bxPauseBox = NULL;
			CGamePlayState::GetInstance()->SetPaused(false);
		}
		if (m_bxSaveBox)
		{
			delete m_bxSaveBox;
			m_bxSaveBox = NULL;
			string text[4]; text[1] = "SAVE GAME"; text[2] = "LOAD GAME"; text[3] = "QUIT"; text[0] = "PAUSED";
			m_bxPauseBox = new CBox(4, text, 320, 300, 0.1f, true);
			m_bxPauseBox->SetActive();
			m_bxPauseBox->SetType(BOX_WITH_BACK);
		}
		if (m_bxLoadBox)
		{
			delete m_bxLoadBox;
			m_bxLoadBox = NULL;
			string text[4]; text[1] = "SAVE GAME"; text[2] = "LOAD GAME"; text[3] = "QUIT"; text[0] = "PAUSED";
			m_bxPauseBox = new CBox(4, text, 320, 300, 0.1f, true);
			m_bxPauseBox->SetActive();
			m_bxPauseBox->SetType(BOX_WITH_BACK);
		}
		if (m_bxMessageBox)
		{
			delete m_bxMessageBox; m_bxMessageBox = NULL;
			m_bxActionBox->SetActive();
		}
	default:
		break;
	}
}
void CBattleMap::PerformAttack()
{
	// no skill selected
	if (m_sCurrSkillName == "NONE")
	{
		int damage = ( (m_vCharacters[m_nCurrCharacter].GetStrength() - m_vCharacters[m_nCurrTarget].GetDefense()) + m_vCharacters[m_nCurrCharacter].GetAccuracy()) * 2;
		damage += rand() % (5 - (-4)) -5;
	
		m_vCharacters[m_nCurrCharacter].DecrementCurrAP(4);
		m_pPlayer->GetTurtles()[m_nCurrCharacter]->SetExperience(m_pPlayer->GetTurtles()[m_nCurrCharacter]->GetExperience()+10);
	
		m_vCharacters[m_nCurrTarget].SetHealth(m_vCharacters[m_nCurrTarget].GetHealth() - damage);
		int index = m_nCurrTarget - 4;
		if (m_nCurrTarget > 0 && m_nNumEnemiesKilled > 0 && index > 0)
			index -= m_nNumEnemiesKilled;
		m_vEnemies[index]->SetHealth(m_vCharacters[m_nCurrTarget].GetHealth() - damage);
		
		/*POINT point;
		point.x = m_vEnemies[index]->GetPosX();
		point.y = m_vEnemies[index]->GetPosY();
		m_pParticleSystem[FIRE].UpdateParticle(0,point);
		m_pParticleSystem[FIRE].DrawParticle();*/

		if (m_vEnemies[index]->GetHealth() <= 0)
		{
			m_pPlayer->GetTurtles()[m_nCurrCharacter]->SetExperience(m_pPlayer->GetTurtles()[m_nCurrCharacter]->GetExperience()+30);
			
			vector<CBase*>::iterator iter = m_vEnemies.begin();
			int count = 0;
			while(iter != m_vEnemies.end())
			{
				if ((*iter) == m_vEnemies[index])
				{
					iter = m_vEnemies.erase(iter);
					break;
				}
				++count;
				++iter;
			}
			--m_nNumCharacters;
			--m_nNumEnemiesLeft;
			++m_nNumEnemiesKilled;
			m_ncurrTargetTile = -1;
			m_nCurrTarget = -1;
		}
	} 
	// a skill has been selected, execute that skill
	else
	{
		m_bExecuteSkill = true;
		m_vCharacters[m_nCurrCharacter].DecrementCurrAP(m_nCurrSkillCost);
		m_pPlayer->GetTurtles()[m_nCurrCharacter]->SetCurrAP(m_vCharacters[m_nCurrCharacter].GetCurrAP());
		m_pPlayer->GetTurtles()[m_nCurrCharacter]->SetExperience(m_pPlayer->GetTurtles()[m_nCurrCharacter]->GetExperience()+15);
		m_pPlayer->GetTurtles()[m_nCurrCharacter]->SetSkillXP(m_pPlayer->GetTurtles()[m_nCurrCharacter]->GetSkillXP()+1);
	}
	CalculateRanges();
}
void CBattleMap::FindPathToTarget()
{
	m_vPath.clear();
	POINT ptCurr = m_vCharacters[m_nCurrCharacter].GetMapCoord(); // begin point
	POINT ptTarget = m_ptEndCoord;	// end point
	int range = m_vCharacters[m_nCurrCharacter].GetCurrAP();	// max distance moveable
	int pathWeight = 0;
	vector<int> pathX;
	vector<int> pathY;
	int oldX = ptCurr.x;
	int oldY = ptCurr.y;

	while (true)
	{
		if (pathWeight-4 > -1)
		{
			if ( (ptCurr.x == pathX[pathWeight-4]) && (ptCurr.y == pathY[pathWeight-4]) )
			{ pathWeight = 10000; break; }
		}
		if (pathWeight-1 > -1)
		{
			pathX.push_back(ptCurr.x);
			pathY.push_back(ptCurr.y);
			m_vPath.push_back(ptCurr);
			oldX = pathX[pathWeight-1];
			oldY = pathY[pathWeight-1];
		}
		++pathWeight;
		if (pathWeight*2 > m_pPlayer->GetTurtles()[m_nCurrCharacter]->GetCurrAP())
			break;
		if (ptTarget.x == ptCurr.x && ptTarget.y == ptCurr.y)
			break;
		if (pathWeight > m_nNumCols)
		{pathWeight = 10000; break;}

		if (ptCurr.x < ptTarget.x && (oldX != ptCurr.x + 1) && 
			m_pTilesL1[ptCurr.y * m_nNumCols + (ptCurr.x+1)].Flag() == FLAG_NONE &&
			!CheckTileOccupied(ptCurr.y * m_nNumCols + (ptCurr.x+1)))
		{ ++ptCurr.x; continue; }
		if (ptCurr.x > ptTarget.x && (oldX != ptCurr.x - 1) && 
			m_pTilesL1[ptCurr.y * m_nNumCols + (ptCurr.x-1)].Flag() == FLAG_NONE &&
			!CheckTileOccupied(ptCurr.y * m_nNumCols + (ptCurr.x-1)))
		{ --ptCurr.x; continue; }
		if (ptCurr.y < ptTarget.y && (oldY != ptCurr.y + 1) && 
			m_pTilesL1[(ptCurr.y+1) * m_nNumCols + ptCurr.x].Flag() == FLAG_NONE &&
			!CheckTileOccupied((ptCurr.y+1) * m_nNumCols + ptCurr.x))
		{ ++ptCurr.y; continue; }
		if (ptCurr.y > ptTarget.y && (oldY != ptCurr.y - 1) && 
			m_pTilesL1[(ptCurr.y-1) * m_nNumCols + ptCurr.x].Flag() == FLAG_NONE &&
			!CheckTileOccupied((ptCurr.y-1) * m_nNumCols + ptCurr.x))
		{ --ptCurr.y; continue; }

		if (ptCurr.x == ptTarget.x)
		{
			if (oldX != ptCurr.x+1 && m_pTilesL1[ptCurr.y * m_nNumCols + ptCurr.x+1].Flag() == FLAG_NONE &&
				!CheckTileOccupied(ptCurr.y * m_nNumCols + ptCurr.x+1) )
			{ ++ptCurr.x; continue; }
			if (oldX != ptCurr.x-1 && m_pTilesL1[ptCurr.y * m_nNumCols + ptCurr.x-1].Flag() == FLAG_NONE &&
				!CheckTileOccupied(ptCurr.y * m_nNumCols + ptCurr.x-1))
			{ --ptCurr.x; continue; }
		}
		else if (ptCurr.y == ptTarget.y)
		{
			if (oldY != ptCurr.y+1 && m_pTilesL1[(ptCurr.y+1) * m_nNumCols + ptCurr.x].Flag() == FLAG_NONE &&
				!CheckTileOccupied((ptCurr.y+1) * m_nNumCols + ptCurr.x))
			{ ++ptCurr.y; continue; }
			if (oldY != ptCurr.y-1 && m_pTilesL1[(ptCurr.y-1) * m_nNumCols + ptCurr.x].Flag() == FLAG_NONE &&
				!CheckTileOccupied((ptCurr.y-1) * m_nNumCols + ptCurr.x))
			{ --ptCurr.y; continue; }
		}
		if ( (ptCurr.x > ptTarget.x) && (oldX != ptCurr.x+1) && m_pTilesL1[ptCurr.y * m_nNumCols + (ptCurr.x+1)].Flag() == FLAG_NONE &&
			!CheckTileOccupied(ptCurr.y * m_nNumCols + (ptCurr.x+1)))
		{++ptCurr.x; continue;}
		else if (oldX != ptCurr.x-1 && m_pTilesL1[ptCurr.y * m_nNumCols + (ptCurr.x-1)].Flag() == FLAG_NONE &&
			!CheckTileOccupied(ptCurr.y * m_nNumCols + (ptCurr.x-1)))
		{--ptCurr.x; continue;}
		if ( (ptCurr.y > ptTarget.y) && (oldY != ptCurr.y+1) && m_pTilesL1[(ptCurr.y+1) * m_nNumCols + ptCurr.x].Flag() == FLAG_NONE &&
			!CheckTileOccupied((ptCurr.y+1) * m_nNumCols + ptCurr.x))
		{++ptCurr.y; continue;}
		else if (oldY != ptCurr.y-1 && m_pTilesL1[(ptCurr.y-1) * m_nNumCols + ptCurr.x].Flag() == FLAG_NONE &&
			!CheckTileOccupied((ptCurr.y-1) * m_nNumCols + ptCurr.x))
		{--ptCurr.y; continue;}
		pathWeight = 10000; break;
	}
	if (m_vPath.size() > 0)
	{
		m_ptStartXY.x = m_vCharacters[m_nCurrCharacter].GetPosX();
		m_ptStartXY.y = m_vCharacters[m_nCurrCharacter].GetPosY();
		for (unsigned int i = 0; i < m_vPath.size(); ++i)
			m_pTilesL1[m_vPath[i].y * m_nNumCols + m_vPath[i].x].SetAlpha(199);
	}
}

void CBattleMap::MoveCamUp(float fElapsedTime)
{
	m_fScrollY += SCROLLSPEED * fElapsedTime;
	// tile offsets
	SetOffsetY((int)m_fScrollY + m_nIsoCenterLeftY - (m_nTileHeight >> 1));
	SetFTosY((int)m_fScrollY - m_nTileHeight);
	for (int i = 0; i < m_nNumCharacters; ++i)
		m_vCharacters[i].SetCurrTile(m_vCharacters[i].GetMapCoord(), GetOffsetX(), GetOffsetY(), m_nTileWidth, m_nTileHeight, m_nNumCols);
	UpdatePositions();
}
void CBattleMap::MoveCamDown(float fElapsedTime)
{
	m_fScrollY -= SCROLLSPEED * fElapsedTime;
	// tile offsets
	SetOffsetY((int)m_fScrollY + m_nIsoCenterLeftY - (m_nTileHeight >> 1));
	SetFTosY((int)m_fScrollY - m_nTileHeight);
	for (int i = 0; i < m_nNumCharacters; ++i)
		m_vCharacters[i].SetCurrTile(m_vCharacters[i].GetMapCoord(), GetOffsetX(), GetOffsetY(), m_nTileWidth, m_nTileHeight, m_nNumCols);
	UpdatePositions();
}
void CBattleMap::MoveCamLeft(float fElapsedTime)
{
	m_fScrollX += SCROLLSPEED * fElapsedTime;
	// tile offsets
	SetOffsetX((int)m_fScrollX + m_nIsoCenterTopX - (m_nTileWidth >> 1));
	SetFTosX((int)m_fScrollX - ((m_nMapWidth >> 1) - (m_nScrenWidth >> 1)) - (m_nTileWidth >> 1));
	for (int i = 0; i < m_nNumCharacters; ++i)
		m_vCharacters[i].SetCurrTile(m_vCharacters[i].GetMapCoord(), GetOffsetX(), GetOffsetY(), m_nTileWidth, m_nTileHeight, m_nNumCols);
	UpdatePositions();
}
void CBattleMap::MoveCamRight(float fElapsedTime)
{
	m_fScrollX -= SCROLLSPEED * fElapsedTime;
	// tile offsets
	SetOffsetX((int)m_fScrollX + m_nIsoCenterTopX - (m_nTileWidth >> 1));
	SetFTosX((int)m_fScrollX - ((m_nMapWidth >> 1) - (m_nScrenWidth >> 1)) - (m_nTileWidth >> 1));
	for (int i = 0; i < m_nNumCharacters; ++i)
		m_vCharacters[i].SetCurrTile(m_vCharacters[i].GetMapCoord(), GetOffsetX(), GetOffsetY(), m_nTileWidth, m_nTileHeight, m_nNumCols);
	UpdatePositions();
}
void CBattleMap::CenterCam(float fElapsedTime)
{

}

void CBattleMap::DrawBoxes()
{
	m_bxActionBox->Render();
	if (m_bDisplaySpecialBox)
		m_bxSkillBox->Render();
	if(m_bDisplayItemBox)
		m_bxItemBox->Render();
	if(m_bxLoadBox)
		m_bxLoadBox->Render();
	if(m_bxSaveBox)
		m_bxSaveBox->Render();
	if(m_bxMessageBox)
		m_bxMessageBox->Render();
}
void CBattleMap::cheat()
{
	switch(godcheat)
	{
	case 0:
		if (CSGD_DirectInput::GetInstance()->KeyPressed(DIK_G))
			godcheat++;
		else if (CSGD_DirectInput::GetInstance()->CheckBufferedKeysEx() != 0)
			godcheat = 0;
		break;
	case 1:
		if (CSGD_DirectInput::GetInstance()->KeyPressed(DIK_O))
			godcheat++;
		else if (CSGD_DirectInput::GetInstance()->CheckBufferedKeysEx() != 0)
			godcheat = 0;
		break;
	case 2:
		if (CSGD_DirectInput::GetInstance()->KeyPressed(DIK_D))
			godcheat++;
		else if (CSGD_DirectInput::GetInstance()->CheckBufferedKeysEx() != 0)
			godcheat = 0;
		break;
	}
	if( godcheat == 3)
	{
		godbool = !godbool;
		godcheat = 0;
	}
	
	switch(APcheat)
	{
	case 0:
		if (CSGD_DirectInput::GetInstance()->KeyPressed(DIK_Q))
			APcheat++;
		else if (CSGD_DirectInput::GetInstance()->CheckBufferedKeysEx() != 0)
			APcheat = 0;
		break;
	case 1:
		if (CSGD_DirectInput::GetInstance()->KeyPressed(DIK_W))
			APcheat++;
		else if (CSGD_DirectInput::GetInstance()->CheckBufferedKeysEx() != 0)
			APcheat = 0;
		break;
	case 2:
		if (CSGD_DirectInput::GetInstance()->KeyPressed(DIK_E))
			APcheat++;
		else if (CSGD_DirectInput::GetInstance()->CheckBufferedKeysEx() != 0)
			APcheat = 0;
		break;
	case 3:
		if (CSGD_DirectInput::GetInstance()->KeyPressed(DIK_R))
			APcheat++;
		else if (CSGD_DirectInput::GetInstance()->CheckBufferedKeysEx() != 0)
			APcheat = 0;
		break;
	case 4:
		if (CSGD_DirectInput::GetInstance()->KeyPressed(DIK_T))
			APcheat++;
		else if (CSGD_DirectInput::GetInstance()->CheckBufferedKeysEx() != 0)
			APcheat = 0;
		break;
	case 5:
		if (CSGD_DirectInput::GetInstance()->KeyPressed(DIK_Y))
			APcheat++;
		else if (CSGD_DirectInput::GetInstance()->CheckBufferedKeysEx() != 0)
			APcheat = 0;
		break;
	}
	if( APcheat == 3)
	{
		APbool = !APbool;
		APcheat = 0;
	}
}