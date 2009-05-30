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
    public partial class SetTileFlagsDlg : Form
    {
        public SetTileFlagsDlg(string f0, string f1, string f2, string f3)
        {
            InitializeComponent();
            flag0 = f0.Remove(0, 2); flag1 = f1.Remove(0, 2); flag2 = f2.Remove(0, 2); flag3 = f3.Remove(0, 2);
            tbFlag0.Text = flag0; tbFlag1.Text = flag1; tbFlag2.Text = flag2; tbFlag3.Text = flag3;
        }

        public event EventHandler setFlagsPushed;

        string flag0;
        public string Flag0
        {
            get { return flag0; }
            set { flag0 = value; }
        }
        string flag1;
        public string Flag1
        {
            get { return flag1; }
            set { flag1 = value; }
        }
        string flag2;
        public string Flag2
        {
            get { return flag2; }
            set { flag2 = value; }
        }
        string flag3;
        public string Flag3
        {
            get { return flag3; }
            set { flag3 = value; }
        }
        bool bflag0Changed = false;
        public bool Bflag0Changed
        {
            get { return bflag0Changed; }
            set { bflag0Changed = value; }
        }
        bool bflag1Changed = false;
        public bool Bflag1Changed
        {
            get { return bflag1Changed; }
            set { bflag1Changed = value; }
        }
        bool bflag2Changed = false;
        public bool Bflag2Changed
        {
            get { return bflag2Changed; }
            set { bflag2Changed = value; }
        }
        bool bflag3Changed = false;
        public bool Bflag3Changed
        {
            get { return bflag3Changed; }
            set { bflag3Changed = value; }
        }
        private void btnSetFlags_Click(object sender, EventArgs e)
        {
            if (setFlagsPushed != null)
                setFlagsPushed(this, new EventArgs());
        }

        private void btnCancelFlags_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        private void tbFlag0_TextChanged(object sender, EventArgs e)
        {
            bflag0Changed = true;
            flag0 = tbFlag0.Text;
        }

        private void tbFlag1_TextChanged(object sender, EventArgs e)
        {
            bflag1Changed = true;
            flag1 = tbFlag1.Text;
        }

        private void tbFlag2_TextChanged(object sender, EventArgs e)
        {
            bflag2Changed = true;
            flag2 = tbFlag2.Text;
        }

        private void tbFlag3_TextChanged(object sender, EventArgs e)
        {
            bflag3Changed = true;
            flag3 = tbFlag3.Text;
        }
    }
}
