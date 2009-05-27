using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.IO;
using System.Windows.Forms;
using Microsoft.DirectX;
using Microsoft.DirectX.Direct3D;
using SGD;
using tile;
using map;
using ColorPicker;

namespace test
{
    enum LAYER {LAYER_ONE, LAYER_TWO, LAYER_FREE, };
    enum LAYER_MODE {SHOW_L1, SHOW_L2, SHOW_FREE, SHOW_BOTH, SHOW_ALL};
    enum SECTION {TOPLEFT, TOPRIGHT, BOTTOMLEFT, BOTTOMRIGHT};
    enum SAVE_TYPE {TEXT, BINARY, XML};
    /// <summary>
    /// The main form.
    /// Saving and loading done in a separate IO.cs partial class
    /// </summary>
    public partial class Form1 : Form
    {
    #region DECLS 1
        const string m_strVersionNumber = "TED-Version-1.0";
        const int MAX_NUM_TILESETS = 4;
        // dialogs
        Help m_helpDlg;
        ImportTilesetDlg m_impTsDlg;
        SetTileFlagsDlg m_stfDlg;
        NewMap m_newMapDlg;
        SetTriggerDlg m_stDlg;

        // d3d and texture manager singletons
        ManagedDirect3D m_mD3d;
        ManagedTextureManager m_mTM;
        // the map
        CMap m_mMap;
        bool m_bChangesMade = false;
        Point m_ptMapMoveOriginal;
        Point m_ptMapMoveEnd;

        // array of tilesets
        CTileset[] m_tsTileset = new CTileset[MAX_NUM_TILESETS];
        // array of components that get added to each newly added tileset tab
        TilesetComponents[] m_tsComponents = new TilesetComponents[MAX_NUM_TILESETS];

        // which tileset are we currently looking at? based on the tabPage indexes
        int m_nCurrTilesetIndex = 0; // index into the array of tilesets, from 0-3
        bool m_bAddTileset = false;
        bool m_bJustClick = false;
        string[] m_strTilesetFilenames = new string[MAX_NUM_TILESETS];
        string[] m_strAutoSaveTilesetFilenames = new string[MAX_NUM_TILESETS];
        int[] m_nCurrImageID = new int[MAX_NUM_TILESETS];

        // the currently selected tile
        CTILE    m_tCurrTile;
        // free placing tile variables
        bool m_bFreePlace = false;
        bool m_bDeleting = false;
        float m_fFreePlaceImageRotX = 0.0f;
        float m_fFreePlaceImageRotY = 0.0f;
        float m_fRotation = 0.0f;
        Point m_ptMouseFreePlacePos;

        // Isometric?
        bool m_bIsIsometric;
        // Four tile flags
        string m_str0 = "0=None";
        string m_str1 = "1=Collision";
        string m_str2 = "2=Not Set";
        string m_str3 = "3=Not Set";
        int m_nCurrTileFlag = 0;
        
        /// <summary>
        /// when shift+right-click, which tile are we clicking?
        /// </summary>
        int m_nCurrTileEditID = -1;
        
        // default filename string
        string m_strDefFileName = Path.GetFullPath(Properties.Resources.defaultTilesetFilename);
        string m_strSaveFileName = "MyMap";
        int m_nLastSaveType = (int)SAVE_TYPE.XML;
        bool m_bHaveSaved = false;

        // timer for drawing delay
        Timer m_timer;
        bool m_bDontDraw = false;

        bool m_bIsGridOn = true;
        bool m_bIsDragging = false;
        //////////////////////////////
        // marquee variables
        bool m_bMarqueeSelect = false;
        bool m_bDrawMarquee = false;
        bool m_bSingleSelect = true;
        bool m_bIsDraggingMarquee = false;
        int m_nMarqueeNumCols = 0;
        Point m_ptTopLeft = new Point(-1, -1);
        Point m_ptBottomRight = new Point(-1, -1);
        Point m_ptDrawTL;
        Point m_ptDrawBR;
        CTILE[] m_tMarqueeTiles;
        //////////////////////////////
        int m_nZoomIncrement = 8;
        int m_nTilesetXoffset = 10;
        int m_nTilesetYoffset = 140;
        int m_nXScrollOffset;
        int m_nYScrollOffset;
        string m_strSelectedID = "Tile ID = ";
        Color[] m_clrKey = new Color[MAX_NUM_TILESETS];
        Color m_clrMapBGclear = Color.Gray;
        Color m_clrTilesetBGClear = Color.White;

        // Layer properties
        int m_nCurrLayer = (int)LAYER.LAYER_ONE;
        int m_nLayerMode = (int)LAYER_MODE.SHOW_L1;
        string m_strLayerLabel = "Current Layer : ";
        bool m_bIsTwoLayersOn = true;
    #endregion

        public Form1()
        {
            InitializeComponent();
            this.Text += " - " + m_strSaveFileName;
            //splitContainer1.Focus();
            //////////////////////////////////////////////////////////////////////////
            // set up the timer 
            m_timer = new Timer();
            m_timer.Enabled = true;
            m_timer.Interval = 2;
            m_timer.Start();
            m_timer.Tick += new EventHandler(timer_Tick);
            //////////////////////////////////////////////////////////////////////////

            //////////////////////////////////////////////////////////////////////////
            // set up the texture mngr & d3d device
            m_mD3d = ManagedDirect3D.Instance;
            m_mD3d.InitManagedDirect3D(this.splitContainer1.Panel2, this.splitContainer1.Panel2.Width,
                                     this.splitContainer1.Panel2.Height, true, false);
            m_mTM = ManagedTextureManager.Instance;
            m_mTM.InitManagedTextureManager(m_mD3d.Device, m_mD3d.Sprite);
            //////////////////////////////////////////////////////////////////////////
            //Set up input boxes

            cbLayerMode.SelectedIndex = 0;
            cbLayer.SelectedIndex = 0;

            cbShowFlags.Checked = true;
            cbShowGrid.Checked = true;
            //////////////////////////////////////////////////////////////////////////
            lbCurrentLayer.Text = m_strLayerLabel + "Layer 1";
            splitContainer1.Panel2.AutoScrollMinSize = new Size((int)nudMapCellSize.Value * (int)nudMapNumCols.Value, (int)nudMapCellSize.Value * (int)nudMapNumRows.Value);
            LoadDefault();
        }

        void timer_Tick(object sender, EventArgs e)
        {
            if (!m_bDontDraw)
            {
	            PanelOnePaint();
	            PanelTwoPaint();
                m_bDontDraw = true;
            }
        }

        private void PanelTwoPaint()
        {
            m_mD3d.ChangeDisplayParam(splitContainer1.Panel2, false);
            m_mD3d.DeviceBegin();
            m_mD3d.Clear(m_clrMapBGclear.R, m_clrMapBGclear.G, m_clrMapBGclear.B);

            if (m_mMap != null)
            {
	            if (m_tsTileset != null)
	            {
	                m_mD3d.SpriteBegin();
                    if (!m_mMap.BIsIsometric)
                    {
                        m_mMap.DrawMap();
                    }
                    else
                        m_mMap.DrawMapIso();
                    if (!m_bDeleting)       // don't draw the free place image if we're deleting, it gets in the way
                    {
	                    if (m_bFreePlace && !m_bDrawMarquee)
	                    {
	                        m_mTM.Draw(m_tCurrTile.ImageID, m_ptMouseFreePlacePos.X, m_ptMouseFreePlacePos.Y, 1.0f, 1.0f, m_tCurrTile.SourceRect,
                                0, 0, m_fRotation, m_clrKey[m_nCurrTilesetIndex].ToArgb());
	                    }
	                    else if (m_bFreePlace && m_bDrawMarquee)
	                    {
	                        Rectangle MarqueeSrcRect = new Rectangle(m_ptTopLeft.X, m_ptTopLeft.Y, m_ptBottomRight.X - m_ptTopLeft.X, m_ptBottomRight.Y - m_ptTopLeft.Y);
                            m_mTM.Draw(m_tCurrTile.ImageID, m_ptMouseFreePlacePos.X, m_ptMouseFreePlacePos.Y, 1.0f, 1.0f, MarqueeSrcRect,
                                0, 0, m_fRotation, m_clrKey[m_nCurrTilesetIndex].ToArgb());
	                    }
                    }

	                m_mD3d.SpriteEnd();
	            }
	            if (m_bIsGridOn)
	            {
	                m_mD3d.LineBegin();
	                m_mMap.GMapGrid.DrawGrid();
	                m_mD3d.LineEnd();
	            }
            }

            m_mD3d.DeviceEnd();
            m_mD3d.Present();
        }

        private void PanelOnePaint()
        {
            if (m_nCurrTilesetIndex == -1)
            {
                m_mD3d.ChangeDisplayParam(splitContainer1.Panel1, false);
                m_mD3d.DeviceBegin();
                m_mD3d.Clear(m_clrTilesetBGClear.R, m_clrTilesetBGClear.G, m_clrTilesetBGClear.B);
                m_mD3d.DeviceEnd();
                m_mD3d.Present();
                return;
            }
            if(m_tsTileset[m_nCurrTilesetIndex] != null)
            {
                m_mD3d.ChangeDisplayParam(splitContainer1.Panel1, false);
                m_mD3d.DeviceBegin();
                m_mD3d.Clear(m_clrTilesetBGClear.R, m_clrTilesetBGClear.G, m_clrTilesetBGClear.B);

                m_mD3d.SpriteBegin();
                m_tsTileset[m_nCurrTilesetIndex].DrawTileset();
                m_mD3d.SpriteEnd();

                m_mD3d.LineBegin();
                m_tsTileset[m_nCurrTilesetIndex].DrawTSGrid();
// 
                if (!m_bDrawMarquee || (m_bDrawMarquee && !m_bIsDraggingMarquee && !m_bFreePlace))
                {
                    m_tsTileset[m_nCurrTilesetIndex].DrawSelectionRect();
                }
                // draws the free place marquee rect, also draws rect while user is dragging on tileset
                else if (m_bDrawMarquee && (m_bFreePlace || m_bIsDraggingMarquee))
                {
                    m_ptDrawTL = m_ptTopLeft;
                    m_ptDrawBR = m_ptBottomRight;
                    m_ptDrawTL.X += m_nTilesetXoffset + (splitContainer1.Panel1.AutoScrollPosition.X);
                    m_ptDrawTL.Y += m_nTilesetYoffset + (splitContainer1.Panel1.AutoScrollPosition.Y);
                    m_ptDrawBR.X += m_nTilesetXoffset + (splitContainer1.Panel1.AutoScrollPosition.X);
                    m_ptDrawBR.Y += m_nTilesetYoffset + (splitContainer1.Panel1.AutoScrollPosition.Y);
                    m_mD3d.DrawRectLine(m_ptDrawTL, m_ptDrawBR, 0, 0, 255);
                }
                m_mD3d.LineEnd();

                m_mD3d.DeviceEnd();
                m_mD3d.Present();
            }
        }

