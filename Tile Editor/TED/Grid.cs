//  [5/27/2009 Ramon Johannessen (RJ)]

/// <summary>
///  File : Grid.cs
///  Purpose : Grid is used to draw the lines for both the map and tileset
/// </summary>
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using Microsoft.DirectX;
using Microsoft.DirectX.Direct3D;
using SGD;

namespace grid
{
    /// <summary>
    /// 
    /// </summary>
    public class CGrid
    {
        bool m_bIsIsometric = false;
        public bool BIsIsometric
        {
            get { return m_bIsIsometric; }
            set { m_bIsIsometric = value; }
        }
        int m_nIsoTopY;
        public int NIsoTopY
        {
            get { return m_nIsoTopY; }
            set { m_nIsoTopY = value; }
        }
        int m_nIsoCenterX;
        public int NIsoCenterX
        {
            get { return m_nIsoCenterX; }
            set { m_nIsoCenterX = value; }
        }
        int m_nZoomIncrement;
        public int ZoomIncrement
        {
            get { return m_nZoomIncrement; }
            set { m_nZoomIncrement = value; }
        }

        int m_nOffsetX;
        int m_nOffsetY;

        int m_nCellWidth;
        public int NCellWidth
        {
            get { return m_nCellWidth; }
            set { m_nCellWidth = value; }
        }
        int m_nCellHeight;
        public int NCellHeight
        {
            get { return m_nCellHeight; }
            set { m_nCellHeight = value; }
        }
        int m_nGridNumVertLines;
        public int NNumVertLines
        {
            get { return m_nGridNumVertLines; }
            set { m_nGridNumVertLines = value; }
        }
        int m_nGridNumHorLines;
        public int NNumHorizontalLines
        {
            get { return m_nGridNumHorLines; }
            set { m_nGridNumHorLines = value; }
        }
        int m_nGridZoom;
        public int NGridZoom
        {
            get { return m_nGridZoom; }
            set { m_nGridZoom = value; }
        }
        int m_nImageWidth;
        public int NImageWidth
        {
            get { return m_nImageWidth; }
            set { m_nImageWidth = value; }
        }
        int m_nImageHeight;
        public int NImageHeight
        {
            get { return m_nImageHeight; }
            set { m_nImageHeight = value; }
        }
        // Grid line variables
        Color clrGridLines;
        Point[] ptGridLinesVert;    
        Point[] ptGridLinesVertEnd; 
        Point[] ptGridLinesHoriz;   
        Point[] ptGridLinesHorizEnd;

        Point[] tilePoints;
        int m_nNumRows;
        int m_nNumCols;
        int m_nTotalTiles;
        int m_nType;
        public int Type
        {
            get { return m_nType; }
            set { m_nType = value; }
        }
        int m_nScrollX;
        int m_nScrollY;

        ManagedDirect3D mD3d;
        ManagedTextureManager mTM;
        int m_nDotID;

        public CGrid(int cellWidth, int cellHeight, int numVertical, int numHorizontal,
            int gridZoom, int zoomIncrement, int xOff, int yOff, bool bIsIso, int type)
        {
            // where to start drawing the grid
            m_nOffsetX = xOff;
            m_nOffsetY = yOff;

            // line color
            clrGridLines = Color.Black;

            // dimensions
            m_nCellWidth        = cellWidth;
            m_nCellHeight       = cellHeight;
            m_nNumRows          = numVertical;
            m_nNumCols          = numHorizontal;
            m_nTotalTiles       = m_nNumCols * m_nNumRows;
            NNumVertLines       = numVertical   + 1;  //always one more line than there are tiles
            NNumHorizontalLines = numHorizontal + 1;
            m_nZoomIncrement    = zoomIncrement;
            NGridZoom           = gridZoom;

            // line begin and end points
            ptGridLinesVertEnd  = new Point[NNumVertLines];
            ptGridLinesVert     = new Point[NNumVertLines];
            ptGridLinesHorizEnd = new Point[NNumHorizontalLines];
            ptGridLinesHoriz    = new Point[NNumHorizontalLines];

            tilePoints = new Point[m_nTotalTiles];

            m_bIsIsometric = bIsIso;
            if (m_bIsIsometric)
            {
                m_nType = type;
                switch (m_nType)
                {
                    case (int)IsoType.ISO_DIAMOND:
                        {
                            m_nIsoCenterX = (m_nNumCols >> 1) * cellWidth + (cellWidth >> 1);
                        }
                        break;
                    case (int)IsoType.ISO_SLIDE:
                        break;
                    case (int)IsoType.ISO_STAG:
                        {
                            // TODO::get these to center for staggered
                            m_nIsoCenterX = cellWidth;
                        }
                        break;
                }
            }
            // the d3d device to draw lines
            mD3d = ManagedDirect3D.Instance;
            mTM = ManagedTextureManager.Instance;
        }
        public void SetDotID(int id)
        {
            m_nDotID = id;
        }

