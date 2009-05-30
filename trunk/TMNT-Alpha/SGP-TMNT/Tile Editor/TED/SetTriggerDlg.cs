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
    public partial class SetTriggerDlg : Form
    {
        public event EventHandler AcceptPushed;
        string trigger;
        public string Trigger
        {
            get { return trigger; }
            set { trigger = value; }
        }
        bool m_bChanges = false;
        public bool Changes
        {
            get { return m_bChanges; }
            set { m_bChanges = value; }
        }
        public SetTriggerDlg(string trigger)
        {
            InitializeComponent();
            tbTrigger.Text = trigger;
            Trigger = trigger;
        }

        private void btnAccept_Click(object sender, EventArgs e)
        {
            if (tbTrigger.Text != "" && Trigger != tbTrigger.Text)
            {
                m_bChanges = true;
	            Trigger = tbTrigger.Text;
            }
            if (AcceptPushed != null)
                AcceptPushed(this, new EventArgs());
        }

        private void btnCancel_Click(object sender, EventArgs e)
        {
            this.Close();
        }
    }
}
