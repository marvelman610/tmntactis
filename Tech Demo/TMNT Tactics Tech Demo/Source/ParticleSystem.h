//////////////////////////////////////////////////////////////////////////
//	Filename	:	ParticleSystem.h
//
//	Author		:	Jose Sierra (JS)
//
//	Purpose		:	To define the CParticleSystem class. This class handles
//					loading, updating, and drawing sprites.
//////////////////////////////////////////////////////////////////////////
#pragma once

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
};
#define MAX_NUM_PARTS 100

class CParticleSystem
{
public:
	PARTICLE* particles;
	IDirect3DVertexDeclaration9 *vertexDecl;
	IDirect3DVertexBuffer9 *particleBuff;
	IDirect3DTexture9 *texture;
	CSGD_TextureManager* m_pTM;
	int m_nImageID;
	int m_nNumParticles;

	CParticleSystem(void)
	{
		srand(unsigned int(time(0)));
		m_pTM = CSGD_TextureManager::GetInstance();
		//particles = NULL;
		vertexDecl = NULL;
		particleBuff = NULL;
		texture = NULL;
	}
	void InitGeometry(void)
	{
		D3DVERTEXELEMENT9 decl[]=
		{
			{0,0,D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
			{0,12,D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
			{0,20,D3DDECLTYPE_FLOAT1,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_PSIZE, 0},
			{0,24,D3DDECLTYPE_D3DCOLOR,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_COLOR,0},
			D3DDECL_END()
		};
		CSGD_Direct3D::GetInstance()->GetDirect3DDevice()->CreateVertexDeclaration(decl, &vertexDecl);
		D3DXCreateTextureFromFile(CSGD_Direct3D::GetInstance()->GetDirect3DDevice(), "resource/SGD_Ship.bmp", &texture);
	}
	void InitParticle(void)
	{
// 		CSGD_Direct3D::GetInstance()->GetDirect3DDevice()->CreateVertexBuffer(MAX_NUM_PARTS*sizeof(VERTEX),
// 			D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY | D3DUSAGE_POINTS, 0,
// 			D3DPOOL_DEFAULT, &particleBuff, NULL);
		m_nNumParticles = MAX_NUM_PARTS;
		particles = new PARTICLE[m_nNumParticles];
		m_nImageID = CSGD_TextureManager::GetInstance()->LoadTexture("Resources/Images/VG_Ship.bmp", D3DCOLOR_XRGB(0,0,0));

		//loop through all the particles 
		for(int i = 0; i < m_nNumParticles; i++)
		{
			particles[i].pos = D3DXVECTOR3(100,100,0.0f);//start position
// 			float x = (rand()%100)*.001f;
// 			float y = (rand()%100)*.001f;
// 			float z = (rand()%100)*.001f;
			particles[i].vel = D3DXVECTOR3(RandomFloat(-100.0f, 100.0f),RandomFloat(-100.0f, 100.0f),0.0f/*RandomFloat(-0.1f, 0.1f)*/);
			particles[i].life = (float)(rand()%100);
			particles[i].size = (float)(rand()%10);
			particles[i].color = D3DCOLOR_ARGB(rand()%255, 255, 255, 255);
		}
	}
	void UpdateParticle(float fElapsedTime, POINT mousePt)
	{
		int x = 0;
		for(int i = 0; i < m_nNumParticles; i++)
		{
			particles[i].pos += particles[i].vel * fElapsedTime;
			particles[i].life++;/* += particles[i].life * fElapsedTime*/;
			//particles[i].size -= 0.05f;
			if(particles[i].life > 100.0f)
			{
				particles[i].vel = D3DXVECTOR3(RandomFloat(-100.0f, 100.0f),RandomFloat(-100.0f, 100.0f),0.0f/*RandomFloat(-0.1f, 0.1f)*/);
				//particles[i].vel = D3DXVECTOR3(0.0f,0.0f,0.0f);
				particles[i].life = 0.0f;
				particles[i].pos = D3DXVECTOR3((float)(mousePt.x) - 50.0f, (float)(mousePt.y), 0.0f);
				particles[i].color = D3DCOLOR_ARGB(rand()%255,rand()%55, rand()%55, rand()%55);
			}
		}
	}
	void DrawParticle(void)
	{
		for (int i = 0; i < m_nNumParticles; ++i)
		{
			CSGD_TextureManager::GetInstance()->DrawWithZSort(m_nImageID, (int)(particles[i].pos.x),
					(int)(particles[i].pos.y), 0.0f);
		}
// 		CSGD_Direct3D::GetInstance()->GetDirect3DDevice()->SetVertexDeclaration(vertexDecl);
// 		//draw point sprites transparent
// 		CSGD_Direct3D::GetInstance()->GetDirect3DDevice()->SetRenderState(D3DRS_ZWRITEENABLE, true);
// 		CSGD_Direct3D::GetInstance()->GetDirect3DDevice()->SetRenderState(D3DRS_ALPHATESTENABLE, true);
// 		CSGD_Direct3D::GetInstance()->GetDirect3DDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
// 		//set the blend modes
// 		CSGD_Direct3D::GetInstance()->GetDirect3DDevice()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
// 		CSGD_Direct3D::GetInstance()->GetDirect3DDevice()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
// 		//set up the point sprites
// 		CSGD_Direct3D::GetInstance()->GetDirect3DDevice()->SetRenderState(D3DRS_POINTSPRITEENABLE, true);
// 		CSGD_Direct3D::GetInstance()->GetDirect3DDevice()->SetRenderState(D3DRS_POINTSCALEENABLE, true);
// 		CSGD_Direct3D::GetInstance()->GetDirect3DDevice()->SetRenderState(D3DRS_POINTSIZE, 1);
// 		CSGD_Direct3D::GetInstance()->GetDirect3DDevice()->SetRenderState(D3DRS_POINTSIZE_MIN, 1);
// 		CSGD_Direct3D::GetInstance()->GetDirect3DDevice()->SetRenderState(D3DRS_POINTSIZE_MAX, 100);
// 		// new size = base size * sqrt(1/(A+B*Dis+C*Dis*Dis))
// 		CSGD_Direct3D::GetInstance()->GetDirect3DDevice()->SetRenderState(D3DRS_POINTSCALE_A, 0);
// 		CSGD_Direct3D::GetInstance()->GetDirect3DDevice()->SetRenderState(D3DRS_POINTSCALE_B, 0);
// 		CSGD_Direct3D::GetInstance()->GetDirect3DDevice()->SetRenderState(D3DRS_POINTSCALE_C, 1);
// 		//use alpha value of vertex color with texture
// 		CSGD_Direct3D::GetInstance()->GetDirect3DDevice()->SetTextureStageState(0,D3DTSS_ALPHAOP, D3DTOP_MODULATE);
// 		//rewrite the dynamic vertex buffer
// 		VERTEX *pvbuff;
// 		particleBuff->Lock(0,1*sizeof(VERTEX), (void**)&pvbuff, D3DLOCK_DISCARD);
// 		for(int i = 0; i < 1; i++)
// 		{
// 			pvbuff->pos = particles[i].pos;
// 			pvbuff->size = particles[i].size;
// 			pvbuff->color = particles[i].color;
// 			pvbuff++;
// 		}
// 		particleBuff->Unlock();
// 		//draw
// 		D3DXMATRIX transform;
// 		D3DXMatrixIdentity(&transform);
// 		CSGD_Direct3D::GetInstance()->GetDirect3DDevice()->SetTransform(D3DTS_WORLD, &transform);
// 		CSGD_Direct3D::GetInstance()->GetDirect3DDevice()->SetTexture(0,texture);
//  		CSGD_Direct3D::GetInstance()->GetDirect3DDevice()->SetStreamSource(0,particleBuff,0,sizeof(VERTEX));
// 		CSGD_Direct3D::GetInstance()->GetDirect3DDevice()->DrawPrimitive(D3DPT_POINTLIST,0,1);
// 		CSGD_Direct3D::GetInstance()->GetDirect3DDevice()->SetTextureStageState(0,D3DTSS_ALPHAOP,D3DTOP_SELECTARG1);
	}
	~CParticleSystem(void)
	{
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
	}
	////////////////////////////////////////////////////////////////////
	// Function : Load
	//
	// Purpose : to load in binary file information and create a particle
	//////////////////////////////////////////////////////////////////////
	void Load(char* binFileName)
	{
		ifstream fs;
		//read in all values
		try
		{
			fs.exceptions(~ios_base::goodbit);
			fs.open(binFileName, ios::in | ios::binary);//attempt to open file
			int numparticles;
			fs.read(reinterpret_cast<char*>(&numparticles), sizeof(int));
			particles = new PARTICLE[numparticles];
			m_nNumParticles = numparticles;
			float emitterposx;
			fs.read(reinterpret_cast<char*>(&emitterposx), sizeof(float));
			float emitterposy;
			fs.read(reinterpret_cast<char*>(&emitterposy), sizeof(float));
			float particleoffsetx;
			fs.read(reinterpret_cast<char*>(&particleoffsetx), sizeof(float));
			float particleoffsety;

			fs.read(reinterpret_cast<char*>(&particleoffsety), sizeof(float));
			float emittervelx;
			fs.read(reinterpret_cast<char*>(&emittervelx), sizeof(float));
			float emittervely;
			fs.read(reinterpret_cast<char*>(&emittervely), sizeof(float));
			float emitterforcex;
			fs.read(reinterpret_cast<char*>(&emitterforcex), sizeof(float));
			float emitterforcey;
			fs.read(reinterpret_cast<char*>(&emitterforcey), sizeof(float));
			char size;
			char buffer[128];
			ZeroMemory(buffer, 128);
			fs.read(reinterpret_cast<char*>(&size), sizeof(char));
			fs.read(buffer, size);
			string fileName = buffer;
			string path = "Resources/Images/";
			path += fileName;
			strcpy_s(buffer, path.c_str());
			m_nImageID = m_pTM->LoadTexture(buffer, D3DCOLOR_XRGB(0,0,0));
			//set texture

			for(int i = 0; i < numparticles; i++)
			{
				//set position etc
				particles[i].pos.x = emitterposx + RandomFloat(-particleoffsetx, particleoffsetx);
				particles[i].pos.y = emitterposy + RandomFloat(-particleoffsety, particleoffsety);
				float particlevelx;
				fs.read(reinterpret_cast<char*>(&particlevelx), sizeof(float));
				particles[i].vel.x = particlevelx;
				float particlevely;
				fs.read(reinterpret_cast<char*>(&particlevely), sizeof(float));
				particles[i].vel.y = particlevely;
				//gravity
				float gravx;
				fs.read(reinterpret_cast<char*>(&gravx), sizeof(float));
				float gravy;
				fs.read(reinterpret_cast<char*>(&gravy), sizeof(float));
				bool bgrav;
				fs.read(reinterpret_cast<char*>(&bgrav), sizeof(bool));
				//color
				int a;
				fs.read(reinterpret_cast<char*>(&a), sizeof(int));
				int r;
				fs.read(reinterpret_cast<char*>(&r), sizeof(int));
				int g;
				fs.read(reinterpret_cast<char*>(&g), sizeof(int));
				int b;
				particles[i].color = D3DCOLOR_ARGB(a,r,g,b);
				fs.read(reinterpret_cast<char*>(&b), sizeof(int));
				//
				bool randlife;
				fs.read(reinterpret_cast<char*>(&randlife), sizeof(bool));

				int particlesize;
				fs.read(reinterpret_cast<char*>(&particlesize), sizeof(int));
				if(randlife == true)
				{
					particles[i].life = (float)(rand()%100);
				}
				//fade out
				bool alphadown;
				fs.read(reinterpret_cast<char*>(&alphadown), sizeof(bool));
			}
		}
		catch (ios_base::failure &)
		{
			if (fs.eof())
			{fs.close();return;}	
		}
	};
};