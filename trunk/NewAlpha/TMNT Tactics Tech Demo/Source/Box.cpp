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
#include "Game.h"
#include "BitmapFont.h"
#include "CSGD_Direct3D.h"
#include "CSGD_TextureManager.h"
#include "CSGD_DirectInput.h"
#include "CSGD_FModManager.h"

#define DEFAULT_SIZE	256.0f
#define MAX_INPUT_SIZE	11

CBox::CBox(int numItems, string* sItems, 
		   int posX, int posY, float posZ /* = 0.1f */, bool bHasTitle /* = false */, 
		   int spacing /* = 35 */, int startX/* = 35 */, int startY/* = 25 */, 
		   int imageID /* = -1 */, float fTextScale /* = 1.0f */, 
		   int red /* = 0 */, int green /* = 0 */, int blue /* = 0 */)
{
	m_sItems = new string[numItems];

	//////////////////////////////////////////////////////////////////////////
	// box properties
	m_bHasTitle = bHasTitle;
	if (bHasTitle)
		m_nTitleWidth = (int)(sItems[0].size() * (34.0f * fTextScale));
	m_bIsActive = m_bIsMsgBox = m_bAcceptInput = m_bEnterText = false;
	m_nBackType = BOX_NO_BACK;
	m_nPosX = posX;
	m_nPosY = posY;
	m_fPosZ = posZ;

	m_nLongestString = 8;	// 8 is the length of the BACK ESC button string, so that's default
	for (int i = 0; i < numItems; ++i)
	{
		m_sItems[i] = sItems[i];
		if (i > 0 && i < 5)
			m_sOriginal[i] = sItems[i];	// hang on to the original in case they cancel with ESC
		if ((int)sItems[i].size() > m_nLongestString)
			m_nLongestString = sItems[i].size();
	}
	m_nLongestString *= (int)(34.0f * fTextScale); 
	m_fScaleX = (float)(m_nLongestString + 50.0f * fTextScale * 4.0f) / DEFAULT_SIZE; 
	m_fScaleY = (float)((numItems * 34 * fTextScale) + ((float)spacing * 4.0f) + startY*2) / DEFAULT_SIZE;
	
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
	m_nCurrSelectedIndex = m_nCurrInputIndex = -1;
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
	m_pDI = CSGD_DirectInput::GetInstance();
	m_pFMOD = CSGD_FModManager::GetInstance();
}

CBox::~CBox()
{
	m_pTM = NULL;
	m_pD3D = NULL;
	m_pBM = NULL;
	m_pAssets = NULL;
	m_pDI = NULL;
	m_pFMOD = NULL;
	delete[] m_sItems;
}

