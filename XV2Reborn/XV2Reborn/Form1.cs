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
using System.Runtime.InteropServices.ComTypes;
using System.Security.AccessControl;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Xml.Linq;
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

        string CSOFileName;
        List<CSO_Data> CSOData = new List<CSO_Data>();
        CSO_Data CSOcurrent;
        bool CSOlck = true;

        string CMSFileName;
        List<Char_Model_Spec> cms = new List<Char_Model_Spec>();
        Char_Model_Spec CMScurrent;
        Char_Model_Spec CMSCopyCMS;
        bool CMScanPaste = false;
        bool CMSlck = true;
        bool[] CMSselective;
        charSkillSet charskillset = new charSkillSet();

        PSC PSCfile = new PSC();
        PSC_Costume PSCCopy;
        string PSCFileName;
        string[] PSCkeys;
        bool PSClck = true;

        List<DraggableButton> buttonCharacters = new List<DraggableButton>();
        // Assuming you have a class-level variable to store the character codes and their corresponding images.
        Dictionary<string, Image> characterImages = new Dictionary<string, Image>();
        string[][][] charaList; // Class-level variable to store the parsed character data.
        Image defaultImage; // Class-level variable to store the default image.

        string datapath;
        string flexsdkpath;
        string language;

        public msg MSGfile;
        string MSGFileName;


        string CUSFileName;
        CUSRegistry CUSfile = new CUSRegistry();

        bool CUSlck = true;
        charSkillSet CUScopySet;
        charSkillSet CUScurrentSet;
        skill CUScopySkill;
        skill CUScurrentSkill;

        public Form1()
        {
            InitializeComponent();
            LoadDefaultImage();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            if(Properties.Settings.Default.language.Length == 0) 
            {
                comboBox1.SelectedIndex = 5;
                Settings.Default.language = comboBox1.SelectedItem.ToString();
                Settings.Default.Save();
            }
            else
            {
                comboBox1.SelectedItem = Settings.Default.language;
            }

            language = Settings.Default.language;

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

            // Load listview Items
            loadLvItems();


            // Load the default MSG file
            MSGFileName = Properties.Settings.Default.datafolder + @"\msg\proper_noun_character_name_" + language + ".msg";
            MSGfile = msgStream.Load(MSGFileName);
            cbListMSG.Items.Clear();
            for (int i = 0; i < MSGfile.data.Length; i++)
                cbListMSG.Items.Add(MSGfile.data[i].ID.ToString() + " - " + MSGfile.data[i].NameID);

            // Load the CMS File
            CMSFileName = datapath + @"/system/char_model_spec.cms";
            cms.Clear();
            cms.AddRange(CMS.Read(CMSFileName));
            CMSselective = new bool[cms.Count];
            CMSCombobox.Items.Clear();
            for (int i = 0; i < cms.Count; i++)
            {
                CMSCombobox.Items.Add(cms[i].id.ToString("000") + " - " + cms[i].shortname);
                CMSselective[i] = false;
            }

            // Load the CSO File
            CSOFileName = datapath + @"/system/chara_sound.cso";
            CSOData.AddRange(CSO.Read(CSOFileName));
            cbListCSO.Items.Clear();
            for (int i = 0; i < CSOData.Count; i++)
                cbListCSO.Items.Add("Character " + CSOData[i].Char_ID.ToString("000") + " - Costume " + CSOData[i].Costume_ID.ToString("00"));

            // Load the CUS File
            CUSFileName = datapath + @"/system/custom_skill.cus";
            CUSfile.readCUS(CUSFileName);
            CUSfile.BuildRegistry(datapath + "\\msg", language);
            CUScbChar.Items.Clear();
            for (int i = 0; i < CUSfile.css.Count; i++)
                CUScbChar.Items.Add("Character " + CUSfile.css[i].charID.ToString("000") + " - Costume " + CUSfile.css[i].costumeID.ToString("00"));

            this.CUScbSuper1.Items.AddRange((object[])this.CUSfile.getSkillList(0));
            this.CUScbSuper2.Items.AddRange((object[])this.CUSfile.getSkillList(0));
            this.CUScbSuper3.Items.AddRange((object[])this.CUSfile.getSkillList(0));
            this.CUScbSuper4.Items.AddRange((object[])this.CUSfile.getSkillList(0));
            this.CUScbUltimate1.Items.AddRange((object[])this.CUSfile.getSkillList(1));
            this.CUScbUltimate2.Items.AddRange((object[])this.CUSfile.getSkillList(1));
            this.CUScbEvasive.Items.AddRange((object[])this.CUSfile.getSkillList(2));
            this.CUScbBlast.Items.AddRange((object[])this.CUSfile.getSkillList(4));
            this.CUScbAwaken.Items.AddRange((object[])this.CUSfile.getSkillList(5));

            // Load the PSC File
            PSCFileName = datapath + @"/system/parameter_spec_char.psc";
            PSCfile.Read(PSCFileName);
            PSCfile.SetSchema("PSC_Schema.csv");

            PSCcbChar.Items.Clear();
            for (int i = 0; i < PSCfile.list.Length; i++)
                PSCcbChar.Items.Add(PSCfile.list[i].id);

            PSClstData.Items.Clear();
            PSCkeys = PSCfile.schema.getKeys();
            foreach (string s in PSCkeys)
            {
                var Item = new ListViewItem(new[] { s, "0" });
                PSClstData.Items.Add(Item);
            }

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


                        msgData[] expand = new msgData[MSGfile.data.Length + 1];
                        Array.Copy(MSGfile.data, expand, MSGfile.data.Length);
                        string nameid = MSGfile.data[MSGfile.data.Length - 1].NameID;
                        int endid = int.Parse(nameid.Substring(nameid.Length - 3, 3));
                        expand[expand.Length - 1].ID = MSGfile.data.Length;
                        expand[expand.Length - 1].Lines = new string[] { modname };
                        expand[expand.Length - 1].NameID = "chara_" + id + "_" + (endid).ToString("000");

                        MSGfile.data = expand;

                        cbListMSG.Items.Clear();
                        for (int i = 0; i < MSGfile.data.Length; i++)
                            cbListMSG.Items.Add(MSGfile.data[i].ID.ToString() + "-" + MSGfile.data[i].NameID);

                        msgStream.Save(MSGfile, MSGFileName);
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

        private void msgSaveToolStripMenuItem_Click(object sender, EventArgs e)
        {
            msgStream.Save(MSGfile, MSGFileName);
            MessageBox.Show("MSG MSGfile Saved Successfully", "Success", MessageBoxButtons.OK, MessageBoxIcon.Information);

            Application.Restart();
        }

        private void msgAddToolStripMenuItem_Click(object sender, EventArgs e)
        {
            msgData[] expand = new msgData[MSGfile.data.Length + 1];
            Array.Copy(MSGfile.data, expand, MSGfile.data.Length);
            string nameid = MSGfile.data[MSGfile.data.Length - 1].NameID;
            int endid = int.Parse(nameid.Substring(nameid.Length - 3, 3));
            expand[expand.Length - 1].ID = MSGfile.data.Length;
            expand[expand.Length - 1].Lines = new string[] { "New Entry" };
            expand[expand.Length - 1].NameID = nameid.Substring(0, nameid.Length - 3) + (endid + 1).ToString("000");

            MSGfile.data = expand;

            cbListMSG.Items.Clear();
            for (int i = 0; i < MSGfile.data.Length; i++)
                cbListMSG.Items.Add(MSGfile.data[i].ID.ToString() + "-" + MSGfile.data[i].NameID);

        }

        private void msgRemoveToolStripMenuItem_Click(object sender, EventArgs e)
        {
            msgData[] reduce = new msgData[MSGfile.data.Length - 1];
            Array.Copy(MSGfile.data, reduce, MSGfile.data.Length - 1);
            MSGfile.data = reduce;

            cbListMSG.Items.Clear();
            for (int i = 0; i < MSGfile.data.Length; i++)
                cbListMSG.Items.Add(MSGfile.data[i].ID.ToString() + "-" + MSGfile.data[i].NameID);

        }

        private void charactersToolStripMenuItem_Click(object sender, EventArgs e)
        {

            MSGFileName = Properties.Settings.Default.datafolder + @"\msg\proper_noun_character_name_" + language + ".msg";
            MSGfile = msgStream.Load(MSGFileName);

            cbListMSG.Items.Clear();
            for (int i = 0; i < MSGfile.data.Length; i++)
                cbListMSG.Items.Add(MSGfile.data[i].ID.ToString() + " - " + MSGfile.data[i].NameID);

        }

        private void superInfoToolStripMenuItem_Click(object sender, EventArgs e)
        {
            MSGFileName = Properties.Settings.Default.datafolder + @"\msg\proper_noun_skill_spa_info_" + language + ".msg";
            MSGfile = msgStream.Load(MSGFileName);

            cbListMSG.Items.Clear();
            for (int i = 0; i < MSGfile.data.Length; i++)
                cbListMSG.Items.Add(MSGfile.data[i].ID.ToString() + " - " + MSGfile.data[i].NameID);
        }

        private void ultimatesInfoToolStripMenuItem_Click(object sender, EventArgs e)
        {
            MSGFileName = Properties.Settings.Default.datafolder + @"\msg\proper_noun_skill_ult_info_" + language + ".msg";
            MSGfile = msgStream.Load(MSGFileName);

            cbListMSG.Items.Clear();
            for (int i = 0; i < MSGfile.data.Length; i++)
                cbListMSG.Items.Add(MSGfile.data[i].ID.ToString() + " - " + MSGfile.data[i].NameID);
        }

        private void evasivesInfoToolStripMenuItem_Click(object sender, EventArgs e)
        {
            MSGFileName = Properties.Settings.Default.datafolder + @"\msg\proper_noun_skill_esc_info_" + language + ".msg";
            MSGfile = msgStream.Load(MSGFileName);

            cbListMSG.Items.Clear();
            for (int i = 0; i < MSGfile.data.Length; i++)
                cbListMSG.Items.Add(MSGfile.data[i].ID.ToString() + " - " + MSGfile.data[i].NameID);
        }

        private void supersToolStripMenuItem_Click(object sender, EventArgs e)
        {
            MSGFileName = Properties.Settings.Default.datafolder + @"\msg\proper_noun_skill_spa_name_" + language + ".msg";
            MSGfile = msgStream.Load(MSGFileName);

            cbListMSG.Items.Clear();
            for (int i = 0; i < MSGfile.data.Length; i++)
                cbListMSG.Items.Add(MSGfile.data[i].ID.ToString() + " - " + MSGfile.data[i].NameID);
        }

        private void ultimatesToolStripMenuItem_Click(object sender, EventArgs e)
        {
            MSGFileName = Properties.Settings.Default.datafolder + @"\msg\proper_noun_skill_ult_name_" + language + ".msg";
            MSGfile = msgStream.Load(MSGFileName);

            cbListMSG.Items.Clear();
            for (int i = 0; i < MSGfile.data.Length; i++)
                cbListMSG.Items.Add(MSGfile.data[i].ID.ToString() + " - " + MSGfile.data[i].NameID);
        }

        private void evasivesToolStripMenuItem_Click(object sender, EventArgs e)
        {
            MSGFileName = Properties.Settings.Default.datafolder + @"\msg\proper_noun_skill_esc_name_" + language + ".msg";
            MSGfile = msgStream.Load(MSGFileName);

            cbListMSG.Items.Clear();
            for (int i = 0; i < MSGfile.data.Length; i++)
                cbListMSG.Items.Add(MSGfile.data[i].ID.ToString() + " - " + MSGfile.data[i].NameID);
        }

        private void awokenToolStripMenuItem_Click(object sender, EventArgs e)
        {
            MSGFileName = Properties.Settings.Default.datafolder + @"\msg\proper_noun_skill_met_name_" + language + ".msg";
            MSGfile = msgStream.Load(MSGFileName);

            cbListMSG.Items.Clear();
            for (int i = 0; i < MSGfile.data.Length; i++)
                cbListMSG.Items.Add(MSGfile.data[i].ID.ToString() + " - " + MSGfile.data[i].NameID);
        }

        private void awokenInfoToolStripMenuItem_Click(object sender, EventArgs e)
        {
            MSGFileName = Properties.Settings.Default.datafolder + @"\msg\proper_noun_skill_met_info_" + language + ".msg";
            MSGfile = msgStream.Load(MSGFileName);

            cbListMSG.Items.Clear();
            for (int i = 0; i < MSGfile.data.Length; i++)
                cbListMSG.Items.Add(MSGfile.data[i].ID.ToString() + " - " + MSGfile.data[i].NameID);
        }

        private void cbList_SelectedIndexChanged(object sender, EventArgs e)
        {
            txtNameMSG.Text = MSGfile.data[cbListMSG.SelectedIndex].NameID;
            txtIDMSG.Text = MSGfile.data[cbListMSG.SelectedIndex].ID.ToString();
            cbLineMSG.Items.Clear();
            for (int i = 0; i < MSGfile.data[cbListMSG.SelectedIndex].Lines.Length; i++)
                cbLineMSG.Items.Add(i);

            cbLineMSG.SelectedIndex = 0;
            txtTextMSG.Text = MSGfile.data[cbListMSG.SelectedIndex].Lines[cbLineMSG.SelectedIndex];

        }

        private void cbLine_SelectedIndexChanged(object sender, EventArgs e)
        {
            txtTextMSG.Text = MSGfile.data[cbListMSG.SelectedIndex].Lines[cbLineMSG.SelectedIndex];

        }

        private void txtName_TextChanged(object sender, EventArgs e)
        {
            MSGfile.data[cbListMSG.SelectedIndex].NameID = txtNameMSG.Text;
            cbListMSG.Items[cbListMSG.SelectedIndex] = MSGfile.data[cbListMSG.SelectedIndex].ID.ToString() + "-" + MSGfile.data[cbListMSG.SelectedIndex].NameID;

        }

        private void txtText_TextChanged(object sender, EventArgs e)
        {
            MSGfile.data[cbListMSG.SelectedIndex].Lines[cbLineMSG.SelectedIndex] = txtTextMSG.Text;
        }

        // CMS

        private void comboBox1_SelectedIndexChanged(object sender, EventArgs e)
        {
            Settings.Default.language = comboBox1.SelectedItem.ToString();
            Settings.Default.Save();
        }

        private void CMSCombobox_SelectedIndexChanged(object sender, EventArgs e)
        {
            CMSlck = false;
            CMScurrent = cms[CMSCombobox.SelectedIndex];
            txtCharCMS.Text = CMScurrent.id.ToString();
            txtSNCMS.Text = CMScurrent.shortname;
            txt1CMS.Text = CMScurrent.unk1.ToString();
            txt2CMS.Text = CMScurrent.unk2.ToString();
            txt3CMS.Text = CMScurrent.unk3.ToString();
            txt4CMS.Text = CMScurrent.unk4.ToString();
            txt5CMS.Text = CMScurrent.unk5.ToString();
            txt6CMS.Text = CMScurrent.Paths[0];
            txt7CMS.Text = CMScurrent.Paths[1];
            txt8CMS.Text = CMScurrent.Paths[2];
            textBox1CMS.Text = CMScurrent.Paths[3];
            txt9CMS.Text = CMScurrent.Paths[4];
            txt10CMS.Text = CMScurrent.Paths[5];
            txt11CMS.Text = CMScurrent.Paths[6];
            txt12CMS.Text = CMScurrent.Paths[7];
            txt13CMS.Text = CMScurrent.Paths[8];
            checkBox1.Checked = CMSselective[CMSCombobox.SelectedIndex];
            CMSlck = true;

        }
        private void txtChar_TextChanged(object sender, EventArgs e)
        {
            int p;
            if (CMSlck && int.TryParse(txtCharCMS.Text, out p))
            {
                CMSlck = false;
                CMScurrent.id = p;
                cms[CMSCombobox.SelectedIndex] = CMScurrent;

                int temp = CMSCombobox.SelectedIndex;
                CMSCombobox.SelectedIndex = 0;
                CMSCombobox.Items.Clear();
                for (int i = 0; i < cms.Count; i++)
                    CMSCombobox.Items.Add(cms[i].id.ToString("000") + " - " + cms[i].shortname);
                CMSCombobox.SelectedIndex = temp;
                CMSlck = true;
            }
        }

        private void txtSN_TextChanged(object sender, EventArgs e)
        {
            if (CMSlck)
            {
                CMSlck = false;
                CMScurrent.shortname = txtSNCMS.Text;
                cms[CMSCombobox.SelectedIndex] = CMScurrent;

                int temp = CMSCombobox.SelectedIndex;
                CMSCombobox.SelectedIndex = 0;
                CMSCombobox.Items.Clear();
                for (int i = 0; i < cms.Count; i++)
                    CMSCombobox.Items.Add(cms[i].id.ToString("000") + " - " + cms[i].shortname);
                CMSCombobox.SelectedIndex = temp;
                CMSlck = true;
            }
        }

        private void txt1_TextChanged(object sender, EventArgs e)
        {
            int p;
            if (int.TryParse(txt1CMS.Text, out p))
            {
                CMScurrent.unk1 = p;
                cms[CMSCombobox.SelectedIndex] = CMScurrent;
            }
        }

        private void txt2_TextChanged(object sender, EventArgs e)
        {
            short p;
            if (short.TryParse(txt2CMS.Text, out p))
            {
                CMScurrent.unk2 = p;
                cms[CMSCombobox.SelectedIndex] = CMScurrent;
            }
        }

        private void txt3_TextChanged(object sender, EventArgs e)
        {
            short p;
            if (short.TryParse(txt3CMS.Text, out p))
            {
                CMScurrent.unk3 = p;
                cms[CMSCombobox.SelectedIndex] = CMScurrent;
            }
        }

        private void txt4_TextChanged(object sender, EventArgs e)
        {
            short p;
            if (short.TryParse(txt4CMS.Text, out p))
            {
                CMScurrent.unk4 = p;
                cms[CMSCombobox.SelectedIndex] = CMScurrent;
            }
        }

        private void txt5_TextChanged(object sender, EventArgs e)
        {
            short p;
            if (short.TryParse(txt5CMS.Text, out p))
            {
                CMScurrent.unk5 = p;
                cms[CMSCombobox.SelectedIndex] = CMScurrent;
            }
        }

        private void txt6_TextChanged(object sender, EventArgs e)
        {
            CMScurrent.Paths[0] = txt6CMS.Text;
            cms[CMSCombobox.SelectedIndex] = CMScurrent;
        }

        private void txt7_TextChanged(object sender, EventArgs e)
        {
            CMScurrent.Paths[1] = txt7CMS.Text;
            cms[CMSCombobox.SelectedIndex] = CMScurrent;
        }

        private void txt8_TextChanged(object sender, EventArgs e)
        {
            CMScurrent.Paths[2] = txt8CMS.Text;
            cms[CMSCombobox.SelectedIndex] = CMScurrent;
        }

        private void textBox1_TextChanged(object sender, EventArgs e)
        {
            CMScurrent.Paths[3] = textBox1CMS.Text;
            cms[CMSCombobox.SelectedIndex] = CMScurrent;
        }

        private void txt9_TextChanged(object sender, EventArgs e)
        {
            CMScurrent.Paths[4] = txt9CMS.Text;
            cms[CMSCombobox.SelectedIndex] = CMScurrent;
        }

        private void txt10_TextChanged(object sender, EventArgs e)
        {
            CMScurrent.Paths[5] = txt10CMS.Text;
            cms[CMSCombobox.SelectedIndex] = CMScurrent;
        }

        private void txt11_TextChanged(object sender, EventArgs e)
        {
            CMScurrent.Paths[6] = txt11CMS.Text;
            cms[CMSCombobox.SelectedIndex] = CMScurrent;
        }

        private void txt12_TextChanged(object sender, EventArgs e)
        {
            CMScurrent.Paths[7] = txt12CMS.Text;
            cms[CMSCombobox.SelectedIndex] = CMScurrent;
        }

        private void txt13_TextChanged(object sender, EventArgs e)
        {
            CMScurrent.Paths[8] = txt13CMS.Text;
            cms[CMSCombobox.SelectedIndex] = CMScurrent;
        }

        private void CMSSave_Click(object sender, EventArgs e)
        {
            CMS.Write(CMSFileName, cms.ToArray());
        }

        private void CMSSelectiveSave_Click(object sender, EventArgs e)
        {
            List<Char_Model_Spec> SelectCMS = new List<Char_Model_Spec>();

            for (int i = 0; i < cms.Count; i++)
            {
                if (CMSselective[i])
                    SelectCMS.Add(cms[i]);

            }

            CMS.Write("Selective.cms", SelectCMS.ToArray());
        }

        private void CMSAppend_Click(object sender, EventArgs e)
        {
            OpenFileDialog browseFile = new OpenFileDialog();
            browseFile.Filter = "Xenoverse Char_Model_Spec (*.cms)|*.cms";
            browseFile.Title = "Browse for CMS File";
            if (browseFile.ShowDialog() == DialogResult.Cancel)
                return;
            CMSFileName = browseFile.FileName;

            cms.AddRange(CMS.Read(CMSFileName));
            CMSselective = new bool[cms.Count];


            CMSCombobox.Items.Clear();
            for (int i = 0; i < cms.Count; i++)
            {
                CMSCombobox.Items.Add(cms[i].id.ToString("000") + " - " + cms[i].shortname);
                CMSselective[i] = false;
            }
        }

        private void CMSAdd_Click(object sender, EventArgs e)
        {
            Char_Model_Spec c = new Char_Model_Spec();
            c.Paths = new string[9];
            cms.Add(c);
            CMSCombobox.Items.Clear();
            for (int i = 0; i < cms.Count; i++)
                CMSCombobox.Items.Add(cms[i].id.ToString("000") + " - " + cms[i].shortname);

            Array.Resize<bool>(ref CMSselective, CMSselective.Length + 1);

            CMSselective[CMSselective.Length - 1] = false;
        }

        private void CMSRemove_Click(object sender, EventArgs e)
        {
            cms.RemoveAt(CMSCombobox.SelectedIndex);
            CMSCombobox.Items.Clear();
            for (int i = 0; i < cms.Count; i++)
                CMSCombobox.Items.Add(cms[i].id.ToString("000") + " - " + cms[i].shortname);

            Array.Resize<bool>(ref CMSselective, CMSselective.Length - 1);
        }

        private void CMSCopy_Click(object sender, EventArgs e)
        {
            CMSCopyCMS = CMScurrent;
        }

        private void CMSPaste_Click(object sender, EventArgs e)
        {
            CMScurrent = CMSCopyCMS;
            cms[CMSCombobox.SelectedIndex] = CMScurrent;
            txtCharCMS.Text = CMScurrent.id.ToString();
            txtSNCMS.Text = CMScurrent.shortname;
            txt1CMS.Text = CMScurrent.unk1.ToString();
            txt2CMS.Text = CMScurrent.unk2.ToString();
            txt3CMS.Text = CMScurrent.unk3.ToString();
            txt4CMS.Text = CMScurrent.unk4.ToString();
            txt5CMS.Text = CMScurrent.unk5.ToString();
            txt6CMS.Text = CMScurrent.Paths[0];
            txt7CMS.Text = CMScurrent.Paths[1];
            txt8CMS.Text = CMScurrent.Paths[2];
            textBox1CMS.Text = CMScurrent.Paths[3];
            txt9CMS.Text = CMScurrent.Paths[4];
            txt10CMS.Text = CMScurrent.Paths[5];
            txt11CMS.Text = CMScurrent.Paths[6];
            txt12CMS.Text = CMScurrent.Paths[7];
            txt13CMS.Text = CMScurrent.Paths[8];
            CMScurrent = cms[CMSCombobox.SelectedIndex];

            int temp = CMSCombobox.SelectedIndex;
            CMSCombobox.SelectedIndex = 0;
            CMSCombobox.Items.Clear();
            for (int i = 0; i < cms.Count; i++)
                CMSCombobox.Items.Add(cms[i].id.ToString("000") + " - " + cms[i].shortname);
            CMSCombobox.SelectedIndex = temp;
        }

        private void CMSInject_Click(object sender, EventArgs e)
        {

        }

        private void checkBox1_CheckedChanged(object sender, EventArgs e)
        {
            CMSselective[CMSCombobox.SelectedIndex] = checkBox1.Checked;
        }
        //////////////////////////////////////////////////////////////////

        // CSO
        private void CSOsaveToolStripMenuItem_Click(object sender, EventArgs e)
        {
            CSO.Write(CSOData.ToArray(), CSOFileName);
        }

        private void CSOaddToolStripMenuItem_Click(object sender, EventArgs e)
        {
            CSO_Data c = new CSO_Data();
            c.Paths = new string[4];
            c.Paths[0] = "";
            c.Paths[1] = "";
            c.Paths[2] = "";
            c.Paths[3] = "";
            CSOData.Add(c);

            cbListCSO.SelectedIndex = 0;
            cbListCSO.Items.Clear();
            for (int i = 0; i < CSOData.Count; i++)
                cbListCSO.Items.Add("Character " + CSOData[i].Char_ID.ToString("000") + " - Costume " + CSOData[i].Costume_ID.ToString("00"));

        }

        private void CSOremoveToolStripMenuItem_Click(object sender, EventArgs e)
        {
            CSOData.RemoveAt(cbListCSO.SelectedIndex);
            cbListCSO.SelectedIndex = 0;
            cbListCSO.Items.Clear();
            for (int i = 0; i < CSOData.Count; i++)
                cbListCSO.Items.Add("Character " + CSOData[i].Char_ID.ToString("000") + " - Costume " + CSOData[i].Costume_ID.ToString("00"));


        }

        private void cbListCSO_SelectedIndexChanged(object sender, EventArgs e)
        {
            CSOlck = false;
            CSOcurrent = CSOData[cbListCSO.SelectedIndex];
            txtCharCSO.Text = CSOcurrent.Char_ID.ToString();
            txtCostumeCSO.Text = CSOcurrent.Costume_ID.ToString();
            txt1CSO.Text = CSOcurrent.Paths[0];
            txt2CSO.Text = CSOcurrent.Paths[1];
            txt3CSO.Text = CSOcurrent.Paths[2];
            txt4CSO.Text = CSOcurrent.Paths[3];
            CSOlck = true;
        }

        private void textBox6CSO_TextChanged(object sender, EventArgs e)
        {
            int p;
            if (CSOlck && int.TryParse(txtCharCSO.Text, out p))
            {
                CSOlck = false;
                CSOcurrent.Char_ID = p;
                CSOData[cbListCSO.SelectedIndex] = CSOcurrent;

                int temp = cbListCSO.SelectedIndex;
                cbListCSO.SelectedIndex = 0;
                cbListCSO.Items.Clear();
                for (int i = 0; i < CSOData.Count; i++)
                    cbListCSO.Items.Add("Character " + CSOData[i].Char_ID.ToString("000") + " - Costume " + CSOData[i].Costume_ID.ToString("00"));
                cbListCSO.SelectedIndex = temp;
                CSOlck = true;
            }
        }

        private void txtCostumeCSO_TextChanged(object sender, EventArgs e)
        {
            int p;
            if (CSOlck && int.TryParse(txtCostumeCSO.Text, out p))
            {
                CSOlck = false;
                CSOcurrent.Costume_ID = p;
                CSOData[cbListCSO.SelectedIndex] = CSOcurrent;
                int temp = cbListCSO.SelectedIndex;
                cbListCSO.SelectedIndex = 0;
                cbListCSO.Items.Clear();
                for (int i = 0; i < CSOData.Count; i++)
                    cbListCSO.Items.Add("Character " + CSOData[i].Char_ID.ToString("000") + " - Costume " + CSOData[i].Costume_ID.ToString("00"));
                cbListCSO.SelectedIndex = temp;
                CSOlck = true;
            }
        }

        private void textBox5CSO_TextChanged(object sender, EventArgs e)
        {
            CSOcurrent.Paths[0] = txt1CSO.Text;
            CSOData[cbListCSO.SelectedIndex] = CSOcurrent;
        }

        private void textBox4CSO_TextChanged(object sender, EventArgs e)
        {
            CSOcurrent.Paths[1] = txt2CSO.Text;
            CSOData[cbListCSO.SelectedIndex] = CSOcurrent;
        }

        private void textBox3CSO_TextChanged(object sender, EventArgs e)
        {
            CSOcurrent.Paths[2] = txt3CSO.Text;
            CSOData[cbListCSO.SelectedIndex] = CSOcurrent;
        }

        private void textBox2CSO_TextChanged(object sender, EventArgs e)
        {
            CSOcurrent.Paths[3] = txt4CSO.Text;
            CSOData[cbListCSO.SelectedIndex] = CSOcurrent;
        }

        //////////////////////////////////////////////////////////////////


        // CUS

        private void UpdateCharlist()
        {
            int temp = CUScbChar.SelectedIndex;
            CUScbChar.SelectedIndex = 0;
            CUScbChar.Items.Clear();
            //if (infoLoaded)
            //{

            //}
            //else
            //{
            for (int i = 0; i < CUSfile.css.Count; i++)
                CUScbChar.Items.Add("Character " + CUSfile.css[i].charID.ToString("000") + " - Costume " + CUSfile.css[i].costumeID.ToString("00"));
            //}
            CUScbChar.SelectedIndex = temp;
        }

        private void UpdateCurrentSkill()
        {
            switch (cbTypeCUS.SelectedIndex)
            {
                case 0:
                    CUSfile.Super[cbSkillCUS.SelectedIndex] = CUScurrentSkill;
                    break;
                case 1:
                    CUSfile.Ultimate[cbSkillCUS.SelectedIndex] = CUScurrentSkill;
                    break;
                case 2:
                    CUSfile.Evasive[cbSkillCUS.SelectedIndex] = CUScurrentSkill;
                    break;
                case 3:
                    CUSfile.blast[cbSkillCUS.SelectedIndex] = CUScurrentSkill;
                    break;
                case 4:
                    CUSfile.Awaken[cbSkillCUS.SelectedIndex] = CUScurrentSkill;
                    break;
            }
        }
        private void UpdateSkillList()
        {
            this.CUSlck = false;
            //int selectedIndex = this.cbSkill.SelectedIndex;
            this.cbSkillCUS.SelectedIndex = -1;
            switch (this.cbTypeCUS.SelectedIndex)
            {
                case 0:
                    this.cbSkillCUS.Items.Clear();
                    this.cbSkillCUS.Items.AddRange((object[])this.CUSfile.getSkillList(0));
                    break;
                case 1:
                    this.cbSkillCUS.Items.Clear();
                    this.cbSkillCUS.Items.AddRange((object[])this.CUSfile.getSkillList(1));
                    break;
                case 2:
                    this.cbSkillCUS.Items.Clear();
                    this.cbSkillCUS.Items.AddRange((object[])this.CUSfile.getSkillList(2));
                    break;
                case 3:
                    this.cbSkillCUS.Items.Clear();
                    this.cbSkillCUS.Items.AddRange((object[])this.CUSfile.getSkillList(4));
                    break;
                case 4:
                    this.cbSkillCUS.Items.Clear();
                    this.cbSkillCUS.Items.AddRange((object[])this.CUSfile.getSkillList(5));
                    break;
            }
            //this.cbSkill.SelectedIndex = selectedIndex;
            this.CUSlck = true;
        }

        private void CUSsaveToolStripMenuItem_Click(object sender, EventArgs e)
        {
            CUSfile.readCUS(CUSFileName);
        }

        private void CUSaddToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (this.tabControl1.SelectedIndex == 0)
            {
                this.CUSfile.css.Add(new charSkillSet()
                {
                    skill = new short[10],
                    charID = 999
                });
                this.CUSlck = false;
                this.UpdateCharlist();
                this.CUSlck = true;
            }
            else
            {
                skill skill = new skill();
                skill.Paths = new string[7];
                SkillReg skillReg = new SkillReg();
                skillReg.name = "";
                skillReg.shortName = "NEW";
                switch (this.cbTypeCUS.SelectedIndex)
                {
                    case 0:
                        this.CUSfile.Super.Add(skill);
                        this.CUSfile.superReg.Add(skillReg);
                        break;
                    case 1:
                        this.CUSfile.Ultimate.Add(skill);
                        this.CUSfile.ultimateReg.Add(skillReg);
                        break;
                    case 2:
                        this.CUSfile.Evasive.Add(skill);
                        this.CUSfile.evasiveReg.Add(skillReg);
                        break;
                    case 3:
                        this.CUSfile.blast.Add(skill);
                        this.CUSfile.blastReg.Add(skillReg);
                        break;
                    case 4:
                        this.CUSfile.Awaken.Add(skill);
                        this.CUSfile.awakenReg.Add(skillReg);
                        break;
                }
                this.cbSkillCUS.SelectedIndex = 0;
                this.UpdateSkillList();
            }

        }

        private void CUSremoveToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (this.tabControl1.SelectedIndex == 0)
            {
                this.CUSfile.css.RemoveAt(this.CUScbChar.SelectedIndex);
                this.CUSlck = false;
                this.UpdateCharlist();
                this.CUSlck = true;
            }
            else
            {
                switch (this.cbTypeCUS.SelectedIndex)
                {
                    case 0:
                        this.CUSfile.Super.RemoveAt(this.cbSkillCUS.SelectedIndex);
                        this.CUSfile.superReg.RemoveAt(this.cbSkillCUS.SelectedIndex);
                        break;
                    case 1:
                        this.CUSfile.Ultimate.RemoveAt(this.cbSkillCUS.SelectedIndex);
                        this.CUSfile.ultimateReg.RemoveAt(this.cbSkillCUS.SelectedIndex);
                        break;
                    case 2:
                        this.CUSfile.Evasive.RemoveAt(this.cbSkillCUS.SelectedIndex);
                        this.CUSfile.evasiveReg.RemoveAt(this.cbSkillCUS.SelectedIndex);
                        break;
                    case 3:
                        this.CUSfile.blast.RemoveAt(this.cbSkillCUS.SelectedIndex);
                        this.CUSfile.blastReg.RemoveAt(this.cbSkillCUS.SelectedIndex);
                        break;
                    case 4:
                        this.CUSfile.Awaken.RemoveAt(this.cbSkillCUS.SelectedIndex);
                        this.CUSfile.awakenReg.RemoveAt(this.cbSkillCUS.SelectedIndex);
                        break;
                }
                this.cbSkillCUS.SelectedIndex = 0;
                this.UpdateSkillList();
            }
        }

        private void CUScopyToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (this.tabControl1.SelectedIndex == 0)
                this.CUScopySet = this.CUScurrentSet;
            else
                this.CUScopySkill = this.CUScurrentSkill;
        }

        private void CUSpasteToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (this.tabControl1.SelectedIndex == 0)
            {
                this.CUScurrentSet.skill = this.CUScopySet.skill;
                this.CUSfile.css[this.CUScbChar.SelectedIndex] = this.CUScurrentSet;
                this.CUSlck = false;
                int selectedIndex = this.CUScbChar.SelectedIndex;
                this.CUScbChar.SelectedIndex = 0;
                this.CUScbChar.Items.Clear();
                for (int index = 0; index < this.CUSfile.css.Count; ++index)
                {
                    ComboBox.ObjectCollection items = this.CUScbChar.Items;
                    string str1 = "Character ";
                    charSkillSet charSkillSet = this.CUSfile.css[index];
                    string str2 = charSkillSet.charID.ToString("000");
                    string str3 = " - Costume ";
                    charSkillSet = this.CUSfile.css[index];
                    string str4 = charSkillSet.costumeID.ToString("00");
                    string str5 = str1 + str2 + str3 + str4;
                    items.Add((object)str5);
                }
                this.CUScbChar.SelectedIndex = selectedIndex;
                this.CUSlck = true;
            }
            else
            {
                this.CUScurrentSkill = this.CUScopySkill;
                this.UpdateCurrentSkill();
                this.UpdateSkillList();
                this.CUSlck = false;
                this.txtShortNameCUS.Text = this.CUScurrentSkill.shortName;
                this.txtidCUS.Text = this.CUScurrentSkill.id.ToString();
                this.txtid2CUS.Text = this.CUScurrentSkill.id2.ToString();
                this.textBox1CUS.Text = this.CUScurrentSkill.racelock.ToString();
                this.txt1CUS.Text = this.CUScurrentSkill.unk1.ToString();
                this.txt2CUS.Text = this.CUScurrentSkill.unk2.ToString();
                this.txtHairCUS.Text = this.CUScurrentSkill.hair.ToString();
                this.txt3CUS.Text = this.CUScurrentSkill.unk3.ToString();
                this.txt4CUS.Text = this.CUScurrentSkill.Paths[0];
                this.txt5CUS.Text = this.CUScurrentSkill.Paths[1];
                this.txt6CUS.Text = this.CUScurrentSkill.Paths[2];
                this.txt7CUS.Text = this.CUScurrentSkill.Paths[3];
                this.txt8CUS.Text = this.CUScurrentSkill.Paths[4];
                this.txt9CUS.Text = this.CUScurrentSkill.Paths[5];
                this.txt10CUS.Text = this.CUScurrentSkill.Paths[6];
                this.txt11CUS.Text = this.CUScurrentSkill.unk4.ToString();
                this.txt12CUS.Text = this.CUScurrentSkill.unk5.ToString();
                this.txt13CUS.Text = this.CUScurrentSkill.unk6.ToString();
                this.txt14CUS.Text = this.CUScurrentSkill.unk7.ToString();
                this.txt15CUS.Text = this.CUScurrentSkill.unk8.ToString();
                this.txt16CUS.Text = this.CUScurrentSkill.unk9.ToString();
                this.txt17CUS.Text = this.CUScurrentSkill.unk10.ToString();
                this.txt18CUS.Text = this.CUScurrentSkill.unk11.ToString();
                this.CUSlck = true;
            }
            //.Text = "Data has been pasted in";

        }

        private void CUScbChar_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (!this.CUSlck)
                return;
            this.CUSlck = false;
            this.CUScurrentSet = this.CUSfile.css[this.CUScbChar.SelectedIndex];
            this.CUStxtCharID.Text = this.CUScurrentSet.charID.ToString();
            this.CUStxtCostID.Text = this.CUScurrentSet.costumeID.ToString();
            this.CUStxtVal.Text = this.CUScurrentSet.costumeID.ToString();
            this.CUScbSuper1.SelectedIndex = this.CUSfile.FindSkillIndex(0, this.CUScurrentSet.skill[0]);
            this.CUScbSuper2.SelectedIndex = this.CUSfile.FindSkillIndex(0, this.CUScurrentSet.skill[1]);
            this.CUScbSuper3.SelectedIndex = this.CUSfile.FindSkillIndex(0, this.CUScurrentSet.skill[2]);
            this.CUScbSuper4.SelectedIndex = this.CUSfile.FindSkillIndex(0, this.CUScurrentSet.skill[3]);
            this.CUScbUltimate1.SelectedIndex = this.CUSfile.FindSkillIndex(1, this.CUScurrentSet.skill[4]);
            this.CUScbUltimate2.SelectedIndex = this.CUSfile.FindSkillIndex(1, this.CUScurrentSet.skill[5]);
            this.CUScbEvasive.SelectedIndex = this.CUSfile.FindSkillIndex(2, this.CUScurrentSet.skill[6]);
            this.CUScbBlast.SelectedIndex = this.CUSfile.FindSkillIndex(4, this.CUScurrentSet.skill[7]);
            this.CUScbAwaken.SelectedIndex = this.CUSfile.FindSkillIndex(5, this.CUScurrentSet.skill[8]);
            this.CUSlck = true;
        }

        private void CUStxtCharID_TextChanged(object sender, EventArgs e)
        {
            int p;
            if (CUSlck && int.TryParse(CUStxtCharID.Text, out p))
            {
                CUSlck = false;
                CUScurrentSet.charID = p;
                CUSfile.css[CUScbChar.SelectedIndex] = CUScurrentSet;

                UpdateCharlist();
                CUSlck = true;
            }
        }

        private void CUStxtCostID_TextChanged(object sender, EventArgs e)
        {
            int p;
            if (CUSlck && int.TryParse(CUStxtCostID.Text, out p))
            {
                CUSlck = false;
                CUScurrentSet.costumeID = p;
                CUSfile.css[CUScbChar.SelectedIndex] = CUScurrentSet;

                UpdateCharlist();
                CUSlck = true;
            }
        }

        private void CUStxtVal_TextChanged(object sender, EventArgs e)
        {
            CUSfile.css[CUScbChar.SelectedIndex].skill[9] = short.Parse(CUStxtVal.Text);
        }

        private void CUScbSuper1_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (!this.CUSlck)
                return;
            this.CUScurrentSet.skill[0] = this.CUSfile.superReg[this.CUScbSuper1.SelectedIndex].id;
            this.CUSfile.css[this.CUScbChar.SelectedIndex] = this.CUScurrentSet;
            //.Text = "";
        }

        private void CUScbSuper2_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (!this.CUSlck)
                return;
            this.CUScurrentSet.skill[1] = this.CUSfile.superReg[this.CUScbSuper2.SelectedIndex].id;
            this.CUSfile.css[this.CUScbChar.SelectedIndex] = this.CUScurrentSet;
            //.Text = "";
        }

        private void CUScbSuper3_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (!this.CUSlck)
                return;
            this.CUScurrentSet.skill[2] = this.CUSfile.superReg[this.CUScbSuper3.SelectedIndex].id;
            this.CUSfile.css[this.CUScbChar.SelectedIndex] = this.CUScurrentSet;
            //.Text = "";
        }

        private void CUScbSuper4_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (!this.CUSlck)
                return;
            this.CUScurrentSet.skill[3] = this.CUSfile.superReg[this.CUScbSuper4.SelectedIndex].id;
            this.CUSfile.css[this.CUScbChar.SelectedIndex] = this.CUScurrentSet;
            //.Text = "";
        }

        private void CUScbUltimate1_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (!this.CUSlck)
                return;
            this.CUScurrentSet.skill[4] = this.CUSfile.ultimateReg[this.CUScbUltimate1.SelectedIndex].id;
            this.CUSfile.css[this.CUScbChar.SelectedIndex] = this.CUScurrentSet;
            //.Text = "";
        }

        private void CUScbUltimate2_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (!this.CUSlck)
                return;
            this.CUScurrentSet.skill[5] = this.CUSfile.ultimateReg[this.CUScbUltimate2.SelectedIndex].id;
            this.CUSfile.css[this.CUScbChar.SelectedIndex] = this.CUScurrentSet;
            //.Text = "";
        }

        private void CUScbEvasive_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (!this.CUSlck)
                return;
            this.CUScurrentSet.skill[6] = this.CUSfile.evasiveReg[this.CUScbEvasive.SelectedIndex].id;
            this.CUSfile.css[this.CUScbChar.SelectedIndex] = this.CUScurrentSet;
            //.Text = "";
        }

        private void CUScbBlast_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (!this.CUSlck)
                return;
            this.CUScurrentSet.skill[7] = this.CUSfile.blastReg[this.CUScbBlast.SelectedIndex].id;
            this.CUSfile.css[this.CUScbChar.SelectedIndex] = this.CUScurrentSet;
            //.Text = "";
        }

        private void CUScbAwaken_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (!this.CUSlck)
                return;
            this.CUScurrentSet.skill[8] = this.CUSfile.awakenReg[this.CUScbAwaken.SelectedIndex].id;
            this.CUSfile.css[this.CUScbChar.SelectedIndex] = this.CUScurrentSet;
            //.Text = "";
        }

        private void cbTypeCUS_SelectedIndexChanged(object sender, EventArgs e)
        {
            this.UpdateSkillList();
        }

        private void cbSkillCUS_SelectedIndexChanged(object sender, EventArgs e)
        {
            switch (cbTypeCUS.SelectedIndex)
            {
                case 0:
                    CUScurrentSkill = CUSfile.Super[cbSkillCUS.SelectedIndex];
                    break;
                case 1:
                    CUScurrentSkill = CUSfile.Ultimate[cbSkillCUS.SelectedIndex];
                    break;
                case 2:
                    CUScurrentSkill = CUSfile.Evasive[cbSkillCUS.SelectedIndex];
                    break;
                case 3:
                    CUScurrentSkill = CUSfile.blast[cbSkillCUS.SelectedIndex];
                    break;
                case 4:
                    CUScurrentSkill = CUSfile.Awaken[cbSkillCUS.SelectedIndex];
                    break;
            }

            CUSlck = false;
            txtShortNameCUS.Text = CUScurrentSkill.shortName;
            txtidCUS.Text = CUScurrentSkill.id.ToString();
            txtid2CUS.Text = CUScurrentSkill.id2.ToString();
            textBox1CUS.Text = CUScurrentSkill.racelock.ToString();
            txt1CUS.Text = CUScurrentSkill.unk1.ToString();
            txt2CUS.Text = CUScurrentSkill.unk2.ToString();
            txtHairCUS.Text = CUScurrentSkill.hair.ToString();
            txt3CUS.Text = CUScurrentSkill.unk3.ToString();
            txt4CUS.Text = CUScurrentSkill.Paths[0];
            txt5CUS.Text = CUScurrentSkill.Paths[1];
            txt6CUS.Text = CUScurrentSkill.Paths[2];
            txt7CUS.Text = CUScurrentSkill.Paths[3];
            txt8CUS.Text = CUScurrentSkill.Paths[4];
            txt9CUS.Text = CUScurrentSkill.Paths[5];
            txt10CUS.Text = CUScurrentSkill.Paths[6];
            txt11CUS.Text = CUScurrentSkill.unk4.ToString();
            txt12CUS.Text = CUScurrentSkill.unk5.ToString();
            txt13CUS.Text = CUScurrentSkill.unk6.ToString();
            txt14CUS.Text = CUScurrentSkill.unk7.ToString();
            txt15CUS.Text = CUScurrentSkill.unk8.ToString();
            txt16CUS.Text = CUScurrentSkill.unk9.ToString();
            txt17CUS.Text = CUScurrentSkill.unk10.ToString();
            txt18CUS.Text = CUScurrentSkill.unk11.ToString();
            CUSlck = true;

        }

        private void txtShortNameCUS_TextChanged(object sender, EventArgs e)
        {
            if (!this.CUSlck)
                return;
            this.CUScurrentSkill.shortName = this.txtShortNameCUS.Text;
            this.UpdateCurrentSkill();
            this.UpdateSkillList();
            //.Text = "";
        }

        private void txtidCUS_TextChanged(object sender, EventArgs e)
        {
            short result;
            if (!this.CUSlck || !short.TryParse(this.txtidCUS.Text, out result))
                return;
            this.CUScurrentSkill.id = result;
            this.UpdateCurrentSkill();
            this.UpdateSkillList();
            //.Text = "";
        }

        private void txtid2CUS_TextChanged(object sender, EventArgs e)
        {
            short result;
            if (!this.CUSlck || !short.TryParse(this.txtid2CUS.Text, out result))
                return;
            this.CUScurrentSkill.id2 = result;
            this.UpdateCurrentSkill();
            //.Text = "";
        }

        private void textBox1CUS_TextChanged(object sender, EventArgs e)
        {
            //race lock
            byte result;
            if (!this.CUSlck || !byte.TryParse(this.textBox1CUS.Text, out result))
                return;
            this.CUScurrentSkill.racelock = result;
            this.UpdateCurrentSkill();
            //.Text = "";
        }

        private void txt3CUS_TextChanged(object sender, EventArgs e)
        {
            short result;
            if (!this.CUSlck || !short.TryParse(this.txt3CUS.Text, out result))
                return;
            this.CUScurrentSkill.unk3 = result;
            this.UpdateCurrentSkill();
            //.Text = "";
        }

        private void txtHairCUS_TextChanged(object sender, EventArgs e)
        {
            short result;
            if (!this.CUSlck || !short.TryParse(this.txtHairCUS.Text, out result))
                return;
            this.CUScurrentSkill.hair = result;
            this.UpdateCurrentSkill();
            //.Text = "";
        }

        private void txt2CUS_TextChanged(object sender, EventArgs e)
        {
            short result;
            if (!this.CUSlck || !short.TryParse(this.txt2CUS.Text, out result))
                return;
            this.CUScurrentSkill.unk2 = result;
            this.UpdateCurrentSkill();
            //.Text = "";
        }

        private void txt1CUS_TextChanged(object sender, EventArgs e)
        {
            byte result;
            if (!this.CUSlck || !byte.TryParse(this.txtidCUS.Text, out result))
                return;
            this.CUScurrentSkill.unk1 = result;
            this.UpdateCurrentSkill();
            //.Text = "";
        }

        private void txt4CUS_TextChanged(object sender, EventArgs e)
        {
            if (!this.CUSlck)
                return;
            this.CUScurrentSkill.Paths[0] = this.txt4CUS.Text;
            this.UpdateCurrentSkill();
            //.Text = "";
        }

        private void txt5CUS_TextChanged(object sender, EventArgs e)
        {
            if (!this.CUSlck)
                return;
            this.CUScurrentSkill.Paths[1] = this.txt5CUS.Text;
            this.UpdateCurrentSkill();
            //.Text = "";
        }

        private void txt6CUS_TextChanged(object sender, EventArgs e)
        {
            if (!this.CUSlck)
                return;
            this.CUScurrentSkill.Paths[2] = this.txt6CUS.Text;
            this.UpdateCurrentSkill();
            //.Text = "";
        }

        private void txt7CUS_TextChanged(object sender, EventArgs e)
        {
            if (!this.CUSlck)
                return;
            this.CUScurrentSkill.Paths[3] = this.txt7CUS.Text;
            this.UpdateCurrentSkill();
            //.Text = "";
        }

        private void txt8CUS_TextChanged(object sender, EventArgs e)
        {
            if (!this.CUSlck)
                return;
            this.CUScurrentSkill.Paths[4] = this.txt8CUS.Text;
            this.UpdateCurrentSkill();
            //.Text = "";
        }

        private void txt9CUS_TextChanged(object sender, EventArgs e)
        {
            if (!this.CUSlck)
                return;
            this.CUScurrentSkill.Paths[5] = this.txt9CUS.Text;
            this.UpdateCurrentSkill();
            //.Text = "";
        }

        private void txt10CUS_TextChanged(object sender, EventArgs e)
        {
            if (!this.CUSlck)
                return;
            this.CUScurrentSkill.Paths[6] = this.txt10CUS.Text;
            this.UpdateCurrentSkill();
            //.Text = "";

        }

        private void txt11CUS_TextChanged(object sender, EventArgs e)
        {
            short result;
            if (!this.CUSlck || !short.TryParse(this.txt11CUS.Text, out result))
                return;
            this.CUScurrentSkill.unk4 = result;
            this.UpdateCurrentSkill();
            //.Text = "";
        }

        private void txt12CUS_TextChanged(object sender, EventArgs e)
        {
            short result;
            if (!this.CUSlck || !short.TryParse(this.txt12CUS.Text, out result))
                return;
            this.CUScurrentSkill.unk5 = result;
            this.UpdateCurrentSkill();
            //.Text = "";

        }

        private void txt13CUS_TextChanged(object sender, EventArgs e)
        {
            short result;
            if (!this.CUSlck || !short.TryParse(this.txt13CUS.Text, out result))
                return;
            this.CUScurrentSkill.unk6 = result;
            this.UpdateCurrentSkill();
            //.Text = "";
        }

        private void txt14CUS_TextChanged(object sender, EventArgs e)
        {
            short result;
            if (!this.CUSlck || !short.TryParse(this.txt14CUS.Text, out result))
                return;
            this.CUScurrentSkill.unk7 = result;
            this.UpdateCurrentSkill();
            //.Text = "";
        }

        private void txt15CUS_TextChanged(object sender, EventArgs e)
        {
            short result;
            if (!this.CUSlck || !short.TryParse(this.txt15CUS.Text, out result))
                return;
            this.CUScurrentSkill.unk8 = result;
            this.UpdateCurrentSkill();
            //.Text = "";
        }

        private void txt16CUS_TextChanged(object sender, EventArgs e)
        {
            short result;
            if (!this.CUSlck || !short.TryParse(this.txt16CUS.Text, out result))
                return;
            this.CUScurrentSkill.unk9 = result;
            this.UpdateCurrentSkill();
            //.Text = "";
        }

        private void txt17CUS_TextChanged(object sender, EventArgs e)
        {
            short result;
            if (!this.CUSlck || !short.TryParse(this.txt17CUS.Text, out result))
                return;
            this.CUScurrentSkill.unk10 = result;
            this.UpdateCurrentSkill();
            //.Text = "";
        }

        private void txt18CUS_TextChanged(object sender, EventArgs e)
        {
            short result;
            if (!this.CUSlck || !short.TryParse(this.txt18CUS.Text, out result))
                return;
            this.CUScurrentSkill.unk11 = (int)result;
            this.UpdateCurrentSkill();
            //.Text = "";
        }

        //////////////////////////////////////////////////////////////////////


        // PSC
        private void PSCcbChar_SelectedIndexChanged(object sender, EventArgs e)
        {
            PSCcbCostume.Items.Clear();
            for (int i = 0; i < PSCfile.list[PSCcbChar.SelectedIndex].Costume_Data.Length; i++)
            {
                PSCcbCostume.Items.Add(PSCfile.schema.getValueString(PSCkeys[0], ref PSCfile.list[PSCcbChar.SelectedIndex].Costume_Data[i].Data));
            }
            PSCtxtCharID.Text = PSCfile.list[PSCcbChar.SelectedIndex].id.ToString();

        }

        private void PSCtxtCharID_TextChanged(object sender, EventArgs e)
        {
            int p;
            if (int.TryParse(PSCtxtCharID.Text, out p))
            {
                PSCfile.list[PSCcbChar.SelectedIndex].id = p;
                PSCcbChar.Items[PSCcbChar.SelectedIndex] = PSCtxtCharID.Text;
            }
        }

        private void PSCcbCostume_SelectedIndexChanged(object sender, EventArgs e)
        {
            PSClstData.Items.Clear();
            foreach (string s in PSCkeys)
            {
                var Item = new ListViewItem(new[] { s, PSCfile.schema.getValueString(s, ref PSCfile.list[PSCcbChar.SelectedIndex].Costume_Data[PSCcbCostume.SelectedIndex].Data) });
                PSClstData.Items.Add(Item);
            }

        }

        private void PSCtxtVal_TextChanged(object sender, EventArgs e)
        {
            if (PSClck)
            {
                PSClstData.SelectedItems[0].SubItems[1].Text = PSCtxtVal.Text;
                PSCfile.schema.setValueString(PSCtxtName.Text, ref PSCfile.list[PSCcbChar.SelectedIndex].Costume_Data[PSCcbCostume.SelectedIndex].Data, PSCtxtVal.Text);
            }
        }

        private void PSClstData_SelectedIndexChanged(object sender, EventArgs e)
        {
            PSClck = false;
            if (PSClstData.SelectedItems.Count != 0)
            {
                PSCtxtName.Text = PSClstData.SelectedItems[0].SubItems[0].Text;
                PSCtxtVal.Text = PSClstData.SelectedItems[0].SubItems[1].Text;
            }
            PSClck = true;
        }

        private void PSCsaveToolStripMenuItem_Click(object sender, EventArgs e)
        {
            PSCfile.Write(PSCFileName);
        }

        private void PSCcharacterToolStripMenuItem_Click(object sender, EventArgs e)
        {
            //add character
            PSCcbChar.SelectedIndex = 0;
            Array.Resize<PSC_Char>(ref PSCfile.list, PSCfile.list.Length + 1);
            PSCfile.list[PSCfile.list.Length - 1].Costume_Data = new PSC_Costume[1];
            PSCfile.list[PSCfile.list.Length - 1].Costume_Data[0].Data = new byte[196];
            PSCcbChar.Items.Clear();
            for (int i = 0; i < PSCfile.list.Length; i++)
                PSCcbChar.Items.Add(PSCfile.list[i].id);
        }

        private void PSCcostumeToolStripMenuItem_Click(object sender, EventArgs e)
        {
            //add costume
            PSCcbCostume.SelectedIndex = 0;
            Array.Resize<PSC_Costume>(ref PSCfile.list[PSCcbChar.SelectedIndex].Costume_Data, PSCfile.list[PSCcbChar.SelectedIndex].Costume_Data.Length + 1);
            PSCfile.list[PSCcbChar.SelectedIndex].Costume_Data[PSCfile.list[PSCcbChar.SelectedIndex].Costume_Data.Length - 1].Data = new byte[196];
            PSCcbCostume.Items.Clear();
            for (int i = 0; i < PSCfile.list[PSCcbChar.SelectedIndex].Costume_Data.Length; i++)
            {
                PSCcbCostume.Items.Add(PSCfile.schema.getValueString(PSCkeys[0], ref PSCfile.list[PSCcbChar.SelectedIndex].Costume_Data[i].Data));
            }
        }

        private void PSCcharacterToolStripMenuItem1_Click(object sender, EventArgs e)
        {
            //remove character
            PSCcbChar.SelectedIndex = 0;
            Array.Resize<PSC_Char>(ref PSCfile.list, PSCfile.list.Length - 1);

            PSCcbChar.Items.Clear();
            for (int i = 0; i < PSCfile.list.Length; i++)
                PSCcbChar.Items.Add(PSCfile.list[i].id);
        }

        private void PSCcostumeToolStripMenuItem1_Click(object sender, EventArgs e)
        {
            //remove costume
            PSCcbCostume.SelectedIndex = 0;
            if (PSCfile.list[PSCcbChar.SelectedIndex].Costume_Data.Length != 1)
                Array.Resize<PSC_Costume>(ref PSCfile.list[PSCcbChar.SelectedIndex].Costume_Data, PSCfile.list[PSCcbChar.SelectedIndex].Costume_Data.Length - 1);

            PSCcbCostume.Items.Clear();
            for (int i = 0; i < PSCfile.list[PSCcbChar.SelectedIndex].Costume_Data.Length; i++)
            {
                PSCcbCostume.Items.Add(PSCfile.schema.getValueString(PSCkeys[0], ref PSCfile.list[PSCcbChar.SelectedIndex].Costume_Data[i].Data));
            }
        }

        private void PSCcopyToolStripMenuItem_Click(object sender, EventArgs e)
        {

        }

        private void pasteToolStripMenuItem_Click(object sender, EventArgs e)
        {

        }
        ////////////////////////////////////////////////////////////////////
        


        // AUR



        ///////////////////////////////////////////////////////////////////
    }
}
