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
using tile;
using grid;

namespace test
{
    /// <summary>
    /// 
    /// </summary>
    public class CTileset
    {
        // the grid
        CGrid m_gTilesetGrid;
        public CGrid GTilesetGrid
        {
            get { return m_gTilesetGrid; }
            set { m_gTilesetGrid = value; }
        }

        // all the tiles from the image
        CTILE[] m_tTilesetTiles;
        public CTILE[] TTilesetTiles
        {
            get { return m_tTilesetTiles; }
            set { m_tTilesetTiles = value; }
        }
        CTILE[] m_tMarqueeTiles;
        public CTILE[] MarqueeTiles
        {
            get { return m_tMarqueeTiles; }
            set { m_tMarqueeTiles = value; }
        }
        // tile set image key color
        Color clrKey;
        // where to start drawing in the control
        int nOffsetX = 10;
        int nOffsetY = 140;
        int nScrollOSx = 0;
        int nScrollOSy = 0;
        int m_nPanelHeight;
        public int NPanelHeight
        {
            get { return m_nPanelHeight; }
            set { m_nPanelHeight = value; }
        }
        int m_nPanelWidth;
        public int NPanelWidth
        {
            get { return m_nPanelWidth; }
            set { m_nPanelWidth = value; }
        }
        // image name (and path)
        string m_strFilePath;
        public string StrFilePath
        {
            get { return m_strFilePath; }
            set { m_strFilePath = value; }
        }
        // tileset image ID for texture manager
        int nTilesetImageID;
        public int NTilesetImageID
        {
            get { return nTilesetImageID; }
            set { nTilesetImageID = value; }
        }
        // pixel width of image
        int m_nImageWidth;
        public int NWidth
        {
            get { return m_nImageWidth; }
            set { m_nImageWidth = value; }
        }
        // pixel height of image
        int m_nImageHeight;
        public int NHeight
        {
            get { return m_nImageHeight; }
            set { m_nImageHeight = value; }
        }

        // current tile set width
        int m_nTilesetWidth;
        public int NTilesetWidth
        {
            get { return m_nTilesetWidth; }
            set { m_nTilesetWidth = value; }
        }
        // current tile set height
        int m_nTilesetHeight;
        public int NTilesetHeight
        {
            get { return m_nTilesetHeight; }
            set { m_nTilesetHeight = value; }
        }
        // each individual cell (always multiples of 2)
        int m_nCellSize;
        public int NCellSize
        {
            get { return m_nCellSize; }
            set { m_nCellSize = value; }
        }
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
        int m_nNumCols;
        public int NNumCols
        {
            get { return m_nNumCols; }
            set { m_nNumCols = value; }
        }
        int m_nNumRows;
        public int NNumRows
        {
            get { return m_nNumRows; }
            set { m_nNumRows = value; }
        }
        int m_nTotalNumTiles;
        public int NTotalNumTiles
        {
            get { return m_nTotalNumTiles; }
            set { m_nTotalNumTiles = value; }
        }
        // the ID of the currently selected tile
        int m_nCurrSelectedTile = -1;
        public int NCurrSelectedTile
        {
            get { return m_nCurrSelectedTile; }
            set { m_nCurrSelectedTile = value; }
        }
        bool m_bDrawMarquee;
        public bool DrawMarquee
        {
            get { return m_bDrawMarquee; }
            set { m_bDrawMarquee = value; }
        }
        // current layer
        int m_nCurrLayer;
        public int NCurrLayer
        {
            get { return m_nCurrLayer; }
            set { m_nCurrLayer = value; }
        }
        bool m_bShowFlags = true;
        public bool BShowFlags
        {
            get { return m_bShowFlags; }
            set { m_bShowFlags = value; }
        }
        bool m_bShowGrid = true;
        public bool ShowGrid
        {
            get { return m_bShowGrid; }
            set { m_bShowGrid = value; }
        }
        ManagedTextureManager mTM;
        ManagedDirect3D mD3d;

        int left;
        int top;
        int width;
        int height;
        int xPOI;
        int yPOI;

