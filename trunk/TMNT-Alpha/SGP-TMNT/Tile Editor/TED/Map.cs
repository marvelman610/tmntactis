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

namespace map
{
    enum LAYER { LAYER_ONE, LAYER_TWO, LAYER_FREE, };
    enum LAYER_MODE { SHOW_L1, SHOW_L2, SHOW_FREE, SHOW_BOTH, SHOW_ALL };
    /// <summary>
    /// 
    /// </summary>
    public class CMap
    {
        const int MAX_NUM_FREETILES = 500;
        const int MAX_NUM_TILESETS = 4;
        // clear layer strings
        string[] m_strClearLayerText = new string[3];

        // the tiles that have been put onto the map
        bool[] m_bContainsTile = new bool[3];
        public bool[] ContainsTile
        {
            get { return m_bContainsTile; }
            set { m_bContainsTile = value; }
        }
        CTILE[] m_tMapTilesLayer1;
        public CTILE[] TMapTiles
        {
            get { return m_tMapTilesLayer1; }
            set { m_tMapTilesLayer1 = value; }
        }
        CTILE[] m_tMapTilesLayer2;
        public CTILE[] TMapTilesLayer2
        {
            get { return m_tMapTilesLayer2; }
            set { m_tMapTilesLayer2 = value; }
        }
        CFREETILE[] m_tFreePlaced = new CFREETILE[MAX_NUM_FREETILES];
        public CFREETILE[] FreePlaced
        {
            get { return m_tFreePlaced; }
            set { m_tFreePlaced = value; }
        }
        int m_nCurrLayer;
        public int NCurrLayer
        {
            get { return m_nCurrLayer; }
            set { m_nCurrLayer = value; }
        }
        int m_nLayerMode;
        public int NLayerMode
        {
            get { return m_nLayerMode; }
            set { m_nLayerMode = value; }
        }
        // the map grid
        CGrid m_gMapGrid;
        public CGrid GMapGrid
        {
            get { return m_gMapGrid; }
            set { m_gMapGrid = value; }
        }

