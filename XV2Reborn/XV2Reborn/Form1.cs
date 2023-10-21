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
using System.Security.Cryptography.X509Certificates;
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

        string AURFileName;
        Aura[] Auras;
        Charlisting[] Chars;
        byte[] backup = new byte[104];
        bool AuraLock = false;
        bool CharLock = false;
        CharName[] clist = new CharName[] {
            new CharName(0, "Goku"),
            new CharName(1, "Bardock"),
            new CharName(2, "Goku SSJ4"),
            new CharName(3, "Goku SSJGod"),
            new CharName(4, "Goku GT"),
            new CharName(5, "Goten"),
            new CharName(6, "Gohan kid"),
            new CharName(7, "Gohan Teen"),
            new CharName(8, "Gohan Adult"),
            new CharName(9, "Piccolo"),
            new CharName(10, "Krillin"),
            new CharName(11, "Yamcha"),
            new CharName(12, "Tien"),
            new CharName(13, "Raditz"),
            new CharName(14, "Saibaman"),
            new CharName(15, "Nappa"),
            new CharName(16, "Vegeta"),
            new CharName(17, "Vegeta SSJ4"),
            new CharName(18, "Guldo"),
            new CharName(19, "Burter"),
            new CharName(20, "Recoome"),
            new CharName(21, "Jeice"),
            new CharName(22, "Ginyu"),
            new CharName(23, "Frieza 1st Form"),
            new CharName(24, "Frieza Final"),
            new CharName(25, "Frieza Full Power"),
            new CharName(26, "Trunks Future"),
            new CharName(27, "Trunks Kid"),
            new CharName(28, "Android 17"),
            new CharName(29, "Super 17"),
            new CharName(30, "Android 18"),
            new CharName(31, "Cell Perfect"),
            new CharName(32, "Cell Full Power"),
            new CharName(33, "Cell Jr."),
            new CharName(34, "Videl"),
            new CharName(35, "Majin Buu"),
            new CharName(36, "Super Buu"),
            new CharName(37, "Kid Buu"),
            new CharName(38, "Gotenks"),
            new CharName(39, "Vegito"),
            new CharName(40, "Broly"),
            new CharName(41, "Beerus"),
            new CharName(42, "Pan"),
            new CharName(47, "Added Character 1"),
            new CharName(48, "Eis Shenron"),
            new CharName(49, "Nuova Shenron"),
            new CharName(50, "Omega Shenron"),
            new CharName(51, "Gogeta SSJ4"),
            new CharName(52, "Hercule"),
            new CharName(53, "Demigra"),
            new CharName(55, "Added Character 2"),
            new CharName(59, "Nabana"),
            new CharName(60, "Raspberry"),
            new CharName(61, "Gohan 4 years old"),
            new CharName(62, "Mira"),
            new CharName(63, "Towa"),
            new CharName(64, "Added Character 3"),
            new CharName(65, "Whis"),
            new CharName(67, "Jaco"),
            new CharName(73, "Villinous Hercule"),
            new CharName(80, "Goku SSGSS"),
            new CharName(81, "Vegeta SSGSS"),
            new CharName(82, "Golden Frieza"),
            new CharName(100, "Human Male"),
            new CharName(101, "Human Female"),
            new CharName(102, "Saiyan Male"),
            new CharName(103, "Saiyan Female"),
            new CharName(104, "Namekian"),
            new CharName(105, "Frieza Race"),
            new CharName(106, "Majin Male"),
            new CharName(107, "Majin Female")
            };

        private IDB file = new IDB();
        private string FileName;
        private string[] keys;
        private bool lck = true;
        private IDB_Data currentItem;
        private IDB_Data copyItem;
        private int index;

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
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            if (Properties.Settings.Default.addonmodlist.Contains("Test123456789"))
            {
                Properties.Settings.Default.addonmodlist.Clear();
            }
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

            loadFiles();
            processDirectory(datapath);
        }
        public string FindCharName(int id)
        {
            for (int n = 0; n < clist.Length; n++)
            {
                if (clist[n].ID == id)
                    return clist[n].Name;
            }

            return "Unknown Character";
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

            lvMods.Items.Clear();
            this.lvMods.Items.AddRange((from i in Properties.Settings.Default.modlist.Cast<string>()
                                        select new ListViewItem(i.Split('|'))).ToArray());

            label1.Text = "Installed Mods: " + lvMods.Items.Count.ToString();
        }

        private void loadFiles()
        {

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

            // Load the AUR File
            AURFileName = Properties.Settings.Default.datafolder + @"/system/aura_setting.aur";
            byte[] AURfile = File.ReadAllBytes(AURFileName);
            Auras = new Aura[BitConverter.ToInt32(AURfile, 8)];
            int AuraAddress = BitConverter.ToInt32(AURfile, 12);
            for (int A = 0; A < Auras.Length; A++)
            {
                int id = BitConverter.ToInt32(AURfile, AuraAddress + (16 * A));
                Auras[id].Color = new int[BitConverter.ToInt32(AURfile, AuraAddress + (16 * A) + 8)];
                int CAddress = BitConverter.ToInt32(AURfile, AuraAddress + (16 * A) + 12);
                for (int C = 0; C < Auras[id].Color.Length; C++)
                    Auras[id].Color[BitConverter.ToInt32(AURfile, CAddress + (C * 8))] = BitConverter.ToInt32(AURfile, CAddress + (C * 8) + 4);
            }

            for (int A = 0; A < Auras.Length; A++)
                cbAuraList.Items.Add(A);
            int WAddress = BitConverter.ToInt32(AURfile, 20);
            Array.Copy(AURfile, WAddress, backup, 0, 104);
            cbAURChar.Items.Clear();
            Chars = new Charlisting[BitConverter.ToInt32(AURfile, 24)];
            int ChAddress = BitConverter.ToInt32(AURfile, 28);
            for (int C = 0; C < Chars.Length; C++)
            {
                Chars[C].Name = BitConverter.ToInt32(AURfile, ChAddress + (C * 16));
                Chars[C].Costume = BitConverter.ToInt32(AURfile, ChAddress + (C * 16) + 4);
                Chars[C].ID = BitConverter.ToInt32(AURfile, ChAddress + (C * 16) + 8);
                Chars[C].inf = BitConverter.ToBoolean(AURfile, ChAddress + (C * 16) + 12);

                cbAURChar.Items.Add(FindCharName(Chars[C].Name) + " - Costume " + Chars[C].Costume.ToString());
            }

            // Load the IDB File
            FileName = datapath + @"/system/item/talisman_item.idb";
            file.Read(FileName);
            file.SetSchema("IDB_Schema.csv");
            cbList.Items.Clear();
            for (int i = 0; i < file.items.Count; i++)
                cbList.Items.Add(file.items[i].id);

            lstData.Items.Clear();
            keys = file.schema.getKeys();
            foreach (string s in keys)
            {
                var Item = new ListViewItem(new[] { s, "0" });
                lstData.Items.Add(Item);
            }
        }
        void InstallModx2m(string arg)
        {
            Clean();

            string temp = Properties.Settings.Default.datafolder + @"\temp";

            if (Directory.Exists(temp) == false)
            {
                Directory.CreateDirectory(temp);
            }

            ZipFile.ExtractToDirectory(arg, temp);

            string xmlfile = temp + "//x2m.xml";

            if (File.Exists(xmlfile))
            {

                string modtype = File.ReadAllLines(xmlfile)[1].Replace("<X2M type=\"", "").Replace("\">", "");
                string modname = File.ReadAllLines(xmlfile)[3].Replace("    <MOD_NAME value=\"", "").Replace("\" />", "");
                string modauthor = File.ReadAllLines(xmlfile)[4].Replace("    <MOD_AUTHOR value=\"", "").Replace("\" />", "");

                var files = Directory.EnumerateFiles(temp, "*.*", SearchOption.AllDirectories);

                if (modtype == "NEW_CHARACTER")
                {
                    string Modid = File.ReadAllLines(xmlfile)[7].Replace("    <ENTRY_NAME value=\"", "").Replace("\" />", "");

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
                        text = text.Replace(@"\UI\SEL.DDS", @"\ui\texture\CHARA01\" + Modid + @"_000.dds");
                        text = text.Replace(@"data\" + Modid, @"data\chara\" + Modid);
                        File.WriteAllText(Properties.Settings.Default.datafolder + @"\installed\" + modname + @".xml", text);

                        MoveDirectory(temp + @"/" + Modid, Properties.Settings.Default.datafolder + @"/chara/" + Modid);
                        File.Move(temp + @"/UI/SEL.DDS", Properties.Settings.Default.datafolder + @"/ui/texture/CHARA01/" + Modid + "_000.dds");

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
                        Properties.Settings.Default.addonmodlist.Add(modname);
                        Properties.Settings.Default.Save();

                        int numberid = 160 + Properties.Settings.Default.addonmodlist.Count; //160 is a fixed number

                        // ADD DATA TO THE CMS FILE HEREEEEE

                        Char_Model_Spec c = new Char_Model_Spec();
                        c.Paths = new string[9];
                        c.Paths[0] = Modid;
                        c.Paths[1] = Modid;
                        c.Paths[2] = Modid;
                        c.Paths[3] = Modid;
                        c.Paths[4] = Modid;
                        c.Paths[5] = Modid;
                        c.Paths[6] = Modid;
                        c.Paths[7] = Modid;
                        c.Paths[8] = Modid;
                        c.shortname = Modid;
                        c.id = numberid;

                        cms.Add(c);
                        CMSCombobox.Items.Clear();
                        for (int i = 0; i < cms.Count; i++)
                            CMSCombobox.Items.Add(cms[i].id.ToString(numberid.ToString()) + " - " + Modid);

                        Array.Resize<bool>(ref CMSselective, CMSselective.Length + 1);

                        CMSselective[CMSselective.Length - 1] = false;
                        CMS.Write(CMSFileName, cms.ToArray());


                        ///////////////////////////////////


                        // ADD DATA TO THE CSO FILE HEREEEEE

                        CSO_Data c2 = new CSO_Data();
                        c2.Paths = new string[4];
                        c2.Paths[0] = "";
                        c2.Paths[1] = "";
                        c2.Paths[2] = "";
                        c2.Paths[3] = "";
                        c2.Char_ID = numberid;
                        CSOData.Add(c2);

                        cbListCSO.SelectedIndex = 0;
                        cbListCSO.Items.Clear();
                        for (int i = 0; i < CSOData.Count; i++)
                            cbListCSO.Items.Add("Character " + CSOData[i].Char_ID.ToString(numberid.ToString()) + " - Costume " + CSOData[i].Costume_ID.ToString("00"));

                        CSO.Write(CSOData.ToArray(), CSOFileName);

                        ///////////////////////////////////

                        // ADD DATA TO THE CUS FILE HEREEEEE
                        this.CUSfile.css.Add(new charSkillSet()
                        {
                            skill = new short[10],
                            charID = numberid
                        });
                        this.CUSlck = false;
                        this.UpdateCharlist();
                        this.CUSlck = true;

                        CUSfile.writeCUS(CUSFileName);

                        ///////////////////////////////////

                        // ADD DATA TO THE PSC FILE HEREEEEE

                        // NOT YET IMPLEMENTED

                        ///////////////////////////////////

                        // ADD DATA TO THE AUR FILE HEREEEEE

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
                                sw.WriteLine(@"XMLSerializer.exe aura_setting.aur");
                            }
                        }

                        p.WaitForExit();

                        string aurtext = File.ReadAllText(datapath + @"/system/aura_setting.aur.xml");
                        aurtext = aurtext.Replace("  </CharacterAuras>\r\n</AUR>", "    <CharacterAura Chara_ID=\"" + numberid + "\" Costume=\"0\" Aura_ID=\"0\" Glare=\"False\" />\r\n  </CharacterAuras>\r\n</AUR>");
                        File.WriteAllText(datapath + @"/system/aura_setting.aur.xml", aurtext);

                        Thread.Sleep(1000);

                        p.Start();

                        using (StreamWriter sw = p.StandardInput)
                        {
                            if (sw.BaseStream.CanWrite)
                            {
                                const string quote = "\"";

                                sw.WriteLine("cd " + Properties.Settings.Default.datafolder + @"\system");
                                sw.WriteLine(@"XMLSerializer.exe " + quote + Properties.Settings.Default.datafolder + @"\system\aura_setting.aur.xml" + quote);
                            }
                        }

                        p.WaitForExit();

                        if (File.Exists(Properties.Settings.Default.datafolder + @"\system\aura_setting.aur.xml"))
                        {
                            File.Delete(Properties.Settings.Default.datafolder + @"\system\aura_setting.aur.xml");
                        }

                        ///////////////////////////////////

                        string Charalist = Properties.Settings.Default.datafolder + @"\scripts\action_script\Charalist.as";

                        var text3 = new StringBuilder();

                        foreach (string s in File.ReadAllLines(Charalist))
                        {
                            text3.AppendLine(s.Replace("]]]", "]],[[\"" + Modid + "\",0,0,0,false,[-1,-1],Dlc_Def]]]"));
                        }

                        using (var file = new StreamWriter(File.Create(Charalist)))
                        {
                            file.Write(text3.ToString());
                        }

                        CompileScripts();

                        msgData[] expand = new msgData[MSGfile.data.Length + 1];
                        Array.Copy(MSGfile.data, expand, MSGfile.data.Length);
                        string nameid = MSGfile.data[MSGfile.data.Length - 1].NameID;
                        expand[expand.Length - 1].ID = MSGfile.data.Length;
                        expand[expand.Length - 1].Lines = new string[] { modname };
                        expand[expand.Length - 1].NameID = "chara_" + Modid + "_" + "000";

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
                else if(modtype == "REPLACER")
                {
                    string[] row = { modname, modauthor, "Replacer" };
                    ListViewItem lvi = new ListViewItem(row);
                    lvMods.Items.Add(lvi);

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
                    text = text.Replace(@"\JUNGLE\data", "");
                    File.WriteAllText(Properties.Settings.Default.datafolder + @"\installed\" + modname + @".xml", text);

                    MoveDirectory(temp + @"/JUNGLE/data" , Properties.Settings.Default.datafolder + @"/");
                }
                else
                {
                    MessageBox.Show("X2M Format \"" + modtype + "\" not currently implemented");
                }
            }
            Clean();
            saveLvItems();
            loadFiles();
            MessageBox.Show("Installation Completed", "Success", MessageBoxButtons.OK, MessageBoxIcon.Information);
        }

        private void uninstallModToolStripMenuItem_Click(object sender, EventArgs e)
        {

            ListView.SelectedIndexCollection indices = lvMods.SelectedIndices;
            if (indices.Count > 0)
            {
                Process p = new Process();
                ProcessStartInfo info = new ProcessStartInfo();
                info.FileName = "cmd.exe";
                info.CreateNoWindow = true;
                info.WindowStyle = ProcessWindowStyle.Hidden;
                info.RedirectStandardInput = true;
                info.UseShellExecute = false;

                if (File.Exists(Properties.Settings.Default.datafolder + @"\installed\" + lvMods.SelectedItems[0].Text + @".xml"))
                {
                    string[] lines = File.ReadAllLines(Properties.Settings.Default.datafolder + @"\installed\" + lvMods.SelectedItems[0].Text + @".xml");

                    foreach (string line in lines)
                    {
                        if(File.Exists(line))
                            File.Delete(line);
                    }

                    File.Delete(Properties.Settings.Default.datafolder + @"\installed\" + lvMods.SelectedItems[0].Text + @".xml");

                    //End
                }

                if (File.Exists(Properties.Settings.Default.datafolder + @"\installed\" + lvMods.SelectedItems[0].Text + @" 2.xml"))
                {
                    string id = File.ReadAllLines(Properties.Settings.Default.datafolder + @"\installed\" + lvMods.SelectedItems[0].Text + @" 2.xml").First();

                    string Charalist = Properties.Settings.Default.datafolder + @"\scripts\action_script\Charalist.as";

                    var text3 = new StringBuilder();

                    foreach (string s in File.ReadAllLines(Charalist))
                    {
                        text3.AppendLine(s.Replace(",[[\"" + id + "\",0,0,0,false,[-1,-1],Dlc_Def]]]", "]"));
                    }

                    using (var file = new StreamWriter(File.Create(Charalist)))
                    {
                        file.Write(text3.ToString());
                    }
                    CompileScripts();

                    //string qxd = Properties.Settings.Default.datafolder + @"\quest\TMQ\tmq_data.qxd";
                    //ReplaceTextInFile(qxd, id, "XXX");
                    File.Delete(Properties.Settings.Default.datafolder + @"\installed\" + lvMods.SelectedItems[0].Text + @"2.xml");

                }

                lvMods.Items.Remove(lvMods.SelectedItems[0]);
                MessageBox.Show("Mod uninstalled successfully", "Success", MessageBoxButtons.OK, MessageBoxIcon.Information);
                saveLvItems();
            }
        }

        private void processDirectory(string startLocation)
        {
            foreach (var directory in Directory.GetDirectories(startLocation))
            {
                processDirectory(directory);
                if (Directory.GetFiles(directory).Length == 0 &&
                    Directory.GetDirectories(directory).Length == 0)
                {
                    Directory.Delete(directory, false);
                }
            }
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
            moddialog.Filter = "Xenoverse 2 mod files | *.x2m";
            moddialog.Multiselect = true;
            moddialog.RestoreDirectory = true;
            moddialog.Title = "Install Mod";

            if (moddialog.ShowDialog() == DialogResult.OK)
            {
                foreach(var mod in moddialog.FileNames) 
                {
                    if (MessageBox.Show("Do you want to install mod \"" + Path.GetFileName(mod) + "\"?", "Mod Installation", MessageBoxButtons.YesNoCancel, MessageBoxIcon.Information) == DialogResult.Yes)
                        InstallModx2m(mod);
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

            if (File.Exists(datapath + "\\ui\\iggy\\CHARASELE.swf"))
                File.Delete(datapath + "\\ui\\iggy\\CHARASELE.swf");
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
        private void comboBox1_SelectedIndexChanged(object sender, EventArgs e)
        {
            Settings.Default.language = comboBox1.SelectedItem.ToString();
            Settings.Default.Save();
        }

        // CSS
        
        ///////////////////////////////////////////////////////////////////

        private void msgSaveToolStripMenuItem_Click(object sender, EventArgs e)
        {
            msgStream.Save(MSGfile, MSGFileName);
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
        private void checkBox1_CheckedChanged(object sender, EventArgs e)
        {
            CMSselective[CMSCombobox.SelectedIndex] = checkBox1.Checked;
        }

        private void editCMSFileToolStripMenuItem_Click(object sender, EventArgs e)
        {
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
                    sw.WriteLine(@"XMLSerializer.exe char_model_spec.cms");
                }
            }

            p.WaitForExit();

            Process p2 = Process.Start(Properties.Settings.Default.datafolder + @"\system\char_model_spec.cms.xml");

            p2.WaitForExit();

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

            MessageBox.Show("CMS File Compiled Successfully", "Success", MessageBoxButtons.OK, MessageBoxIcon.Information);

            if (File.Exists(Properties.Settings.Default.datafolder + @"\system\char_model_spec.cms.xml"))
            {
                File.Delete(Properties.Settings.Default.datafolder + @"\system\char_model_spec.cms.xml");
            }
            loadFiles();
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

        private void editCSOFileToolStripMenuItem_Click(object sender, EventArgs e)
        {
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
                    sw.WriteLine(@"XMLSerializer.exe chara_sound.cso");
                }
            }

            p.WaitForExit();

            Process p2 = Process.Start(Properties.Settings.Default.datafolder + @"\system\chara_sound.cso.xml");

            p2.WaitForExit();

            p.Start();

            using (StreamWriter sw = p.StandardInput)
            {
                if (sw.BaseStream.CanWrite)
                {
                    const string quote = "\"";

                    sw.WriteLine("cd " + Properties.Settings.Default.datafolder + @"\system");
                    sw.WriteLine(@"XMLSerializer.exe " + quote + Properties.Settings.Default.datafolder + @"\system\chara_sound.cso.xml" + quote);
                }
            }

            p.WaitForExit();

            MessageBox.Show("CSO File Compiled Successfully", "Success", MessageBoxButtons.OK, MessageBoxIcon.Information);

            if (File.Exists(Properties.Settings.Default.datafolder + @"\system\chara_sound.cso.xml"))
            {
                File.Delete(Properties.Settings.Default.datafolder + @"\system\chara_sound.cso.xml");
            }
            loadFiles();
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
            CUSfile.writeCUS(CUSFileName);
        }

        private void CUSaddToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (this.tabControl2.SelectedIndex == 0)
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
            if (this.tabControl2.SelectedIndex == 0)
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
            if (this.tabControl2.SelectedIndex == 0)
                this.CUScopySet = this.CUScurrentSet;
            else
                this.CUScopySkill = this.CUScurrentSkill;
        }

        private void CUSpasteToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (this.tabControl2.SelectedIndex == 0)
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

        private void editCUSFileToolStripMenuItem_Click(object sender, EventArgs e)
        {
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
            loadFiles();
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
            PSCCopy = PSCfile.list[PSCcbChar.SelectedIndex].Costume_Data[PSCcbCostume.SelectedIndex];

        }

        private void pasteToolStripMenuItem_Click(object sender, EventArgs e)
        {
            PSCfile.list[PSCcbChar.SelectedIndex].Costume_Data[PSCcbCostume.SelectedIndex] = PSCCopy;
            PSCcbCostume.Items[PSCcbCostume.SelectedIndex] = PSCfile.schema.getValueString(PSCkeys[0], ref PSCfile.list[PSCcbChar.SelectedIndex].Costume_Data[PSCcbCostume.SelectedIndex].Data);
            PSClstData.Items.Clear();
            foreach (string s in PSCkeys)
            {
                var Item = new ListViewItem(new[] { s, PSCfile.schema.getValueString(s, ref PSCfile.list[PSCcbChar.SelectedIndex].Costume_Data[PSCcbCostume.SelectedIndex].Data) });
                PSClstData.Items.Add(Item);
            }
        }

        ////////////////////////////////////////////////////////////////////

        // AUR

        struct Aura
        {
            public int[] Color;
        }

        struct Charlisting
        {
            public int Name;
            public int Costume;
            public int ID;
            public bool inf;
        }

        struct CharName
        {
            public int ID;
            public string Name;

            public CharName(int i, string n)
            {
                ID = i;
                Name = n;
            }
        }

        private void saveAURFileToolStripMenuItem_Click(object sender, EventArgs e)
        {
            List<byte> file = new List<byte>();
            byte[] signature = new byte[] { 0x23, 0x41, 0x55, 0x52, 0xFE, 0xFF, 0x20, 0x00 };
            byte[] Top = new byte[24];
            byte[] Aura1 = new byte[16 * Auras.Length];
            List<byte> Aura2 = new List<byte>();
            Array.Copy(BitConverter.GetBytes(Auras.Length), 0, Top, 0, 4);
            Array.Copy(BitConverter.GetBytes(32), 0, Top, 4, 4);
            for (int A = 0; A < Auras.Length; A++)
            {
                Array.Copy(BitConverter.GetBytes(A), 0, Aura1, (A * 16), 4);
                Array.Copy(BitConverter.GetBytes(Auras[A].Color.Length), 0, Aura1, (A * 16) + 8, 4);
                Array.Copy(BitConverter.GetBytes(32 + Aura1.Length + Aura2.Count), 0, Aura1, (A * 16) + 12, 4);
                for (int C = 0; C < Auras[A].Color.Length; C++)
                {
                    Aura2.AddRange(BitConverter.GetBytes(C));
                    if (Auras[A].Color[C] < 0)
                        Aura2.AddRange(new byte[] { 0xFF, 0xFF, 0xFF, 0xFF });
                    else
                        Aura2.AddRange(BitConverter.GetBytes(Auras[A].Color[C]));
                }
            }

            int length = 32 + Aura1.Length + Aura2.Count;

            Array.Copy(BitConverter.GetBytes(7), 0, Top, 8, 4);
            Array.Copy(BitConverter.GetBytes(length), 0, Top, 12, 4);
            //backup shift - 28,39,49,58,69,80,93
            Array.Copy(BitConverter.GetBytes(length + 28), 0, backup, 0, 4);
            Array.Copy(BitConverter.GetBytes(length + 39), 0, backup, 4, 4);
            Array.Copy(BitConverter.GetBytes(length + 49), 0, backup, 8, 4);
            Array.Copy(BitConverter.GetBytes(length + 58), 0, backup, 12, 4);
            Array.Copy(BitConverter.GetBytes(length + 69), 0, backup, 16, 4);
            Array.Copy(BitConverter.GetBytes(length + 80), 0, backup, 20, 4);
            Array.Copy(BitConverter.GetBytes(length + 93), 0, backup, 24, 4);

            length += backup.Length;

            byte[] filler = new byte[16 - (length % 16)];

            if (filler.Length != 16)
                length += filler.Length;

            Array.Copy(BitConverter.GetBytes(Chars.Length), 0, Top, 16, 4);
            Array.Copy(BitConverter.GetBytes(length), 0, Top, 20, 4);

            List<byte> Charbytes = new List<byte>();

            for (int C = 0; C < Chars.Length; C++)
            {
                Charbytes.AddRange(BitConverter.GetBytes(Chars[C].Name));
                Charbytes.AddRange(BitConverter.GetBytes(Chars[C].Costume));
                Charbytes.AddRange(BitConverter.GetBytes(Chars[C].ID));
                Charbytes.AddRange(BitConverter.GetBytes(Chars[C].inf));
                Charbytes.AddRange(new byte[] { 0x00, 0x00, 0x00 });
            }

            file.AddRange(signature);
            file.AddRange(Top);
            file.AddRange(Aura1);
            file.AddRange(Aura2);
            file.AddRange(backup);
            if (filler.Length != 16)
                file.AddRange(filler);
            file.AddRange(Charbytes);

            FileStream newfile = new FileStream(AURFileName, FileMode.Create);
            newfile.Write(file.ToArray(), 0, file.Count);
            newfile.Close();
        }

        private void addAuraToolStripMenuItem_Click(object sender, EventArgs e)
        {
            // add aura

            Aura[] Expand = new Aura[Auras.Length + 1];
            Array.Copy(Auras, Expand, Auras.Length);
            Auras = Expand;
            Auras[Auras.Length - 1].Color = new int[] { 0, 0, 0, 0, 0, 0, 0 };

            cbAuraList.Items.Clear();
            for (int A = 0; A < Auras.Length; A++)
                cbAuraList.Items.Add(A);
        }

        private void removeAuraToolStripMenuItem_Click(object sender, EventArgs e)
        {
            // remove aura

            if (cbAuraList.Items.Count > 22)
            {
                Aura[] reduce = new Aura[Auras.Length - 1];
                Array.Copy(Auras, reduce, Auras.Length - 1);
            }

            cbAuraList.Items.Clear();
            for (int A = 0; A < Auras.Length; A++)
                cbAuraList.Items.Add(A);
        }

        private void cbAURChar_SelectedIndexChanged(object sender, EventArgs e)
        {
            CharLock = true;
            txtAURID.Text = Chars[cbAURChar.SelectedIndex].ID.ToString();
            chkInf.Checked = Chars[cbAURChar.SelectedIndex].inf;
            CharLock = false;
        }

        private void cbAuraList_SelectedIndexChanged(object sender, EventArgs e)
        {
            AuraLock = true;
            txtBStart.Text = Auras[cbAuraList.SelectedIndex].Color[0].ToString();
            txtBLoop.Text = Auras[cbAuraList.SelectedIndex].Color[1].ToString();
            txtBEnd.Text = Auras[cbAuraList.SelectedIndex].Color[2].ToString();
            txtKiCharge.Text = Auras[cbAuraList.SelectedIndex].Color[3].ToString();
            txtkiMax.Text = Auras[cbAuraList.SelectedIndex].Color[4].ToString();
            txtHenshinStart.Text = Auras[cbAuraList.SelectedIndex].Color[5].ToString();
            txtHenshinEnd.Text = Auras[cbAuraList.SelectedIndex].Color[6].ToString();
            AuraLock = false;
        }

        private void editAURFileToolStripMenuItem_Click(object sender, EventArgs e)
        {
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
                    sw.WriteLine(@"XMLSerializer.exe aura_setting.aur");
                }
            }

            p.WaitForExit();

            Process p2 = Process.Start(Properties.Settings.Default.datafolder + @"\system\aura_setting.aur.xml");

            p2.WaitForExit();

            p.Start();

            using (StreamWriter sw = p.StandardInput)
            {
                if (sw.BaseStream.CanWrite)
                {
                    const string quote = "\"";

                    sw.WriteLine("cd " + Properties.Settings.Default.datafolder + @"\system");
                    sw.WriteLine(@"XMLSerializer.exe " + quote + Properties.Settings.Default.datafolder + @"\system\aura_setting.aur.xml" + quote);
                }
            }

            p.WaitForExit();

            MessageBox.Show("AUR File Compiled Successfully", "Success", MessageBoxButtons.OK, MessageBoxIcon.Information);

            if (File.Exists(Properties.Settings.Default.datafolder + @"\system\aura_setting.aur.xml"))
            {
                File.Delete(Properties.Settings.Default.datafolder + @"\system\aura_setting.aur.xml");
            }
            loadFiles();
        }

        private void txtAURID_TextChanged(object sender, EventArgs e)
        {
            int Num;
            if (int.TryParse(txtAURID.Text, out Num) && !CharLock)
                Chars[cbAURChar.SelectedIndex].ID = Num;
        }

        private void chkInf_CheckedChanged(object sender, EventArgs e)
        {
            if (!CharLock)
                Chars[cbAURChar.SelectedIndex].inf = chkInf.Checked;
        }

        private void txtBStart_TextChanged(object sender, EventArgs e)
        {
            int Num;
            if (int.TryParse(txtBStart.Text, out Num) && !AuraLock)
                Auras[cbAuraList.SelectedIndex].Color[0] = Num;
        }

        private void txtBEnd_TextChanged(object sender, EventArgs e)
        {
            int Num;
            if (int.TryParse(txtBEnd.Text, out Num) && !AuraLock)
                Auras[cbAuraList.SelectedIndex].Color[2] = Num;
        }

        private void txtkiMax_TextChanged(object sender, EventArgs e)
        {
            int Num;
            if (int.TryParse(txtkiMax.Text, out Num) && !AuraLock)
                Auras[cbAuraList.SelectedIndex].Color[4] = Num;
        }

        private void txtHenshinEnd_TextChanged(object sender, EventArgs e)
        {
            int Num;
            if (int.TryParse(txtHenshinEnd.Text, out Num) && !AuraLock)
                Auras[cbAuraList.SelectedIndex].Color[6] = Num;
        }

        private void txtHenshinStart_TextChanged(object sender, EventArgs e)
        {
            int Num;
            if (int.TryParse(txtHenshinStart.Text, out Num) && !AuraLock)
                Auras[cbAuraList.SelectedIndex].Color[5] = Num;
        }

        private void txtKiCharge_TextChanged(object sender, EventArgs e)
        {
            int Num;
            if (int.TryParse(txtKiCharge.Text, out Num) && !AuraLock)
                Auras[cbAuraList.SelectedIndex].Color[3] = Num;
        }

        private void txtBLoop_TextChanged(object sender, EventArgs e)
        {
            int Num;
            if (int.TryParse(txtBLoop.Text, out Num) && !AuraLock)
                Auras[cbAuraList.SelectedIndex].Color[1] = Num;
        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            Clean();
            if(Directory.Exists(datapath))
                processDirectory(datapath);
        }



        ///////////////////////////////////////////////////////////////////

        // IDB
        private void comboBoxlist_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (!this.lck)
                return;
            this.lck = false;
            this.currentItem = this.file.items[this.cbList.SelectedIndex];
            this.txtID.Text = this.currentItem.id.ToString();
            this.txtStar.Text = this.currentItem.star.ToString();
            this.txtNa.Text = this.currentItem.name.ToString();
            this.txtInfo.Text = this.currentItem.desc.ToString();
            this.txtType.Text = this.currentItem.type.ToString();
            this.txtUnk1.Text = this.currentItem.unk1.ToString();
            this.txtUnk2.Text = this.currentItem.unk2.ToString();
            this.txtUnk3.Text = this.currentItem.unk3.ToString();
            this.txtBuy.Text = this.currentItem.buy.ToString();
            this.txtSell.Text = this.currentItem.sell.ToString();
            this.txtRace.Text = this.currentItem.racelock.ToString();
            this.txtTP.Text = this.currentItem.tp.ToString();
            this.txtExtra.Text = this.currentItem.extra.ToString();
            switch (this.cbEffect.SelectedIndex)
            {
                case 0:
                    this.lstData.Items.Clear();
                    foreach (string key in this.keys)
                        this.lstData.Items.Add(new ListViewItem(new string[2]
                        {
              key,
              this.file.schema.getValueString(key, ref this.currentItem.E_set1)
                        }));
                    break;
                case 1:
                    this.lstData.Items.Clear();
                    foreach (string key in this.keys)
                        this.lstData.Items.Add(new ListViewItem(new string[2]
                        {
              key,
              this.file.schema.getValueString(key, ref this.currentItem.E_set2)
                        }));
                    break;
                case 2:
                    this.lstData.Items.Clear();
                    foreach (string key in this.keys)
                        this.lstData.Items.Add(new ListViewItem(new string[2]
                        {
              key,
              this.file.schema.getValueString(key, ref this.currentItem.E_set3)
                        }));
                    break;
            }
            this.index = this.cbList.SelectedIndex;
            this.lck = true;
        }

        private void cbEffect_SelectedIndexChanged(object sender, EventArgs e)
        {
            switch (this.cbEffect.SelectedIndex)
            {
                case 0:
                    this.lstData.Items.Clear();
                    foreach (string key in this.keys)
                        this.lstData.Items.Add(new ListViewItem(new string[2]
                        {
              key,
              this.file.schema.getValueString(key, ref this.currentItem.E_set1)
                        }));
                    break;
                case 1:
                    this.lstData.Items.Clear();
                    foreach (string key in this.keys)
                        this.lstData.Items.Add(new ListViewItem(new string[2]
                        {
              key,
              this.file.schema.getValueString(key, ref this.currentItem.E_set2)
                        }));
                    break;
                case 2:
                    this.lstData.Items.Clear();
                    foreach (string key in this.keys)
                        this.lstData.Items.Add(new ListViewItem(new string[2]
                        {
              key,
              this.file.schema.getValueString(key, ref this.currentItem.E_set3)
                        }));
                    break;
            }
        }

        private void lstData_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (this.lstData.SelectedItems.Count == 0 || !this.lck)
                return;
            this.lck = false;
            this.txtName.Text = this.lstData.SelectedItems[0].SubItems[0].Text;
            this.txtVal.Text = this.lstData.SelectedItems[0].SubItems[1].Text;
            this.lck = true;
        }

        private void txtVal_TextChanged(object sender, EventArgs e)
        {
            if (!this.lck)
                return;
            switch (this.cbEffect.SelectedIndex)
            {
                case 0:
                    this.lstData.SelectedItems[0].SubItems[1].Text = this.txtVal.Text;
                    this.file.schema.setValueString(this.txtName.Text, ref this.currentItem.E_set1, this.txtVal.Text);
                    break;
                case 1:
                    this.lstData.SelectedItems[0].SubItems[1].Text = this.txtVal.Text;
                    this.file.schema.setValueString(this.txtName.Text, ref this.currentItem.E_set2, this.txtVal.Text);
                    break;
                case 2:
                    this.lstData.SelectedItems[0].SubItems[1].Text = this.txtVal.Text;
                    this.file.schema.setValueString(this.txtName.Text, ref this.currentItem.E_set3, this.txtVal.Text);
                    break;
            }
            this.file.items[this.index] = this.currentItem;
        }

        private void txtID_TextChanged(object sender, EventArgs e)
        {
            short result;
            if (!short.TryParse(this.txtID.Text, out result) || !this.lck)
                return;
            this.currentItem.id = result;
            if (this.index >= 0)
                this.file.items[this.index] = this.currentItem;
            this.cbList.Items.Clear();
            for (int index = 0; index < this.file.items.Count; ++index)
                this.cbList.Items.Add((object)this.file.items[index].id);
        }

        private void txtStar_TextChanged(object sender, EventArgs e)
        {
            short result;
            if (!short.TryParse(this.txtStar.Text, out result) || !this.lck)
                return;
            this.currentItem.star = result;
            this.file.items[this.index] = this.currentItem;
        }

        private void txtNa_TextChanged(object sender, EventArgs e)
        {
            short result;
            if (!short.TryParse(this.txtNa.Text, out result) || !this.lck)
                return;
            this.currentItem.name = result;
            this.file.items[this.index] = this.currentItem;
        }

        private void txtInfo_TextChanged(object sender, EventArgs e)
        {
            short result;
            if (!short.TryParse(this.txtInfo.Text, out result) || !this.lck)
                return;
            this.currentItem.desc = result;
            this.file.items[this.index] = this.currentItem;
        }

        private void txtType_TextChanged(object sender, EventArgs e)
        {
            short result;
            if (!short.TryParse(this.txtType.Text, out result) || !this.lck)
                return;
            this.currentItem.type = result;
            this.file.items[this.index] = this.currentItem;
        }

        private void txtUnk1_TextChanged(object sender, EventArgs e)
        {
            short result;
            if (!short.TryParse(this.txtUnk1.Text, out result) || !this.lck)
                return;
            this.currentItem.unk1 = result;
            this.file.items[this.index] = this.currentItem;
        }

        private void txtUnk2_TextChanged(object sender, EventArgs e)
        {
            short result;
            if (!short.TryParse(this.txtUnk2.Text, out result) || !this.lck)
                return;
            this.currentItem.unk2 = result;
            this.file.items[this.index] = this.currentItem;
        }

        private void txtUnk3_TextChanged(object sender, EventArgs e)
        {
            short result;
            if (!short.TryParse(this.txtUnk3.Text, out result) || !this.lck)
                return;
            this.currentItem.unk3 = result;
            this.file.items[this.index] = this.currentItem;
        }

        private void txtBuy_TextChanged(object sender, EventArgs e)
        {
            int result;
            if (!int.TryParse(this.txtBuy.Text, out result) || !this.lck)
                return;
            this.currentItem.buy = result;
            this.file.items[this.index] = this.currentItem;
        }

        private void txtSell_TextChanged(object sender, EventArgs e)
        {
            int result;
            if (!int.TryParse(this.txtSell.Text, out result) || !this.lck)
                return;
            this.currentItem.sell = result;
            this.file.items[this.index] = this.currentItem;
        }

        private void txtRace_TextChanged(object sender, EventArgs e)
        {
            int result;
            if (!int.TryParse(this.txtRace.Text, out result) || !this.lck)
                return;
            this.currentItem.racelock = result;
            this.file.items[this.index] = this.currentItem;
        }

        private void txtTP_TextChanged(object sender, EventArgs e)
        {
            int result;
            if (!int.TryParse(this.txtTP.Text, out result) || !this.lck)
                return;
            this.currentItem.tp = result;
            this.file.items[this.index] = this.currentItem;
        }

        private void txtExtra_TextChanged(object sender, EventArgs e)
        {
            int result;
            if (!int.TryParse(this.txtExtra.Text, out result) || !this.lck)
                return;
            this.currentItem.extra = result;
            this.file.items[this.index] = this.currentItem;
        }
        private void saveToolStripMenuItem_Click(object sender, EventArgs e) => this.file.Write(this.FileName);

        private void copyToolStripMenuItem_Click(object sender, EventArgs e) => this.copyItem = this.currentItem;

        private void pasteToolStripMenuItem1_Click(object sender, EventArgs e)
        {
            short id = this.currentItem.id;
            this.currentItem = this.copyItem;
            this.currentItem.id = id;
            if (!this.lck)
                return;
            this.lck = false;
            this.txtID.Text = this.currentItem.id.ToString();
            this.txtStar.Text = this.currentItem.star.ToString();
            this.txtNa.Text = this.currentItem.name.ToString();
            this.txtInfo.Text = this.currentItem.desc.ToString();
            this.txtType.Text = this.currentItem.type.ToString();
            this.txtUnk1.Text = this.currentItem.unk1.ToString();
            this.txtUnk2.Text = this.currentItem.unk2.ToString();
            this.txtUnk3.Text = this.currentItem.unk3.ToString();
            this.txtBuy.Text = this.currentItem.buy.ToString();
            this.txtSell.Text = this.currentItem.sell.ToString();
            this.txtRace.Text = this.currentItem.racelock.ToString();
            this.txtTP.Text = this.currentItem.tp.ToString();
            this.txtExtra.Text = this.currentItem.extra.ToString();
            switch (this.cbEffect.SelectedIndex)
            {
                case 0:
                    this.lstData.Items.Clear();
                    foreach (string key in this.keys)
                        this.lstData.Items.Add(new ListViewItem(new string[2]
                        {
              key,
              this.file.schema.getValueString(key, ref this.currentItem.E_set1)
                        }));
                    break;
                case 1:
                    this.lstData.Items.Clear();
                    foreach (string key in this.keys)
                        this.lstData.Items.Add(new ListViewItem(new string[2]
                        {
              key,
              this.file.schema.getValueString(key, ref this.currentItem.E_set2)
                        }));
                    break;
                case 2:
                    this.lstData.Items.Clear();
                    foreach (string key in this.keys)
                        this.lstData.Items.Add(new ListViewItem(new string[2]
                        {
              key,
              this.file.schema.getValueString(key, ref this.currentItem.E_set3)
                        }));
                    break;
            }
            this.file.items[this.cbList.SelectedIndex] = this.currentItem;
            this.lck = true;
            this.cbList.Items.Clear();
            for (int index = 0; index < this.file.items.Count; ++index)
                this.cbList.Items.Add((object)this.file.items[index].id);
        }

        private void talismanToolStripMenuItem_Click(object sender, EventArgs e)
        {
            // Load the IDB File
            FileName = datapath + @"/system/item/talisman_item.idb";
            file.Read(FileName);
            file.SetSchema("IDB_Schema.csv");
            cbList.Items.Clear();
            for (int i = 0; i < file.items.Count; i++)
                cbList.Items.Add(file.items[i].id);

            lstData.Items.Clear();
            keys = file.schema.getKeys();
            foreach (string s in keys)
            {
                var Item = new ListViewItem(new[] { s, "0" });
                lstData.Items.Add(Item);
            }
        }

        private void accessoryToolStripMenuItem_Click(object sender, EventArgs e)
        {
            // Load the IDB File
            FileName = datapath + @"/system/item/accessory_item.idb";
            file.Read(FileName);
            file.SetSchema("IDB_Schema.csv");
            cbList.Items.Clear();
            for (int i = 0; i < file.items.Count; i++)
                cbList.Items.Add(file.items[i].id);

            lstData.Items.Clear();
            keys = file.schema.getKeys();
            foreach (string s in keys)
            {
                var Item = new ListViewItem(new[] { s, "0" });
                lstData.Items.Add(Item);
            }
        }

        private void battleToolStripMenuItem_Click(object sender, EventArgs e)
        {
            // Load the IDB File
            FileName = datapath + @"/system/item/battle_item.idb";
            file.Read(FileName);
            file.SetSchema("IDB_Schema.csv");
            cbList.Items.Clear();
            for (int i = 0; i < file.items.Count; i++)
                cbList.Items.Add(file.items[i].id);

            lstData.Items.Clear();
            keys = file.schema.getKeys();
            foreach (string s in keys)
            {
                var Item = new ListViewItem(new[] { s, "0" });
                lstData.Items.Add(Item);
            }
        }

        private void costumeTopToolStripMenuItem_Click(object sender, EventArgs e)
        {
            // Load the IDB File
            FileName = datapath + @"/system/item/costume_top_item.idb";
            file.Read(FileName);
            file.SetSchema("IDB_Schema.csv");
            cbList.Items.Clear();
            for (int i = 0; i < file.items.Count; i++)
                cbList.Items.Add(file.items[i].id);

            lstData.Items.Clear();
            keys = file.schema.getKeys();
            foreach (string s in keys)
            {
                var Item = new ListViewItem(new[] { s, "0" });
                lstData.Items.Add(Item);
            }
        }

        private void costumeGlovesToolStripMenuItem_Click(object sender, EventArgs e)
        {
            // Load the IDB File
            FileName = datapath + @"/system/item/costume_gloves_item.idb";
            file.Read(FileName);
            file.SetSchema("IDB_Schema.csv");
            cbList.Items.Clear();
            for (int i = 0; i < file.items.Count; i++)
                cbList.Items.Add(file.items[i].id);

            lstData.Items.Clear();
            keys = file.schema.getKeys();
            foreach (string s in keys)
            {
                var Item = new ListViewItem(new[] { s, "0" });
                lstData.Items.Add(Item);
            }
        }

        private void costumeBottomToolStripMenuItem_Click(object sender, EventArgs e)
        {
            // Load the IDB File
            FileName = datapath + @"/system/item/costume_bottom_item.idb";
            file.Read(FileName);
            file.SetSchema("IDB_Schema.csv");
            cbList.Items.Clear();
            for (int i = 0; i < file.items.Count; i++)
                cbList.Items.Add(file.items[i].id);

            lstData.Items.Clear();
            keys = file.schema.getKeys();
            foreach (string s in keys)
            {
                var Item = new ListViewItem(new[] { s, "0" });
                lstData.Items.Add(Item);
            }
        }

        private void costumeShoesToolStripMenuItem_Click(object sender, EventArgs e)
        {
            // Load the IDB File
            FileName = datapath + @"/system/item/costume_shoes_item.idb";
            file.Read(FileName);
            file.SetSchema("IDB_Schema.csv");
            cbList.Items.Clear();
            for (int i = 0; i < file.items.Count; i++)
                cbList.Items.Add(file.items[i].id);

            lstData.Items.Clear();
            keys = file.schema.getKeys();
            foreach (string s in keys)
            {
                var Item = new ListViewItem(new[] { s, "0" });
                lstData.Items.Add(Item);
            }
        }

        private void extraToolStripMenuItem_Click(object sender, EventArgs e)
        {
            // Load the IDB File
            FileName = datapath + @"/system/item/extra_item.idb";
            file.Read(FileName);
            file.SetSchema("IDB_Schema.csv");
            cbList.Items.Clear();
            for (int i = 0; i < file.items.Count; i++)
                cbList.Items.Add(file.items[i].id);

            lstData.Items.Clear();
            keys = file.schema.getKeys();
            foreach (string s in keys)
            {
                var Item = new ListViewItem(new[] { s, "0" });
                lstData.Items.Add(Item);
            }
        }

        private void materialToolStripMenuItem_Click(object sender, EventArgs e)
        {
            // Load the IDB File
            FileName = datapath + @"/system/item/material_item.idb";
            file.Read(FileName);
            file.SetSchema("IDB_Schema.csv");
            cbList.Items.Clear();
            for (int i = 0; i < file.items.Count; i++)
                cbList.Items.Add(file.items[i].id);

            lstData.Items.Clear();
            keys = file.schema.getKeys();
            foreach (string s in keys)
            {
                var Item = new ListViewItem(new[] { s, "0" });
                lstData.Items.Add(Item);
            }
        }

        private void skillToolStripMenuItem_Click(object sender, EventArgs e)
        {
            // Load the IDB File
            FileName = datapath + @"/system/item/skill_item.idb";
            file.Read(FileName);
            file.SetSchema("IDB_Schema.csv");
            cbList.Items.Clear();
            for (int i = 0; i < file.items.Count; i++)
                cbList.Items.Add(file.items[i].id);

            lstData.Items.Clear();
            keys = file.schema.getKeys();
            foreach (string s in keys)
            {
                var Item = new ListViewItem(new[] { s, "0" });
                lstData.Items.Add(Item);
            }
        }

        private void toolStripMenuItem14_Click(object sender, EventArgs e)
        {
            this.file.items.RemoveAt(this.cbList.SelectedIndex);
            this.cbList.SelectedIndex = 0;
            this.cbList.Items.Clear();
            for (int index = 0; index < this.file.items.Count; ++index)
                this.cbList.Items.Add((object)this.file.items[index].id);
        }

        private void toolStripMenuItem13_Click(object sender, EventArgs e)
        {
            IDB_Data idbData = new IDB_Data();
            idbData.id = this.file.items[this.file.items.Count - 1].id;
            ++idbData.id;
            idbData.E_set1 = new byte[224];
            idbData.E_set2 = new byte[224];
            idbData.E_set3 = new byte[224];
            this.file.items.Add(idbData);
            this.cbList.Items.Clear();
            for (int index = 0; index < this.file.items.Count; ++index)
                this.cbList.Items.Add((object)this.file.items[index].id);
        }

        //////////////////////////////////////////////////////////////////
        private void exitToolStripMenuItem1_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        private void lvMods_DragDrop(object sender, DragEventArgs e)
        {
            string[] files = (string[])e.Data.GetData(DataFormats.FileDrop);
            foreach(string file in files)
            {
                if (file.EndsWith(".x2m"))
                {
                    if (MessageBox.Show("Do you want to install mod \"" + Path.GetFileName(file) + "\"?", "Mod Installation", MessageBoxButtons.YesNoCancel, MessageBoxIcon.Information) == DialogResult.Yes)
                        InstallModx2m(file);
                }
            }

        }

        private void lvMods_DragEnter(object sender, DragEventArgs e)
        {
            e.Effect = DragDropEffects.All;
        }

    }
}
