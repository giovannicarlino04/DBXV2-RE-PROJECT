using System.Collections.Generic;
using System.IO;

namespace XV2Reborn
{
    public class IDB
    {
        public SchemaBinary schema;
        public List<IDB_Data> items = new List<IDB_Data>();

        public void SetSchema(string schemaPath)
        {
            this.schema = new SchemaBinary();
            this.schema.ReadSchema(schemaPath);
        }

        public void Read(string FileName)
        {
            using (BinaryReader binaryReader = new BinaryReader((Stream)File.Open(FileName, FileMode.Open)))
            {
                binaryReader.BaseStream.Seek(8L, SeekOrigin.Begin);
                int length = binaryReader.ReadInt32();
                binaryReader.BaseStream.Seek(16L, SeekOrigin.Begin);
                IDB_Data[] collection = new IDB_Data[length];
                for (int index = 0; index < length; ++index)
                {
                    binaryReader.BaseStream.Seek((long)(16 + 720 * index), SeekOrigin.Begin);
                    collection[index].id = binaryReader.ReadInt16();
                    collection[index].star = binaryReader.ReadInt16();
                    collection[index].name = binaryReader.ReadInt16();
                    collection[index].desc = binaryReader.ReadInt16();
                    collection[index].type = binaryReader.ReadInt16();
                    collection[index].unk1 = binaryReader.ReadInt16();
                    collection[index].unk2 = binaryReader.ReadInt16();
                    collection[index].unk3 = binaryReader.ReadInt16();
                    collection[index].buy = binaryReader.ReadInt32();
                    collection[index].sell = binaryReader.ReadInt32();
                    collection[index].racelock = binaryReader.ReadInt32();
                    collection[index].tp = binaryReader.ReadInt32();
                    collection[index].extra = binaryReader.ReadInt32();
                    binaryReader.BaseStream.Seek(12L, SeekOrigin.Current);
                    collection[index].E_set1 = binaryReader.ReadBytes(224);
                    collection[index].E_set2 = binaryReader.ReadBytes(224);
                    collection[index].E_set3 = binaryReader.ReadBytes(224);
                }
                this.items.AddRange((IEnumerable<IDB_Data>)collection);
            }
        }

        public void Write(string FileName)
        {
            using (BinaryWriter binaryWriter = new BinaryWriter((Stream)File.Open(FileName, FileMode.Create)))
            {
                binaryWriter.Write(new byte[8]
                {
          (byte) 35,
          (byte) 73,
          (byte) 68,
          (byte) 66,
          (byte) 254,
          byte.MaxValue,
          (byte) 7,
          (byte) 0
                });
                binaryWriter.Write(this.items.Count);
                binaryWriter.Write(16);
                for (int index = 0; index < this.items.Count; ++index)
                {
                    binaryWriter.Write(this.items[index].id);
                    binaryWriter.Write(this.items[index].star);
                    binaryWriter.Write(this.items[index].name);
                    binaryWriter.Write(this.items[index].desc);
                    binaryWriter.Write(this.items[index].type);
                    binaryWriter.Write(this.items[index].unk1);
                    binaryWriter.Write(this.items[index].unk2);
                    binaryWriter.Write(this.items[index].unk3);
                    binaryWriter.Write(this.items[index].buy);
                    binaryWriter.Write(this.items[index].sell);
                    binaryWriter.Write(this.items[index].racelock);
                    binaryWriter.Write(this.items[index].tp);
                    binaryWriter.Write(this.items[index].extra);
                    binaryWriter.BaseStream.Seek(12L, SeekOrigin.Current);
                    binaryWriter.Write(this.items[index].E_set1);
                    binaryWriter.Write(this.items[index].E_set2);
                    binaryWriter.Write(this.items[index].E_set3);
                }
            }
        }

        public void Sort()
        {
        }
    }
}
