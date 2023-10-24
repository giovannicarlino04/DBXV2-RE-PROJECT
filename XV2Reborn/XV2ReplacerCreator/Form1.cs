using System.IO.Compression;
using System.Xml;

namespace XV2ReplacerCreator
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
                && sfd.ShowDialog() == DialogResult.OK)
            {
                // Specify the path where you want to save the XML file
                string xmlFilePath = txtFolder.Text + "/xv2mod.xml";

                if (File.Exists(xmlFilePath))
                    File.Delete(xmlFilePath);

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
                    writer.WriteAttributeString("type", "REPLACER");

                    WriteElementWithValue(writer, "MOD_NAME", txtName.Text);
                    WriteElementWithValue(writer, "MOD_AUTHOR", txtAuthor.Text);
                    WriteElementWithValue(writer, "MOD_VERSION", txtVersion.Text);

                    writer.WriteEndElement(); // Close XV2MOD
                    writer.WriteEndDocument(); // Close the document
                }

                Console.WriteLine("XML file created at: " + xmlFilePath);

                ZipFile.CreateFromDirectory(txtFolder.Text, sfd.FileName);

                if (File.Exists(xmlFilePath))
                    File.Delete(xmlFilePath);

                MessageBox.Show("Mod Created Successfully!", "Success", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }

            // Helper method to write an element with a value
            static void WriteElementWithValue(XmlWriter writer, string elementName, string value)
            {
                writer.WriteStartElement(elementName);
                writer.WriteAttributeString("value", value);
                writer.WriteEndElement();
            }
        }

        private void btnFolder_Click(object sender, EventArgs e)
        {
            FolderBrowserDialog fbd = new FolderBrowserDialog();
            fbd.Description = "Select Mod Folder";
            fbd.UseDescriptionForTitle = true;

            if (fbd.ShowDialog() == DialogResult.OK &&
                Directory.Exists(fbd.SelectedPath))
                txtFolder.Text = fbd.SelectedPath;
        }

        private void txtVersion_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (!char.IsDigit(e.KeyChar) && e.KeyChar != (char)8)  // Allow digits and Backspace
            {
                e.Handled = true;  // Mark the event as handled, preventing non-digit input
            }
        }
    }
}