#include "BcmFile.h"
#include "debug.h"

#define COPY_IN(n) entry.n = file_entries[i].n
#define COPY_OUT(n) file_entries[i].n = entry.n

TiXmlElement *BcmEntry::Decompile(TiXmlNode *root, uint32_t idx) const
{
    TiXmlElement *entry_root = new TiXmlElement("BcmEntry");

    entry_root->SetAttribute("idx", idx);

    if (ptr_30 != 0)
    {
        entry_root->SetAttribute("ptr_30", ((ptr_30-sizeof(BCMHeader)) / sizeof(BCMEntry)));
    }

    if (ptr_34 != 0)
    {
        entry_root->SetAttribute("ptr_34", ((ptr_34-sizeof(BCMHeader)) / sizeof(BCMEntry)));
    }

    if (ptr_3C != 0)
    {
        entry_root->SetAttribute("ptr_3C", ((ptr_3C-sizeof(BCMHeader)) / sizeof(BCMEntry)));
    }

    Utils::WriteParamUnsigned(entry_root, "U_00", unk_00, true);
    Utils::WriteParamUnsigned(entry_root, "U_04", unk_04, true);
    Utils::WriteParamUnsigned(entry_root, "U_08", unk_08, true);
    Utils::WriteParamUnsigned(entry_root, "U_0A", unk_0A, true);
    Utils::WriteParamUnsigned(entry_root, "U_0C", unk_0C, true);
    Utils::WriteParamUnsigned(entry_root, "U_10", unk_10, true);
    Utils::WriteParamUnsigned(entry_root, "U_14", unk_14, true);
    Utils::WriteParamUnsigned(entry_root, "U_18", unk_18, true);
    Utils::WriteParamUnsigned(entry_root, "U_1C", unk_1C, true);
    Utils::WriteParamUnsigned(entry_root, "BAC_INDEX", bac_index, true);
    Utils::WriteParamUnsigned(entry_root, "U_22", unk_22, true);
    Utils::WriteParamUnsigned(entry_root, "U_24", unk_24, true);
    Utils::WriteParamUnsigned(entry_root, "U_26", unk_26, true);
    Utils::WriteParamUnsigned(entry_root, "U_28", unk_28, true);
    Utils::WriteParamUnsigned(entry_root, "BAC_INDEX2", bac_index2, true);
    Utils::WriteParamUnsigned(entry_root, "U_2C", unk_2C, true);
    Utils::WriteParamUnsigned(entry_root, "U_2E", unk_2E, true);

    Utils::WriteParamUnsigned(entry_root, "U_38", unk_38, true);
    Utils::WriteParamUnsigned(entry_root, "KI_COST", ki_cost);
    Utils::WriteParamUnsigned(entry_root, "U_44", unk_44, true);
    Utils::WriteParamUnsigned(entry_root, "U_48", unk_48, true);
    Utils::WriteParamUnsigned(entry_root, "U_4C", unk_4C, true);
    Utils::WriteParamUnsigned(entry_root, "U_50", unk_50, true);
    Utils::WriteParamUnsigned(entry_root, "STAMINA_COST", stamina_cost);
    Utils::WriteParamUnsigned(entry_root, "U_58", unk_58, true);
    Utils::WriteParamUnsigned(entry_root, "KI_REQUIRED", ki_required);
    Utils::WriteParamUnsigned(entry_root, "HEALTH_REQUIRED", health_required);
    Utils::WriteParamUnsigned(entry_root, "TRANS_MODIFIER", trans_modifier);
    Utils::WriteParamUnsigned(entry_root, "U_66", unk_66, true);
    Utils::WriteParamUnsigned(entry_root, "U_68", unk_68, true);
    Utils::WriteParamUnsigned(entry_root, "U_6C", unk_6C, true);

    root->LinkEndChild(entry_root);
    return entry_root;
}

