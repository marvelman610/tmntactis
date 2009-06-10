//////////////////////////////////////////////////////////////////////////
//	Filename	:	ParticleSystem.h
//
//	Author		:	Jose Sierra (JS)
//
//	Purpose		:	To define the CParticleSystem class. This class handles
//					loading, updating, and drawing sprites.
//////////////////////////////////////////////////////////////////////////
#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H

#include "CSGD_Direct3D.h"
#include "Game.h"
#include <ctime>
#include <fstream>
#include <string>
#include <exception>
#include "CSGD_TextureManager.h"
#include <iostream>
using namespace std;

#define RandomFloat(min,max) (((rand()/(float)RAND_MAX)*((max)-(min)))+(min))

struct VERTEX
{
	D3DXVECTOR3 pos;
	D3DXVECTOR2 uv;
	float size;
	D3DCOLOR color;
};
struct PARTICLE
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 vel;
	float life;
	float size;
	D3DCOLOR color;
	int sa, sr, sg, sb;
	int ca, cr, cg, cb;
	int ea, er, eg, eb;
	float m_fScale;
};
#define MAX_NUM_PARTS 100

class CParticleSystem
{
public:
	CSGD_Direct3D* m_pd3d;
	PARTICLE* particles;
	IDirect3DVertexDeclaration9 *vertexDecl;
	IDirect3DVertexBuffer9 *particleBuff;
	IDirect3DTexture9 *texture;
	CSGD_TextureManager* m_pTM;
	
	float m_fOffsetX;
	float m_fOffsetY;
	float m_fForceX;
	float m_fForceY;
	float m_fGravityX;
	float m_fGravityY;
	float m_fGravPointX;
	float m_fGravPointY;
	float m_fVelDiff;
	//float m_fScale;
	float m_fRotation;
	float m_fRotationVelocity;
	int m_nImageID;
	int m_nNumParticles;
	int m_nMaxLife;
	int m_nDestinationBlend;
	int m_nSourceBlend;
	bool m_bGravityPoint;
	bool m_bGravity;
	bool m_bAlphaChange;
	bool m_bColorChange;
	bool m_bColorChangeRand;
	bool m_bCollision;
	bool m_bScaling;
	bool m_bLoop;
	bool m_bRandAge;
	bool m_bRotation;
	bool m_bVelDiff;
	DWORD srcblend;
	DWORD destblend;

	CParticleSystem(void)
	{
		srand(unsigned int(time(0)));
		m_fOffsetX = m_fOffsetY = m_fForceX = m_fForceY = m_fGravityX = m_fGravityY = m_fGravPointX = m_fGravPointY=
			m_fVelDiff = m_fRotation = m_fRotationVelocity = 0.0f;

		srcblend = destblend = NULL;
		m_nImageID = -1;
		m_nNumParticles = m_nMaxLife = m_nDestinationBlend = m_nSourceBlend = 0;

		m_bGravityPoint = m_bGravity = m_bAlphaChange = m_bColorChange = m_bColorChangeRand = m_bCollision = 
			m_bScaling = m_bLoop = m_bRandAge = m_bRotation = m_bVelDiff = false;
		
		m_pTM = CSGD_TextureManager::GetInstance();
		m_pd3d = CSGD_Direct3D::GetInstance();
		//particles = NULL;
		vertexDecl = NULL;
		particleBuff = NULL;
		texture = NULL;
	}
	
