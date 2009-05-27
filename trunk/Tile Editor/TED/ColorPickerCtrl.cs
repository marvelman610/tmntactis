using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Data;
using System.Text;
using System.Windows.Forms;
using System.Reflection;
using Microsoft.DirectX;
using Microsoft.DirectX.Direct3D;
using SGD;

namespace ColorPicker
{
	public partial class ColorPickerCtrl : UserControl
	{
        string a = "A:";
        string r = "R:";
        string g = "G:";
        string b = "B:";
        ManagedDirect3D mD3d;
        ManagedTextureManager mTM;
        int m_nImageID = -1;
        public int ImageID
        {
            get { return m_nImageID; }
            set { m_nImageID = value; }
        }
        float m_nScaleX;
        float m_nScaleY;
        Color m_clrKey = Color.White;
		Color m_selectedColor = Color.AntiqueWhite;
		public Color SelectedColor
		{
			get { return Color.FromArgb((int)Math.Floor(255f*m_opacity), m_selectedColor);; }
			set
			{
				m_opacity = (float)value.A / 255f;
				value = Color.FromArgb(255, value);
				//m_colorWheel.SelectedColor = value;
// 				if (m_colorTable.ColorExist(value) == false)
// 					m_colorTable.SetCustomColor(value);
// 				m_colorTable.SelectedItem = value;
//				m_opacitySlider.Percent = m_opacity;
			}
		}
        public ColorPickerCtrl(int imageID, float scaleX, float scaleY)
		{
			InitializeComponent();
            m_nScaleX = scaleX;
            m_nScaleY = scaleY;
            mD3d = ManagedDirect3D.Instance;
            mTM = ManagedTextureManager.Instance;
            m_nImageID = imageID;
		}
		float m_opacity = 1;
		void OnOpacityValueChanged(object sender, EventArgs e)
		{
			UpdateInfo();
		}
		void UpdateInfo()
		{
			Color c = Color.FromArgb((int)Math.Floor(255f*m_opacity), m_selectedColor);
		}
		void OnColorSamplePaint(object sender, PaintEventArgs e)
		{
			Color c = Color.FromArgb((int)Math.Floor(255f*m_opacity), m_selectedColor);
			SolidBrush b = new SolidBrush(c);
			c = Color.FromArgb(255, m_selectedColor);
			b = new SolidBrush(c);
		}

        private void panel1_Paint(object sender, PaintEventArgs e)
        {
            if (m_nImageID != -1)
            {
                mD3d.ChangeDisplayParam(this.panel1, false);
                mD3d.DeviceBegin();
                mD3d.Clear(255, 255, 255);
                mD3d.SpriteBegin();
                mTM.Draw(m_nImageID, 0, 0, m_nScaleX, m_nScaleY, Rectangle.Empty, 0, 0, 0.0f, m_clrKey.ToArgb());
                mD3d.SpriteEnd();
                mD3d.DeviceEnd();
                mD3d.Present();
            }
        }

        private void ColorPickerCtrl_MouseMove(object sender, MouseEventArgs e)
        {
            lblA.Text = a + this.m_eyedropColorPicker.SelectedColor.A.ToString();
            lblR.Text = r + this.m_eyedropColorPicker.SelectedColor.R.ToString();
            lblG.Text = g + this.m_eyedropColorPicker.SelectedColor.G.ToString();
            lblB.Text = b + this.m_eyedropColorPicker.SelectedColor.B.ToString();
        }

        private void panel1_MouseMove(object sender, MouseEventArgs e)
        {
            lblA.Text = a + this.m_eyedropColorPicker.SelectedColor.A.ToString();
            lblR.Text = r + this.m_eyedropColorPicker.SelectedColor.R.ToString();
            lblG.Text = g + this.m_eyedropColorPicker.SelectedColor.G.ToString();
            lblB.Text = b + this.m_eyedropColorPicker.SelectedColor.B.ToString();
        }

	}
}