void CBox::CheckMouse(POINT mousePt)
{
		// is the mouse in the box?
	if (mousePt.x >= m_nPosX && mousePt.x <= m_nBoxRight && mousePt.y >= m_nPosY && mousePt.y <= m_nBoxBottom && !m_bEnterText)
	{
		m_nAlpha = 255; 
		m_bIsMouseInBox = true;
		if (m_pDI->MouseButtonPressed(MOUSE_LEFT))
		{
			m_pFMOD->PlaySound(m_pAssets->aMMmenuClickSnd);
			if(!m_pFMOD->SetVolume(m_pAssets->aMMmenuClickSnd, CGame::GetInstance()->GetSFXVolume()))
				MessageBox(0, "VOLUME NOT SET", "ERROR", MB_OK);
		}
		// it has a back button?
		if (mousePt.x >= (m_nBoxRight-(25+(int)(300.0f*m_fTextScale))) && mousePt.x <= m_nBoxRight && 
			mousePt.y >= m_nBoxBottom - (40.0f*m_fTextScale) && mousePt.y <= m_nBoxBottom)
		{
			m_nCurrSelectedIndex = BTN_BACK;
			return;
		}
		if (!m_bIsMsgBox)
			m_nCurrSelectedIndex = (mousePt.y - m_nStartTextY) / (int)((float)m_nSpacing*1.5f);
		// it has an enter button?
		if (mousePt.x >= m_nPosX && mousePt.x <= (m_nPosX+(int)(200.0f*m_fTextScale)) && 
			mousePt.y >= m_nBoxBottom - (40.0f*m_fTextScale) && mousePt.y <= m_nBoxBottom)
		{
			m_nCurrSelectedIndex = BTN_ENTER;
			return;
		}
		if (m_pDI->MouseButtonPressed(MOUSE_LEFT) && m_bAcceptInput)
		{
			m_sItems[m_nCurrSelectedIndex].clear();
			m_nCurrInputIndex = m_nCurrSelectedIndex;
			m_bEnterText = true;
		}
		if (m_bHasTitle)
		{
 			if (m_nCurrSelectedIndex == 0)
			{m_nCurrSelectedIndex = -1;}
		}

		if (m_nCurrSelectedIndex > m_nNumItems)
		{m_nCurrSelectedIndex = -1;}
	}
	else if (m_bEnterText && m_bAcceptInput)
	{
		// it has an enter button?
		if (mousePt.x >= m_nPosX && mousePt.x <= (m_nPosX+(int)(200.0f*m_fTextScale)) && 
			mousePt.y >= m_nBoxBottom - (40.0f*m_fTextScale) && mousePt.y <= m_nBoxBottom)
		{			
// 			m_pFMOD->PlaySound(m_pAssets->aMMmenuClickSnd);
// 			if(!m_pFMOD->SetVolume(m_pAssets->aMMmenuClickSnd, CGame::GetInstance()->GetSFXVolume()))
// 				MessageBox(0, "VOLUME NOT SET", "ERROR", MB_OK);
			m_nCurrSelectedIndex = BTN_ENTER;
		}
		else
			m_nCurrSelectedIndex = -1;
	}
	else
	{ 
		if (m_nBackType != BOX_WITH_BACK)
			m_nAlpha = 100; 
		m_bIsMouseInBox = false;
		m_nCurrSelectedIndex = -1;
	}
}

