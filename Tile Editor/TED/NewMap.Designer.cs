namespace test
{
    partial class NewMap
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
            this.components = new System.ComponentModel.Container();
            this.tbColumns = new System.Windows.Forms.TextBox();
            this.tbRows = new System.Windows.Forms.TextBox();
            this.cbIsometric = new System.Windows.Forms.CheckBox();
            this.tbCellsize = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.btnCreate = new System.Windows.Forms.Button();
            this.btnCancel = new System.Windows.Forms.Button();
            this.toolTip1 = new System.Windows.Forms.ToolTip(this.components);
            this.label3 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.tbIsoWidth = new System.Windows.Forms.TextBox();
            this.tbIsoHeight = new System.Windows.Forms.TextBox();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.cbIsoType = new System.Windows.Forms.ComboBox();
            this.groupBox1.SuspendLayout();
            this.SuspendLayout();
            // 
            // tbColumns
            // 
            this.tbColumns.Location = new System.Drawing.Point(9, 31);
            this.tbColumns.Name = "tbColumns";
            this.tbColumns.Size = new System.Drawing.Size(47, 20);
            this.tbColumns.TabIndex = 0;
            this.tbColumns.Text = "10";
            this.tbColumns.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            // 
            // tbRows
            // 
            this.tbRows.Location = new System.Drawing.Point(65, 31);
            this.tbRows.Name = "tbRows";
            this.tbRows.Size = new System.Drawing.Size(43, 20);
            this.tbRows.TabIndex = 1;
            this.tbRows.Text = "10";
            this.tbRows.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            // 
            // cbIsometric
            // 
            this.cbIsometric.AutoSize = true;
            this.cbIsometric.Location = new System.Drawing.Point(11, 21);
            this.cbIsometric.Name = "cbIsometric";
            this.cbIsometric.Size = new System.Drawing.Size(68, 17);
            this.cbIsometric.TabIndex = 2;
            this.cbIsometric.Text = "Isometric";
            this.toolTip1.SetToolTip(this.cbIsometric, "Is the map going to be Isometric?");
            this.cbIsometric.UseVisualStyleBackColor = true;
            this.cbIsometric.CheckedChanged += new System.EventHandler(this.cbIsometric_CheckedChanged);
            // 
            // tbCellsize
            // 
            this.tbCellsize.Location = new System.Drawing.Point(197, 31);
            this.tbCellsize.Name = "tbCellsize";
            this.tbCellsize.Size = new System.Drawing.Size(47, 20);
            this.tbCellsize.TabIndex = 3;
            this.tbCellsize.Text = "32";
            this.tbCellsize.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(9, 12);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(47, 13);
            this.label1.TabIndex = 4;
            this.label1.Text = "Columns";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(62, 12);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(34, 13);
            this.label2.TabIndex = 5;
            this.label2.Text = "Rows";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(197, 12);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(47, 13);
            this.label4.TabIndex = 7;
            this.label4.Text = "Cell Size";
            this.toolTip1.SetToolTip(this.label4, "This sets the width and height of each cell");
            // 
            // btnCreate
            // 
            this.btnCreate.Location = new System.Drawing.Point(33, 148);
            this.btnCreate.Name = "btnCreate";
            this.btnCreate.Size = new System.Drawing.Size(75, 23);
            this.btnCreate.TabIndex = 8;
            this.btnCreate.Text = "Create";
            this.btnCreate.UseVisualStyleBackColor = true;
            this.btnCreate.Click += new System.EventHandler(this.btnCreate_Click);
            // 
            // btnCancel
            // 
            this.btnCancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.btnCancel.Location = new System.Drawing.Point(149, 148);
            this.btnCancel.Name = "btnCancel";
            this.btnCancel.Size = new System.Drawing.Size(75, 23);
            this.btnCancel.TabIndex = 9;
            this.btnCancel.Text = "Cancel";
            this.btnCancel.UseVisualStyleBackColor = true;
            this.btnCancel.Click += new System.EventHandler(this.btnCancel_Click);
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(134, 49);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(55, 13);
            this.label3.TabIndex = 10;
            this.label3.Text = "Cell Width";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(134, 23);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(58, 13);
            this.label5.TabIndex = 11;
            this.label5.Text = "Cell Height";
            // 
            // tbIsoWidth
            // 
            this.tbIsoWidth.Enabled = false;
            this.tbIsoWidth.Location = new System.Drawing.Point(194, 46);
            this.tbIsoWidth.Name = "tbIsoWidth";
            this.tbIsoWidth.Size = new System.Drawing.Size(49, 20);
            this.tbIsoWidth.TabIndex = 2;
            this.tbIsoWidth.Text = "0";
            this.tbIsoWidth.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            // 
            // tbIsoHeight
            // 
            this.tbIsoHeight.Enabled = false;
            this.tbIsoHeight.Location = new System.Drawing.Point(194, 20);
            this.tbIsoHeight.Name = "tbIsoHeight";
            this.tbIsoHeight.Size = new System.Drawing.Size(49, 20);
            this.tbIsoHeight.TabIndex = 1;
            this.tbIsoHeight.Text = "0";
            this.tbIsoHeight.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.cbIsoType);
            this.groupBox1.Controls.Add(this.tbIsoHeight);
            this.groupBox1.Controls.Add(this.tbIsoWidth);
            this.groupBox1.Controls.Add(this.label5);
            this.groupBox1.Controls.Add(this.label3);
            this.groupBox1.Controls.Add(this.cbIsometric);
            this.groupBox1.Location = new System.Drawing.Point(1, 57);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(252, 81);
            this.groupBox1.TabIndex = 14;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Isometric";
            // 
            // cbIsoType
            // 
            this.cbIsoType.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cbIsoType.Enabled = false;
            this.cbIsoType.FormattingEnabled = true;
            this.cbIsoType.Items.AddRange(new object[] {
            "Diamond",
            "Staggered",
            "Slide"});
            this.cbIsoType.Location = new System.Drawing.Point(11, 44);
            this.cbIsoType.Name = "cbIsoType";
            this.cbIsoType.Size = new System.Drawing.Size(96, 21);
            this.cbIsoType.TabIndex = 12;
            // 
            // NewMap
            // 
            this.AcceptButton = this.btnCreate;
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.CancelButton = this.btnCancel;
            this.ClientSize = new System.Drawing.Size(256, 182);
            this.Controls.Add(this.btnCancel);
            this.Controls.Add(this.btnCreate);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.tbCellsize);
            this.Controls.Add(this.tbRows);
            this.Controls.Add(this.tbColumns);
            this.Controls.Add(this.groupBox1);
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "NewMap";
            this.ShowIcon = false;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Create New Map";
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TextBox tbColumns;
        private System.Windows.Forms.TextBox tbRows;
        private System.Windows.Forms.CheckBox cbIsometric;
        private System.Windows.Forms.TextBox tbCellsize;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Button btnCreate;
        private System.Windows.Forms.Button btnCancel;
        private System.Windows.Forms.ToolTip toolTip1;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.TextBox tbIsoWidth;
        private System.Windows.Forms.TextBox tbIsoHeight;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.ComboBox cbIsoType;
    }
}