        private void LoadDefault()
        {
            int ID = m_mTM.LoadTexture(m_strDefFileName, Color.FromArgb(255, 255, 255).ToArgb());
            int imageWidth = m_mTM.GetTextureWidth(ID);
            int imageHeight= m_mTM.GetTextureHeight(ID);

            splitContainer1.Panel1.AutoScrollMinSize = new Size(imageWidth, imageHeight + m_nTilesetYoffset + 5);

            // set up default starting selected tile
            m_tCurrTile = new CTILE();
            m_tCurrTile.NSourceID = 0;
            m_tCurrTile.SourceRect = new Rectangle(0, 0, 63, 32);
            m_tCurrTile.NTileFlag = 0;
            m_tCurrTile.ImageID = ID;
            lblTileID.Text += ID.ToString();

            // set up default Map
            m_mMap = new CMap( 62, 30, (int)nudMapNumCols.Value, (int)nudMapNumRows.Value, m_nZoomIncrement, true);

            m_bIsIsometric = true;
            nudMapNumCols.Enabled = false;
            nudMapNumRows.Enabled = false;
            nudMapCellSize.Enabled = false;
            nudMapZoom.Enabled = false;
            m_mMap.NPanelWidth = splitContainer1.Panel2.Width;
            m_mMap.NPanelHeight = splitContainer1.Panel2.Height;
            m_nCurrImageID[m_nCurrTilesetIndex] = ID;
            m_clrKey[m_nCurrTilesetIndex] = Color.White;
            m_mMap.ClrTilesetKey = m_clrKey[m_nCurrTilesetIndex];

            m_strTilesetFilenames[m_nCurrTilesetIndex] = Path.GetFullPath(m_strDefFileName);

            AddTileset(ID, m_clrKey[m_nCurrTilesetIndex], imageWidth, imageHeight, 63, 32, m_strTilesetFilenames[m_nCurrTilesetIndex]);
        }

        private void AddTileset(int nImageID, Color clrKey, int nImageWidth, int nImageHeight, int nCellWidth, int nCellHeight, string strFilename)
        {
            string choppedFileName = Path.GetFileNameWithoutExtension(strFilename);
            if (choppedFileName.Length > 10)
            {
                choppedFileName = choppedFileName.Remove(10, choppedFileName.Length - 10);
                tabControl1.TabPages.Add(choppedFileName);
            }
            else
                tabControl1.TabPages.Add(choppedFileName);
            m_tsTileset[tabControl1.TabCount - 2] = new CTileset(nImageID, clrKey, nImageWidth, nImageHeight, nCellWidth, nCellHeight, strFilename);
            m_nCurrTilesetIndex = tabControl1.TabCount - 2;

            m_tsTileset[m_nCurrTilesetIndex].NCurrSelectedTile = m_tCurrTile.NSourceID;
            m_tsTileset[m_nCurrTilesetIndex].NTilesetImageID = nImageID;

            m_tsTileset[m_nCurrTilesetIndex].SetSelectionRect();
            m_tsTileset[m_nCurrTilesetIndex].NPanelWidth = splitContainer1.Panel1.Width;
            m_tsTileset[m_nCurrTilesetIndex].NPanelHeight = splitContainer1.Panel1.Height;
            m_tsTileset[m_nCurrTilesetIndex].OffsetImage(m_nXScrollOffset, m_nYScrollOffset);
            m_tsTileset[m_nCurrTilesetIndex].GTilesetGrid.Offset(m_nXScrollOffset, m_nYScrollOffset);

            tbAnchorX.Text = m_tCurrTile.AnchorX.ToString();
            tbAnchorY.Text = m_tCurrTile.AnchorY.ToString();

            // set up the tileset's components
            m_tsComponents[m_nCurrTilesetIndex] = new TilesetComponents();
            m_tsComponents[m_nCurrTilesetIndex].AddAll(tabControl1.TabPages[tabControl1.TabCount-1], this.toolTip1, this);

            // add default flags to cbTileFlags
            m_tsComponents[m_nCurrTilesetIndex].cbTileFlag.Items.Add(m_str0);
            m_tsComponents[m_nCurrTilesetIndex].cbTileFlag.Items.Add(m_str1);
            m_tsComponents[m_nCurrTilesetIndex].cbTileFlag.Items.Add(m_str2);
            m_tsComponents[m_nCurrTilesetIndex].cbTileFlag.Items.Add(m_str3);
            m_tsComponents[m_nCurrTilesetIndex].cbTileFlag.SelectedIndex = 0;

            m_tsComponents[m_nCurrTilesetIndex].nudImageWidth.Value = nImageWidth;
            m_tsComponents[m_nCurrTilesetIndex].nudImageHeight.Value = nImageHeight;
            m_tsComponents[m_nCurrTilesetIndex].nudCellWidth.Value = nCellWidth;
            m_tsComponents[m_nCurrTilesetIndex].nudCellHeight.Value = nCellHeight;
            m_tsComponents[m_nCurrTilesetIndex].nudTilesetGridWidth.Value = nImageWidth / nCellWidth;
            m_tsComponents[m_nCurrTilesetIndex].nudTilesetGridHeight.Value = nImageHeight / nCellHeight;

            m_bJustClick = true;
            tabControl1.SelectedIndex = tabControl1.TabCount - 1;
            m_bAddTileset = false;
            m_bDrawMarquee = m_bMarqueeSelect = false;

            splitContainer1.Panel1.AutoScrollMinSize = new Size(nImageWidth, nImageHeight + m_nTilesetYoffset + 5);
        }

        private void nudMapNumCols_ValueChanged(object sender, EventArgs e)
        {
            m_mMap.GMapGrid.GridAdjust((int)nudMapNumCols.Value, (int)nudMapNumRows.Value);
            m_mMap.NTotalNumTiles = (int)nudMapNumCols.Value * (int)nudMapNumRows.Value;
            m_mMap.NewMapTileArray();
            splitContainer1.Panel2.AutoScrollMinSize = new Size(m_mMap.NMapWidth, m_mMap.NMapHeight);
        }

        private void nudMapNumRows_ValueChanged(object sender, EventArgs e)
        {
            nudMapNumCols_ValueChanged(sender, e);
        }

        private void nudGridZoom_ValueChanged(object sender, EventArgs e)
        {
            m_mMap.GMapGrid.ZoomAdjust((float)nudMapZoom.Value);
            m_mMap.ZoomAdjust((float)nudMapZoom.Value);
            splitContainer1.Panel2.AutoScrollMinSize = new Size((int)nudMapCellSize.Value * (int)nudMapNumCols.Value, (int)nudMapCellSize.Value * (int)nudMapNumRows.Value);
        }

        private void nudMapCellSize_ValueChanged(object sender, EventArgs e)
        {
            m_mMap.GMapGrid.CellSizeAdjust((int)nudMapCellSize.Value);
            m_mMap.CellSizeAdjust((int)nudMapCellSize.Value);
            splitContainer1.Panel2.AutoScrollMinSize = new Size((int)nudMapCellSize.Value * (int)nudMapNumCols.Value, (int)nudMapCellSize.Value * (int)nudMapNumRows.Value);
        }

        void impTsDlg_CreatePushed(object sender, EventArgs e)
        {
            ImportTilesetDlg impDlg = sender as ImportTilesetDlg;
            if (impDlg != null)
            {
                // replacing a tileset
                if (!m_bAddTileset)
                {
                    // set up first selected default tile
                    Rectangle srcRect = new Rectangle(0, 0, impDlg.Ts.NCellWidth, impDlg.Ts.NCellHeight);
                    m_tCurrTile = new CTILE(0, srcRect, m_nCurrTileFlag, m_nCurrImageID[m_nCurrTilesetIndex]);
	
	                m_mMap.ClrTilesetKey = impDlg.ClrKey;
                    m_clrKey[m_nCurrTilesetIndex] = impDlg.ClrKey;
	                m_mMap.NZoom = (int)nudMapZoom.Value;

                    m_strTilesetFilenames[m_nCurrTilesetIndex] = Path.GetFullPath(impDlg.StrFileName);

                    m_nCurrImageID[m_nCurrTilesetIndex] = impDlg.Ts.NTilesetImageID;
                    AddTileset(impDlg.Ts.NTilesetImageID, impDlg.ClrKey, impDlg.Ts.NTilesetWidth, impDlg.Ts.NTilesetHeight,
                                            impDlg.Ts.NCellWidth, impDlg.Ts.NCellHeight, m_strTilesetFilenames[m_nCurrTilesetIndex]);

	                m_impTsDlg.Close();
	
	                nudMapNumRows.Enabled = false;
	                nudMapNumCols.Enabled = false;
	                nudMapCellSize.Enabled = false;

                } 
                // adding a new tileset
                else
                {
                    // set up default starting selected tile
                    Rectangle SourceRect = new Rectangle(0, 0, impDlg.Ts.NCellWidth, impDlg.Ts.NCellHeight);
                    m_tCurrTile = new CTILE(0, SourceRect, m_nCurrTileFlag, impDlg.Ts.NTilesetImageID);
                    AddTileset(impDlg.Ts.NTilesetImageID, impDlg.ClrKey, impDlg.Ts.NTilesetWidth, impDlg.Ts.NTilesetHeight,
                                            impDlg.Ts.NCellWidth, impDlg.Ts.NCellHeight, impDlg.StrFileName);

                    m_strTilesetFilenames[m_nCurrTilesetIndex] = Path.GetFullPath(impDlg.StrFileName);

                    m_nCurrImageID[m_nCurrTilesetIndex] = impDlg.Ts.NTilesetImageID;
                    m_clrKey[m_nCurrTilesetIndex] = impDlg.ClrKey;

                    m_impTsDlg.Close();
                }
            }
        }
        void impTsDlg_Close(object sender, FormClosingEventArgs e)
        {
            m_impTsDlg = null;
            m_bDontDraw = false;
        }

