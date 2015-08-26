using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace ModuloFun
{
    // this is a nasty comment with the word poopy in it

    // throw some crappy thing in

    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            int padding = (4 - (( (int)numericUpDown1.Value) % 4)) % 4;
            numericUpDown2.Value = padding;
        }
    }

    // this is an excellent comment we should keep?
    //here is a trial comment
    // here is another
}
