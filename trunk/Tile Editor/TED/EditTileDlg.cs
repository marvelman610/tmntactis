using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using tile;

namespace test
{
    public partial class EditTileDlg : Form
    {
        public event EventHandler acceptPushed;

        public event EventHandler flagChanged;
        public event EventHandler posXchanged;
        public event EventHandler posYchanged;
        public event EventHandler scaleXchanged;
        public event EventHandler scaleYchanged;
        public event EventHandler sourceXchanged;
        public event EventHandler sourceYchanged;
        public event EventHandler srcWidthchanged;
        public event EventHandler srcHeightchanged;
        public event EventHandler poiXchanged;
        public event EventHandler poiYchanged;
        public event EventHandler rotChanged;

        int m_tileID;
        public int TileID
        {
            get { return m_tileID; }
            set { m_tileID = value; }
        }

        public EditTileDlg(int tileID, CFREETILE freeTile, string f0, string f1, string f2, string f3, string f4, string f5)
        {
            InitializeComponent();
            cbTileFlag.Items.Add(f0);
            cbTileFlag.Items.Add(f1);
            cbTileFlag.Items.Add(f2);
            cbTileFlag.Items.Add(f3);
            cbTileFlag.Items.Add(f4);
            cbTileFlag.Items.Add(f5);
            cbTileFlag.SelectedIndex = freeTile.NTileFlag;

            m_tileID = tileID;
            cbTileFlag.Text = freeTile.NTileFlag.ToString();
            nudPosX.Value = freeTile.MapPt.X;
            nudPosY.Value = freeTile.MapPt.Y;
            nudScaleX.Value = 1;
            nudScaleY.Value = 1;
            nudSourceX.Value = freeTile.SourceRect.Left;
            nudSourceY.Value = freeTile.SourceRect.Top;
            nudSourceWidth.Value = freeTile.SourceRect.Width;
            nudSourceHeight.Value = freeTile.SourceRect.Height;
            nudPOIX.Value = freeTile.AnchorX;
            nudPOIY.Value = freeTile.AnchorY;
            nudRot.Value = (decimal)freeTile.Rotation;
        }

        private void btnAccept_Click(object sender, EventArgs e)
        {
            if (acceptPushed != null)
                acceptPushed(this, new EventArgs());
        }

        private void btnCancel_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        private void nudPosX_ValueChanged(object sender, EventArgs e)
        {
            if (posXchanged != null)
                posXchanged(this, new EventArgs());
        }

        private void nudPosY_ValueChanged(object sender, EventArgs e)
        {
            if (posYchanged != null)
                posYchanged(this, new EventArgs());

        }

        private void nudRot_ValueChanged(object sender, EventArgs e)
        {
            if (rotChanged != null)
                rotChanged(this, new EventArgs());
        }

        private void nudPOIX_ValueChanged(object sender, EventArgs e)
        {
            if (poiXchanged != null)
                poiXchanged(this, new EventArgs());
        }

        private void nudPOIY_ValueChanged(object sender, EventArgs e)
        {
            if ( poiYchanged != null)
                poiYchanged(this, new EventArgs());
        }

        private void nudScaleX_ValueChanged(object sender, EventArgs e)
        {
            if ( scaleXchanged != null)
                scaleXchanged(this, new EventArgs());
        }

        private void nudScaleY_ValueChanged(object sender, EventArgs e)
        {
            if ( scaleYchanged != null)
                scaleYchanged(this, new EventArgs());

        }

        private void nudSourceX_ValueChanged(object sender, EventArgs e)
        {
            if ( sourceXchanged != null)
                sourceXchanged(this, new EventArgs());
        }

        private void nudSourceY_ValueChanged(object sender, EventArgs e)
        {
            if ( sourceYchanged != null)
                sourceYchanged(this, new EventArgs());

        }

        private void nudSourceWidth_ValueChanged(object sender, EventArgs e)
        {
            if ( srcWidthchanged != null)
                srcWidthchanged(this, new EventArgs());
        }

        private void nudSourceHeight_ValueChanged(object sender, EventArgs e)
        {
            if ( srcHeightchanged != null)
                srcHeightchanged(this, new EventArgs());
        }

        private void cbTileFlag_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (flagChanged != null)
                flagChanged(this, new EventArgs());
        }
    }
}
