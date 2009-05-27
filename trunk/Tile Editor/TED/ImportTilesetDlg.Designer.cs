namespace test
{
    partial class ImportTilesetDlg
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.btnOpenImage = new System.Windows.Forms.Button();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.nudCellHeight = new System.Windows.Forms.NumericUpDown();
            this.nudCellWidth = new System.Windows.Forms.NumericUpDown();
            this.label4 = new System.Windows.Forms.Label();
            this.btnKeyClr = new System.Windows.Forms.Button();
            this.nudImageHeight = new System.Windows.Forms.NumericUpDown();
            this.label3 = new System.Windows.Forms.Label();
            this.nudImageWidth = new System.Windows.Forms.NumericUpDown();
            this.label2 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.btnCreateTileset = new System.Windows.Forms.Button();
            this.btnCancel = new System.Windows.Forms.Button();
            this.statusStrip1 = new System.Windows.Forms.StatusStrip();
            this.toolStripStatusLabel1 = new System.Windows.Forms.ToolStripStatusLabel();
            this.panel1 = new System.Windows.Forms.Panel();
            this.label6 = new System.Windows.Forms.Label();
            this.groupBox1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.nudCellHeight)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.nudCellWidth)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.nudImageHeight)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.nudImageWidth)).BeginInit();
            this.statusStrip1.SuspendLayout();
            this.SuspendLayout();
            // 
            // btnOpenImage
            // 
            this.btnOpenImage.Location = new System.Drawing.Point(174, 85);
            this.btnOpenImage.Name = "btnOpenImage";
            this.btnOpenImage.Size = new System.Drawing.Size(99, 23);
            this.btnOpenImage.TabIndex = 0;
            this.btnOpenImage.Text = "Import Image";
            this.btnOpenImage.UseVisualStyleBackColor = true;
            this.btnOpenImage.Click += new System.EventHandler(this.btnOpenImage_Click);
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.nudCellHeight);
            this.groupBox1.Controls.Add(this.nudCellWidth);
            this.groupBox1.Controls.Add(this.label4);
            this.groupBox1.Controls.Add(this.btnKeyClr);
            this.groupBox1.Controls.Add(this.nudImageHeight);
            this.groupBox1.Controls.Add(this.label3);
            this.groupBox1.Controls.Add(this.nudImageWidth);
            this.groupBox1.Controls.Add(this.label2);
            this.groupBox1.Controls.Add(this.label5);
            this.groupBox1.Controls.Add(this.label1);
            this.groupBox1.Location = new System.Drawing.Point(12, 12);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(135, 210);
            this.groupBox1.TabIndex = 1;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Tileset Properties";
            // 
            // nudCellHeight
            // 
            this.nudCellHeight.Location = new System.Drawing.Point(70, 33);
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
            this.nudCellHeight.TabIndex = 8;
            this.nudCellHeight.Value = new decimal(new int[] {
            32,
            0,
            0,
            0});
            // 
            // nudCellWidth
            // 
            this.nudCellWidth.Location = new System.Drawing.Point(7, 33);
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
            this.nudCellWidth.Size = new System.Drawing.Size(54, 20);
            this.nudCellWidth.TabIndex = 7;
            this.nudCellWidth.Value = new decimal(new int[] {
            32,
            0,
            0,
            0});
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(41, 153);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(52, 13);
            this.label4.TabIndex = 6;
            this.label4.Text = "Key Color";
            // 
            // btnKeyClr
            // 
            this.btnKeyClr.BackColor = System.Drawing.SystemColors.ActiveCaptionText;
            this.btnKeyClr.ForeColor = System.Drawing.Color.CornflowerBlue;
            this.btnKeyClr.Location = new System.Drawing.Point(31, 174);
            this.btnKeyClr.Name = "btnKeyClr";
            this.btnKeyClr.Size = new System.Drawing.Size(75, 23);
            this.btnKeyClr.TabIndex = 2;
            this.btnKeyClr.UseVisualStyleBackColor = false;
            this.btnKeyClr.Click += new System.EventHandler(this.btnKeyClr_Click);
            // 
            // nudImageHeight
            // 
            this.nudImageHeight.Enabled = false;
            this.nudImageHeight.Location = new System.Drawing.Point(33, 120);
            this.nudImageHeight.Maximum = new decimal(new int[] {
            2048,
            0,
            0,
            0});
            this.nudImageHeight.Name = "nudImageHeight";
            this.nudImageHeight.Size = new System.Drawing.Size(63, 20);
            this.nudImageHeight.TabIndex = 5;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(33, 104);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(70, 13);
            this.label3.TabIndex = 4;
            this.label3.Text = "Image Height";
            // 
            // nudImageWidth
            // 
            this.nudImageWidth.Enabled = false;
            this.nudImageWidth.Location = new System.Drawing.Point(33, 76);
            this.nudImageWidth.Maximum = new decimal(new int[] {
            2048,
            0,
            0,
            0});
            this.nudImageWidth.Name = "nudImageWidth";
            this.nudImageWidth.Size = new System.Drawing.Size(63, 20);
            this.nudImageWidth.TabIndex = 3;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(33, 60);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(67, 13);
            this.label2.TabIndex = 2;
            this.label2.Text = "Image Width";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(67, 16);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(58, 13);
            this.label5.TabIndex = 0;
            this.label5.Text = "Cell Height";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(6, 16);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(55, 13);
            this.label1.TabIndex = 0;
            this.label1.Text = "Cell Width";
            // 
            // btnCreateTileset
            // 
            this.btnCreateTileset.Location = new System.Drawing.Point(174, 123);
            this.btnCreateTileset.Name = "btnCreateTileset";
            this.btnCreateTileset.Size = new System.Drawing.Size(99, 46);
            this.btnCreateTileset.TabIndex = 2;
            this.btnCreateTileset.Text = "Create Tileset";
            this.btnCreateTileset.UseVisualStyleBackColor = true;
            this.btnCreateTileset.Click += new System.EventHandler(this.btnCreateTileset_Click);
            // 
            // btnCancel
            // 
            this.btnCancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.btnCancel.Location = new System.Drawing.Point(186, 186);
            this.btnCancel.Name = "btnCancel";
            this.btnCancel.Size = new System.Drawing.Size(75, 23);
            this.btnCancel.TabIndex = 3;
            this.btnCancel.Text = "Cancel";
            this.btnCancel.UseVisualStyleBackColor = true;
            this.btnCancel.Click += new System.EventHandler(this.btnCancel_Click);
            // 
            // statusStrip1
            // 
            this.statusStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripStatusLabel1});
            this.statusStrip1.Location = new System.Drawing.Point(0, 230);
            this.statusStrip1.Name = "statusStrip1";
            this.statusStrip1.Size = new System.Drawing.Size(511, 22);
            this.statusStrip1.TabIndex = 5;
            this.statusStrip1.Text = "statusStrip1";
            // 
            // toolStripStatusLabel1
            // 
            this.toolStripStatusLabel1.Name = "toolStripStatusLabel1";
            this.toolStripStatusLabel1.Size = new System.Drawing.Size(107, 17);
            this.toolStripStatusLabel1.Text = "Image : None loaded";
            // 
            // panel1
            // 
            this.panel1.Location = new System.Drawing.Point(300, 27);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(200, 191);
            this.panel1.TabIndex = 9;
            this.panel1.Paint += new System.Windows.Forms.PaintEventHandler(this.panel1_Paint);
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(300, 8);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(77, 13);
            this.label6.TabIndex = 10;
            this.label6.Text = "Image Preview";
            // 
            // ImportTilesetDlg
            // 
            this.AcceptButton = this.btnCreateTileset;
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.CancelButton = this.btnCancel;
            this.ClientSize = new System.Drawing.Size(511, 252);
            this.Controls.Add(this.label6);
            this.Controls.Add(this.panel1);
            this.Controls.Add(this.statusStrip1);
            this.Controls.Add(this.btnCancel);
            this.Controls.Add(this.btnCreateTileset);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.btnOpenImage);
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "ImportTilesetDlg";
            this.ShowIcon = false;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Create New Tileset From Image";
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.nudCellHeight)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.nudCellWidth)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.nudImageHeight)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.nudImageWidth)).EndInit();
            this.statusStrip1.ResumeLayout(false);
            this.statusStrip1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button btnOpenImage;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.NumericUpDown nudImageHeight;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.NumericUpDown nudImageWidth;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Button btnKeyClr;
        private System.Windows.Forms.Button btnCreateTileset;
        private System.Windows.Forms.Button btnCancel;
        private System.Windows.Forms.StatusStrip statusStrip1;
        private System.Windows.Forms.ToolStripStatusLabel toolStripStatusLabel1;
        private System.Windows.Forms.NumericUpDown nudCellHeight;
        private System.Windows.Forms.NumericUpDown nudCellWidth;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.Label label6;

    }
}