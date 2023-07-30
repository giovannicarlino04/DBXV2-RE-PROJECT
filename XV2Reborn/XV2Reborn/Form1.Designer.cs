namespace XV2Reborn
{
    partial class Form1
    {
        /// <summary>
        /// Variabile di progettazione necessaria.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Pulire le risorse in uso.
        /// </summary>
        /// <param name="disposing">ha valore true se le risorse gestite devono essere eliminate, false in caso contrario.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Codice generato da Progettazione Windows Form

        /// <summary>
        /// Metodo necessario per il supporto della finestra di progettazione. Non modificare
        /// il contenuto del metodo con l'editor di codice.
        /// </summary>
        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Form1));
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.fileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.installModToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
            this.exitToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.uninstallModToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolsToolStripMenuItem1 = new System.Windows.Forms.ToolStripMenuItem();
            this.compileScriptsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator2 = new System.Windows.Forms.ToolStripSeparator();
            this.clearInstallationToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.tabControl1 = new System.Windows.Forms.TabControl();
            this.tabPage1 = new System.Windows.Forms.TabPage();
            this.label1 = new System.Windows.Forms.Label();
            this.lvMods = new System.Windows.Forms.ListView();
            this.columnHeader1 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader2 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader3 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader4 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.tabPage2 = new System.Windows.Forms.TabPage();
            this.tabPage3 = new System.Windows.Forms.TabPage();
            this.tabPage4 = new System.Windows.Forms.TabPage();
            this.tabPage5 = new System.Windows.Forms.TabPage();
            this.tabPage6 = new System.Windows.Forms.TabPage();
            this.groupBox5 = new System.Windows.Forms.GroupBox();
            this.cbList = new System.Windows.Forms.ComboBox();
            this.cbLine = new System.Windows.Forms.ComboBox();
            this.label13 = new System.Windows.Forms.Label();
            this.txtText = new System.Windows.Forms.TextBox();
            this.label14 = new System.Windows.Forms.Label();
            this.txtName = new System.Windows.Forms.TextBox();
            this.label15 = new System.Windows.Forms.Label();
            this.txtID = new System.Windows.Forms.TextBox();
            this.menuStrip3 = new System.Windows.Forms.MenuStrip();
            this.toolStripMenuItem2 = new System.Windows.Forms.ToolStripMenuItem();
            this.msgSaveToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem4 = new System.Windows.Forms.ToolStripMenuItem();
            this.msgAddToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.msgRemoveToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.openToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.charactersToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.skillsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.supersToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.ultimatesToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.evasivesToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.awokenToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.superInfoToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.ultimatesInfoToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.evasivesInfoToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.awokenInfoToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.tabPage7 = new System.Windows.Forms.TabPage();
            this.flowLayoutPanelCharacters = new System.Windows.Forms.FlowLayoutPanel();
            this.menuStrip6 = new System.Windows.Forms.MenuStrip();
            this.fileToolStripMenuItem2 = new System.Windows.Forms.ToolStripMenuItem();
            this.saveCSSToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.menuStrip1.SuspendLayout();
            this.tabControl1.SuspendLayout();
            this.tabPage1.SuspendLayout();
            this.tabPage6.SuspendLayout();
            this.groupBox5.SuspendLayout();
            this.menuStrip3.SuspendLayout();
            this.tabPage7.SuspendLayout();
            this.menuStrip6.SuspendLayout();
            this.SuspendLayout();
            // 
            // menuStrip1
            // 
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileToolStripMenuItem,
            this.toolsToolStripMenuItem,
            this.toolsToolStripMenuItem1});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Size = new System.Drawing.Size(1025, 24);
            this.menuStrip1.TabIndex = 0;
            this.menuStrip1.Text = "menuStrip1";
            // 
            // fileToolStripMenuItem
            // 
            this.fileToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.installModToolStripMenuItem,
            this.toolStripSeparator1,
            this.exitToolStripMenuItem});
            this.fileToolStripMenuItem.Name = "fileToolStripMenuItem";
            this.fileToolStripMenuItem.Size = new System.Drawing.Size(37, 20);
            this.fileToolStripMenuItem.Text = "File";
            // 
            // installModToolStripMenuItem
            // 
            this.installModToolStripMenuItem.Name = "installModToolStripMenuItem";
            this.installModToolStripMenuItem.Size = new System.Drawing.Size(133, 22);
            this.installModToolStripMenuItem.Text = "Install Mod";
            this.installModToolStripMenuItem.Click += new System.EventHandler(this.installModToolStripMenuItem_Click);
            // 
            // toolStripSeparator1
            // 
            this.toolStripSeparator1.Name = "toolStripSeparator1";
            this.toolStripSeparator1.Size = new System.Drawing.Size(130, 6);
            // 
            // exitToolStripMenuItem
            // 
            this.exitToolStripMenuItem.Name = "exitToolStripMenuItem";
            this.exitToolStripMenuItem.Size = new System.Drawing.Size(133, 22);
            this.exitToolStripMenuItem.Text = "Exit";
            // 
            // toolsToolStripMenuItem
            // 
            this.toolsToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.uninstallModToolStripMenuItem});
            this.toolsToolStripMenuItem.Name = "toolsToolStripMenuItem";
            this.toolsToolStripMenuItem.Size = new System.Drawing.Size(39, 20);
            this.toolsToolStripMenuItem.Text = "Edit";
            // 
            // uninstallModToolStripMenuItem
            // 
            this.uninstallModToolStripMenuItem.Name = "uninstallModToolStripMenuItem";
            this.uninstallModToolStripMenuItem.Size = new System.Drawing.Size(148, 22);
            this.uninstallModToolStripMenuItem.Text = "Uninstall Mod";
            // 
            // toolsToolStripMenuItem1
            // 
            this.toolsToolStripMenuItem1.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.compileScriptsToolStripMenuItem,
            this.toolStripSeparator2,
            this.clearInstallationToolStripMenuItem});
            this.toolsToolStripMenuItem1.Name = "toolsToolStripMenuItem1";
            this.toolsToolStripMenuItem1.Size = new System.Drawing.Size(46, 20);
            this.toolsToolStripMenuItem1.Text = "Tools";
            // 
            // compileScriptsToolStripMenuItem
            // 
            this.compileScriptsToolStripMenuItem.Name = "compileScriptsToolStripMenuItem";
            this.compileScriptsToolStripMenuItem.Size = new System.Drawing.Size(162, 22);
            this.compileScriptsToolStripMenuItem.Text = "Compile Scripts";
            this.compileScriptsToolStripMenuItem.Click += new System.EventHandler(this.compileScriptsToolStripMenuItem_Click);
            // 
            // toolStripSeparator2
            // 
            this.toolStripSeparator2.Name = "toolStripSeparator2";
            this.toolStripSeparator2.Size = new System.Drawing.Size(159, 6);
            // 
            // clearInstallationToolStripMenuItem
            // 
            this.clearInstallationToolStripMenuItem.Name = "clearInstallationToolStripMenuItem";
            this.clearInstallationToolStripMenuItem.Size = new System.Drawing.Size(162, 22);
            this.clearInstallationToolStripMenuItem.Text = "Clear Installation";
            this.clearInstallationToolStripMenuItem.Click += new System.EventHandler(this.clearInstallationToolStripMenuItem_Click);
            // 
            // tabControl1
            // 
            this.tabControl1.Controls.Add(this.tabPage1);
            this.tabControl1.Controls.Add(this.tabPage2);
            this.tabControl1.Controls.Add(this.tabPage3);
            this.tabControl1.Controls.Add(this.tabPage4);
            this.tabControl1.Controls.Add(this.tabPage5);
            this.tabControl1.Controls.Add(this.tabPage6);
            this.tabControl1.Controls.Add(this.tabPage7);
            this.tabControl1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tabControl1.Location = new System.Drawing.Point(0, 24);
            this.tabControl1.Name = "tabControl1";
            this.tabControl1.SelectedIndex = 0;
            this.tabControl1.Size = new System.Drawing.Size(1025, 510);
            this.tabControl1.TabIndex = 1;
            // 
            // tabPage1
            // 
            this.tabPage1.BackColor = System.Drawing.SystemColors.Control;
            this.tabPage1.Controls.Add(this.label1);
            this.tabPage1.Controls.Add(this.lvMods);
            this.tabPage1.Location = new System.Drawing.Point(4, 22);
            this.tabPage1.Name = "tabPage1";
            this.tabPage1.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage1.Size = new System.Drawing.Size(1017, 484);
            this.tabPage1.TabIndex = 0;
            this.tabPage1.Text = "Mods";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.label1.Location = new System.Drawing.Point(3, 468);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(87, 13);
            this.label1.TabIndex = 1;
            this.label1.Text = "Installed Mods: 0";
            // 
            // lvMods
            // 
            this.lvMods.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader1,
            this.columnHeader2,
            this.columnHeader3,
            this.columnHeader4});
            this.lvMods.Dock = System.Windows.Forms.DockStyle.Fill;
            this.lvMods.HideSelection = false;
            this.lvMods.Location = new System.Drawing.Point(3, 3);
            this.lvMods.Name = "lvMods";
            this.lvMods.Size = new System.Drawing.Size(1011, 478);
            this.lvMods.TabIndex = 0;
            this.lvMods.UseCompatibleStateImageBehavior = false;
            this.lvMods.View = System.Windows.Forms.View.Details;
            // 
            // columnHeader1
            // 
            this.columnHeader1.Text = "Name";
            this.columnHeader1.Width = 200;
            // 
            // columnHeader2
            // 
            this.columnHeader2.Text = "Author";
            this.columnHeader2.Width = 200;
            // 
            // columnHeader3
            // 
            this.columnHeader3.Text = "Version";
            // 
            // columnHeader4
            // 
            this.columnHeader4.Text = "Type";
            this.columnHeader4.Width = 200;
            // 
            // tabPage2
            // 
            this.tabPage2.BackColor = System.Drawing.SystemColors.Control;
            this.tabPage2.Location = new System.Drawing.Point(4, 22);
            this.tabPage2.Name = "tabPage2";
            this.tabPage2.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage2.Size = new System.Drawing.Size(1017, 484);
            this.tabPage2.TabIndex = 1;
            this.tabPage2.Text = "CMS/CSO";
            // 
            // tabPage3
            // 
            this.tabPage3.BackColor = System.Drawing.SystemColors.Control;
            this.tabPage3.Location = new System.Drawing.Point(4, 22);
            this.tabPage3.Name = "tabPage3";
            this.tabPage3.Size = new System.Drawing.Size(1017, 484);
            this.tabPage3.TabIndex = 2;
            this.tabPage3.Text = "CUS";
            // 
            // tabPage4
            // 
            this.tabPage4.BackColor = System.Drawing.SystemColors.Control;
            this.tabPage4.Location = new System.Drawing.Point(4, 22);
            this.tabPage4.Name = "tabPage4";
            this.tabPage4.Size = new System.Drawing.Size(1017, 484);
            this.tabPage4.TabIndex = 3;
            this.tabPage4.Text = "PSC";
            // 
            // tabPage5
            // 
            this.tabPage5.BackColor = System.Drawing.SystemColors.Control;
            this.tabPage5.Location = new System.Drawing.Point(4, 22);
            this.tabPage5.Name = "tabPage5";
            this.tabPage5.Size = new System.Drawing.Size(1017, 484);
            this.tabPage5.TabIndex = 4;
            this.tabPage5.Text = "AUR";
            // 
            // tabPage6
            // 
            this.tabPage6.BackColor = System.Drawing.SystemColors.Control;
            this.tabPage6.Controls.Add(this.groupBox5);
            this.tabPage6.Controls.Add(this.menuStrip3);
            this.tabPage6.Location = new System.Drawing.Point(4, 22);
            this.tabPage6.Name = "tabPage6";
            this.tabPage6.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage6.Size = new System.Drawing.Size(1017, 484);
            this.tabPage6.TabIndex = 5;
            this.tabPage6.Text = "MSG";
            // 
            // groupBox5
            // 
            this.groupBox5.Controls.Add(this.cbList);
            this.groupBox5.Controls.Add(this.cbLine);
            this.groupBox5.Controls.Add(this.label13);
            this.groupBox5.Controls.Add(this.txtText);
            this.groupBox5.Controls.Add(this.label14);
            this.groupBox5.Controls.Add(this.txtName);
            this.groupBox5.Controls.Add(this.label15);
            this.groupBox5.Controls.Add(this.txtID);
            this.groupBox5.Location = new System.Drawing.Point(8, 90);
            this.groupBox5.Name = "groupBox5";
            this.groupBox5.Size = new System.Drawing.Size(277, 250);
            this.groupBox5.TabIndex = 20;
            this.groupBox5.TabStop = false;
            this.groupBox5.Text = "MSG";
            // 
            // cbList
            // 
            this.cbList.FormattingEnabled = true;
            this.cbList.Location = new System.Drawing.Point(15, 33);
            this.cbList.Name = "cbList";
            this.cbList.Size = new System.Drawing.Size(162, 21);
            this.cbList.TabIndex = 9;
            // 
            // cbLine
            // 
            this.cbLine.FormattingEnabled = true;
            this.cbLine.Location = new System.Drawing.Point(182, 33);
            this.cbLine.Name = "cbLine";
            this.cbLine.Size = new System.Drawing.Size(78, 21);
            this.cbLine.TabIndex = 10;
            // 
            // label13
            // 
            this.label13.AutoSize = true;
            this.label13.Location = new System.Drawing.Point(13, 103);
            this.label13.Name = "label13";
            this.label13.Size = new System.Drawing.Size(28, 13);
            this.label13.TabIndex = 16;
            this.label13.Text = "Text";
            // 
            // txtText
            // 
            this.txtText.Location = new System.Drawing.Point(15, 121);
            this.txtText.Multiline = true;
            this.txtText.Name = "txtText";
            this.txtText.Size = new System.Drawing.Size(243, 115);
            this.txtText.TabIndex = 11;
            // 
            // label14
            // 
            this.label14.AutoSize = true;
            this.label14.Location = new System.Drawing.Point(179, 59);
            this.label14.Name = "label14";
            this.label14.Size = new System.Drawing.Size(18, 13);
            this.label14.TabIndex = 15;
            this.label14.Text = "ID";
            // 
            // txtName
            // 
            this.txtName.Location = new System.Drawing.Point(15, 74);
            this.txtName.Name = "txtName";
            this.txtName.Size = new System.Drawing.Size(162, 20);
            this.txtName.TabIndex = 12;
            // 
            // label15
            // 
            this.label15.AutoSize = true;
            this.label15.Location = new System.Drawing.Point(15, 56);
            this.label15.Name = "label15";
            this.label15.Size = new System.Drawing.Size(35, 13);
            this.label15.TabIndex = 14;
            this.label15.Text = "Name";
            // 
            // txtID
            // 
            this.txtID.Location = new System.Drawing.Point(182, 74);
            this.txtID.Name = "txtID";
            this.txtID.ReadOnly = true;
            this.txtID.Size = new System.Drawing.Size(78, 20);
            this.txtID.TabIndex = 13;
            // 
            // menuStrip3
            // 
            this.menuStrip3.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripMenuItem2,
            this.toolStripMenuItem4,
            this.openToolStripMenuItem});
            this.menuStrip3.Location = new System.Drawing.Point(3, 3);
            this.menuStrip3.Name = "menuStrip3";
            this.menuStrip3.Padding = new System.Windows.Forms.Padding(7, 3, 0, 3);
            this.menuStrip3.Size = new System.Drawing.Size(1011, 25);
            this.menuStrip3.TabIndex = 19;
            this.menuStrip3.Text = "menuStrip3";
            // 
            // toolStripMenuItem2
            // 
            this.toolStripMenuItem2.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.msgSaveToolStripMenuItem});
            this.toolStripMenuItem2.Name = "toolStripMenuItem2";
            this.toolStripMenuItem2.Size = new System.Drawing.Size(37, 19);
            this.toolStripMenuItem2.Text = "File";
            // 
            // msgSaveToolStripMenuItem
            // 
            this.msgSaveToolStripMenuItem.Name = "msgSaveToolStripMenuItem";
            this.msgSaveToolStripMenuItem.Size = new System.Drawing.Size(126, 22);
            this.msgSaveToolStripMenuItem.Text = "Save MSG";
            // 
            // toolStripMenuItem4
            // 
            this.toolStripMenuItem4.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.msgAddToolStripMenuItem,
            this.msgRemoveToolStripMenuItem});
            this.toolStripMenuItem4.Name = "toolStripMenuItem4";
            this.toolStripMenuItem4.Size = new System.Drawing.Size(39, 19);
            this.toolStripMenuItem4.Text = "Edit";
            // 
            // msgAddToolStripMenuItem
            // 
            this.msgAddToolStripMenuItem.Name = "msgAddToolStripMenuItem";
            this.msgAddToolStripMenuItem.Size = new System.Drawing.Size(117, 22);
            this.msgAddToolStripMenuItem.Text = "Add";
            // 
            // msgRemoveToolStripMenuItem
            // 
            this.msgRemoveToolStripMenuItem.Name = "msgRemoveToolStripMenuItem";
            this.msgRemoveToolStripMenuItem.Size = new System.Drawing.Size(117, 22);
            this.msgRemoveToolStripMenuItem.Text = "Remove";
            // 
            // openToolStripMenuItem
            // 
            this.openToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.charactersToolStripMenuItem,
            this.skillsToolStripMenuItem});
            this.openToolStripMenuItem.Name = "openToolStripMenuItem";
            this.openToolStripMenuItem.Size = new System.Drawing.Size(48, 19);
            this.openToolStripMenuItem.Text = "Open";
            // 
            // charactersToolStripMenuItem
            // 
            this.charactersToolStripMenuItem.Name = "charactersToolStripMenuItem";
            this.charactersToolStripMenuItem.Size = new System.Drawing.Size(130, 22);
            this.charactersToolStripMenuItem.Text = "Characters";
            // 
            // skillsToolStripMenuItem
            // 
            this.skillsToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.supersToolStripMenuItem,
            this.ultimatesToolStripMenuItem,
            this.evasivesToolStripMenuItem,
            this.awokenToolStripMenuItem,
            this.superInfoToolStripMenuItem,
            this.ultimatesInfoToolStripMenuItem,
            this.evasivesInfoToolStripMenuItem,
            this.awokenInfoToolStripMenuItem});
            this.skillsToolStripMenuItem.Name = "skillsToolStripMenuItem";
            this.skillsToolStripMenuItem.Size = new System.Drawing.Size(130, 22);
            this.skillsToolStripMenuItem.Text = "Skills";
            // 
            // supersToolStripMenuItem
            // 
            this.supersToolStripMenuItem.Name = "supersToolStripMenuItem";
            this.supersToolStripMenuItem.Size = new System.Drawing.Size(148, 22);
            this.supersToolStripMenuItem.Text = "Supers";
            // 
            // ultimatesToolStripMenuItem
            // 
            this.ultimatesToolStripMenuItem.Name = "ultimatesToolStripMenuItem";
            this.ultimatesToolStripMenuItem.Size = new System.Drawing.Size(148, 22);
            this.ultimatesToolStripMenuItem.Text = "Ultimates";
            // 
            // evasivesToolStripMenuItem
            // 
            this.evasivesToolStripMenuItem.Name = "evasivesToolStripMenuItem";
            this.evasivesToolStripMenuItem.Size = new System.Drawing.Size(148, 22);
            this.evasivesToolStripMenuItem.Text = "Evasives";
            // 
            // awokenToolStripMenuItem
            // 
            this.awokenToolStripMenuItem.Name = "awokenToolStripMenuItem";
            this.awokenToolStripMenuItem.Size = new System.Drawing.Size(148, 22);
            this.awokenToolStripMenuItem.Text = "Awoken";
            // 
            // superInfoToolStripMenuItem
            // 
            this.superInfoToolStripMenuItem.Name = "superInfoToolStripMenuItem";
            this.superInfoToolStripMenuItem.Size = new System.Drawing.Size(148, 22);
            this.superInfoToolStripMenuItem.Text = "Super Info";
            // 
            // ultimatesInfoToolStripMenuItem
            // 
            this.ultimatesInfoToolStripMenuItem.Name = "ultimatesInfoToolStripMenuItem";
            this.ultimatesInfoToolStripMenuItem.Size = new System.Drawing.Size(148, 22);
            this.ultimatesInfoToolStripMenuItem.Text = "Ultimates Info";
            // 
            // evasivesInfoToolStripMenuItem
            // 
            this.evasivesInfoToolStripMenuItem.Name = "evasivesInfoToolStripMenuItem";
            this.evasivesInfoToolStripMenuItem.Size = new System.Drawing.Size(148, 22);
            this.evasivesInfoToolStripMenuItem.Text = "Evasives Info";
            // 
            // awokenInfoToolStripMenuItem
            // 
            this.awokenInfoToolStripMenuItem.Name = "awokenInfoToolStripMenuItem";
            this.awokenInfoToolStripMenuItem.Size = new System.Drawing.Size(148, 22);
            this.awokenInfoToolStripMenuItem.Text = "Awoken Info";
            // 
            // tabPage7
            // 
            this.tabPage7.BackColor = System.Drawing.SystemColors.Control;
            this.tabPage7.Controls.Add(this.flowLayoutPanelCharacters);
            this.tabPage7.Controls.Add(this.menuStrip6);
            this.tabPage7.Location = new System.Drawing.Point(4, 22);
            this.tabPage7.Name = "tabPage7";
            this.tabPage7.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage7.Size = new System.Drawing.Size(1017, 484);
            this.tabPage7.TabIndex = 6;
            this.tabPage7.Text = "CSS";
            // 
            // flowLayoutPanelCharacters
            // 
            this.flowLayoutPanelCharacters.Location = new System.Drawing.Point(1, 183);
            this.flowLayoutPanelCharacters.Name = "flowLayoutPanelCharacters";
            this.flowLayoutPanelCharacters.Size = new System.Drawing.Size(839, 171);
            this.flowLayoutPanelCharacters.TabIndex = 2;
            // 
            // menuStrip6
            // 
            this.menuStrip6.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileToolStripMenuItem2});
            this.menuStrip6.Location = new System.Drawing.Point(3, 3);
            this.menuStrip6.Name = "menuStrip6";
            this.menuStrip6.Size = new System.Drawing.Size(1011, 24);
            this.menuStrip6.TabIndex = 3;
            this.menuStrip6.Text = "menuStrip6";
            // 
            // fileToolStripMenuItem2
            // 
            this.fileToolStripMenuItem2.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.saveCSSToolStripMenuItem});
            this.fileToolStripMenuItem2.Name = "fileToolStripMenuItem2";
            this.fileToolStripMenuItem2.Size = new System.Drawing.Size(37, 20);
            this.fileToolStripMenuItem2.Text = "File";
            // 
            // saveCSSToolStripMenuItem
            // 
            this.saveCSSToolStripMenuItem.Name = "saveCSSToolStripMenuItem";
            this.saveCSSToolStripMenuItem.Size = new System.Drawing.Size(180, 22);
            this.saveCSSToolStripMenuItem.Text = "Save CSS";
            this.saveCSSToolStripMenuItem.Click += new System.EventHandler(this.saveCSSToolStripMenuItem_Click);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1025, 534);
            this.Controls.Add(this.tabControl1);
            this.Controls.Add(this.menuStrip1);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MainMenuStrip = this.menuStrip1;
            this.Name = "Form1";
            this.Text = "XV2Reborn";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            this.tabControl1.ResumeLayout(false);
            this.tabPage1.ResumeLayout(false);
            this.tabPage1.PerformLayout();
            this.tabPage6.ResumeLayout(false);
            this.tabPage6.PerformLayout();
            this.groupBox5.ResumeLayout(false);
            this.groupBox5.PerformLayout();
            this.menuStrip3.ResumeLayout(false);
            this.menuStrip3.PerformLayout();
            this.tabPage7.ResumeLayout(false);
            this.tabPage7.PerformLayout();
            this.menuStrip6.ResumeLayout(false);
            this.menuStrip6.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.ToolStripMenuItem fileToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem installModToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
        private System.Windows.Forms.ToolStripMenuItem exitToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem toolsToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem uninstallModToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem toolsToolStripMenuItem1;
        private System.Windows.Forms.ToolStripMenuItem compileScriptsToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator2;
        private System.Windows.Forms.ToolStripMenuItem clearInstallationToolStripMenuItem;
        private System.Windows.Forms.TabControl tabControl1;
        private System.Windows.Forms.TabPage tabPage1;
        private System.Windows.Forms.ListView lvMods;
        private System.Windows.Forms.ColumnHeader columnHeader1;
        private System.Windows.Forms.ColumnHeader columnHeader2;
        private System.Windows.Forms.ColumnHeader columnHeader3;
        private System.Windows.Forms.ColumnHeader columnHeader4;
        private System.Windows.Forms.TabPage tabPage2;
        private System.Windows.Forms.TabPage tabPage3;
        private System.Windows.Forms.TabPage tabPage4;
        private System.Windows.Forms.TabPage tabPage5;
        private System.Windows.Forms.TabPage tabPage6;
        private System.Windows.Forms.TabPage tabPage7;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.GroupBox groupBox5;
        public System.Windows.Forms.ComboBox cbList;
        private System.Windows.Forms.ComboBox cbLine;
        private System.Windows.Forms.Label label13;
        private System.Windows.Forms.TextBox txtText;
        private System.Windows.Forms.Label label14;
        private System.Windows.Forms.TextBox txtName;
        private System.Windows.Forms.Label label15;
        private System.Windows.Forms.TextBox txtID;
        private System.Windows.Forms.MenuStrip menuStrip3;
        private System.Windows.Forms.ToolStripMenuItem toolStripMenuItem2;
        private System.Windows.Forms.ToolStripMenuItem msgSaveToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem toolStripMenuItem4;
        private System.Windows.Forms.ToolStripMenuItem msgAddToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem msgRemoveToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem openToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem charactersToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem skillsToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem supersToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem ultimatesToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem evasivesToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem awokenToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem superInfoToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem ultimatesInfoToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem evasivesInfoToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem awokenInfoToolStripMenuItem;
        private System.Windows.Forms.FlowLayoutPanel flowLayoutPanelCharacters;
        private System.Windows.Forms.MenuStrip menuStrip6;
        private System.Windows.Forms.ToolStripMenuItem fileToolStripMenuItem2;
        private System.Windows.Forms.ToolStripMenuItem saveCSSToolStripMenuItem;
    }
}

