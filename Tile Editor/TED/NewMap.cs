//  [5/27/2009 Ramon Johannessen (RJ)]

/// <summary>
///  File : NewMap.cs
///  Purpose : The dialog box for creating a new map
/// </summary>
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

enum IsoType {ISO_DIAMOND, ISO_STAG, ISO_SLIDE, RECT}

namespace test
{
    public partial class NewMap : Form
    {
        public NewMap()
        {
            InitializeComponent();
            cbIsoType.SelectedIndex = 0;
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
        int nIsoType;
        public int NIsoType
        {
            get { return nIsoType; }
            set { nIsoType = value; }
        }
        public event EventHandler createPushed;

        private void btnCreate_Click(object sender, EventArgs e)
        {
            if (!cbIsometric.Checked)
            {
	            if (tbCellsize.Text.Length > 0 && tbCellsize.Text != "0" && 
                    tbColumns.Text.Length > 0 && tbColumns.Text != "0" && 
                    tbRows.Text.Length > 0 && tbRows.Text != "0")
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
                    MessageBox.Show("All fields must contain non-negative values", "For Normal Maps...", MessageBoxButtons.OK, MessageBoxIcon.Error);
                }
            }
            else if (cbIsometric.Checked)
            {
                switch (cbIsoType.SelectedIndex)
                {
                    case (int)IsoType.ISO_DIAMOND:
                        {
                            if (tbCellsize.Text.Length > 0  && tbCellsize.Text != "0" && 
                                tbColumns.Text.Length > 0   && tbColumns.Text != "0" && 
                                tbRows.Text.Length > 0      && tbRows.Text != "0" && 
                                tbIsoHeight.Text.Length > 0 && tbIsoHeight.Text != "0" && 
                                tbIsoWidth.Text.Length > 0  && tbIsoWidth.Text != "0" &&
                                tbColumns.Text == tbRows.Text)
                            {
                                nIsoWidth   = int.Parse(tbIsoWidth.Text);
                                nIsoHeight  = int.Parse(tbIsoHeight.Text);
                                nColumns    = int.Parse(tbColumns.Text);
                                nRows       = int.Parse(tbRows.Text);
                                bIsIsometric= cbIsometric.Checked;
                                nIsoType    = cbIsoType.SelectedIndex;

                                if (createPushed != null)
                                    createPushed(this, new EventArgs());
                            }
                            else
                            {
                                MessageBox.Show("All fields must contain non-negative values, and the number of columns and rows must be the same.", 
                                    "For Diamond Maps...", MessageBoxButtons.OK, MessageBoxIcon.Error);
                            }
                            break;
                        }
                    case (int)IsoType.ISO_STAG:
                        {
                            if (tbCellsize.Text.Length > 0  && tbCellsize.Text != "0" && 
                                tbColumns.Text.Length > 0   && tbColumns.Text != "0" && 
                                tbRows.Text.Length > 0      && tbRows.Text != "0" && 
                                tbIsoHeight.Text.Length > 0 && tbIsoHeight.Text != "0" && 
                                tbIsoWidth.Text.Length > 0  && tbIsoWidth.Text != "0")
                            {
                                nIsoWidth = int.Parse(tbIsoWidth.Text);
                                nIsoHeight = int.Parse(tbIsoHeight.Text);
                                nColumns = int.Parse(tbColumns.Text);
                                nRows = int.Parse(tbRows.Text);
                                bIsIsometric = cbIsometric.Checked;
                                nIsoType = cbIsoType.SelectedIndex;

                                if (createPushed != null)
                                    createPushed(this, new EventArgs());
                            }
                            else
                            {
                                MessageBox.Show("All fields must contain non-negative values.", "For Staggered Maps...", MessageBoxButtons.OK, MessageBoxIcon.Error);
                            }
                            break;
                        }
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
            cbIsoType.Enabled = !cbIsoType.Enabled;
        }

        // check to make sure the entered text is a numeric value
        private void tbColumns_TextChanged(object sender, EventArgs e)
        {
            if (tbColumns.Text.Length > 0)
            {
                int index = tbColumns.Text.Length-1;
                if (tbColumns.Text[index] < 48 || tbColumns.Text[index] > 57)
                {
                    tbColumns.Text = tbColumns.Text.Remove(index);
                }
            }
        }

        private void tbRows_TextChanged(object sender, EventArgs e)
        {
            if (tbRows.Text.Length > 0)
            {
                int index = tbRows.Text.Length - 1;
                if (tbRows.Text[index] < 48 || tbRows.Text[index] > 57) 
                    tbRows.Text = tbRows.Text.Remove(index);
            }
        }

        private void tbIsoHeight_TextChanged(object sender, EventArgs e)
        {
            if (tbIsoHeight.Text.Length > 0)
            {
                int index = tbIsoHeight.Text.Length - 1;
                if (tbIsoHeight.Text[index] < 48 || tbIsoHeight.Text[index] > 57)
                    tbIsoHeight.Text = tbIsoHeight.Text.Remove(index);
            }
        }

        private void tbIsoWidth_TextChanged(object sender, EventArgs e)
        {
            if (tbIsoWidth.Text.Length > 0)
            {
                int index = tbIsoWidth.Text.Length - 1;
                if (tbIsoWidth.Text[index] < 48 || tbIsoWidth.Text[index] > 57)
                    tbIsoWidth.Text = tbIsoWidth.Text.Remove(index);
            }
        }

        private void tbCellsize_TextChanged(object sender, EventArgs e)
        {
            if (tbCellsize.Text.Length > 0)
            {
                int index = tbCellsize.Text.Length - 1;
                if (tbCellsize.Text[index] < 48 || tbCellsize.Text[index] > 57)
                    tbCellsize.Text = tbCellsize.Text.Remove(index);
            }
        }

    }
}