        //  Instead of using lines to draw the grid, 
        //  I will use dots to represent the top center of the tile
        
        //  This will require different plotting functions depending
        //  on what method is used to for drawing the map (diamond, staggered, or slide)
        //  for selection purposes, a "rect" will be drawn around the currently
        //  hovered-over tile to notify the user where they are in the grid

        //  an option could also be given to turn on the outside grid lines so they
        //  know where the boundaries of the map are..
        private Point PlotPointDiamond(Point pt, int xOffset, int yOffset)
        {
            Point newPt = new Point();
            newPt.X = (pt.X - pt.Y) * (m_nCellWidth >> 1) + xOffset;
            newPt.Y = (pt.X + pt.Y) * (m_nCellHeight >> 1) + yOffset;
            return newPt;
        }
        private Point PlotPointSlide(Point pt, int xOffset, int yOffset)
        {
            Point newPt = new Point();
            newPt.X = pt.X * m_nCellWidth + pt.Y * (m_nCellWidth >> 1) + xOffset;
            newPt.Y = pt.Y * (m_nCellHeight >> 1) + yOffset;
            return newPt;
        }
        private Point PlotPointStag(Point pt, int xOffset, int yOffset)
        {
            Point newPt = new Point();
            newPt.X = pt.X * m_nCellWidth + (pt.Y&1) * (m_nCellWidth >> 1) + xOffset;
            newPt.Y = pt.Y * (m_nCellHeight >> 1) + yOffset;
            return newPt;
        }
        private Point PlotPointRect(Point pt, int xOffset, int yOffset)
        {
            Point newPt = new Point();
            newPt.X = pt.X;
            newPt.Y = pt.Y;
            return newPt;
        }

        public void SetGridPts()
        {
            if (m_bIsIsometric)
            {
                for (int y = 0; y < m_nNumRows; ++y)
                {
                    for (int x = 0; x < m_nNumCols; ++x)
                    {
                        switch (m_nType)
                        {
                            case (int)IsoType.ISO_DIAMOND:
                                tilePoints[y * m_nNumCols + x] = PlotPointDiamond(new Point(x, y), m_nIsoCenterX + m_nScrollX, m_nIsoTopY + m_nScrollY);
                                break;
                            case (int)IsoType.ISO_SLIDE:
                                tilePoints[y * m_nNumCols + x] = PlotPointSlide(new Point(x, y), m_nIsoCenterX + m_nScrollX, m_nIsoTopY + m_nScrollY);
                                break;
                            case (int)IsoType.ISO_STAG:
                                tilePoints[y * m_nNumCols + x] = PlotPointStag(new Point(x, y), m_nIsoCenterX + (m_nCellWidth >> 1) + m_nScrollX, m_nIsoTopY + m_nScrollY);
                                break;
                        }
                    }
                }
            }
        }

        private void SetGridLines( )
        {
            if (!m_bIsIsometric)
            {
	            for (int i = 0; i < m_nGridNumVertLines; ++i)
	            {
	                ptGridLinesVert[i].X = (i * (NCellWidth + m_nGridZoom)) + m_nOffsetX;
	                ptGridLinesVert[i].Y = m_nOffsetY;
	                ptGridLinesVertEnd[i].X = ptGridLinesVert[i].X;
	                ptGridLinesVertEnd[i].Y = ((m_nCellHeight + m_nGridZoom) * m_nGridNumHorLines - (m_nCellHeight + m_nGridZoom)) + m_nOffsetY;
	            }
	            for (int i2 = 0; i2 < m_nGridNumHorLines; ++i2)
	            {
	                ptGridLinesHoriz[i2].X = m_nOffsetX;
	                ptGridLinesHoriz[i2].Y = (i2 * (m_nCellHeight + m_nGridZoom)) + m_nOffsetY;
	                ptGridLinesHorizEnd[i2].X = ((NCellWidth + m_nGridZoom) * m_nGridNumVertLines - (NCellWidth + m_nGridZoom)) + m_nOffsetX;
	                ptGridLinesHorizEnd[i2].Y = ptGridLinesHoriz[i2].Y;
	            }
            } 
            else
            {

            }
        }