        // CTORs
        public CTileset()
        {
            mD3d = ManagedDirect3D.Instance;
            mTM = ManagedTextureManager.Instance;
        }
        public CTileset(int imageID, Color key, int width, int height, int cellWidth, int cellHeight,  string filepath)
        {
            mD3d = ManagedDirect3D.Instance;
            mTM = ManagedTextureManager.Instance;
            m_nCurrLayer      = 1;
            nTilesetImageID = imageID;
            m_nImageWidth     = m_nTilesetWidth = width;
            m_nImageHeight    = m_nTilesetHeight = height;
            m_strFilePath     = filepath;
            m_nCellSize       = cellWidth;
            NCellWidth        = cellWidth;
            m_nCellHeight     = cellHeight;
            m_nNumCols        = m_nImageWidth / cellWidth;
            m_nNumRows        = m_nImageHeight / cellHeight;
            m_nTotalNumTiles  = m_nNumCols * m_nNumRows;

            m_gTilesetGrid = new CGrid(cellWidth, cellHeight, m_nNumCols, m_nNumRows, 0, 0, nOffsetX, nOffsetY, false);
            m_gTilesetGrid.NImageHeight = m_nImageHeight;
            m_gTilesetGrid.NImageWidth = m_nImageWidth;
            m_strFilePath = filepath;
            if (key != null)
                clrKey = key;
            else
                clrKey = Color.White;
            SetupTileset(0, imageID);
        }
        public void SetupTileset(int tileFlag, int imageID)
        {
            m_tTilesetTiles = new CTILE[m_nTotalNumTiles];
            for (int id = 0; id < m_nTotalNumTiles; ++id)
            {
                int x = id % m_nNumCols * m_nCellWidth;
                int y = id / m_nNumCols * m_nCellWidth;

                Rectangle srcRect = new Rectangle( x, y, m_nCellWidth, m_nCellHeight);
                m_tTilesetTiles[id] = new CTILE(id, srcRect, tileFlag, imageID);
            }
        }
        public void DrawTSGrid()
        {
            if (m_bShowGrid)
                m_gTilesetGrid.DrawGrid();
        }
        public void DrawSelectionRect()
        {
            if (m_nCurrSelectedTile != -1)
            {
                //top line
                mD3d.DrawLine(left, top, width, top, 0, 0, 255);
                //bottom line
                mD3d.DrawLine(left, height, width, height, 0, 0, 255);
                //left line
                mD3d.DrawLine(left, top, left, height, 0, 0, 255);
                //right line
                mD3d.DrawLine(width, top, width, height, 0, 0, 255);

                // draw crosshair for anchor point
                // horizontal line
                mD3d.DrawLine(left + xPOI - 6, top + yPOI, left + xPOI + 6, top + yPOI, 255, 0, 0);
                // vertical line
                mD3d.DrawLine(left + xPOI, top + yPOI - 6, left + xPOI, top + yPOI + 6, 255, 0, 0);
            }
        }
        public void SetSelectionRect()
        {
            left = (((NCurrSelectedTile % m_nNumCols) * m_nCellWidth) + (nOffsetX + nScrollOSx));
            top = (((NCurrSelectedTile / m_nNumCols) * m_nCellHeight) + (nOffsetY + nScrollOSy));
            width = left + m_nCellWidth;
            height = top + m_nCellHeight;
            // anchor point crosshairs
            xPOI = m_tTilesetTiles[NCurrSelectedTile].AnchorX;
            yPOI = m_tTilesetTiles[NCurrSelectedTile].AnchorY;
        }
        public void SetMarqueeSelectionRect(int firstTileID, int numCols, int numRows)
        {
            left = (((firstTileID % m_nNumCols) * m_nCellWidth) + (nOffsetX + nScrollOSx));
            top = (((firstTileID / m_nNumCols) * m_nCellHeight) + (nOffsetY + nScrollOSy));
            width = left + numCols * m_nCellWidth;
            height = top + numRows * m_nCellHeight;
        }
        public void OffsetImage(int x, int y)
        {
            nScrollOSx += x;
            nScrollOSy += y;
        }
        public void AdjustCellWidth(int cellWidth)
        {
            m_nCellWidth = cellWidth;
            GTilesetGrid.CellSizeAdjustWidth(cellWidth);
        }
        public void AdjustCellHeight(int cellHeight)
        {
            m_nCellHeight = cellHeight;
            GTilesetGrid.CellSizeAdjustHeight(cellHeight);
        }
        public void DrawTileset()
        {
            for (int id = 0; id < m_nTotalNumTiles; ++id )
            {
                int x = nOffsetX + nScrollOSx + (id % m_nNumCols * m_nCellSize);
                int y = nOffsetY + nScrollOSy + (id / m_nNumCols * m_nCellSize);
                if (x < m_nPanelWidth && y <m_nPanelHeight)
                {
	                mTM.Draw(nTilesetImageID, 
	                    x, y,
	                    1.0f, 1.0f,
	                    m_tTilesetTiles[id].SourceRect,
	                    0, 0, 0.0f,
                        Color.White.ToArgb());
                    if (m_bShowFlags)
                    {
                        if (m_nCurrLayer == 1)
                            mD3d.DrawText(m_tTilesetTiles[id].NTileFlag.ToString(), x + 2, y + 2, 0, 200, 0);
                        else if (m_nCurrLayer == 2)
                            mD3d.DrawText(m_tTilesetTiles[id].NTileFlag.ToString(), x + m_nCellSize - 10, y + 2, 255, 0, 0);
                        else
                            mD3d.DrawText(m_tTilesetTiles[id].NTileFlag.ToString(), x + m_nCellSize / 2 - 3, y + 2, 0, 0, 255);
                    }
                }
            }
        }
    }
}