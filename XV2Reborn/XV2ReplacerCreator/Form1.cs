using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.IO.Compression;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace XV2ReplacerCreator
{
    public partial class Form1 : Form
    {
        FolderBrowserDialog fbd = new FolderBrowserDialog();
        SaveFileDialog sfd = new SaveFileDialog();

        public Form1()
        {
            InitializeComponent();
        }

        private void saveToolStripMenuItem_Click(object sender, EventArgs e)
        {
            sfd.Filter = ".xv2mod files | *.xv2mod";
            sfd.Title = "Save Mod";

            if (Directory.Exists(txtFolder.Text) && txtName.Text.Length > 0 && txtAuthor.Text.Length > 0)
            {
                string xmlpath = txtFolder.Text + "\\modinfo.xml";

                File.WriteAllText(xmlpath, txtName.Text + "\n" + txtAuthor.Text);

                if (sfd.ShowDialog() == DialogResult.OK)
                {
                    ZipFile.CreateFromDirectory(txtFolder.Text, sfd.FileName);
                    MessageBox.Show("Mod Created Successfully", "Success", MessageBoxButtons.OK, MessageBoxIcon.Information);
                }
                else
                {
                    return;
                }
            }
            else
            {
                MessageBox.Show("Invalid Format", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private void button1_Click(object sender, EventArgs e)
        {
            fbd.Description = "Select Mod Folder";

            if (fbd.ShowDialog() == DialogResult.OK)
            {
                txtFolder.Text = fbd.SelectedPath;
            }
        }
    }
}
