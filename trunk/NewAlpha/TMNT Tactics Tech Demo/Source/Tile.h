//////////////////////////////////////////////////////////////////////////
//	Filename	:	CTile.h
//
//	Author		:	Ramon Johannessen (RJ)
//
//	Purpose		:	To define the CTile and CFreeTile classes...which will be used in drawing
//					the individual tiles of the battle maps
//////////////////////////////////////////////////////////////////////////
#ifndef TILE_H
#define TILE_H

#include <string>
using std::string;

// for all regular tiles that are put into the grid
class CTile
{
private:
	int			m_nSourceID;		// where from the source is this tile?
	//int			m_nSourceTS;
	int 		m_nImageID;			// which image does this tile come from?
	int			m_nDestXID;			// the ID of the tile on the battle map
	int			m_nDestYID;

	int 		m_nHeight;			// tile's width
	int 		m_nWidth;			// tile's height
	int 		m_nFlag;			// collision/other flags

	// these two are used for pathfinding
	int			m_nCost;
	int			m_nTerrainCost;
	//////////////////////////////////////////////////////////////////////////

	int			m_nAlpha;
	string		m_strTrigger;		// trigger string, used to trigger a specific event from this tile
	RECT		m_rSrc;				// the source rect of the tile

public:
	CTile(void)
	{
		m_nSourceID = 0;
		m_nImageID = 0;
		m_nDestYID = 0;
		m_nDestXID = -1;
		m_nHeight = 0;
		m_nWidth = 0;
		m_nFlag = 0;
		m_nAlpha = 255;
		m_strTrigger = "";
		m_nCost = -1; m_nTerrainCost = 2;
		m_rSrc.left = m_rSrc.right = m_rSrc.top = m_rSrc.bottom = 0;
			
	}
	~CTile(void){}
	CTile(int nSourceID, int nImageID, int numCols, int nDestX, int nDestY, int nWidth, int nHeight, int nFlag, string strTrigger)
	{
		// setup source rect here, once and for all
		int left, top, right, bottom; 
		left = nSourceID % numCols * nWidth;
		top = (nSourceID / numCols) * nHeight;
		right = left + nWidth;
		bottom = top + nHeight;
		RECT rect = {left, top, right, bottom};
		m_rSrc = rect;
		//////////////////////////////////////////////////////////////////////////
		m_nAlpha = 255;
		m_nImageID = nImageID;
		m_nSourceID = nSourceID;
		m_nDestXID = nDestX;
		m_nDestYID = nDestY;
		m_nWidth = nWidth;
		m_nHeight = nHeight;
		m_nFlag = nFlag;
		m_strTrigger = strTrigger;
		m_nCost = -1; m_nTerrainCost = 2;
	}

	//////////////////////////////////////////////////////////////////////////
	//	Accessors
	//////////////////////////////////////////////////////////////////////////
	RECT* SourceRect()		  { return &m_rSrc;}
	int  SourceID()		const { return m_nSourceID; }
	int  ImageID()		const { return m_nImageID; }
	int  DestXID()		const { return m_nDestXID;}
	int  DestYID()		const { return m_nDestYID;}
	int  Height()		const { return m_nHeight; }
	int  Width()		const { return m_nWidth; }
	int  Flag()			const { return m_nFlag; }
	int  Alpha()		const { return m_nAlpha;}
	int	 Cost()			const { return m_nCost;}
	int  TerrainCost()	const { return m_nTerrainCost;}
	string Trigger()	const { return m_strTrigger; }

	//////////////////////////////////////////////////////////////////////////
	//	Mutators
	//////////////////////////////////////////////////////////////////////////
// 	void SourceID(int val) { m_nSourceID = val; }
// 	void ImageID(int val) { m_nImageID = val; }
// 	void DestinationID(int val) { m_nDestinationID = val; }
// 	void Height(int val) { m_nHeight = val; }
// 	void Width(int val) { m_nWidth = val; }
// 	void Flag(int val) { m_nFlag = val; }
// 	void Trigger(std::string val) { m_strTrigger = val; }
	void SetAlpha(int alpha) {m_nAlpha = alpha;}
	void SetCost(int cost)	 {m_nCost = cost;}
	void SetTerrainCost(int cost) {m_nTerrainCost = cost;}

};

// for all freely placed tiles that are NOT put into the grid...mostly the tiles that make it look pretty
class CFreeTile
{
private:
	int m_nSourceX;			// where from the source is this tile?
	int m_nSourceY;

	int m_nImageID;			// which image does this tile come from?
	//int m_nSourceTS;
	int m_nDestX;			// where is the tile placed on the screen?
	int m_nDestY;
	int m_nHeight;			// tile's width
	int m_nWidth;			// tile's height
	float m_fRotation;
	int m_nFlag;			// collision/other flags
	int m_nAlpha;			// tile's alpha
	std::string m_strTrigger;	// trigger string, used to trigger a specific event from this tile
	RECT m_rSrc;

public:
	CFreeTile() {}
	~CFreeTile() {}
	CFreeTile(int nSourceX, int nSourceY, int nImageID, int nDestX, int nDestY, int nWidth, int nHeight, int nFlag, string strTrigger, float rotation = 0.0f)
	{
		RECT rect = {nSourceX, nSourceY, nSourceX + nWidth, nSourceY + nHeight};
		m_rSrc = rect;
		m_nImageID = nImageID;
		m_nSourceX = nSourceX;
		m_nSourceY = nSourceY;
		m_nDestX = nDestX;
		m_nDestY = nDestY;
		m_nWidth = nWidth;
		m_nHeight = nHeight;
		m_nFlag = nFlag;
		m_strTrigger = strTrigger;
		m_fRotation = rotation;
		m_nAlpha = 255;
	}

	//////////////////////////////////////////////////////////////////////////
	//	Accessors
	//////////////////////////////////////////////////////////////////////////
	RECT* SourceRect()		  { return &m_rSrc;		}
	int ImageID()		const { return m_nImageID;	}
	//int SourceTS()		const { return m_nSourceTS; }
	int SourceX()		const { return m_nSourceX;	}
	int SourceY()		const { return m_nSourceY;	}
	int DestX()			const { return m_nDestX;	}
	int DestY()			const { return m_nDestY;	}
	int Height()		const { return m_nHeight;	}
	int Width()			const { return m_nWidth;	}
	int Flag()			const { return m_nFlag;		}
	int Alpha()			const { return m_nAlpha;	}
	float Rotation()	const { return m_fRotation;}
	std::string Trigger() const { return m_strTrigger; }

	//////////////////////////////////////////////////////////////////////////
	//	Mutators
	//////////////////////////////////////////////////////////////////////////
// 	void ImageID(int val) { m_nImageID = val; }
// 	void SourceX(int val) { m_nSourceX = val; }
// 	void SourceY(int val) { m_nSourceY = val; }
// 	void DestX(int val) { m_nDestX = val; }
// 	void DestY(int val) { m_nDestY = val; }
// 	void Height(int val) { m_nHeight = val; }
// 	void Width(int val) { m_nWidth = val; }
// 	void Flag(int val) { m_nFlag = val; }
// 	void Trigger(std::string val) { m_strTrigger = val; }
	void SetAlpha(int alpha) {m_nAlpha = alpha;}
};
#endif