        // each individual cell (always multiples of 2)
        int m_nCellSize;
        public int NCellSize
        {
            get { return m_nCellSize; }
            set { m_nCellSize = value; }
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

        // dimensions
        bool m_bIsIsometric;
        public bool BIsIsometric
        {
            get { return m_bIsIsometric; }
            set { m_bIsIsometric = value; }
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
        int m_nMapWidth;
        public int NMapWidth
        {
            get { return m_nMapWidth; }
            set { m_nMapWidth = value; }
        }
        int m_nMapHeight;
        public int NMapHeight
        {
            get { return m_nMapHeight; }
            set { m_nMapHeight = value; }
        }
        // tileset image ID for texture manager
//         int[] m_nTilesetImageID = new int[MAX_NUM_TILESETS];
//         public int NTilesetImageID
//         {
//             get { return m_nTilesetImageID; }
//             set { m_nTilesetImageID = value; }
//         }
        int m_nPanelWidth;
        public int NPanelWidth
        {
            get { return m_nPanelWidth; }
            set { m_nPanelWidth = value; }
        }
        int m_nPanelHeight;
        public int NPanelHeight
        {
            get { return m_nPanelHeight; }
            set { m_nPanelHeight = value; }
        }
        // account for scrolling
        int nScrollOSx = 0;

        int nScrollOSy = 0;
        // key color
        Color m_clrTilesetKey;
        public System.Drawing.Color ClrTilesetKey
        {
            get { return m_clrTilesetKey; }
            set { m_clrTilesetKey = value; }
        }
        // zooming
        float m_nZoom;
        public float NZoom
        {
            get { return m_nZoom; }
            set { m_nZoom = value; }
        }
        int nZoomIncrement;

        bool m_bShowFlags = true;
        public bool BShowFlags
        {
            get { return m_bShowFlags; }
            set { m_bShowFlags = value; }
        }
        ManagedTextureManager mTM;
        ManagedDirect3D mD3d;

        public CMap(int mapWidth, int mapHeight, int cellSize, int numCols, int numRows, Color key, int zoom, int zoomIncrement)
        {
            m_nCurrLayer = (int)LAYER.LAYER_ONE;
            m_nLayerMode = (int)LAYER_MODE.SHOW_L1;
            mTM = ManagedTextureManager.Instance;
            mD3d = ManagedDirect3D.Instance;
            m_nMapWidth = mapWidth;
            m_nMapHeight = mapHeight;
            m_nCellSize = cellSize;
            m_nCellWidth = cellSize;
            m_nCellHeight = cellSize;
            m_nNumCols = numCols;
            m_nNumRows = numRows;
            m_clrTilesetKey = key;
            m_nZoom = zoom;
            nZoomIncrement = zoomIncrement;
            m_nTotalNumTiles = numCols * numRows;
            m_bIsIsometric = false;

            m_gMapGrid = new CGrid(m_nCellSize, m_nCellSize, m_nNumRows, m_nNumCols, 0, nZoomIncrement, 0, 0, false);
            SetClearStrings();
            NewMapTileArray();
        }

        public CMap(int cellSize, int numCols, int numRows, int zoomIncrement, bool bisometric)
        {
            m_nCurrLayer = (int)LAYER.LAYER_ONE;
            m_nLayerMode = (int)LAYER_MODE.SHOW_L1;
            mTM = ManagedTextureManager.Instance;
            mD3d = ManagedDirect3D.Instance;
            m_nMapWidth = cellSize * numCols;
            m_nMapHeight = cellSize * numRows;
            m_nCellSize = cellSize;
            m_nCellWidth = cellSize;
            m_nCellHeight = cellSize;
            m_nNumCols = numCols;
            m_nNumRows = numRows;
            m_nZoom = 1.0f;
            nZoomIncrement = zoomIncrement;
            m_nTotalNumTiles = numCols * numRows;
            m_bIsIsometric = bisometric;

            m_gMapGrid = new CGrid(m_nCellSize, m_nCellSize, m_nNumRows, m_nNumCols, 0, nZoomIncrement, 0, 0, false);
            SetClearStrings();
            NewMapTileArray();
        }
        // isometric Map constructor
        public CMap(int isoWidth, int isoHeight, int numCols, int numRows, int zoomIncrement, bool bisometric)
        {
            m_nCurrLayer = (int)LAYER.LAYER_ONE;
            m_nLayerMode = (int)LAYER_MODE.SHOW_L1;
            mTM = ManagedTextureManager.Instance;
            mD3d = ManagedDirect3D.Instance;
            m_nMapWidth = isoWidth * numCols;
            m_nMapHeight = isoHeight * numRows;

            // TODO:: how to do zoom with iso?
/*            nCellSize = cellSize;*/
            m_nCellWidth = isoWidth;
            m_nCellHeight = isoHeight;

            m_nNumCols = numCols;
            m_nNumRows = numRows;
            m_nZoom = 1.0f;
            nZoomIncrement = zoomIncrement;
            m_nTotalNumTiles = numCols * numRows;
            m_bIsIsometric = bisometric;

            m_gMapGrid = new CGrid(isoWidth, isoHeight, m_nNumRows, m_nNumCols, 0, nZoomIncrement, 0, 0, true);
            SetClearStrings();
            NewMapTileArray();
        }
        public void SetClearStrings()
        {
            m_strClearLayerText[0] = "Are you sure you want to clear all tiles in Layer 1?";
            m_strClearLayerText[1] = "Are you sure you want to clear all tiles in Layer 2?";
            m_strClearLayerText[2] = "Are you sure you want to clear all tiles in Free Place Layer?";
        }
        public void AddFreePlacedTile(CTILE tile, int posX, int posY)
        {
            int count = 0;
            for (int i = 0; i < MAX_NUM_FREETILES; ++i )
            {
                CFREETILE tNewFree = new CFREETILE(posX, posY, tile.SourceRect, tile.NTileFlag, tile.ImageID);

                // sort the tiles by their y coordinate, so that when rendered,
                // they will render top to bottom (lowest y first)

                // TODO:: write recursive sort, start at LAST not-null index and decrement
                //          pushing each tile up one in the array until the Y coord of the new
                //          tile is greater than the next (next=preceding tile)
//                 while (m_tFreePlaced[count] != null)
//                 {
//                     if (tNewFree.MapPt.Y < m_tFreePlaced[count].MapPt.Y)
//                     {
//                         CFREETILE tempFree = m_tFreePlaced[count];
//                         m_tFreePlaced[count] = tNewFree;
//                         m_tFreePlaced[i+1] = tempFree;
//                         m_bContainsTile[m_nCurrLayer] = true;
//                         return;
//                     }
//                     ++count;
//                 }
                if (m_tFreePlaced[i] == null)
                {
                    m_tFreePlaced[i] = tNewFree;
                    m_bContainsTile[m_nCurrLayer] = true;
                    return;
                }
            }
        }
        private void SortFreePlaced()
        {

        }
        public void AddMarqueeFreePlaceTile(CTILE tile, int posX, int posY, Rectangle srcRect)
        {
            for (int i = 0; i < MAX_NUM_FREETILES; ++i)
            {
                if (m_tFreePlaced[i] == null)
                {
                    CFREETILE tNewFree = new CFREETILE(posX, posY, srcRect, tile.NTileFlag, tile.ImageID);
                    m_tFreePlaced[i] = tNewFree;
                    m_bContainsTile[m_nCurrLayer] = true;
                    return;
                }
            }
        }
        public void AddTile(CTILE tile, int tileID)
        {
            CTILE addTile = new CTILE();
            addTile = tile;
            if (tileID < m_nTotalNumTiles)
            {
                if (m_nCurrLayer == (int)LAYER.LAYER_ONE)
                {
                    m_tMapTilesLayer1[tileID] = addTile;
                    m_bContainsTile[(int)LAYER.LAYER_ONE] = true;
                }
                else
                {
                    m_tMapTilesLayer2[tileID] = addTile;
                    m_bContainsTile[(int)LAYER.LAYER_TWO] = true;
                }
            }
        }
        public void RemoveFreeTile(int index)
        {
            m_tFreePlaced[index] = null;
            m_bContainsTile[m_nCurrLayer] = false;
        }

        public void NewMapTileArray()
        {
            int i = 0;
            // if we already a first (and second) layer
            if (m_tMapTilesLayer1 != null)
            {
                //////////////////////////////////////////////////////////////////////////
                // First layer
                CTILE[] tempIDs = m_tMapTilesLayer1;
                m_tMapTilesLayer1 = new CTILE[m_nTotalNumTiles];
                int lengthNew = m_tMapTilesLayer1.GetLength(0);
                int lengthOld = tempIDs.GetLength(0);
                if (lengthNew > lengthOld)
                {
                    for (i = lengthOld; i < lengthNew; ++i)
                        m_tMapTilesLayer1[i] = new CTILE();
                    for (i = 0; i < lengthOld; ++i)
                        m_tMapTilesLayer1[i] = tempIDs[i];
                }
                else
                {
                    int length = m_tMapTilesLayer1.GetLength(0);
                    for (i = 0; i < lengthNew; ++i)
                        m_tMapTilesLayer1[i] = new CTILE();
                    for (i = 0; i < m_nTotalNumTiles; ++i)
                        m_tMapTilesLayer1[i] = tempIDs[i];
                }
                //////////////////////////////////////////////////////////////////////////
                // Second layer
                CTILE[] tempIDsLayer2 = m_tMapTilesLayer2;
                m_tMapTilesLayer2 = new CTILE[m_nTotalNumTiles];
                lengthNew = m_tMapTilesLayer2.GetLength(0);
                lengthOld = tempIDsLayer2.GetLength(0);
                if (lengthNew > lengthOld)
                {
                    for (i = lengthOld; i < lengthNew; ++i)
                        m_tMapTilesLayer2[i] = new CTILE();
                    for (i = 0; i < lengthOld; ++i)
                        m_tMapTilesLayer2[i] = tempIDsLayer2[i];
                }
                else
                {
                    for (i = 0; i < lengthNew; ++i)
                        m_tMapTilesLayer2[i] = new CTILE();
                    for (i = 0; i < m_nTotalNumTiles; ++i)
                        m_tMapTilesLayer2[i] = tempIDsLayer2[i];
                }
            }
            else
            {
                // First layer
                m_tMapTilesLayer1 = new CTILE[m_nTotalNumTiles];
                for ( i = 0; i < m_nTotalNumTiles; ++i)
                    m_tMapTilesLayer1[i] = new CTILE();
                // Second layer
                m_tMapTilesLayer2 = new CTILE[m_nTotalNumTiles];
                for (i = 0; i < m_nTotalNumTiles; ++i)
                    m_tMapTilesLayer2[i] = new CTILE();
            }
        }
        public void DrawMap()
        {
            int x = 0, y = 0, id = 0;
            // scale the image with the zoom
            float scale = m_nZoom;

            switch (m_nLayerMode)
            {
                case (int)LAYER_MODE.SHOW_L1:
                    for (; id < m_nTotalNumTiles; ++id)
                    {
                        x = id % m_nNumCols * m_nCellSize + nScrollOSx;
                        y = id / m_nNumCols * m_nCellSize + nScrollOSy;

                        if (m_tMapTilesLayer1[id].NSourceID != -1 && x < m_nPanelWidth && y < m_nPanelHeight)
                        {
                            mTM.Draw(m_tMapTilesLayer1[id].ImageID, x, y,
                                 scale, scale,
                                 m_tMapTilesLayer1[id].SourceRect, 0, 0, 0.0f, m_clrTilesetKey.ToArgb());
                            if (m_bShowFlags)
                                mD3d.DrawText(m_tMapTilesLayer1[id].NTileFlag.ToString(), x + 2, y + 2, 0, 255, 0);
                        }
                    }
                    break;
                case (int)LAYER_MODE.SHOW_L2:
                    for (; id < m_nTotalNumTiles; ++id)
                    {
                        x = id % m_nNumCols * m_nCellSize + nScrollOSy;
                        y = id / m_nNumCols * m_nCellSize + nScrollOSx;

                        if (m_tMapTilesLayer2[id].NSourceID != -1 && x < m_nPanelWidth && y < m_nPanelHeight)
                        {
                            mTM.Draw(m_tMapTilesLayer2[id].ImageID, x, y,
                                 scale, scale,
                                 m_tMapTilesLayer2[id].SourceRect, 0, 0, 0.0f, m_clrTilesetKey.ToArgb());
                            if (m_bShowFlags)
                                mD3d.DrawText(m_tMapTilesLayer2[id].NTileFlag.ToString(), x + m_nCellSize - 10, y + 5, 255, 0, 0);
                        }
                    }
                    break;
                case (int)LAYER_MODE.SHOW_BOTH:
                    for (; id < m_nTotalNumTiles; ++id)
                    {
                        x = id % m_nNumCols * m_nCellSize + nScrollOSx;
                        y = id / m_nNumCols * m_nCellSize + nScrollOSy;

                        if (m_tMapTilesLayer1[id].NSourceID != -1 && x < m_nPanelWidth && y < m_nPanelHeight)
                        {
                            mTM.Draw(m_tMapTilesLayer1[id].ImageID, x, y,
                                 scale, scale,
                                 m_tMapTilesLayer1[id].SourceRect, 0, 0, 0.0f, m_clrTilesetKey.ToArgb());
                            if (m_bShowFlags && m_tMapTilesLayer1[id].NSourceID != -1)
                                mD3d.DrawText(m_tMapTilesLayer1[id].NTileFlag.ToString(), x + 2, y + 2, 0, 255, 0);
                        }
                        if (m_tMapTilesLayer2[id].NSourceID != -1 && x < m_nPanelWidth && y < m_nPanelHeight)
                        {
                            mTM.Draw(m_tMapTilesLayer2[id].ImageID, x, y,
                                 scale, scale,
                                 m_tMapTilesLayer2[id].SourceRect, 0, 0, 0.0f, m_clrTilesetKey.ToArgb());
                            if (m_bShowFlags)
                            {
                                if (m_tMapTilesLayer1[id].NSourceID != -1)
                                	mD3d.DrawText(m_tMapTilesLayer1[id].NTileFlag.ToString(), x + 2, y + 2, 0, 255, 0);
                                mD3d.DrawText(m_tMapTilesLayer2[id].NTileFlag.ToString(), x + m_nCellSize - 10, y + 2, 255, 0, 0);
                            }
                        }
                    }
                    break;
                case (int)LAYER_MODE.SHOW_ALL:
                    for (; id < m_nTotalNumTiles; ++id)
                    {
                        x = id % m_nNumCols * m_nCellSize + nScrollOSx;
                        y = id / m_nNumCols * m_nCellSize + nScrollOSy;

                        if (m_tMapTilesLayer1[id].NSourceID != -1 && x < m_nPanelWidth && y < m_nPanelHeight)
                        {
                            mTM.Draw(m_tMapTilesLayer1[id].ImageID, x, y,
                                 scale, scale,
                                 m_tMapTilesLayer1[id].SourceRect, 0, 0, 0.0f, m_clrTilesetKey.ToArgb());
                            if (m_bShowFlags && m_tMapTilesLayer1[id].NSourceID != -1)
                                mD3d.DrawText(m_tMapTilesLayer1[id].NTileFlag.ToString(), x + 2, y + 2, 0, 255, 0);
                        }
                        if (m_tMapTilesLayer2[id].NSourceID != -1 && x < m_nPanelWidth && y < m_nPanelHeight)
                        {
                            mTM.Draw(m_tMapTilesLayer2[id].ImageID, x, y,
                                 scale, scale,
                                 m_tMapTilesLayer2[id].SourceRect, 0, 0, 0.0f, m_clrTilesetKey.ToArgb());
                            if (m_bShowFlags)
                            {
                                if (m_tMapTilesLayer1[id].NSourceID != -1)
                                    mD3d.DrawText(m_tMapTilesLayer1[id].NTileFlag.ToString(), x + 2, y + 2, 0, 255, 0);
                                mD3d.DrawText(m_tMapTilesLayer2[id].NTileFlag.ToString(), x + m_nCellSize - 10, y + 2, 255, 0, 0);
                            }
                        }
                    }
                    for (int i = 0; i < MAX_NUM_FREETILES; ++i)
                    {
                        if (m_tFreePlaced[i] != null)
                        {
                            mTM.Draw(m_tFreePlaced[i].ImageID, m_tFreePlaced[i].MapPt.X, m_tFreePlaced[i].MapPt.Y, 1.0f, 1.0f, m_tFreePlaced[i].SourceRect, 0, 0, 0.0f, m_clrTilesetKey.ToArgb());
                            if (m_bShowFlags) 
                                mD3d.DrawText(m_tFreePlaced[i].NTileFlag.ToString(), m_tFreePlaced[i].MapPt.X + 1, m_tFreePlaced[i].MapPt.Y, 0, 0, 255);
                        }
                    }
                    break;
                case (int)LAYER_MODE.SHOW_FREE:
                    for (int i = 0; i < MAX_NUM_FREETILES; ++i)
                    {
                        if (m_tFreePlaced[i] != null)
                        {
                            mTM.Draw(m_tFreePlaced[i].ImageID, m_tFreePlaced[i].MapPt.X, m_tFreePlaced[i].MapPt.Y, 1.0f, 1.0f, m_tFreePlaced[i].SourceRect, 0, 0, 0.0f, m_clrTilesetKey.ToArgb());
                            if (m_bShowFlags) 
                                mD3d.DrawText(m_tFreePlaced[i].NTileFlag.ToString(), m_tFreePlaced[i].MapPt.X + 1, m_tFreePlaced[i].MapPt.Y, 0, 0, 255);
                        }
                    }
                    break;
            }
        }
        Point IsoTilePlot(Point pt, int xOffset)
        {
            Point newPt = new Point();
            newPt.X = (pt.X - pt.Y) * (m_nCellWidth / 2) + xOffset + nScrollOSx;
            newPt.Y = (pt.X + pt.Y) * (m_nCellHeight / 2) + nScrollOSy;
            return newPt;
        }
        public void DrawMapIso()
        {
            int x = 0, y = 0, id = 0;

            // scale the image with the zoom
            float scale = m_nZoom;
            int xOffset = m_gMapGrid.NIsoCenterTopX - (m_nCellWidth >> 1);

            switch (m_nLayerMode)
            {
                case (int)LAYER_MODE.SHOW_L1:
                    for (; y < m_nNumRows; ++y)
                    {
                        for (x = 0; x < m_nNumCols; ++x )
                        {
                            Point mapPt = new Point(x, y);
                            mapPt = IsoTilePlot(mapPt, xOffset);
                            id = y * m_nNumCols + x;

                            if (m_tMapTilesLayer1[id].NSourceID != -1 && mapPt.X < m_nPanelWidth && mapPt.Y < m_nPanelHeight)
                            {
                                mTM.Draw(m_tMapTilesLayer1[id].ImageID, mapPt.X, mapPt.Y,
                                     scale, scale,
                                     m_tMapTilesLayer1[id].SourceRect, 0, 0, 0.0f, m_clrTilesetKey.ToArgb());
                                if (m_bShowFlags)
                                    mD3d.DrawText(m_tMapTilesLayer1[id].NTileFlag.ToString(), mapPt.X + 14, mapPt.Y + 8, 0, 255, 0);
                            }
                        }
                    }
                    break;
                case (int)LAYER_MODE.SHOW_L2:
                    for (; y < m_nNumRows; ++y)
                    {
                        for (x = 0; x < m_nNumCols; ++x)
                        {
                            Point mapPt = new Point(x, y);
                            mapPt = IsoTilePlot(mapPt, xOffset);
                            id = y * m_nNumCols + x;

                            if (m_tMapTilesLayer2[id].NSourceID != -1 && mapPt.X < m_nPanelWidth && mapPt.Y < m_nPanelHeight)
                            {
                                mTM.Draw(m_tMapTilesLayer2[id].ImageID, mapPt.X, mapPt.Y,
                                     scale, scale,
                                     m_tMapTilesLayer2[id].SourceRect, 0, 0, 0.0f, m_clrTilesetKey.ToArgb());
                                if (m_bShowFlags)
                                    mD3d.DrawText(m_tMapTilesLayer2[id].NTileFlag.ToString(), mapPt.X + m_nCellWidth - 20, mapPt.Y + 8, 255, 0, 0);
                            }
                        }
                    }
                    break;
                case (int)LAYER_MODE.SHOW_BOTH:
                    for (; y < m_nNumRows; ++y)
                    {
                        for (x = 0; x < m_nNumCols; ++x)
                        {
                            Point mapPt = new Point(x, y);
                            mapPt = IsoTilePlot(mapPt, xOffset);
                            id = y * m_nNumCols + x;

                            if (m_tMapTilesLayer1[id].NSourceID != -1 && mapPt.X < m_nPanelWidth && mapPt.Y < m_nPanelHeight)
                            {
                                mTM.Draw(m_tMapTilesLayer1[id].ImageID, mapPt.X, mapPt.Y,
                                     scale, scale,
                                     m_tMapTilesLayer1[id].SourceRect, 0, 0, 0.0f, m_clrTilesetKey.ToArgb());
                                if (m_bShowFlags && m_tMapTilesLayer1[id].NSourceID != -1)
                                    mD3d.DrawText(m_tMapTilesLayer1[id].NTileFlag.ToString(), mapPt.X + 14, mapPt.Y + 8, 0, 255, 0);
                            }
                            if (m_tMapTilesLayer2[id].NSourceID != -1 && mapPt.X < m_nPanelWidth && mapPt.Y < m_nPanelHeight)
                            {
                                mTM.Draw(m_tMapTilesLayer2[id].ImageID, mapPt.X, mapPt.Y,
                                     scale, scale,
                                     m_tMapTilesLayer2[id].SourceRect, 0, 0, 0.0f, m_clrTilesetKey.ToArgb());
                                if (m_bShowFlags)
                                {
                                    if (m_tMapTilesLayer1[id].NSourceID != -1)
                                        mD3d.DrawText(m_tMapTilesLayer1[id].NTileFlag.ToString(), mapPt.X + 14, mapPt.Y + 8, 0, 255, 0);
                                    mD3d.DrawText(m_tMapTilesLayer2[id].NTileFlag.ToString(), mapPt.X + m_nCellWidth - 20, mapPt.Y + 8, 255, 0, 0);
                                }
                            }
                        }
                    }
                    break;
                case (int)LAYER_MODE.SHOW_ALL:
                    for (; y < m_nNumRows; ++y)
                    {
                        for (x = 0; x < m_nNumCols; ++x)
                        {
                            Point mapPt = new Point(x, y);
                            mapPt = IsoTilePlot(mapPt, xOffset);
                            id = y * m_nNumCols + x;

                            if (m_tMapTilesLayer1[id].NSourceID != -1 && mapPt.X < m_nPanelWidth && mapPt.Y < m_nPanelHeight)
                            {
                                mTM.Draw(m_tMapTilesLayer1[id].ImageID, mapPt.X, mapPt.Y,
                                     scale, scale,
                                     m_tMapTilesLayer1[id].SourceRect, 0, 0, 0.0f, m_clrTilesetKey.ToArgb());
                                if (m_bShowFlags && m_tMapTilesLayer1[id].NSourceID != -1)
                                    mD3d.DrawText(m_tMapTilesLayer1[id].NTileFlag.ToString(), mapPt.X + 14, mapPt.Y + 8, 0, 255, 0);
                            }
                            if (m_tMapTilesLayer2[id].NSourceID != -1 && mapPt.X < m_nPanelWidth && mapPt.Y < m_nPanelHeight)
                            {
                                mTM.Draw(m_tMapTilesLayer2[id].ImageID, mapPt.X, mapPt.Y,
                                     scale, scale,
                                     m_tMapTilesLayer2[id].SourceRect, 0, 0, 0.0f, m_clrTilesetKey.ToArgb());
                                if (m_bShowFlags)
                                {
                                    if (m_tMapTilesLayer1[id].NSourceID != -1)
                                        mD3d.DrawText(m_tMapTilesLayer1[id].NTileFlag.ToString(), mapPt.X + 14, mapPt.Y + 8, 0, 255, 0);
                                    mD3d.DrawText(m_tMapTilesLayer2[id].NTileFlag.ToString(), mapPt.X + m_nCellWidth - 20, mapPt.Y + 8, 255, 0, 0);
                                }
                            }
                        }
                    }
                    for (int i = 0; i < MAX_NUM_FREETILES; ++i)
                    {
                        if (m_tFreePlaced[i] != null)
                        {
                            mTM.Draw(m_tFreePlaced[i].ImageID, m_tFreePlaced[i].MapPt.X, m_tFreePlaced[i].MapPt.Y, 1.0f, 1.0f, m_tFreePlaced[i].SourceRect, 0, 0, 0.0f, m_clrTilesetKey.ToArgb());
                            if (m_bShowFlags)
	                            mD3d.DrawText(m_tFreePlaced[i].NTileFlag.ToString(), m_tFreePlaced[i].MapPt.X + 1, m_tFreePlaced[i].MapPt.Y, 0, 0, 255);
                        }
                    }
                    break;
                case (int)LAYER_MODE.SHOW_FREE:
                    for (int i = 0; i < MAX_NUM_FREETILES; ++i)
                    {
                        if (m_tFreePlaced[i] != null)
                        {
                            mTM.Draw(m_tFreePlaced[i].ImageID, m_tFreePlaced[i].MapPt.X, m_tFreePlaced[i].MapPt.Y, 1.0f, 1.0f, m_tFreePlaced[i].SourceRect, 0, 0, 0.0f, m_clrTilesetKey.ToArgb());
                            if (m_bShowFlags) 
                                mD3d.DrawText(m_tFreePlaced[i].NTileFlag.ToString(), m_tFreePlaced[i].MapPt.X + 1, m_tFreePlaced[i].MapPt.Y, 0, 0, 255);
                        }
                    }
                    break;
            }
        }

        public void ClearMap()
        {
            if (DialogResult.Yes == MessageBox.Show (m_strClearLayerText[m_nCurrLayer], "Clear Map", MessageBoxButtons.YesNo))
            {
                if (m_nCurrLayer == (int)LAYER.LAYER_ONE)
                {
                    for (int id = 0; id < m_nTotalNumTiles; ++id)
                        m_tMapTilesLayer1[id].NSourceID = -1;
                    m_bContainsTile[m_nCurrLayer] = false;
                }
                else if (m_nCurrLayer == (int)LAYER.LAYER_TWO)
                {
                    for (int id = 0; id < m_nTotalNumTiles; ++id)
                        m_tMapTilesLayer2[id].NSourceID = -1;
                    m_bContainsTile[m_nCurrLayer] = false;
                }
                else
                {
                    for (int id = 0; id < m_tFreePlaced.GetLength(0); ++id)
                        RemoveFreeTile(id);
                    m_bContainsTile[(int)LAYER.LAYER_FREE] = false;
                }
            }
        }
        public void OffsetImage(int x, int y)
        {
            nScrollOSx += x;
            nScrollOSy += y;
        }
        public void ZoomAdjust(float zoom)
        {
            m_nZoom = zoom;
            if (zoom == 1.0f)
                m_nCellSize = 32;
            else
                m_nCellSize = (int)(32.0f + (-(1.0f - zoom) * 32.0f));
        }
        public void CellSizeAdjust(int cellSize)
        {
            m_nCellSize = cellSize;
        }
    }
}
