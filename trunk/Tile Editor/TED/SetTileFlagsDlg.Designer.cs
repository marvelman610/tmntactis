namespace test
{
    partial class SetTileFlagsDlg
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
            this.btnSetFlags = new System.Windows.Forms.Button();
            this.btnCancelFlags = new System.Windows.Forms.Button();
            this.tbFlag0 = new System.Windows.Forms.TextBox();
            this.tbFlag1 = new System.Windows.Forms.TextBox();
            this.tbFlag2 = new System.Windows.Forms.TextBox();
            this.tbFlag3 = new System.Windows.Forms.TextBox();
            this.SuspendLayout();
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(12, 12);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(36, 13);
            this.label1.TabIndex = 0;
            this.label1.Text = "Flag 0";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(12, 36);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(36, 13);
            this.label2.TabIndex = 1;
            this.label2.Text = "Flag 1";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(12, 60);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(36, 13);
            this.label3.TabIndex = 2;
            this.label3.Text = "Flag 2";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(12, 84);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(36, 13);
            this.label4.TabIndex = 3;
            this.label4.Text = "Flag 3";
            // 
            // btnSetFlags
            // 
            this.btnSetFlags.Location = new System.Drawing.Point(15, 120);
            this.btnSetFlags.Name = "btnSetFlags";
            this.btnSetFlags.Size = new System.Drawing.Size(99, 23);
            this.btnSetFlags.TabIndex = 4;
            this.btnSetFlags.Text = "Apply and Close";
            this.btnSetFlags.UseVisualStyleBackColor = true;
            this.btnSetFlags.Click += new System.EventHandler(this.btnSetFlags_Click);
            // 
            // btnCancelFlags
            // 
            this.btnCancelFlags.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.btnCancelFlags.Location = new System.Drawing.Point(120, 120);
            this.btnCancelFlags.Name = "btnCancelFlags";
            this.btnCancelFlags.Size = new System.Drawing.Size(75, 23);
            this.btnCancelFlags.TabIndex = 5;
            this.btnCancelFlags.Text = "Cancel";
            this.btnCancelFlags.UseVisualStyleBackColor = true;
            this.btnCancelFlags.Click += new System.EventHandler(this.btnCancelFlags_Click);
            // 
            // tbFlag0
            // 
            this.tbFlag0.Location = new System.Drawing.Point(73, 9);
            this.tbFlag0.Name = "tbFlag0";
            this.tbFlag0.Size = new System.Drawing.Size(100, 20);
            this.tbFlag0.TabIndex = 6;
            this.tbFlag0.TextChanged += new System.EventHandler(this.tbFlag0_TextChanged);
            // 
            // tbFlag1
            // 
            this.tbFlag1.Location = new System.Drawing.Point(73, 33);
            this.tbFlag1.Name = "tbFlag1";
            this.tbFlag1.Size = new System.Drawing.Size(100, 20);
            this.tbFlag1.TabIndex = 7;
            this.tbFlag1.TextChanged += new System.EventHandler(this.tbFlag1_TextChanged);
            // 
            // tbFlag2
            // 
            this.tbFlag2.Location = new System.Drawing.Point(73, 57);
            this.tbFlag2.Name = "tbFlag2";
            this.tbFlag2.Size = new System.Drawing.Size(100, 20);
            this.tbFlag2.TabIndex = 8;
            this.tbFlag2.TextChanged += new System.EventHandler(this.tbFlag2_TextChanged);
            // 
            // tbFlag3
            // 
            this.tbFlag3.Location = new System.Drawing.Point(73, 81);
            this.tbFlag3.Name = "tbFlag3";
            this.tbFlag3.Size = new System.Drawing.Size(100, 20);
            this.tbFlag3.TabIndex = 9;
            this.tbFlag3.TextChanged += new System.EventHandler(this.tbFlag3_TextChanged);
            // 
            // SetTileFlagsDlg
            // 
            this.AcceptButton = this.btnSetFlags;
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.CancelButton = this.btnCancelFlags;
            this.ClientSize = new System.Drawing.Size(211, 159);
            this.Controls.Add(this.tbFlag3);
            this.Controls.Add(this.tbFlag2);
            this.Controls.Add(this.tbFlag1);
            this.Controls.Add(this.tbFlag0);
            this.Controls.Add(this.btnCancelFlags);
            this.Controls.Add(this.btnSetFlags);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.MaximizeBox = false;
            this.Name = "SetTileFlagsDlg";
            this.ShowIcon = false;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Set Tile Flags";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Button btnSetFlags;
        private System.Windows.Forms.Button btnCancelFlags;
        private System.Windows.Forms.TextBox tbFlag0;
        private System.Windows.Forms.TextBox tbFlag1;
        private System.Windows.Forms.TextBox tbFlag2;
        private System.Windows.Forms.TextBox tbFlag3;
    }
}