        private void toolStripMenuItem2_Click(object sender, EventArgs e)
        {
            // Creating a new map
            if (m_newMapDlg == null)
            {
                m_newMapDlg = new NewMap();
                m_newMapDlg.FormClosing += new FormClosingEventHandler(newMap_FormClosing);
                m_newMapDlg.createPushed += new EventHandler(newMap_createPushed);
                m_newMapDlg.Show(this);
            }
        }

        void newMap_createPushed(object sender, EventArgs e)
        {
            NewMap newMap = sender as NewMap;
            if (newMap != null)
            {
                m_bIsIsometric = newMap.BIsIsometric;
                if (!newMap.BIsIsometric)
                {
                    m_mMap = new CMap(newMap.NCellSize, newMap.NColumns, newMap.NRows, m_nZoomIncrement, false);
                    nudMapCellSize.Value = newMap.NCellSize;
                    nudMapCellSize.Enabled = true;
                    splitContainer1.Panel2.AutoScrollMinSize = new Size(m_mMap.NMapWidth, m_mMap.NMapHeight);
                } 
                else
                {
                    m_mMap = new CMap(newMap.NIsoWidth, newMap.NIsoHeight, newMap.NColumns, newMap.NRows, m_nZoomIncrement, true);
                    nudMapNumCols.Enabled = false;
                    nudMapNumRows.Enabled = false;
                    nudMapCellSize.Enabled = false;
                    nudMapZoom.Enabled = false;
                    splitContainer1.Panel2.AutoScrollMinSize = new Size(m_mMap.NMapWidth, m_mMap.NMapHeight);
                }
                if (m_nCurrTilesetIndex == -1)
                    m_nCurrTilesetIndex = 0;
                m_mMap.NPanelWidth = splitContainer1.Panel2.Width;
                m_mMap.NPanelHeight = splitContainer1.Panel2.Height;
                nudMapNumCols.Value = newMap.NColumns;
                nudMapNumRows.Value = newMap.NRows;
                if (m_tsTileset[m_nCurrTilesetIndex] != null)
	                m_nCurrImageID[m_nCurrTilesetIndex] = m_tsTileset[m_nCurrTilesetIndex].NTilesetImageID;
                m_mMap.ClrTilesetKey = m_clrKey[m_nCurrTilesetIndex];
                m_strSaveFileName = "MyMap";
                this.Text = "TED - " + m_strSaveFileName;
                newMap.Close();
            }

        }

        void newMap_FormClosing(object sender, FormClosingEventArgs e)
        {
            m_newMapDlg = null;
        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (m_bChangesMade)
            {
                DialogResult dr = MessageBox.Show(this, "Would you like to save changes?", "Save before exit..", 
                                    MessageBoxButtons.YesNoCancel, MessageBoxIcon.Question);
                if (dr == DialogResult.Yes)
                {
                    if (SaveFile(m_bHaveSaved))
                    {
                    }
                    else
                    { e.Cancel = true; return; }
                    
                }
                else if (dr == DialogResult.Cancel)
                {
                    e.Cancel = true;
                    return;
                }
            }
            m_mTM.ShutdownManagedTextureManager();
        }

        public void nudTilesetWidth_ValueChanged(object sender, EventArgs e)
        {
            if (m_tsTileset[m_nCurrTilesetIndex] != null)
            {
                m_tsTileset[m_nCurrTilesetIndex].GTilesetGrid.GridAdjust(((int)m_tsComponents[m_nCurrTilesetIndex].nudTilesetGridWidth.Value),
                                                                    ((int)m_tsComponents[m_nCurrTilesetIndex].nudTilesetGridHeight.Value));
                m_bDontDraw = false;
            }
        }

        public void nudTilesetHeight_ValueChanged(object sender, EventArgs e)
        {
            nudTilesetWidth_ValueChanged(sender, e);
        }

        private void splitContainer1_Panel1_Scroll(object sender, ScrollEventArgs e)
        {
            ScrollOrientation so = e.ScrollOrientation;
            if (so == ScrollOrientation.HorizontalScroll)
            {
                m_tsTileset[m_nCurrTilesetIndex].GTilesetGrid.Offset(-(e.NewValue - e.OldValue), 0);
                m_tsTileset[m_nCurrTilesetIndex].OffsetImage(-(e.NewValue - e.OldValue), 0);
                m_nXScrollOffset = -(e.NewValue);
            }
            else
            {
                m_tsTileset[m_nCurrTilesetIndex].GTilesetGrid.Offset(0, -(e.NewValue - e.OldValue));
                m_tsTileset[m_nCurrTilesetIndex].OffsetImage(0, -(e.NewValue - e.OldValue));
                m_nYScrollOffset = -(e.NewValue);
            }
            m_bDontDraw = false;
        }

        private void splitContainer1_Panel2_Scroll(object sender, ScrollEventArgs e)
        {
            ScrollOrientation so = e.ScrollOrientation;
            if (so == ScrollOrientation.HorizontalScroll)
            {
                m_mMap.GMapGrid.Offset(-(e.NewValue - e.OldValue), 0);
                m_mMap.OffsetImage(-(e.NewValue - e.OldValue), 0);
            }
            else
            {
                m_mMap.GMapGrid.Offset(0, -(e.NewValue - e.OldValue));
                m_mMap.OffsetImage(0, -(e.NewValue - e.OldValue));
            }
            m_bDontDraw = false;
        }

        protected override void OnPaintBackground(PaintEventArgs e)
        {
            base.OnPaintBackground(e);
        }

        protected override bool ProcessCmdKey(ref Message msg, Keys keyData)
        {
            if (keyData == Keys.Add)
            {
                m_fRotation += 0.1f;
            }
            else if (keyData == Keys.Subtract)
            {
                m_fRotation -= 0.1f;
            }
            if (keyData == Keys.G)
            {
                cbShowGrid.Checked = !cbShowGrid.Checked;
                m_bIsGridOn = cbShowGrid.Checked;
                m_tsTileset[m_nCurrTilesetIndex].ShowGrid = m_bIsGridOn;
            }
            // Fills all tiles with currently selected tile
            else if (keyData == Keys.F && m_nCurrLayer != (int)LAYER.LAYER_FREE)
            {
                if (DialogResult.Yes == MessageBox.Show("Are you sure you want to fill all tiles?", "Fill all...", MessageBoxButtons.YesNo))
                {
                    for (int i = 0; i < m_mMap.NTotalNumTiles; ++i)
                        m_mMap.AddTile(m_tCurrTile, i);
                }
            }
            else if (keyData == Keys.Space && m_bIsTwoLayersOn)
            {
                if (cbLayer.SelectedIndex < (int)LAYER.LAYER_FREE)
                    cbLayer.SelectedIndex++;
                else
                    cbLayer.SelectedIndex = (int)LAYER.LAYER_ONE;
            }
            else if (keyData == Keys.C)
                m_mMap.ClearMap();
            else if (keyData == Keys.Q && m_bIsTwoLayersOn)
                cbLayerMode.SelectedIndex = (int)LAYER_MODE.SHOW_L1;
            else if (keyData == Keys.W && m_bIsTwoLayersOn)
                cbLayerMode.SelectedIndex = (int)LAYER_MODE.SHOW_L2;
            else if (keyData == Keys.E && m_bIsTwoLayersOn)
                cbLayerMode.SelectedIndex = (int)LAYER_MODE.SHOW_BOTH;
            else if (keyData == Keys.R && m_bIsTwoLayersOn)
                cbLayerMode.SelectedIndex = (int)LAYER_MODE.SHOW_ALL;
            #region tileFlagKeys 1
            else if (keyData == Keys.D0)
            {
                if (!m_bMarqueeSelect)
                {
                    if (m_tsComponents[m_nCurrTilesetIndex].cbTileFlag.SelectedIndex == 0)
                        m_tCurrTile.NTileFlag = 0;
                    m_tsComponents[m_nCurrTilesetIndex].cbTileFlag.SelectedIndex = 0;
                }
                else
                {
                    m_tsComponents[m_nCurrTilesetIndex].cbTileFlag.SelectedIndex = 0;
                    for (int i = 0; i < m_tMarqueeTiles.GetLength(0); ++i)
                    {
                        m_tMarqueeTiles[i].NTileFlag = 0;
                        m_tsTileset[m_nCurrTilesetIndex].TTilesetTiles[m_tMarqueeTiles[i].NSourceID].NTileFlag = 0;
                    }
                }
            }
            else if (keyData == Keys.D1)
            {
                if (!m_bMarqueeSelect)
                {
                    if (m_tsComponents[m_nCurrTilesetIndex].cbTileFlag.SelectedIndex == 1)
                        m_tCurrTile.NTileFlag = 1;
                    m_tsComponents[m_nCurrTilesetIndex].cbTileFlag.SelectedIndex = 1;
                }
                else
                {
                    m_tsComponents[m_nCurrTilesetIndex].cbTileFlag.SelectedIndex = 1;
                    for (int i = 0; i < m_tMarqueeTiles.GetLength(0); ++i)
                    {
                        m_tMarqueeTiles[i].NTileFlag = 1;
                        m_tsTileset[m_nCurrTilesetIndex].TTilesetTiles[m_tMarqueeTiles[i].NSourceID].NTileFlag = 1;
                    }
                }
            }
            else if (keyData == Keys.D2)
            {
                if (!m_bMarqueeSelect)
                {
                    if (m_tsComponents[m_nCurrTilesetIndex].cbTileFlag.SelectedIndex == 2)
                        m_tCurrTile.NTileFlag = 2;
                    m_tsComponents[m_nCurrTilesetIndex].cbTileFlag.SelectedIndex = 2;
                }
                else
                {
                    m_tsComponents[m_nCurrTilesetIndex].cbTileFlag.SelectedIndex = 2;
                    for (int i = 0; i < m_tMarqueeTiles.GetLength(0); ++i)
                    {
                        m_tMarqueeTiles[i].NTileFlag = 2;
                        m_tsTileset[m_nCurrTilesetIndex].TTilesetTiles[m_tMarqueeTiles[i].NSourceID].NTileFlag = 2;
                    }
                }
            }
            else if (keyData == Keys.D3)
            {
                if (!m_bMarqueeSelect)
                {
                    if (m_tsComponents[m_nCurrTilesetIndex].cbTileFlag.SelectedIndex == 3)
                        m_tCurrTile.NTileFlag = 3;
                    m_tsComponents[m_nCurrTilesetIndex].cbTileFlag.SelectedIndex = 3;
                }
                else
                {
                    m_tsComponents[m_nCurrTilesetIndex].cbTileFlag.SelectedIndex = 3;
                    for (int i = 0; i < m_tMarqueeTiles.GetLength(0); ++i)
                    {
                        m_tMarqueeTiles[i].NTileFlag = 3;
                        m_tsTileset[m_nCurrTilesetIndex].TTilesetTiles[m_tMarqueeTiles[i].NSourceID].NTileFlag = 3;
                    }
                }
            }
            #endregion
            else if (keyData == Keys.S)
            {
                cbShowFlags.Checked = !cbShowFlags.Checked;
                m_mMap.BShowFlags = cbShowFlags.Checked;
                m_tsTileset[m_nCurrTilesetIndex].BShowFlags = cbShowFlags.Checked;
            }
            else if (keyData == Keys.Escape)
                this.Close();
            
            m_bDontDraw = false;
            return base.ProcessCmdKey(ref msg, keyData);
        }

