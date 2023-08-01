#include "X2mCostumeFile.h"
#include "X2mFile.h"
#include "debug.h"

TiXmlElement *X2mCostumeEntry::Decompile(TiXmlNode *root) const
{
    TiXmlElement *entry_root = new TiXmlElement("X2mCostumeEntry");

    Utils::WriteParamGUID(entry_root, "GUID", guid);
    Utils::WriteParamMultipleUnsigned(entry_root, "PARTSETS", partsets);
    Utils::WriteParamMultipleUnsigned(entry_root, "RACES", races);

    if (idb_entries.size() > 0)
        Utils::WriteParamMultipleUnsigned(entry_root, "IDB_ENTRIES", idb_entries, true);

    if (costume_types.size() > 0)
        Utils::WriteParamMultipleUnsigned(entry_root, "COSTUME_TYPES", costume_types);

    root->LinkEndChild(entry_root);
    return entry_root;
}

bool X2mCostumeEntry::Compile(const TiXmlElement *root)
{
    if (!Utils::GetParamGUID(root, "GUID", guid))
        return false;

    if (!Utils::GetParamMultipleUnsigned(root, "PARTSETS", partsets))
        return false;

    if (partsets.size() == 0)
    {
        DPRINTF("%s: There must be at least one partset in PARTSETS.\n", FUNCNAME);
        return false;
    }

    if (!Utils::GetParamMultipleUnsigned(root, "RACES", races))
        return false;

    if (races.size() == 0)
    {
        DPRINTF("%s: There must be at least one race in RACES.\n", FUNCNAME);
        return false;
    }

    for (uint8_t race : races)
    {
        if (race >= X2M_CR_NUM)
        {
            DPRINTF("%s: Invalid race code %d\n", FUNCNAME, race);
            return false;
        }
    }

    if (!Utils::ReadParamMultipleUnsigned(root, "IDB_ENTRIES", idb_entries))
        idb_entries.clear();

    if (!Utils::ReadParamMultipleUnsigned(root, "COSTUME_TYPES", costume_types))
        costume_types.clear();

    if (costume_types.size() != idb_entries.size())
    {
        DPRINTF("%s: Number of costume types must be same than number of idb entries.\n", FUNCNAME);
        return false;
    }

    for (uint8_t type : costume_types)
    {
        if (type >= COSTUME_MAX)
        {
            DPRINTF("%s: Invalid value %d for costume_type\n", FUNCNAME, type);
            return false;
        }
    }

    return true;
}

X2mCostumeFile::X2mCostumeFile()
{

}

X2mCostumeFile::~X2mCostumeFile()
{

}

void X2mCostumeFile::Reset()
{
    costumes.clear();
}

TiXmlDocument *X2mCostumeFile::Decompile() const
{
    TiXmlDocument *doc = new TiXmlDocument();

    TiXmlDeclaration* decl = new TiXmlDeclaration("1.0", "utf-8", "" );
    doc->LinkEndChild(decl);

    TiXmlElement *root = new TiXmlElement("X2mCostumeFile");
    Utils::WriteComment(root, "This file is used by XV2 Mods Installer to track installed costumes. DON'T DELETE THIS FILE OR Xv2 Mods Installer won't be able to update/uninstall costumes properly.");

    for (const X2mCostumeEntry &costume : costumes)
    {
        costume.Decompile(root);
    }

    doc->LinkEndChild(root);
    return doc;
}

bool X2mCostumeFile::Compile(TiXmlDocument *doc, bool)
{
    Reset();

    TiXmlHandle handle(doc);
    const TiXmlElement *root = Utils::FindRoot(&handle, "X2mCostumeFile");

    if (!root)
    {
        DPRINTF("Cannot find\"X2mCostumeFile\" in xml.\n");
        return false;
    }

    for (const TiXmlElement *elem = root->FirstChildElement(); elem; elem = elem->NextSiblingElement())
    {
        const std::string &name = elem->ValueStr();

        if (name == "X2mCostumeEntry")
        {
            X2mCostumeEntry costume;

            if (!costume.Compile(elem))
            {
                costumes.clear();
                return false;
            }

            costumes.push_back(costume);
        }
    }

    return true;
}

X2mCostumeEntry *X2mCostumeFile::FindCostume(const uint8_t *guid)
{
    for (X2mCostumeEntry &costume : costumes)
    {
        if (memcmp(costume.guid, guid, sizeof(costume.guid)) == 0)
            return &costume;
    }

    return nullptr;
}

X2mCostumeEntry *X2mCostumeFile::FindCostumeByPartSet(uint16_t partset)
{
    for (X2mCostumeEntry &costume : costumes)
    {
        for (uint16_t ps : costume.partsets)
        {
            if (ps == partset)
                return &costume;
        }
    }

    return nullptr;
}

void X2mCostumeFile::AddCostume(const X2mCostumeEntry &entry)
{
    X2mCostumeEntry *existing = FindCostume(entry.guid);

    if (existing)
    {
        *existing = entry;
    }
    else
    {
        costumes.push_back(entry);
    }
}

void X2mCostumeFile::RemoveCostume(const uint8_t *guid)
{
    for (size_t i = 0; i < costumes.size(); i++)
    {
        X2mCostumeEntry &costume = costumes[i];

        if (memcmp(costume.guid, guid, sizeof(costume.guid)) == 0)
        {
            costumes.erase(costumes.begin()+i);
            i--;
        }
    }
}
