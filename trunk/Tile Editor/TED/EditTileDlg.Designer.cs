namespace test
{
    partial class EditTileDlg
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
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.label6 = new System.Windows.Forms.Label();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.nudScaleY = new System.Windows.Forms.NumericUpDown();
            this.nudScaleX = new System.Windows.Forms.NumericUpDown();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.nudRot = new System.Windows.Forms.NumericUpDown();
            this.nudPosY = new System.Windows.Forms.NumericUpDown();
            this.nudPosX = new System.Windows.Forms.NumericUpDown();
            this.cbTileFlag = new System.Windows.Forms.ComboBox();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.label10 = new System.Windows.Forms.Label();
            this.label9 = new System.Windows.Forms.Label();
            this.nudSourceHeight = new System.Windows.Forms.NumericUpDown();
            this.label8 = new System.Windows.Forms.Label();
            this.label7 = new System.Windows.Forms.Label();
            this.nudSourceWidth = new System.Windows.Forms.NumericUpDown();
            this.nudSourceY = new System.Windows.Forms.NumericUpDown();
            this.nudSourceX = new System.Windows.Forms.NumericUpDown();
            this.btnAccept = new System.Windows.Forms.Button();
            this.groupBox4 = new System.Windows.Forms.GroupBox();
            this.nudPOIY = new System.Windows.Forms.NumericUpDown();
            this.nudPOIX = new System.Windows.Forms.NumericUpDown();
            this.label12 = new System.Windows.Forms.Label();
            this.label11 = new System.Windows.Forms.Label();
            this.groupBox1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.nudScaleY)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.nudScaleX)).BeginInit();
            this.groupBox2.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.nudRot)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.nudPosY)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.nudPosX)).BeginInit();
            this.groupBox3.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.nudSourceHeight)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.nudSourceWidth)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.nudSourceY)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.nudSourceX)).BeginInit();
            this.groupBox4.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.nudPOIY)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.nudPOIX)).BeginInit();
            this.SuspendLayout();
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(8, 25);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(14, 13);
            this.label1.TabIndex = 0;
            this.label1.Text = "X";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(8, 47);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(14, 13);
            this.label2.TabIndex = 1;
            this.label2.Text = "Y";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(2, 22);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(44, 13);
            this.label3.TabIndex = 2;
            this.label3.Text = "Scale X";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(2, 50);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(44, 13);
            this.label4.TabIndex = 3;
            this.label4.Text = "Scale Y";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(34, 234);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(33, 13);
            this.label5.TabIndex = 4;
            this.label5.Text = "Flag :";
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(1, 70);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(27, 13);
            this.label6.TabIndex = 5;
            this.label6.Text = "Rot.";
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.nudScaleY);
            this.groupBox1.Controls.Add(this.nudScaleX);
            this.groupBox1.Controls.Add(this.label3);
            this.groupBox1.Controls.Add(this.label4);
            this.groupBox1.Location = new System.Drawing.Point(118, 131);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(95, 78);
            this.groupBox1.TabIndex = 6;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Size";
            // 
            // nudScaleY
            // 
            this.nudScaleY.Enabled = false;
            this.nudScaleY.Location = new System.Drawing.Point(46, 48);
            this.nudScaleY.Maximum = new decimal(new int[] {
            10000,
            0,
            0,
            0});
            this.nudScaleY.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.nudScaleY.Name = "nudScaleY";
            this.nudScaleY.Size = new System.Drawing.Size(43, 20);
            this.nudScaleY.TabIndex = 5;
            this.nudScaleY.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.nudScaleY.ValueChanged += new System.EventHandler(this.nudScaleY_ValueChanged);
            // 
            // nudScaleX
            // 
            this.nudScaleX.Enabled = false;
            this.nudScaleX.Location = new System.Drawing.Point(46, 18);
            this.nudScaleX.Maximum = new decimal(new int[] {
            10000,
            0,
            0,
            0});
            this.nudScaleX.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.nudScaleX.Name = "nudScaleX";
            this.nudScaleX.Size = new System.Drawing.Size(43, 20);
            this.nudScaleX.TabIndex = 4;
            this.nudScaleX.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.nudScaleX.ValueChanged += new System.EventHandler(this.nudScaleX_ValueChanged);
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.nudRot);
            this.groupBox2.Controls.Add(this.nudPosY);
            this.groupBox2.Controls.Add(this.nudPosX);
            this.groupBox2.Controls.Add(this.label1);
            this.groupBox2.Controls.Add(this.label6);
            this.groupBox2.Controls.Add(this.label2);
            this.groupBox2.Location = new System.Drawing.Point(11, 5);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(96, 103);
            this.groupBox2.TabIndex = 7;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Position";
            // 
            // nudRot
            // 
            this.nudRot.DecimalPlaces = 1;
            this.nudRot.Increment = new decimal(new int[] {
            1,
            0,
            0,
            65536});
            this.nudRot.Location = new System.Drawing.Point(26, 68);
            this.nudRot.Maximum = new decimal(new int[] {
            63,
            0,
            0,
            65536});
            this.nudRot.Minimum = new decimal(new int[] {
            63,
            0,
            0,
            -2147418112});
            this.nudRot.Name = "nudRot";
            this.nudRot.Size = new System.Drawing.Size(55, 20);
            this.nudRot.TabIndex = 8;
            this.nudRot.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.nudRot.ValueChanged += new System.EventHandler(this.nudRot_ValueChanged);
            // 
            // nudPosY
            // 
            this.nudPosY.Location = new System.Drawing.Point(26, 45);
            this.nudPosY.Maximum = new decimal(new int[] {
            10000,
            0,
            0,
            0});
            this.nudPosY.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.nudPosY.Name = "nudPosY";
            this.nudPosY.Size = new System.Drawing.Size(55, 20);
            this.nudPosY.TabIndex = 7;
            this.nudPosY.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.nudPosY.ValueChanged += new System.EventHandler(this.nudPosY_ValueChanged);
            // 
            // nudPosX
            // 
            this.nudPosX.Location = new System.Drawing.Point(25, 22);
            this.nudPosX.Maximum = new decimal(new int[] {
            10000,
            0,
            0,
            0});
            this.nudPosX.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.nudPosX.Name = "nudPosX";
            this.nudPosX.Size = new System.Drawing.Size(56, 20);
            this.nudPosX.TabIndex = 6;
            this.nudPosX.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.nudPosX.ValueChanged += new System.EventHandler(this.nudPosX_ValueChanged);
            // 
            // cbTileFlag
            // 
            this.cbTileFlag.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cbTileFlag.FormattingEnabled = true;
            this.cbTileFlag.Location = new System.Drawing.Point(85, 231);
            this.cbTileFlag.Name = "cbTileFlag";
            this.cbTileFlag.Size = new System.Drawing.Size(121, 21);
            this.cbTileFlag.TabIndex = 8;
            this.cbTileFlag.SelectedIndexChanged += new System.EventHandler(this.cbTileFlag_SelectedIndexChanged);
            // 
            // groupBox3
            // 
            this.groupBox3.Controls.Add(this.label10);
            this.groupBox3.Controls.Add(this.label9);
            this.groupBox3.Controls.Add(this.nudSourceHeight);
            this.groupBox3.Controls.Add(this.label8);
            this.groupBox3.Controls.Add(this.label7);
            this.groupBox3.Controls.Add(this.nudSourceWidth);
            this.groupBox3.Controls.Add(this.nudSourceY);
            this.groupBox3.Controls.Add(this.nudSourceX);
            this.groupBox3.Location = new System.Drawing.Point(118, 5);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Size = new System.Drawing.Size(109, 119);
            this.groupBox3.TabIndex = 9;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "Source Rect";
            // 
            // label10
            // 
            this.label10.AutoSize = true;
            this.label10.Location = new System.Drawing.Point(1, 93);
            this.label10.Name = "label10";
            this.label10.Size = new System.Drawing.Size(38, 13);
            this.label10.TabIndex = 11;
            this.label10.Text = "Height";
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.Location = new System.Drawing.Point(1, 68);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(35, 13);
            this.label9.TabIndex = 5;
            this.label9.Text = "Width";
            // 
            // nudSourceHeight
            // 
            this.nudSourceHeight.Location = new System.Drawing.Point(40, 91);
            this.nudSourceHeight.Maximum = new decimal(new int[] {
            10000,
            0,
            0,
            0});
            this.nudSourceHeight.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.nudSourceHeight.Name = "nudSourceHeight";
            this.nudSourceHeight.Size = new System.Drawing.Size(51, 20);
            this.nudSourceHeight.TabIndex = 10;
            this.nudSourceHeight.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.nudSourceHeight.ValueChanged += new System.EventHandler(this.nudSourceHeight_ValueChanged);
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(7, 43);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(14, 13);
            this.label8.TabIndex = 4;
            this.label8.Text = "Y";
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(7, 20);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(14, 13);
            this.label7.TabIndex = 3;
            this.label7.Text = "X";
            // 
            // nudSourceWidth
            // 
            this.nudSourceWidth.Location = new System.Drawing.Point(40, 66);
            this.nudSourceWidth.Maximum = new decimal(new int[] {
            10000,
            0,
            0,
            0});
            this.nudSourceWidth.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.nudSourceWidth.Name = "nudSourceWidth";
            this.nudSourceWidth.Size = new System.Drawing.Size(51, 20);
            this.nudSourceWidth.TabIndex = 2;
            this.nudSourceWidth.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.nudSourceWidth.ValueChanged += new System.EventHandler(this.nudSourceWidth_ValueChanged);
            // 
            // nudSourceY
            // 
            this.nudSourceY.Location = new System.Drawing.Point(40, 41);
            this.nudSourceY.Maximum = new decimal(new int[] {
            10000,
            0,
            0,
            0});
            this.nudSourceY.Name = "nudSourceY";
            this.nudSourceY.Size = new System.Drawing.Size(51, 20);
            this.nudSourceY.TabIndex = 1;
            this.nudSourceY.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.nudSourceY.ValueChanged += new System.EventHandler(this.nudSourceY_ValueChanged);
            // 
            // nudSourceX
            // 
            this.nudSourceX.Location = new System.Drawing.Point(40, 18);
            this.nudSourceX.Maximum = new decimal(new int[] {
            10000,
            0,
            0,
            0});
            this.nudSourceX.Name = "nudSourceX";
            this.nudSourceX.Size = new System.Drawing.Size(51, 20);
            this.nudSourceX.TabIndex = 0;
            this.nudSourceX.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.nudSourceX.ValueChanged += new System.EventHandler(this.nudSourceX_ValueChanged);
            // 
            // btnAccept
            // 
            this.btnAccept.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.btnAccept.Location = new System.Drawing.Point(69, 271);
            this.btnAccept.Name = "btnAccept";
            this.btnAccept.Size = new System.Drawing.Size(95, 23);
            this.btnAccept.TabIndex = 0;
            this.btnAccept.Text = "Accept (close)";
            this.btnAccept.UseVisualStyleBackColor = true;
            this.btnAccept.Click += new System.EventHandler(this.btnAccept_Click);
            // 
            // groupBox4
            // 
            this.groupBox4.Controls.Add(this.nudPOIY);
            this.groupBox4.Controls.Add(this.nudPOIX);
            this.groupBox4.Controls.Add(this.label12);
            this.groupBox4.Controls.Add(this.label11);
            this.groupBox4.Location = new System.Drawing.Point(11, 131);
            this.groupBox4.Name = "groupBox4";
            this.groupBox4.Size = new System.Drawing.Size(96, 78);
            this.groupBox4.TabIndex = 12;
            this.groupBox4.TabStop = false;
            this.groupBox4.Text = "Point of Interest";
            // 
            // nudPOIY
            // 
            this.nudPOIY.Location = new System.Drawing.Point(25, 47);
            this.nudPOIY.Maximum = new decimal(new int[] {
            10000,
            0,
            0,
            0});
            this.nudPOIY.Name = "nudPOIY";
            this.nudPOIY.Size = new System.Drawing.Size(56, 20);
            this.nudPOIY.TabIndex = 3;
            this.nudPOIY.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.nudPOIY.ValueChanged += new System.EventHandler(this.nudPOIY_ValueChanged);
            // 
            // nudPOIX
            // 
            this.nudPOIX.Location = new System.Drawing.Point(25, 20);
            this.nudPOIX.Maximum = new decimal(new int[] {
            10000,
            0,
            0,
            0});
            this.nudPOIX.Name = "nudPOIX";
            this.nudPOIX.Size = new System.Drawing.Size(56, 20);
            this.nudPOIX.TabIndex = 2;
            this.nudPOIX.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.nudPOIX.ValueChanged += new System.EventHandler(this.nudPOIX_ValueChanged);
            // 
            // label12
            // 
            this.label12.AutoSize = true;
            this.label12.Location = new System.Drawing.Point(8, 49);
            this.label12.Name = "label12";
            this.label12.Size = new System.Drawing.Size(14, 13);
            this.label12.TabIndex = 1;
            this.label12.Text = "Y";
            // 
            // label11
            // 
            this.label11.AutoSize = true;
            this.label11.Location = new System.Drawing.Point(8, 22);
            this.label11.Name = "label11";
            this.label11.Size = new System.Drawing.Size(14, 13);
            this.label11.TabIndex = 0;
            this.label11.Text = "X";
            // 
            // EditTileDlg
            // 
            this.AcceptButton = this.btnAccept;
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(236, 306);
            this.Controls.Add(this.groupBox4);
            this.Controls.Add(this.btnAccept);
            this.Controls.Add(this.groupBox3);
            this.Controls.Add(this.cbTileFlag);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.groupBox2);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "EditTileDlg";
            this.Opacity = 0.85;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Edit Tile";
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.nudScaleY)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.nudScaleX)).EndInit();
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.nudRot)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.nudPosY)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.nudPosX)).EndInit();
            this.groupBox3.ResumeLayout(false);
            this.groupBox3.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.nudSourceHeight)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.nudSourceWidth)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.nudSourceY)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.nudSourceX)).EndInit();
            this.groupBox4.ResumeLayout(false);
            this.groupBox4.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.nudPOIY)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.nudPOIX)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.GroupBox groupBox3;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.Label label10;
        private System.Windows.Forms.Button btnAccept;
        private System.Windows.Forms.GroupBox groupBox4;
        private System.Windows.Forms.Label label12;
        private System.Windows.Forms.Label label11;
        public System.Windows.Forms.NumericUpDown nudScaleY;
        public System.Windows.Forms.NumericUpDown nudScaleX;
        public System.Windows.Forms.NumericUpDown nudRot;
        public System.Windows.Forms.NumericUpDown nudPosY;
        public System.Windows.Forms.NumericUpDown nudPosX;
        public System.Windows.Forms.ComboBox cbTileFlag;
        public System.Windows.Forms.NumericUpDown nudSourceWidth;
        public System.Windows.Forms.NumericUpDown nudSourceY;
        public System.Windows.Forms.NumericUpDown nudSourceX;
        public System.Windows.Forms.NumericUpDown nudSourceHeight;
        public System.Windows.Forms.NumericUpDown nudPOIY;
        public System.Windows.Forms.NumericUpDown nudPOIX;
    }
}