bool BcmEntry::Compile(const TiXmlElement *root)
{
    if (Utils::ReadAttrUnsigned(root, "ptr_30", &ptr_30))
    {
        ptr_30 = sizeof(BCMHeader) + ptr_30 * sizeof(BCMEntry);
    }
    else
    {
        ptr_30 = 0;
    }

    if (Utils::ReadAttrUnsigned(root, "ptr_34", &ptr_34))
    {
        ptr_34 = sizeof(BCMHeader) + ptr_34 * sizeof(BCMEntry);
    }
    else
    {
        ptr_34 = 0;
    }

    if (Utils::ReadAttrUnsigned(root, "ptr_3C", &ptr_3C))
    {
        ptr_3C = sizeof(BCMHeader) + ptr_3C * sizeof(BCMEntry);
    }
    else
    {
        ptr_3C = 0;
    }

    if (!Utils::GetParamUnsigned(root, "U_00", &unk_00)) return false;
    if (!Utils::GetParamUnsigned(root, "U_04", &unk_04)) return false;
    if (!Utils::GetParamUnsigned(root, "U_08", &unk_08)) return false;
    if (!Utils::GetParamUnsigned(root, "U_0A", &unk_0A)) return false;
    if (!Utils::GetParamUnsigned(root, "U_0C", &unk_0C)) return false;
    if (!Utils::GetParamUnsigned(root, "U_10", &unk_10)) return false;
    if (!Utils::GetParamUnsigned(root, "U_14", &unk_14)) return false;
    if (!Utils::GetParamUnsigned(root, "U_18", &unk_18)) return false;
    if (!Utils::GetParamUnsigned(root, "U_1C", &unk_1C)) return false;
    if (!Utils::GetParamUnsigned(root, "BAC_INDEX", &bac_index)) return false;
    if (!Utils::GetParamUnsigned(root, "U_22", &unk_22)) return false;
    if (!Utils::GetParamUnsigned(root, "U_24", &unk_24)) return false;
    if (!Utils::GetParamUnsigned(root, "U_26", &unk_26)) return false;
    if (!Utils::GetParamUnsigned(root, "U_28", &unk_28)) return false;
    if (!Utils::GetParamUnsigned(root, "BAC_INDEX2", &bac_index2)) return false;
    if (!Utils::GetParamUnsigned(root, "U_2C", &unk_2C)) return false;
    if (!Utils::GetParamUnsigned(root, "U_2E", &unk_2E)) return false;
    if (!Utils::GetParamUnsigned(root, "U_38", &unk_38)) return false;
    if (!Utils::GetParamUnsigned(root, "KI_COST", &ki_cost)) return false;
    if (!Utils::GetParamUnsigned(root, "U_44", &unk_44)) return false;
    if (!Utils::GetParamUnsigned(root, "U_48", &unk_48)) return false;
    if (!Utils::GetParamUnsigned(root, "U_4C", &unk_4C)) return false;
    if (!Utils::GetParamUnsigned(root, "U_50", &unk_50)) return false;
    if (!Utils::GetParamUnsigned(root, "STAMINA_COST", &stamina_cost)) return false;
    if (!Utils::GetParamUnsigned(root, "U_58", &unk_58)) return false;
    if (!Utils::GetParamUnsigned(root, "KI_REQUIRED", &ki_required)) return false;
    if (!Utils::GetParamUnsigned(root, "HEALTH_REQUIRED", &health_required)) return false;
    if (!Utils::GetParamUnsigned(root, "TRANS_MODIFIER", &trans_modifier)) return false;
    if (!Utils::GetParamUnsigned(root, "U_66", &unk_66)) return false;
    if (!Utils::GetParamUnsigned(root, "U_68", &unk_68)) return false;
    if (!Utils::GetParamUnsigned(root, "U_6C", &unk_6C)) return false;

    return true;
}

BcmFile::BcmFile()
{
    this->big_endian = false;
}

BcmFile::~BcmFile()
{

}

void BcmFile::Reset()
{
    entries.clear();
}

bool BcmFile::Load(const uint8_t *buf, size_t size)
{
    Reset();

    if (!buf || size < sizeof(BCMHeader))
        return false;

    const BCMHeader *hdr = (const BCMHeader *)buf;
    if (hdr->signature != BCM_SIGNATURE)
        return false;

    const BCMEntry *file_entries = (const BCMEntry *)(buf+hdr->data_start);
    entries.resize(hdr->num_entries);

    for (size_t i = 0; i < entries.size(); i++)
    {
        BcmEntry &entry = entries[i];

        COPY_IN(unk_00);
        COPY_IN(unk_04);
        COPY_IN(unk_08);
        COPY_IN(unk_0A);
        COPY_IN(unk_0C);
        COPY_IN(unk_10);
        COPY_IN(unk_14);
        COPY_IN(unk_18);
        COPY_IN(unk_1C);
        COPY_IN(bac_index);
        COPY_IN(unk_22);
        COPY_IN(unk_24);
        COPY_IN(unk_26);
        COPY_IN(unk_28);
        COPY_IN(bac_index2);
        COPY_IN(unk_2C);
        COPY_IN(unk_2E);
        COPY_IN(ptr_30);
        COPY_IN(ptr_34);
        COPY_IN(unk_38);
        COPY_IN(ptr_3C);
        COPY_IN(ki_cost);
        COPY_IN(unk_44);
        COPY_IN(unk_48);
        COPY_IN(unk_4C);
        COPY_IN(unk_50);
        COPY_IN(stamina_cost);
        COPY_IN(unk_58);
        COPY_IN(ki_required);
        COPY_IN(health_required);
        COPY_IN(trans_modifier);
        COPY_IN(unk_66);
        COPY_IN(unk_68);
        COPY_IN(unk_6C);
    }

    return true;
}

