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
	PARTICLE particles[MAX_NUM_PARTS];
	IDirect3DVertexDeclaration9 *vertexDecl;
	IDirect3DVertexBuffer9 *particleBuff;
	IDirect3DTexture9 *texture;
	int texID;

	CParticleSystem(void)
	{
		srand(unsigned int(time(0)));
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
		texID = CSGD_TextureManager::GetInstance()->LoadTexture("resource/SGD_Ship.bmp", D3DCOLOR_XRGB(0,0,0));
	}
	void InitParticle(void)
	{
// 		CSGD_Direct3D::GetInstance()->GetDirect3DDevice()->CreateVertexBuffer(MAX_NUM_PARTS*sizeof(VERTEX),
// 			D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY | D3DUSAGE_POINTS, 0,
// 			D3DPOOL_DEFAULT, &particleBuff, NULL);
		texID = CSGD_TextureManager::GetInstance()->LoadTexture("Resources/Images/VG_Ship.bmp", D3DCOLOR_XRGB(0,0,0));

		//loop through all the particles 
		for(int i = 0; i < MAX_NUM_PARTS; i++)
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
		for(int i = 0; i < MAX_NUM_PARTS; i++)
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
		for (int i = 0; i < MAX_NUM_PARTS; ++i)
		{
			CSGD_TextureManager::GetInstance()->DrawWithZSort(texID, (int)(particles[i].pos.x),
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
};