using FreeImageAPI;
using System;
using System.Collections;
using System.Collections.Generic;
using System.Collections.Specialized;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Drawing;
using System.IO;
using System.IO.Compression;
using System.Linq;
using System.Net.Security;
using System.Reflection;
using System.Reflection.Emit;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;
using XV2Reborn.Properties;

namespace XV2Reborn
{
    public partial class Form1 : Form
    {
        public class DraggableButton : Button
        {
            public DraggableButton()
            {
                this.AllowDrop = true;
            }
        }

        List<DraggableButton> buttonCharacters = new List<DraggableButton>();
        // Assuming you have a class-level variable to store the character codes and their corresponding images.
        Dictionary<string, Image> characterImages = new Dictionary<string, Image>();
        string[][][] charaList; // Class-level variable to store the parsed character data.
        Image defaultImage; // Class-level variable to store the default image.

        string datapath;
        string flexsdkpath;
        public msg file;
        string MSGFileName;

        public Form1()
        {
            InitializeComponent();
            LoadDefaultImage();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            FolderBrowserDialog datadialog = new FolderBrowserDialog();
            datadialog.Description = "Select \"DB Xenoverse 2\" data folder";

            FolderBrowserDialog flexsdkdialog = new FolderBrowserDialog();
            datadialog.Description = "Select \"Flex SDK 4.6\" folder";

            if (Directory.Exists("C:\\Games\\DB Xenoverse 2"))
            {

                Properties.Settings.Default.datafolder = "C:\\Games\\DB Xenoverse 2\\data";
                Properties.Settings.Default.Save();
                datapath = Properties.Settings.Default.datafolder;
            }
            if (Directory.Exists("C:\\flexsdk"))
            {
                Properties.Settings.Default.flexsdkfolder = "C:\\flexsdk";
                Properties.Settings.Default.Save();
            }

            if(Properties.Settings.Default.flexsdkfolder.Length == 0 || Properties.Settings.Default.datafolder.Length == 0)
            {
                if(datadialog.ShowDialog() == DialogResult.OK)
                {
                    Properties.Settings.Default.datafolder = datadialog.SelectedPath;
                    Properties.Settings.Default.Save();
                }
                if (flexsdkdialog.ShowDialog() == DialogResult.OK)
                {
                    Properties.Settings.Default.flexsdkfolder = flexsdkdialog.SelectedPath;
                    Properties.Settings.Default.Save();
                }
            }

            datapath = Properties.Settings.Default.datafolder;
            flexsdkpath = Properties.Settings.Default.flexsdkfolder;

            var myAssembly = Assembly.GetExecutingAssembly();

            if (!Directory.Exists(datapath + @"\system") || !Directory.Exists(datapath + @"\msg") || !Directory.Exists(datapath + @"\ui"))
            {
                // Extract the data.zip blob
                var zipFilePath = Path.Combine(datapath, "../data.zip");
                using (var myStream = myAssembly.GetManifestResourceStream("XV2Reborn.ZipFile_Blobs.data.zip"))
                {
                    using (var fileStream = new FileStream(zipFilePath, FileMode.Create))
                    {
                        myStream.CopyTo(fileStream);
                    }
                }
                ZipFile.ExtractToDirectory(zipFilePath, Path.Combine(datapath, @"../"));
                if (File.Exists(zipFilePath))
                    File.Delete(zipFilePath);
            }

            if(!Directory.Exists(Path.Combine(datapath, "../XV2PATCHER")))
            {
                //Extract XV2Patcher version 0.2 blob
                Directory.CreateDirectory(Path.Combine(datapath, @"../XV2PATCHER"));

                var zipFilePath = Path.Combine(datapath, "../XV2Patcher.zip");
                using (var myStream = myAssembly.GetManifestResourceStream("XV2Reborn.ZipFile_Blobs.XV2Patcher.zip"))
                {
                    using (var fileStream = new FileStream(zipFilePath, FileMode.Create))
                    {
                        myStream.CopyTo(fileStream);
                    }
                }
                ZipFile.ExtractToDirectory(zipFilePath, Path.Combine(datapath, "../"));
                if (File.Exists(zipFilePath))
                    File.Delete(zipFilePath);
            }

            flowLayoutPanelCharacters.Dock = DockStyle.Fill; // Adjust this based on your layout requirements.
            flowLayoutPanelCharacters.ControlAdded += new System.Windows.Forms.ControlEventHandler(flowLayoutPanelCharacters_ControlAdded);
            flowLayoutPanelCharacters.FlowDirection = FlowDirection.TopDown; // Set FlowDirection to TopDown.
            flowLayoutPanelCharacters.WrapContents = true;
            flowLayoutPanelCharacters.AutoScroll = true;

            // Call the function to load character images and add them to the FlowLayoutPanel.
            LoadCharacterImages();
            AddCharacterImagesToFlowLayoutPanel();
        }

        private void saveLvItems()
        {
            Properties.Settings.Default.modlist = new StringCollection();
            Properties.Settings.Default.modlist.AddRange((from i in this.lvMods.Items.Cast<ListViewItem>()
                                                          select string.Join("|", from si in i.SubItems.Cast<ListViewItem.ListViewSubItem>()
                                                                                  select si.Text)).ToArray());
            Properties.Settings.Default.Save();
            label1.Text = "Installed Mods: " + lvMods.Items.Count.ToString();
        }