        private void splitContainer1_Panel1_MouseClick(object sender, MouseEventArgs e)
        {
            if ( m_nCurrTilesetIndex != -1 && m_tsTileset[m_nCurrTilesetIndex] != null && m_bSingleSelect)
            {
	            if (e.X <= m_tsTileset[m_nCurrTilesetIndex].NWidth && e.Y > m_nTilesetYoffset && e.Y < m_tsTileset[m_nCurrTilesetIndex].NHeight + m_nTilesetYoffset)
	            {
                    m_bDontDraw = false;
		            Point click = new Point(e.X, e.Y);
                    click.X += (-m_nTilesetXoffset + (-splitContainer1.Panel1.AutoScrollPosition.X));
		            click.Y += (-m_nTilesetYoffset + (-splitContainer1.Panel1.AutoScrollPosition.Y));
                    Point anchor = click;
                    
		            click.X = (click.X / m_tsTileset[m_nCurrTilesetIndex].NCellWidth);
		            click.Y = (click.Y / m_tsTileset[m_nCurrTilesetIndex].NCellHeight);
		            m_tsTileset[m_nCurrTilesetIndex].NCurrSelectedTile = click.Y * m_tsTileset[m_nCurrTilesetIndex].NNumCols + click.X;

                    // add a tile from the tile set to the currently selected
                    int nSourceID = m_tsTileset[m_nCurrTilesetIndex].NCurrSelectedTile;
                    int newTileFlag = m_tsTileset[m_nCurrTilesetIndex].TTilesetTiles[nSourceID].NTileFlag;
                    int left = (nSourceID % m_tsTileset[m_nCurrTilesetIndex].NNumCols) * m_tsTileset[m_nCurrTilesetIndex].NCellWidth;
                    int top = (nSourceID / m_tsTileset[m_nCurrTilesetIndex].NNumCols) * m_tsTileset[m_nCurrTilesetIndex].NCellHeight;
                    Rectangle sRect = new Rectangle(left, top, m_tsTileset[m_nCurrTilesetIndex].NCellWidth, m_tsTileset[m_nCurrTilesetIndex].NCellHeight);
                    m_tCurrTile = new CTILE(nSourceID, sRect, newTileFlag, m_nCurrImageID[m_nCurrTilesetIndex]);
                    tbAnchorX.Text = m_tsTileset[m_nCurrTilesetIndex].TTilesetTiles[nSourceID].AnchorX.ToString();
                    tbAnchorY.Text = m_tsTileset[m_nCurrTilesetIndex].TTilesetTiles[nSourceID].AnchorY.ToString();
                    m_tsComponents[m_nCurrTilesetIndex].cbTileFlag.SelectedIndex = newTileFlag;
                    m_tsTileset[m_nCurrTilesetIndex].DrawMarquee = false;

                    lblTileID.Text = m_strSelectedID + nSourceID.ToString();
                    if (m_bFreePlace)
                    {
                        m_ptMouseFreePlacePos = e.Location;
                    }
		            if (MouseButtons.Right == e.Button)
                    {
                        m_tCurrTile.AnchorX = anchor.X - left;
                        m_tCurrTile.AnchorY = anchor.Y - top;
                        m_tsTileset[m_nCurrTilesetIndex].TTilesetTiles[nSourceID].AnchorX = anchor.X - left;
                        m_tsTileset[m_nCurrTilesetIndex].TTilesetTiles[nSourceID].AnchorY = anchor.Y - top;
                        tbAnchorX.Text = m_tCurrTile.AnchorX.ToString();
                        tbAnchorY.Text = m_tCurrTile.AnchorY.ToString();
                    }
                    m_tsTileset[m_nCurrTilesetIndex].SetSelectionRect();
	            }
            }
        }
        private void splitContainer1_Panel1_MouseDown(object sender, MouseEventArgs e)
        {
            m_ptTopLeft.X = -1;
            m_ptTopLeft.Y = -1;
            m_ptBottomRight.X = -1;
            m_ptBottomRight.Y = -1;
            if (m_ptTopLeft.X == -1)
            {
                Point click = new Point(e.X, e.Y);
                click.X += (-m_nTilesetXoffset + (-splitContainer1.Panel1.AutoScrollPosition.X));
                click.Y += (-m_nTilesetYoffset + (-splitContainer1.Panel1.AutoScrollPosition.Y));
                m_bMarqueeSelect = false;
                m_bDrawMarquee = false;
                m_bIsDraggingMarquee = false;
	            m_ptTopLeft.X = click.X;
	            m_ptTopLeft.Y = click.Y;
            }

        }
        private void splitContainer1_Panel1_MouseMove(object sender, MouseEventArgs e)
        {
            if (m_nCurrTilesetIndex != -1)
            {
	            Point ptNewest = new Point(e.X, e.Y);
	            ptNewest.X += (-m_nTilesetXoffset + (-splitContainer1.Panel1.AutoScrollPosition.X));
	            ptNewest.Y += (-m_nTilesetYoffset + (-splitContainer1.Panel1.AutoScrollPosition.Y));
	            if (MouseButtons.Left == e.Button)
	            {
                    m_bDontDraw = false;
		            if (!m_bDrawMarquee && m_ptTopLeft.X != -1)
		            {
			            if (m_ptTopLeft.X >= ptNewest.X + 8 || m_ptTopLeft.X <= ptNewest.X - 8 ||
			                    m_ptTopLeft.Y >= ptNewest.Y + 8 || m_ptTopLeft.Y <= ptNewest.Y - 8)
			            {
			                m_bDrawMarquee = true;
                            m_tsTileset[m_nCurrTilesetIndex].DrawMarquee = true;
                            m_bSingleSelect = false;
                            m_bIsDraggingMarquee = true;
		                    m_ptBottomRight.X = ptNewest.X;
		                    m_ptBottomRight.Y = ptNewest.Y;
			            }
		            }
		            if (m_bDrawMarquee)
		            {
		                m_bMarqueeSelect = true;
		                m_ptBottomRight.X = ptNewest.X;
		                m_ptBottomRight.Y = ptNewest.Y;
		            }
	            }
	            else if (MouseButtons.Right == e.Button)
	            {
                    m_bDontDraw = false;
                    int nSourceID = m_tsTileset[m_nCurrTilesetIndex].NCurrSelectedTile;
	                int left = (nSourceID % m_tsTileset[m_nCurrTilesetIndex].NNumCols) * m_tsTileset[m_nCurrTilesetIndex].NCellWidth;
	                int top = (nSourceID / m_tsTileset[m_nCurrTilesetIndex].NNumCols) * m_tsTileset[m_nCurrTilesetIndex].NCellHeight;
	
	                if (ptNewest.X > left && ptNewest.X < left + m_tsTileset[m_nCurrTilesetIndex].NCellWidth
	                    && ptNewest.Y > top && ptNewest.Y < top + m_tsTileset[m_nCurrTilesetIndex].NCellHeight)
	                {
	                    m_tCurrTile.AnchorX = ptNewest.X - left;
	                    m_tCurrTile.AnchorY = ptNewest.Y - top;
	                    m_tsTileset[m_nCurrTilesetIndex].TTilesetTiles[nSourceID].AnchorX = ptNewest.X - left;
	                    m_tsTileset[m_nCurrTilesetIndex].TTilesetTiles[nSourceID].AnchorY = ptNewest.Y - top;
	                    tbAnchorX.Text = m_tCurrTile.AnchorX.ToString();
	                    tbAnchorY.Text = m_tCurrTile.AnchorY.ToString();
	                    m_tsTileset[m_nCurrTilesetIndex].SetSelectionRect();
	                }
	                else if (ptNewest.X < left)
	                {
	                    m_tCurrTile.AnchorX = 0;
	                    m_tsTileset[m_nCurrTilesetIndex].TTilesetTiles[nSourceID].AnchorX = 0;
	                    tbAnchorX.Text = "0";
	                    m_tsTileset[m_nCurrTilesetIndex].SetSelectionRect();
	                }
	                else if (ptNewest.X > left + m_tsTileset[m_nCurrTilesetIndex].NCellWidth)
	                {
	                    m_tCurrTile.AnchorX = m_tsTileset[m_nCurrTilesetIndex].NCellWidth;
	                    m_tsTileset[m_nCurrTilesetIndex].TTilesetTiles[nSourceID].AnchorX = m_tCurrTile.AnchorX;
	                    tbAnchorX.Text = m_tsTileset[m_nCurrTilesetIndex].NCellWidth.ToString();
	                    m_tsTileset[m_nCurrTilesetIndex].SetSelectionRect();
	                }
	                else if (ptNewest.Y < top)
	                {
	                    m_tCurrTile.AnchorY = 0;
	                    m_tsTileset[m_nCurrTilesetIndex].TTilesetTiles[nSourceID].AnchorY = m_tCurrTile.AnchorY;
	                    tbAnchorY.Text = "0";
	                    m_tsTileset[m_nCurrTilesetIndex].SetSelectionRect();
	                }
	                else if (ptNewest.Y > top + m_tsTileset[m_nCurrTilesetIndex].NCellHeight)
	                {
	                    m_tCurrTile.AnchorX = m_tsTileset[m_nCurrTilesetIndex].NCellHeight;
	                    m_tsTileset[m_nCurrTilesetIndex].TTilesetTiles[nSourceID].AnchorY = m_tCurrTile.AnchorY;
	                    tbAnchorY.Text = m_tsTileset[m_nCurrTilesetIndex].NCellHeight.ToString();
	                    m_tsTileset[m_nCurrTilesetIndex].SetSelectionRect();
	                }
	            }
            }
        }
        private void splitContainer1_Panel1_MouseUp(object sender, MouseEventArgs e)
        {
            if (m_bDrawMarquee)
            {
                m_bDontDraw = false;
                m_bSingleSelect = true;
                m_bIsDraggingMarquee = false;
                int swap;
                if (m_ptBottomRight.X < m_ptTopLeft.X)
                {
                    swap = m_ptTopLeft.X;
                    m_ptTopLeft.X = m_ptBottomRight.X;
                    m_ptBottomRight.X = swap;
                }
                if (m_ptBottomRight.Y < m_ptTopLeft.Y)
                {
                    swap = m_ptTopLeft.Y;
                    m_ptTopLeft.Y = m_ptBottomRight.Y;
                    m_ptBottomRight.Y = swap;
                }
	            int startCol = m_ptTopLeft.X / m_tsTileset[m_nCurrTilesetIndex].GTilesetGrid.NCellWidth;
	            int sourceX = startCol * m_tsTileset[m_nCurrTilesetIndex].GTilesetGrid.NCellWidth;
	            int endCol = m_ptBottomRight.X / m_tsTileset[m_nCurrTilesetIndex].GTilesetGrid.NCellWidth;
	
	            int startRow = m_ptTopLeft.Y / m_tsTileset[m_nCurrTilesetIndex].GTilesetGrid.NCellHeight;
	            int sourceY = startRow * m_tsTileset[m_nCurrTilesetIndex].GTilesetGrid.NCellHeight;
	            int endRow = m_ptBottomRight.Y / m_tsTileset[m_nCurrTilesetIndex].GTilesetGrid.NCellHeight;
	
	            int totalNumTiles = ((endCol + 1) - startCol) * ((endRow + 1) - startRow);
	            m_nMarqueeNumCols = (endCol + 1) - startCol;
	            int currCol = startCol;
	            int currRow = startRow;
	
	            int ID = currRow * m_tsTileset[m_nCurrTilesetIndex].NNumCols + currCol;
	            int startID = ID;
	            int count = 0;
	
	            m_tMarqueeTiles = new CTILE[totalNumTiles];
                m_tsTileset[m_nCurrTilesetIndex].MarqueeTiles = new CTILE[totalNumTiles];
	            for (int i = 0; i < totalNumTiles; ++i)
	            {
	                Rectangle sRect = new Rectangle(sourceX, sourceY, m_tsTileset[m_nCurrTilesetIndex].GTilesetGrid.NCellWidth, m_tsTileset[m_nCurrTilesetIndex].GTilesetGrid.NCellHeight);
                    m_tMarqueeTiles[i] = new CTILE(ID, sRect, m_tsTileset[m_nCurrTilesetIndex].TTilesetTiles[ID].NTileFlag, m_nCurrImageID[m_nCurrTilesetIndex]);
                    m_tsTileset[m_nCurrTilesetIndex].MarqueeTiles[i] = m_tMarqueeTiles[i];
	                currCol++;
	                ID++;
	                if (currCol > endCol)
	                {
	                    count++;
	                    currCol = startCol;
	                    currRow++;
	                    ID = startID + (m_tsTileset[m_nCurrTilesetIndex].NNumCols * count);
	                    sourceY = currRow * m_tsTileset[m_nCurrTilesetIndex].GTilesetGrid.NCellHeight;
	                }
	                sourceX = currCol * m_tsTileset[m_nCurrTilesetIndex].GTilesetGrid.NCellWidth;
	            }
                m_tsTileset[m_nCurrTilesetIndex].SetMarqueeSelectionRect(m_tMarqueeTiles[0].NSourceID, m_nMarqueeNumCols, (endRow + 1) - startRow);
                m_tsComponents[m_nCurrTilesetIndex].cbTileFlag.SelectedIndex = m_tMarqueeTiles[0].NTileFlag;
            }
        }

