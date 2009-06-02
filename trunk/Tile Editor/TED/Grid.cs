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
        int m_nIsoCenterLeftY;
        public int NIsoCenterLeftY
        {
            get { return m_nIsoCenterLeftY; }
            set { m_nIsoCenterLeftY = value; }
        }
        int m_nIsoCenterTopX;
        public int NIsoCenterTopX
        {
            get { return m_nIsoCenterTopX; }
            set { m_nIsoCenterTopX = value; }
        }
        int m_nZoomIncrement;
        public int ZoomIncrement
        {
            get { return m_nZoomIncrement; }
            set { m_nZoomIncrement = value; }
        }

        int offsetX;
        int offsetY;

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

        ManagedDirect3D mD3d;

        public CGrid(int cellWidth, int cellHeight, int numVertical, int numHorizontal, int gridZoom, int zoomIncrement, int xOff, int yOff, bool bIsIso)
        {
            // where to start drawing the grid
            offsetX = xOff;
            offsetY = yOff;

            // line color
            clrGridLines = Color.Black;

            // dimensions
            m_nCellWidth        = cellWidth;
            m_nCellHeight       = cellHeight;
            NNumVertLines       = numVertical   + 1;  //always one more line than there are tiles
            NNumHorizontalLines = numHorizontal + 1;
            m_nZoomIncrement    = zoomIncrement;
            NGridZoom           = gridZoom;

            // line begin and end points
            ptGridLinesVertEnd  = new Point[NNumVertLines];
            ptGridLinesVert     = new Point[NNumVertLines];
            ptGridLinesHorizEnd = new Point[NNumHorizontalLines];
            ptGridLinesHoriz    = new Point[NNumHorizontalLines];

            m_bIsIsometric = bIsIso;
            if (m_bIsIsometric)
            {
                m_nIsoCenterLeftY = (int)((((float)(NNumHorizontalLines-1)) / 2.0f) * cellHeight);
                m_nIsoCenterTopX  = (int)((((float)(NNumVertLines-1))       / 2.0f) * cellWidth);
            }
            // the d3d device to draw lines
            mD3d = ManagedDirect3D.Instance;
            SetGridLines();
        }
        private void SetGridLines( )
        {
            if (!m_bIsIsometric)
            {
	            for (int i = 0; i < m_nGridNumVertLines; ++i)
	            {
	                ptGridLinesVert[i].X = (i * (NCellWidth + m_nGridZoom)) + offsetX;
	                ptGridLinesVert[i].Y = offsetY;
	                ptGridLinesVertEnd[i].X = ptGridLinesVert[i].X;
	                ptGridLinesVertEnd[i].Y = ((m_nCellHeight + m_nGridZoom) * m_nGridNumHorLines - (m_nCellHeight + m_nGridZoom)) + offsetY;
	            }
	            for (int i2 = 0; i2 < m_nGridNumHorLines; ++i2)
	            {
	                ptGridLinesHoriz[i2].X = offsetX;
	                ptGridLinesHoriz[i2].Y = (i2 * (m_nCellHeight + m_nGridZoom)) + offsetY;
	                ptGridLinesHorizEnd[i2].X = ((NCellWidth + m_nGridZoom) * m_nGridNumVertLines - (NCellWidth + m_nGridZoom)) + offsetX;
	                ptGridLinesHorizEnd[i2].Y = ptGridLinesHoriz[i2].Y;
	            }
            } 
            else
            {
                for (int i = 0; i < m_nGridNumVertLines; ++i )
                {
                    // lines start from left corner and go northeast
                    // they progress to the southeast
                    ptGridLinesVert[i].X = i * ((m_nCellWidth >> 1) + m_nGridZoom) + offsetX;
                    ptGridLinesVertEnd[i].X = m_nIsoCenterTopX + (i * ((m_nCellWidth >> 1) + m_nGridZoom)) + offsetX;
                    ptGridLinesVert[i].Y    = m_nIsoCenterLeftY + (i * (m_nCellHeight >> 1)) + offsetY;
                    ptGridLinesVertEnd[i].Y = i * (m_nCellHeight >> 1) + offsetY;
                }
                for (int i2 = 0; i2 < m_nGridNumHorLines; ++i2 )
                {
                    // lines start from top corner and go southeast
                    // they progress southwest
                    ptGridLinesHoriz[i2].X = m_nIsoCenterTopX - (i2 * ((m_nCellWidth >> 1) + m_nGridZoom)) + offsetX;
                    ptGridLinesHorizEnd[i2].X = (m_nCellWidth + m_nGridZoom) * (NNumVertLines - 1) - (i2 * ((m_nCellWidth >> 1) + m_nGridZoom)) + offsetX;
                    ptGridLinesHoriz[i2].Y = i2 * (m_nCellHeight >> 1) + offsetY;
                    ptGridLinesHorizEnd[i2].Y = m_nIsoCenterLeftY + (i2 * (m_nCellHeight >> 1)) + offsetY;
                }
            }
        }

        public void DrawGrid()
        {
            for (int i = 0; i < m_nGridNumHorLines; ++i)
            {
                mD3d.DrawLine(ptGridLinesHoriz[i].X, ptGridLinesHoriz[i].Y,
                                ptGridLinesHorizEnd[i].X, ptGridLinesHorizEnd[i].Y,
                                clrGridLines.R, clrGridLines.G, clrGridLines.B);
            }
            for (int i2 = 0; i2 < m_nGridNumVertLines; ++i2)
            {
                mD3d.DrawLine(ptGridLinesVert[i2].X, ptGridLinesVert[i2].Y,
                                ptGridLinesVertEnd[i2].X, ptGridLinesVertEnd[i2].Y,
                                clrGridLines.R, clrGridLines.G, clrGridLines.B);
            }
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
        public int CellSizeAdjustWidth(int cellWidth)  // for isometric adjust width
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
                ptGridLinesVert[i].X = (i * (NCellWidth + m_nGridZoom)) + offsetX;
                ptGridLinesVert[i].Y = offsetY;
                ptGridLinesVertEnd[i].X = ptGridLinesVert[i].X;
                ptGridLinesVertEnd[i].Y = ((m_nCellHeight + m_nGridZoom) * m_nGridNumHorLines - (m_nCellHeight + m_nGridZoom)) + offsetY;
            }
            return m_nCellWidth;
        }
        public int CellSizeAdjustHeight(int cellHeight)    // for isometric adjust height
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
                ptGridLinesHoriz[i2].X = offsetX;
                ptGridLinesHoriz[i2].Y = (i2 * (m_nCellHeight + m_nGridZoom)) + offsetY;
                ptGridLinesHorizEnd[i2].X = ((NCellWidth + m_nGridZoom) * m_nGridNumVertLines - (NCellWidth + m_nGridZoom)) + offsetX;
                ptGridLinesHorizEnd[i2].Y = ptGridLinesHoriz[i2].Y;
            }
            return m_nCellHeight;
        }
        public void Offset(int x, int y)
        {
            for (int i = 0; i < m_nGridNumVertLines; ++i)
            {
                ptGridLinesVert[i].X    += x;
                ptGridLinesVert[i].Y    += y;
                ptGridLinesVertEnd[i].X += x;
                ptGridLinesVertEnd[i].Y += y;

            }
            for (int i2 = 0; i2 < m_nGridNumHorLines; ++i2)
            {
                ptGridLinesHoriz[i2].X      += x;
                ptGridLinesHoriz[i2].Y      += y;
                ptGridLinesHorizEnd[i2].X   += x;
                ptGridLinesHorizEnd[i2].Y   += y;
            }
        }
        public void ZeroOffset()
        {
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
        }
        public float ABS (float nIN)
        {
            float result = 1.0f - nIN;
            if (result < 0)
                return -result;
            return result;
        }
    }
}