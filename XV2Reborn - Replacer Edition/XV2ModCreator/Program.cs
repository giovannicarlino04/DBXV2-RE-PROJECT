﻿using System.IO.Compression;
using System.Xml;

namespace XV2ModCreator
{
    class XV2ModCreator {
        static void buildXVModFile(string modfolder)
        {
            string modfile = modfolder + @".xv2mod";

            Console.WriteLine("Insert mod name for " + modfolder );
            string modname = Console.ReadLine();
            Console.WriteLine("Insert mod author for " + modfolder);
            string modauthor = Console.ReadLine();

            if (modfolder.Length > 0 && Directory.Exists(modfolder))
            {
                // Specify the path where you want to save the XML file
                string xmlFilePath = modfolder + "/xv2mod.xml";

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

                    WriteElementWithValue(writer, "MOD_NAME", modname);
                    WriteElementWithValue(writer, "MOD_AUTHOR", modauthor);

                    writer.WriteEndElement(); // Close XV2MOD
                    writer.WriteEndDocument(); // Close the document
                }

                Console.WriteLine("XML file created at: " + xmlFilePath);

                ZipFile.CreateFromDirectory(modfolder, modfile);

                if (File.Exists(xmlFilePath))
                    File.Delete(xmlFilePath);

                Console.WriteLine("Mod Created Successfully!");
            }

            // Helper method to write an element with a value
            static void WriteElementWithValue(XmlWriter writer, string elementName, string value)
            {
                writer.WriteStartElement(elementName);
                writer.WriteAttributeString("value", value);
                writer.WriteEndElement();
            }
        }
        static void Main(string[] args)
        {
            foreach(string arg in args)
            {
                buildXVModFile(arg);
            }
        }

}
}