        private void splitContainer1_Panel2_MouseClick(object sender, MouseEventArgs e)
        {
            if (m_nCurrTilesetIndex == -1)
                return;
            m_bDontDraw = false;
            Point click = new Point(e.X, e.Y);
            Point freeClick = new Point(e.X, e.Y);
            click.X += (-splitContainer1.Panel2.AutoScrollPosition.X);
            click.Y += (-splitContainer1.Panel2.AutoScrollPosition.Y);
            freeClick = click;
            m_bDeleting = false;

            #region non-isometric 1
            if (!m_bIsIsometric)
            {
                int numVertLines = (click.X / m_mMap.NCellSize) / 2;
                int numHorLines = (click.Y / m_mMap.NCellSize);
                click.X = ((click.X + numVertLines) / m_mMap.NCellSize);
                click.Y = ((click.Y + numHorLines) / m_mMap.NCellSize);
                int ID = click.Y * (m_mMap.GMapGrid.NNumVertLines - 1) + click.X;
                if (e.X < m_mMap.NMapWidth && e.Y < m_mMap.NMapHeight && m_tsTileset[m_nCurrTilesetIndex] != null)
                {
                    m_bChangesMade = true;
                    if (e.Button == MouseButtons.Right)
                    {
                        if (!m_bFreePlace)
                        {
	                        if (m_nCurrLayer == (int)LAYER.LAYER_ONE)
	                            m_mMap.TMapTiles[ID] = new CTILE();
	                        else
                                m_mMap.TMapTilesLayer2[ID] = new CTILE();
                            return;
                        }
                    }
                    else
                    {
	                    bool bIsTileAlreadPlaced = false;
	
	                    if (!m_bMarqueeSelect)
	                    {
	                        if (m_nCurrLayer == (int)LAYER.LAYER_ONE)
	                            if (m_mMap.TMapTiles[ID].NSourceID == m_tCurrTile.NSourceID)
	                                bIsTileAlreadPlaced = true;
	                            else
	                                if (m_mMap.TMapTilesLayer2[ID].NSourceID == m_tCurrTile.NSourceID)
	                                    bIsTileAlreadPlaced = true;
	                        if (!bIsTileAlreadPlaced)
	                        {
	                            if (!m_bFreePlace)
		                            m_mMap.AddTile(m_tCurrTile, ID);
                                else if (m_bFreePlace && !m_bIsDragging)
                                    m_mMap.AddFreePlacedTile(m_tCurrTile, freeClick.X, freeClick.Y);
                            }
	                    }
                        // we are now adding all marquee selected tiles to the map...if the click point is valid
	                    else if (m_bMarqueeSelect)
	                    {
	                        if (e.X + -splitContainer1.Panel2.AutoScrollPosition.X + m_nMarqueeNumCols * m_tsTileset[m_nCurrTilesetIndex].NCellWidth < m_mMap.NPanelWidth)
	                        {
	                            int ColCount = 0;
	                            int RowCount = 0;
	                            int startID = ID;
	                            int startX = freeClick.X;
	                            int currX = startX;
	                            int currY = freeClick.Y;
	
	                            if (!m_bFreePlace)
	                            {
	                                for (int i = 0; i < m_tMarqueeTiles.GetLength(0); ++i)
	                                {
	                                        m_mMap.AddTile(m_tMarqueeTiles[i], ID++);
	                                        ++ColCount;
	                                        if (ColCount == m_nMarqueeNumCols)
	                                        {
	                                            ++RowCount;
	                                            ID = startID + (m_mMap.NNumCols * RowCount);
	                                            ColCount = 0;
	                                        }
	                                }
	                            }
	                            else
	                            {
                                    // free place tile, when marquee selected, is broken up into
                                    // cellWidth & cellHeight sized tiles, any left over just makes
                                    // extra smaller tiles, but they are still individual tiles.
                                    // this makes it possible to set collision and such in more detail
                                    int currWidth, currHeight, numRows, numCols, currTilesetX, currTilesetY;
                                    currWidth   = m_tsTileset[m_nCurrTilesetIndex].NCellWidth;
                                    currHeight  = m_tsTileset[m_nCurrTilesetIndex].NCellHeight;
                                    numRows     = (m_ptBottomRight.Y - m_ptTopLeft.Y + 32) / currHeight;
                                    numCols     = (m_ptBottomRight.X - m_ptTopLeft.X + 32) / currWidth;
                                    currTilesetX = m_ptTopLeft.X;
                                    currTilesetY = m_ptTopLeft.Y;
	                                for (int i = 0; i < numCols * numRows; ++i)
	                                {
                                        Rectangle srcRect = new Rectangle(m_ptTopLeft.X + (ColCount * m_tsTileset[m_nCurrTilesetIndex].NCellWidth), 
                                                                          m_ptTopLeft.Y + (RowCount * m_tsTileset[m_nCurrTilesetIndex].NCellHeight),
                                                                          currWidth, currHeight);
	                                    m_mMap.AddMarqueeFreePlaceTile(m_tMarqueeTiles[i], currX, currY, srcRect);
	                                    ++ColCount;
	                                    currX += m_tsTileset[m_nCurrTilesetIndex].NCellWidth;
                                        currTilesetX += m_tsTileset[m_nCurrTilesetIndex].NCellWidth;

                                        if (ColCount == numCols-1)
                                        {
                                            currWidth = m_ptBottomRight.X - currTilesetX;
                                        }
                                        if (ColCount == numCols)
	                                    {
	                                        ++RowCount;
	                                        currX = startX;
                                            currY = freeClick.Y + m_tsTileset[m_nCurrTilesetIndex].NCellHeight * RowCount;
                                            currWidth = m_tsTileset[m_nCurrTilesetIndex].NCellWidth;
                                            currTilesetY += m_tsTileset[m_nCurrTilesetIndex].NCellHeight;
                                            currTilesetX = m_ptTopLeft.X;
	                                        ColCount = 0;
                                        }
                                        if (RowCount == numRows - 1)
                                        {
                                            currHeight = m_ptBottomRight.Y - currTilesetY;
                                        }
	                                }
	                            }
	                        }
                            else
                            {
                                // placing tiles off the right side of the map...not working right?
                            }
                        }
                    }
                }
            }
            #endregion
            // we are in isometric view...
            #region isometric 1 
            else
            {
                int xID = 0, yID = 0;
                int cellHeight = m_mMap.GMapGrid.NCellHeight;
                int cellWidth  = m_mMap.GMapGrid.NCellWidth;
                // isometric ID equation
                xID = ((cellWidth * click.Y) + (cellHeight * (click.X - m_mMap.GMapGrid.NIsoCenterTopX))) / (cellWidth * cellHeight);
                yID = ((cellWidth * click.Y) - (cellHeight * (click.X - m_mMap.GMapGrid.NIsoCenterTopX))) / (cellWidth * cellHeight);
                lblXID.Text = "X ID = " + xID + " Y ID = " + yID;

                if (xID > -1 && xID < m_mMap.NNumCols &&
                        yID > -1 && yID < m_mMap.NNumRows)
                {
                    // changing a tile's trigger
                    if (Control.ModifierKeys == Keys.Shift && MouseButtons.Right == e.Button)
                    {
                        m_nCurrTileEditID = yID * m_mMap.NNumCols + xID;
                        switch (m_nCurrLayer)
                        {
                            case (int)LAYER.LAYER_ONE:
                                if (m_mMap.TMapTiles[m_nCurrTileEditID].NSourceID != -1)
                                {
                                    OpenSetTiggerDlg(m_mMap.TMapTiles[m_nCurrTileEditID].Trigger);
                                    return;
                                }
                                break;
                            case (int)LAYER.LAYER_TWO:
                                if (m_mMap.TMapTilesLayer2[m_nCurrTileEditID].NSourceID != -1)
                                {
                                    OpenSetTiggerDlg(m_mMap.TMapTilesLayer2[m_nCurrTileEditID].Trigger);
                                    return;
                                }
                                break;
                            case (int)LAYER.LAYER_FREE:
                                for (int i = 0; i < m_mMap.FreePlaced.GetLength(0); ++i)
                                {
                                    if (m_mMap.FreePlaced[i] == null)
                                        continue;
                                    if (freeClick.X > m_mMap.FreePlaced[i].MapPt.X &&
                                            freeClick.X < m_mMap.FreePlaced[i].MapPt.X + m_tsTileset[m_nCurrTilesetIndex].NCellWidth &&
                                            freeClick.Y > m_mMap.FreePlaced[i].MapPt.Y &&
                                            freeClick.Y < m_mMap.FreePlaced[i].MapPt.Y + m_tsTileset[m_nCurrTilesetIndex].NCellHeight)
                                    {
                                        m_nCurrTileEditID = i;
                                        OpenSetTiggerDlg(m_mMap.FreePlaced[i].Trigger);
                                        return;
                                    }
                                }
                                break;
                        }
                    }
                    // left button == placing
                    if (MouseButtons.Left == e.Button)
                    {
                        int tileID = yID * m_mMap.NNumCols + xID;
                        if (m_bMarqueeSelect)
                        {
                            // TODO:: implement the marquee placement, not free place
                            if (!m_bFreePlace)
                            {
                                m_bChangesMade = true;
                                return;
                            }
                        }
                        else if (!m_bMarqueeSelect)
                        {
                            if (m_bFreePlace && !m_bIsDragging)
                            {
                                m_bChangesMade = true;
                                m_mMap.AddFreePlacedTile(m_tCurrTile, freeClick.X, freeClick.Y);
                                return;
                            }
                            else if (!m_bFreePlace)
                            {
                                m_bChangesMade = true;
                                m_mMap.AddTile(m_tCurrTile, tileID);
                                return;
                            }
                        }
                    }
                    // right button == deleting, only if not changing a tile's trigger (shift is held)
                    else
                    {
                        if (!m_bFreePlace)
                        {
                            m_bChangesMade = true;
                            int tileID = yID * m_mMap.NNumCols + xID;
                            if (m_nCurrLayer == (int)LAYER.LAYER_ONE)
                                m_mMap.TMapTiles[tileID] = new CTILE();
                            else
                                m_mMap.TMapTilesLayer2[tileID] = new CTILE();
                            return;
                        }
                    }
                }
                // left button == placing
                if (MouseButtons.Left == e.Button && m_bFreePlace)
                {
                    m_bChangesMade = true;
                    // free place tile, when marquee selected, is broken up into
                    // cellWidth & cellHeight sized tiles, any left over just makes
                    // extra smaller tiles, but they are still individual tiles.
                    // this makes it possible to set collision and such in more detail
                    int ColCount = 0;
                    int RowCount = 0;
                    int startX = freeClick.X;
                    int currX = startX;
                    int currY = freeClick.Y;
                    int currWidth, currHeight, numRows, numCols, currTilesetX, currTilesetY;
                    currWidth = m_tsTileset[m_nCurrTilesetIndex].NCellWidth;
                    currHeight = m_tsTileset[m_nCurrTilesetIndex].NCellHeight;
                    numRows = (m_ptBottomRight.Y - m_ptTopLeft.Y + 32) / currHeight;
                    numCols = (m_ptBottomRight.X - m_ptTopLeft.X + 32) / currWidth;
                    currTilesetX = m_ptTopLeft.X;
                    currTilesetY = m_ptTopLeft.Y;
                    if (m_bMarqueeSelect)
                    {
                        for (int i = 0; i < numCols * numRows; ++i)
	                    {
	                        Rectangle srcRect = new Rectangle(m_ptTopLeft.X + (ColCount * m_tsTileset[m_nCurrTilesetIndex].NCellWidth),
	                                                          m_ptTopLeft.Y + (RowCount * m_tsTileset[m_nCurrTilesetIndex].NCellHeight),
	                                                          currWidth, currHeight);
	                        m_mMap.AddMarqueeFreePlaceTile(m_tMarqueeTiles[i], currX, currY, srcRect);
	                        ++ColCount;
	                        currX += m_tsTileset[m_nCurrTilesetIndex].NCellWidth;
	                        currTilesetX += m_tsTileset[m_nCurrTilesetIndex].NCellWidth;
	
	                        if (ColCount == numCols - 1)
	                        {
	                            currWidth = m_ptBottomRight.X - currTilesetX;
	                        }
	                        if (ColCount == numCols)
	                        {
	                            ++RowCount;
	                            currX = startX;
	                            currY = freeClick.Y + m_tsTileset[m_nCurrTilesetIndex].NCellHeight * RowCount;
	                            currWidth = m_tsTileset[m_nCurrTilesetIndex].NCellWidth;
	                            currTilesetY += m_tsTileset[m_nCurrTilesetIndex].NCellHeight;
	                            currTilesetX = m_ptTopLeft.X;
	                            ColCount = 0;
	                        }
	                        if (RowCount == numRows - 1)
	                        {
	                            currHeight = m_ptBottomRight.Y - currTilesetY;
	                        }
	                    }
                    } 
                    else
                    {
                        if (m_bFreePlace && !m_bIsDragging)
                        {
                            m_bChangesMade = true;
                            m_mMap.AddFreePlacedTile(m_tCurrTile, freeClick.X, freeClick.Y);
                            return;
                        }
                    }
                }

            }
            #endregion
            // deleting any free placed tiles
            if (m_bFreePlace && MouseButtons.Right == e.Button)
            {
                m_bDeleting = true;
                for (int i = 0; i < m_mMap.FreePlaced.GetLength(0); ++i )
                {
                    if (m_mMap.FreePlaced[i] == null)
                        continue;
                    if (freeClick.X > m_mMap.FreePlaced[i].MapPt.X &&
                            freeClick.X < m_mMap.FreePlaced[i].MapPt.X + m_tsTileset[m_nCurrTilesetIndex].NCellWidth &&
                            freeClick.Y > m_mMap.FreePlaced[i].MapPt.Y &&
                            freeClick.Y < m_mMap.FreePlaced[i].MapPt.Y + m_tsTileset[m_nCurrTilesetIndex].NCellHeight)
                    {
                        m_bChangesMade = true;
                        m_mMap.RemoveFreeTile(i);
                        return;
                    }
                }
            }
        }

