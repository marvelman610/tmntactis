////////////////////////////////////////////////////////////////////////////
//	File Name	:	"CGamePlayState.h"
//
//	Author		:	Matt DiMatteo (MD)
//
//	Purpose		:	The Gameplay state derived from the base state
//					to hold all data needed to play the game.
////////////////////////////////////////////////////////////////////////////
#pragma once
#include "IGameState.h"
#include "SGD Wrappers/CSGD_Direct3D.h"
#include "SGD Wrappers/CSGD_TextureManager.h"
#include "SGD Wrappers/CSGD_DirectSound.h"
#include "SGD Wrappers/CSGD_WaveManager.h"
#include "SGD Wrappers/CSGD_DirectInput.h"
#include "CSGD_ObjectFactory.h"
#include "CSGD_ObjectManager.h"
#include "CEnemy.h"
#include "CPlayer.h"
#include"CBitmapFont.h"
#include "CGame.h"
#include <string>
using std::string;

class CGamePlayState : public IGameState
{
private:
	int m_nBackground;
	int m_nSoundID1;
	int m_nCHEATID;
	int m_nGameOver;

	int fireball;
	float timer;

	int numenemies;
	
	float enemiesKilled;
	int totalScore;
	float accuracy;
	float bulletsFired;

	int blank;

	float m_fElapsedTime;
	DWORD m_dwPreviousTime;

	int m_nTime;

	float tally;
	int count;

	int playercount;
	float m_fGameTime;

	CBitmapFont font;
	int m_nCurrentMenuSelection;
	int m_nCursor;
	int gunsound;

	CSGD_Direct3D* m_pD3D;
	CSGD_TextureManager* m_pTM;
	CSGD_DirectInput* m_pDI;
	CSGD_WaveManager* m_pWM;
	CSGD_DirectSound* m_pDS;
	CSGD_ObjectFactory<string, CBase>* m_pOF;
	CSGD_ObjectManager* m_pOM;

	bool bIsPaused;
	bool gameover;
	int lives;

	int boss;
	int points;

	float pos;
	int pointscheat;

	///////////////////////////////////////////////////////////////////
	//	Function:	"CGamePlayState(Constructor)"
	///////////////////////////////////////////////////////////////////
	CGamePlayState(void) {}
	CGamePlayState(const CGamePlayState&);
	CGamePlayState& operator=(const CGamePlayState&);

	///////////////////////////////////////////////////////////////////
	//	Function:	"~CGamePlayState(Destructor)"
	///////////////////////////////////////////////////////////////////
	~CGamePlayState(void) {}

public:
	//////////////////////////////////////
	//	Function:	Accessors
	//	Purpose	:	To get the specified type
	///////////////////////////////////////
	//inline int GetScore() {return m_nScore;}
	inline float GetPos() {return pos;}
	inline bool GetPaused(){return bIsPaused;}
	inline int GetBoss() {return boss;}
	inline int GetNumEnemies() {return numenemies;}
	inline int GetPoints() {return points;}
	inline int Gettscore() {return totalScore;}

	inline float GetenemiesKilled() {return enemiesKilled;}
	inline int GettotalScore()    {return totalScore;}
	inline float Getaccuracy()    {return accuracy;}
	inline float GetbulletsFired()  {return bulletsFired;}

	int GetPlayercount() {return playercount;}
  
	//////////////////////////////////////
	//	Function:	Mutators
	//
	//	Purpose	:	To set the specified type
	///////////////////////////////////////
	inline void SetPlayerCount(int count) {playercount = count;}
	//inline void SetScore(int score) {m_nScore = score;}
	inline void SetPos(float f) {pos = f;}
	inline void SetBoss(int b) {boss = b;}
	inline void SetNumEnemies(int a) {numenemies = a;}
	inline void SetPoints(int a) {points = a;}

	inline void SetenemiesKilled(float e) {enemiesKilled = e;}
	inline void SettotalScore(int s)    {totalScore = s;}
	inline void Setaccuracy(float a)    {accuracy = a;}
	inline void SetbulletsFired(float f)  {bulletsFired = f;}
	
	///////////////////////////////////////////////////////////////////
	// Function: "GetInstance"
	//
	//  Purpose: Gets instance to the only instance of the class.
	///////////////////////////////////////////////////////////////////
	static CGamePlayState* GetInstance();

	///////////////////////////////////////////////////////////////////
	// Function: "Enter"
	//
	//  Purpose: Initializes variables and pointers upon entering the state.
	///////////////////////////////////////////////////////////////////
	void Enter();
		
	///////////////////////////////////////////////////////////////////
	// Function: "Input"
	//
	//  Purpose: Receives input from player.
	///////////////////////////////////////////////////////////////////
	bool Input();

	///////////////////////////////////////////////////////////////////
	// Function: "Update"
	//
	// Purpose: Updates game objects based on time.
	////////////////////////////////////////////////////////////////////
	void Update();

	////////////////////////////////////////////////////////////////////
	// Function: “Render”
	//
	// Purpose: Draws game objects to the screen.
	////////////////////////////////////////////////////////////////////
	void Render();

	////////////////////////////////////////////////////////////////////
	// Function: “Exit”
	//
	// Purpose: Unloads any assets aquired since entering. It does this 
	//			and then exits the state .
	////////////////////////////////////////////////////////////////////
	void Exit();
	
	////////////////////////////////////////////////////////////////////
	// Function: “cheat”
	//
	// Purpose: Handles cheat input
	////////////////////////////////////////////////////////////////////
	void cheat();

};