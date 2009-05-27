namespace ColorPicker
{
	partial class ColorPickerDialog
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
            this.m_cancel = new System.Windows.Forms.Button();
            this.m_ok = new System.Windows.Forms.Button();
            this.m_colorTabPage = new System.Windows.Forms.TabPage();
            this.m_tabControl = new System.Windows.Forms.TabControl();
            this.m_colorTabPage.SuspendLayout();
            this.m_tabControl.SuspendLayout();
            this.SuspendLayout();
            // 
            // m_cancel
            // 
            this.m_cancel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.m_cancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.m_cancel.Location = new System.Drawing.Point(6, 341);
            this.m_cancel.Name = "m_cancel";
            this.m_cancel.Size = new System.Drawing.Size(75, 23);
            this.m_cancel.TabIndex = 2;
            this.m_cancel.Text = "&Cancel";
            this.m_cancel.UseVisualStyleBackColor = true;
            // 
            // m_ok
            // 
            this.m_ok.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.m_ok.DialogResult = System.Windows.Forms.DialogResult.OK;
            this.m_ok.Location = new System.Drawing.Point(6, 291);
            this.m_ok.Name = "m_ok";
            this.m_ok.Size = new System.Drawing.Size(75, 23);
            this.m_ok.TabIndex = 2;
            this.m_ok.Text = "&OK";
            this.m_ok.UseVisualStyleBackColor = true;
            this.m_ok.Click += new System.EventHandler(this.m_ok_Click);
            // 
            // m_colorTabPage
            // 
            this.m_colorTabPage.Controls.Add(this.m_cancel);
            this.m_colorTabPage.Controls.Add(this.m_ok);
            this.m_colorTabPage.Location = new System.Drawing.Point(4, 22);
            this.m_colorTabPage.Name = "m_colorTabPage";
            this.m_colorTabPage.Padding = new System.Windows.Forms.Padding(3);
            this.m_colorTabPage.Size = new System.Drawing.Size(662, 405);
            this.m_colorTabPage.TabIndex = 0;
            this.m_colorTabPage.Text = "Select Key Color";
            this.m_colorTabPage.UseVisualStyleBackColor = true;
            // 
            // m_tabControl
            // 
            this.m_tabControl.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.m_tabControl.Controls.Add(this.m_colorTabPage);
            this.m_tabControl.Location = new System.Drawing.Point(4, 5);
            this.m_tabControl.Name = "m_tabControl";
            this.m_tabControl.SelectedIndex = 0;
            this.m_tabControl.Size = new System.Drawing.Size(670, 431);
            this.m_tabControl.TabIndex = 1;
            this.m_tabControl.Selected += new System.Windows.Forms.TabControlEventHandler(this.OnSelected);
            // 
            // ColorPickerDialog
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(681, 447);
            this.Controls.Add(this.m_tabControl);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "ColorPickerDialog";
            this.Text = "Color Picker";
            this.m_colorTabPage.ResumeLayout(false);
            this.m_tabControl.ResumeLayout(false);
            this.ResumeLayout(false);

		}

		#endregion

        private System.Windows.Forms.Button m_ok;
        private System.Windows.Forms.Button m_cancel;
        private System.Windows.Forms.TabPage m_colorTabPage;
        private System.Windows.Forms.TabControl m_tabControl;
	}
}