        public void CenterOnY(int windowHeight, int mapHeight)
        {
            if (windowHeight > mapHeight)
                m_nIsoTopY = ((windowHeight - mapHeight) >> 1) - m_nCellHeight;
            else
                m_nIsoTopY = 0;// (windowHeight >> 1) - (mapHeight >> 1);
            SetGridPts();
        }
        public void CenterOnX(int windowWidth, int mapWidth)
        {
            if (windowWidth - 5 > mapWidth)
                m_nIsoCenterX = ((windowWidth - mapWidth) >> 1) - m_nCellWidth;
            else
                m_nIsoCenterX = m_nCellWidth;// (windowWidth >> 1) - (mapWidth >> 1);
            SetGridPts();
        }
        public void SetMapX(int x)
        {
            m_nIsoCenterX = x;
        }
        public void SetMapY(int y)
        {
            m_nIsoTopY = y;
        }

        public void DrawGrid(bool iso, Color clr)
        {
            if (iso)
            {
                Rectangle srcRect = new Rectangle(new Point(0, 0), new Size(3, 3));
                for (int i = 0; i < m_nTotalTiles; ++i)
                    mTM.Draw(m_nDotID, tilePoints[i].X - 1 + m_nScrollX, tilePoints[i].Y - 1 + m_nScrollY, 1.0f, 1.0f,
                        srcRect, 0, 0, 0.0f, clr.ToArgb());
            }
            else
            {
                for (int i = 0; i < m_nGridNumHorLines; ++i)
                {
                    mD3d.DrawLine(ptGridLinesHoriz[i].X + m_nScrollX, ptGridLinesHoriz[i].Y + m_nScrollY,
                                       ptGridLinesHorizEnd[i].X + m_nScrollX, ptGridLinesHorizEnd[i].Y + m_nScrollY,
                                       clrGridLines.R, clrGridLines.G, clrGridLines.B);
                }
                for (int i2 = 0; i2 < m_nGridNumVertLines; ++i2)
                {
                    mD3d.DrawLine(ptGridLinesVert[i2].X + m_nScrollX, ptGridLinesVert[i2].Y + m_nScrollY,
                                       ptGridLinesVertEnd[i2].X + m_nScrollX, ptGridLinesVertEnd[i2].Y + m_nScrollY,
                                        clrGridLines.R, clrGridLines.G, clrGridLines.B);
                }
            }
        }
        public void DrawSelectionRect(int xID, int yID)
        {
            Point topPt = new Point();
            Point btmPt;

            switch (m_nType)
            {
                case (int)IsoType.ISO_DIAMOND:
                    topPt = PlotPointDiamond(new Point(xID, yID), m_nIsoCenterX + m_nScrollX, m_nIsoTopY + m_nScrollY);
                    break;
                case (int)IsoType.ISO_SLIDE:                                    
                    topPt = PlotPointSlide(new Point(xID, yID), m_nIsoCenterX + m_nScrollX, m_nIsoTopY + m_nScrollY);
                    break;
                case (int)IsoType.ISO_STAG:
                    topPt = PlotPointStag(new Point(xID, yID), m_nIsoCenterX + (m_nCellWidth >> 1) + m_nScrollX, m_nIsoTopY + m_nScrollY);
                    break;
                case (int)IsoType.RECT:
                    break;
            }
            btmPt = topPt; btmPt.Y = topPt.Y + m_nCellHeight;
            int halfWidth = (m_nCellWidth >> 1);
            int halfHeight = (m_nCellHeight >> 1);
            // top to left
            mD3d.DrawLine(topPt.X, topPt.Y, topPt.X - halfWidth, topPt.Y + halfHeight, 255, 255, 255);
            // top to right
            mD3d.DrawLine(topPt.X, topPt.Y, topPt.X + halfWidth, topPt.Y + halfHeight, 255, 255, 255);
            // btm to left
            mD3d.DrawLine(btmPt.X, btmPt.Y, btmPt.X - halfWidth, btmPt.Y - halfHeight, 255, 255, 255);
            // btm to right
            mD3d.DrawLine(btmPt.X, btmPt.Y, btmPt.X + halfWidth, btmPt.Y - halfHeight, 255, 255, 255);
        }

