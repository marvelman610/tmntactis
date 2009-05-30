#pragma once

class CParticleSystem
{
private:
	// make vector
	float m_fPosX;
	float m_fPosY;
	// make vector
	float m_fVelX;
	float m_fVelY;
	// make vector
	float m_fGravity;

	float m_fWeight;
	float m_fLifespan;
	float m_fCurrentAge;
	D3DColor m_cColor;
	DWORD m_dwSize;
	char* m_szTextureName;
	DIRECT3DTEXTURE* m_pParticleTexture;
	DIRECT3DVERTEXBUFFER9* m_pVertexBuffer;
	
	// Constructor
	CParticleSystem(void);
	// Copy Constructor
	CParticleSystem(const CParticleSystem&);
	// Assignment Operator
	CParticleSystem& operator=(const CParticleSystem&);
	// Destructor
	~CParticleSystem(void);
public:
	//////////////////////////////////////////////////////////////////
	//	Function:	Initialize
	//
	//	Purpose:	To initialize the particle system and its objects
	//////////////////////////////////////////////////////////////////
	void Initialize(void);
	//////////////////////////////////////////////////////////////////
	//	Function:	Render
	//
	//	Purpose:	To render all the managed particles
	/////////////////////////////////////////////////////////////////
	void Render(void);
	/////////////////////////////////////////////////////////////////
	//	Function:	Update
	//
	//	Purpose:	To update the managed particles
	/////////////////////////////////////////////////////////////////
	void Update(float fElapsedTime);
	/////////////////////////////////////////////////////////////////
	//	Function:	GetInstance
	//
	//	Purpose: returns the instance of the singleton
	/////////////////////////////////////////////////////////////////
	static CParticleSystem* GetInstance(void);
};