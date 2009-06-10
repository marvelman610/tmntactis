//////////////////////////////////////////////////////////////////////////
//	Filename	:	Box.h
//
//	Author		:	Ramon Johannessen (RJ)
//
//	Purpose		:	To dynamically create menu boxes and populate them with
//					specific information.
//////////////////////////////////////////////////////////////////////////
#include "Box.h"
#include "Assets.h"
#include "BitmapFont.h"
#include "CSGD_Direct3D.h"
#include "CSGD_TextureManager.h"

#define DEFAULT_SIZE 256.0f

CBox::CBox(int numItems, string* sItems, 
		   int posX, int posY, float posZ /* = 0.1f */,  
		   int spacing /* = 35 */, int startX/* = 35 */, int startY/* = 25 */, 
		   int imageID /* = -1 */, float fTextScale /* = 1.0f */, 
		   int red /* = 0 */, int green /* = 0 */, int blue /* = 0 */)
{
	m_sItems = new string[numItems];
	//////////////////////////////////////////////////////////////////////////
	// box properties
	m_bIsActive = false;
	m_nType = BOX_NO_BACK;
	m_nPosX = posX;
	m_nPosY = posY;
	m_fPosZ = posZ;

	m_nLongestString = 0;
	for (int i = 0; i < numItems; ++i)
	{
		m_sItems[i] = sItems[i];
		if ((int)sItems[i].size() > m_nLongestString)
			m_nLongestString = sItems[i].size();
	}
	m_nLongestString *= 34 * fTextScale; 
	m_fScaleX = (float)(m_nLongestString + (float)spacing * 2.5f) / DEFAULT_SIZE; 
	m_fScaleY = (float)((numItems * 34 * fTextScale) + ((float)spacing * 2.5f) + startY) / DEFAULT_SIZE;
	
	m_nBoxWidth = (int)(DEFAULT_SIZE * m_fScaleX);
	m_nBoxHeight = (int)(DEFAULT_SIZE * m_fScaleY);

	m_nBoxRight = m_nPosX + m_nBoxWidth;
	m_nBoxBottom = m_nPosY + m_nBoxHeight;

	//////////////////////////////////////////////////////////////////////////
	// text properties
	m_fTextZ = posZ - 0.01f;
	m_nStartTextX = m_nPosX + startX;
	m_nStartTextY = m_nPosY + startY;
	m_nSpacing = spacing;
	m_fTextScale = fTextScale;

	//////////////////////////////////////////////////////////////////////////
	// items
	m_nCurrSelectedIndex = -1;
	m_nNumItems = numItems;
	m_nAlpha = 255;
	r = red; g = green; b = blue;

	//////////////////////////////////////////////////////////////////////////
	if (imageID > -1)
		m_nCurrImage = imageID;	// specify set to an image that has already been loaded
	else
		m_nCurrImage = CAssets::GetInstance()->aBMactionBoxID;	// default BG image

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
	if (m_nType != BOX_WITH_BACK)
	{
		// is the mouse in the box?
		if (mousePt.x >= m_nPosX && mousePt.x <= m_nBoxRight && mousePt.y >= m_nPosY && mousePt.y <= m_nBoxBottom)
		{ m_nAlpha = 255; m_bIsMouseInBox = true;}
		else
		{ m_nAlpha = 100; m_bIsMouseInBox = false;}
	}
	else // it has a back button?
	{
		if (mousePt.x >= (m_nBoxRight-(25+300.0f*m_fTextScale)) && mousePt.x <= m_nBoxRight && 
				mousePt.y >= m_nBoxBottom - (40.0f*m_fTextScale) && mousePt.y <= m_nBoxBottom)
		{
			m_nCurrSelectedIndex = BTN_BACK;
			return;
		}
	}
	if (mousePt.x >= m_nPosX && mousePt.x <= m_nBoxRight && mousePt.y >= m_nPosY && mousePt.y <= m_nBoxBottom)
	{
		m_nCurrSelectedIndex = (mousePt.y - m_nStartTextY) / (int)((float)m_nSpacing*1.5f);
	}
	else
		m_nCurrSelectedIndex = -1;
	if (m_nCurrSelectedIndex > m_nNumItems)
		m_nCurrSelectedIndex = - 1;
}

void CBox::Render()
{
	m_pTM->DrawWithZSort(CurrImage(), PosX(), PosY(), PosZ(), m_fScaleX, m_fScaleY, NULL, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(m_nAlpha, 255, 255, 255));

	//m_pBM->ChangeBMFont(m_pAssets->aBitmapFont2ID, 16, 16, 18);
	for (int i = 0; i < m_nNumItems; ++i)
	{
		if (i == m_nCurrSelectedIndex)
			m_dwColor = D3DCOLOR_ARGB(m_nAlpha, 255,50,50/*r, g, b*/);
		else
			m_dwColor = D3DCOLOR_ARGB(m_nAlpha, 255,255,255/*r, g, b*/);
		m_pBM->DrawString(m_sItems[i].c_str(), m_nStartTextX, m_nStartTextY+(int)((float)i*((float)m_nSpacing*1.5f)), m_fTextZ, m_fTextScale, m_dwColor);
	}
	if (m_nType == BOX_WITH_BACK)
	{
		if (m_nCurrSelectedIndex == BTN_BACK)
			m_dwColor = D3DCOLOR_ARGB(m_nAlpha, 255,50,50/*r, g, b*/);
		else
			m_dwColor = D3DCOLOR_ARGB(m_nAlpha, 255,255,255/*r, g, b*/);
		m_pBM->DrawString("BACK-ESC", (m_nBoxRight-(25+300.0f*m_fTextScale)), m_nBoxBottom-(int)(40.0f*m_fTextScale), m_fTextZ, m_fTextScale, m_dwColor);
	}
	//m_pBM->Reset();
}

int CBox::Input(POINT mousePt)
{
	if (m_bIsActive)
		CheckMouse(mousePt);

	return m_nCurrSelectedIndex;
}

void CBox::SetActive(bool bIsActive)
{
	m_bIsActive = bIsActive;
	if (!bIsActive)
		m_nAlpha = 150;
	else
		m_nAlpha = 255;
}