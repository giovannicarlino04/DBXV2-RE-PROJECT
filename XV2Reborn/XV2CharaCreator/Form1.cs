using Microsoft.VisualBasic.ApplicationServices;
using System.IO;
using System;
using System.IO.Compression;
using System.Xml;
using System.Windows.Forms;
using XV2Reborn;

namespace XV2CharaCreator
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void buildXV2ModFileToolStripMenuItem_Click(object sender, EventArgs e)
        {
            SaveFileDialog sfd = new SaveFileDialog();
            sfd.Title = "Save Mod File";
            sfd.Filter = "Xenoverse 2 Mod Files (*.xv2mod)|*.xv2mod";

            if (txtName.Text.Length > 0 && txtAuthor.Text.Length > 0
                && Directory.Exists(txtFolder.Text)
                && sfd.ShowDialog() == DialogResult.OK
                && File.Exists(textBox1.Text))
            {
                // Specify the path where you want to save the XML file
                string xmlFilePath = "C:/XV2CharaCreatorTemp/xv2mod.xml";

                if (Directory.Exists("C:/XV2CharaCreatorTemp"))
                    Directory.Delete("C:/XV2CharaCreatorTemp", true);
                Directory.CreateDirectory("C:/XV2CharaCreatorTemp");

                // Create an XmlWriterSettings instance for formatting the XML
                XmlWriterSettings settings = new XmlWriterSettings
                {
                    Indent = true,
                    IndentChars = "    ", // Use four spaces for indentation
                };

                // Create the XmlWriter and write the XML content
                using (XmlWriter writer = XmlWriter.Create(xmlFilePath, settings))
                {
                    writer.WriteStartDocument();
                    writer.WriteStartElement("XV2MOD");
                    writer.WriteAttributeString("type", "ADDED_CHARACTER");

                    WriteElementWithValue(writer, "MOD_NAME", txtName.Text);
                    WriteElementWithValue(writer, "MOD_AUTHOR", txtAuthor.Text);
                    WriteElementWithValue(writer, "MOD_VERSION", txtVersion.Text);


                    // Let's start adding the actual character attributes (AUR, CMS, CSO, etc...)
                    WriteElementWithValue(writer, "AUR_ID", txtAuraID.Text);
                    if (cbAuraGlare.Checked)
                        WriteElementWithValue(writer, "AUR_GLARE", "1");
                    else
                        WriteElementWithValue(writer, "AUR_GLARE", "0");

                    WriteElementWithValue(writer, "CMS_BCS", txtBCS.Text);
                    WriteElementWithValue(writer, "CMS_EAN", txtEAN.Text);
                    WriteElementWithValue(writer, "CMS_FCE_EAN", txtFCEEAN.Text);
                    WriteElementWithValue(writer, "CMS_CAM_EAN", txtCAMEAN.Text);
                    WriteElementWithValue(writer, "CMS_BAC", txtBAC.Text);
                    WriteElementWithValue(writer, "CMS_BCM", txtBCM.Text);
                    WriteElementWithValue(writer, "CMS_BAI", txtBAI.Text);

                    WriteElementWithValue(writer, "CSO_1", txtCSO1.Text);
                    WriteElementWithValue(writer, "CSO_2", txtCSO2.Text);
                    WriteElementWithValue(writer, "CSO_3", txtCSO3.Text);
                    WriteElementWithValue(writer, "CSO_4", txtCSO4.Text);


                    SetComboBoxValue(writer, cbSuper1, "CUS_SUPER_1");
                    SetComboBoxValue(writer, cbSuper2, "CUS_SUPER_2");
                    SetComboBoxValue(writer, cbSuper3, "CUS_SUPER_3");
                    SetComboBoxValue(writer, cbSuper4, "CUS_SUPER_4");
                    SetComboBoxValue(writer, cbUltimate1, "CUS_ULTIMATE_1");
                    SetComboBoxValue(writer, cbUltimate2, "CUS_ULTIMATE_2");
                    SetComboBoxValue(writer, cbEvasive, "CUS_EVASIVE");
                    SetComboBoxValue(writer, cbBlast, "CUS_BLAST");
                    SetComboBoxValue(writer, cbAwaken, "CUS_AWAKEN");

                    WriteElementWithValue(writer, "MSG_CHARACTER_NAME", txtMSG1.Text);
                    WriteElementWithValue(writer, "MSG_COSTUME_NAME", txtMSG2.Text);

                    writer.WriteEndElement(); // Close XV2MOD
                    writer.WriteEndDocument(); // Close the document
                }

                Console.WriteLine("XML file created at: " + xmlFilePath);
                Directory.CreateDirectory("C:/XV2CharaCreatorTemp/chara");
                Directory.Move(txtFolder.Text, @"C:/XV2CharaCreatorTemp/chara/" + txtCharID.Text);
                Directory.CreateDirectory("C:/XV2CharaCreatorTemp/ui/texture/CHARA01");
                File.Move(textBox1.Text, @"C:/XV2CharaCreatorTemp/ui/texture/CHARA01/" + txtCharID.Text + "_000.DDS");

                ZipFile.CreateFromDirectory(@"C:/XV2CharaCreatorTemp/", sfd.FileName);

                if (File.Exists(xmlFilePath))
                    File.Delete(xmlFilePath);

                MessageBox.Show("Mod Created Successfully!", "Success", MessageBoxButtons.OK, MessageBoxIcon.Information);
                Directory.Delete("C:/XV2CharaCreatorTemp", true);

            }

        }
        // Helper method to write an element with a value
        static void WriteElementWithValue(XmlWriter writer, string elementName, string value)
        {
            writer.WriteStartElement(elementName);
            writer.WriteAttributeString("value", value);
            writer.WriteEndElement();
        }
        private void SetComboBoxValue(XmlWriter xmlWriter, ComboBox comboBox, string elementName)
        {
            if (comboBox.SelectedIndex >= 0)
                WriteElementWithValue(xmlWriter, elementName, comboBox.SelectedItem.ToString());
            else
                WriteElementWithValue(xmlWriter, elementName, "");
        }


        private void btnGenID_Click(object sender, EventArgs e)
        {
            const string chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
            Random random = new Random();
            char[] id = new char[3];
            string generatedID;

            for (int i = 0; i < 3; i++)
            {
                id[i] = chars[random.Next(chars.Length)];
            }

            generatedID = new string(id);
            txtCharID.Text = generatedID;

        }

        private void btnFolder_Click(object sender, EventArgs e)
        {
            if (txtCharID.Text.Length == 3)
            {
                FolderBrowserDialog fbd = new FolderBrowserDialog();
                fbd.Description = $"Select {txtCharID.Text} Folder";
                fbd.UseDescriptionForTitle = true;

                if (fbd.ShowDialog() == DialogResult.OK)
                {
                    string selectedPath = fbd.SelectedPath;
                    string selectedDirName = Path.GetFileName(selectedPath);

                    if (selectedDirName == txtCharID.Text && Directory.Exists(selectedPath))
                    {
                        txtFolder.Text = selectedPath;
                    }
                    else
                    {
                        MessageBox.Show($"Please select the folder matching {txtCharID.Text}.", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    }
                }
            }
        }
        private void Form1_Load(object sender, EventArgs e)
        {
            if (!Directory.Exists("C:/Games/DB Xenoverse 2/data"))
            {
                FolderBrowserDialog fbd = new FolderBrowserDialog();
                fbd.UseDescriptionForTitle = true;
                fbd.Description = "Select XV2 Data Folder, if you can't find it PLEASE START XV2Reborn FIRST";

                if (fbd.ShowDialog() == DialogResult.OK)
                {
                    string datapath = fbd.SelectedPath;
                    string CUSFileName;
                    CUSRegistry CUSfile = new CUSRegistry();

                    // Load the CUS File
                    CUSFileName = datapath + @"/system/custom_skill.cus";
                    CUSfile.readCUS(CUSFileName);
                    CUSfile.BuildRegistry();

                    cbSuper1.Items.AddRange((object[])CUSfile.getSkillList(0));
                    cbSuper2.Items.AddRange((object[])CUSfile.getSkillList(0));
                    cbSuper3.Items.AddRange((object[])CUSfile.getSkillList(0));
                    cbSuper4.Items.AddRange((object[])CUSfile.getSkillList(0));
                    cbUltimate1.Items.AddRange((object[])CUSfile.getSkillList(1));
                    cbUltimate2.Items.AddRange((object[])CUSfile.getSkillList(1));
                    cbEvasive.Items.AddRange((object[])CUSfile.getSkillList(2));
                    cbBlast.Items.AddRange((object[])CUSfile.getSkillList(4));
                    cbAwaken.Items.AddRange((object[])CUSfile.getSkillList(5));
                }

            }
            else
            {
                string datapath = "C:/Games/DB Xenoverse 2/data";
                string CUSFileName;
                CUSRegistry CUSfile = new CUSRegistry();

                // Load the CUS File
                CUSFileName = datapath + @"/system/custom_skill.cus";
                CUSfile.readCUS(CUSFileName);
                CUSfile.BuildRegistry();

                cbSuper1.Items.AddRange((object[])CUSfile.getSkillList(0));
                cbSuper2.Items.AddRange((object[])CUSfile.getSkillList(0));
                cbSuper3.Items.AddRange((object[])CUSfile.getSkillList(0));
                cbSuper4.Items.AddRange((object[])CUSfile.getSkillList(0));
                cbUltimate1.Items.AddRange((object[])CUSfile.getSkillList(1));
                cbUltimate2.Items.AddRange((object[])CUSfile.getSkillList(1));
                cbEvasive.Items.AddRange((object[])CUSfile.getSkillList(2));
                cbBlast.Items.AddRange((object[])CUSfile.getSkillList(4));
                cbAwaken.Items.AddRange((object[])CUSfile.getSkillList(5));
            }
        }
        private void txtAuraID_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (!char.IsDigit(e.KeyChar) && e.KeyChar != (char)8)  // Allow digits and Backspace
            {
                e.Handled = true;  // Mark the event as handled, preventing non-digit input
            }
        }

        private void txtVersion_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (!char.IsDigit(e.KeyChar) && e.KeyChar != (char)8)  // Allow digits and Backspace
            {
                e.Handled = true;  // Mark the event as handled, preventing non-digit input
            }
        }

        private void button1_Click(object sender, EventArgs e)
        {
            OpenFileDialog ofd = new OpenFileDialog();
            ofd.Title = $"Select .DDS file";
            ofd.Filter = "Direct Draw Surface files (*.DDS)|*.DDS";
            if (ofd.ShowDialog() == DialogResult.OK && File.Exists(ofd.FileName))
            {
                textBox1.Text = ofd.FileName;
            }
        }

        private void sToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Close();
        }
    }
}