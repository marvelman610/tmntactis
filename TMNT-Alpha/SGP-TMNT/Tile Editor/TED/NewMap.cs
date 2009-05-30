using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace test
{
    public partial class NewMap : Form
    {
        public NewMap()
        {
            InitializeComponent();
        }

        int nCellSize;
        public int NCellSize
        {
            get { return nCellSize; }
            set { nCellSize = value; }
        }
        int nColumns;
        public int NColumns
        {
            get { return nColumns; }
            set { nColumns = value; }
        }
        int nRows;
        public int NRows
        {
            get { return nRows; }
            set { nRows = value; }
        }
        bool bIsIsometric;
        public bool BIsIsometric
        {
            get { return bIsIsometric; }
            set { bIsIsometric = value; }
        }
        int nIsoWidth;
        public int NIsoWidth
        {
            get { return nIsoWidth; }
            set { nIsoWidth = value; }
        }
        int nIsoHeight;
        public int NIsoHeight
        {
            get { return nIsoHeight; }
            set { nIsoHeight = value; }
        }

        public event EventHandler createPushed;

        private void btnCreate_Click(object sender, EventArgs e)
        {
            if (!cbIsometric.Checked)
            {
	            if (tbCellsize.Text != "0" && tbColumns.Text != "0" && tbRows.Text != "0")
	            {
		            nCellSize   = int.Parse(tbCellsize.Text);
		            nColumns    = int.Parse(tbColumns.Text);
		            nRows       = int.Parse(tbRows.Text);
		            bIsIsometric= cbIsometric.Checked;
		
		            if (createPushed != null)
		                createPushed(this, new EventArgs());
                }
                else
                {
                    MessageBox.Show("All fields must be greater than 0", "For Normal Maps...", MessageBoxButtons.OK, MessageBoxIcon.Error);
                }
            }
            else if (cbIsometric.Checked)
            {
                if (tbCellsize.Text != "0" && tbColumns.Text != "0" && tbRows.Text != "0" && tbIsoHeight.Text != "0" && tbIsoWidth.Text != "0" &&
                    tbColumns.Text == tbRows.Text)
                {
	                nIsoWidth   = int.Parse(tbIsoWidth.Text);
	                nIsoHeight  = int.Parse(tbIsoHeight.Text);
	                nColumns    = int.Parse(tbColumns.Text);
	                nRows       = int.Parse(tbRows.Text);
	                bIsIsometric= cbIsometric.Checked;
	
	                if (createPushed != null)
	                    createPushed(this, new EventArgs());
                }
                else
                {
                    MessageBox.Show("All fields must be greater than 0, and the number of columns and rows must be the same.", "For Isometric Maps...", MessageBoxButtons.OK, MessageBoxIcon.Error);
                }
            }
        }

        private void btnCancel_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        private void cbIsometric_CheckedChanged(object sender, EventArgs e)
        {
            tbIsoHeight.Enabled = !tbIsoHeight.Enabled;
            tbIsoWidth.Enabled  = !tbIsoWidth.Enabled;
            tbCellsize.Enabled = !tbCellsize.Enabled;
        }


    }
}