        public void GridAdjust(int vertLines, int horizontalLines)
        {
            NNumVertLines       = vertLines + 1;
            NNumHorizontalLines = horizontalLines + 1;

            ptGridLinesVertEnd  = new Point[NNumVertLines];
            ptGridLinesVert     = new Point[NNumVertLines];
            ptGridLinesHorizEnd = new Point[NNumHorizontalLines];
            ptGridLinesHoriz    = new Point[NNumHorizontalLines];
            SetGridLines();
        }
        public int GridAdjustNumCols(int numCols)
        {
            return CellSizeAdjustWidth(m_nImageWidth / numCols);
        }
        public int GridAdjustNumRows(int numRows)
        {
            return CellSizeAdjustHeight(m_nImageHeight / numRows);
        }
        public void ZoomAdjust(float zoom)
        {
            m_nGridZoom = (int)(-(1.0f - zoom) * 32.0f);
            SetGridLines();
        }
        // for the map grid which must contain a square, no rectangles...etc. (in non-isometric only)
        public void CellSizeAdjust(int cellSize)
        {
            m_nCellWidth = cellSize;
            m_nCellHeight = cellSize;
            SetGridLines();
        }
        public int CellSizeAdjustWidth(int cellWidth) 
        {
            m_nCellWidth = cellWidth;
            if (m_nGridNumVertLines < m_nImageWidth / cellWidth + 1)
            {
                m_nGridNumVertLines = m_nImageWidth / cellWidth + 1;
                ptGridLinesVertEnd = new Point[m_nGridNumVertLines];
                ptGridLinesVert = new Point[m_nGridNumVertLines];
            }
            m_nGridNumVertLines = m_nImageWidth / cellWidth + 1;
            for (int i = 0; i < m_nGridNumVertLines; ++i)
            {
                ptGridLinesVert[i].X = (i * (NCellWidth + m_nGridZoom)) + m_nOffsetX;
                ptGridLinesVert[i].Y = m_nOffsetY;
                ptGridLinesVertEnd[i].X = ptGridLinesVert[i].X;
                ptGridLinesVertEnd[i].Y = ((m_nCellHeight + m_nGridZoom) * m_nGridNumHorLines - (m_nCellHeight + m_nGridZoom)) + m_nOffsetY;
            }
            return m_nCellWidth;
        }
        public int CellSizeAdjustHeight(int cellHeight)
        {
            m_nCellHeight = cellHeight;
            if (m_nGridNumHorLines < m_nImageHeight / cellHeight + 1)
            {
                m_nGridNumHorLines = m_nImageHeight / cellHeight + 1;
                ptGridLinesHorizEnd = new Point[m_nGridNumHorLines];
                ptGridLinesHoriz = new Point[m_nGridNumHorLines];
            }
            m_nGridNumHorLines = m_nImageHeight / cellHeight + 1;
            for (int i2 = 0; i2 < m_nGridNumHorLines; ++i2)
            {
                ptGridLinesHoriz[i2].X = m_nOffsetX;
                ptGridLinesHoriz[i2].Y = (i2 * (m_nCellHeight + m_nGridZoom)) + m_nOffsetY;
                ptGridLinesHorizEnd[i2].X = ((NCellWidth + m_nGridZoom) * m_nGridNumVertLines - (NCellWidth + m_nGridZoom)) + m_nOffsetX;
                ptGridLinesHorizEnd[i2].Y = ptGridLinesHoriz[i2].Y;
            }
            return m_nCellHeight;
        }
        public void Offset(int x, int y)
        {
            m_nScrollX += x;
            m_nScrollY += y;
//             for (int i = 0; i < m_nGridNumVertLines; ++i)
//             {
//                 ptGridLinesVert[i].X    += x;
//                 ptGridLinesVert[i].Y    += y;
//                 ptGridLinesVertEnd[i].X += x;
//                 ptGridLinesVertEnd[i].Y += y;
// 
//             }
//             for (int i2 = 0; i2 < m_nGridNumHorLines; ++i2)
//             {
//                 ptGridLinesHoriz[i2].X      += x;
//                 ptGridLinesHoriz[i2].Y      += y;
//                 ptGridLinesHorizEnd[i2].X   += x;
//                 ptGridLinesHorizEnd[i2].Y   += y;
//             }
//             for (int i = 0; i < m_nTotalTiles; ++i)
//             { tilePoints[i].X += x; tilePoints[i].Y += y; }
        }
        public void ZeroOffset()
        {
            m_nScrollX = m_nScrollY = 0;
            int x = 0, y = 0;
            for (int i = 0; i < m_nGridNumVertLines; ++i)
            {
                ptGridLinesVert[i].X = x;
                ptGridLinesVert[i].Y = y;
                ptGridLinesVertEnd[i].X = x;
                ptGridLinesVertEnd[i].Y = y;
            }
            for (int i2 = 0; i2 < m_nGridNumHorLines; ++i2)
            {
                ptGridLinesHoriz[i2].X = x;
                ptGridLinesHoriz[i2].Y = y;
                ptGridLinesHorizEnd[i2].X = x;
                ptGridLinesHorizEnd[i2].Y = y;
            }
            for (int i = 0; i < m_nTotalTiles; ++i )
            { tilePoints[i].X = tilePoints[i].Y = 0; }
        }
        public float ABS (float nIN)
        {
            //float result = 1.0f - nIN;
            if (nIN < 0)
                return -nIN;
            return nIN;
        }
    }
}