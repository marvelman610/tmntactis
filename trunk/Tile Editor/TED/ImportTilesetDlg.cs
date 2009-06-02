//  [5/27/2009 Ramon Johannessen (RJ)]

/// <summary>
///  File : ImportTilesetDlg.cs
///  Purpose : This dialog is displayed for the user to choose
/// a tileset image to load and to set the properties
/// of how that tileset will be used, such as width, height, keycolor, etc...
/// </summary>
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
using ColorPicker;

namespace test
{
    public partial class ImportTilesetDlg : Form
    {
        ColorPickerDialog clrPicker;
        public ImportTilesetDlg()
        {
            InitializeComponent();
            btnKeyClr.Enabled = false;
        }
        // the new tileset
        CTileset m_ts;
        public CTileset Ts
        {
            get { return m_ts; }
            set { m_ts = value; }
        }

        string m_strFileName = null;
        public string StrFileName
        {
            get { return m_strFileName; }
            set { m_strFileName = value; }
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
        int m_nImageID = -1;
        public int NImageID
        {
            get { return m_nImageID; }
            set { m_nImageID = value; }
        }
        Color m_clrKey = Color.White;
        public System.Drawing.Color ClrKey
        {
            get { return m_clrKey; }
            set { m_clrKey = value; }
        }
        // scale for preview image
        float scaleX;
        float scaleY;
        float thisScaleX;
        float thisScaleY;

        // handle event when user pushes "create tile set"
        public event EventHandler createPushed;
        public event EventHandler cancelPushed;

        ManagedTextureManager mTM;
        ManagedDirect3D mD3d;

        private void btnOpenImage_Click(object sender, EventArgs e)
        {
            OpenFileDialog ofd = new OpenFileDialog();
            ofd.Filter = "All Files|*.*|Bitmap Files|*.bmp";
            ofd.FileName = ".bmp";

            if (DialogResult.OK == ofd.ShowDialog())
            {
                string ext = Path.GetExtension(ofd.FileName);

                if (ext == ".bmp" || ext == ".jpg" || ext == ".png" || ext == ".tiff")
                {
                    m_strFileName = ofd.FileName;
                    mTM = ManagedTextureManager.Instance;
                    mD3d = ManagedDirect3D.Instance;

                    m_nImageID = mTM.LoadTexture(m_strFileName, Color.FromArgb (m_clrKey.R, m_clrKey.G, m_clrKey.B).ToArgb ());
                    nudImageWidth.Value = (Decimal)mTM.GetTextureWidth(m_nImageID);
                    nudImageHeight.Value = (Decimal)mTM.GetTextureHeight(m_nImageID);
                    toolStripStatusLabel1.Text = "Image : Loaded -- " + m_strFileName;
                    scaleX = 494f / (float)(mTM.GetTextureWidth(m_nImageID));
                    scaleY = 424f / (float)(mTM.GetTextureHeight(m_nImageID));
                    thisScaleX = panel1.Width / (float)(mTM.GetTextureWidth(m_nImageID));
                    thisScaleY = panel1.Height / (float)(mTM.GetTextureHeight(m_nImageID));
                    btnKeyClr.Enabled = true;
                } 
            }
        }

        private void btnCreateTileset_Click(object sender, EventArgs e)
        {
            if (m_strFileName != null)
            {
                NCellWidth = (int)nudCellWidth.Value;
                NCellHeight = (int)nudCellHeight.Value;
                if (NCellWidth >= nudCellWidth.Minimum && NCellWidth <= nudCellWidth.Maximum &&
                    NCellHeight >= nudCellHeight.Minimum && m_nCellHeight <= nudCellHeight.Maximum)
                {
                    m_ts = new CTileset(m_nImageID, m_clrKey,
                                    (int)nudImageWidth.Value,
                                    (int)nudImageHeight.Value,
                                    NCellWidth, NCellHeight, m_strFileName);
                    if (createPushed != null)
                        createPushed(this, new EventArgs());
                }
            } 
            else
            {
                MessageBox.Show("You have not loaded an image", "Please load an image", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private void btnCancel_Click(object sender, EventArgs e)
        {
            if (cancelPushed != null)
                cancelPushed(this, new EventArgs());
            this.Close();
        }

        private void btnKeyClr_Click(object sender, EventArgs e)
        {
            btnKeyClr.BackColor = m_clrKey;
            clrPicker = new ColorPickerDialog(m_nImageID, scaleX, scaleY);
            clrPicker.OKpushed += new EventHandler(CP_OKpushed);
            if (DialogResult.OK == clrPicker.ShowDialog())
            {
                mTM.ReleaseTexture(m_nImageID);
                m_nImageID = mTM.LoadTexture(m_strFileName, Color.FromArgb(m_clrKey.R, m_clrKey.G, m_clrKey.B).ToArgb());
                panel1.Invalidate();
            }
        }

        void CP_OKpushed(object sender, EventArgs e)
        {
            m_clrKey = clrPicker.CP.EyedropColorPicker.SelectedColor;
            btnKeyClr.BackColor = m_clrKey;
            m_nImageID = clrPicker.CP.ImageID;
            mTM.ReleaseTexture(m_nImageID);
            m_nImageID = mTM.LoadTexture(m_strFileName, Color.FromArgb(m_clrKey.R, m_clrKey.G, m_clrKey.B).ToArgb());
        }

        private void panel1_Paint(object sender, PaintEventArgs e)
        {
            if (m_nImageID != -1)
            {
                mD3d.ChangeDisplayParam(this.panel1, false);
                mD3d.DeviceBegin();
                mD3d.Clear(255, 255, 255);
                mD3d.SpriteBegin();
                mTM.Draw(m_nImageID, 0, 0, thisScaleX, thisScaleY, Rectangle.Empty, 0, 0, 0.0f, m_clrKey.ToArgb());
                mD3d.SpriteEnd();
                mD3d.DeviceEnd();
                mD3d.Present();
            }

        }
    }
}
