using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace AGS.Editor
{
    public partial class NumberEntryDialog : Form
    {
        private NumberEntryDialog(string titleText, string headerText, int initialValue)
        {
            InitializeComponent();
            this.Text = titleText;
            lblHeader.Text = headerText;
            udNumber.Value = initialValue;
            udNumber.Select();
            udNumber.Select(0, ((int)udNumber.Value).ToString().Length);
        }

        public int Number
        {
            get { return (int)udNumber.Value; }
            set { udNumber.Value = value; }
        }

        public static int Show(string titleBar, string headerText, int currentValue)
        {
            int result = -1;
            NumberEntryDialog dialog = new NumberEntryDialog(titleBar, headerText, currentValue);
            if (dialog.ShowDialog() == DialogResult.OK)
            {
                result = dialog.Number;
            }
            dialog.Dispose();
            return result;
        }

        private void btnCancel_Click(object sender, EventArgs e)
        {
            this.DialogResult = DialogResult.Cancel;
            this.Close();
        }

        private void btnOK_Click(object sender, EventArgs e)
        {
            this.DialogResult = DialogResult.OK;
            this.Close();
        }
    }
}