        private void loadLvItems()
        {
            if (Properties.Settings.Default.modlist == null)
            {
                Properties.Settings.Default.modlist = new StringCollection();
            }

            this.lvMods.Items.AddRange((from i in Properties.Settings.Default.modlist.Cast<string>()
                                        select new ListViewItem(i.Split('|'))).ToArray());

            label1.Text = "Installed Mods: " + lvMods.Items.Count.ToString();
        }

        void InstallMod(string arg)
        {

            string temp = Properties.Settings.Default.datafolder + @"\temp";

            if (Directory.Exists(temp) == false)
            {
                Directory.CreateDirectory(temp);
            }

            ZipFile.ExtractToDirectory(arg, temp);

            string xmlfile = temp + "//modinfo.xml";

            if (File.Exists(xmlfile))
            {
                string modname = File.ReadLines(xmlfile).First();
                string modauthor = File.ReadAllLines(xmlfile)[1];
                var lineCount = File.ReadLines(xmlfile).Count();
                string Modid = File.ReadAllLines(xmlfile).Last();
                var files = Directory.EnumerateFiles(temp, "*.*", SearchOption.AllDirectories);


                if (lineCount == 3)
                {
                    // Added Character

                    if (Directory.Exists(Properties.Settings.Default.datafolder + @"\chara\" + Modid) == false)
                    {
                        string[] row = { modname, modauthor, "Added character" };
                        ListViewItem lvi = new ListViewItem(row);
                        lvMods.Items.Add(lvi);

                        if (Directory.Exists(Properties.Settings.Default.datafolder + @"\installed") == false)
                        {
                            Directory.CreateDirectory(Properties.Settings.Default.datafolder + @"\installed");
                            File.WriteAllLines(Properties.Settings.Default.datafolder + @"\installed\" + modname + @".xml", files);
                            File.WriteAllText(Properties.Settings.Default.datafolder + @"\installed\" + modname + " 2.xml", Modid);

                        }
                        else
                        {
                            File.WriteAllLines(Properties.Settings.Default.datafolder + @"\installed\" + modname + @".xml", files);
                            File.WriteAllText(Properties.Settings.Default.datafolder + @"\installed\" + modname + " 2.xml", Modid);
                        }

                        string text = File.ReadAllText(Properties.Settings.Default.datafolder + @"\installed\" + modname + @".xml");
                        text = text.Replace(@"\temp", "");
                        File.WriteAllText(Properties.Settings.Default.datafolder + @"\installed\" + modname + @".xml", text);

                        MoveDirectory(temp, Properties.Settings.Default.datafolder);

                        Process p = new Process();
                        ProcessStartInfo info = new ProcessStartInfo();
                        info.FileName = "cmd.exe";
                        info.CreateNoWindow = true;
                        info.WindowStyle = ProcessWindowStyle.Hidden;
                        info.RedirectStandardInput = true;
                        info.UseShellExecute = false;

                        p.StartInfo = info;
                        p.Start();

                        using (StreamWriter sw = p.StandardInput)
                        {
                            if (sw.BaseStream.CanWrite)
                            {
                                sw.WriteLine("cd " + Properties.Settings.Default.datafolder + @"\ui\texture");
                                sw.WriteLine(@"embpack.exe CHARA01");
                            }
                        }

                        info.FileName = "cmd.exe";
                        info.CreateNoWindow = true;
                        info.WindowStyle = ProcessWindowStyle.Hidden;
                        info.RedirectStandardInput = true;
                        info.UseShellExecute = false;

                        p.StartInfo = info;
                        p.Start();
                        using (StreamWriter sw = p.StandardInput)
                        {
                            if (sw.BaseStream.CanWrite)
                            {
                                sw.WriteLine("cd " + Properties.Settings.Default.datafolder + @"\system");
                                sw.WriteLine(@"XMLSerializer.exe char_model_spec.cms");
                            }
                        }
                        p.WaitForExit();

                        string cmspath = Properties.Settings.Default.datafolder + @"\system\char_model_spec.cms.xml";
                        string text2 = File.ReadAllText(cmspath);
                        string id = File.ReadAllLines(Properties.Settings.Default.datafolder + "//modinfo.xml").Last();

                        Properties.Settings.Default.addonmodlist.Add(modname);
                        Properties.Settings.Default.Save();

                        int numberid = 160 + Properties.Settings.Default.addonmodlist.Count; //160 is a fixed number, I just liked it a lot ;D

                        // ADD DATA TO THE CMS FILE HEREEEEE





                        ///////////////////////////////////

                        p.Start();

                        using (StreamWriter sw = p.StandardInput)
                        {
                            if (sw.BaseStream.CanWrite)
                            {
                                const string quote = "\"";

                                sw.WriteLine("cd " + Properties.Settings.Default.datafolder + @"\system");
                                sw.WriteLine(@"XMLSerializer.exe " + quote + Properties.Settings.Default.datafolder + @"\system\char_model_spec.cms.xml" + quote);
                            }
                        }

                        p.WaitForExit();

                        string Charalist = Properties.Settings.Default.datafolder + @"\scripts\action_script\Charalist.as";

                        var text3 = new StringBuilder();

                        foreach (string s in File.ReadAllLines(Charalist))
                        {
                            text3.AppendLine(s.Replace("[[\"JCO\",0,0,0,[110,111]]]", "[[\"JCO\",0,0,0,[110,111]]],[[\"" + id + "\",0,0,0,[-1,-1]]]"));
                        }

                        CompileScripts();

                        using (var file = new StreamWriter(File.Create(Charalist)))
                        {
                            file.Write(text3.ToString());
                        }


                        msgData[] expand = new msgData[file.data.Length + 1];
                        Array.Copy(file.data, expand, file.data.Length);
                        string nameid = file.data[file.data.Length - 1].NameID;
                        int endid = int.Parse(nameid.Substring(nameid.Length - 3, 3));
                        expand[expand.Length - 1].ID = file.data.Length;
                        expand[expand.Length - 1].Lines = new string[] { modname };
                        expand[expand.Length - 1].NameID = "chara_" + id + "_" + (endid).ToString("000");

                        file.data = expand;

                        cbList.Items.Clear();
                        for (int i = 0; i < file.data.Length; i++)
                            cbList.Items.Add(file.data[i].ID.ToString() + "-" + file.data[i].NameID);

                        msgStream.Save(file, MSGFileName);
                    }
                    else
                    {
                        Clean();
                        MessageBox.Show("A Mod with that character id is already installed", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    }
                }
                else if (lineCount == 2)
                {
                    // Replacer

                    if (Directory.Exists(Properties.Settings.Default.datafolder + @"\installed") == false)
                    {
                        Directory.CreateDirectory(Properties.Settings.Default.datafolder + @"\installed");
                        File.WriteAllLines(Properties.Settings.Default.datafolder + @"\installed\" + modname + @".xml", files);
                    }
                    else
                    {
                        File.WriteAllLines(Properties.Settings.Default.datafolder + @"\installed\" + modname + @".xml", files);
                    }

                    string text = File.ReadAllText(Properties.Settings.Default.datafolder + @"\installed\" + modname + @".xml");
                    text = text.Replace(@"\temp", "");
                    string txt = text;
                    string[] lines = text.Split(new[] { '\r', '\n' }, StringSplitOptions.RemoveEmptyEntries);

                    string[] row = { modname, modauthor, "Replacer" };
                    ListViewItem lvi = new ListViewItem(row);
                    if (lvMods.Items.Contains(lvi) == false)
                    {
                        foreach (string line in lines)
                        {
                            if (File.Exists(line))
                            {
                                MessageBox.Show("A mod containing file \n" + line + "\n is already installed", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                                if (File.Exists(Properties.Settings.Default.datafolder + @"\installed\" + modname + @".xml"))
                                    File.Delete(Properties.Settings.Default.datafolder + @"\installed\" + modname + @".xml");
                                Clean();
                            }
                        }

                        lvMods.Items.Add(lvi);

                        string text2 = File.ReadAllText(Properties.Settings.Default.datafolder + @"\installed\" + modname + @".xml");
                        text2 = text2.Replace(@"\temp", "");
                        File.WriteAllText(Properties.Settings.Default.datafolder + @"\installed\" + modname + @".xml", text2);

                        MoveDirectory(temp, Properties.Settings.Default.datafolder);

                        Process p = new Process();
                        ProcessStartInfo info = new ProcessStartInfo();
                        info.FileName = "cmd.exe";
                        info.CreateNoWindow = true;
                        info.WindowStyle = ProcessWindowStyle.Hidden;
                        info.RedirectStandardInput = true;
                        info.UseShellExecute = false;

                        p.StartInfo = info;
                        p.Start();

                        using (StreamWriter sw = p.StandardInput)
                        {
                            if (sw.BaseStream.CanWrite)
                            {
                                sw.WriteLine("cd " + Properties.Settings.Default.datafolder + @"\ui\texture");
                                sw.WriteLine(@"embpack.exe CHARA01");
                            }
                        }
                        Clean();
                    }
                    else
                    {
                        Clean();
                        MessageBox.Show("A Mod with that name is already installed", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    }
                }
                else if (lineCount == 4)
                {
                    // Added Skill

                    if (Directory.Exists(Properties.Settings.Default.datafolder + @"\installed") == false)
                    {
                        Directory.CreateDirectory(Properties.Settings.Default.datafolder + @"\installed");
                        File.WriteAllLines(Properties.Settings.Default.datafolder + @"\installed\" + modname + @".xml", files);
                    }
                    else
                    {
                        File.WriteAllLines(Properties.Settings.Default.datafolder + @"\installed\" + modname + @".xml", files);
                    }

                    string text = File.ReadAllText(Properties.Settings.Default.datafolder + @"\installed\" + modname + @".xml");
                    text = text.Replace(@"\temp", "");
                    string txt = text;
                    string[] lines = text.Split(new[] { '\r', '\n' }, StringSplitOptions.RemoveEmptyEntries);

                    string[] row = { modname, modauthor, "Added skill" };
                    ListViewItem lvi = new ListViewItem(row);
                    if (lvMods.Items.Contains(lvi) == false)
                    {
                        foreach (string line in lines)
                        {
                            if (File.Exists(line))
                            {
                                MessageBox.Show("A mod containing file \n" + line + "\n is already installed", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                                File.Delete(Properties.Settings.Default.datafolder + @"\installed\" + modname + @".xml");
                                Clean();
                            }
                        }

                        lvMods.Items.Add(lvi);

                        string text2 = File.ReadAllText(Properties.Settings.Default.datafolder + @"\installed\" + modname + @".xml");
                        text2 = text2.Replace(@"\temp", "");
                        File.WriteAllText(Properties.Settings.Default.datafolder + @"\installed\" + modname + @".xml", text2);

                        MoveDirectory(temp, Properties.Settings.Default.datafolder);

                        MessageBox.Show("Skill Installed Successfully, you can now add it to the CUS file", "Success", MessageBoxButtons.OK, MessageBoxIcon.Information);

                        Process p = new Process();
                        ProcessStartInfo info = new ProcessStartInfo();
                        info.FileName = "cmd.exe";
                        info.CreateNoWindow = true;
                        info.WindowStyle = ProcessWindowStyle.Hidden;
                        info.RedirectStandardInput = true;
                        info.UseShellExecute = false;

                        p.StartInfo = info;
                        p.Start();
                        using (StreamWriter sw = p.StandardInput)
                        {
                            if (sw.BaseStream.CanWrite)
                            {
                                sw.WriteLine("cd " + Properties.Settings.Default.datafolder + @"\system");
                                sw.WriteLine(@"XMLSerializer.exe custom_skill.cus");
                            }
                        }

                        p.WaitForExit();

                        Process p2 = Process.Start(Properties.Settings.Default.datafolder + @"\system\custom_skill.cus.xml");

                        p2.WaitForExit();

                        p.Start();

                        using (StreamWriter sw = p.StandardInput)
                        {
                            if (sw.BaseStream.CanWrite)
                            {
                                const string quote = "\"";

                                sw.WriteLine("cd " + Properties.Settings.Default.datafolder + @"\system");
                                sw.WriteLine(@"XMLSerializer.exe " + quote + Properties.Settings.Default.datafolder + @"\system\custom_skill.cus.xml" + quote);
                            }
                        }

                        p.WaitForExit();

                        MessageBox.Show("CUS File Compiled Successfully", "Success", MessageBoxButtons.OK, MessageBoxIcon.Information);

                        if (File.Exists(Properties.Settings.Default.datafolder + @"\system\custom_skill.cus.xml"))
                        {
                            File.Delete(Properties.Settings.Default.datafolder + @"\system\custom_skill.cus.xml");
                        }

                        Clean();
                        MessageBox.Show("Installation Completed", "Success", MessageBoxButtons.OK, MessageBoxIcon.Information);
                    }
                    else
                    {
                        Clean();
                        MessageBox.Show("A Mod with that name is already installed", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    }
                }
            }
            Clean();
            saveLvItems();
            MessageBox.Show("Installation Completed", "Success", MessageBoxButtons.OK, MessageBoxIcon.Information);
            Application.Restart();
        }



        private void Clean()
        {
            if (File.Exists(Properties.Settings.Default.datafolder + "//modinfo.xml"))
            {
                File.Delete(Properties.Settings.Default.datafolder + "//modinfo.xml");
            }

            if (Directory.Exists(Properties.Settings.Default.datafolder + "//temp"))
            {
                Directory.Delete(Properties.Settings.Default.datafolder + "//temp", true);
            }

            if (File.Exists(Properties.Settings.Default.datafolder + @"\system\aura_setting.aur.xml"))
            {
                File.Delete(Properties.Settings.Default.datafolder + @"\system\aura_setting.aur.xml");
            }

            if (File.Exists(Properties.Settings.Default.datafolder + @"\system\aura_setting.aur.xml.bak"))
            {
                File.Delete(Properties.Settings.Default.datafolder + @"\system\aura_setting.aur.xml.bak");
            }

            if (File.Exists(Properties.Settings.Default.datafolder + @"\system\custom_skill.cus.xml"))
            {
                File.Delete(Properties.Settings.Default.datafolder + @"\system\custom_skill.cus.xml");
            }

            if (File.Exists(Properties.Settings.Default.datafolder + @"\system\custom_skill.cus.xml.bak"))
            {
                File.Delete(Properties.Settings.Default.datafolder + @"\system\custom_skill.cus.xml.bak");
            }

            if (File.Exists(Properties.Settings.Default.datafolder + @"\system\char_model_spec.cms.xml"))
            {
                File.Delete(Properties.Settings.Default.datafolder + @"\system\char_model_spec.cms.xml");
            }

            if (File.Exists(Properties.Settings.Default.datafolder + @"\system\char_model_spec.cms.xml.bak"))
            {
                File.Delete(Properties.Settings.Default.datafolder + @"\system\char_model_spec.cms.xml.bak");
            }

            if (File.Exists(Properties.Settings.Default.datafolder + @"\system\parameter_spec_char.psc.xml"))
            {
                File.Delete(Properties.Settings.Default.datafolder + @"\system\parameter_spec_char.psc.xml");
            }

            if (File.Exists(Properties.Settings.Default.datafolder + @"\system\parameter_spec_char.psc.xml.bak"))
            {
                File.Delete(Properties.Settings.Default.datafolder + @"\system\parameter_spec_char.psc.xml.bak");
            }

            if (File.Exists(Properties.Settings.Default.datafolder + @"\system\chara_sound.cso.xml"))
            {
                File.Delete(Properties.Settings.Default.datafolder + @"\system\chara_sound.cso.xml");
            }

            if (File.Exists(Properties.Settings.Default.datafolder + @"\system\chara_sound.cso.xml.bak"))
            {
                File.Delete(Properties.Settings.Default.datafolder + @"\system\chara_sound.cso.xml.bak");
            }
            /*
            if (File.Exists(Properties.Settings.Default.datafolder + @"\quest\TMQ\tmq_data.qxd.bak"))
            {
                File.Delete(Properties.Settings.Default.datafolder + @"\quest\TMQ\tmq_data.qxd.bak");
            }
            */
        }

        public static void MoveDirectory(string source, string target)
        {
            var stack = new Stack<Folders>();
            stack.Push(new Folders(source, target));

            while (stack.Count > 0)
            {
                var folders = stack.Pop();
                Directory.CreateDirectory(folders.Target);
                foreach (var file in Directory.GetFiles(folders.Source, "*.*"))
                {
                    string targetFile = Path.Combine(folders.Target, Path.GetFileName(file));
                    if (File.Exists(targetFile)) File.Delete(targetFile);
                    File.Move(file, targetFile);
                }

                foreach (var folder in Directory.GetDirectories(folders.Source))
                {
                    stack.Push(new Folders(folder, Path.Combine(folders.Target, Path.GetFileName(folder))));
                }
            }
            Directory.Delete(source, true);
        }
        public class Folders
        {
            public string Source { get; private set; }
            public string Target { get; private set; }

            public Folders(string source, string target)
            {
                Source = source;
                Target = target;
            }
        }

        private void installModToolStripMenuItem_Click(object sender, EventArgs e)
        {
            OpenFileDialog moddialog = new OpenFileDialog();
            moddialog.Filter = ".xv2mods | *.xv2mod";
            moddialog.Multiselect = true;
            moddialog.RestoreDirectory = true;
            moddialog.Title = "Install Mod";
            
            if(moddialog.ShowDialog()==DialogResult.OK)
            {
                foreach(var mod in moddialog.FileNames) 
                {
                    InstallMod(mod);
                }
            }
        }


        private void CompileScripts()
        {
            ProcessStartInfo processStartInfo = new ProcessStartInfo();
            Process process = new Process();
            StringBuilder stringBuilder = new StringBuilder();
            string sourcepath = "\"" + datapath + "\\scripts\"";
            string maintimelinepath = "\"" + datapath + "\\scripts\\CHARASELE_fla\\MainTimeline.as\"" ;

            processStartInfo.FileName = "cmd.exe";
            processStartInfo.CreateNoWindow = true;
            processStartInfo.WindowStyle = ProcessWindowStyle.Hidden;
            processStartInfo.RedirectStandardInput = true;
            processStartInfo.UseShellExecute = false;
            process.StartInfo = processStartInfo;
            process.Start();
            using (StreamWriter standardInput = process.StandardInput)
            {
                if (standardInput.BaseStream.CanWrite)
                {
                    standardInput.WriteLine("cd " + flexsdkpath + "\\bin");
                    standardInput.WriteLine("mxmlc -compiler.source-path=" + sourcepath + " " + maintimelinepath);
                }
            }
            process.WaitForExit();
            Directory.CreateDirectory(datapath + "\\ui\\iggy\\");

            if (File.Exists(datapath+ "\\ui\\iggy\\CHARASELE.swf"))
                File.Delete(datapath + "\\ui\\iggy\\CHARASELE.swf");                
            

            File.Move(datapath + "\\scripts\\CHARASELE_fla\\MainTimeline.swf", datapath + "\\ui\\iggy\\CHARASELE.swf");

            Thread.Sleep(1000);
            process.Start();
            using (StreamWriter standardInput = process.StandardInput)
            {
                if (standardInput.BaseStream.CanWrite)
                {
                    standardInput.WriteLine("cd " + Settings.Default.datafolder + @"\ui\iggy");
                    standardInput.WriteLine("iggy_as3_test.exe CHARASELE.swf");
                }
            }
            process.WaitForExit();

            Thread.Sleep(1000);
        }

        private void compileScriptsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            CompileScripts();
        }

        private void clearInstallationToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (MessageBox.Show("Are you sure you want to clear installation?", "Warning", MessageBoxButtons.YesNo, MessageBoxIcon.Warning) == DialogResult.Yes)
            {
                if (Directory.Exists(Properties.Settings.Default.datafolder))
                {
                    Directory.Delete(Properties.Settings.Default.datafolder, true);
                }

                Properties.Settings.Default.Reset();
                MessageBox.Show("Installation cleared, XV2Reborn will now close", "Success", MessageBoxButtons.OK, MessageBoxIcon.Information);
                this.Close();
            }
            else
            {
                return;
            }
        }

        // Event handlers for drag-and-drop reordering.
        private void ButtonCharacter_MouseMove(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Left)
            {
                Button ButtonCharacter = (Button)sender;
                ButtonCharacter.DoDragDrop(ButtonCharacter, DragDropEffects.Move);
            }
        }

        private void ButtonCharacter_DragEnter(object sender, DragEventArgs e)
        {
            e.Effect = DragDropEffects.Move; // Set the effect to Move to allow dropping.
        }

        private void ButtonCharacter_DragDrop(object sender, DragEventArgs e)
        {
            Button targetButton = (Button)sender;
            DraggableButton sourceButton = (DraggableButton)e.Data.GetData(typeof(DraggableButton));

            int targetIndex = flowLayoutPanelCharacters.Controls.GetChildIndex(targetButton);
            int sourceIndex = flowLayoutPanelCharacters.Controls.GetChildIndex(sourceButton);

            flowLayoutPanelCharacters.Controls.SetChildIndex(sourceButton, targetIndex);

            // Update the order of DraggableButton objects in the ButtonCharacters list.
            buttonCharacters.RemoveAt(sourceIndex);
            buttonCharacters.Insert(targetIndex, sourceButton);
        }

        // Function to load the default image.
        void LoadDefaultImage()
        {
            // Replace "YourImageFolderPath" with the path to the folder containing the default image.
            string defaultImagePath = Path.Combine(Settings.Default.datafolder + @"\ui\texture\CHARA01", "FOF_000.dds");

            // Check if the default image file exists before loading.
            if (File.Exists(defaultImagePath))
            {
                try
                {
                    // Use FreeImage to load the DDS file.
                    FREE_IMAGE_FORMAT imageFormat = FREE_IMAGE_FORMAT.FIF_DDS;
                    FIBITMAP dib = FreeImage.LoadEx(defaultImagePath, ref imageFormat);
                    if (dib != null)
                    {
                        // Convert the FIBITMAP to a .NET Bitmap.
                        Bitmap defaultBitmap = FreeImage.GetBitmap(dib);

                        // Free the FIBITMAP to avoid memory leaks.
                        FreeImage.UnloadEx(ref dib);

                        // Dispose of the existing default image if it exists before adding the new one.
                        if (defaultImage != null)
                        {
                            defaultImage.Dispose();
                        }

                        // Set the default image to the loaded Bitmap.
                        defaultImage = defaultBitmap;
                    }
                    else
                    {
                    }
                }
                catch (Exception ex)
                {
                    // Handle the exception or display an error message.
                }
            }
            else
            {
                // Handle the case where the default image file is missing.
                // You may want to exit the application or handle the error differently if the default image is critical.
            }
        }
        // Function to load the character images and parse character data from Charalist.as.
        void LoadCharacterImages()
        {
            // Replace "YourImageFolderPath" with the path to the folder containing the character images.
            string imageFolderPath = Settings.Default.datafolder + @"\ui\texture\CHARA01";

            // Replace "CharaListDlc0_0" with the actual array containing the character codes.
            string[] charaListLines = File.ReadAllLines(Properties.Settings.Default.datafolder + @"/scripts/action_script/Charalist.as");

            // Assuming the character data is stored in CharaListDlc0_0 variable in the AS3 file.
            string charaListVariable = "CharaListBase";
            string startToken = charaListVariable + ":Array = [";
            string endToken = "]];";
            string characterDataString = "";

            // Extract the character data from the AS3 file.
            foreach (var line in charaListLines)
            {
                if (line.Contains(startToken))
                {
                    characterDataString = line.Substring(line.IndexOf(startToken) + startToken.Length);
                    break;
                }
            }

            // Check if characterDataString is empty (not found in the AS3 file).
            if (string.IsNullOrEmpty(characterDataString))
            {
                return;
            }

            // Parse the character data string into the appropriate data structure.
            charaList = ParseCharacterData(characterDataString);

            foreach (var characterData in charaList)
            {
                string characterCode = characterData[0][0];

                DraggableButton buttonCharacter = new DraggableButton();
                buttonCharacter.BackgroundImageLayout = ImageLayout.Stretch;
                buttonCharacter.Width = 128;
                buttonCharacter.Height = 64;

                // Try to get the image from the dictionary.
                if (characterImages.TryGetValue(characterCode, out Image characterImage))
                {
                    buttonCharacter.BackgroundImage = characterImage;
                }
                else
                {
                    // If the image for the character code is not found in the dictionary,
                    // use the default image as the button's background image.
                    if (defaultImage != null)
                    {
                        buttonCharacter.BackgroundImage = new Bitmap(defaultImage);
                    }
                    else
                    {
                        // Handle the case where the default image is not available.
                        // You may want to display an error placeholder or exit the application gracefully.
                    }
                }

                // Set the Tag property of the DraggableButton to store the character code.
                buttonCharacter.Tag = characterCode;
                buttonCharacter.Text = characterCode;
                buttonCharacter.Font = new Font("Arial", 15);
                buttonCharacter.ForeColor = SystemColors.MenuText;


                // Wire up the DragEnter, DragDrop, and MouseMove events for the DraggableButton.
                buttonCharacter.DragEnter += ButtonCharacter_DragEnter;
                buttonCharacter.DragDrop += ButtonCharacter_DragDrop;
                buttonCharacter.MouseMove += ButtonCharacter_MouseMove;

                // Add the DraggableButton to the list and the FlowLayoutPanel.
                buttonCharacters.Add(buttonCharacter);
                flowLayoutPanelCharacters.Controls.Add(buttonCharacter);

                // Check if the image is not already loaded in the dictionary.
                if (!characterImages.ContainsKey(characterCode))
                {
                    // Try loading the image with "_000" suffix first.
                    string imagePathWithSuffix = Path.Combine(imageFolderPath, characterCode + "_000.dds");

                    try
                    {
                        // Use FreeImage to load the DDS file.
                        FREE_IMAGE_FORMAT imageFormat = FREE_IMAGE_FORMAT.FIF_DDS;
                        FIBITMAP dib = FreeImage.LoadEx(imagePathWithSuffix, ref imageFormat);
                        if (dib == null)
                        {
                            // If the image with suffix doesn't exist, try without the suffix.
                            string imagePathWithoutSuffix = Path.Combine(imageFolderPath, characterCode + ".dds");
                            dib = FreeImage.LoadEx(imagePathWithoutSuffix, ref imageFormat);
                        }

                        if (dib != null)
                        {
                            // Convert the FIBITMAP to a .NET Bitmap.
                            Bitmap characterBitmap = FreeImage.GetBitmap(dib);

                            // Free the FIBITMAP to avoid memory leaks.
                            FreeImage.UnloadEx(ref dib);

                            // If the image could not be loaded, use the default image.
                            if (!characterImages.ContainsKey(characterCode))
                            {
                                if (defaultImage != null)
                                {
                                    characterImages.Add(characterCode, defaultImage);
                                }
                                else
                                {
                                    // Handle the case where the default image is not available.
                                    // You may want to exit the application or handle the error differently if the default image is critical.
                                }
                            }

                            // Add the character image to the dictionary.
                            characterImages[characterCode] = characterBitmap;
                        }
                        else
                        {
                            // If both the images (with and without suffix) are not found, use the default image.
                            LoadDefaultImage();
                            if (defaultImage != null)
                            {
                                characterImages.Add(characterCode, new Bitmap(defaultImage));
                            }
                        }
                    }
                    catch (Exception ex)
                    {
                        // Handle the exception or display an error message.
                    }
                }
            }
        }

        // Function to parse the character data string into a multidimensional array.
        private string[][][] ParseCharacterData(string characterDataString)
        {
            // Check if the characterDataString is empty or null.
            if (string.IsNullOrEmpty(characterDataString))
            {
                // Return an empty array or throw an exception as appropriate.
                return new string[0][][];
            }

            // Split the characterDataString to get individual character arrays.
            string[] characterArrays = characterDataString.Split(new string[] { "],[[" }, StringSplitOptions.RemoveEmptyEntries);

            // Initialize the charaList array to hold the character data.
            string[][][] charaList = new string[characterArrays.Length][][];

            for (int i = 0; i < characterArrays.Length; i++)
            {
                // Split each character array to get individual character data.
                string[] characterData = characterArrays[i].Replace("[[", "").Replace("]]", "").Split(new string[] { "],[" }, StringSplitOptions.RemoveEmptyEntries);

                // Initialize the character data array for the current character.
                charaList[i] = new string[characterData.Length][];

                for (int j = 0; j < characterData.Length; j++)
                {
                    // Split each character's data into individual values.
                    string[] characterValues = characterData[j].Split(',');

                    // Initialize the array to store the character's values.
                    charaList[i][j] = new string[characterValues.Length];

                    for (int k = 0; k < characterValues.Length; k++)
                    {
                        // Remove unwanted characters and store the value.
                        charaList[i][j][k] = characterValues[k].Replace("[", "").Replace("]", "").Replace("\"", "").Trim();
                    }
                }
            }

            return charaList;
        }

        private void flowLayoutPanelCharacters_ControlAdded(object sender, ControlEventArgs e)
        {
            // Check if the control added is a DraggableButton.
            if (e.Control is DraggableButton buttonCharacter)
            {
                // Find the index of the button in the FlowLayoutPanel.
                int index = flowLayoutPanelCharacters.Controls.IndexOf(buttonCharacter);

                // Calculate the column index (0 to 2) of the button in the current row.
                int columnIndex = index % 3;

                // Set the flow break for every third control in each row except for the last row.
                if (columnIndex == 2 && index < flowLayoutPanelCharacters.Controls.Count - 1)
                {
                    flowLayoutPanelCharacters.SetFlowBreak(buttonCharacter, true);
                }
                else
                {
                    flowLayoutPanelCharacters.SetFlowBreak(buttonCharacter, false);
                }
            }
        }

        // Function to add images to the FlowLayoutPanel.
        void AddCharacterImagesToFlowLayoutPanel()
        {
            // Clear the FlowLayoutPanel before adding images to avoid duplicates when reloading.
            flowLayoutPanelCharacters.Controls.Clear();

            string imageFolderPath = Settings.Default.datafolder + @"\ui\texture\CHARA01";
            string defaultImagePath = Path.Combine(imageFolderPath, "FOF_000.dds");

            // Load the default image outside the loop
            LoadDefaultImage();

            foreach (var characterArray in charaList)
            {
                foreach (var characterData in characterArray)
                {
                    // The character code is the first element in the characterData array.
                    string characterCode = characterData[0].ToString();

                    // Check if the character code exists in the dictionary.
                    if (characterImages.ContainsKey(characterCode))
                    {
                        // Use the existing character image from the dictionary.
                        DraggableButton buttonCharacter = new DraggableButton();
                        buttonCharacter.BackgroundImageLayout = ImageLayout.Stretch; // Set BackgroundImageLayout to Stretch.
                        buttonCharacter.Width = 128;
                        buttonCharacter.Height = 64;
                        buttonCharacter.Image = characterImages[characterCode];

                        // Add the DraggableButton to the FlowLayoutPanel.
                        flowLayoutPanelCharacters.Controls.Add(buttonCharacter);

                        // Set the Tag property of the DraggableButton to store the character code.
                        buttonCharacter.Tag = characterCode;

                        // Wire up the DragEnter, DragDrop, and MouseMove events for the DraggableButton.
                        buttonCharacter.DragEnter += ButtonCharacter_DragEnter;
                        buttonCharacter.DragDrop += ButtonCharacter_DragDrop;
                        buttonCharacter.MouseMove += ButtonCharacter_MouseMove;
                    }
                    else
                    {
                        // Handle the case where the character image is not found.
                        // Use the default image if available.
                        if (defaultImage != null)
                        {
                            DraggableButton buttonCharacter = new DraggableButton();
                            buttonCharacter.BackgroundImageLayout = ImageLayout.Stretch; // Set BackgroundImageLayout to Stretch.
                            buttonCharacter.Width = 128;
                            buttonCharacter.Height = 64;
                            buttonCharacter.Image = new Bitmap(defaultImage);

                            // Add the DraggableButton to the FlowLayoutPanel.
                            flowLayoutPanelCharacters.Controls.Add(buttonCharacter);

                            // Set the Tag property of the DraggableButton to store the character code.
                            buttonCharacter.Tag = characterCode;
                            buttonCharacter.Text = characterCode;
                            buttonCharacter.Font = new Font("Arial", 15);
                            buttonCharacter.ForeColor = SystemColors.MenuText;

                            // Wire up the DragEnter, DragDrop, and MouseMove events for the DraggableButton.
                            buttonCharacter.DragEnter += ButtonCharacter_DragEnter;
                            buttonCharacter.DragDrop += ButtonCharacter_DragDrop;
                            buttonCharacter.MouseMove += ButtonCharacter_MouseMove;
                        }
                    }
                }
            }
        }

        // Function to handle the reordering of character slots in the FlowLayoutPanel.
        void ReorderCharacterSlots()
        {
            // Create a list to store the ordered character data.
            List<string[][]> orderedCharacterData = new List<string[][]>();

            // Create a list to keep track of the processed character codes.
            List<string> processedCharacterCodes = new List<string>();

            // Iterate through the DraggableButton controls in the FlowLayoutPanel.
            foreach (DraggableButton buttonCharacter in buttonCharacters)
            {
                // Get the character code associated with the DraggableButton.
                string characterCode = buttonCharacter.Tag.ToString();

                // Check if the character code has already been processed.
                if (!processedCharacterCodes.Contains(characterCode))
                {
                    // Find the character array in charaList with the matching character code.
                    var characterDataArray = charaList.FirstOrDefault(c => c[0][0] == characterCode);

                    if (characterDataArray != null)
                    {
                        // Add the character data array to the ordered list.
                        orderedCharacterData.Add(characterDataArray);

                        // Mark the character code as processed.
                        processedCharacterCodes.Add(characterCode);
                    }
                    else
                    {
                        // Handle the case where the character data is not found.
                        // You can display an error message or log the issue for debugging.
                    }
                }
            }

            // Convert the ordered character data list back to an array.
            charaList = orderedCharacterData.ToArray();
        }
        // Function to save the updated order in the Charalist.as file.
        void SaveCharacterOrderToFile()
        {
            string charaListFilePath = Properties.Settings.Default.datafolder + @"/scripts/action_script/Charalist.as";

            // Assuming the character data is stored in CharaListDlc0_0 variable in the AS3 file.
            string charaListVariable = "public static var CharaListBase";
            string startToken = charaListVariable + ":Array = [";
            string endToken = "]]"; // Fixed the endToken to match the original array structure.
            string characterDataString = "";

            // Read the existing content of Charalist.as.
            string[] charaListLines = File.ReadAllLines(charaListFilePath);

            // Find the line that contains the character data and update it.
            for (int i = 0; i < charaListLines.Length; i++)
            {
                if (charaListLines[i].Contains(startToken))
                {
                    // Remove the old character data from the line.
                    int startIndex = charaListLines[i].IndexOf(startToken) + startToken.Length;
                    int endIndex = charaListLines[i].LastIndexOf(endToken) + endToken.Length;
                    charaListLines[i] = charaListLines[i].Remove(startIndex, endIndex - startIndex);

                    // Generate the new character data string based on the updated charaList.
                    characterDataString = GenerateCharacterDataString(charaList);

                    // Update the line with the new character data.
                    charaListLines[i] = charaListVariable + ":Array = [" + characterDataString;
                    break;
                }
            }

            // Write the updated content back to Charalist.as.
            File.WriteAllLines(charaListFilePath, charaListLines);

            // Display a message indicating successful save.
            MessageBox.Show("Character order saved successfully!", "Success", MessageBoxButtons.OK, MessageBoxIcon.Information);
        }

        // Function to generate the character data string based on the updated charaList.
        private string GenerateCharacterDataString(string[][][] updatedCharaList)
        {
            // Create a new character data string based on the updated charaList.
            string newCharacterDataString = "";

            for (int i = 0; i < updatedCharaList.Length; i++)
            {
                string[][] characterArray = updatedCharaList[i];
                newCharacterDataString += "[";
                for (int j = 0; j < characterArray.Length; j++)
                {
                    var characterData = characterArray[j];

                    // The character code is the first element in the characterData array.
                    string characterCode = characterData[0];

                    // Convert the costume data and voice IDs to strings.
                    List<string> costumeDataStrings = new List<string>();
                    for (int k = 1; k < characterData.Length; k += 7)
                    {
                        string costumeData = "[\"" + characterCode + "\"," + characterData[k] + "," + characterData[k + 1] + "," +
                                             characterData[k + 2] + "," + characterData[k + 3] + ",[" + characterData[k + 4] + "," +
                                             characterData[k + 5] + "]]";
                        costumeDataStrings.Add(costumeData);
                    }

                    // Join the costume data strings and append to the new character data string.
                    newCharacterDataString += string.Join(",", costumeDataStrings);

                    if (j != characterArray.Length - 1)
                    {
                        newCharacterDataString += ",";
                    }
                }

                newCharacterDataString += "]";
                if (i != updatedCharaList.Length - 1)
                {
                    newCharacterDataString += ",";
                }
            }


            newCharacterDataString += "];";
            return newCharacterDataString;
        }
        private void RemoveSemicolonBeforeSquareBracket()
        {
            string CharaList = File.ReadAllText(Properties.Settings.Default.datafolder + @"/scripts/action_script/CharaList.as");

            while (CharaList.Contains(";]"))
            {
                CharaList = CharaList.Replace(";]", "]");
                Debug.WriteLine("Replaced semicolon with null");
            }

            // Now, CharaList contains the updated content with the semicolons removed.
            // You can write the updated content back to the file if needed.
            File.WriteAllText(Properties.Settings.Default.datafolder + @"/scripts/action_script/CharaList.as", CharaList);
        }

        private void saveCSSToolStripMenuItem_Click(object sender, EventArgs e)
        {
            // Reorder the character slots in the FlowLayoutPanel based on the user's arrangement.
            ReorderCharacterSlots();

            // Save the updated character order in the Charalist.as file.
            SaveCharacterOrderToFile();

            // Remove the semicolon before the square bracket
            RemoveSemicolonBeforeSquareBracket();

            // Compile Scripts
            CompileScripts();
        }
    }
}