	void InitParticle(void)
	{
		m_nNumParticles = MAX_NUM_PARTS;
		particles = new PARTICLE[m_nNumParticles];
		m_nImageID = CSGD_TextureManager::GetInstance()->LoadTexture("Resources/Images/VG_Ship.bmp", D3DCOLOR_XRGB(0,0,0));

		m_fOffsetX = 10.0f;
		m_fOffsetY = 10.0f;
		m_fForceX = 0.0f;
		m_fForceY = 0.0f;
		m_fGravityX = 0.0f;
		m_fGravityY = 1.0f;
		m_fGravPointX = 0.0f;
		m_fGravPointY= 0.0f;
		m_fVelDiff = 10.0f;
		//m_fScale = 1.0f;
		m_fRotation = 0.01f;
		m_fRotationVelocity = 0.01f;

		m_nMaxLife = 100;
		/*m_nStartAlpha = 255;
		m_nStartRed = 255;
		m_nStartGreen = 255;
		m_nStartBlue = 255;
		m_nEndAlpha = 255;
		m_nEndRed = 255;
		m_nEndGreen = 255;
		m_nEndBlue = 255;*/
		m_nDestinationBlend = 0;
		m_nSourceBlend = 0;

		m_bGravityPoint = false;
		m_bGravity = false;
		m_bAlphaChange = false;
		m_bColorChange = false;
		m_bColorChangeRand = false;
		m_bCollision = false;
		m_bScaling = false;
		m_bLoop = true;
		m_bRandAge = true;
		m_bRotation = true;
		m_bVelDiff = false;

		//loop through all the particles 
		for(int i = 0; i < m_nNumParticles; i++)
		{
			particles[i].sa = particles[i].sr = particles[i].sg = particles[i].sb = 
				particles[i].ca = particles[i].cr = particles[i].cg = particles[i].cb = 
				particles[i].ea = particles[i].er = particles[i].eg = particles[i].eb = 255;
			particles[i].m_fScale = 1.0f;

			particles[i].pos = D3DXVECTOR3(100,100,0.0f);//start position
			particles[i].vel = D3DXVECTOR3(RandomFloat(-100.0f, 100.0f),RandomFloat(-100.0f, 100.0f),0.0f);
			if(m_bRandAge == true)
			{
				particles[i].life = (float)(rand()%100);
			}
			else particles[i].life = 0.0f;

			particles[i].size = (float)(rand()%10);

			if(m_bColorChangeRand == true)
			{
				particles[i].color = D3DCOLOR_ARGB(rand()%255, 255, 255, 255);
			}
			else particles[i].color = D3DCOLOR_ARGB(particles[i].sa, particles[i].sr, particles[i].sg, particles[i].sb);
		}
	}
	void UpdateParticle(float fElapsedTime, POINT mousePt)
	{
		int x = 0;

		m_fGravPointX = mousePt.x;
		m_fGravPointY = mousePt.y;

		D3DXVECTOR3 gravPoint = D3DXVECTOR3(m_fGravPointX, m_fGravPointY, 0);
		for(int i = 0; i < m_nNumParticles; i++)
		{
			if(m_bGravityPoint == true)
			{
				float length;
				D3DXVECTOR3 subtract;
				D3DXVECTOR3 newVel;
				D3DXVECTOR3 scale;
				
				D3DXVec3Subtract(&subtract, &gravPoint, &particles[i].pos);
				length = D3DXVec3Length(&subtract) * 0.00005f;
				D3DXVec3Scale(&newVel, &subtract, length);

				D3DXVec3Add(&particles[i].vel, &particles[i].vel, &newVel);
				
			}
			if(m_bGravity == true)
			{
				particles[i].pos.x += m_fGravityX;
				particles[i].pos.y += m_fGravityY;
			}
			if(m_bAlphaChange == true)
			{
				if(particles[i].ca < particles[i].ea)
				{
					particles[i].ca++;
				}
				particles[i].color = D3DCOLOR_ARGB(particles[i].ca, particles[i].cr, particles[i].cg, particles[i].cb);
			}
			if(m_bColorChange == true)
			{
				if( particles[i].ca < particles[i].ea)
				{
					particles[i].ca++;
				}
				else if(particles[i].ca > particles[i].ea)
				{
					particles[i].ca--;
				}
				if(particles[i].cr < particles[i].er)
				{
					particles[i].cr++;
				}
				else if(particles[i].cr > particles[i].er)
				{
					particles[i].cr--;
				}
				if(particles[i].cg < particles[i].eg)
				{
					particles[i].cg++;
				}
				else if(particles[i].cg > particles[i].eg)
				{
					particles[i].cg--;
				}
				if(particles[i].cb < particles[i].eb)
				{
					particles[i].cb++;
				}
				else if(particles[i].cb > particles[i].eb)
				{
					particles[i].cb--;
				}
				particles[i].color = D3DCOLOR_ARGB(particles[i].ca, particles[i].cr, particles[i].cg, particles[i].cb);
			}
			
			if(m_bCollision == true){}
			if(m_bScaling == true){}
			
			if(m_bRotation == true)
			{
				m_fRotation += m_fRotationVelocity;
			}

			if(m_bVelDiff == true){}

			particles[i].pos += particles[i].vel * fElapsedTime;
			particles[i].life++;
			if(m_bLoop == true)
			{
				if(particles[i].life > m_nMaxLife)
				{
					particles[i].vel = D3DXVECTOR3(RandomFloat(-100.0f, 100.0f),RandomFloat(-100.0f, 100.0f),0.0f);
					particles[i].life = 0.0f;
					particles[i].pos = D3DXVECTOR3((float)(mousePt.x) - 50.0f, (float)(mousePt.y), 0.0f);
					if(m_bColorChangeRand == true)
					{
						particles[i].color = D3DCOLOR_ARGB(rand()%255,rand()%55, rand()%55, rand()%55);
					}
					else particles[i].color = D3DCOLOR_ARGB(particles[i].sa, particles[i].sr, particles[i].sg, particles[i].sb);
				}
			}
		}
	}
	void DrawParticle(void)
	{
		//CSGD_Direct3D::GetInstance()->GetDirect3DDevice()->SetVertexDeclaration(vertexDecl);
 		//draw point sprites transparent
 		//CSGD_Direct3D::GetInstance()->GetDirect3DDevice()->SetRenderState(D3DRS_ZWRITEENABLE, true);
 		//CSGD_Direct3D::GetInstance()->GetDirect3DDevice()->SetRenderState(D3DRS_ALPHATESTENABLE, true);
 		//CSGD_Direct3D::GetInstance()->GetDirect3DDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
 		//set the blend modes
 		//CSGD_Direct3D::GetInstance()->GetDirect3DDevice()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_INVSRCALPHA);
 		//CSGD_Direct3D::GetInstance()->GetDirect3DDevice()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCCOLOR);
 		//set up the point sprites
 		//CSGD_Direct3D::GetInstance()->GetDirect3DDevice()->SetRenderState(D3DRS_POINTSPRITEENABLE, true);
 		//CSGD_Direct3D::GetInstance()->GetDirect3DDevice()->SetRenderState(D3DRS_POINTSCALEENABLE, true);
 		//CSGD_Direct3D::GetInstance()->GetDirect3DDevice()->SetRenderState(D3DRS_POINTSIZE, 1);
 		//CSGD_Direct3D::GetInstance()->GetDirect3DDevice()->SetRenderState(D3DRS_POINTSIZE_MIN, 1);
 		//CSGD_Direct3D::GetInstance()->GetDirect3DDevice()->SetRenderState(D3DRS_POINTSIZE_MAX, 100);
 		//// new size = base size * sqrt(1/(A+B*Dis+C*Dis*Dis))
 		//CSGD_Direct3D::GetInstance()->GetDirect3DDevice()->SetRenderState(D3DRS_POINTSCALE_A, 0);
 		//CSGD_Direct3D::GetInstance()->GetDirect3DDevice()->SetRenderState(D3DRS_POINTSCALE_B, 0);
 		//CSGD_Direct3D::GetInstance()->GetDirect3DDevice()->SetRenderState(D3DRS_POINTSCALE_C, 1);
 		//use alpha value of vertex color with texture
 		//CSGD_Direct3D::GetInstance()->GetDirect3DDevice()->SetTextureStageState(0,D3DTSS_ALPHAOP, D3DTOP_MODULATE);
 		//rewrite the dynamic vertex buffer
 		//VERTEX *pvbuff;
 		//particleBuff->Lock(0,1*sizeof(VERTEX), (void**)&pvbuff, D3DLOCK_DISCARD);
 		//for(int i = 0; i < 1; i++)
 		//{
 		//	pvbuff->pos = particles[i].pos;
 		//	pvbuff->size = particles[i].size;
 		//	pvbuff->color = particles[i].color;
 		//	pvbuff++;
 		//}
 		//particleBuff->Unlock();
 		////draw
 		//D3DXMATRIX transform;
 		//D3DXMatrixIdentity(&transform);
 		//CSGD_Direct3D::GetInstance()->GetDirect3DDevice()->SetTransform(D3DTS_WORLD, &transform);
 		//CSGD_Direct3D::GetInstance()->GetDirect3DDevice()->SetTexture(0,texture);
  		//CSGD_Direct3D::GetInstance()->GetDirect3DDevice()->SetStreamSource(0,particleBuff,0,sizeof(VERTEX));
 		//CSGD_Direct3D::GetInstance()->GetDirect3DDevice()->DrawPrimitive(D3DPT_POINTLIST,0,1);
 		//CSGD_Direct3D::GetInstance()->GetDirect3DDevice()->SetTextureStageState(0,D3DTSS_ALPHAOP,D3DTOP_SELECTARG1);

		// DRAW		
 		//set the blend modes
		
		m_pd3d->GetDirect3DDevice()->GetRenderState(D3DRS_SRCBLEND,&srcblend);
		m_pd3d->GetDirect3DDevice()->GetRenderState(D3DRS_DESTBLEND, &destblend);

		m_pd3d->GetSprite()->Flush();
		
 		m_pd3d->GetDirect3DDevice()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
 		m_pd3d->GetDirect3DDevice()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

		for (int i = 0; i < m_nNumParticles; ++i)
		{
			CSGD_TextureManager::GetInstance()->Draw(m_nImageID, (int)(particles[i].pos.x),
				(int)(particles[i].pos.y), particles[i].m_fScale, particles[i].m_fScale, NULL, particles[i].pos.x,
				particles[i].pos.y, m_fRotation, particles[i].color );
		}

		m_pd3d->GetSprite()->Flush();


		m_pd3d->GetDirect3DDevice()->SetRenderState(D3DRS_SRCBLEND, srcblend);
		m_pd3d->GetDirect3DDevice()->SetRenderState(D3DRS_DESTBLEND, destblend);

	}

