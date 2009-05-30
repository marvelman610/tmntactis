#include "CParticleSystem.h"

CParticleSystem::CParticleSystem(void)
{
	m_fPosX = m_fPosY = m_fVelX = m_fVelY = m_fGravity = m_fWeight = m_fLifespan = m_fCurrentAge = 0.0f;
	m_cColor = D3DCOLOR_XRGB(0,0,0);
	m_dwSize = 0;
	m_szTextureName = NULL;
	m_pParticleTexture = NULL;
	m_pVertexBuffer = NULL;
}

CParticleSystem::~CParticleSystem(void)
{
}

void CParticleSystem::Initialize(void)
{
	// load binary file
}

void CParticleSystem::Render(void)
{
	//read in each particles attributes first then render
	//each his own way
}

void CParticleSystem::Update(float fElapsedTime)
{
	// update according to velocity 

	// update according to gravity or other forces
}

CParticleSystem* CParticleSystem::GetInstance(void)
{
	static CParticleSystem instance;
	return &instance;
}