#pragma once

#include "SGD Wrappers/CSGD_Direct3D.h"
#include "SGD Wrappers/CSGD_TextureManager.h"
#include "CGame.h"
#include <ctime>
#include <fstream>
#include <string>
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
	int life;
	float size;
	D3DCOLOR color;
};

class CParticleSystem
{
public:
	PARTICLE *particles;
	IDirect3DVertexDeclaration9 *vertexDecl;
	IDirect3DVertexBuffer9 *particleBuff;
	IDirect3DTexture9 *texture;
	int m_nImageID;

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function : Constructor
	//
	// Purpose : To initialize values so there arent random access problems with the variables
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	CParticleSystem(void)
	{
		srand(unsigned int(time(0)));
		particles = NULL;
		vertexDecl = NULL;
		particleBuff = NULL;
		texture = NULL;
		m_nImageID = CSGD_TextureManager::GetInstance()->LoadTexture("source//SGD_Ship.bmp", D3DCOLOR_XRGB(0,0,0));
	}
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function : Init Geometry
	//
	// Purpose : To set up the vertex declaration for the point sprites in 3d space
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
		D3DXCreateTextureFromFile(CSGD_Direct3D::GetInstance()->GetDirect3DDevice(), "source\\SGD_Ship.bmp", &texture);
		//CSGD_TextureManager::GetInstance()->Load(
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function : InitParticle
	//
	// Purpose : Initializing the particle list
	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void InitParticle(void)
	{
		CSGD_Direct3D::GetInstance()->GetDirect3DDevice()->CreateVertexBuffer(100*sizeof(VERTEX),
			D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY | D3DUSAGE_POINTS, 0,
			D3DPOOL_DEFAULT, &particleBuff, NULL);

		//loop through all the particles 
		for(int i = 0; i < 100; i++)
		{
			particles[i].pos = D3DXVECTOR3(100,100,0);//start position
			float x = (rand()%100)*.001f;
			float y = (rand()%100)*.001f;
			float z = (rand()%100)*.001f;
			particles[i].vel = D3DXVECTOR3(RandomFloat(-0.1f, 0.1f),RandomFloat(-0.1f, 0.1f),RandomFloat(-0.1f, 0.1f));
			particles[i].life = rand()%100;
			particles[i].size = (float)(rand()%10);
			particles[i].color = D3DCOLOR_ARGB(rand()%255, rand()%55, rand()%55, rand()%55);

		}
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function : UpdateParticle
	//
	// Purpose : updates the particle position, life, color and velocity
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void UpdateParticle(void)
	{
		int x = 0;
		for(int i = 0; i < 100; i++)
		{
			particles[i].pos += particles[i].vel;
			particles[i].life++;
			//particles[i].size -= 0.05f;
			if(particles[i].life >100)
			{
				particles[i].vel = D3DXVECTOR3(RandomFloat(-0.01f, 0.01f),RandomFloat(-0.1f, 0.1f),RandomFloat(-0.01f, 0.01f));
				particles[i].life = 0;
				particles[i].pos = D3DXVECTOR3(100,100,0);
				particles[i].color = D3DCOLOR_ARGB(rand()%255, rand()%55, rand()%55, rand()%55);
			}
		}
	}
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function : DrawParticle
	//
	// Purpose : To render the particle on the screen
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	void DrawParticle(void)
	{
		CSGD_Direct3D::GetInstance()->GetDirect3DDevice()->SetVertexDeclaration(vertexDecl);
		//draw point sprites transparent
		CSGD_Direct3D::GetInstance()->GetDirect3DDevice()->SetRenderState(D3DRS_ZWRITEENABLE, false);
		CSGD_Direct3D::GetInstance()->GetDirect3DDevice()->SetRenderState(D3DRS_ALPHATESTENABLE, false);
		CSGD_Direct3D::GetInstance()->GetDirect3DDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
		//set the blend modes
		CSGD_Direct3D::GetInstance()->GetDirect3DDevice()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		CSGD_Direct3D::GetInstance()->GetDirect3DDevice()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		//set up the point sprites
		CSGD_Direct3D::GetInstance()->GetDirect3DDevice()->SetRenderState(D3DRS_POINTSPRITEENABLE, true);
		CSGD_Direct3D::GetInstance()->GetDirect3DDevice()->SetRenderState(D3DRS_POINTSCALEENABLE, true);
		CSGD_Direct3D::GetInstance()->GetDirect3DDevice()->SetRenderState(D3DRS_POINTSIZE, 1);
		CSGD_Direct3D::GetInstance()->GetDirect3DDevice()->SetRenderState(D3DRS_POINTSIZE_MIN, 1);
		CSGD_Direct3D::GetInstance()->GetDirect3DDevice()->SetRenderState(D3DRS_POINTSIZE_MAX, 100);
		// new size = base size * sqrt(1/(A+B*Dis+C*Dis*Dis))
		CSGD_Direct3D::GetInstance()->GetDirect3DDevice()->SetRenderState(D3DRS_POINTSCALE_A, 0);
		CSGD_Direct3D::GetInstance()->GetDirect3DDevice()->SetRenderState(D3DRS_POINTSCALE_B, 0);
		CSGD_Direct3D::GetInstance()->GetDirect3DDevice()->SetRenderState(D3DRS_POINTSCALE_C, 1);
		//use alpha value of vertex color with texture
		CSGD_Direct3D::GetInstance()->GetDirect3DDevice()->SetTextureStageState(0,D3DTSS_ALPHAOP, D3DTOP_MODULATE);
		//rewrite the dynamic vertex buffer
		VERTEX *pvbuff;
		particleBuff->Lock(0,100*sizeof(VERTEX), (void**)&pvbuff, D3DLOCK_DISCARD);
		for(int i = 0; i < 100; i++)
		{
			pvbuff->pos = particles[i].pos;
			pvbuff->size = particles[i].size;
			pvbuff->color = particles[i].color;
			pvbuff++;
			CSGD_TextureManager::GetInstance()->Draw(m_nImageID, pvbuff->pos.x, pvbuff->pos.y, 0.0f,0.0f,NULL, 0.0f,0.0f,0.0f,pvbuff->color);
		}
		particleBuff->Unlock();
		//draw
		D3DXMATRIX transform;
		D3DXMatrixIdentity(&transform);
		CSGD_Direct3D::GetInstance()->GetDirect3DDevice()->SetTransform(D3DTS_WORLD, &transform);
		CSGD_Direct3D::GetInstance()->GetDirect3DDevice()->SetTexture(0,texture);
		CSGD_Direct3D::GetInstance()->GetDirect3DDevice()->SetStreamSource(0,particleBuff,0,sizeof(VERTEX));
		CSGD_Direct3D::GetInstance()->GetDirect3DDevice()->DrawPrimitive(D3DPT_POINTLIST,0,100);
		CSGD_Direct3D::GetInstance()->GetDirect3DDevice()->SetTextureStageState(0,D3DTSS_ALPHAOP,D3DTOP_SELECTARG1);
	}
	//////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// Destructor cleans up memory
	//////////////////////////////////////////////////////////////////////////////////////////////////
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
	void Load(void)
	{
		//read in all values
		try
		{
			fstream fs("save.dat", ios::in | ios::binary /*| ios::app*/);//attempt to open file
			fs.exceptions(~ios_base::goodbit);
			int numparticles;
			fs.read(reinterpret_cast<char*>(&numparticles), sizeof(int));
			particles = new PARTICLE[numparticles];
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
			char buffer[128];
			fs.read(reinterpret_cast<char*>(&buffer), sizeof(buffer));
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
				fs.read(reinterpret_cast<char*>(&b), sizeof(int));
				//
				bool randlife;
				fs.read(reinterpret_cast<char*>(&randlife), sizeof(bool));
				
				int particlesize;
				fs.read(reinterpret_cast<char*>(&particlesize), sizeof(int));
				if(randlife == true)
				{
					particles[i].life = rand()%100;
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