	~CParticleSystem(void)
	{
		if (particles)
		{
			delete[] particles;
		}
		if(vertexDecl)
		{
			vertexDecl->Release();
		}
		if(particleBuff)
		{
			particleBuff->Release();
		}
		if(texture)
		{
			texture->Release();
		}
		if(m_nImageID > -1)
		{
			m_pTM->UnloadTexture(m_nImageID);
		}
	}
	///////////////////////////////////////////////////////////////////////
	// Function : Load
	//
	// Purpose : to load in binary file information and create a particle
	///////////////////////////////////////////////////////////////////////
	void Load(char* binFileName)
	{
		ifstream fs;
		//read in all values
		try
		{
			fs.exceptions(~ios_base::goodbit);
			fs.open(binFileName, ios::in | ios::binary);//attempt to open file
			
			/*char buffer[512];
			ZeroMemory(buffer, 512);
			char size;
			fs.read(reinterpret_cast<char*>(&size),sizeof(char));
			fs.read(buffer, sizeof(size));
			string fileName = buffer;
			string path = "Resources/Images/";
			path += fileName;
			strcpy_s(buffer, path.c_str());*/

			//set texture
			m_nImageID = m_pTM->LoadTexture("Resources/Images/VG_Particle.png", D3DCOLOR_XRGB(0,0,0));
			
			int numparticles;
			fs.read(reinterpret_cast<char*>(&numparticles), sizeof(int));
			particles = new PARTICLE[numparticles];
			m_nNumParticles = numparticles;

			int maxlife;
			fs.read(reinterpret_cast<char*>(&maxlife), sizeof(int));
			m_nMaxLife = maxlife;

			float forcex;
			fs.read(reinterpret_cast<char*>(&forcex), sizeof(float));
			m_fForceX = forcex;
			float forcey;
			fs.read(reinterpret_cast<char*>(&forcey), sizeof(float));
			m_fForceY = forcey;

			float offsetx;
			fs.read(reinterpret_cast<char*>(&offsetx), sizeof(float));
			m_fOffsetX = offsetx;
			float offsety;
			fs.read(reinterpret_cast<char*>(&offsety), sizeof(float));
			m_fOffsetY = offsety;
			
			for(int i = 0; i < numparticles; i++)
			{
				//velocity
				float particlevelx;
				fs.read(reinterpret_cast<char*>(&particlevelx), sizeof(float));
				particles[i].vel.x = particlevelx;
				float particlevely;
				fs.read(reinterpret_cast<char*>(&particlevely), sizeof(float));
				particles[i].vel.y = particlevely;

				//gravity
				bool bGravPoint;
				fs.read(reinterpret_cast<char*>(&bGravPoint), sizeof(bool));
				//m_bGravityPoint = bGravPoint;
				m_bGravityPoint = true;

				bool bGrav;
				fs.read(reinterpret_cast<char*>(&bGrav), sizeof(bool));
				m_bGravity = bGrav;

				float gravx;
				fs.read(reinterpret_cast<char*>(&gravx), sizeof(float));
				m_fGravityX = gravx;

				float gravy;
				fs.read(reinterpret_cast<char*>(&gravy), sizeof(float));
				m_fGravityY = gravy;


				//start color
				int salpha;
				fs.read(reinterpret_cast<char*>(&salpha), sizeof(int));
				particles[i].sa = salpha;

				int sblue;
				fs.read(reinterpret_cast<char*>(&sblue), sizeof(int));
				particles[i].sb = sblue;

				int sgreen;
				fs.read(reinterpret_cast<char*>(&sgreen), sizeof(int));
				particles[i].sg = sgreen;

				int sred;
				fs.read(reinterpret_cast<char*>(&sred), sizeof(int));
				particles[i].sr = sred;

				int eAlpha;
				fs.read(reinterpret_cast<char*>(&eAlpha), sizeof(int));
				particles[i].ea = eAlpha;

				int eBlue;
				fs.read(reinterpret_cast<char*>(&eBlue), sizeof(int));
				particles[i].eb = eBlue;

				int eGreen;
				fs.read(reinterpret_cast<char*>(&eGreen), sizeof(int));
				particles[i].eg = eGreen;

				int eRed;
				fs.read(reinterpret_cast<char*>(&eRed), sizeof(int));
				particles[i].er = eRed;

				//change with start argb
				particles[i].color = D3DCOLOR_ARGB(salpha,sred,sgreen,sblue);
				particles[i].ca = salpha;
				particles[i].cr = sred;
				particles[i].cg = sgreen;
				particles[i].cb = sblue;

				// color changes
				bool bAlphaChange;
				fs.read(reinterpret_cast<char*>(&bAlphaChange), sizeof(bool));
				m_bAlphaChange = bAlphaChange;

				bool bColorChange;
				fs.read(reinterpret_cast<char*>(&bColorChange), sizeof(bool));
				m_bColorChange = bColorChange;

				bool bColorChangeRand;
				fs.read(reinterpret_cast<char*>(&bColorChangeRand), sizeof(bool));
				m_bColorChangeRand = bColorChangeRand;


				//collision
				bool bCollision;
				fs.read(reinterpret_cast<char*>(&bCollision), sizeof(bool));
				m_bCollision = bCollision;

				//scaling
				bool bScaling;
				fs.read(reinterpret_cast<char*>(&bScaling), sizeof(bool));
				m_bScaling = bScaling;

				//scale
				float fScale;
				fs.read(reinterpret_cast<char*>(&fScale), sizeof(float));
				particles[i].m_fScale = fScale;

				//loop
				bool bLoop;
				fs.read(reinterpret_cast<char*>(&bLoop), sizeof(bool));
				m_bLoop = bLoop;

				//rand age
				bool bRandAge;
				fs.read(reinterpret_cast<char*>(&bRandAge), sizeof(bool));
				m_bRandAge = bRandAge;

				//rotation
				bool bRotation;
				fs.read(reinterpret_cast<char*>(&bRotation), sizeof(bool));
				m_bRotation = bRotation;
				//m_bRotation = true;

				float fRotation;
				fs.read(reinterpret_cast<char*>(&fRotation), sizeof(fRotation));
				m_fRotation = fRotation;

				float fRotationVelocity;
				fs.read(reinterpret_cast<char*>(&fRotationVelocity), sizeof(fRotationVelocity));
				m_fRotationVelocity = fRotationVelocity;

				//velocity difference
				bool bVelDiff;
				fs.read(reinterpret_cast<char*>(&bVelDiff), sizeof(bVelDiff));
				m_bVelDiff = bVelDiff;

				float fVelDiff;
				fs.read(reinterpret_cast<char*>(&fVelDiff), sizeof(fVelDiff));
				m_fVelDiff = fVelDiff;

				////dest blend
				//int nDestBlend;
				//fs.read(reinterpret_cast<char*>(&nDestBlend), sizeof(nDestBlend));
				//m_nDestinationBlend = nDestBlend;

				////source blend
				//int nSourceBlend;
				//fs.read(reinterpret_cast<char*>(&nSourceBlend), sizeof(nSourceBlend));
				//m_nSourceBlend = nSourceBlend;

			}
			for(int i = 0; i < m_nNumParticles; i++)
			{
				particles[i].pos = D3DXVECTOR3(0,0,0);
				particles[i].life = (float)(rand()% m_nMaxLife);
			}
		}
		catch (ios_base::failure &)
		{
			if (fs.eof())
			{fs.close();return;}	
		}
	};
};

#endif