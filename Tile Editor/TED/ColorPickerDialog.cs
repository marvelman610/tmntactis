//  [5/27/2009 Ramon Johannessen (RJ)]

/// <summary>
///  File : ColorPickerDialog.cs
///  Purpose : Used when loading a new tileset image to set the alpha color
/// </summary>
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace ColorPicker
{
	public partial class ColorPickerDialog : Form
	{
        ColorPickerCtrl m_colorPicker;
        public ColorPicker.ColorPickerCtrl ColorPicker
        {
            get { return m_colorPicker; }
            set { m_colorPicker = value; }
        }
        public event EventHandler OKpushed;

        public ColorPicker.ColorPickerCtrl CP
        {
            get { return m_colorPicker; }
            set { m_colorPicker = value; }
        }
		public ColorPickerDialog(int imageID, float scaleX, float scaleY)
		{
			InitializeComponent();

            m_colorPicker = new ColorPickerCtrl(imageID, scaleX, scaleY);
			m_colorPicker.Dock = DockStyle.Fill;
			m_colorTabPage.Controls.Add(m_colorPicker);
		}

		private void OnSelected(object sender, TabControlEventArgs e)
		{
		}

        private void m_ok_Click(object sender, EventArgs e)
        {
            if (OKpushed != null)
                OKpushed(this, new EventArgs());
        }
	}
}