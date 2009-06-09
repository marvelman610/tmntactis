#include "Box.h"
#include "Assets.h"
#include "BitmapFont.h"
#include "CSGD_Direct3D.h"
#include "CSGD_TextureManager.h"

CBox::CBox(int numItems, string* sItems, int posX, int posY, float posZ /* = 0.1f */, 
		   int width /* = 256 */, int height /* = -1 */, int spacing /* = 35 */, int startX/* =40 */, 
		   int startY/* =40 */, int imageID /* = -1 */, float fScaleX /* = 1.0f */, float fScaleY /* = 1.0f */, 
		   float fTextScale /* = 1.0f */, int red /* = 0 */, int green /* = 0 */, int blue /* = 0 */)
{
	m_nPosX = posX;
	m_nPosY = posY;
	m_nPosZ = posZ;
	m_nStartTextX = m_nPosX + startX;
	m_nStartTextY = m_nPosY + startY;
	m_nSpacing = spacing;
	m_nBoxWidth = width;
	if (height > -1)
		m_nBoxHeight = height;
	else
		m_nBoxHeight = ((posY - startY) >> 1) + (int)(32.0f * numItems);
	m_nBoxRight = m_nPosX + width;
	m_nBoxBottom = m_nPosY + m_nBoxHeight;
	m_fScaleY = fScaleY;
	m_fScaleX = fScaleX;
	m_fTextScale = fTextScale;

	m_nCurrSelectedIndex = -1;
	m_bIsActive = false;
	m_nNumItems = numItems;
	r = red; g = green; b = blue;

	int longestString = 0;
	m_sItems = new string[m_nNumItems];
	for (int i = 0; i < m_nNumItems; ++i)
	{
		m_sItems[i] = sItems[i];
		if (sItems[i].size() > longestString)
			longestString = sItems[i].size();
	}
	if (longestString * 32 > m_nBoxWidth)
	{
		m_nBoxWidth = longestString * 32 + ((m_nStartTextX - m_nPosX) * 2);
		m_nBoxRight = m_nPosX + m_nBoxWidth;
		m_fScaleX = 1.0f + (float)(1.0f - ((float)width / (float)m_nBoxWidth)); 
	}

	if (imageID > -1)
		m_nCurrImage = imageID;	// set to an image that has already been loaded
	else
		m_nCurrImage = CAssets::GetInstance()->aBMskillBoxID;	// default BG image
	m_pTM = CSGD_TextureManager::GetInstance();
	m_pD3D= CSGD_Direct3D::GetInstance();
	m_pBM = CBitmapFont::GetInstance();
	m_pAssets = CAssets::GetInstance();
}

CBox::~CBox()
{
	m_pTM = NULL;
	delete[] m_sItems;
}

void CBox::CheckMouse(POINT mousePt)
{
	if (mousePt.x >= m_nPosX && mousePt.x <= m_nBoxRight && mousePt.y >= m_nPosY && mousePt.y <= m_nBoxBottom)
	{ m_nAlpha = 150; }
	else
	{ m_nAlpha = 255; }

	if (mousePt.x >= m_nPosX && mousePt.x <= m_nBoxRight && mousePt.y >= m_nPosY && mousePt.y <= m_nBoxBottom)
	{
		m_nCurrSelectedIndex = mousePt.y / ( (m_nStartTextY + 32) * (mousePt.y / (m_nStartTextY + m_nSpacing) ) );
	}
	else
		m_nCurrSelectedIndex = -1;
	if (m_nCurrSelectedIndex > m_nNumItems-1)
		m_nCurrSelectedIndex = - 1;
}

void CBox::Render()
{
	m_pTM->DrawWithZSort(CurrImage(), PosX(), PosY(), PosZ(), m_fScaleX, m_fScaleY, NULL, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(m_nAlpha, 255, 255, 255));
	if (m_nCurrSelectedIndex > -1)
	{
		m_pTM->DrawWithZSort(m_pAssets->aBpointerID, m_nPosX-5, m_nStartTextY + (m_nCurrSelectedIndex*m_nSpacing), 0.0f);
		RECT btnRect;
		btnRect.left = m_nStartTextX-10; 
		btnRect.right = m_nBoxRight-10; 
		btnRect.top = m_nStartTextY + (m_nSpacing*m_nCurrSelectedIndex); 
		btnRect.bottom = btnRect.top + (int)(32.0f * m_fTextScale);
		m_pD3D->DrawLine(btnRect.left, btnRect.top, btnRect.right, btnRect.top, 0,0,255);	// top line
		m_pD3D->DrawLine(btnRect.left, btnRect.top, btnRect.left, btnRect.bottom, 0,0,255);	// left line
		m_pD3D->DrawLine(btnRect.right, btnRect.top, btnRect.right, btnRect.bottom, 0,0,255);	// right line
		m_pD3D->DrawLine(btnRect.left, btnRect.bottom, btnRect.right, btnRect.bottom, 0,0,255);	// bottom line
	}

	//m_pBM->ChangeBMFont(m_pAssets->aBitmapFont2ID, 16, 16, 18);
	m_dwColor = D3DCOLOR_ARGB(m_nAlpha, 255,255,255/*r, g, b*/);
	for (int i = 0; i < m_nNumItems; ++i)
	{
		m_pBM->DrawString(m_sItems[i].c_str(), m_nStartTextX, m_nStartTextY+(i*m_nSpacing), m_fTextScale, m_dwColor);
	}
	//m_pBM->Reset();

}

int CBox::Input(POINT mousePt)
{
	if (!m_bIsActive)
		m_nAlpha = 150;
	else
		CheckMouse(mousePt);
	return m_nCurrSelectedIndex;
}