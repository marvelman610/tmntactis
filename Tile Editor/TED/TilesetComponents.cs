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
    /// <summary>
    /// 
    /// </summary>
    class TilesetComponents
    {
        ToolTip toolTip1;
        public NumericUpDown nudTilesetGridHeight = new NumericUpDown();
        public NumericUpDown nudTilesetGridWidth = new NumericUpDown();
        public NumericUpDown nudCellWidth = new NumericUpDown();
        public NumericUpDown nudCellHeight = new NumericUpDown();
        public NumericUpDown nudImageWidth = new NumericUpDown();
        public NumericUpDown nudImageHeight = new NumericUpDown();
        public ComboBox cbTileFlag = new ComboBox();

        public Label lblRows = new Label();
        public Label lblCols = new Label();
        public Label lblTileWidth = new Label();
        public Label lblTileHeight = new Label();
        public Label lblTileFlag = new Label();
        public Label lblImageWidth = new Label();
        public Label lblImageHeight = new Label();

        public GroupBox groupBox1 = new GroupBox();
        public GroupBox groupBox2 = new GroupBox();

        public void AddAll(TabPage tp, ToolTip tt, Form1 mainForm)
        {
            toolTip1 = tt;
            tp.Controls.Add(this.lblRows);
            tp.Controls.Add(this.nudImageHeight);
            tp.Controls.Add(this.nudImageWidth);
            tp.Controls.Add(this.lblImageHeight);
            tp.Controls.Add(this.lblImageWidth);
            tp.Controls.Add(this.groupBox1);
            tp.Controls.Add(this.groupBox2);
            tp.Controls.Add(this.nudCellHeight);
            tp.Controls.Add(this.nudCellWidth);
            tp.Controls.Add(this.cbTileFlag);
            tp.Controls.Add(this.lblTileFlag);
            tp.Controls.Add(this.nudTilesetGridHeight);
            tp.Controls.Add(this.lblTileHeight);
            tp.Controls.Add(this.lblTileWidth);
            tp.Controls.Add(this.nudTilesetGridWidth);
            tp.Controls.Add(this.lblCols);

            SetValues(mainForm);
        }

        private void SetValues(Form1 mainForm)
        {
//             ((System.ComponentModel.ISupportInitialize)(this.nudImageHeight)).BeginInit();
//             ((System.ComponentModel.ISupportInitialize)(this.nudImageWidth)).BeginInit();
//             this.groupBox1.SuspendLayout();
//             ((System.ComponentModel.ISupportInitialize)(this.nudCellHeight)).BeginInit();
//             ((System.ComponentModel.ISupportInitialize)(this.nudCellWidth)).BeginInit();
//             ((System.ComponentModel.ISupportInitialize)(this.nudTilesetGridHeight)).BeginInit();
//             ((System.ComponentModel.ISupportInitialize)(this.nudTilesetGridWidth)).BeginInit();

            lblRows.Text = "Rows";
            lblCols.Text = "Columns";
            lblTileWidth.Text = "Tile Width";
            lblTileHeight.Text = "Tile Height";
            lblTileFlag.Text = "Tile Flag";
            lblImageWidth.Text = "Image Width";
            lblImageHeight.Text = "Image Height";

            lblRows.Location = new Point(7, 59);
            lblCols.Location = new Point(4, 15);
            lblTileWidth.Location = new Point(78, 16);
            lblTileHeight.Location = new Point(139, 16);
            lblImageHeight.Location = new Point(228, 61);
            lblImageWidth.Location = new Point(228, 19);

            this.lblRows.AutoSize = true;
            this.lblRows.Location = new System.Drawing.Point(7, 59);
            this.lblRows.Name = "lblRows";
            this.lblRows.Size = new System.Drawing.Size(34, 13);
            this.lblRows.TabIndex = 7;
            this.lblRows.Text = "Rows";
            this.toolTip1.SetToolTip(this.lblRows, "Number of rows in the tileset grid.");
            // 
            // nudImageHeight
            // 
            this.nudImageHeight.Location = new System.Drawing.Point(231, 77);
            this.nudImageHeight.Maximum = new decimal(new int[] {
            2048,
            0,
            0,
            0});
            this.nudImageHeight.Name = "nudImageHeight";
            this.nudImageHeight.Size = new System.Drawing.Size(64, 20);
            this.nudImageHeight.TabIndex = 5;
            // 
            // nudImageWidth
            // 
            this.nudImageWidth.Location = new System.Drawing.Point(231, 36);
            this.nudImageWidth.Maximum = new decimal(new int[] {
            2048,
            0,
            0,
            0});
            this.nudImageWidth.Name = "nudImageWidth";
            this.nudImageWidth.Size = new System.Drawing.Size(64, 20);
            this.nudImageWidth.TabIndex = 4;
            // 
            // lblImageHeight
            // 
            this.lblImageHeight.AutoSize = true;
            this.lblImageHeight.Location = new System.Drawing.Point(228, 61);
            this.lblImageHeight.Name = "lblImageHeight";
            this.lblImageHeight.Size = new System.Drawing.Size(70, 13);
            this.lblImageHeight.TabIndex = 2;
            this.lblImageHeight.Text = "Image Height";
            // 
            // lblImageWidth
            // 
            this.lblImageWidth.AutoSize = true;
            this.lblImageWidth.Location = new System.Drawing.Point(228, 19);
            this.lblImageWidth.Name = "lblImageWidth";
            this.lblImageWidth.Size = new System.Drawing.Size(67, 13);
            this.lblImageWidth.TabIndex = 1;
            this.lblImageWidth.Text = "Image Width";
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.nudCellHeight);
            this.groupBox1.Controls.Add(this.nudCellWidth);
            this.groupBox1.Controls.Add(this.cbTileFlag);
            this.groupBox1.Controls.Add(this.lblTileFlag);
            this.groupBox1.Controls.Add(this.nudTilesetGridHeight);
            this.groupBox1.Controls.Add(this.lblTileHeight);
            this.groupBox1.Controls.Add(this.lblTileWidth);
            this.groupBox1.Controls.Add(this.nudTilesetGridWidth);
            this.groupBox1.Controls.Add(this.lblCols);
            this.groupBox1.Location = new System.Drawing.Point(3, 2);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(208, 103);
            this.groupBox1.TabIndex = 10;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Grid Properties";
            // 
            // nudCellHeight
            // 
            this.nudCellHeight.Location = new System.Drawing.Point(142, 32);
            this.nudCellHeight.Maximum = new decimal(new int[] {
            256,
            0,
            0,
            0});
            this.nudCellHeight.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.nudCellHeight.Name = "nudCellHeight";
            this.nudCellHeight.Size = new System.Drawing.Size(55, 20);
            this.nudCellHeight.TabIndex = 13;
            this.nudCellHeight.Value = new decimal(new int[] {
            32,
            0,
            0,
            0});
            this.nudCellHeight.ValueChanged += new System.EventHandler(mainForm.nudCellHeight_ValueChanged);
            // 
            // nudCellWidth
            // 
            this.nudCellWidth.Location = new System.Drawing.Point(84, 32);
            this.nudCellWidth.Maximum = new decimal(new int[] {
            256,
            0,
            0,
            0});
            this.nudCellWidth.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.nudCellWidth.Name = "nudCellWidth";
            this.nudCellWidth.Size = new System.Drawing.Size(49, 20);
            this.nudCellWidth.TabIndex = 12;
            this.nudCellWidth.Value = new decimal(new int[] {
            32,
            0,
            0,
            0});
            this.nudCellWidth.ValueChanged += new System.EventHandler(mainForm.nudCellWidth_ValueChanged);
            // 
            // cbTileFlag
            // 
            this.cbTileFlag.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cbTileFlag.FormattingEnabled = true;
            this.cbTileFlag.Location = new System.Drawing.Point(84, 75);
            this.cbTileFlag.Name = "cbTileFlag";
            this.cbTileFlag.Size = new System.Drawing.Size(113, 21);
            this.cbTileFlag.TabIndex = 11;
            this.toolTip1.SetToolTip(this.lblTileFlag, "Set flag of currently selected tile (0, 1, 2, 3).\r\n");
            this.cbTileFlag.SelectedIndexChanged += new System.EventHandler(mainForm.comboBox1_SelectedIndexChanged);
            // 
            // lblTileFlag
            // 
            this.lblTileFlag.AutoSize = true;
            this.lblTileFlag.Location = new System.Drawing.Point(81, 56);
            this.lblTileFlag.Name = "lblTileFlag";
            this.lblTileFlag.Size = new System.Drawing.Size(47, 13);
            this.lblTileFlag.TabIndex = 10;
            this.lblTileFlag.Text = "Tile Flag";
            // 
            // nudTilesetGridHeight
            // 
            this.nudTilesetGridHeight.Location = new System.Drawing.Point(7, 76);
            this.nudTilesetGridHeight.Maximum = new decimal(new int[] {
            256,
            0,
            0,
            0});
            this.nudTilesetGridHeight.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.nudTilesetGridHeight.Name = "nudTilesetGridHeight";
            this.nudTilesetGridHeight.Size = new System.Drawing.Size(63, 20);
            this.nudTilesetGridHeight.TabIndex = 9;
            this.nudTilesetGridHeight.Value = new decimal(new int[] {
            32,
            0,
            0,
            0});
            this.nudTilesetGridHeight.ValueChanged += new System.EventHandler(mainForm.nudTilesetHeight_ValueChanged);
            // 
            // lblTileHeight
            // 
            this.lblTileHeight.AutoSize = true;
            this.lblTileHeight.Location = new System.Drawing.Point(139, 16);
            this.lblTileHeight.Name = "lblTileHeight";
            this.lblTileHeight.Size = new System.Drawing.Size(58, 13);
            this.lblTileHeight.TabIndex = 0;
            this.lblTileHeight.Text = "Tile Height";
            this.toolTip1.SetToolTip(this.lblTileHeight, "Individual cell height of the tileset tiles.");
            // 
            // lblTileWidth
            // 
            this.lblTileWidth.AutoSize = true;
            this.lblTileWidth.Location = new System.Drawing.Point(78, 16);
            this.lblTileWidth.Name = "lblTileWidth";
            this.lblTileWidth.Size = new System.Drawing.Size(55, 13);
            this.lblTileWidth.TabIndex = 0;
            this.lblTileWidth.Text = "Tile Width";
            this.toolTip1.SetToolTip(this.lblTileWidth, "Individual cell height of the tileset tiles.");
            // 
            // nudTilesetGridWidth
            // 
            this.nudTilesetGridWidth.Location = new System.Drawing.Point(7, 32);
            this.nudTilesetGridWidth.Maximum = new decimal(new int[] {
            256,
            0,
            0,
            0});
            this.nudTilesetGridWidth.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.nudTilesetGridWidth.Name = "nudTilesetGridWidth";
            this.nudTilesetGridWidth.Size = new System.Drawing.Size(63, 20);
            this.nudTilesetGridWidth.TabIndex = 8;
            this.nudTilesetGridWidth.Value = new decimal(new int[] {
            32,
            0,
            0,
            0});
            this.nudTilesetGridWidth.ValueChanged += new System.EventHandler(mainForm.nudTilesetWidth_ValueChanged);
            // 
            // lblCols
            // 
            this.lblCols.AutoSize = true;
            this.lblCols.Location = new System.Drawing.Point(4, 15);
            this.lblCols.Name = "lblCols";
            this.lblCols.Size = new System.Drawing.Size(47, 13);
            this.lblCols.TabIndex = 6;
            this.lblCols.Tag = "";
            this.lblCols.Text = "Columns";
            this.toolTip1.SetToolTip(this.lblCols, "Number of columns in the tileset grid.");
            // 
            // groupBox2
            // 
            this.groupBox2.Location = new System.Drawing.Point(216, 3);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(102, 102);
            this.groupBox2.TabIndex = 11;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Image Properties";

            nudImageWidth.Enabled = false;
            nudImageHeight.Enabled = false;
        }
    }

}