        void OpenSetTiggerDlg(string currTileTrigger)
        {
            if (m_stDlg != null)
            {
                m_stDlg.Close();
                m_stDlg = null;
            }
            m_stDlg = new SetTriggerDlg(currTileTrigger);
            m_stDlg.FormClosing += new FormClosingEventHandler(stDlg_FormClosing);
            m_stDlg.AcceptPushed += new EventHandler(stDlg_AcceptPushed);
            m_stDlg.Show(this);

        }

        void stDlg_AcceptPushed(object sender, EventArgs e)
        {
            SetTriggerDlg stDlg = sender as SetTriggerDlg;
            if (stDlg.Changes)
            {
                m_bChangesMade = true;
	            switch (m_nCurrLayer)
	            {
	                case (int)LAYER.LAYER_ONE:
                        m_mMap.TMapTiles[m_nCurrTileEditID].Trigger = stDlg.Trigger;
	                    break;
	                case (int)LAYER.LAYER_TWO:
	                    m_mMap.TMapTilesLayer2[m_nCurrTileEditID].Trigger = stDlg.Trigger;
	                    break;
	                case (int)LAYER.LAYER_FREE:
                        m_mMap.FreePlaced[m_nCurrTileEditID].Trigger = stDlg.Trigger;
	                    break;
	            }
            }
            stDlg.Close();
        }
        void stDlg_FormClosing(object sender, FormClosingEventArgs e)
        {
            m_stDlg = null;
        }

        bool bPanel2StartPtCalculated = false;
        Point ptFirstPanel2Click;
        private void splitContainer1_Panel2_MouseDown(object sender, MouseEventArgs e)
        {
            // Map move click point:
            m_ptMapMoveOriginal = e.Location;

            //////////////////////////////////////////////////////////////////////////
            // only start dragging if the mouse has moved more than a certain amount of pixels
            if (!bPanel2StartPtCalculated)
            {
                ptFirstPanel2Click = new Point(e.X + (-splitContainer1.Panel2.AutoScrollOffset.X), e.Y + (-splitContainer1.Panel2.AutoScrollOffset.Y));
                bPanel2StartPtCalculated = true;
            }
            //////////////////////////////////////////////////////////////////////////
            if (m_bFreePlace && e.Button == MouseButtons.Right)
                m_bDeleting = true;
        }

