namespace ColorPicker
{
	partial class ColorPickerCtrl
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

		#region Component Designer generated code

		/// <summary> 
		/// Required method for Designer support - do not modify 
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
            this.components = new System.ComponentModel.Container();
            this.panel1 = new System.Windows.Forms.Panel();
            this.m_tooltip = new System.Windows.Forms.ToolTip(this.components);
            this.label1 = new System.Windows.Forms.Label();
            this.lblA = new System.Windows.Forms.Label();
            this.lblR = new System.Windows.Forms.Label();
            this.lblG = new System.Windows.Forms.Label();
            this.lblB = new System.Windows.Forms.Label();
            this.m_eyedropColorPicker = new ColorPicker.EyedropColorPicker();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // panel1
            // 
            this.panel1.BackColor = System.Drawing.Color.Transparent;
            this.panel1.Location = new System.Drawing.Point(89, 6);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(496, 441);
            this.panel1.TabIndex = 9;
            this.panel1.Paint += new System.Windows.Forms.PaintEventHandler(this.panel1_Paint);
            this.panel1.MouseMove += new System.Windows.Forms.MouseEventHandler(this.panel1_MouseMove);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(7, 16);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(54, 13);
            this.label1.TabIndex = 10;
            this.label1.Text = "Click here";
            // 
            // lblA
            // 
            this.lblA.AutoSize = true;
            this.lblA.Location = new System.Drawing.Point(2, 152);
            this.lblA.Name = "lblA";
            this.lblA.Size = new System.Drawing.Size(17, 13);
            this.lblA.TabIndex = 11;
            this.lblA.Text = "A:";
            // 
            // lblR
            // 
            this.lblR.AutoSize = true;
            this.lblR.Location = new System.Drawing.Point(2, 169);
            this.lblR.Name = "lblR";
            this.lblR.Size = new System.Drawing.Size(18, 13);
            this.lblR.TabIndex = 12;
            this.lblR.Text = "R:";
            // 
            // lblG
            // 
            this.lblG.AutoSize = true;
            this.lblG.Location = new System.Drawing.Point(2, 186);
            this.lblG.Name = "lblG";
            this.lblG.Size = new System.Drawing.Size(18, 13);
            this.lblG.TabIndex = 13;
            this.lblG.Text = "G:";
            // 
            // lblB
            // 
            this.lblB.AutoSize = true;
            this.lblB.Location = new System.Drawing.Point(2, 203);
            this.lblB.Name = "lblB";
            this.lblB.Size = new System.Drawing.Size(17, 13);
            this.lblB.TabIndex = 14;
            this.lblB.Text = "B:";
            // 
            // m_eyedropColorPicker
            // 
            this.m_eyedropColorPicker.BackColor = System.Drawing.SystemColors.Control;
            this.m_eyedropColorPicker.Location = new System.Drawing.Point(2, 35);
            this.m_eyedropColorPicker.Name = "m_eyedropColorPicker";
            this.m_eyedropColorPicker.SelectedColor = System.Drawing.Color.Empty;
            this.m_eyedropColorPicker.Size = new System.Drawing.Size(60, 60);
            this.m_eyedropColorPicker.TabIndex = 2;
            this.m_eyedropColorPicker.TabStop = false;
            this.m_tooltip.SetToolTip(this.m_eyedropColorPicker, "Color Selector. Click and Drag to pick a color from the screen");
            this.m_eyedropColorPicker.Zoom = 4;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(2, 103);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(74, 13);
            this.label2.TabIndex = 15;
            this.label2.Text = "Hold and drag";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(4, 118);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(73, 13);
            this.label3.TabIndex = 16;
            this.label3.Text = "to select color";
            // 
            // ColorPickerCtrl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.Color.Transparent;
            this.Controls.Add(this.label3);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.lblB);
            this.Controls.Add(this.lblG);
            this.Controls.Add(this.lblR);
            this.Controls.Add(this.lblA);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.m_eyedropColorPicker);
            this.Controls.Add(this.panel1);
            this.Name = "ColorPickerCtrl";
            this.Padding = new System.Windows.Forms.Padding(3, 3, 0, 0);
            this.Size = new System.Drawing.Size(680, 463);
            this.MouseMove += new System.Windows.Forms.MouseEventHandler(this.ColorPickerCtrl_MouseMove);
            this.ResumeLayout(false);
            this.PerformLayout();

		}

		#endregion

        //private ColorWheelCtrl m_colorWheel;
		private System.Windows.Forms.Panel panel1;
		private EyedropColorPicker m_eyedropColorPicker;
        public ColorPicker.EyedropColorPicker EyedropColorPicker
        {
            get { return m_eyedropColorPicker; }
            set { m_eyedropColorPicker = value; }
        }
        private System.Windows.Forms.ToolTip m_tooltip;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label lblA;
        private System.Windows.Forms.Label lblR;
        private System.Windows.Forms.Label lblG;
        private System.Windows.Forms.Label lblB;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
	}
}