void CBox::Render()
{
	m_pTM->DrawWithZSort(CurrImage(), PosX(), PosY(), PosZ(), m_fScaleX, m_fScaleY, NULL, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(m_nAlpha, 255, 255, 255));

	//m_pBM->ChangeBMFont(m_pAssets->aBitmapFont2ID, 16, 16, 18);
	for (int i = 0; i < m_nNumItems; ++i)
	{
		if (i == m_nCurrSelectedIndex || i == m_nCurrInputIndex)	// color the currently selected item
			m_dwColor = D3DCOLOR_ARGB(m_nAlpha, 255,50,50/*r, g, b*/);
		else
			m_dwColor = D3DCOLOR_ARGB(m_nAlpha, 255,255,255/*r, g, b*/);
		if (!m_bHasTitle || i > 0)
			m_pBM->DrawString(m_sItems[i].c_str(), m_nStartTextX, m_nStartTextY+(int)((float)i*((float)m_nSpacing*1.5f)), m_fTextZ, m_fTextScale, m_dwColor);
		else // drawing the Title text, centered, and underlined
		{
			int centerBox = m_nBoxRight - (m_nBoxWidth >> 1);
			int centerStr = (m_nTitleWidth >> 1);
			m_pBM->DrawString(m_sItems[i].c_str(), centerBox-centerStr, m_nStartTextY+(int)((float)i*((float)m_nSpacing*1.5f)), m_fTextZ, m_fTextScale, m_dwColor);
			m_pD3D->DrawLine(centerBox-centerStr+5, m_nStartTextY + (int)((float)m_nSpacing*1.2f), centerBox-centerStr + m_nTitleWidth, m_nStartTextY + (int)((float)m_nSpacing*1.2f),
								0, 0, 0);
		}
	}
// 	else
// 	{
// 		m_dwColor = D3DCOLOR_ARGB(m_nAlpha, 255,50,50/*r, g, b*/);
// 		int centerBox = m_nBoxRight - (m_nBoxWidth >> 1);
// 		int centerStr = (m_nTitleWidth >> 1);
// 		m_pBM->DrawString(m_sInput.c_str(), centerBox-centerStr, m_nStartTextY, m_fTextZ, m_fTextScale, m_dwColor);
// 
// 	}
	if (m_nBackType == BOX_WITH_BACK)
	{
		if (m_nCurrSelectedIndex == BTN_BACK)
			m_dwColor = D3DCOLOR_ARGB(m_nAlpha, 255,50,50/*r, g, b*/);
		else
			m_dwColor = D3DCOLOR_ARGB(m_nAlpha, 255,255,255/*r, g, b*/);
		if (!m_bIsMsgBox)
			m_pBM->DrawString("BACK-ESC", (m_nBoxRight-(25+(int)(300.0f*m_fTextScale*0.8f))), m_nBoxBottom-(int)(40.0f*m_fTextScale), m_fTextZ, m_fTextScale * 0.7f, m_dwColor);
		else
			m_pBM->DrawString("OK", (m_nBoxRight-(25+(int)(300.0f*m_fTextScale*0.8f))), m_nBoxBottom-(int)(40.0f*m_fTextScale), m_fTextZ, m_fTextScale * 0.7f, m_dwColor);
	}
	if (m_bAcceptInput)
	{
	    if (m_nCurrSelectedIndex == BTN_ENTER || m_nCurrInputIndex == BTN_ENTER)
			m_dwColor = D3DCOLOR_ARGB(m_nAlpha, 255,50,50/*r, g, b*/);
		else
			m_dwColor = D3DCOLOR_ARGB(m_nAlpha, 255,255,255/*r, g, b*/);
		if (m_bAcceptInput)
			m_pBM->DrawString("ENTER", m_nPosX+25, m_nBoxBottom-(int)(40.0f*m_fTextScale), m_fTextZ, m_fTextScale * 0.7f, m_dwColor );
	}
	//m_pBM->Reset();
}

int CBox::Input(POINT mousePt)
{
	if (m_bIsActive)
		CheckMouse(mousePt);
	if (m_bIsActive && m_bAcceptInput)
		CheckKeys();

	return m_nCurrSelectedIndex;
}

void CBox::SetActive(bool bIsActive)
{
	m_bIsActive = bIsActive;
	if (!bIsActive)
	{
		m_nAlpha = 150;
		m_bIsMouseInBox = false;
	}
	else
		m_nAlpha = 255;
}

void CBox::CheckKeys()
{
	if (m_pDI->CheckBufferedKeysEx())
	{
		if (m_pDI->KeyPressed(DIK_BACKSPACE) && m_sItems[m_nCurrInputIndex].size() > 0)
		{
			m_sItems[m_nCurrInputIndex].erase(m_sItems[m_nCurrInputIndex].size()-1, 1);
		}
		else if (m_sItems[m_nCurrInputIndex].size() < MAX_INPUT_SIZE && !m_pDI->KeyPressed(DIK_ESCAPE))
			m_sItems[m_nCurrInputIndex] += m_pDI->CheckKeys();
	}
	if (m_pDI->KeyPressed(DIK_ESCAPE) && m_bAcceptInput && m_nCurrInputIndex > -1)
	{
		m_sItems[m_nCurrInputIndex] = m_sOriginal[m_nCurrInputIndex];
		m_nCurrSelectedIndex = m_nCurrInputIndex = -1;
		m_bEnterText = false;
	}
	else if (m_pDI->KeyPressed(DIK_ESCAPE))
	{
		m_nCurrSelectedIndex = BTN_BACK;
		m_nCurrInputIndex = -1;
	}
	if (m_pDI->KeyPressed(DIK_RETURN) && m_sItems[m_nCurrInputIndex].size() > 0)
	{
		// accept the input...store it, and close the box
		m_nCurrSelectedIndex = BTN_ENTER;
	}
}