        private void splitContainer1_Panel2_MouseMove(object sender, MouseEventArgs e)
        {
            //////////////////////////////////////////////////////////////////////////
            // only start dragging if the mouse has moved more than a certain amount of pixels
            if (Control.ModifierKeys != Keys.Control)
            {
	            if (bPanel2StartPtCalculated)
	            {
		            Point ptLatest = e.Location;
		            ptLatest.X += (-splitContainer1.Panel2.AutoScrollOffset.X);
		            ptLatest.Y += (-splitContainer1.Panel2.AutoScrollOffset.Y);
		            if (ptLatest.X >= ptFirstPanel2Click.X + 5 || ptLatest.X <= ptFirstPanel2Click.X - 5 ||
		                        m_ptDrawTL.Y >= ptFirstPanel2Click.Y + 5 || ptLatest.Y <= ptFirstPanel2Click.Y - 5)
		                m_bIsDragging = true;
	            }
	
	            if(m_bIsDragging)
	                splitContainer1_Panel2_MouseClick(sender, e);
	            if (m_bFreePlace)
	            {
	                m_bDontDraw = false;
	                m_ptMouseFreePlacePos = e.Location;
	            }
	            lblCursorPos.Text = "X=" + e.X + " Y=" + e.Y;
            } 
            else if (Control.ModifierKeys == Keys.Control && e.Button == MouseButtons.Left)
            {
                // TODO:: move the map around with the mouse
                // use map offset to adjust, starting at the top-left corner (0,0)

                // beginning at the click Point, calculate how far the mouse moves
                // before the button is released...update as we move.
                int i = 0;

            }
            #region MousedragAddTiles 0
//             if (e.Button == MouseButtons.Left)
//             {
//                 if (e.X > (mMap.GMapGrid.NNumVertLines-1) * mMap.GMapGrid.NCellSize + (mMap.GMapGrid.NGridZoom * mMap.GMapGrid.ZoomIncrement) && !bIsDragging)
//                 {
//                     bIsDragging = false;
//                     if (nudMapNumCols.Value < nudMapNumCols.Maximum)
//                     {
// 	                    nudMapNumCols.Value += 1;
//                     }
//                 }
//                 else if (e.Y > (mMap.GMapGrid.NNumHorizontalLines - 1) * mMap.GMapGrid.NCellSize + (mMap.GMapGrid.NGridZoom * mMap.GMapGrid.ZoomIncrement) && !bIsDragging)
//                 {
//                     bIsDragging = false;
//                     if (nudMapNumRows.Value < nudMapNumRows.Maximum)
//                     {
//                         nudMapNumRows.Value += 1;
//                     }
//                 }
//                 
//             }
//             else if (e.Button == MouseButtons.Right)
//             {
//                 if (e.X > (mMap.GMapGrid.NNumVertLines-1) * mMap.GMapGrid.NCellSize + (mMap.GMapGrid.NGridZoom * mMap.GMapGrid.ZoomIncrement))
//                 {
//                     if (nudMapNumCols.Value > 1)
//                     {
//                     	nudMapNumCols.Value -= 1;
//                     }
//                 }
//                 else if (e.Y > (mMap.GMapGrid.NNumHorizontalLines-1) * mMap.GMapGrid.NCellSize + (mMap.GMapGrid.NGridZoom * mMap.GMapGrid.ZoomIncrement))
//                 {
//                     if (nudMapNumRows.Value > 1)
//                     {
// 	                    nudMapNumRows.Value -= 1;
//                     }
//                 }
//             }
            #endregion

        }

        private void splitContainer1_Panel2_MouseUp(object sender, MouseEventArgs e)
        {
            m_bIsDragging = false;
            m_bDeleting = false;
            bPanel2StartPtCalculated = false;
        }

        private void splitContainer1_Panel1_Click(object sender, EventArgs e)
        {
            cbLayer.Select();
	        if (m_nCurrTilesetIndex != 0 && tabControl1.SelectedIndex - 1 != m_nCurrTilesetIndex)
            {
                m_bJustClick = true;
		        tabControl1.SelectedIndex = m_nCurrTilesetIndex;
	        }
        }

        private void cbLayer_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (m_mMap != null && m_bIsTwoLayersOn && m_nCurrTilesetIndex != -1)
            {
                m_bDontDraw = false;
                m_bFreePlace = false;
	            m_nCurrLayer = cbLayer.SelectedIndex;
                m_mMap.NCurrLayer = m_nCurrLayer;
                m_tsTileset[m_nCurrTilesetIndex].NCurrLayer = m_nCurrLayer + 1;
                if (m_nCurrLayer == (int)LAYER.LAYER_TWO)
                {
                    cbLayerMode.SelectedIndex = (int)LAYER_MODE.SHOW_BOTH;
                    lbCurrentLayer.Text = m_strLayerLabel + "Layer 2";
                }
                else if (m_nCurrLayer == (int)LAYER.LAYER_ONE)
                {
                    cbLayerMode.SelectedIndex = (int)LAYER_MODE.SHOW_L1;
                    lbCurrentLayer.Text = m_strLayerLabel + "Layer 1";
                }
                else if (m_nCurrLayer == (int)LAYER.LAYER_FREE)
                {
                    cbLayerMode.SelectedIndex = (int)LAYER_MODE.SHOW_ALL;
                    lbCurrentLayer.Text = m_strLayerLabel + "Free Tiles";
                    m_bFreePlace = true;
                }
            }
        }