uint8_t *BcmFile::Save(size_t *psize)
{
    size_t size = sizeof(BCMHeader) + entries.size()*sizeof(BCMEntry);
    uint8_t *buf = new uint8_t[size];
    memset(buf, 0, size);

    BCMHeader *hdr = (BCMHeader *)buf;
    hdr->signature = BCM_SIGNATURE;
    hdr->endianess_check = 0xFFFE;
    hdr->num_entries = (uint32_t)entries.size();
    hdr->data_start = sizeof(BCMHeader);

    BCMEntry *file_entries = (BCMEntry *)(hdr+1);

    for (size_t i = 0; i < entries.size(); i++)
    {
        const BcmEntry &entry = entries[i];

        COPY_OUT(unk_00);
        COPY_OUT(unk_04);
        COPY_OUT(unk_08);
        COPY_OUT(unk_0A);
        COPY_OUT(unk_0C);
        COPY_OUT(unk_10);
        COPY_OUT(unk_14);
        COPY_OUT(unk_18);
        COPY_OUT(unk_1C);
        COPY_OUT(bac_index);
        COPY_OUT(unk_22);
        COPY_OUT(unk_24);
        COPY_OUT(unk_26);
        COPY_OUT(unk_28);
        COPY_OUT(bac_index2);
        COPY_OUT(unk_2C);
        COPY_OUT(unk_2E);
        COPY_OUT(ptr_30);
        COPY_OUT(ptr_34);
        COPY_OUT(unk_38);
        COPY_OUT(ptr_3C);
        COPY_OUT(ki_cost);
        COPY_OUT(unk_44);
        COPY_OUT(unk_48);
        COPY_OUT(unk_4C);
        COPY_OUT(unk_50);
        COPY_OUT(stamina_cost);
        COPY_OUT(unk_58);
        COPY_OUT(ki_required);
        COPY_OUT(health_required);
        COPY_OUT(trans_modifier);
        COPY_OUT(unk_66);
        COPY_OUT(unk_68);
        COPY_OUT(unk_6C);
    }

    *psize = size;
    return buf;
}

TiXmlDocument *BcmFile::Decompile() const
{
    TiXmlDocument *doc = new TiXmlDocument();

    TiXmlDeclaration* decl = new TiXmlDeclaration("1.0", "utf-8", "" );
    doc->LinkEndChild(decl);

    TiXmlElement *root = new TiXmlElement("BCM");

    for (size_t i = 0; i < entries.size(); i++)
    {
        entries[i].Decompile(root, (uint32_t)i);
    }

    doc->LinkEndChild(root);
    return doc;
}

bool BcmFile::Compile(TiXmlDocument *doc, bool)
{
    Reset();

    TiXmlHandle handle(doc);
    const TiXmlElement *root = Utils::FindRoot(&handle, "BCM");

    if (!root)
    {
        DPRINTF("Cannot find\"BCM\" in xml.\n");
        return false;
    }

    entries.resize(Utils::GetElemCount(root, "BcmEntry"));
    if (entries.size() == 0)
        return true;

    std::vector<bool> processed;
    processed.resize(entries.size(), false);

    for (const TiXmlElement *elem = root->FirstChildElement(); elem; elem = elem->NextSiblingElement())
    {
        if (elem->ValueStr() == "BcmEntry")
        {
            uint32_t idx;
            if (!Utils::ReadAttrUnsigned(elem, "idx", &idx))
            {
                DPRINTF("%s: Attribute \"idx\" is not optional.\n", FUNCNAME);
                return false;
            }

            if (idx >= entries.size())
            {
                DPRINTF("%s: idx = %d is out of bounds\n", FUNCNAME, idx);
                return false;
            }

            if (processed[idx])
            {
                DPRINTF("%s: idx=%d is duplicated.\n", FUNCNAME, idx);
                return false;
            }

            BcmEntry &entry = entries[idx];

            if (!entry.Compile(elem))
                return false;

            processed[idx] = true;
        }
    }

    return true;
}