        private void cbLayerMode_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (m_mMap != null && m_bIsTwoLayersOn)
            {
	            m_nLayerMode = cbLayerMode.SelectedIndex;
	            m_mMap.NLayerMode = m_nLayerMode;
                m_bDontDraw = false;
            }
        }

        private void toolStripMenuItem6_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        public void comboBox1_SelectedIndexChanged(object sender, EventArgs e)
        {
            m_nCurrTileFlag = m_tsComponents[m_nCurrTilesetIndex].cbTileFlag.SelectedIndex;
            for (int i = 0; i < tabControl1.TabCount-1; ++i )
            {
                if (m_tsTileset[m_nCurrTilesetIndex] != null)
                {
                    if (!m_bMarqueeSelect)
                    {
                        m_tsTileset[m_nCurrTilesetIndex].TTilesetTiles[m_tCurrTile.NSourceID].NTileFlag = m_nCurrTileFlag;
                        m_tCurrTile.NTileFlag = m_nCurrTileFlag;
                    }
                }
            }
            m_bDontDraw = false;
        }

        #region setTileFlags
        private void setTileFlagsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (m_stfDlg == null)
            {
                m_stfDlg = new SetTileFlagsDlg(m_str0, m_str1, m_str2, m_str3);
                m_stfDlg.FormClosing += new FormClosingEventHandler(stfDlg_FormClosing);
                m_stfDlg.setFlagsPushed += new EventHandler(stfDlg_setFlagsPushed);
                m_stfDlg.Show(this);
            }
        }

        void stfDlg_setFlagsPushed(object sender, EventArgs e)
        {
            SetTileFlagsDlg stfDlg = sender as SetTileFlagsDlg;
            if (stfDlg.Bflag0Changed)
            {
                m_str0 = "0=" + stfDlg.Flag0;
                m_tsComponents[m_nCurrTilesetIndex].cbTileFlag.Items.RemoveAt(0);
                m_tsComponents[m_nCurrTilesetIndex].cbTileFlag.Items.Insert(0, "0=" + stfDlg.Flag0);
            }
            if (stfDlg.Bflag1Changed)
            {
                m_str1 = "1=" + stfDlg.Flag1;
                m_tsComponents[m_nCurrTilesetIndex].cbTileFlag.Items.RemoveAt(1);
                m_tsComponents[m_nCurrTilesetIndex].cbTileFlag.Items.Insert(1, "1=" + stfDlg.Flag1);
            }
            if (stfDlg.Bflag2Changed)
            {
                m_str2 = "2=" + stfDlg.Flag2;
                m_tsComponents[m_nCurrTilesetIndex].cbTileFlag.Items.RemoveAt(2);
                m_tsComponents[m_nCurrTilesetIndex].cbTileFlag.Items.Insert(2, "2=" + stfDlg.Flag2);
            }
            if (stfDlg.Bflag3Changed)
            {
                m_str3 = "3=" + stfDlg.Flag3;
                m_tsComponents[m_nCurrTilesetIndex].cbTileFlag.Items.RemoveAt(3);
                m_tsComponents[m_nCurrTilesetIndex].cbTileFlag.Items.Insert(3, "3=" + stfDlg.Flag3);
            }
            m_tsComponents[m_nCurrTilesetIndex].cbTileFlag.SelectedIndex = 0;
            stfDlg.Close();
        }

        void stfDlg_FormClosing(object sender, FormClosingEventArgs e)
        {
            m_stfDlg = null;
        }
        #endregion

        private void splitContainer1_Panel1_Resize(object sender, EventArgs e)
        {
            for (int i = 0; i < tabControl1.TabCount-1; ++i )
            {
                if (m_tsTileset[i] != null)
                {
                    m_tsTileset[i].NPanelWidth = splitContainer1.Panel1.Width;
                    m_tsTileset[i].NPanelHeight = splitContainer1.Panel1.Height;
                }
            }
        }

        private void splitContainer1_Panel2_Resize(object sender, EventArgs e)
        {
            if (m_mMap != null)
            {
                m_mMap.NPanelWidth = splitContainer1.Panel2.Width;
                m_mMap.NPanelHeight = splitContainer1.Panel2.Height;
            }
        }


        private void twoLayersToolStripMenuItem_Click(object sender, EventArgs e)
        {
            m_bIsTwoLayersOn = !m_bIsTwoLayersOn;
            cbLayerMode.SelectedIndex = (int)LAYER_MODE.SHOW_L1;
            cbLayer.SelectedIndex = (int)LAYER.LAYER_ONE;
            lbCurrentLayer.Text = m_strLayerLabel + "Layer 1";
            if (!m_bIsTwoLayersOn)
            {
                cbLayer.Enabled = false;
            }
            else
            {
                cbLayer.Enabled = true;
            }
        }

        private void cbShowFlags_CheckedChanged(object sender, EventArgs e)
        {
            if (m_mMap != null && m_nCurrTilesetIndex != -1)
            {
                m_bDontDraw = false;
	            m_mMap.BShowFlags = cbShowFlags.Checked;
                m_tsTileset[m_nCurrTilesetIndex].BShowFlags = cbShowFlags.Checked;
            }
        }
        private void cbShowGrid_CheckedChanged(object sender, EventArgs e)
        {
            if (m_mMap != null && m_nCurrTilesetIndex != -1)
            {
                m_bDontDraw = false;
                m_bIsGridOn = cbShowGrid.Checked;
                m_tsTileset[m_nCurrTilesetIndex].ShowGrid = m_bIsGridOn;
            }
        }

        public void nudCellWidth_ValueChanged(object sender, EventArgs e)
        {
            m_tsTileset[m_nCurrTilesetIndex].AdjustCellWidth(((int)m_tsComponents[m_nCurrTilesetIndex].nudCellWidth.Value));
            m_tsTileset[m_nCurrTilesetIndex].SetSelectionRect();
            // add a tile from the tile set to the currently selected
            int nSourceID = m_tsTileset[m_nCurrTilesetIndex].NCurrSelectedTile;
            int newTileFlag = m_tsTileset[m_nCurrTilesetIndex].TTilesetTiles[nSourceID].NTileFlag;
            int left = (nSourceID % m_tsTileset[m_nCurrTilesetIndex].NNumCols) * m_tsTileset[m_nCurrTilesetIndex].NCellWidth;
            int top = (nSourceID / m_tsTileset[m_nCurrTilesetIndex].NNumCols) * m_tsTileset[m_nCurrTilesetIndex].NCellHeight;
            Rectangle sRect = new Rectangle(left, top, m_tsTileset[m_nCurrTilesetIndex].NCellWidth, m_tsTileset[m_nCurrTilesetIndex].NCellHeight);
            m_tCurrTile = new CTILE(nSourceID, sRect, newTileFlag, m_nCurrImageID[m_nCurrTilesetIndex]);
            m_tsComponents[m_nCurrTilesetIndex].cbTileFlag.SelectedIndex = newTileFlag;
            m_bDontDraw = false;
        }
        public void nudCellHeight_ValueChanged(object sender, EventArgs e)
        {
            m_tsTileset[m_nCurrTilesetIndex].AdjustCellHeight((int)m_tsComponents[m_nCurrTilesetIndex].nudCellHeight.Value);
            m_tsTileset[m_nCurrTilesetIndex].SetSelectionRect();
            // add a tile from the tile set to the currently selected
            int nSourceID = m_tsTileset[m_nCurrTilesetIndex].NCurrSelectedTile;
            int newTileFlag = m_tsTileset[m_nCurrTilesetIndex].TTilesetTiles[nSourceID].NTileFlag;
            int left = (nSourceID % m_tsTileset[m_nCurrTilesetIndex].NNumCols) * m_tsTileset[m_nCurrTilesetIndex].NCellWidth;
            int top = (nSourceID / m_tsTileset[m_nCurrTilesetIndex].NNumCols) * m_tsTileset[m_nCurrTilesetIndex].NCellHeight;
            Rectangle sRect = new Rectangle(left, top, m_tsTileset[m_nCurrTilesetIndex].NCellWidth, m_tsTileset[m_nCurrTilesetIndex].NCellHeight);
            m_tCurrTile = new CTILE(nSourceID, sRect, newTileFlag, m_nCurrImageID[m_nCurrTilesetIndex]);
            m_tsComponents[m_nCurrTilesetIndex].cbTileFlag.SelectedIndex = newTileFlag;
            m_bDontDraw = false;
        }

        private void allTheHelpYouNeedToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (m_helpDlg == null)
            {
                m_helpDlg = new Help();
                m_helpDlg.Show(this);
            }
        }

        private void changeBGColorToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ColorDialog cd = new ColorDialog();

            if (DialogResult.OK == cd.ShowDialog(this))
            {
                m_clrMapBGclear = cd.Color;
            }
        }
        private void tilesetToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ColorDialog cd = new ColorDialog();
            if (DialogResult.OK == cd.ShowDialog(this))
            {
                m_clrTilesetBGClear = cd.Color;
            }
        }

        private void importTilesetAddToolStripMenuItem_Click(object sender, EventArgs e)
        {
            m_bAddTileset = true;
            if (m_impTsDlg == null)
            {
                if (m_nCurrTilesetIndex == -1)
                    m_nCurrTilesetIndex = 0;
                else
                    m_nCurrTilesetIndex++;
                if (m_nCurrTilesetIndex == MAX_NUM_TILESETS)
                {
                    m_nCurrTilesetIndex = MAX_NUM_TILESETS - 1; 
                    MessageBox.Show(this, "You cannot add any more tilesets.", "Tileset limit reached!", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    return;
                }
                m_bDontDraw = true;
                m_impTsDlg = new ImportTilesetDlg();
                m_impTsDlg.Text = "Add New Tileset...";

                m_impTsDlg.FormClosing += new FormClosingEventHandler(impTsDlg_Close);
                m_impTsDlg.createPushed += new EventHandler(impTsDlg_CreatePushed);
                m_impTsDlg.Show(this);
            }
        }

        private void tabControl1_Selecting(object sender, TabControlCancelEventArgs e)
        {
            if (tabControl1.SelectedIndex != 0 && !m_bJustClick)
            {
                m_nCurrTilesetIndex = tabControl1.SelectedIndex - 1;
                while (m_tsTileset[m_nCurrTilesetIndex] == null)
                {
                    m_nCurrTilesetIndex++;
                }
                int selectedTile = m_tsTileset[m_nCurrTilesetIndex].NCurrSelectedTile;
                int newTileFlag = m_tsTileset[m_nCurrTilesetIndex].TTilesetTiles[selectedTile].NTileFlag;
                int left = (selectedTile % m_tsTileset[m_nCurrTilesetIndex].NNumCols) * m_tsTileset[m_nCurrTilesetIndex].NCellWidth;
                int top = (selectedTile / m_tsTileset[m_nCurrTilesetIndex].NNumCols) * m_tsTileset[m_nCurrTilesetIndex].NCellHeight;
                Rectangle sRect = new Rectangle(left, top, m_tsTileset[m_nCurrTilesetIndex].NCellWidth, m_tsTileset[m_nCurrTilesetIndex].NCellHeight);
                m_tCurrTile = new CTILE(selectedTile, sRect, newTileFlag, m_nCurrImageID[m_nCurrTilesetIndex]);
                m_bDrawMarquee = m_tsTileset[m_nCurrTilesetIndex].DrawMarquee;
                m_bMarqueeSelect = m_bDrawMarquee;
                if (m_bDrawMarquee)
                {
                    m_tMarqueeTiles = m_tsTileset[m_nCurrTilesetIndex].MarqueeTiles;
                }
            }
            m_bDontDraw = false;
            m_bJustClick = false;
        }

        private void removeCurrentTilesetToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (tabControl1.TabCount > 1)
            {
	            if (DialogResult.Yes == MessageBox.Show(this, "Are you sure you want to remove the " + m_strTilesetFilenames[m_nCurrTilesetIndex] + " tile set?",
	                    "Close tile set...", MessageBoxButtons.YesNoCancel, MessageBoxIcon.Warning))
	            {
	                m_tsTileset[m_nCurrTilesetIndex] = null;
                    tabControl1.TabPages.RemoveAt(m_nCurrTilesetIndex + 1);
                    if (tabControl1.TabCount > 0 && m_tsTileset[0] == null)
                    {
                        m_nCurrTilesetIndex = 1;
                        m_tsTileset[0] = m_tsTileset[1];
                        m_tsTileset[1] = null;
                        m_nCurrImageID[0] = m_nCurrImageID[1];
                        m_nCurrImageID[1] = -1;
                        m_tsComponents[0] = m_tsComponents[1];
                        m_tsComponents[1] = null;
                    }
	                tabControl1.SelectedIndex = tabControl1.TabCount - 1;
	            }
            } 
            if (tabControl1.TabCount == 1)
            {
                m_nCurrTilesetIndex = -1;
                m_tCurrTile = new CTILE();
            }
        }

        private void tbAnchorX_TextChanged(object sender, EventArgs e)
        {
            if (tbAnchorX.Text != "")
            {
	            if (int.Parse(tbAnchorX.Text) >= 0 && int.Parse(tbAnchorX.Text) <= m_tsTileset[m_nCurrTilesetIndex].NCellWidth)
	            {
	                m_tsTileset[m_nCurrTilesetIndex].TTilesetTiles[m_tCurrTile.NSourceID].AnchorX = int.Parse(tbAnchorX.Text);
	                m_tCurrTile.AnchorX = int.Parse(tbAnchorX.Text);
	                m_tsTileset[m_nCurrTilesetIndex].SetSelectionRect();
	            }
	            else
	                m_tsTileset[m_nCurrTilesetIndex].TTilesetTiles[m_tCurrTile.NSourceID].AnchorX = 0;
            }
        }

        private void tbAnchorY_TextChanged(object sender, EventArgs e)
        {
            if (tbAnchorY.Text != "")
            {
	            if (int.Parse(tbAnchorY.Text) >= 0 && int.Parse(tbAnchorY.Text) <= m_tsTileset[m_nCurrTilesetIndex].NCellHeight)
	            {
	                m_tsTileset[m_nCurrTilesetIndex].TTilesetTiles[m_tCurrTile.NSourceID].AnchorY = int.Parse(tbAnchorY.Text);
	                m_tCurrTile.AnchorY = int.Parse(tbAnchorY.Text);
	                m_tsTileset[m_nCurrTilesetIndex].SetSelectionRect();
	            }
	            else
	                m_tsTileset[m_nCurrTilesetIndex].TTilesetTiles[m_tCurrTile.NSourceID].AnchorY = 0;
            }
        }

        private void tbAnchorX_MouseDown(object sender, MouseEventArgs e)
        {
            tbAnchorX.SelectAll();
        }

        private void tbAnchorY_MouseDown(object sender, MouseEventArgs e)
        {
            tbAnchorY.SelectAll();
        }

        private void importTilesetReplaceToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (!m_bAddTileset && DialogResult.Yes == MessageBox.Show(this, "Are you sure you want to remove the " + m_strTilesetFilenames[m_nCurrTilesetIndex] + " tile set?",
                    "Replace tile set...", MessageBoxButtons.YesNoCancel, MessageBoxIcon.Warning))
            {
                if (m_impTsDlg == null)
                {
                    m_tsTileset[m_nCurrTilesetIndex] = null;
                    tabControl1.TabPages.RemoveAt(m_nCurrTilesetIndex+1);
                    m_bJustClick = true;
                    tabControl1.SelectedIndex = tabControl1.TabCount - 1;
                    if (tabControl1.TabCount > 0 && m_tsTileset[0] == null)
                    {
                        m_nCurrTilesetIndex = 1;
                    }
                    m_bDontDraw = true;
                    m_impTsDlg = new ImportTilesetDlg();
                    m_impTsDlg.Text = "Replace Tileset...";

                    m_impTsDlg.FormClosing += new FormClosingEventHandler(impTsDlg_Close);
                    m_impTsDlg.createPushed += new EventHandler(impTsDlg_CreatePushed);
                    m_impTsDlg.Show(this);
                }
            }
        }

        private void Form1_Activated(object sender, EventArgs e)
        {
            m_bDontDraw = false;

        }

        private void Form1_ResizeEnd(object sender, EventArgs e)
        {
            m_bDontDraw = false;
        }

        private void Form1_LocationChanged(object sender, EventArgs e)
        {
            m_bDontDraw = false;

        }
    }
}
