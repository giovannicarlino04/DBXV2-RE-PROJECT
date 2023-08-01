#include <algorithm>
#include "X2mFile.h"
#include "IniFile.h"
#include "debug.h"

static const std::vector<std::string> aura_types =
{
    "BoostStart",
    "BoostLoop",
    "BoostEnd",
    "KiaiCharge",
    "KiryokuMax",
    "HenshinStart",
    "HenshinEnd"
};

const std::vector<std::string> x2m_cr_code =
{
    "HUM",
    "HUF",
    "NMC",
    "FRI",
    "MAM",
    "MAF"
};

void X2mSlotEntry::CopyFrom(const CharaListSlotEntry &entry, bool name)
{
    costume_index = entry.costume_index;
    model_preset = entry.model_preset;
    flag_gk2 = entry.flag_gk2;
    voices_id_list[0] = entry.voices_id_list[0];
    voices_id_list[1] = entry.voices_id_list[1];
    audio_files[0].clear();
    audio_files[1].clear();

    if (name)
    {
        for (int i = 0; i < XV2_LANG_NUM; i++)
        {
            if (entry.code.length() != 5 || entry.code[0] != '"' || entry.code[4] != '"')
            {
                costume_name[i].clear();
            }
            else
            {
                std::string code = entry.code.substr(1, 3);

                if (!Xenoverse2::GetCharaCostumeName(code, costume_index, model_preset, costume_name[i], i))
                    costume_name[i].clear();
            }
        }
    }
}

void X2mSlotEntry::CopyTo(CharaListSlotEntry &entry, const std::string &code) const
{
    if (voices_id_list[0] == (int)X2M_DUMMY_ID || voices_id_list[1] == (int)X2M_DUMMY_ID)
    {
        DPRINTF("%s: PROGRAM BUG, audio should have been assigned before calling this function.\n", FUNCNAME);
        exit(-1);
        return;
    }

    entry.with_undefined = false;
    entry.code = std::string("\"") + code + std::string("\"");
    entry.costume_index = costume_index;
    entry.model_preset = model_preset;
    entry.unlock_index = 0;
    entry.flag_gk2 = flag_gk2;
    entry.voices_id_list[0] = voices_id_list[0];
    entry.voices_id_list[1] = voices_id_list[1];
    entry.dlc = "Dlc_Def";
}

TiXmlElement *X2mSlotEntry::Decompile(TiXmlNode *root, bool new_format) const
{
    TiXmlElement *entry_root = new TiXmlElement("SlotEntry");

    entry_root->SetAttribute("costume_index", costume_index);
    Utils::WriteParamUnsigned(entry_root, "MODEL_PRESET", (int64_t)model_preset, true);
    Utils::WriteParamString(entry_root, "FLAG_GK2", (flag_gk2) ? "true" : "false");

    std::vector<uint32_t> voices = { (uint32_t)voices_id_list[0], (uint32_t)voices_id_list[1] };
    Utils::WriteParamMultipleUnsigned(entry_root, "VOICES_ID_LIST", voices, true);

    for (size_t i = 0; i < XV2_LANG_NUM; i++)
    {
        if (costume_name[i].length() == 0)
        {
            if (i == XV2_LANG_ENGLISH)
                return nullptr;
        }
        else
        {
            std::string name = "COSTUME_NAME_" + Utils::ToUpperCase(xv2_lang_codes[i]);
            Utils::WriteParamString(entry_root, name.c_str(), costume_name[i]);
        }
    }

    if (new_format)
    {
        if (audio_files[0].length() > 0)
            Utils::WriteParamString(entry_root, "AUDIO0", audio_files[0]);

        if (audio_files[1].length() > 0)
            Utils::WriteParamString(entry_root, "AUDIO1", audio_files[1]);
    }

    root->LinkEndChild(entry_root);
    return entry_root;
}

bool X2mSlotEntry::Compile(const TiXmlElement *root, bool new_format)
{
    if (!Utils::ReadAttrUnsigned(root, "costume_index", (uint32_t *)&costume_index))
    {
        DPRINTF("%s: costume_index attribute is not optional.\n", FUNCNAME);
        return false;
    }

    if (!Utils::GetParamUnsigned(root, "MODEL_PRESET", (uint32_t *)&model_preset))
        return false;

    std::string flag;

    if (!Utils::GetParamString(root, "FLAG_GK2", flag))
        return false;

    flag = Utils::ToLowerCase(flag);
    if (flag == "true" || flag == "1")
    {
        flag_gk2 = true;
    }
    else if (flag == "false" || flag == "0")
    {
        flag_gk2 = false;
    }
    else
    {
        DPRINTF("%s: Cannot parse FLAG_GK2 param (%s).\n", FUNCNAME, flag.c_str());
        return false;
    }

    if (!Utils::GetParamMultipleUnsigned(root, "VOICES_ID_LIST", (uint32_t *)voices_id_list, 2))
        return false;

    for (size_t i = 0; i < XV2_LANG_NUM; i++)
    {
        std::string name = "COSTUME_NAME_" + Utils::ToUpperCase(xv2_lang_codes[i]);

        if (i == XV2_LANG_ENGLISH)
        {
            if (!Utils::GetParamString(root, name.c_str(), costume_name[i]))
                return false;
        }
        else
        {
            Utils::ReadParamString(root, name.c_str(), costume_name[i]);
        }
    }

    audio_files[0].clear();
    audio_files[1].clear();

    if (new_format)
    {
        if (Utils::ReadParamString(root, "AUDIO0", audio_files[0]))
            voices_id_list[0] = X2M_DUMMY_ID;

        if (Utils::ReadParamString(root, "AUDIO1", audio_files[1]))
            voices_id_list[1] = X2M_DUMMY_ID;
    }

    return true;
}

TiXmlElement *SevEntryHL::Decompile(TiXmlNode *root) const
{
    TiXmlElement *entry_root = new TiXmlElement("SevEntryHL");

    entry_root->SetAttribute("costume_id", Utils::UnsignedToString(costume_id, true));
    entry_root->SetAttribute("copy_char", copy_char);
    entry_root->SetAttribute("copy_costume", Utils::UnsignedToString(copy_costume, true));

    root->LinkEndChild(entry_root);
    return entry_root;
}

bool SevEntryHL::Compile(const TiXmlElement *root)
{
    if (!Utils::ReadAttrString(root, "copy_char", copy_char))
    {
        DPRINTF("%s: copy_char attribute is not optional.\n", FUNCNAME);
        return false;
    }

    if (!Utils::ReadAttrUnsigned(root, "costume_id", &costume_id))
        costume_id = 0xFFFFFFFF;

    if (!Utils::ReadAttrUnsigned(root, "copy_costume", &copy_costume))
        copy_costume = 0xFFFFFFFF;

    return true;
}

TiXmlElement *SevEventsLL::Decompile(TiXmlNode *root) const
{
    TiXmlElement *entry_root = new TiXmlElement("SubEntry");

    entry_root->SetAttribute("im_source", (im_source) ? "true" : "false");
    entry_root->SetAttribute("char_code", char_code);
    entry_root->SetAttribute("costume_id", Utils::UnsignedToString(costume_id, true));

    for (const SevEvent &event : events)
        event.Decompile(entry_root);

    root->LinkEndChild(entry_root);
    return entry_root;
}

bool SevEventsLL::Compile(const TiXmlElement *root)
{
    std::string temp;

    if (!Utils::ReadAttrString(root, "im_source", temp))
    {
        DPRINTF("%s: im_source is not optional.\n", FUNCNAME);
        return false;
    }

    temp = Utils::ToLowerCase(temp);
    im_source = (temp == "true" || temp == "1");

    if (!Utils::ReadAttrString(root, "char_code", char_code))
    {
        if (!im_source)
        {
            DPRINTF("%s: char_code is not optional when im_source=false.\n", FUNCNAME);
            return false;
        }
    }

    if (!im_source && char_code.length() != 3)
    {
        DPRINTF("%s: char_code must be a 3-letter code when im_source=false.\n", FUNCNAME);
        return false;
    }

    if (!Utils::ReadAttrUnsigned(root, "costume_id", &costume_id))
    {
        DPRINTF("%s: costume_id is not optional.\n", FUNCNAME);
        return false;
    }

    events.clear();

    for (const TiXmlElement *elem = root->FirstChildElement(); elem; elem = elem->NextSiblingElement())
    {
        if (elem->ValueStr() == "Event")
        {
           SevEvent event;

           if (!event.Compile(elem))
               return false;

           events.push_back(event);
        }
    }

    return true;
}

TiXmlElement *SevEntryLL::Decompile(TiXmlNode *root) const
{
    TiXmlElement *entry_root = new TiXmlElement("SevEntryLL");

    entry_root->SetAttribute("costume_id", Utils::UnsignedToString(costume_id, true));

    for (const SevEventsLL &events : sub_entries)
        events.Decompile(entry_root);

    root->LinkEndChild(entry_root);
    return entry_root;
}

bool SevEntryLL::Compile(const TiXmlElement *root)
{
    if (!Utils::ReadAttrUnsigned(root, "costume_id", &costume_id))
    {
        DPRINTF("%s: costume_id is not optional\n", FUNCNAME);
        return false;
    }

    for (const TiXmlElement *elem = root->FirstChildElement(); elem; elem = elem->NextSiblingElement())
    {
        if (elem->ValueStr() == "SubEntry")
        {
           SevEventsLL events;

           if (!events.Compile(elem))
               return false;

           sub_entries.push_back(events);
        }       
    }

    return true;
}

TiXmlElement *X2mCharaName::Decompile(TiXmlNode *root) const
{
    TiXmlElement *entry_root = new TiXmlElement("X2mCharaName");
    entry_root->SetAttribute("costume_index", costume_index);

    for (size_t i = 0; i < XV2_LANG_NUM; i++)
    {
        const std::string &name = chara_name[i];

        if (name.length() == 0)
        {
            if (i == XV2_LANG_ENGLISH)
            {
                DPRINTF("%s: Name for language english is not optional.\n", FUNCNAME);
                return nullptr;
            }

            continue;
        }

        const std::string temp_str = "CHARA_NAME_" + Utils::ToUpperCase(xv2_lang_codes[i]);
        Utils::WriteParamString(entry_root, temp_str.c_str(), name);
    }

    root->LinkEndChild(entry_root);
    return entry_root;
}

bool X2mCharaName::Compile(const TiXmlElement *root)
{
    if (!Utils::ReadAttrUnsigned(root, "costume_index", &costume_index))
    {
       DPRINTF("%s: attribute costume_index is not optional.\n", FUNCNAME);
       return false;
    }

    for (size_t i = 0; i < XV2_LANG_NUM; i++)
    {
        chara_name[i].clear();
        const std::string temp_str = "CHARA_NAME_" + Utils::ToUpperCase(xv2_lang_codes[i]);

        Utils::ReadParamString(root, temp_str.c_str(), chara_name[i]);

        if (i == XV2_LANG_ENGLISH && chara_name[i].length() == 0)
        {
            DPRINTF("%s: Name for language english is not optional.\n", FUNCNAME);
            return false;
        }
    }

    return true;
}

TiXmlElement *X2mSkillTransName::Decompile(TiXmlNode *root) const
{
    TiXmlElement *entry_root = new TiXmlElement("X2mSkillTransName");

    for (size_t i = 0; i < XV2_LANG_NUM; i++)
    {
        const std::string &name = trans_name[i];

        if (name.length() == 0)
        {
            if (i == XV2_LANG_ENGLISH)
            {
                DPRINTF("%s: Name for language english is not optional.\n", FUNCNAME);
                return nullptr;
            }

            continue;
        }

        const std::string temp_str = "TRANS_NAME_" + Utils::ToUpperCase(xv2_lang_codes[i]);
        Utils::WriteParamString(entry_root, temp_str.c_str(), name);
    }

    root->LinkEndChild(entry_root);
    return entry_root;
}

bool X2mSkillTransName::Compile(const TiXmlElement *root)
{
    for (size_t i = 0; i < XV2_LANG_NUM; i++)
    {
        trans_name[i].clear();
        const std::string temp_str = "TRANS_NAME_" + Utils::ToUpperCase(xv2_lang_codes[i]);

        Utils::ReadParamString(root, temp_str.c_str(), trans_name[i]);

        if (i == XV2_LANG_ENGLISH && trans_name[i].length() == 0)
        {
            DPRINTF("%s: Name for language english is not optional.\n", FUNCNAME);
            return false;
        }
    }

    return true;
}

TiXmlElement *X2mDepends::Decompile(TiXmlNode *root) const
{
    TiXmlElement *entry_root = new TiXmlElement("X2mDepends");

    entry_root->SetAttribute("id", Utils::UnsignedToString(id, true));
    entry_root->SetAttribute("name", name);

    Utils::WriteParamGUID(entry_root, "GUID", guid);

    if (type == X2mDependsType::SKILL)
    {
        Utils::WriteParamString(entry_root, "TYPE", "SKILL");
    }
    else if (type == X2mDependsType::COSTUME)
    {
        Utils::WriteParamString(entry_root, "TYPE", "COSTUME");
    }
    else
    {
        delete entry_root;
        return nullptr;
    }

    root->LinkEndChild(entry_root);
    return entry_root;
}

bool X2mDepends::Compile(const TiXmlElement *root)
{
    if (!Utils::ReadAttrUnsigned(root, "id", &id))
    {
        DPRINTF("%s: Attribute \"id\" is not optional.\n", FUNCNAME);
        return false;
    }

    if (!Utils::ReadAttrString(root, "name", name))
    {
        DPRINTF("%s: Attribute \"name\" is not optional.\n", FUNCNAME);
        return false;
    }

    if (!Utils::GetParamGUID(root, "GUID", guid))
        return false;

    std::string type_str;

    if (!Utils::GetParamString(root, "TYPE", type_str))
        return false;

    if (type_str == "SKILL")
    {
        type = X2mDependsType::SKILL;
    }
    else if (type_str == "COSTUME")
    {
        type = X2mDependsType::COSTUME;
    }
    else
    {
        DPRINTF("%s: Unrecognized type \"%s\"\n", FUNCNAME, type_str.c_str());
        return false;
    }

    return true;
}

TiXmlElement *X2mSkillAura::Decompile(TiXmlNode *root) const
{
    TiXmlElement *entry_root = new TiXmlElement("X2mSkillAura");

    if (!data.Decompile(entry_root))
        return nullptr;

    if (data.aur_aura_id == X2M_INVALID_ID16)
    {
        if (!aura.Decompile(entry_root, aura_types))
            return nullptr;
    }

    root->LinkEndChild(entry_root);
    return entry_root;
}

bool X2mSkillAura::Compile(const TiXmlElement *root)
{
    const TiXmlElement *elem;
    if (Utils::GetElemCount(root, "CusAuraData", &elem) == 0)
    {
        DPRINTF("%s: CusAuraData is not optional.\n", FUNCNAME);
        return false;
    }

    if (!data.Compile(elem))
        return false;

    aura = AurAura();

    if (data.aur_aura_id == X2M_INVALID_ID16)
    {
        if (Utils::GetElemCount(root, "Aura", &elem) == 0)
        {
            DPRINTF("%s: if data.aur_aur_id is X2M_INVALID_ID, aura is not optional.\n", FUNCNAME);
            return false;
        }

        if (!aura.Compile(elem, aura_types))
            return false;
    }

    return true;
}

TiXmlElement *X2mItem::Decompile(TiXmlNode *root) const
{
    TiXmlElement *entry_root = new TiXmlElement("X2mItem");
    std::string temp_str;

    for (size_t i = 0; i < XV2_LANG_NUM; i++)
    {
        const std::string &name = item_name[i];
        const std::string &desc = item_desc[i];

        if (name.length() == 0)
        {
            if (i == XV2_LANG_ENGLISH)
            {
                DPRINTF("%s: Name for language english is not optional.\n", FUNCNAME);
                return nullptr;
            }

            continue;
        }

        if (desc.length() == 0)
        {
            if (i == XV2_LANG_ENGLISH)
            {
                DPRINTF("%s: Desc for language english is not optional.\n", FUNCNAME);
                return nullptr;
            }

            continue;
        }

        temp_str = "ITEM_NAME_" + Utils::ToUpperCase(xv2_lang_codes[i]);
        Utils::WriteParamString(entry_root, temp_str.c_str(), name);

        temp_str = "ITEM_DESC_" + Utils::ToUpperCase(xv2_lang_codes[i]);
        Utils::WriteParamString(entry_root, temp_str.c_str(), desc);
    }    

    if (item_type == X2mItemType::TOP)
    {
        temp_str = "TOP";
    }
    else if (item_type == X2mItemType::BOTTOM)
    {
        temp_str = "BOTTOM";
    }
    else if (item_type == X2mItemType::GLOVES)
    {
        temp_str = "GLOVES";
    }
    else if (item_type == X2mItemType::SHOES)
    {
        temp_str = "SHOES";
    }
    else if (item_type == X2mItemType::ACCESSORY)
    {
        temp_str = "ACCESSORY";
    }
    else
    {
        DPRINTF("%s: Internal error, costume item type.\n", FUNCNAME);
        return nullptr;
    }

    Utils::WriteParamString(entry_root, "ITEM_TYPE", temp_str);

    if (!idb.Decompile(entry_root))
        return nullptr;

    root->LinkEndChild(entry_root);
    return entry_root;
}

bool X2mItem::Compile(const TiXmlElement *root)
{
    std::string temp_str;

    for (size_t i = 0; i < XV2_LANG_NUM; i++)
    {
        item_name[i].clear();
        temp_str = "ITEM_NAME_" + Utils::ToUpperCase(xv2_lang_codes[i]);

        Utils::ReadParamString(root, temp_str.c_str(), item_name[i]);

        if (i == XV2_LANG_ENGLISH && item_name[i].length() == 0)
        {
            DPRINTF("%s: Name for language english is not optional.\n", FUNCNAME);
            return false;
        }

        item_desc[i].clear();
        temp_str = "ITEM_DESC_" + Utils::ToUpperCase(xv2_lang_codes[i]);

        Utils::ReadParamString(root, temp_str.c_str(), item_desc[i]);

        if (i == XV2_LANG_ENGLISH && item_desc[i].length() == 0)
        {
            DPRINTF("%s: Desc for language english is not optional.\n", FUNCNAME);
            return false;
        }
    }    

    if (!Utils::GetParamString(root, "ITEM_TYPE", temp_str))
        return false;

    if (temp_str == "TOP")
    {
        item_type = X2mItemType::TOP;
    }
    else if (temp_str == "BOTTOM")
    {
        item_type = X2mItemType::BOTTOM;
    }
    else if (temp_str == "GLOVES")
    {
        item_type = X2mItemType::GLOVES;
    }
    else if (temp_str == "SHOES")
    {
        item_type = X2mItemType::SHOES;
    }
    else if (temp_str == "ACCESSORY")
    {
        item_type = X2mItemType::ACCESSORY;
    }
    else
    {
        DPRINTF("%s: Unrecognized item type for COSTUME_ITEM_TYPE\n", FUNCNAME);
        return false;
    }

    const TiXmlElement *elem;
    if (Utils::GetElemCount(root, "IdbEntry", &elem) == 0)
    {
        DPRINTF("%s: Not IdbEntry inside X2mItem.\n", FUNCNAME);
        return false;
    }

    if (!idb.Compile(elem))
        return false;

    idb.id = idb.name_id = idb.desc_id = X2M_DUMMY_ID16;
    return true;
}

TiXmlElement *X2mPartSet::Decompile(TiXmlNode *root) const
{
    TiXmlElement *entry_root = new TiXmlElement("X2mPartSet");

    Utils::WriteParamUnsigned(entry_root, "RACE", race);

    if (!bcs.Decompile(entry_root, -1))
        return nullptr;

    root->LinkEndChild(entry_root);
    return entry_root;
}

bool X2mPartSet::Compile(const TiXmlElement *root)
{
    if (!Utils::GetParamUnsigned(root, "RACE", &race))
        return false;

    if (race >= X2M_CR_NUM)
    {
        DPRINTF("%s: race value %d out of bounds.\n", FUNCNAME, race);
        return false;
    }

    const TiXmlElement *bcs_root;
    if (Utils::GetElemCount(root, "PartSet", &bcs_root) == 0)
    {
        DPRINTF("%s: Was exepecting a partset here.\n", FUNCNAME);
        return false;
    }

    if (!bcs.Compile(bcs_root))
        return false;

    return true;
}

X2mFile::X2mFile()
{
    Reset();
}

X2mFile::~X2mFile()
{

}

void X2mFile::Reset()
{
    ZipFile::DeleteAll();

    type = X2mType::REPLACER;
    format_version = X2M_CURRENT_VERSION;

    mod_name.clear();
    mod_author.clear();
    mod_version = 1.0f;
    Utils::GetRandomData(mod_guid, sizeof(mod_guid));
    Utils::GetRandomData(udata, sizeof(udata));

    entry_name.clear();
    chara_name.clear();
    chara_name.resize(XV2_LANG_NUM);
    mult_chara_names.clear();
    is_ozaru = false;
    body_shape = -1;

    slot_entries.clear();
    cms_entry.id = X2M_INVALID_ID;    
    skill_sets.clear();
    cso_entries.clear();
    psc_entries.clear();
    aur_entries.clear();
    sev_hl_entries.clear();
    sev_ll_entries.clear();
    cml_entries.clear();
    hci_entries.clear();

    chara_skill_depends.clear();
    chara_auras.clear();

    skill_name.clear();
    skill_name.resize(XV2_LANG_NUM);
    skill_desc.clear();
    skill_desc.resize(XV2_LANG_NUM);
    skill_type = X2mSkillType::SUPER;
    skill_trans_names.clear();
    skill_entry.id = X2M_INVALID_ID16;
    skill_idb_entry.id = X2M_INVALID_ID16;
    skill_pup_entries.clear();
    skill_aura_entries.clear();
    skill_costume_depend.id = X2M_INVALID_ID;

    costume_items.clear();
    costume_partsets.clear();     

    // Temp variables
    temp_pup_in.clear();
    temp_pup_out.clear();
    temp_cus_aur_in.clear();
    temp_cus_aur_out.clear();
    temp_aur_in.clear();
    temp_aur_out.clear();
    update_costume_entry = X2mCostumeEntry();
}

bool X2mFile::Validate()
{
    if (format_version > X2M_CURRENT_VERSION)
    {
        DPRINTF("This x2m requires a newer version of the installer/creator.\n");
        return false;
    }

    if (mod_name.length() == 0)
    {
        DPRINTF("%s: MOD_NAME cannot be empty.\n", FUNCNAME);
        return false;
    }

    if (mod_author.length() == 0)
    {
        DPRINTF("%s: MOD_AUTHOR cannot be empty.\n", FUNCNAME);
        return false;
    }

    if (type == X2mType::NEW_CHARACTER)
    {
        if (entry_name.length() != 3)
        {
            DPRINTF("%s: ENTRY_NAME must be 3 characters long!\n", FUNCNAME);
            return false;
        }

        if (Xenoverse2::IsOriginalChara(entry_name))
        {
            DPRINTF("%s: ENTRY_NAME cannot be an original character for the game.\n", FUNCNAME);
            return false;
        }

        if (!DirExists(entry_name))
        {
            DPRINTF("%s: Directory \"%s\" must exist in the package.\n", FUNCNAME, entry_name.c_str());
            return false;
        }

        if (chara_name[XV2_LANG_ENGLISH].length() == 0)
        {
            DPRINTF("%s: CHARA_NAME_EN cannot be empty.\n", FUNCNAME);
            return false;
        }

        if (!SelPortraitExists())
        {
            DPRINTF("%s: %s doesn't exist.\n", FUNCNAME, X2M_SEL_PORTRAIT);
            return false;
        }

        if (!BtlPortraitExists())
        {
            // Battle portrait was mandatory
            if (format_version <  X2M_MIN_VERSION_AUTO_BTL_PORTRAITS)
            {
                DPRINTF("%s: %s doesn't exist.\n", FUNCNAME, X2M_BTL_PORTRAIT);
                return false;
            }
        }

        if (slot_entries.size() == 0)
        {
            DPRINTF("%s: There must be at least one SlotEntry.\n", FUNCNAME);
            return false;
        }

        if (slot_entries.size() > XV2_MAX_SUBSLOTS)
        {
            DPRINTF("%s: Number of slots entries cannot be bigger than %d\n", FUNCNAME, XV2_MAX_SUBSLOTS);
            return false;
        }

        for (const X2mSlotEntry &entry: slot_entries)
        {
            if (entry.costume_name[XV2_LANG_ENGLISH].length() == 0)
            {
                DPRINTF("%s: COSTUME_NAME_EN is not optional.\n", FUNCNAME);
                return false;
            }            
        }        

        if (cms_entry.id == X2M_INVALID_ID)
        {
            DPRINTF("%s: There is no cms entry!\n", FUNCNAME);
            return false;
        }

        if (skill_sets.size() == 0)
        {
            DPRINTF("%s: There must be at least one SkillSet.\n", FUNCNAME);
            return false;
        }

        if (skill_sets.size() != slot_entries.size())
        {
            DPRINTF("%s: Number of skill sets must be same as number of slot entries.\n", FUNCNAME);
            return false;
        }

        if (slot_entries.size() > 1)
        {
            struct Combination
            {
                int costume_index;
                int model_preset;

                inline bool operator==(const Combination &rhs) const
                {
                    return (this->costume_index == rhs.costume_index && this->model_preset == rhs.model_preset);
                }

                inline bool operator!=(const Combination &rhs) const
                {
                    return !(*this == rhs);
                }
            };        

            std::vector<Combination> combinations;

            for (size_t i = 0; i < slot_entries.size(); i++)
            {
                Combination comb;

                comb.costume_index = slot_entries[i].costume_index;
                comb.model_preset = slot_entries[i].model_preset;

                if (std::find(combinations.begin(), combinations.end(), comb) == combinations.end())
                {
                    combinations.push_back(comb);
                }
                else
                {
                    DPRINTF("%s: Every combination of costume_index+model_preset must be unique.\n", FUNCNAME);
                    return false;
                }
            }
        }

        size_t num_costumes = GetNumCostumes();

        if (HasCso())
        {
            if (cso_entries.size() != num_costumes)
            {
                DPRINTF("%s: The number of cso entries must be equal to the number of different costume_index.\n", FUNCNAME);
                return false;
            }
        }

        if (HasPsc())
        {
            if (psc_entries.size() != num_costumes)
            {
                DPRINTF("%s: The number of psc entries must be equal to the number of different costume_index.\n", FUNCNAME);
                return false;
            }
        }

        if (HasAur())
        {
            if (aur_entries.size() != num_costumes)
            {
                DPRINTF("%s: The number of aur entries must be equal to the number of different costume_index.\n", FUNCNAME);
                return false;
            }
        }

        if (HasSevHL() && HasSevLL())
        {
            DPRINTF("%s: A mod cannot have SevHL and SevLL at the same time.\n", FUNCNAME);
            return false;
        }

        if (HasSevHL())
        {
            std::unordered_set<uint32_t> costumes;

            for (const SevEntryHL &entry : sev_hl_entries)
            {
                if (entry.copy_char == entry_name)
                {
                    DPRINTF("%s: Recursive prevention unit, copy_char cannot be the entry_name of the mod.\n", FUNCNAME);
                    return false;
                }

                costumes.insert(entry.costume_id);
            }

            if (costumes.size() != sev_hl_entries.size())
            {
                DPRINTF("%s: Each costume_id in each sev_hl entry must be unique.\n", FUNCNAME);
                return false;
            }
        }
        else if (HasSevLL())
        {
            std::unordered_set<uint32_t> costumes;

            for (const SevEntryLL &entry : sev_ll_entries)
            {
                for (const SevEventsLL &sub_entry : entry.sub_entries)
                {
                    if (!sub_entry.im_source && sub_entry.char_code.length() == 0)
                    {
                        DPRINTF("%s: In SevEventsLL: if im_source is false, char_code cannot be empty.\n", FUNCNAME);
                        return false;
                    }
                }

                costumes.insert(entry.costume_id);
            }

            if (costumes.size() != sev_ll_entries.size())
            {
                DPRINTF("%s: Each costume_id in each sev_ll entry must be unique.\n", FUNCNAME);
                return false;
            }
        }

        if (HasCml())
        {
            if (cml_entries.size() != num_costumes)
            {
                DPRINTF("%s: The number of cml entries must be equal to the number of different costume_index.\n", FUNCNAME);
                return false;
            }
        }

        if (format_version >= X2M_MIN_VERSION_CSS_AUDIO)
        {
            for (size_t i = 0; i < slot_entries.size(); i++)
            {
                const X2mSlotEntry &slot = slot_entries[i];

                for (size_t j = 0; j < 2; j++)
                {
                    if (slot.audio_files[j].length() > 0)
                    {
                        if (!CssAudioFileExists(slot.audio_files[j]))
                        {
                            DPRINTF("%s: The audio file with base name \"%s\" doesn't exist!\n", FUNCNAME, slot.audio_files[j].c_str());
                            return false;
                        }
                    }
                }
            }
        }

        if (HasCharaSkillDepends())
        {
            std::unordered_set<std::string> dep_guids;
            std::unordered_set<uint32_t> dep_ids;

            for (const X2mDepends &dep : chara_skill_depends)
            {
                const std::string guid_str = Utils::GUID2String(dep.guid);

                if (dep_guids.find(guid_str) != dep_guids.end())
                {
                    DPRINTF("%s: Chara Skill Depends with guid %s was previously defined.\n", FUNCNAME, guid_str.c_str());
                    return false;
                }

                if (dep_ids.find(dep.id) != dep_ids.end())
                {
                    DPRINTF("%s: Chara Skill Depends with id 0x%x was previously defined.\n", FUNCNAME, dep.id);
                    return false;
                }

                if (dep.id < X2M_SKILL_DEPENDS_BEGIN || dep.id >= X2M_SKILL_DEPENDS_END)
                {
                    DPRINTF("%s: id 0x%x is outside of the valid range for Chara Skill Depends.\n", FUNCNAME, dep.id);
                    return false;
                }

                if (!IsCharaSkillDependsReferenced(dep))
                {
                    DPRINTF("%s: Chara Skill Depends with id 0x%x is not referenced in the CusSkillSets.\n", FUNCNAME, dep.id);
                    return false;
                }

                dep_guids.insert(guid_str);
                dep_ids.insert(dep.id);
            }
        }

        if (chara_auras.size() != 0 && chara_auras.size() != aur_entries.size())
        {
            DPRINTF("%s: chara_auras size must match aur entries or be 0.\n", FUNCNAME);
            return false;
        }
    }
    else if (type == X2mType::NEW_SKILL)
    {
        if (skill_type == X2mSkillType::BLAST)
        {
            DPRINTF("%s: BLAST skill type is currently not supported.\n", FUNCNAME);
            return false;
        }

        if (skill_name[XV2_LANG_ENGLISH].length() == 0)
        {
            DPRINTF("%s: SKILL_NAME_EN cannot be empty.\n", FUNCNAME);
            return false;
        }

        if (skill_desc[XV2_LANG_ENGLISH].length() == 0)
        {
            for (int i = 0; i < XV2_LANG_NUM; i++)
            {
                if (i == XV2_LANG_ENGLISH)
                    continue;

                if (skill_desc[XV2_LANG_ENGLISH].length() != 0)
                {
                    DPRINTF("%s: skill description in language %d, but not in english.\n", FUNCNAME, i);
                    return false;
                }
            }
        }

        if (skill_type == X2mSkillType::AWAKEN)
        {
            if (skill_trans_names.size() == 0)
            {
                DPRINTF("%s: Awaken skill, but no transformation names.\n", FUNCNAME);
                return false;
            }

            for (const X2mSkillTransName &tn : skill_trans_names)
            {
                if (tn.trans_name[XV2_LANG_ENGLISH].length() == 0)
                {
                    DPRINTF("%s: Tansformation name empty for english language.\n", FUNCNAME);
                    return false;
                }
            }
        }

        if (!SkillDirectoryExists())
        {
            DPRINTF("%s: Skill directory doesn't exist.\n", FUNCNAME);
            return false;
        }

        if (skill_entry.id == X2M_INVALID_ID16)
        {
            DPRINTF("%s: There is no skill entry!\n", FUNCNAME);
            return false;
        }

        if (skill_entry.name.length() != 3 && skill_entry.name.length() != 4)
        {
            DPRINTF("%s: Skill name in <Skill> must be 3 or 4 character string.\n", FUNCNAME);
            return false;
        }

        if (HasSkillCostumeDepend())
        {
            if (!IsSkillCostumeDependReferenced())
            {
                DPRINTF("%s: Skill Costume Depends is not referenced in the cus entry.\n", FUNCNAME);
                return false;

            }
        }
    }    
    else if (type == X2mType::NEW_COSTUME)
    {
        if (costume_partsets.size() == 0)
        {
            DPRINTF("%s: There must be at least one <PartSet>.\n", FUNCNAME);
            return false;
        }

        int sets_count = -1;

        for (uint8_t race = 0; race < X2M_CR_NUM; race++)
        {
            size_t count = GetNumCostumePartSets(race);

            if (count != 0)
            {
                if (sets_count == -1)
                {
                    sets_count = (int)sets_count;
                }
                else if (sets_count != (int)count)
                {
                    DPRINTF("%s: The number of sets for all races must be the same.\n", FUNCNAME);
                    return false;
                }
            }
        }

        for (const X2mItem &item : costume_items)
        {
            if (item.idb.model >= GetEffectiveNumCostumePartSets())
            {
                DPRINTF("%s: idb model cannot be >= than number of partsets.\n", FUNCNAME);
                return false;
            }
        }
    }
    else if (type == X2mType::REPLACER)
    {
        if (!JungleExists())
        {
            DPRINTF("%s: Jungle must exist for a replacer mod.\n", FUNCNAME);
            return false;
        }
    }

    else
    {
        DPRINTF("%s: Unknown mod type.\n", FUNCNAME);
        return false;
    }

    return true;
}

bool X2mFile::Decompile()
{
    if (!Validate())
        return false;

    TiXmlDocument doc;

    TiXmlDeclaration* decl = new TiXmlDeclaration("1.0", "utf-8", "" );
    doc.LinkEndChild(decl);

    TiXmlElement *root = new TiXmlElement("X2M");
    std::string temp_str;

    if (type == X2mType::REPLACER)
        temp_str = "REPLACER";
    else if (type == X2mType::NEW_SKILL)
        temp_str = "NEW_SKILL";
    else if (type == X2mType::NEW_COSTUME)
        temp_str = "NEW_COSTUME";
    else
        temp_str = "NEW_CHARACTER";

    root->SetAttribute("type", temp_str);
    Utils::WriteParamFloat(root, "X2M_FORMAT_VERSION", format_version);

    Utils::WriteParamString(root, "MOD_NAME", mod_name);
    Utils::WriteParamString(root, "MOD_AUTHOR", mod_author);
    Utils::WriteParamFloat(root, "MOD_VERSION", mod_version);
    Utils::WriteParamGUID(root, "MOD_GUID", mod_guid);

    if (format_version >= X2M_MIN_VERSION_UDATA)
        Utils::WriteParamString(root, "UDATA", Utils::Base64Encode(udata, sizeof(udata), false));

    if (type == X2mType::NEW_CHARACTER)
    {
        Utils::WriteParamString(root, "ENTRY_NAME", entry_name);

        if (is_ozaru)
        {
            Utils::WriteParamString(root, "IS_OZARU", "true");
        }

        if (body_shape >= 0)
        {
            Utils::WriteParamUnsigned(root, "BODY_SHAPE", body_shape);
        }

        for (size_t i = 0; i < XV2_LANG_NUM; i++)
        {
            const std::string &name = chara_name[i];

            if (name.length() == 0)
                continue;

            temp_str = "CHARA_NAME_" + Utils::ToUpperCase(xv2_lang_codes[i]);
            Utils::WriteParamString(root, temp_str.c_str(), name);
        }

        if (HasMultNames())
        {
            for (const X2mCharaName &name : mult_chara_names)
            {
                if (!name.Decompile(root))
                    return false;
            }
        }

        for (const X2mSlotEntry &entry : slot_entries)
        {
            if (!entry.Decompile(root, format_version >= X2M_MIN_VERSION_CSS_AUDIO))
                return false;
        }

        cms_entry.id = X2M_DUMMY_ID;

        if (!cms_entry.Decompile(root))
            return false;

        for (CusSkillSet &entry : skill_sets)
        {
            entry.char_id = entry.costume_id = X2M_DUMMY_ID;
            entry.model_preset = X2M_DUMMY_ID16;

            if (!entry.Decompile(root))
                return false;
        }

        for (CsoEntry &entry : cso_entries)
        {
            entry.char_id = entry.costume_id = X2M_DUMMY_ID;

            if (!entry.Decompile(root))
                return false;
        }

        for (PscSpecEntry &entry : psc_entries)
        {
            entry.costume_id = entry.costume_id2 = X2M_DUMMY_ID;

            if (!entry.Decompile(root))
                return false;
        }

        for (AurCharaLink &entry : aur_entries)
        {
            entry.char_id = entry.costume_id = X2M_DUMMY_ID;

            if (!entry.Decompile(root))
                return false;
        }

        for (const SevEntryHL &entry : sev_hl_entries)
        {
            if (!entry.Decompile(root))
                return false;
        }

        for (const SevEntryLL &entry : sev_ll_entries)
        {
            if (!entry.Decompile(root))
                return false;
        }

        for (CmlEntry &entry : cml_entries)
        {
            entry.char_id = entry.costume_id = X2M_DUMMY_ID16;

            if (!entry.Decompile(root))
                return false;
        }

        std::vector<HciEntry> used_hci_entries;

        for (HciEntry &entry : hci_entries)
        {
            entry.char_id = X2M_DUMMY_ID16;

            if (std::find(used_hci_entries.begin(), used_hci_entries.end(), entry) == used_hci_entries.end())
            {
                used_hci_entries.push_back(entry);

                if (!entry.Decompile(root))
                    return false;
            }
        }

        for (const X2mDepends &dep : chara_skill_depends)
        {
            if (!dep.Decompile(root))
                return false;
        }

        for (AurAura &aur : chara_auras)
        {
            aur.id = X2M_DUMMY_ID;

            if (!aur.Decompile(root, aura_types))
                return false;
        }
    }
    else if (type == X2mType::NEW_SKILL)
    {
        if (skill_type == X2mSkillType::SUPER)
            temp_str = "SUPER";
        else if (skill_type == X2mSkillType::ULTIMATE)
            temp_str = "ULTIMATE";
        else if (skill_type == X2mSkillType::EVASIVE)
            temp_str = "EVASIVE";
        else if (skill_type == X2mSkillType::BLAST)
            temp_str = "BLAST";
        else
            temp_str = "AWAKEN";

        Utils::WriteParamString(root, "SKILL_TYPE", temp_str);

        for (size_t i = 0; i < XV2_LANG_NUM; i++)
        {
            const std::string &name = skill_name[i];

            if (name.length() == 0)
                continue;

            temp_str = "SKILL_NAME_" + Utils::ToUpperCase(xv2_lang_codes[i]);
            Utils::WriteParamString(root, temp_str.c_str(), name);
        }

        for (size_t i = 0; i < XV2_LANG_NUM; i++)
        {
            const std::string &desc = skill_desc[i];

            if (desc.length() == 0)
                continue;

            temp_str = "SKILL_DESC_" + Utils::ToUpperCase(xv2_lang_codes[i]);
            Utils::WriteParamString(root, temp_str.c_str(), desc);
        }

        if (skill_type == X2mSkillType::AWAKEN)
        {
            for (const X2mSkillTransName &tn : skill_trans_names)
            {
                if (!tn.Decompile(root))
                    return false;
            }
        }

        skill_entry.id = skill_entry.id2 = X2M_DUMMY_ID16;

        if (!skill_entry.Decompile(root))
            return false;

        if (HasSkillIdbEntry())
        {
            skill_idb_entry.id = skill_idb_entry.name_id = skill_idb_entry.desc_id = skill_idb_entry.type = X2M_DUMMY_ID16;

            if (!skill_idb_entry.Decompile(root))
                return false;
        }

        if (HasSkillPup())
        {
            for (PupEntry &entry : skill_pup_entries)
            {
                entry.id = X2M_DUMMY_ID;
                if (!entry.Decompile(root))
                    return false;
            }
        }

        if (HasSkillAura())
        {
            for (X2mSkillAura &aura : skill_aura_entries)
            {
                aura.data.cus_aura_id = X2M_DUMMY_ID16;
                aura.aura.id = X2M_DUMMY_ID;

                if (!aura.Decompile(root))
                    return false;
            }
        }

        if (HasSkillCostumeDepend())
        {
            if (!skill_costume_depend.Decompile(root))
                return false;
        }
    }
    else if (type == X2mType::NEW_COSTUME)
    {
        for (X2mItem &item : costume_items)
        {
            item.idb.id = item.idb.name_id = item.idb.desc_id = X2M_DUMMY_ID16;
            if (!item.Decompile(root))
                return false;
        }

        for (const X2mPartSet &set : costume_partsets)
        {
            if (!set.Decompile(root))
                return false;
        }
    }

    doc.LinkEndChild(root);

    TiXmlPrinter printer;
    doc.Accept(&printer);

    const char *xml_buf = printer.CStr();
    return WriteFile("x2m.xml", xml_buf, strlen(xml_buf));
}

bool X2mFile::Compile()
{
    TiXmlDocument doc;
    char *xml_buf;

    xml_buf = ReadTextFile("x2m.xml");
    if (!xml_buf)
    {
        DPRINTF("%s: x2m.xml not found.\n", FUNCNAME);
        return false;
    }

    doc.Parse(xml_buf);
    delete[] xml_buf;

    if (doc.ErrorId() != 0)
    {
        DPRINTF("%s: cannot parse xml (wrong formatted xml?)\n\n"
                "This is what tinyxml says: %s (%d). Line=%d, col=%d\n", FUNCNAME, doc.ErrorDesc(), doc.ErrorId(), doc.ErrorRow(), doc.ErrorCol());

        return false;
    }

    TiXmlHandle handle(&doc);
    TiXmlElement *root = Utils::FindRoot(&handle, "X2M");
    if (!root)
    {
        DPRINTF("%s: Cannot find \"X2M\" in xml.\n", FUNCNAME);
        return false;
    }

    if (!Utils::GetParamFloat(root, "X2M_FORMAT_VERSION", &format_version))
        return false;

    std::string temp_str;

    if (!Utils::ReadAttrString(root, "type", temp_str))
    {
        DPRINTF("%s: type attribute is not optional.\n", FUNCNAME);
        return false;
    }

    if (temp_str == "REPLACER")
    {
        type = X2mType::REPLACER;
    }
    else if (temp_str == "NEW_CHARACTER")
    {
        type = X2mType::NEW_CHARACTER;
    }
    else
    {
        if (format_version >= X2M_MIN_VERSION_SKILLS && temp_str == "NEW_SKILL")
        {
            type = X2mType::NEW_SKILL;
        }
        else if (format_version >= X2M_MIN_VERSION_COSTUME && temp_str == "NEW_COSTUME")
        {
            type = X2mType::NEW_COSTUME;
        }
        else
        {
            DPRINTF("%s: Unrecognized type: \"%s\"\n", FUNCNAME, temp_str.c_str());
            return false;
        }
    }

    if (format_version >= X2M_MIN_VERSION_UDATA && type != X2mType::REPLACER)
    {
        if (!Utils::GetParamString(root, "UDATA", temp_str))
            return false;

        size_t size;
        uint8_t *temp_buf = Utils::Base64Decode(temp_str, &size);

        if (!temp_buf || size != sizeof(udata))
        {
            DPRINTF("%s: corrupted file.\n", FUNCNAME);
            if (temp_buf)
                delete[] temp_buf;

            return false;
        }

        memcpy(udata, temp_buf, sizeof(udata));
        delete[] temp_buf;
    }

    if (!Utils::GetParamString(root, "MOD_NAME", mod_name))
        return false;

    if (!Utils::GetParamString(root, "MOD_AUTHOR", mod_author))
        return false;

    if (!Utils::GetParamFloat(root, "MOD_VERSION", &mod_version))
        return false;

    if (!Utils::GetParamGUID(root, "MOD_GUID", mod_guid))
        return false;

    if (type == X2mType::NEW_CHARACTER)
    {
        if (!Utils::GetParamString(root, "ENTRY_NAME", entry_name))
            return false;

        entry_name = Utils::ToUpperCase(entry_name);

        for (int i = 0; i < XV2_LANG_NUM; i++)
        {
            temp_str = "CHARA_NAME_" + Utils::ToUpperCase(xv2_lang_codes[i]);
            Utils::ReadParamString(root, temp_str.c_str(), chara_name[i]);
        }

        std::string ozaru_str;

        if (format_version >= X2M_MIN_VERSION_OZARU && Utils::ReadParamString(root, "IS_OZARU", ozaru_str))
        {
            is_ozaru = (Utils::ToLowerCase(ozaru_str) == "true" || ozaru_str == "1");
        }
        else
        {
            is_ozaru = false; // Not really needed, done at Reset
        }

        if (format_version >= X2M_MIN_VERSION_BODY_SHAPES)
        {
            if (!Utils::ReadParamUnsigned(root, "BODY_SHAPE", (uint32_t *)&body_shape))
                body_shape = -1;
        }
    }    
    else if (type == X2mType::NEW_SKILL)
    {
        if (!Utils::GetParamString(root, "SKILL_TYPE", temp_str))
            return false;

        if (temp_str == "SUPER")
            skill_type = X2mSkillType::SUPER;
        else if (temp_str == "ULTIMATE")
            skill_type = X2mSkillType::ULTIMATE;
        else if (temp_str == "EVASIVE")
            skill_type= X2mSkillType::EVASIVE;
        else if (temp_str == "BLAST")
            skill_type = X2mSkillType::BLAST;
        else if (temp_str == "AWAKEN")
            skill_type = X2mSkillType::AWAKEN;
        else
        {
            DPRINTF("%s: Unrecognized skill type \"%s\"\n", FUNCNAME, temp_str.c_str());
            return false;
        }

        for (int i = 0; i < XV2_LANG_NUM; i++)
        {
            temp_str = "SKILL_NAME_" + Utils::ToUpperCase(xv2_lang_codes[i]);
            Utils::ReadParamString(root, temp_str.c_str(), skill_name[i]);

            temp_str = "SKILL_DESC_" + Utils::ToUpperCase(xv2_lang_codes[i]);
            Utils::ReadParamString(root, temp_str.c_str(), skill_desc[i]);
        }
    }

    for (const TiXmlElement *elem = root->FirstChildElement(); elem; elem = elem->NextSiblingElement())
    {
        const std::string &param_name = elem->ValueStr();

        if (type == X2mType::NEW_CHARACTER)
        {
            if (param_name == "SlotEntry")
            {
                X2mSlotEntry entry;

                if (!entry.Compile(elem, format_version >= X2M_MIN_VERSION_CSS_AUDIO))
                    return false;

                slot_entries.push_back(entry);
            }
            else if (param_name == "X2mCharaName" && format_version >= X2M_MIN_VERSION_MULTIPLE_CHAR_NAMES)
            {
                X2mCharaName name;

                if (!name.Compile(elem))
                    return false;

                mult_chara_names.push_back(name);
            }
            else if (param_name == "Entry")
            {
                if (!cms_entry.Compile(elem))
                {
                    cms_entry.id = X2M_INVALID_ID;
                    return false;
                }
            }
            else if (param_name == "SkillSet")
            {
                CusSkillSet entry;

                if (!entry.Compile(elem))
                    return false;

                skill_sets.push_back(entry);
            }
            else if (param_name == "CsoEntry" && format_version >= X2M_MIN_VERSION_CSO)
            {
                CsoEntry entry;

                if (!entry.Compile(elem))
                    return false;

                cso_entries.push_back(entry);
            }
            else if (param_name == "PscSpecEntry" && format_version >= X2M_MIN_VERSION_PSC)
            {
                PscSpecEntry entry;

                if (!entry.Compile(elem))
                    return false;

                psc_entries.push_back(entry);
            }
            else if (param_name == "CharacLink" && format_version >= X2M_MIN_VERSION_AUR)
            {
                AurCharaLink entry;

                if (!entry.Compile(elem))
                    return false;

                aur_entries.push_back(entry);
            }
            else if (param_name == "SevEntryHL" && format_version >= X2M_MIN_VERSION_SEV)
            {
                SevEntryHL entry;

                if (!entry.Compile(elem))
                    return false;

                sev_hl_entries.push_back(entry);
            }
            else if (param_name == "SevEntryLL" && format_version >= X2M_MIN_VERSION_SEV)
            {
                SevEntryLL entry;

                if (!entry.Compile(elem))
                    return false;

                sev_ll_entries.push_back(entry);
            }
            else if (param_name == "CmlEntry" && format_version >= X2M_MIN_VERSION_CML)
            {
                CmlEntry entry;

                if (!entry.Compile(elem))
                    return false;

                cml_entries.push_back(entry);
            }
            else if (param_name == "HciEntry" && format_version >= X2M_MIN_VERSION_HCI)
            {
                HciEntry entry;

                if (!entry.Compile(elem))
                    return false;

                entry.char_id = X2M_DUMMY_ID16;

                if (std::find(hci_entries.begin(), hci_entries.end(), entry) == hci_entries.end())
                {
                    hci_entries.push_back(entry);
                }
            }
            else if (param_name == "X2mDepends" && format_version >= X2M_MIN_VERSION_SKILLS)
            {
                X2mDepends entry;

                if (!entry.Compile(elem))
                    return false;

                if (entry.type == X2mDependsType::SKILL)
                {
                    chara_skill_depends.push_back(entry);
                }
            }
            else if (param_name == "Aura" && format_version >= X2M_MIN_VERSION_AURAS)
            {
                AurAura aura;

                if (!aura.Compile(elem, aura_types))
                    return false;

                chara_auras.push_back(aura);
            }
        } // End NEW_CHARACTER
        else if (type == X2mType::NEW_SKILL)
        {
            if (param_name == "Skill")
            {
                if (!skill_entry.Compile(elem))
                {
                    skill_entry.id = skill_entry.id2 = X2M_INVALID_ID16;
                    return false;
                }

                skill_entry.id = X2M_DUMMY_ID16;
            }
            else if (param_name == "IdbEntry")
            {
                if (!skill_idb_entry.Compile(elem))
                {
                    skill_idb_entry.id = X2M_INVALID_ID16;
                    return false;
                }
            }
            else if (param_name == "PupEntry")
            {
                PupEntry entry;

                if (!entry.Compile(elem))
                    return false;

                skill_pup_entries.push_back(entry);
            }
            else if (skill_type == X2mSkillType::AWAKEN && param_name == "X2mSkillTransName")
            {
                X2mSkillTransName tn;

                if (!tn.Compile(elem))
                    return false;

                skill_trans_names.push_back(tn);
            }
            else if (param_name == "X2mSkillAura" && format_version >= X2M_MIN_VERSION_AURAS)
            {
                X2mSkillAura aura;

                if (!aura.Compile(elem))
                    return false;

                aura.data.cus_aura_id = X2M_DUMMY_ID16;
                aura.aura.id = X2M_DUMMY_ID;

                skill_aura_entries.push_back(aura);
            }
            else if (param_name == "X2mDepends" && format_version >= X2M_MIN_VERSION_COSTUME)
            {
                if (!skill_costume_depend.Compile(elem) || skill_costume_depend.type != X2mDependsType::COSTUME)
                {
                    skill_costume_depend.id = X2M_INVALID_ID;
                    return false;
                }
            }
        } // END NEW_SKILL
        else if (type == X2mType::NEW_COSTUME)
        {
            if (param_name == "X2mItem")
            {
                X2mItem item;

                if (!item.Compile(elem))
                    return false;

                costume_items.push_back(item);
            }
            else if (param_name == "X2mPartSet")
            {
                X2mPartSet set;

                if (!set.Compile(elem))
                    return false;

                costume_partsets.push_back(set);
            }

        } // END NEW COSTUME
    }

    return Validate();
}

bool X2mFile::Load(const uint8_t *buf, size_t size)
{
    Reset();

    if (!ZipFile::Load(buf, size))
        return false;

    return Compile();
}

uint8_t *X2mFile::Save(size_t *psize)
{
    if (!Decompile())
        return nullptr;

    return ZipFile::Save(psize);
}

bool X2mFile::CharaDirectoryExists() const
{
    if (entry_name.length() == 0)
        return false;

    return DirExists(entry_name);
}

void X2mFile::DeleteCharaDirectory()
{
    if (entry_name.length() == 0)
        return;

    DeleteDir(entry_name);
}

size_t X2mFile::GetNumCostumes() const
{
    std::unordered_set<int32_t> costumes;

    for (const X2mSlotEntry &slot : slot_entries)
    {
        costumes.insert(slot.costume_index);
    }

    return costumes.size();
}

bool X2mFile::CostumeExists(int32_t costume_id) const
{
    std::unordered_set<int32_t> costumes;

    for (const X2mSlotEntry &slot : slot_entries)
    {
        costumes.insert(slot.costume_index);
    }

    return (costumes.find(costume_id) != costumes.end());
}

X2mFile *X2mFile::CreateDummyPackage() const
{
    X2mFile *dummy = new X2mFile();

    dummy->type = type;
    dummy->format_version = format_version;

    dummy->mod_name = mod_name;
    dummy->mod_author = mod_author;
    dummy->mod_version = mod_version;
    memcpy(dummy->mod_guid, mod_guid, sizeof(mod_guid));
    memcpy(dummy->udata, udata, sizeof(udata));

    dummy->entry_name = entry_name;
    dummy->chara_name = chara_name;
    dummy->mult_chara_names = mult_chara_names;
    dummy->is_ozaru = is_ozaru;
    dummy->body_shape = body_shape;

    dummy->slot_entries = slot_entries;
    dummy->cms_entry = cms_entry;
    dummy->skill_sets = skill_sets;
    dummy->cso_entries = cso_entries;
    dummy->psc_entries = psc_entries;
    dummy->aur_entries = aur_entries;
    dummy->sev_hl_entries = sev_hl_entries;
    dummy->sev_ll_entries = sev_ll_entries;
    dummy->cml_entries = cml_entries;
    dummy->hci_entries = hci_entries;

    dummy->chara_skill_depends = chara_skill_depends;
    dummy->chara_auras = chara_auras;

    dummy->skill_name = skill_name;
    dummy->skill_desc = skill_desc;
    dummy->skill_type = skill_type;
    dummy->skill_trans_names = skill_trans_names;
    dummy->skill_entry = skill_entry;
    dummy->skill_idb_entry = skill_idb_entry;
    dummy->skill_pup_entries = skill_pup_entries;
    dummy->skill_aura_entries = skill_aura_entries;
    dummy->skill_costume_depend = skill_costume_depend;

    dummy->costume_items = costume_items;
    dummy->costume_partsets = costume_partsets;      

    size_t num_entries = GetNumEntries();
    const std::string dummy_content = "DUMMY";

    for (size_t i = 0; i < num_entries; i++)
    {
        zip_stat_t zstat;

        if (zip_stat_index(archive, i, 0, &zstat) == -1)
            continue;

        if (!(zstat.valid & ZIP_STAT_SIZE))
            continue;

        if (!(zstat.valid & ZIP_STAT_NAME))
            continue;

        std::string entry_path = Utils::NormalizePath(zstat.name);

        if (entry_path.back() == '/') // Stupid explicit directory entry added by winrar and others
            continue;

        if (!dummy->WriteFile(entry_path, dummy_content.c_str(), dummy_content.length()))
        {
            delete dummy;
            return nullptr;
        }
    }

    return dummy;
}

bool X2mFile::HasDangerousJungle() const
{
    if (!JungleExists())
        return false;

    static const std::vector<std::string> dangerous_paths =
    {
        std::string(X2M_JUNGLE) + "data/system/",
        std::string(X2M_JUNGLE) + "XV2PATCHER/",
        std::string(X2M_JUNGLE) + "bin/"
    };

    static const std::vector<std::string> dangerous_files =
    {
        std::string(X2M_JUNGLE) + "data/ui/iggy/CHARASELE.iggy",
        std::string(X2M_JUNGLE) + "data/ui/texture/CHARA01.emb",
        std::string(X2M_JUNGLE) + "data/ui/CharaImage/chara_image.hci",
        std::string(X2M_JUNGLE) + "data/sound/VOX/Sys/CRT_CS_vox.acb",
        std::string(X2M_JUNGLE) + "data/sound/VOX/Sys/CRT_CS_vox.awb",
        std::string(X2M_JUNGLE) + "data/sound/VOX/Sys/en/CRT_CS_vox.acb",
        std::string(X2M_JUNGLE) + "data/sound/VOX/Sys/en/CRT_CS_vox.awb",
        std::string(X2M_JUNGLE) + "data/chara/HUM/HUM.bcs",
        std::string(X2M_JUNGLE) + "data/chara/HUF/HUF.bcs",
        std::string(X2M_JUNGLE) + "data/chara/NMC/NMC.bcs",
        std::string(X2M_JUNGLE) + "data/chara/FRI/FRI.bcs",
        std::string(X2M_JUNGLE) + "data/chara/MAM/MAM.bcs",
        std::string(X2M_JUNGLE) + "data/chara/MAF/MAF.bcs"
    };

    for (const std::string &path : dangerous_paths)
    {
        if (DirExists(path))
            return true;
    }

    for (const std::string &file : dangerous_files)
    {
        if (FileExists(file))
            return true;
    }

    return false;
}

bool X2mFile::CssAudioFileExists(const std::string &name, bool english) const
{
    if (english)
        return FileExists(std::string(X2M_CSS_AUDIO) + name + "_en.hca");

    return FileExists(std::string(X2M_CSS_AUDIO) + name + "_jp.hca");
}

bool X2mFile::CssAudioFileExists(const std::string &name) const
{
    return (CssAudioFileExists(name, false) || CssAudioFileExists(name, true));
}

HcaFile *X2mFile::LoadCssAudioFile(const std::string &name, bool english, bool fallback)
{
    const std::string jp_path = std::string(X2M_CSS_AUDIO) + name + "_jp.hca";
    const std::string en_path = std::string(X2M_CSS_AUDIO) + name + "_en.hca";

    size_t size;
    uint8_t *buf = ReadFile((english) ? en_path : jp_path, &size);

    if (!buf)
    {
        if (!fallback)
            return nullptr;

        // Load from the other language
        buf = ReadFile((english) ? jp_path : en_path, &size);
        if (!buf)
            return nullptr;
    }

    HcaFile *hca = new HcaFile();
    if (!hca->Load(buf, size))
    {
        delete hca;
        return nullptr;
    }

    hca->SetCiphType(0);
    return hca;
}

size_t X2mFile::GetCssAudioFiles(std::vector<std::string> &files)
{
    files.clear();

    size_t num_entries = GetNumEntries();

    for (size_t i = 0; i < num_entries; i++)
    {
        zip_stat_t zstat;

        if (zip_stat_index(archive, i, 0, &zstat) == -1)
            continue;

        if (!(zstat.valid & ZIP_STAT_NAME))
            continue;

        std::string entry_path = Utils::NormalizePath(zstat.name);

        if (Utils::BeginsWith(entry_path, X2M_CSS_AUDIO, false))
        {
            if (Utils::EndsWith(entry_path, "_jp.hca", false) || Utils::EndsWith(entry_path, "_en.hca", false))
            {
                std::string base = Utils::GetFileNameString(entry_path);
                if (base.length() == strlen("_jp.hca"))
                    continue;

                base = Utils::ToUpperCase(base.substr(0, base.length()-strlen("_jp.hca")));

                if (std::find(files.begin(), files.end(), base) == files.end())
                    files.push_back(base);
            }
        }
    }

    return files.size();
}

static bool css_audio_visitor(const std::string &path, bool, void *param)
{
    X2mFile *pthis = (X2mFile *)param;

    if (Utils::EndsWith(path, "_jp.hca", false) || Utils::EndsWith(path, "_en.hca", false))
    {
        size_t size;
        uint8_t *buf = Utils::ReadFile(path, &size);
        if (buf)
        {
            std::string base = Utils::GetFileNameString(path);
            if (base.length() <= strlen("_jp.hca"))
            {
                delete[] buf;
                return true;
            }

            pthis->WriteFile(X2M_CSS_AUDIO + base, buf, size);
            delete[] buf;
        }
    }

    return true;
}

size_t X2mFile::SetCssAudioDir(const std::string &dir_path)
{
    Utils::VisitDirectory(dir_path, true, false, false, css_audio_visitor, this);

    std::vector<std::string> files;
    return GetCssAudioFiles(files);
}

void X2mFile::DeleteCssAudio()
{
    DeleteDir(X2M_CSS_AUDIO);
}

bool X2mFile::CharaSkillDependsExist(const uint8_t *guid) const
{
    for (const X2mDepends &dep : chara_skill_depends)
    {
        if (memcmp(dep.guid, guid, 16) == 0)
            return true;
    }

    return false;
}

bool X2mFile::CharaSkillDependsExist(const std::string &guid) const
{
    uint8_t guid_bin[16];

    if (!Utils::String2GUID(guid_bin, guid))
        return false;

    return CharaSkillDependsExist(guid_bin);
}

bool X2mFile::CharaSkillDependsExist(uint16_t id) const
{
    for (const X2mDepends &dep : chara_skill_depends)
    {
        if (dep.id == id)
            return true;
    }

    return false;
}

X2mDepends *X2mFile::FindCharaSkillDepends(const uint8_t *guid)
{
    for (X2mDepends &dep : chara_skill_depends)
    {
        if (memcmp(dep.guid, guid, 16) == 0)
            return &dep;
    }

    return nullptr;
}

X2mDepends *X2mFile::FindCharaSkillDepends(const std::string &guid)
{
    uint8_t guid_bin[16];

    if (!Utils::String2GUID(guid_bin, guid))
        return nullptr;

    return FindCharaSkillDepends(guid_bin);
}

X2mDepends *X2mFile::FindCharaSkillDepends(uint16_t id)
{
    for (X2mDepends &dep : chara_skill_depends)
    {
        if (dep.id == id)
            return &dep;
    }

    return nullptr;
}

bool X2mFile::AddCharaSkillDepends(const X2mDepends &depends)
{
    if (CharaSkillDependsExist(depends.guid) || CharaSkillDependsExist(depends.id))
        return false;

    chara_skill_depends.push_back(depends);
    return true;
}

bool X2mFile::AddCharaSkillDepends(X2mFile *skill_x2m, bool update)
{
    if (skill_x2m->GetType() != X2mType::NEW_SKILL)
        return false;

    X2mDepends *existing = FindCharaSkillDepends(skill_x2m->mod_guid);
    if (existing)
    {
        if (!update)
            return false;

        existing->name = skill_x2m->GetModName();
        return true;
    }

    X2mDepends new_dep;

    new_dep.type = X2mDependsType::SKILL;
    memcpy(new_dep.guid, skill_x2m->mod_guid, 16);
    new_dep.name = skill_x2m->GetModName();

    for (uint16_t id = X2M_SKILL_DEPENDS_BEGIN; id < X2M_SKILL_DEPENDS_END; id++)
    {
        if (!CharaSkillDependsExist(id))
        {
            new_dep.id = id;
            chara_skill_depends.push_back(new_dep);
            return true;
        }
    }

    return false;
}

X2mDepends *X2mFile::AddCharaSkillDepends(const uint8_t *guid, const std::string &name)
{
    if (CharaSkillDependsExist(guid))
        return nullptr;

    X2mDepends new_dep;

    new_dep.type = X2mDependsType::SKILL;
    memcpy(new_dep.guid, guid, 16);
    new_dep.name = name;

    for (uint16_t id = X2M_SKILL_DEPENDS_BEGIN; id < X2M_SKILL_DEPENDS_END; id++)
    {
        if (!CharaSkillDependsExist(id))
        {
            new_dep.id = id;
            chara_skill_depends.push_back(new_dep);
            return &chara_skill_depends.back();
        }
    }

    return nullptr;
}

bool X2mFile::RemoveCharaSkillDepends(const uint8_t *guid)
{
    for (size_t i = 0; i < chara_skill_depends.size(); i++)
    {
        if (memcmp(chara_skill_depends[i].guid, guid, 16) == 0)
        {
            chara_skill_depends.erase(chara_skill_depends.begin()+i);
            return true;
        }
    }

    return true; // Yes true
}

bool X2mFile::RemoveCharaSkillDepends(const std::string &guid)
{
    uint8_t guid_bin[16];

    if (!Utils::String2GUID(guid_bin, guid))
        return false;

    return RemoveCharaSkillDepends(guid_bin);
}

bool X2mFile::CharaSkillDependsHasAttachment(size_t idx) const
{
    if (idx >= chara_skill_depends.size())
        return false;

    const std::string att_path = X2M_CHARA_SKILLS_ATTACHMENTS + Utils::GUID2String(chara_skill_depends[idx].guid) + ".x2m";
    return FileExists(att_path);
}

bool X2mFile::CharaSkillDependsHasAttachment(const uint8_t *guid) const
{
    for (size_t i = 0; i < chara_skill_depends.size(); i++)
    {
        if (memcmp(chara_skill_depends[i].guid, guid, 16) == 0)
        {
            return CharaSkillDependsHasAttachment(i);
        }
    }

    return false;
}

bool X2mFile::CharaSkillDependsHasAttachment(const std::string &guid) const
{
    uint8_t guid_bin[16];

    if (!Utils::String2GUID(guid_bin, guid))
        return false;

    return CharaSkillDependsHasAttachment(guid_bin);
}

bool X2mFile::SetCharaSkillDependsAttachment(size_t idx, X2mFile *attachment)
{
    if (idx >= chara_skill_depends.size())
        return false;

    if (attachment->GetType() != X2mType::NEW_SKILL)
        return false;

    X2mDepends &dep = chara_skill_depends[idx];

    if (memcmp(dep.guid, attachment->mod_guid, 16) != 0)
        return false;

    size_t size;
    uint8_t *buf = attachment->Save(&size);

    if (!buf)
        return false;

    const std::string att_path = X2M_CHARA_SKILLS_ATTACHMENTS + Utils::GUID2String(dep.guid) + ".x2m";
    bool ret = WriteFile(att_path, buf, size);
    delete[] buf;

    return ret;
}

bool X2mFile::SetCharaSkillDependsAttachment(X2mFile *attachment)
{
    for (size_t i = 0; i < chara_skill_depends.size(); i++)
    {
        if (memcmp(chara_skill_depends[i].guid, attachment->mod_guid, 16) == 0)
        {
            return SetCharaSkillDependsAttachment(i, attachment);
        }
    }

    return false;
}

bool X2mFile::RemoveCharaSkillDependsAttachment(const uint8_t *guid)
{
    for (const X2mDepends &dep : chara_skill_depends)
    {
        if (memcmp(dep.guid, guid, 16) == 0)
        {
            const std::string att_path = X2M_CHARA_SKILLS_ATTACHMENTS + Utils::GUID2String(dep.guid) + ".x2m";
            return RemoveFile(att_path);
        }
    }

    return false;
}

bool X2mFile::RemoveCharaSkillDependsAttachment(const std::string &guid)
{
    uint8_t guid_bin[16];

    if (!Utils::String2GUID(guid_bin, guid))
        return false;

    return RemoveCharaSkillDependsAttachment(guid_bin);
}

X2mFile *X2mFile::LoadCharaSkillDependsAttachment(const uint8_t *guid)
{
    if (!CharaSkillDependsHasAttachment(guid))
        return nullptr;

    const std::string att_path = X2M_CHARA_SKILLS_ATTACHMENTS + Utils::GUID2String(guid) + ".x2m";
    size_t size;
    uint8_t *buf = ReadFile(att_path, &size);
    if (!buf)
        return nullptr;

    X2mFile *ret = new X2mFile();
    if (!ret->Load(buf, size))
    {
        delete[] buf;
        delete ret;
        return nullptr;
    }

    if (ret->type != X2mType::NEW_SKILL)
    {
        DPRINTF("%s: Some retard has specified as skill attachment a mod that is not a skill.\n", FUNCNAME);
        return nullptr;
    }

    return ret;
}

X2mFile *X2mFile::LoadCharaSkillDependsAttachment(const std::string &guid)
{
    uint8_t guid_bin[16];

    if (!Utils::String2GUID(guid_bin, guid))
        return nullptr;

    return LoadCharaSkillDependsAttachment(guid_bin);
}

bool X2mFile::IsCharaSkillDependsReferenced(const X2mDepends &depends) const
{
    for (const CusSkillSet &skill_set : skill_sets)
    {
        for (int i = 0; i < 9; i++)
        {
            if (skill_set.char_skills[i] == depends.id)
                return true;
        }
    }

    return false;
}

size_t X2mFile::GetNumCharaAuras(bool install_mode) const
{
    if (!install_mode)
        return chara_auras.size();

    if (format_version < X2M_MIN_VERSION_AURAS || chara_auras.size() != aur_entries.size())
        return 0;

    size_t num = 0;

    for (size_t i = 0; i < aur_entries.size(); i++)
    {
        if (aur_entries[i].aura_id == X2M_INVALID_ID)
        {
            num++;
        }
    }

    return num;
}

size_t X2mFile::GetNumSkillCustomAuras() const
{
    if (!HasSkillAura())
        return 0;

    size_t num = 0;

    for (const X2mSkillAura &aura : skill_aura_entries)
    {
        if (aura.data.aur_aura_id == X2M_INVALID_ID16)
            num++;
    }

    return num;
}

CusSkill *X2mFile::FindInstalledSkill(const uint8_t *guid, X2mSkillType type)
{

    std::vector<CmsEntryXV2 *> entries;
    FindX2mSkillCMS(entries);

    for (CmsEntryXV2 *entry : entries)
    {
        for (int i = 0; i < 10; i++)
        {
            uint16_t id = (entry->id*10) + i;
            id = IdFromId2(id, type);

            CusSkill *skill = game_cus->FindSkillAnyByID(id);
            if (!skill)
                continue;

            const std::string skill_path = GetSkillDirectory(*skill, type);

            if (skill_path.length() != 0)
            {
                IniFile ini;

                if (xv2fs->LoadFile(&ini, skill_path + X2M_SKILL_INI))
                {
                    uint8_t this_guid[16];
                    std::string guid_str;

                    if (ini.GetStringValue("General", "GUID", guid_str))
                    {
                        if (Utils::String2GUID(this_guid, guid_str) && memcmp(this_guid, guid, sizeof(this_guid)) == 0)
                        {
                            return skill;
                        }
                    }
                }
            }
        }
    }

    return nullptr;
}

bool X2mFile::FindInstalledSkill()
{
    if (type != X2mType::NEW_SKILL)
        return false;

    if (skill_entry.id != X2M_DUMMY_ID16 || skill_entry.id2 != X2M_DUMMY_ID16)
    {
        DPRINTF("%s: Internal error, this function should not be called after the skill got an id assigned.\n", FUNCNAME);
        return false;
    }

    std::vector<CmsEntryXV2 *> entries;
    FindX2mSkillCMS(entries);

    for (CmsEntryXV2 *entry : entries)
    {
        for (int i = 0; i < 10; i++)
        {
            uint16_t id = (entry->id*10) + i;
            id = IdFromId2(id);

            const CusSkill *skill = game_cus->FindSkillAnyByID(id);
            if (!skill)
                continue;

            if (skill->name == skill_entry.name)
            {
                skill_entry.id2 = skill->id2;
                const std::string skill_path = GetSkillDirectory();

                if (skill_path.length() != 0)
                {
                    IniFile ini;

                    if (xv2fs->LoadFile(&ini, skill_path + X2M_SKILL_INI))
                    {
                        uint8_t guid[16];
                        std::string guid_str;

                        if (ini.GetStringValue("General", "GUID", guid_str))
                        {
                            if (Utils::String2GUID(guid, guid_str) && memcmp(guid, mod_guid, sizeof(mod_guid)) == 0)
                            {
                                // Found
                                skill_entry.id = skill->id;

                                temp_pup_in.clear();
                                temp_cus_aur_in.clear();
                                temp_aur_in.clear();

                                ini.GetMultipleIntegersValues("General", "PUP", temp_pup_in, true);
                                ini.GetMultipleIntegersValues("General", "CUS_AUR", temp_cus_aur_in, true);
                                ini.GetMultipleIntegersValues("General", "AUR", temp_aur_in, true);

                                return true;
                            }
                        }
                    }
                }

                skill_entry.id2 = X2M_DUMMY_ID16; // Revert
            }
        }
    }

    return false;
}

bool X2mFile::IsSkillCostumeDepends(const uint8_t *guid) const
{
    return (memcmp(skill_costume_depend.guid, guid, 16) == 0);
}

bool X2mFile::IsSkillCostumeDepends(const std::string &guid) const
{
    uint8_t guid_bin[16];

    if (!Utils::String2GUID(guid_bin, guid))
        return false;

    return IsSkillCostumeDepends(guid_bin);
}

bool X2mFile::IsSkillCostumeDepends(uint16_t id) const
{
    return (skill_costume_depend.id == id);
}

bool X2mFile::SetSkillCostumeDepend(const X2mDepends &depends)
{
    skill_costume_depend = depends;
    return true;
}

bool X2mFile::SetSkillCostumeDepend(X2mFile *costume_x2m)
{
    if (costume_x2m->GetType() != X2mType::NEW_COSTUME)
        return false;

    skill_costume_depend.type = X2mDependsType::COSTUME;
    memcpy(skill_costume_depend.guid, costume_x2m->mod_guid, 16);
    skill_costume_depend.name = costume_x2m->GetModName();
    skill_costume_depend.id = X2M_COSTUME_DEPENDS_ID;

    return true;
}

X2mDepends *X2mFile::SetSkillCostumeDepend(const uint8_t *guid, const std::string &name)
{
    skill_costume_depend.type = X2mDependsType::COSTUME;
    memcpy(skill_costume_depend.guid, guid, 16);
    skill_costume_depend.name = name;
    skill_costume_depend.id = X2M_COSTUME_DEPENDS_ID;

    return nullptr;
}

bool X2mFile::SkillCostumeDependHasAttachment() const
{
    const std::string att_path = X2M_SKILLS_COSTUME_ATTACHMENTS + Utils::GUID2String(skill_costume_depend.guid) + ".x2m";
    return FileExists(att_path);
}

bool X2mFile::SetSkillCostumeDependAttachment(X2mFile *attachment)
{
    if (attachment->type != X2mType::NEW_COSTUME)
        return false;

    if (memcmp(skill_costume_depend.guid, attachment->mod_guid, 16) != 0)
        return false;

    size_t size;
    uint8_t *buf = attachment->Save(&size);

    if (!buf)
        return false;

    const std::string att_path = X2M_SKILLS_COSTUME_ATTACHMENTS + Utils::GUID2String(skill_costume_depend.guid) + ".x2m";
    bool ret = WriteFile(att_path, buf, size);
    delete[] buf;

    return ret;
}

bool X2mFile::RemoveSkillCostumeDependAttachment()
{
    const std::string att_path = X2M_SKILLS_COSTUME_ATTACHMENTS + Utils::GUID2String(skill_costume_depend.guid) + ".x2m";
    return RemoveFile(att_path);
}

X2mFile *X2mFile::LoadSkillCostumeDependAttachment()
{
    if (!HasSkillCostumeDepend())
        return nullptr;

    const std::string att_path = X2M_SKILLS_COSTUME_ATTACHMENTS + Utils::GUID2String(skill_costume_depend.guid) + ".x2m";
    size_t size;
    uint8_t *buf = ReadFile(att_path, &size);
    if (!buf)
        return nullptr;

    X2mFile *ret = new X2mFile();
    if (!ret->Load(buf, size))
    {
        delete[] buf;
        delete ret;
        return nullptr;
    }

    if (ret->type != X2mType::NEW_COSTUME)
    {
        DPRINTF("%s: Some retard has specified as costume attachment a mod that is not a costume.\n", FUNCNAME);
        return nullptr;
    }

    return ret;
}

bool X2mFile::IsSkillCostumeDependReferenced() const
{
    return (skill_entry.partset == skill_costume_depend.id);
}

size_t X2mFile::GetNumCostumePartSets(uint8_t race) const
{
    size_t count = 0;

    for (const X2mPartSet &set : costume_partsets)
    {
        if (set.race == race)
            count++;
    }

    return count;
}

size_t X2mFile::GetEffectiveNumCostumePartSets() const
{
    for (int i = 0; i < X2M_CR_NUM; i++)
    {
        size_t count = GetNumCostumePartSets(i);
        if (count != 0)
            return count;
    }

    return 0;
}

size_t X2mFile::GetNumCostumeUsedRaces() const
{
    size_t count = 0;

    for (int i = 0; i < X2M_CR_NUM; i++)
    {
        if (GetNumCostumePartSets(i) != 0)
            count++;
    }

    return count;
}

const X2mPartSet &X2mFile::GetCostumePartSet(uint8_t race, size_t idx) const
{
    size_t i = 0;

    for (const X2mPartSet &set : costume_partsets)
    {
        if (set.race == race)
        {
            if (i == idx)
            {
                return set;
            }

            i++;
        }
    }

    DPRINTF("%s: Fatal error: Index out of bounds. idx=%Id, race=%d\n", FUNCNAME, idx, race);
    exit(-1); // Todo: replace with exception

    // Make the compiler hapy

    X2mPartSet *ret = new X2mPartSet();
    return *ret;
}

X2mPartSet &X2mFile::GetCostumePartSet(uint8_t race, size_t idx)
{
    size_t i = 0;

    for (X2mPartSet &set : costume_partsets)
    {
        if (set.race == race)
        {
            if (i == idx)
            {
                return set;
            }

            i++;
        }
    }

    DPRINTF("%s: Fatal error: Index out of bounds. idx=%Id, race=%d\n", FUNCNAME, idx, race);
    exit(-1); // Todo: replace with exception

    // Make the compiler hapy

    X2mPartSet *ret = new X2mPartSet();
    return *ret;
}

void X2mFile::ClearCostumePartSets(uint8_t race)
{
    for (size_t i = 0; i < costume_partsets.size(); i++)
    {
        if (costume_partsets[i].race == race)
        {
            costume_partsets.erase(costume_partsets.begin()+i);
            i--;
        }
    }
}

X2mCostumeEntry *X2mFile::FindInstalledCostume()
{
    X2mCostumeEntry *costume = game_costume_file->FindCostume(mod_guid);
    if (costume)
    {
        update_costume_entry = *costume; // Copy
        return &update_costume_entry;
    }

    update_costume_entry = X2mCostumeEntry(); // Reset
    return nullptr;
}

X2mCostumeEntry *X2mFile::FindInstalledCostume(const uint8_t *guid)
{
    return game_costume_file->FindCostume(guid);
}

bool X2mFile::InstallCharaName()
{
    if (type != X2mType::NEW_CHARACTER)
        return false;

    for (int i = 0; i < XV2_LANG_NUM; i++)
    {
        std::string *name;

        Xenoverse2::RemoveAllCharaName(entry_name, i);

        if (i == XV2_LANG_ENGLISH)
        {
            if (chara_name[i].length() == 0)
                return false;

            name = &chara_name[i];
        }
        else
        {
            if (chara_name[i].length() == 0)
            {
                name = &chara_name[XV2_LANG_ENGLISH];
            }
            else
            {
                name = &chara_name[i];
            }
        }

        if (!Xenoverse2::SetCharaName(entry_name, *name, i))
            return false;

        if (HasMultNames())
        {
            for (size_t j = 0; j < slot_entries.size(); j++)
            {
                std::string specific_name = *name;
                const X2mSlotEntry &slot = slot_entries[j];

                for (size_t k = 0; k < mult_chara_names.size(); k++)
                {
                    const X2mCharaName &m_chara_name = mult_chara_names[k];

                    if (m_chara_name.costume_index == (uint32_t)slot.costume_index)
                    {
                        if (i == XV2_LANG_ENGLISH)
                        {
                            if (m_chara_name.chara_name[i].length() == 0)
                                return false;

                            specific_name = m_chara_name.chara_name[i];
                        }
                        else
                        {
                            if (m_chara_name.chara_name[i].length() == 0)
                            {
                                specific_name = m_chara_name.chara_name[XV2_LANG_ENGLISH];
                            }
                            else
                            {
                                specific_name = m_chara_name.chara_name[i];
                            }
                        }

                        break;
                    }
                } // end k loop

                if (!Xenoverse2::SetCharaName(entry_name, specific_name, i, (int)j))
                    return false;
            }
        }
    }

    return true;
}

bool X2mFile::InstallCostumeNames()
{
    if (type != X2mType::NEW_CHARACTER)
        return false;

    if (slot_entries.size() == 0)
        return false;

    for (size_t i = 0; i < slot_entries.size(); i++)
    {
        const X2mSlotEntry &entry = slot_entries[i];

        for (int j = 0; j < XV2_LANG_NUM; j++)
        {
            const std::string *name;

            if (j == XV2_LANG_ENGLISH)
            {
                if (entry.costume_name[j].length() == 0)
                    return false;

                name = &entry.costume_name[j];
            }
            else
            {
                if (entry.costume_name[j].length() == 0)
                {
                    name = &entry.costume_name[XV2_LANG_ENGLISH];
                }
                else
                {
                    name = &entry.costume_name[j];
                }
            }

            if (!Xenoverse2::SetCharaCostumeName(entry_name, entry.costume_index, entry.model_preset, *name, j))
                return false;
        }

    }

    return true;
}

void X2mFile::UninstallCssAudio()
{
    for (int i = 0; i < 100; i++)
    {
        const std::string name = std::string("X2M_") + entry_name + "_" + Utils::ToStringAndPad(i, 2);
        Xenoverse2::FreeCssSound(name, false);
        Xenoverse2::FreeCssSound(name, true);
    }
}

bool X2mFile::InstallCssAudio(X2mSlotEntry &entry)
{
    if (format_version < X2M_MIN_VERSION_CSS_AUDIO)
        return true;

    for (int i = 0 ; i < 2; i++)
    {
        if (entry.voices_id_list[i] != (int)X2M_DUMMY_ID)
            continue;

        if (entry.audio_files[i].length() == 0)
        {
            DPRINTF("%s: Internal error, audio file path is empty and voices id list is dummy.\n", FUNCNAME);
            return false;
        }

        auto it = std::find(installed_css_audio.begin(), installed_css_audio.end(), entry.audio_files[i]);
        if (it != installed_css_audio.end())
        {
            size_t index = it - installed_css_audio.begin();
            entry.voices_id_list[i] = installed_css_cue[index];
            continue;
        }

        if (installed_css_audio.size() == 100)
        {
            DPRINTF("%s: Audio overflow, 100 or more audio entries in the mod.\n", FUNCNAME);
            return false;
        }

        if (!CssAudioFileExists(entry.audio_files[i]))
        {
            DPRINTF("%s: Internal error, audio file with base name \"%s\" doesn't exist in package.\n", FUNCNAME, entry.audio_files[i].c_str());
            return false;
        }

        HcaFile *hca_jp = LoadCssAudioFile(entry.audio_files[i], false, true);
        if (!hca_jp)
        {
            DPRINTF("%s: Cannot load css audio sound \"%s\". Most likely reasons is that it is an invalid HCA file.\n", FUNCNAME, entry.audio_files[i].c_str());
            return false;
        }

        HcaFile *hca_en = LoadCssAudioFile(entry.audio_files[i], true, true);
        if (!hca_en)
        {
            DPRINTF("%s: Cannot load css audio sound \"%s\" (2). Most likely reasons is that it is an invalid HCA file.\n", FUNCNAME, entry.audio_files[i].c_str());
            delete hca_jp;
            return false;
        }

        uint32_t cue_id_jp, cue_id_en;
        const std::string cue_name = "X2M_" + entry_name + "_" + Utils::ToStringAndPad((int)installed_css_audio.size(), 2);

        cue_id_jp = Xenoverse2::SetCssSound(cue_name, false, *hca_jp);
        delete hca_jp;

        if (cue_id_jp == (uint32_t)-1)
        {
            DPRINTF("%s: SetCssSound failed when trying to set japanese track.\n", FUNCNAME);
            delete hca_en;
            return false;
        }

        cue_id_en = Xenoverse2::SetCssSound(cue_name, true, *hca_en);
        delete hca_en;

        if (cue_id_en == (uint32_t)-1)
        {
            DPRINTF("%s: SetCssSound failed when trying to set english track.\n", FUNCNAME);
            return false;
        }

        if (cue_id_jp != cue_id_en)
        {
            DPRINTF("%s: English and Japanese CRT_CS_vox.acb are unsynchronized and cannot assign a common cue_id.\n\nPossible reasons:\n"
                    "- One of them was deleted by user\n."
                    "- One of them was edited by the user to add a track and the other one wasn't.\n"
                    "- One of them was set by the user to a previous version and the other wasn't.\n", FUNCNAME);
            return false;
        }

        entry.voices_id_list[i] = cue_id_jp;
        installed_css_audio.push_back(entry.audio_files[i]);
        installed_css_cue.push_back(cue_id_jp);
    }

    return true;
}

size_t X2mFile::FindX2mSkillCMS(std::vector<CmsEntryXV2 *> &entries)
{
    entries.clear();

    for (CmsEntry *entry : *game_cms)
    {
        CmsEntryXV2 *entry_xv2 = dynamic_cast<CmsEntryXV2 *>(entry);
        if (!entry_xv2)
            continue;

        if (entry_xv2->unk_10 == X2M_DUMMY_ID)
            entries.push_back(entry_xv2);
    }

    return entries.size();
}

uint16_t X2mFile::IdFromId2(uint16_t id2) const
{
    if (skill_type == X2mSkillType::SUPER)
    {
        return id2 + CUS_SUPER_START;
    }
    else if (skill_type == X2mSkillType::ULTIMATE)
    {
        return id2 + CUS_ULTIMATE_START;
    }
    else if (skill_type == X2mSkillType::EVASIVE)
    {
        return id2 + CUS_EVASIVE_START;
    }
    else if (skill_type == X2mSkillType::BLAST)
    {
        return id2 + CUS_BLAST_START;
    }
    else if (skill_type == X2mSkillType::AWAKEN)
    {
        return id2 + CUS_AWAKEN_START;
    }

    DPRINTF("%s: Internal error.\n", FUNCNAME);
    exit(-1);

    return 0xFFFF;
}

uint16_t X2mFile::IdFromId2(uint16_t id2, X2mSkillType skill_type)
{
    if (skill_type == X2mSkillType::SUPER)
    {
        return id2 + CUS_SUPER_START;
    }
    else if (skill_type == X2mSkillType::ULTIMATE)
    {
        return id2 + CUS_ULTIMATE_START;
    }
    else if (skill_type == X2mSkillType::EVASIVE)
    {
        return id2 + CUS_EVASIVE_START;
    }
    else if (skill_type == X2mSkillType::BLAST)
    {
        return id2 + CUS_BLAST_START;
    }
    else if (skill_type == X2mSkillType::AWAKEN)
    {
        return id2 + CUS_AWAKEN_START;
    }

    DPRINTF("%s: Internal error.\n", FUNCNAME);
    exit(-1);

    return 0xFFFF;
}

bool X2mFile::AssignSkillIds()
{
    if (type != X2mType::NEW_SKILL)
    {
        DPRINTF("%s: Internal error, this function must only be called for skill x2m.\n", FUNCNAME);
        return false;
    }

    std::vector<CmsEntryXV2 *> entries;
    FindX2mSkillCMS(entries);

    int type;

    if (skill_type == X2mSkillType::SUPER)
    {
        type = CUS_SKILL_TYPE_SUPER;
    }
    else if (skill_type == X2mSkillType::ULTIMATE)
    {
        type = CUS_SKILL_TYPE_ULTIMATE;
    }
    else if (skill_type == X2mSkillType::EVASIVE)
    {
        type = CUS_SKILL_TYPE_EVASIVE;
    }
    else if (skill_type == X2mSkillType::BLAST)
    {
        type = CUS_SKILL_TYPE_BLAST;
    }
    else if (skill_type == X2mSkillType::AWAKEN)
    {
        type = CUS_SKILL_TYPE_AWAKEN;
    }
    else
    {
        DPRINTF("%s Internal error.\n", FUNCNAME);
        return false;
    }

    for (CmsEntryXV2 *entry : entries)
    {
        for (int i = 0; i < 10; i++)
        {
            if (!game_cus->IsSkillInUse(entry->id, i, type))
            {
                skill_entry.id2 = (entry->id*10)+i;
                skill_entry.id = IdFromId2(skill_entry.id2);
                return true;
            }
        }
    }

    CmsEntryXV2 dummy_char;
    bool found = false;

    for (int i = 0; i < 100; i++)
    {
        dummy_char.name = "X";

        for (int j = 1; j < 3; j++)
        {            
            while (1)
            {
                char b;

                Utils::GetRandomData(&b, 1);

                if ((b >= 'A' && b <= 'Z') || (b >= '0' && b <= '9'))
                {
                    dummy_char.name += b;
                    break;
                }
            }
        }

        if (!game_cms->FindEntryByName(dummy_char.name))
        {
            found = true;
            break;
        }
    }

    if (!found)
    {
        // If we didn't find an entry that starts by X, let's find any other entry
        while (1)
        {
            dummy_char.name.clear();

            for (int j = 0; j < 3; j++)
            {
                while (1)
                {
                    char b;

                    Utils::GetRandomData(&b, 1);

                    if ((b >= 'A' && b <= 'Z') || (b >= '0' && b <= '9'))
                    {
                        dummy_char.name += b;
                        break;
                    }
                }
            }

            if (!game_cms->FindEntryByName(dummy_char.name))
                break;
        }
    }

    dummy_char.unk_10 = X2M_DUMMY_ID;
    dummy_char.unk_16 = dummy_char.load_cam_dist = dummy_char.unk_18 = dummy_char.unk_1A = X2M_DUMMY_ID16;

    if (!game_cms->AddEntryXV2(dummy_char, true))
        return false;

    if (dummy_char.id >= 500)
    {
        DPRINTF("%s: Overflow of characters. Cannot get a proper fake character entry for skill.\n"
                "Uninstall some characters, then install this mod, and then reinstall again those characters.\n", FUNCNAME);
        return false;
    }

    skill_entry.id2 = dummy_char.id*10;
    skill_entry.id = IdFromId2(skill_entry.id2);

    return true;
}

std::string X2mFile::GetSkillDirectory() const
{
    if (type != X2mType::NEW_SKILL)
    {
        DPRINTF("%s: This function can ony be called by skills x2m.\n", FUNCNAME);
        return std::string();
    }

    std::string ret = "data/skill/";

    if (skill_type == X2mSkillType::SUPER)
        ret += "SPA/";
    else if (skill_type == X2mSkillType::ULTIMATE)
        ret += "ULT/";
    else if (skill_type == X2mSkillType::EVASIVE)
        ret += "ESC/";
    else if (skill_type == X2mSkillType::BLAST)
        ret += "BLT/";
    else if (skill_type == X2mSkillType::AWAKEN)
        ret += "MET/";
    else
    {
        DPRINTF("%s: Internal error.\n", FUNCNAME);
        return std::string();
    }

    ret += Utils::ToStringAndPad(skill_entry.id2, 3);
    ret += '_';

    CmsEntry *entry = game_cms->FindEntryByID(skill_entry.id2 / 10);
    if (!entry)
        return std::string();

    ret += entry->name;
    ret += '_';
    ret += skill_entry.name;
    ret += '/';

    return ret;
}

std::string X2mFile::GetSkillDirectory(const CusSkill &skill_entry, X2mSkillType skill_type)
{
    std::string ret = "data/skill/";

    if (skill_type == X2mSkillType::SUPER)
        ret += "SPA/";
    else if (skill_type == X2mSkillType::ULTIMATE)
        ret += "ULT/";
    else if (skill_type == X2mSkillType::EVASIVE)
        ret += "ESC/";
    else if (skill_type == X2mSkillType::BLAST)
        ret += "BLT/";
    else if (skill_type == X2mSkillType::AWAKEN)
        ret += "MET/";
    else
    {
        DPRINTF("%s: Internal error.\n", FUNCNAME);
        return std::string();
    }

    ret += Utils::ToStringAndPad(skill_entry.id2, 3);
    ret += '_';

    CmsEntry *entry = game_cms->FindEntryByID(skill_entry.id2 / 10);
    if (!entry)
        return std::string();

    ret += entry->name;
    ret += '_';
    ret += skill_entry.name;
    ret += '/';

    return ret;
}

uint16_t X2mFile::GetCusSkillType() const
{
    if (skill_type == X2mSkillType::SUPER)
    {
        return CUS_SKILL_TYPE_SUPER;
    }
    else if (skill_type == X2mSkillType::ULTIMATE)
    {
        return CUS_SKILL_TYPE_ULTIMATE;
    }
    else if (skill_type == X2mSkillType::EVASIVE)
    {
        return CUS_SKILL_TYPE_EVASIVE;
    }
    else if (skill_type == X2mSkillType::BLAST)
    {
        return CUS_SKILL_TYPE_BLAST;
    }
    else if (skill_type == X2mSkillType::AWAKEN)
    {
        return CUS_SKILL_TYPE_AWAKEN;
    }

    return 0;
}

void X2mFile::AssignSkillIdbIds()
{
    if (!HasSkillIdbEntry())
        return;

    if (skill_entry.id2 == X2M_DUMMY_ID16 || skill_entry.id2 == X2M_INVALID_ID16)
        return;

    skill_idb_entry.id = skill_idb_entry.name_id = skill_idb_entry.desc_id = skill_entry.id2;
    skill_idb_entry.type = GetCusSkillType();
}

bool X2mFile::InstallSlots(bool update)
{
    if (type != X2mType::NEW_CHARACTER)
        return false;

    UninstallCssAudio();
    installed_css_audio.clear();
    installed_css_cue.clear();

    if (update)
    {
        std::vector<CharaListSlotEntry *> l_entries;
        chara_list->FindSlotsByCode(std::string("\"") + entry_name + std::string("\""), l_entries);

        if (l_entries.size() != slot_entries.size())
        {
            DPRINTF("%s: Update mode failed because there are different num of slots in the list than in the mod. Try uninstalling the mod and install it again.\n", FUNCNAME);
            return false;
        }

        for (size_t i = 0; i < l_entries.size(); i++)
        {
            X2mSlotEntry &x_entry = slot_entries[i];

            if (!InstallCssAudio(x_entry))
                return false;

            x_entry.CopyTo(*(l_entries[i]), entry_name);
        }

        return true;
    }

    std::vector<CharaListSlot> &chara_slots = chara_list->GetSlots();

    if (chara_slots.size() >= XV2_MAX_SLOTS)
        return false;

    CharaListSlot slot;

    slot.entries.resize(slot_entries.size());

    for (size_t i = 0; i < slot_entries.size(); i++)
    {
        X2mSlotEntry &x_entry = slot_entries[i];
        CharaListSlotEntry &l_entry = slot.entries[i];

        if (!InstallCssAudio(x_entry))
            return false;

        x_entry.CopyTo(l_entry, entry_name);
    }

    chara_slots.push_back(slot);
    return true;
}

bool X2mFile::InstallSlots(const std::vector<size_t> &positions)
{
    if (type != X2mType::NEW_CHARACTER)
        return false;

    UninstallCssAudio();
    installed_css_audio.clear();
    installed_css_cue.clear();

    if (positions.size() != slot_entries.size())
        return false;

    std::vector<CharaListSlot> &chara_slots = chara_list->GetSlots();

    // Check first
    for (size_t pos : positions)
    {
        if (pos >= chara_slots.size())
            return false;

        if (chara_slots[pos].entries.size() >= XV2_MAX_SUBSLOTS)
            return false;
    }

    for (size_t i = 0; i < slot_entries.size(); i++)
    {
        X2mSlotEntry &x_entry = slot_entries[i];
        CharaListSlotEntry l_entry;

        if (!InstallCssAudio(x_entry))
            return false;

        x_entry.CopyTo(l_entry, entry_name);
        chara_slots[positions[i]].entries.push_back(l_entry);

        if (chara_slots[positions[i]].entries.size() >= XV2_MAX_SUBSLOTS)
            return false;
    }

    return true;
}

bool X2mFile::InstallCms(bool update)
{
    if (type != X2mType::NEW_CHARACTER)
        return false;

    if (cms_entry.id == X2M_INVALID_ID)
        return false;

    cms_entry.name = entry_name;

    if (update)
    {
        CmsEntryXV2 *entry = dynamic_cast<CmsEntryXV2 *>(game_cms->FindEntryByName(cms_entry.name));

        if (!entry)
        {
            DPRINTF("%s: Update failed because cms entry of mod doesn't exist. A fix may be to uninstall mod and install it again.\n", FUNCNAME);
            return false;
        }

        cms_entry.id = entry->id;
        *entry = cms_entry;
        return true;
    }

    return game_cms->AddEntryXV2(cms_entry, true);
}

void X2mFile::AssignCharaCustomSkills(CusSkillSet &x_set)
{
    if (!HasCharaSkillDepends())
        return;

    for (int i = 0; i < 9; i++)
    {
        if (x_set.char_skills[i] >= X2M_SKILL_DEPENDS_BEGIN && x_set.char_skills[i] < X2M_SKILL_DEPENDS_END)
        {
            X2mDepends *dep = FindCharaSkillDepends(x_set.char_skills[i]);

            if (dep)
            {
                X2mSkillType st;

                if (i < 4)
                {
                    st = X2mSkillType::SUPER;
                }
                else if (i < 6)
                {
                    st = X2mSkillType::ULTIMATE;
                }
                else if (i == 6)
                {
                    st = X2mSkillType::EVASIVE;
                }
                else if (i == 7)
                {
                    st = X2mSkillType::BLAST;
                }
                else
                {
                    st = X2mSkillType::AWAKEN;
                }

                CusSkill *skill = FindInstalledSkill(dep->guid,st);
                if (skill)
                {
                    x_set.char_skills[i] = skill->id;
                    continue;
                }
            }

            x_set.char_skills[i] = 0xFFFF; // If not found, unassign skill
        }
    }
}

bool X2mFile::InstallCus(bool update)
{
    if (cms_entry.id == X2M_DUMMY_ID || cms_entry.id == X2M_INVALID_ID)
    {
        DPRINTF("%s: Internal error. InstallCms must be called before this!!!\n", FUNCNAME);
        return false;
    }

    if (skill_sets.size() != slot_entries.size())
    {
        DPRINTF("%s: Internal error.\n", FUNCNAME);
        return false;
    }

    if (update)
    {
        std::vector<CusSkillSet *> sets;

        game_cus->FindSkillSetsByCharID(cms_entry.id, sets);

        if (sets.size() != skill_sets.size())
        {
            DPRINTF("%s: Update mode failed because there are different num of skillset in the cus than in the mod (for this char). Try uninstalling the mod and install it again.\n", FUNCNAME);
            return false;
        }

        for (size_t i = 0; i < sets.size(); i++)
        {
            CusSkillSet &x_set = skill_sets[i];

            x_set.char_id = cms_entry.id;
            x_set.costume_id = slot_entries[i].costume_index;
            x_set.model_preset = slot_entries[i].model_preset;
            AssignCharaCustomSkills(x_set);

            *(sets[i]) = x_set;
        }

        return true;
    }

    // New install

    for (size_t i = 0; i < skill_sets.size(); i++)
    {
        CusSkillSet &x_set = skill_sets[i];

        x_set.char_id = cms_entry.id;
        x_set.costume_id = slot_entries[i].costume_index;
        x_set.model_preset = slot_entries[i].model_preset;
        AssignCharaCustomSkills(x_set);

        if (!game_cus->AddSkillSet(x_set, (i == 0)))
        {
            DPRINTF("%s: AddSkillSet %d  failed. You probably had a dead id in the cus file, caused by manual editing or a replacer mod.\n", FUNCNAME, i);
            return false;
        }
    }

    return true;
}

bool X2mFile::FillCsoEntries()
{
    std::unordered_set<int32_t> costumes;
    size_t i = 0;

    for (const X2mSlotEntry &slot : slot_entries)
    {
        if (costumes.find(slot.costume_index) == costumes.end())
        {
            costumes.insert(slot.costume_index);

            if (i >= cso_entries.size())
                return false;

            cso_entries[i].char_id = cms_entry.id;
            cso_entries[i].costume_id = slot.costume_index;
            i++;
        }
    }

    return (costumes.size() == cso_entries.size());
}

bool X2mFile::InstallCso(bool update)
{
    if (cms_entry.id == X2M_DUMMY_ID || cms_entry.id == X2M_INVALID_ID)
    {
        DPRINTF("%s: Internal error. InstallCms must be called before this!!!\n", FUNCNAME);
        return false;
    }

    std::vector<CsoEntry *> entries;
    game_cso->FindEntriesByCharID(cms_entry.id, entries);

    if (!HasCso())
    {
        if (entries.size() != 0)
        {
            DPRINTF("%s: Update mode failed because there were cso entry(es) in the cso, but 0 here. Uninstall the mod and reinstall it will fix this.\n", FUNCNAME);
            return false;
        }

        return true; // Yes, true!
    }

    if (cso_entries.size() != GetNumCostumes())
    {
        DPRINTF("%s: Internal error.\n", FUNCNAME);
        return false;
    }

    if (update)
    {
        if (entries.size() == 0)
        {
            goto regular_install;
        }
        else if (entries.size() != cso_entries.size())
        {
            DPRINTF("%s: Update mode failed because there are different num of cso entries in the cso than in the mod (for this char). Try uninstalling the mod and install it again.\n", FUNCNAME);
            return false;
        }

        if (!FillCsoEntries())
        {
            DPRINTF("%s: FillCsoEntries failed.\n", FUNCNAME);
            return false;
        }

        for (size_t i = 0; i < cso_entries.size(); i++)
        {
            const CsoEntry &entry = cso_entries[i];
            *(entries[i]) = entry;
        }

        return true;
    }

regular_install:

    if (!FillCsoEntries())
    {
        DPRINTF("%s: FillCsoEntries failed.\n", FUNCNAME);
        return false;
    }

    for (size_t i = 0; i < cso_entries.size(); i++)
    {
        const CsoEntry &entry = cso_entries[i];

        if (!game_cso->AddEntry(entry, (i == 0)))
            return false;
    }

    return true;
}

bool X2mFile::FillPscEntries()
{
    std::unordered_set<int32_t> costumes;
    size_t i = 0;

    for (const X2mSlotEntry &slot : slot_entries)
    {
        if (costumes.find(slot.costume_index) == costumes.end())
        {
            costumes.insert(slot.costume_index);

            if (i >= psc_entries.size())
                return false;

            psc_entries[i].costume_id = psc_entries[i].costume_id2 = slot.costume_index;
            i++;
        }
    }

    return (costumes.size() == psc_entries.size());
}

bool X2mFile::InstallPsc()
{
    if (cms_entry.id == X2M_DUMMY_ID || cms_entry.id == X2M_INVALID_ID)
    {
        DPRINTF("%s: Internal error. InstallCms must be called before this!!!\n", FUNCNAME);
        return false;
    }

    PscEntry *entry;
    PscEntry new_entry;
    bool add_entry;

    entry = game_psc->FindEntry(cms_entry.id);

    if (!HasPsc())
    {
        if (entry)
        {
            DPRINTF("%s: Update mode failed because there was a psc entry in the psc, but 0 here. Uninstalling the mod and reinstall it will fix this.\n", FUNCNAME);
            return false;
        }

        return true; // Yes, true!
    }

    if (entry)
    {
        add_entry = false;
    }
    else
    {
        add_entry = true;
        entry = &new_entry;
    }

    entry->char_id = cms_entry.id;
    entry->specs.clear();

    if (!FillPscEntries())
    {
        DPRINTF("%s: FillPscEntries failed.\n", FUNCNAME);
        return false;
    }

    for (const PscSpecEntry &spec : psc_entries)
    {
        entry->specs.push_back(spec);
    }

    if (add_entry)
    {
        game_psc->AddEntry(new_entry);
    }

    return true;
}

void X2mFile::GetInstalledCustomAuraChara()
{
    temp_aur_in.clear();

    IniFile ini;

    if (!xv2fs->LoadFile(&ini, "data/chara/" + entry_name + "/" + X2M_CHARA_INI))
        return;

    ini.GetMultipleIntegersValues("General", "AUR", temp_aur_in, true);
}

bool X2mFile::InstallCustomAuraChara()
{
    GetInstalledCustomAuraChara();
    temp_aur_out.clear();

    size_t num_custom = GetNumCharaAuras(true);

    if (temp_aur_in.size() == num_custom)
    {
        if (num_custom == 0)
            return true;

        // Case of "perfect update"
        std::vector<AurAura *> update_entries;

        for (int32_t aur_id : temp_aur_in)
        {
            AurAura *aura = game_aur->FindAuraByID((uint32_t)aur_id);
            if (!aura)
                break;

            update_entries.push_back(aura);
        }

        if (update_entries.size() != num_custom)
            goto new_install; // Some of the ids were not found, remove and do new install

        for (size_t i = 0, j = 0; i < aur_entries.size(); i++)
        {
            if (j >= update_entries.size())
            {
                DPRINTF("%s: Internal coding error.\n", FUNCNAME);
                return false;
            }

            if (aur_entries[i].aura_id != X2M_INVALID_ID)
                continue;

            AurAura *update_aura = update_entries[j];
            AurAura &aura = chara_auras[i];

            aura.id = update_aura->id;
            aur_entries[i].aura_id = aura.id;
            *update_aura = aura;

            temp_aur_out.push_back((int32_t)aura.id);
            j++;
        }

        goto setup_aura;
    }

new_install:

    // Case of new install or update where old and new have different amount of custom aura entries

    // Remove first existing entries
    for (int i = (int) temp_aur_in.size()-1; i >= 0; i--)
    {
        game_aur->RemoveAuraIfLast((uint32_t)temp_aur_in[i]);
    }

    if (!HasCharaAura(true))
        return true;

    for (size_t i = 0; i < chara_auras.size(); i++)
    {
        if (aur_entries[i].aura_id != X2M_INVALID_ID)
            continue;

        AurAura &aura = chara_auras[i];

        if (!game_aur->AddAura(aura))
            return false;

        aur_entries[i].aura_id = aura.id;
        temp_aur_out.push_back((int32_t)aura.id);
    }

setup_aura:

    // Nothing to do, setup happened before
    return true;
}

bool X2mFile::FillAurEntries()
{
    std::unordered_set<int32_t> costumes;
    size_t i = 0;

    for (const X2mSlotEntry &slot : slot_entries)
    {
        if (costumes.find(slot.costume_index) == costumes.end())
        {
            costumes.insert(slot.costume_index);

            if (i >= aur_entries.size())
                return false;

            aur_entries[i].char_id = cms_entry.id;
            aur_entries[i].costume_id = slot.costume_index;
            i++;
        }
    }

    return (costumes.size() == aur_entries.size());
}

bool X2mFile::InstallAur(bool update)
{
    if (cms_entry.id == X2M_DUMMY_ID || cms_entry.id == X2M_INVALID_ID)
    {
        DPRINTF("%s: Internal error. InstallCms must be called before this!!!\n", FUNCNAME);
        return false;
    }

    if (!InstallCustomAuraChara())
        return false;

    std::vector<AurCharaLink *> entries;
    game_aur->FindCharaLinks(cms_entry.id, entries);

    if (!HasAur())
    {
        if (entries.size() != 0)
        {
            DPRINTF("%s: Update mode failed because there were aur entry(es) in the aur, but 0 here. Uninstalling the mod and reinstall it will fix this.\n", FUNCNAME);
            return false;
        }

        return true; // Yes, true!
    }

    if (aur_entries.size() != GetNumCostumes())
    {
        DPRINTF("%s: Internal error.\n", FUNCNAME);
        return false;
    }

    if (update)
    {
        if (entries.size() == 0)
        {
            goto regular_install;
        }
        else if (entries.size() != aur_entries.size())
        {
            DPRINTF("%s: Update mode failed because there are different num of aur entries in the aur than in the mod (for this char). Try uninstalling the mod and install it again.\n", FUNCNAME);
            return false;
        }

        if (!FillAurEntries())
        {
            DPRINTF("%s: FillAurEntries failed.\n", FUNCNAME);
            return false;
        }

        for (size_t i = 0; i < aur_entries.size(); i++)
        {
            const AurCharaLink &entry = aur_entries[i];
            *(entries[i]) = entry;
        }

        return true;
    }

regular_install:

    if (!FillAurEntries())
    {
        DPRINTF("%s: FillAurEntries failed.\n", FUNCNAME);
        return false;
    }

    for (size_t i = 0; i < aur_entries.size(); i++)
    {
        const AurCharaLink &entry = aur_entries[i];

        if (!game_aur->AddCharaLink(entry, (i == 0)))
            return false;
    }

    return true;
}

void X2mFile::RebuildSevHL(const std::vector<SevTableEntryHL> &hl_table)
{
    if (hl_table.size() == 0)
        return;    

    for (const SevTableEntryHL &hl_entry : hl_table)
    {
        CmsEntry *cms_entry = game_cms->FindEntryByName(hl_entry.char_id);
        if (!cms_entry)
            continue;

        game_sev->RemoveAllReferencesToChar(cms_entry->id);
    }

    std::vector<SevEntry> &sev_entries = game_sev->GetEntries();    

    for (size_t i = 0; i < sev_entries.size(); i++)
    {
        const SevEntry &sev_entry = sev_entries[i];
        CmsEntry *copy_cms_entry = game_cms->FindEntryByID(sev_entry.char_id);

        if (!copy_cms_entry)
            continue;

        for (const SevTableEntryHL &hl_entry : hl_table)
        {
            if (hl_entry.copy_char == copy_cms_entry->name && hl_entry.copy_costume == sev_entry.costume_id)
            {
                CmsEntry *cms_entry = game_cms->FindEntryByName(hl_entry.char_id);
                if (!cms_entry)
                    continue;

                SevEntry new_sev_entry = sev_entry;
                new_sev_entry.char_id = cms_entry->id;
                new_sev_entry.costume_id = hl_entry.costume_id;

                sev_entries.push_back(new_sev_entry);
            }
        }
    }

    for (size_t s = 0; s < sev_entries.size(); s++)
    {
        SevEntry sev_entry = sev_entries[s]; // Copy
        bool commit = false;

        for (size_t i = 0; i < sev_entry.chars_events.size(); i++)
        {
            const SevCharEvents cevents = sev_entry.chars_events[i]; // copy
            CmsEntry *copy_cms_entry = game_cms->FindEntryByID(cevents.char_id);

            if (!copy_cms_entry)
                continue;

            for (const SevTableEntryHL &hl_entry : hl_table)
            {
                if (hl_entry.copy_char == copy_cms_entry->name && hl_entry.copy_costume == cevents.costume_id)
                {
                    CmsEntry *cms_entry = game_cms->FindEntryByName(hl_entry.char_id);
                    if (!cms_entry)
                        continue;

                    if (cms_entry->id == copy_cms_entry->id)
                    {
                        //DPRINTF("Infinite case\n");
                        continue;
                    }

                    SevCharEvents new_cevents = cevents;
                    new_cevents.char_id = cms_entry->id;
                    new_cevents.costume_id = hl_entry.costume_id;

                    sev_entry.chars_events.push_back(new_cevents);
                    commit = true;
                }
            }
        }

        if (commit)
        {
            sev_entries[s] = sev_entry; // Commit
            //UPRINTF("Commit 0x%Ix\n", s);
        }
    }
}

void X2mFile::RebuildSevLL(const std::vector<SevTableEntryLL> &ll_table)
{
    if (ll_table.size() == 0)
        return;

    for (const SevTableEntryLL &ll_entry : ll_table)
    {
        CmsEntry *cms_entry = game_cms->FindEntryByName(ll_entry.char_id);
        if (!cms_entry)
            continue;

        game_sev->RemoveAllReferencesToChar(cms_entry->id);
    }

    std::vector<SevEntry> &sev_entries = game_sev->GetEntries();

    // Let's go first for the we are source ones.
    for (const SevTableEntryLL &ll_entry : ll_table)
    {
        CmsEntry *cms_entry = game_cms->FindEntryByName(ll_entry.char_id);
        if (!cms_entry)
            continue;

        SevEntry new_entry;

        new_entry.char_id = cms_entry->id;
        new_entry.costume_id = ll_entry.costume_id;

        for (const SevEventsLL &sub_entry : ll_entry.sub_entries)
        {
            if (!sub_entry.im_source)
                continue;

            CmsEntry *other_cms_entry = nullptr;

            if (sub_entry.char_code.length() != 0)
            {
                other_cms_entry = game_cms->FindEntryByName(sub_entry.char_code);
                if (!other_cms_entry)
                    continue;
            }

            SevCharEvents cevents;

            cevents.char_id = (other_cms_entry) ? other_cms_entry->id : 0xFFFFFFFF;
            cevents.costume_id = sub_entry.costume_id;
            cevents.events = sub_entry.events;

            new_entry.chars_events.push_back(cevents);
        }

        if (new_entry.chars_events.size() != 0)
        {
            sev_entries.push_back(new_entry);
        }
    }

    // Let's go now we the destinations
    for (const SevTableEntryLL &ll_entry : ll_table)
    {
        CmsEntry *cms_entry = game_cms->FindEntryByName(ll_entry.char_id);
        if (!cms_entry)
            continue;

        for (const SevEventsLL &sub_entry : ll_entry.sub_entries)
        {
            if (sub_entry.im_source)
                continue;

            CmsEntry *other_cms_entry = game_cms->FindEntryByName(sub_entry.char_code);
            if (!other_cms_entry)
                continue;

            SevEntry *other_entry = game_sev->FindEntry(other_cms_entry->id, sub_entry.costume_id);
            if (!other_entry)
                continue;

            SevCharEvents new_cevents;

            new_cevents.char_id = cms_entry->id;
            new_cevents.costume_id = ll_entry.costume_id;
            new_cevents.events = sub_entry.events;

            other_entry->chars_events.push_back(new_cevents);
        }
    }
}

bool X2mFile::InstallSev(std::vector<SevTableEntryHL> &hl_table, std::vector<SevTableEntryLL> &ll_table)
{
    if (cms_entry.id == X2M_DUMMY_ID || cms_entry.id == X2M_INVALID_ID)
    {
        DPRINTF("%s: Internal error. InstallCms must be called before this!!!\n", FUNCNAME);
        return false;
    }

    // Remove ourselves from tables if found (for updating, since costumes or copy data may not match)
    for (size_t i = 0; i < hl_table.size(); i++)
    {
        const SevTableEntryHL &entry_hl = hl_table[i];

        if (entry_hl.char_id == entry_name)
        {
            hl_table.erase(hl_table.begin()+i);
            i--;
        }
    }    

    for (size_t i = 0; i < ll_table.size(); i++)
    {
        const SevTableEntryLL &entry_ll = ll_table[i];

        if (entry_ll.char_id == entry_name)
        {
            ll_table.erase(ll_table.begin()+i);
            i--;
        }
    }

    if (HasSevHL())
    {
        for (const SevEntryHL &entry : sev_hl_entries)
        {
            SevTableEntryHL new_hl_entry;

            new_hl_entry.char_id = entry_name;
            new_hl_entry.costume_id = entry.costume_id;
            new_hl_entry.copy_char = entry.copy_char;
            new_hl_entry.copy_costume = entry.copy_costume;

            hl_table.push_back(new_hl_entry);
        }
    }
    else if (HasSevLL())
    {
        for (const SevEntryLL &entry : sev_ll_entries)
        {
            SevTableEntryLL new_ll_entry;

            new_ll_entry.char_id = entry_name;
            new_ll_entry.costume_id = entry.costume_id;
            new_ll_entry.sub_entries = entry.sub_entries;

            ll_table.push_back(new_ll_entry);
        }
    }

     game_sev->RemoveAllReferencesToChar(cms_entry.id); // Needed in case update doesn't have sev

    // Tables must always rebuild, regardless of if the mod has sev or not
    RebuildSevLL(ll_table); // LL table must go first
    RebuildSevHL(hl_table);

    return true;
}

bool X2mFile::FillCmlEntries()
{
    std::unordered_set<int32_t> costumes;
    size_t i = 0;

    for (const X2mSlotEntry &slot : slot_entries)
    {
        if (costumes.find(slot.costume_index) == costumes.end())
        {
            costumes.insert(slot.costume_index);

            if (i >= cml_entries.size())
                return false;

            cml_entries[i].char_id = cms_entry.id;
            cml_entries[i].costume_id = slot.costume_index;
            i++;
        }
    }

    return (costumes.size() == cml_entries.size());
}

bool X2mFile::InstallCml(bool update)
{
    if (cms_entry.id == X2M_DUMMY_ID || cms_entry.id == X2M_INVALID_ID)
    {
        DPRINTF("%s: Internal error. InstallCms must be called before this!!!\n", FUNCNAME);
        return false;
    }

    std::vector<CmlEntry *> entries;
    game_cml->FindEntriesByCharID(cms_entry.id, entries);

    if (!HasCml())
    {
        if (entries.size() != 0)
        {
            DPRINTF("%s: Update mode failed because there were cml entry(es) in the cml, but 0 here. Uninstalling the mod and reinstall it will fix this.\n", FUNCNAME);
            return false;
        }

        return true; // Yes, true!
    }

    if (cml_entries.size() != GetNumCostumes())
    {
        DPRINTF("%s: Internal error.\n", FUNCNAME);
        return false;
    }

    if (update)
    {
        if (entries.size() == 0)
        {
            goto regular_install;
        }
        else if (entries.size() != cml_entries.size())
        {
            DPRINTF("%s: Update mode failed because there are different num of cml entries in the cml than in the mod (for this char). Try uninstalling the mod and install it again.\n", FUNCNAME);
            return false;
        }

        if (!FillCmlEntries())
        {
            DPRINTF("%s: FillCmlEntries failed.\n", FUNCNAME);
            return false;
        }

        for (size_t i = 0; i < cml_entries.size(); i++)
        {
            const CmlEntry &entry = cml_entries[i];
            *(entries[i]) = entry;
        }

        return true;
    }

regular_install:

    if (!FillCmlEntries())
    {
        DPRINTF("%s: FillCmlEntries failed.\n", FUNCNAME);
        return false;
    }

    for (size_t i = 0; i < cml_entries.size(); i++)
    {
        const CmlEntry &entry = cml_entries[i];

        if (!game_cml->AddEntry(entry, (i == 0)))
            return false;
    }

    return true;
}

bool X2mFile::InstallHci()
{
    if (cms_entry.id == X2M_DUMMY_ID || cms_entry.id == X2M_INVALID_ID)
    {
        DPRINTF("%s: Internal error. InstallCms must be called before this!!!\n", FUNCNAME);
        return false;
    }

    game_hci->RemoveEntries(cms_entry.id); // Remove from previous version (if any)

    if (!HasHci())
        return true; // Yes, true

    for (HciEntry &entry : hci_entries)
    {
        entry.char_id = cms_entry.id;

        if (!game_hci->AddEntry(entry, false))
            return false;
    }

    return true;
}

bool X2mFile::InstallSelPortrait()
{
    if (type != X2mType::NEW_CHARACTER)
        return false;

    if (!SelPortraitExists())
        return false;

    size_t size;
    uint8_t *buf = ReadFile(X2M_SEL_PORTRAIT, &size);
    if (!buf)
        return false;

    std::string name = entry_name + "_000.dds";
    bool ret = Xenoverse2::SetSelPortrait(name, buf, size);

    delete[] buf;
    return ret;
}

bool X2mFile::InstallPreBaked()
{
    if (type != X2mType::NEW_CHARACTER)
        return false;

    if (cms_entry.id == X2M_DUMMY_ID || cms_entry.id == X2M_INVALID_ID)
    {
        DPRINTF("%s: Internal error. InstallCms must be called before this!!!\n", FUNCNAME);
        return false;
    }

    if (is_ozaru)
    {
        game_prebaked->AddOzaru(entry_name);
    }
    else
    {
        game_prebaked->RemoveOzaru(entry_name);
    }

    game_prebaked->RemoveBodyShape(cms_entry.id);
    game_prebaked->RemoveAutoBtlPortrait(cms_entry.id);

    if (UsesBodyShape())
    {
        game_prebaked->AddBodyShape(cms_entry.id, body_shape);
    }

    if (!BtlPortraitExists())
    {
        game_prebaked->AddAutoBtlPortrait(cms_entry.id);
    }

    return true;
}

bool X2mFile::InstallBtlPortrait(bool update)
{
    if (type != X2mType::NEW_CHARACTER)
        return false;

    if (!BtlPortraitExists())
    {
        if (update)
        {
            // Previous version "could" have a battle portrait, so if it exists, it should be removed here
            UninstallBtlPortrait(); // ignore errors
        }

        return true;
    }

    size_t size;
    uint8_t *buf = ReadFile(X2M_BTL_PORTRAIT, &size);
    if (!buf)
        return false;

    std::string emb_path = std::string("data/ui/texture/") + entry_name + ".emb";
    bool ret = xv2fs->WriteFile(emb_path, buf, size);

    delete[] buf;
    return ret;
}

bool X2mFile::InstallDir(const std::string &in_path, const std::string &out_path)
{
    size_t num_entries = GetNumEntries();

    std::string proper_in_path = in_path;
    std::string proper_out_path = out_path;

    if (proper_in_path.back() != '/')
        proper_in_path += '/';

    if (proper_out_path.length() != 0 && proper_out_path.back() != '/')
        proper_out_path += '/';

    for (size_t i = 0; i < num_entries; i++)
    {
        zip_stat_t zstat;

        if (zip_stat_index(archive, i, 0, &zstat) == -1)
            continue;

        if (!(zstat.valid & ZIP_STAT_NAME))
            continue;

        if (!(zstat.valid & ZIP_STAT_SIZE))
            continue;

        std::string entry_path = Utils::NormalizePath(zstat.name);

        if (entry_path.back() == '/')
            continue;

        if (!Utils::BeginsWith(entry_path, proper_in_path, false))
            continue;

        size_t size;
        uint8_t *buf = ReadFile(zstat.name, &size);
        if (!buf)
            return false;

        std::string out_file = proper_out_path + entry_path.substr(proper_in_path.length());
        bool ret = xv2fs->WriteFile(out_file, buf, size);
        delete[] buf;

        if (!ret)
            return false;
    }

    return true;
}

bool X2mFile::InstallCharaFiles()
{
    if (type != X2mType::NEW_CHARACTER)
        return false;

    if (!CharaDirectoryExists())
        return false;

    if (!InstallDir(entry_name, "data/chara/" + entry_name))
        return false;

    if (temp_aur_out.size() != 0)
    {
        IniFile ini;
        const std::string ini_base = "# This ini file is needed by XV2 Mods Installer to track the character custom auras.\n"
                                     "# DON'T DELETE IT OR AURAS WILL BE UNABLE TO BE UNINSTALLED OR PROPERLY UPDATED";

        ini.Load((const uint8_t *)ini_base.c_str(), ini_base.length());
        ini.SetMultipleIntegersValues("General", "AUR", temp_aur_out, true);

        if (!xv2fs->SaveFile(&ini, "data/chara/" + entry_name + "/" + X2M_CHARA_INI))
            return false;
    }

    return true;
}

bool X2mFile::InstallJungle()
{
    if (!JungleExists())
        return false;

    return InstallDir(X2M_JUNGLE, "");
}

bool X2mFile::UninstallCharaName()
{
    if (type != X2mType::NEW_CHARACTER)
        return false;

    for (int i = 0; i < XV2_LANG_NUM; i++)
    {
        if (!Xenoverse2::RemoveAllCharaName(entry_name, i))
            return false;
    }

    return true;
}

bool X2mFile::UninstallCostumeNames()
{
    if (type != X2mType::NEW_CHARACTER)
        return false;

    if (slot_entries.size() == 0)
        return false;

    for (size_t i = 0; i < slot_entries.size(); i++)
    {
        const X2mSlotEntry &slot = slot_entries[i];

        for (int j = 0; j < XV2_LANG_NUM; j++)
        {
            if (!Xenoverse2::RemoveCharaCostumeName(entry_name, slot.costume_index, slot.model_preset, j))
                return false;
        }
    }

    return true;
}

bool X2mFile::UninstallSlots()
{
    if (type != X2mType::NEW_CHARACTER)
        return false;

    if (slot_entries.size() == 0)
        return false;

    UninstallCssAudio();
    chara_list->RemoveSlots(std::string("\"") + entry_name + std::string("\""));
    return true;
}

bool X2mFile::UninstallCms()
{
    if (type != X2mType::NEW_CHARACTER)
        return false;

    if (cms_entry.id == X2M_INVALID_ID)
        return false;

    cms_entry.name = entry_name;

    CmsEntryXV2 *entry = dynamic_cast<CmsEntryXV2 *>(game_cms->FindEntryByName(cms_entry.name));
    if (entry)
    {
        cms_entry.id = entry->id;
    }
    else
    {
        return true;
    }

    return game_cms->RemoveEntry(entry_name);
}

bool X2mFile::UninstallCus()
{
    if (type != X2mType::NEW_CHARACTER)
        return false;

    if (cms_entry.id == X2M_INVALID_ID)
        return false;

    if (cms_entry.id == X2M_DUMMY_ID)
        return true; // Yes, true

    game_cus->RemoveSkillSets(cms_entry.id);
    return true;
}

bool X2mFile::UninstallCso()
{
    if (type != X2mType::NEW_CHARACTER)
        return false;

    if (cms_entry.id == X2M_INVALID_ID)
        return false;

    if (cms_entry.id == X2M_DUMMY_ID)
        return true; // Yes, true

    game_cso->RemoveEntries(cms_entry.id);
    return true;
}

bool X2mFile::UninstallPsc()
{
    if (type != X2mType::NEW_CHARACTER)
        return false;

    if (cms_entry.id == X2M_INVALID_ID)
        return false;

    if (cms_entry.id == X2M_DUMMY_ID)
        return true; // Yes, true

    game_psc->RemoveEntry(cms_entry.id);
    return true;
}

bool X2mFile::UninstallAur()
{
    if (type != X2mType::NEW_CHARACTER)
        return false;

    if (cms_entry.id == X2M_INVALID_ID)
        return false;

    if (cms_entry.id == X2M_DUMMY_ID)
        return true; // Yes, true

    GetInstalledCustomAuraChara();

    for (int i = (int) temp_aur_in.size()-1; i >= 0; i--)
    {
        game_aur->RemoveAuraIfLast((uint32_t)temp_aur_in[i]);
    }

    game_aur->RemoveCharaLinks(cms_entry.id);
    return true;
}

bool X2mFile::UninstallSev(std::vector<SevTableEntryHL> &hl_table, std::vector<SevTableEntryLL> &ll_table)
{
    if (type != X2mType::NEW_CHARACTER)
        return false;

    if (cms_entry.id == X2M_INVALID_ID)
        return false;

    if (cms_entry.id == X2M_DUMMY_ID)
        return true; // Yes, true

    // Remove ourselves from tables if found
    for (size_t i = 0; i < hl_table.size(); i++)
    {
        const SevTableEntryHL &entry_hl = hl_table[i];

        if (entry_hl.char_id == entry_name)
        {
            hl_table.erase(hl_table.begin()+i);
            i--;
        }
    }

    for (size_t i = 0; i < ll_table.size(); i++)
    {
        const SevTableEntryLL &entry_ll = ll_table[i];

        if (entry_ll.char_id == entry_name)
        {
            ll_table.erase(ll_table.begin()+i);
            i--;
        }
    }

    game_sev->RemoveAllReferencesToChar(cms_entry.id);
    RebuildSevLL(ll_table); // LL must go first
    RebuildSevHL(hl_table);

    return true;
}

bool X2mFile::UninstallCml()
{
    if (type != X2mType::NEW_CHARACTER)
        return false;

    if (cms_entry.id == X2M_INVALID_ID)
        return false;

    if (cms_entry.id == X2M_DUMMY_ID)
        return true; // Yes, true

    game_cml->RemoveEntries(cms_entry.id);
    return true;
}

bool X2mFile::UninstallHci()
{
    if (type != X2mType::NEW_CHARACTER)
        return false;

    if (cms_entry.id == X2M_INVALID_ID)
        return false;

    if (cms_entry.id == X2M_DUMMY_ID)
        return true; // Yes, true

    game_hci->RemoveEntries(cms_entry.id);
    return true;
}

bool X2mFile::UninstallSelPortrait()
{
    if (type != X2mType::NEW_CHARACTER)
        return false;

    std::string name = entry_name + "_000.dds";
    return Xenoverse2::RemoveSelPortrait(name);
}

bool X2mFile::UninstallPreBaked()
{
    if (type != X2mType::NEW_CHARACTER)
        return false;

    game_prebaked->RemoveOzaru(entry_name);
    game_prebaked->RemoveBodyShape(cms_entry.id);
    game_prebaked->RemoveAutoBtlPortrait(cms_entry.id);
    return true;
}

bool X2mFile::UninstallBtlPortrait()
{
    if (type != X2mType::NEW_CHARACTER)
        return false;

    std::string emb_path = std::string("data/ui/texture/") + entry_name + ".emb";
    xv2fs->RemoveFile(emb_path); // Ignore error

    return true;
}

bool X2mFile::UninstallDir(const std::string &in_path, const std::string &out_path)
{
    size_t num_entries = GetNumEntries();

    std::string proper_in_path = in_path;
    std::string proper_out_path = out_path;

    if (proper_in_path.back() != '/')
        proper_in_path += '/';

    if (proper_out_path.length() != 0 && proper_out_path.back() != '/')
        proper_out_path += '/';

    for (size_t i = 0; i < num_entries; i++)
    {
        zip_stat_t zstat;

        if (zip_stat_index(archive, i, 0, &zstat) == -1)
            continue;

        if (!(zstat.valid & ZIP_STAT_NAME))
            continue;

        if (!(zstat.valid & ZIP_STAT_SIZE))
            continue;

        std::string entry_path = Utils::NormalizePath(zstat.name);

        if (entry_path.back() == '/')
            continue;

        if (!Utils::BeginsWith(entry_path, proper_in_path, false))
            continue;

        std::string out_file = proper_out_path + entry_path.substr(proper_in_path.length());
        bool ret = xv2fs->RemoveFile(out_file);

        if (!ret)
            return false;
    }

    return true;
}

bool X2mFile::UninstallCharaFiles(bool remove_empty)
{
    if (type != X2mType::NEW_CHARACTER)
        return false;

    if (!CharaDirectoryExists())
        return false;

    const std::string path = "data/chara/" + entry_name;

    if (!UninstallDir(entry_name, path))
        return false;

    xv2fs->RemoveFile("data/chara/" + entry_name + "/" + X2M_CHARA_INI);

    if (remove_empty)
        xv2fs->RemoveEmptyDir(path);

    return true;
}

bool X2mFile::UninstallJungle()
{
    if (!JungleExists())
        return false;

    return UninstallDir(X2M_JUNGLE, "");
}

bool X2mFile::InstallPupSkill()
{
    if (type != X2mType::NEW_SKILL)
        return false;

    temp_pup_out.clear();

    if (temp_pup_in.size() == skill_pup_entries.size())
    {
        if (!HasSkillPup())
            return true;

        // Case of update where old and new have same amount of pup entries
        std::vector<PupEntry *> update_entries;

        for (int32_t pup_id : temp_pup_in)
        {
            PupEntry *entry = game_pup->FindEntryByID((uint32_t)pup_id);
            if (!entry)
                break;

            update_entries.push_back(entry);
        }

        if (update_entries.size() != skill_pup_entries.size())
            goto new_install; // Some of the ids were not found, remove and do new install

        for (size_t i = 0; i < skill_pup_entries.size(); i++)
        {
            PupEntry *update_entry = update_entries[i];
            PupEntry &entry = skill_pup_entries[i];

            entry.id = update_entry->id;
            *update_entry = entry;
        }

        goto setup_pup;
    }

new_install:
    // Case of new install or update where old and new have different amount of pup entries

    // Remove first existing entries
    for (int32_t pup_id : temp_pup_in)
    {
        game_pup->RemoveEntry((uint32_t)pup_id);
    }

    if (!HasSkillPup())
        return true;

    if (!game_pup->AddConsecutiveEntries(skill_pup_entries))
        return false;

setup_pup:

    skill_entry.pup_id = skill_pup_entries[0].id;

    for (const PupEntry &entry : skill_pup_entries)
    {
        temp_pup_out.push_back((int32_t)entry.id);
    }

    return true;
}

bool X2mFile::InstallCustomAuraSkill()
{
    temp_aur_out.clear();

    size_t num_custom = GetNumSkillCustomAuras();

    if (temp_aur_in.size() == num_custom)
    {
        if (!HasSkillCustomAuras())
            return true;

        // Case of "perfect update"
        std::vector<AurAura *> update_entries;

        for (int32_t aur_id : temp_aur_in)
        {
            AurAura *aura = game_aur->FindAuraByID((uint32_t)aur_id);
            if (!aura)
                break;

            update_entries.push_back(aura);
        }

        if (update_entries.size() != num_custom)
            goto new_install; // Some of the ids were not found, remove and do new install

        for (size_t i = 0, j = 0; i < skill_aura_entries.size(); i++)
        {
            if (j >= update_entries.size())
            {
                DPRINTF("%s: Internal coding error.\n", FUNCNAME);
                return false;
            }

            if (skill_aura_entries[i].data.aur_aura_id != X2M_INVALID_ID16)
                continue;

            AurAura *update_aura = update_entries[j];
            AurAura &aura = skill_aura_entries[i].aura;

            aura.id = update_aura->id;
            skill_aura_entries[i].data.aur_aura_id = (uint16_t)aura.id;
            *update_aura = aura;

            temp_aur_out.push_back((int32_t)aura.id);
            j++;
        }

        goto setup_aura;
    }

new_install:

    // Case of new install or update where old and new have different amount of custom aura entries

    // Remove first existing entries
    for (int i = (int) temp_aur_in.size()-1; i >= 0; i--)
    {
        game_aur->RemoveAuraIfLast((uint32_t)temp_aur_in[i]);
    }

    if (!HasSkillCustomAuras())
        return true;

    for (size_t i = 0; i < skill_aura_entries.size(); i++)
    {
        if (skill_aura_entries[i].data.aur_aura_id != X2M_INVALID_ID16)
            continue;

        AurAura &aura = skill_aura_entries[i].aura;

        if (!game_aur->AddAura(aura))
            return false;

        skill_aura_entries[i].data.aur_aura_id = (uint16_t)aura.id;
        temp_aur_out.push_back((int32_t)aura.id);
    }

setup_aura:

    // Nothing to do, setup happened before
    return true;
}

bool X2mFile::InstallAuraSkill()
{
    std::vector<CusAuraData> temp;

    if (type != X2mType::NEW_SKILL)
        return false;

    if (!InstallCustomAuraSkill())
        return false;

    temp_cus_aur_out.clear();

    if (temp_cus_aur_in.size() == skill_aura_entries.size())
    {
        if (!HasSkillAura())
            return true;

        // Case of update where old and new have same amount of entries
        std::vector<CusAuraData *> update_entries;

        for (int32_t cus_aur_id : temp_cus_aur_in)
        {
            CusAuraData *data = game_prebaked->FindAuraData((uint16_t)cus_aur_id);
            if (!data)
                break;

            update_entries.push_back(data);
        }

        if (update_entries.size() != skill_aura_entries.size())
            goto new_install; // Some of the ids were not found, remove and do new install

        for (size_t i = 0; i < skill_aura_entries.size(); i++)
        {
            CusAuraData *update_data = update_entries[i];
            CusAuraData &data = skill_aura_entries[i].data;

            data.cus_aura_id = update_data->cus_aura_id;
            *update_data = data;
        }

        goto setup_data;
    }

new_install:
    // Case of new install or update where old and new have different amount of pup entries

    // Remove first existing entries
    for (int32_t cus_aur_id : temp_cus_aur_in)
    {
        game_prebaked->RemoveAuraData((uint16_t)cus_aur_id);
    }

    if (!HasSkillAura())
        return true;

    for (size_t i = 0; i < skill_aura_entries.size(); i++)
    {
        temp.push_back(skill_aura_entries[i].data);
    }

    if (!game_prebaked->AddConsecutiveAuraData(temp))
        return false;

    // Update data from temp
    for (size_t i = 0; i < skill_aura_entries.size(); i++)
    {
        skill_aura_entries[i].data = temp[i];
    }

setup_data:

    skill_entry.aura = skill_aura_entries[0].data.cus_aura_id;

    for (const X2mSkillAura &aura : skill_aura_entries)
    {
        temp_cus_aur_out.push_back((int32_t)aura.data.cus_aura_id);
    }

    return true;
}

bool X2mFile::InstallCusSkill()
{
    if (type != X2mType::NEW_SKILL)
        return false;

    if (skill_entry.id == X2M_INVALID_ID16)
        return false;

    if (HasSkillCostumeDepend())
    {
        X2mCostumeEntry *costume = FindInstalledCostume(skill_costume_depend.guid);

        if (costume && costume->partsets.size() > 0)
        {
            skill_entry.partset = costume->partsets.front();
        }
        else
        {
            skill_entry.partset = 0xFFFF;
        }
    }

    if (skill_entry.id == X2M_DUMMY_ID16)
    {
        // New install
        if (!AssignSkillIds())
        {
            DPRINTF("%s: Failed in assigning an id to the skill.\n", FUNCNAME);
            return false;
        }

        if (!game_cus->AddSkillAuto(skill_entry))
        {
            DPRINTF("%s: AddSkillAuto failed.\n", FUNCNAME);
            return false;
        }
    }
    else
    {
        CusSkill *skill = game_cus->FindSkillAnyByID(skill_entry.id);
        if (!skill)
        {
            DPRINTF("%s: Internal error, cannot find skill for update.\n", FUNCNAME);
            return false;
        }

        *skill = skill_entry; // Update
    }

    return true;
}

bool X2mFile::InstallSkillName()
{
    if (type != X2mType::NEW_SKILL)
        return false;

    if (skill_entry.id == X2M_INVALID_ID16 || skill_entry.id == X2M_DUMMY_ID16)
    {
        DPRINTF("%s: InstallCusSkill must be called before this!\n", FUNCNAME);
        return false;
    }

    if (skill_name[XV2_LANG_ENGLISH].length() == 0)
        return false;

    for (int i = 0; i < XV2_LANG_NUM; i++)
    {
        const std::string *name;

        if (skill_name[i].length() != 0)
            name = &skill_name[i];
        else
            name = &skill_name[XV2_LANG_ENGLISH];

        if (skill_type == X2mSkillType::SUPER)
        {
            if (!Xenoverse2::SetSuperSkillName(skill_entry.id2, *name, i))
                return false;
        }
        else if (skill_type == X2mSkillType::ULTIMATE)
        {
            if (!Xenoverse2::SetUltimateSkillName(skill_entry.id2, *name, i))
                return false;
        }
        else if (skill_type == X2mSkillType::EVASIVE)
        {
            if (!Xenoverse2::SetEvasiveSkillName(skill_entry.id2, *name, i))
                return false;
        }
        else if (skill_type == X2mSkillType::AWAKEN)
        {
            if (!Xenoverse2::SetAwakenSkillName(skill_entry.id2, *name, i))
                return false;
        }
        else
        {
            DPRINTF("%s: Internal error.\n", FUNCNAME);
            return false;
        }
    }

    if (skill_type == X2mSkillType::AWAKEN)
    {
        // Clean up first (for updated with different number of entries)
        for (int i = 0; i < 100; i++)
        {
            for (int lang = 0; lang < XV2_LANG_NUM; lang++)
                Xenoverse2::RemoveBtlHudAwakenName(skill_entry.id2, i, lang);
        }

        for (size_t i = 0; i < skill_trans_names.size(); i++)
        {
            const X2mSkillTransName &tn = skill_trans_names[i];

            for (int lang = 0; lang < XV2_LANG_NUM; lang++)
            {
                const std::string *name;

                if (tn.trans_name[lang].length() != 0)
                    name = &tn.trans_name[lang];
                else
                    name = &tn.trans_name[XV2_LANG_ENGLISH];

                if (!Xenoverse2::SetBtlHudAwakenName(skill_entry.id2, (uint16_t)i, *name, lang))
                    return false;
            }
        }
    }

    return true;
}

bool X2mFile::InstallSkillDesc()
{
    if (type != X2mType::NEW_SKILL)
        return false;

    if (skill_entry.id == X2M_INVALID_ID16 || skill_entry.id == X2M_DUMMY_ID16)
    {
        DPRINTF("%s: InstallCusSkill must be called before this!\n", FUNCNAME);
        return false;
    }

    if (skill_desc[XV2_LANG_ENGLISH].length() == 0)
    {
        // This is for case where previous version had description and current one doesn't.
        return UninstallSkillDesc();
    }

    for (int i = 0; i < XV2_LANG_NUM; i++)
    {
        const std::string *desc;

        if (skill_desc[i].length() != 0)
            desc = &skill_desc[i];
        else
            desc = &skill_desc[XV2_LANG_ENGLISH];

        if (skill_type == X2mSkillType::SUPER)
        {
            if (!Xenoverse2::SetSuperSkillDesc(skill_entry.id2, *desc, i))
                return false;
        }
        else if (skill_type == X2mSkillType::ULTIMATE)
        {
            if (!Xenoverse2::SetUltimateSkillDesc(skill_entry.id2, *desc, i))
                return false;
        }
        else if (skill_type == X2mSkillType::EVASIVE)
        {
            if (!Xenoverse2::SetEvasiveSkillDesc(skill_entry.id2, *desc, i))
                return false;
        }
        else if (skill_type == X2mSkillType::AWAKEN)
        {
            if (!Xenoverse2::SetAwakenSkillDesc(skill_entry.id2, *desc, i))
                return false;
        }
        else
        {
            DPRINTF("%s: Internal error.\n", FUNCNAME);
            return false;
        }
    }

    return true;
}

bool X2mFile::InstallIdbSkill()
{
    if (type != X2mType::NEW_SKILL)
        return false;

    if (skill_entry.id == X2M_INVALID_ID16 || skill_entry.id == X2M_DUMMY_ID16)
    {
        DPRINTF("%s: InstallCusSkill must be called before this!\n", FUNCNAME);
        return false;
    }

    if (!HasSkillIdbEntry())
    {
        // Uninstall previous idb (if any)
        game_skill_idb->RemoveEntry(skill_entry.id2, GetCusSkillType());
        return true;
    }

    AssignSkillIdbIds();
    return game_skill_idb->AddEntry(skill_idb_entry, true);
}

bool X2mFile::InstallSkillVisitor(const std::string &path, void *param)
{
    X2mFile *pthis = (X2mFile *)param;
    const std::string skill_dir = pthis->GetSkillDirectory();
    const std::string filename = Utils::GetFileNameString(path);

    size_t size;
    uint8_t *buf = pthis->ReadFile(path, &size);
    if (!buf)
        return false;

    if (Utils::BeginsWith(filename, X2M_SKILL_PREFIX, false))
    {
        CmsEntry *entry = game_cms->FindEntryByID(pthis->skill_entry.id2 / 10);
        if (!entry)
        {
            delete[] buf;
            return false;
        }

        std::string out_path = skill_dir + Utils::ToStringAndPad(pthis->skill_entry.id2, 3) + "_" + entry->name + "_" + pthis->skill_entry.name;
        out_path += filename.substr(strlen(X2M_SKILL_PREFIX));

        if (Utils::EndsWith(path, ".bac", false))
        {
            BacFile bac;

            if (!bac.Load(buf, size))
            {
                DPRINTF("%s: Failed to load internal x2m bac file: \"%s\"\n", FUNCNAME, path.c_str());
                delete[] buf;
                return false;
            }

            delete[] buf;
            bac.ChangeReferencesToSkill(X2M_DUMMY_ID16, pthis->skill_entry.id2);

            if (!xv2fs->SaveFile(&bac, out_path))
                return false;

            return true;
        }
        else if (Utils::EndsWith(path, ".bsa", false))
        {
            BsaFile bsa;

            if (!bsa.Load(buf, size))
            {
                DPRINTF("%s: Failed to load internal x2m bsa file: \"%s\"\n", FUNCNAME, path.c_str());
                delete[] buf;
                return false;
            }

            delete[] buf;
            bsa.ChangeReferencesToSkill(X2M_DUMMY_ID16, pthis->skill_entry.id2);

            if (!xv2fs->SaveFile(&bsa, out_path))
                return false;

            return true;
        }
        else
        {
            bool ret = xv2fs->WriteFile(out_path, buf, size);
            delete[] buf;

            return ret;
        }
    }

    std::string out_path = skill_dir + filename;

    bool ret = xv2fs->WriteFile(out_path, buf, size);
    delete[] buf;

    return ret;
}

bool X2mFile::InstallSkillFiles()
{
    if (type != X2mType::NEW_SKILL)
        return false;

    if (!SkillDirectoryExists())
        return false;

    if (skill_entry.id == X2M_INVALID_ID16 || skill_entry.id == X2M_DUMMY_ID16)
    {
        DPRINTF("%s: InstallCusSkill must be called before this!\n", FUNCNAME);
        return false;
    }

    const std::string skill_dir = GetSkillDirectory();
    if (skill_dir.length() == 0)
    {
        DPRINTF("%s: GetSkillDirectory failed.\n", FUNCNAME);
        return false;
    }

    if (!VisitDirectory(X2M_SKILL, InstallSkillVisitor, this))
        return false;

    IniFile ini;
    const std::string ini_base = "# This ini file is needed by XV2 Mods Installer to track the skill.\n"
                                 "# DON'T DELETE IT OR SKILL WILL BE UNABLE TO BE UNINSTALLED OR PROPERLY UPDATED";

    ini.Load((const uint8_t *)ini_base.c_str(), ini_base.length());
    ini.SetStringValue("General", "GUID", Utils::GUID2String(mod_guid));

    if (temp_pup_out.size() != 0)
        ini.SetMultipleIntegersValues("General", "PUP", temp_pup_out, true);

    if (temp_cus_aur_out.size() != 0)
        ini.SetMultipleIntegersValues("General", "CUS_AUR", temp_cus_aur_out, true);

    if (temp_aur_out.size() != 0)
        ini.SetMultipleIntegersValues("General", "AUR", temp_aur_out, true);

    if (!xv2fs->SaveFile(&ini, skill_dir + X2M_SKILL_INI))
        return false;

    return true;
}

bool X2mFile::UninstallPupSkill()
{
    if (type != X2mType::NEW_SKILL)
        return false;

    if (skill_entry.id == X2M_INVALID_ID16)
        return false;

    for (int32_t pup_id : temp_pup_in)
    {
        game_pup->RemoveEntry((uint32_t)pup_id);
    }

    return true;
}

bool X2mFile::UninstallAuraSkill()
{
    if (type != X2mType::NEW_SKILL)
        return false;

    if (skill_entry.id == X2M_INVALID_ID16)
        return false;

    for (int i = (int) temp_aur_in.size()-1; i >= 0; i--)
    {
        game_aur->RemoveAuraIfLast((uint32_t)temp_aur_in[i]);
    }

    for (int32_t cus_aur_id : temp_cus_aur_in)
    {
        game_prebaked->RemoveAuraData((uint16_t)cus_aur_id);
    }

    return true;
}

bool X2mFile::UninstallCusSkill()
{
    if (type != X2mType::NEW_SKILL)
        return false;

    if (skill_entry.id == X2M_INVALID_ID16)
        return false;

    if (skill_entry.id == X2M_DUMMY_ID16)
        return true; // Yes, true    

    game_cus->RemoveAllReferencesToSkill(skill_entry.id);
    game_cus->RemoveSkillAuto(skill_entry.id);
    return true;
}

bool X2mFile::UninstallSkillName()
{
    if (type != X2mType::NEW_SKILL)
        return false;

    if (skill_entry.id == X2M_INVALID_ID16)
        return false;

    if (skill_entry.id == X2M_DUMMY_ID16)
        return true; // Yes, true

    for (int i = 0; i < XV2_LANG_NUM; i++)
    {
        if (skill_type == X2mSkillType::SUPER)
        {
            if (!Xenoverse2::RemoveSuperSkillName(skill_entry.id2, i))
                return false;
        }
        else if (skill_type == X2mSkillType::ULTIMATE)
        {
            if (!Xenoverse2::RemoveUltimateSkillName(skill_entry.id2, i))
                return false;
        }
        else if (skill_type == X2mSkillType::EVASIVE)
        {
            if (!Xenoverse2::RemoveEvasiveSkillName(skill_entry.id2, i))
                return false;
        }
        else if (skill_type == X2mSkillType::AWAKEN)
        {
            if (!Xenoverse2::RemoveAwakenSkillName(skill_entry.id2, i))
                return false;
        }
        else
        {
            DPRINTF("%s: Internal error.\n", FUNCNAME);
            return false;
        }
    }

    if (skill_type == X2mSkillType::AWAKEN)
    {
        for (int i = 0; i < 100; i++)
        {
            for (int lang = 0; lang < XV2_LANG_NUM; lang++)
                Xenoverse2::RemoveBtlHudAwakenName(skill_entry.id2, i, lang);
        }
    }

    return true;
}

bool X2mFile::UninstallSkillDesc()
{
    if (type != X2mType::NEW_SKILL)
        return false;

    if (skill_entry.id == X2M_INVALID_ID16)
        return false;

    if (skill_entry.id == X2M_DUMMY_ID16)
        return true; // Yes, true

    for (int i = 0; i < XV2_LANG_NUM; i++)
    {
        if (skill_type == X2mSkillType::SUPER)
        {
            if (!Xenoverse2::RemoveSuperSkillDesc(skill_entry.id2, i))
                return false;
        }
        else if (skill_type == X2mSkillType::ULTIMATE)
        {
            if (!Xenoverse2::RemoveUltimateSkillDesc(skill_entry.id2, i))
                return false;
        }
        else if (skill_type == X2mSkillType::EVASIVE)
        {
            if (!Xenoverse2::RemoveEvasiveSkillDesc(skill_entry.id2, i))
                return false;
        }
        else if (skill_type == X2mSkillType::AWAKEN)
        {
            if (!Xenoverse2::RemoveAwakenSkillDesc(skill_entry.id2, i))
                return false;
        }
        else
        {
            DPRINTF("%s: Internal error.\n", FUNCNAME);
            return false;
        }
    }

    return true;
}

bool X2mFile::UninstallIdbSkill()
{
    if (type != X2mType::NEW_SKILL)
        return false;

    if (skill_entry.id == X2M_INVALID_ID16)
        return false;

    if (skill_entry.id == X2M_DUMMY_ID16)
        return true; // Yes, true

    game_skill_idb->RemoveEntry(skill_entry.id2, GetCusSkillType());
    return true;
}

bool X2mFile::UninstallSkillFiles(bool remove_empty)
{
    if (type != X2mType::NEW_SKILL)
        return false;

    if (!SkillDirectoryExists())
        return false;

    if (skill_entry.id == X2M_INVALID_ID16)
        return false;

    if (skill_entry.id == X2M_DUMMY_ID16)
        return true; // Yes true

    const std::string path = GetSkillDirectory();
    if (path.length() == 0)
        return false;

    xv2fs->RemoveDir(path, remove_empty);
    return true;
}

static bool ResolvePartSetString(std::string &str, uint16_t base, uint16_t num)
{
    if (str.length() == 0)
        return false;

    static const uint16_t lower_limit = 10000;
    uint16_t upper_limit = lower_limit+num;

    for (uint16_t i = lower_limit; i < upper_limit; i++)
    {
        std::string str_num = Utils::ToString(i);
        size_t pos = str.find(str_num);

        if (pos != std::string::npos)
        {
            std::string temp = str.substr(0, pos);
            temp += Utils::ToStringAndPad((i-lower_limit)+base, 3);
            temp += str.substr(pos+str_num.length());

            str = temp;
            return true;
        }
    }

    return false;
}

static void ResolvePartSet(BcsPartSet &set, uint16_t base, uint16_t num)
{
    if (!set.valid)
        return;

    static const uint16_t lower_limit = 10000;
    uint16_t upper_limit = lower_limit+num;

    for (size_t i = 0; i < set.parts.size(); i++)
    {
        BcsPart &part = set.parts[i];
        if (!part.valid)
            continue;

        if (part.model >= lower_limit && part.model < upper_limit)
        {
            part.model = (part.model-lower_limit) + base;
        }

        if (part.model2 >= lower_limit && part.model2 < upper_limit)
        {
            part.model2 = (part.model2-lower_limit) + base;
        }

        for (std::string &str : part.files)
        {
            ResolvePartSetString(str, base, num);
        }

        for (BcsPhysics &ph : part.physics)
        {
            for (std::string &str : ph.unk_28)
            {
                ResolvePartSetString(str, base, num);
            }
        }
    }
}

bool X2mFile::InstallCostumePartSets()
{
    if (type != X2mType::NEW_COSTUME)
        return false;

    static std::vector<uint16_t> candidates_partsets;
    static std::vector<BcsFile *> races_bcs;

    if (candidates_partsets.size() == 0)
    {
        for (uint16_t i = 535; i <= 549; i++)
            candidates_partsets.push_back(i);

        for (uint16_t i = 555; i <= 599; i++)
            candidates_partsets.push_back(i);

        for (uint16_t i = 631; i <= 649; i++)
            candidates_partsets.push_back(i);

        for (uint16_t i = 670; i <= 699; i++)
            candidates_partsets.push_back(i);

        for (uint16_t i = 741; i <= 799; i++)
            candidates_partsets.push_back(i);

        for (uint16_t i = 801; i <= 830; i++)
            candidates_partsets.push_back(i);

        for (uint16_t i = 866; i <= 884; i++)
            candidates_partsets.push_back(i);

        for (uint16_t i = 921; i <= 929; i++)
            candidates_partsets.push_back(i);

        for (uint16_t i = 942; i <= 999; i++)
            candidates_partsets.push_back(i);
    }

    if (races_bcs.size() == 0)
    {
        races_bcs.push_back(game_hum_bcs);
        races_bcs.push_back(game_huf_bcs);
        races_bcs.push_back(game_nmc_bcs);
        races_bcs.push_back(game_fri_bcs);
        races_bcs.push_back(game_mam_bcs);
        races_bcs.push_back(game_maf_bcs);
    }

    bool update = (update_costume_entry.partsets.size() > 0);
    size_t num = GetEffectiveNumCostumePartSets();

    if (!update)
    {
        size_t num_found = 0;
        uint16_t partset = 0;

        for (size_t i = 0; i < num && num_found != num; i++)
        {
            for (size_t j = 0; j < candidates_partsets.size(); j++)
            {
                uint16_t this_partset = candidates_partsets[j];

                if (j > 0 && this_partset != (candidates_partsets[j-1]+1))
                    num_found = 0;

                bool in_use = false;

                for (BcsFile *bcs : races_bcs)
                {
                    const std::vector<BcsPartSet> &sets = bcs->GetPartSets();

                    if (this_partset < sets.size() && sets[this_partset].valid)
                    {
                        in_use = true;
                        break;
                    }
                }

                if (!in_use)
                {
                    if (game_costume_file->FindCostumeByPartSet(this_partset))
                        in_use = true;
                }

                if (in_use)
                {
                    num_found = 0;
                    continue;
                }

                if (num_found == 0)
                    partset = this_partset;

                num_found++;

                if (num_found == num)
                    break;
            }
        }

        if (num_found != num)
        {
            DPRINTF("%s: Overflow, cannot assign suitable(s) partset(s) id(s)\n", FUNCNAME);
            return false;
        }

        for (size_t i = 0; i < num; i++)
        {
            update_costume_entry.partsets.push_back(partset+(uint16_t)i);
        }

        if (update_costume_entry.races.size() != 0)
        {
            DPRINTF("%s: Internal error, races size should be zero in this point.\n", FUNCNAME);
            return false;
        }

        for (uint8_t i = 0; i < X2M_CR_NUM; i++)
        {
            if (GetNumCostumePartSets(i) > 0)
                update_costume_entry.races.push_back(i);
        }
    }
    else
    {
        if (num != update_costume_entry.partsets.size())
        {
            DPRINTF("%s: The function shouldn't have been called in this state.\n", FUNCNAME);
            return false;
        }
    }

    if (update_costume_entry.partsets.size() == 0)
    {
        DPRINTF("%s: num of partsets shouldn't have been 0 zt this point!\n", FUNCNAME);
        return 0;
    }

    for (size_t i = 0; i < update_costume_entry.partsets.size(); i++)
    {
        uint16_t partset = update_costume_entry.partsets[i];

        for (uint8_t race = 0; race < X2M_CR_NUM; race++)
        {
            if (race >= races_bcs.size())
            {
                DPRINTF("%s: Internal error on races array size.\n", FUNCNAME);
                return false;
            }

            if (GetNumCostumePartSets(race) > 0)
            {
                BcsFile *bcs = races_bcs[race];
                std::vector<BcsPartSet> &sets = bcs->GetPartSets();

                if (partset >= sets.size())
                    sets.resize(partset+1);

                BcsPartSet &new_ps = GetCostumePartSet(race, i).bcs;

                ResolvePartSet(new_ps, update_costume_entry.partsets[0], (uint16_t)num);
                sets[partset] = new_ps;
            }
        }
    }

    return true;
}

static void RemoveCostumeNameReferences(uint16_t name_id)
{
    for (IdbEntry &entry : *game_top_idb)
    {
        if (entry.name_id == name_id)
            entry.name_id = 0xFFFF;
    }

    for (IdbEntry &entry : *game_bottom_idb)
    {
        if (entry.name_id == name_id)
            entry.name_id = 0xFFFF;
    }

    for (IdbEntry &entry : *game_gloves_idb)
    {
        if (entry.name_id == name_id)
            entry.name_id = 0xFFFF;
    }

    for (IdbEntry &entry : *game_shoes_idb)
    {
        if (entry.name_id == name_id)
            entry.name_id = 0xFFFF;
    }
}

static std::string ChooseCostumeEntryName(const IdbEntry &idb)
{
    std::string str = "wear_";

    if (idb.racelock == IDB_RACE_HUM || idb.racelock == IDB_RACE_SYM || idb.racelock == (IDB_RACE_HUM|IDB_RACE_SYM))
    {
        str += "hum_";
    }
    else if (idb.racelock == IDB_RACE_HUF || idb.racelock == IDB_RACE_SYF || idb.racelock == (IDB_RACE_HUF|IDB_RACE_SYF))
    {
        str += "huf_";
    }
    else if (idb.racelock == IDB_RACE_NMC)
    {
        str += "nmc_";
    }
    else if (idb.racelock == IDB_RACE_FRI)
    {
        str += "fri_";
    }
    else if (idb.racelock == IDB_RACE_MAM)
    {
        str += "mam_";
    }
    else if (idb.racelock == IDB_RACE_MAF)
    {
        str += "maf_";
    }
    else if (idb.racelock == (IDB_RACE_MAM|IDB_RACE_MAF))
    {
        str += "mar_";
    }
    else
    {
        str += "cmn_";
    }

    return str;
}

bool X2mFile::InstallCostumeCostumeNames()
{
    if (type != X2mType::NEW_COSTUME)
        return false;

    if (update_costume_entry.idb_entries.size() != 0 && update_costume_entry.idb_entries.size() != costume_items.size())
    {
        DPRINTF("%s: This function shouldn't have been called in this state.\n", FUNCNAME);
        return false;
    }

    if (update_costume_entry.partsets.size() == 0)
    {
        DPRINTF("%s: This function should have been called after InstallCostumePartSet.\n", FUNCNAME);
        return false;
    }

    // Delete any existing name before
    for (size_t i = 0; i < update_costume_entry.idb_entries.size(); i++)
    {
        uint16_t idb_id = update_costume_entry.idb_entries[i];
        uint8_t type = update_costume_entry.costume_types[i];

        if (type == COSTUME_ACCESSORY)
            continue;

        IdbEntry *entry = nullptr;

        if (type == COSTUME_TOP)
        {
            entry = game_top_idb->FindEntryByID(idb_id);
        }
        else if (type == COSTUME_BOTTOM)
        {
            entry = game_bottom_idb->FindEntryByID(idb_id);
        }
        else if (type == COSTUME_GLOVES)
        {
            entry = game_gloves_idb->FindEntryByID(idb_id);
        }
        else if (type == COSTUME_SHOES)
        {
            entry = game_shoes_idb->FindEntryByID(idb_id);
        }

        if (!entry)
        {
            DPRINTF("%s: Internal error, cannot find installed idb entry for update.\n", FUNCNAME);
            return false;
        }

        uint16_t name_id = entry->name_id;
        if (name_id == 0xFFFF)
            continue;

        RemoveCostumeNameReferences(name_id);

        for (int l = 0; l < XV2_LANG_NUM; l++)
        {
            if (!Xenoverse2::RemoveCacCostumeName(name_id, l, (l==0)))
            {
                DPRINTF("%s: Internal error, fail in RemoveCacCostumeName.\n", FUNCNAME);
                return false;
            }
        }
    }

    // Now add new names
    for (size_t i = 0; i < costume_items.size(); i++)
    {
        X2mItem &item = costume_items[i];
        bool name_found = false;

        if (item.item_type == X2mItemType::ACCESSORY)
            continue;

        for (size_t prev = 0; prev < i; prev++)
        {
            const X2mItem &prev_item = costume_items[prev];

            if (prev_item.item_type == X2mItemType::ACCESSORY)
                continue;

            if (prev_item.item_name == item.item_name)
            {
                item.idb.name_id = prev_item.idb.name_id;
                name_found = true;
                break;
            }
        }

        if (name_found)
            continue;

        for (int l = 0; l < XV2_LANG_NUM; l++)
        {
            const std::string *name;

            if (item.item_name[l].length() != 0)
                name = &item.item_name[l];
            else
                name = &item.item_name[XV2_LANG_ENGLISH];            

            if (!Xenoverse2::AddCacCostumeName(ChooseCostumeEntryName(item.idb), *name, l, &item.idb.name_id))
            {
                DPRINTF("%s: AddCacCostumeName failed.\n", FUNCNAME);
                return false;
            }
        }
    }

    return true;
}

static void RemoveAccessoryNameReferences(uint16_t name_id)
{
    for (IdbEntry &entry : *game_accesory_idb)
    {
        if (entry.name_id == name_id)
            entry.name_id = 0xFFFF;
    }
}

bool X2mFile::InstallCostumeAccessoryNames()
{
    if (type != X2mType::NEW_COSTUME)
        return false;

    if (update_costume_entry.idb_entries.size() != 0 && update_costume_entry.idb_entries.size() != costume_items.size())
    {
        DPRINTF("%s: This function shouldn't have been called in this state.\n", FUNCNAME);
        return false;
    }

    if (update_costume_entry.partsets.size() == 0)
    {
        DPRINTF("%s: This function should have been called after InstallCostumePartSet.\n", FUNCNAME);
        return false;
    }

    // Delete any existing name before
    for (size_t i = 0; i < update_costume_entry.idb_entries.size(); i++)
    {
        uint16_t idb_id = update_costume_entry.idb_entries[i];
        uint8_t type = update_costume_entry.costume_types[i];

        if (type != COSTUME_ACCESSORY)
            continue;

        IdbEntry *entry = game_accesory_idb->FindEntryByID(idb_id);
        if (!entry)
        {
            DPRINTF("%s: Internal error, cannot find installed idb entry for update.\n", FUNCNAME);
            return false;
        }

        uint16_t name_id = entry->name_id;
        if (name_id == 0xFFFF)
            continue;

        RemoveAccessoryNameReferences(name_id);

        for (int l = 0; l < XV2_LANG_NUM; l++)
        {
            if (!Xenoverse2::RemoveAccesoryName(name_id, l, (l==0)))
            {
                DPRINTF("%s: Internal error, fail in RemoveAccesoryName.\n", FUNCNAME);
                return false;
            }
        }
    }

    // Now add new names
    for (size_t i = 0; i < costume_items.size(); i++)
    {
        X2mItem &item = costume_items[i];
        bool name_found = false;

        if (item.item_type != X2mItemType::ACCESSORY)
            continue;

        for (size_t prev = 0; prev < i; prev++)
        {
            const X2mItem &prev_item = costume_items[prev];

            if (prev_item.item_type != X2mItemType::ACCESSORY)
                continue;

            if (prev_item.item_name == item.item_name)
            {
                item.idb.name_id = prev_item.idb.name_id;
                name_found = true;
                break;
            }
        }

        if (name_found)
            continue;

        for (int l = 0; l < XV2_LANG_NUM; l++)
        {
            const std::string *name;

            if (item.item_name[l].length() != 0)
                name = &item.item_name[l];
            else
                name = &item.item_name[XV2_LANG_ENGLISH];

            if (!Xenoverse2::AddAccesoryName(*name, l, &item.idb.name_id))
            {
                DPRINTF("%s: AddAccesoryName failed.\n", FUNCNAME);
                return false;
            }
        }
    }

    return true;
}

static void RemoveCostumeDescReferences(uint16_t desc_id)
{
    for (IdbEntry &entry : *game_top_idb)
    {
        if (entry.desc_id == desc_id)
            entry.desc_id = 0xFFFF;
    }

    for (IdbEntry &entry : *game_bottom_idb)
    {
        if (entry.desc_id == desc_id)
            entry.desc_id = 0xFFFF;
    }

    for (IdbEntry &entry : *game_gloves_idb)
    {
        if (entry.desc_id == desc_id)
            entry.desc_id = 0xFFFF;
    }

    for (IdbEntry &entry : *game_shoes_idb)
    {
        if (entry.desc_id == desc_id)
            entry.desc_id = 0xFFFF;
    }
}

static std::string ChooseCostumeEntryDesc(const IdbEntry &idb)
{
    std::string str = "wear_";

    if (idb.racelock == IDB_RACE_HUM || idb.racelock == IDB_RACE_SYM || idb.racelock == (IDB_RACE_HUM|IDB_RACE_SYM))
    {
        str += "hum_";
    }
    else if (idb.racelock == IDB_RACE_HUF || idb.racelock == IDB_RACE_SYF || idb.racelock == (IDB_RACE_HUF|IDB_RACE_SYF))
    {
        str += "huf_";
    }
    else if (idb.racelock == IDB_RACE_NMC)
    {
        str += "nmc_";
    }
    else if (idb.racelock == IDB_RACE_FRI)
    {
        str += "fri_";
    }
    else if (idb.racelock == IDB_RACE_MAM)
    {
        str += "mam_";
    }
    else if (idb.racelock == IDB_RACE_MAF)
    {
        str += "maf_";
    }
    else if (idb.racelock == (IDB_RACE_MAM|IDB_RACE_MAF))
    {
        str += "mar_";
    }
    else
    {
        str += "cmn_";
    }

    str += "eff_";
    return str;
}

bool X2mFile::InstallCostumeCostumeDescs()
{
    if (type != X2mType::NEW_COSTUME)
        return false;

    if (update_costume_entry.idb_entries.size() != 0 && update_costume_entry.idb_entries.size() != costume_items.size())
    {
        DPRINTF("%s: This function shouldn't have been called in this state.\n", FUNCNAME);
        return false;
    }

    if (update_costume_entry.partsets.size() == 0)
    {
        DPRINTF("%s: This function should have been called after InstallCostumePartSet.\n", FUNCNAME);
        return false;
    }

    // Delete any existing desc before
    for (size_t i = 0; i < update_costume_entry.idb_entries.size(); i++)
    {
        uint16_t idb_id = update_costume_entry.idb_entries[i];
        uint8_t type = update_costume_entry.costume_types[i];

        if (type == COSTUME_ACCESSORY)
            continue;

        IdbEntry *entry = nullptr;

        if (type == COSTUME_TOP)
        {
            entry = game_top_idb->FindEntryByID(idb_id);
        }
        else if (type == COSTUME_BOTTOM)
        {
            entry = game_bottom_idb->FindEntryByID(idb_id);
        }
        else if (type == COSTUME_GLOVES)
        {
            entry = game_gloves_idb->FindEntryByID(idb_id);
        }
        else if (type == COSTUME_SHOES)
        {
            entry = game_shoes_idb->FindEntryByID(idb_id);
        }

        if (!entry)
        {
            DPRINTF("%s: Internal error, cannot find installed idb entry for update.\n", FUNCNAME);
            return false;
        }

        uint16_t desc_id = entry->desc_id;
        if (desc_id == 0xFFFF)
            continue;

        RemoveCostumeDescReferences(desc_id);

        for (int l = 0; l < XV2_LANG_NUM; l++)
        {
            if (!Xenoverse2::RemoveCacCostumeDesc(desc_id, l, (l==0)))
            {
                DPRINTF("%s: Internal error, fail in RemoveCacCostumeDesc.\n", FUNCNAME);
                return false;
            }
        }
    }

    // Now add new descs
    for (size_t i = 0; i < costume_items.size(); i++)
    {
        X2mItem &item = costume_items[i];
        bool desc_found = false;

        if (item.item_type == X2mItemType::ACCESSORY)
            continue;

        for (size_t prev = 0; prev < i; prev++)
        {
            const X2mItem &prev_item = costume_items[prev];

            if (prev_item.item_type == X2mItemType::ACCESSORY)
                continue;

            if (prev_item.item_desc == item.item_desc)
            {
                item.idb.desc_id = prev_item.idb.desc_id;
                desc_found = true;
                break;
            }
        }

        if (desc_found)
            continue;

        for (int l = 0; l < XV2_LANG_NUM; l++)
        {
            const std::string *desc;

            if (item.item_desc[l].length() != 0)
                desc = &item.item_desc[l];
            else
                desc = &item.item_desc[XV2_LANG_ENGLISH];

            if (!Xenoverse2::AddCacCostumeDesc(ChooseCostumeEntryDesc(item.idb), *desc, l, &item.idb.desc_id))
            {
                DPRINTF("%s: AddCacCostumeDesc failed.\n", FUNCNAME);
                return false;
            }
        }
    }

    return true;
}

static void RemoveAccessoryDescReferences(uint16_t desc_id)
{
    for (IdbEntry &entry : *game_accesory_idb)
    {
        if (entry.desc_id == desc_id)
            entry.desc_id = 0xFFFF;
    }
}

bool X2mFile::InstallCostumeAccessoryDescs()
{
    if (type != X2mType::NEW_COSTUME)
        return false;

    if (update_costume_entry.idb_entries.size() != 0 && update_costume_entry.idb_entries.size() != costume_items.size())
    {
        DPRINTF("%s: This function shouldn't have been called in this state.\n", FUNCNAME);
        return false;
    }

    if (update_costume_entry.partsets.size() == 0)
    {
        DPRINTF("%s: This function should have been called after InstallCostumePartSet.\n", FUNCNAME);
        return false;
    }

    // Delete any existing desc before
    for (size_t i = 0; i < update_costume_entry.idb_entries.size(); i++)
    {
        uint16_t idb_id = update_costume_entry.idb_entries[i];
        uint8_t type = update_costume_entry.costume_types[i];

        if (type != COSTUME_ACCESSORY)
            continue;

        IdbEntry *entry = game_accesory_idb->FindEntryByID(idb_id);
        if (!entry)
        {
            DPRINTF("%s: Internal error, cannot find installed idb entry for update.\n", FUNCNAME);
            return false;
        }

        uint16_t desc_id = entry->desc_id;
        if (desc_id == 0xFFFF)
            continue;

        RemoveAccessoryDescReferences(desc_id);

        for (int l = 0; l < XV2_LANG_NUM; l++)
        {
            if (!Xenoverse2::RemoveAccesoryDesc(desc_id, l, (l==0)))
            {
                DPRINTF("%s: Internal error, fail in RemoveAccesoryDesc.\n", FUNCNAME);
                return false;
            }
        }
    }

    // Now add new descs
    for (size_t i = 0; i < costume_items.size(); i++)
    {
        X2mItem &item = costume_items[i];
        bool desc_found = false;

        if (item.item_type != X2mItemType::ACCESSORY)
            continue;

        for (size_t prev = 0; prev < i; prev++)
        {
            const X2mItem &prev_item = costume_items[prev];

            if (prev_item.item_type != X2mItemType::ACCESSORY)
                continue;

            if (prev_item.item_desc == item.item_desc)
            {
                item.idb.desc_id = prev_item.idb.desc_id;
                desc_found = true;
                break;
            }
        }

        if (desc_found)
            continue;

        for (int l = 0; l < XV2_LANG_NUM; l++)
        {
            const std::string *desc;

            if (item.item_desc[l].length() != 0)
                desc = &item.item_desc[l];
            else
                desc = &item.item_desc[XV2_LANG_ENGLISH];

            if (!Xenoverse2::AddAccesoryDesc(*desc, l, &item.idb.desc_id))
            {
                DPRINTF("%s: AddAccesoryDesc failed.\n", FUNCNAME);
                return false;
            }
        }
    }

    return true;
}

bool X2mFile::AddCostumeIdb(IdbFile *cost_idb, IdbEntry &entry, uint16_t id_start)
{
    static const std::vector<IdbFile *> idb_files =
    {
        game_top_idb, game_bottom_idb, game_gloves_idb, game_shoes_idb
    };

    for (entry.id = id_start; entry.id != 0x8000; entry.id++)
    {
        bool found = true;

        for (IdbFile *idb : idb_files)
        {
            if (idb->FindEntryByID(entry.id))
            {
                found = false;
                break;
            }
        }

        if (found)
        {
            return cost_idb->AddEntry(entry,  false);
        }
    }

    return false;
}

bool X2mFile::InstallCostumeIdb()
{
    if (type != X2mType::NEW_COSTUME)
        return false;

    if (update_costume_entry.partsets.size() == 0)
    {
        DPRINTF("%s: This function should have been called after InstallCostumePartSet.\n", FUNCNAME);
        return false;
    }

    // Assign partsets before
    for (X2mItem &item : costume_items)
    {
        item.idb.model += update_costume_entry.partsets[0];
    }

    static const uint16_t idb_start_costume = 0x10C;
    static const uint16_t idb_start_accessory = 0x6C;

    if (update_costume_entry.idb_entries.size() == 0)
    {
        if (costume_items.size() == 0)
            return true;

        for (size_t i = 0; i < costume_items.size(); i++)
        {
            X2mItem &item = costume_items[i];
            X2mItemType type = item.item_type;

            if (item.idb.name_id == 0xFFFF || item.idb.name_id == X2M_INVALID_ID16 || item.idb.name_id == X2M_DUMMY_ID16)
            {
                DPRINTF("%s: Name should have been assigned before.\n", FUNCNAME);
                return false;
            }

            if (item.idb.desc_id == 0xFFFF || item.idb.desc_id == X2M_INVALID_ID16 || item.idb.desc_id == X2M_DUMMY_ID16)
            {
                DPRINTF("%s: Desc should have been assigned before.\n", FUNCNAME);
                return false;
            }

            if (type == X2mItemType::TOP)
            {
                if (!AddCostumeIdb(game_top_idb, item.idb, idb_start_costume))
                {
                    DPRINTF("%s: AddCostumeIdb failed.\n", FUNCNAME);
                    return false;
                }

                update_costume_entry.costume_types.push_back(COSTUME_TOP);
            }
            else if (type == X2mItemType::BOTTOM)
            {
                if (!AddCostumeIdb(game_bottom_idb, item.idb, idb_start_costume))
                {
                    DPRINTF("%s: AddCostumeIdb failed.\n", FUNCNAME);
                    return false;
                }

                update_costume_entry.costume_types.push_back(COSTUME_BOTTOM);
            }
            else if (type == X2mItemType::GLOVES)
            {
                if (!AddCostumeIdb(game_gloves_idb, item.idb, idb_start_costume))
                {
                    DPRINTF("%s: AddCostumeIdb failed.\n", FUNCNAME);
                    return false;
                }
                update_costume_entry.costume_types.push_back(COSTUME_GLOVES);
            }
            else if (type == X2mItemType::SHOES)
            {
                if (!AddCostumeIdb(game_shoes_idb, item.idb, idb_start_costume))
                {
                    DPRINTF("%s: AddCostumeIdb failed.\n", FUNCNAME);
                    return false;
                }

                update_costume_entry.costume_types.push_back(COSTUME_SHOES);
            }
            else
            {
                if (!game_accesory_idb->AddEntryAuto(item.idb, idb_start_accessory))
                {
                    DPRINTF("%s: AddEntryAuto failed.\n", FUNCNAME);
                    return false;
                }

                update_costume_entry.costume_types.push_back(COSTUME_ACCESSORY);
            }

            update_costume_entry.idb_entries.push_back(item.idb.id);
        }
    }
    else
    {
        if (update_costume_entry.idb_entries.size() != costume_items.size())
        {
            DPRINTF("%s: This function shouldn't have been called in this state.\n", FUNCNAME);
            return false;
        }

        for (size_t i = 0; i < costume_items.size(); i++)
        {
            X2mItem &item = costume_items[i];
            X2mItemType type = item.item_type;

            if (item.idb.name_id == 0xFFFF || item.idb.name_id == X2M_INVALID_ID16 || item.idb.name_id == X2M_DUMMY_ID16)
            {
                DPRINTF("%s: Name should have been assigned before (on update).\n", FUNCNAME);
                return false;
            }

            if (item.idb.desc_id == 0xFFFF || item.idb.desc_id == X2M_INVALID_ID16 || item.idb.desc_id == X2M_DUMMY_ID16)
            {
                DPRINTF("%s: Desc should have been assigned before (on update).\n", FUNCNAME);
                return false;
            }

            if (type == X2mItemType::TOP)
            {
                if (update_costume_entry.costume_types[i] != COSTUME_TOP)
                {
                    DPRINTF("%s: Mismatch on update costume type, this should have been caught before.\n", FUNCNAME);
                    return false;
                }

                item.idb.id = update_costume_entry.idb_entries[i];
                IdbEntry *entry = game_top_idb->FindEntryByID(item.idb.id);
                if (!entry)
                {
                    DPRINTF("%s: Failed to get existing idb entry, this should have been caught before.\n", FUNCNAME);
                    return false;
                }

                *entry = item.idb; // Update
            }
            else if (type == X2mItemType::BOTTOM)
            {
                if (update_costume_entry.costume_types[i] != COSTUME_BOTTOM)
                {
                    DPRINTF("%s: Mismatch on update costume type, this should have been caught before.\n", FUNCNAME);
                    return false;
                }

                item.idb.id = update_costume_entry.idb_entries[i];
                IdbEntry *entry = game_bottom_idb->FindEntryByID(item.idb.id);
                if (!entry)
                {
                    DPRINTF("%s: Failed to get existing idb entry, this should have been caught before.\n", FUNCNAME);
                    return false;
                }

                *entry = item.idb; // Update
            }
            else if (type == X2mItemType::GLOVES)
            {
                if (update_costume_entry.costume_types[i] != COSTUME_GLOVES)
                {
                    DPRINTF("%s: Mismatch on update costume type, this should have been caught before.\n", FUNCNAME);
                    return false;
                }

                item.idb.id = update_costume_entry.idb_entries[i];
                IdbEntry *entry = game_gloves_idb->FindEntryByID(item.idb.id);
                if (!entry)
                {
                    DPRINTF("%s: Failed to get existing idb entry, this should have been caught before.\n", FUNCNAME);
                    return false;
                }

                *entry = item.idb; // Update
            }
            else if (type == X2mItemType::SHOES)
            {
                if (update_costume_entry.costume_types[i] != COSTUME_SHOES)
                {
                    DPRINTF("%s: Mismatch on update costume type, this should have been caught before.\n", FUNCNAME);
                    return false;
                }

                item.idb.id = update_costume_entry.idb_entries[i];
                IdbEntry *entry = game_shoes_idb->FindEntryByID(item.idb.id);
                if (!entry)
                {
                    DPRINTF("%s: Failed to get existing idb entry, this should have been caught before.\n", FUNCNAME);
                    return false;
                }

                *entry = item.idb; // Update
            }
            else
            {
                if (update_costume_entry.costume_types[i] != COSTUME_ACCESSORY)
                {
                    DPRINTF("%s: Mismatch on update costume type, this should have been caught before.\n", FUNCNAME);
                    return false;
                }

                item.idb.id = update_costume_entry.idb_entries[i];
                IdbEntry *entry = game_accesory_idb->FindEntryByID(item.idb.id);
                if (!entry)
                {
                    DPRINTF("%s: Failed to get existing idb entry, this should have been caught before.\n", FUNCNAME);
                    return false;
                }

                *entry = item.idb; // Update
            }
        }
    }

    return true;
}

bool X2mFile::InstallCostumeFile()
{
    if (type != X2mType::NEW_COSTUME)
        return false;

    if (update_costume_entry.partsets.size() == 0)
    {
        DPRINTF("%s: This function should have been called after InstallCostumePartSet.\n", FUNCNAME);
        return false;
    }

    memcpy(update_costume_entry.guid, mod_guid, sizeof(mod_guid));
    game_costume_file->AddCostume(update_costume_entry);

    return true;
}

bool X2mFile::InstallCostumeVisitor(const std::string &path, void *param)
{
    std::string name = Utils::GetFileNameString(path);
    std::string output_path = "data/chara/" + path.substr(0, 3) + "/";
    X2mFile *pthis = (X2mFile *)param;

    if (!ResolvePartSetString(name, pthis->update_costume_entry.partsets[0], (uint16_t)pthis->update_costume_entry.partsets.size()))
        return true; // Skip this file

    output_path += name;

    size_t size;
    uint8_t *buf = pthis->ReadFile(path, &size);
    if (!buf)
        return false;

    if (!xv2fs->WriteFile(output_path, buf, size))
        return false;

    return true;
}

bool X2mFile::InstallCostumeFiles()
{
    if (type != X2mType::NEW_COSTUME)
        return false;

    if (update_costume_entry.partsets.size() == 0)
    {
        DPRINTF("%s: This function should have been called after InstallCostumePartSet.\n", FUNCNAME);
        return false;
    }

    for (uint8_t race = 0; race < X2M_CR_NUM; race++)
    {
        if (GetNumCostumePartSets(race) > 0)
        {
            if (DirExists(x2m_cr_code[race]))
            {
                if (!VisitDirectory(x2m_cr_code[race], InstallCostumeVisitor, this))
                    return false;
            }
        }
    }

    return true;
}

bool X2mFile::UninstallCostumePartSets()
{
    if (type != X2mType::NEW_COSTUME)
        return false;

    for (uint16_t partset : update_costume_entry.partsets)
    {
        for (uint8_t race : update_costume_entry.races)
        {
            BcsFile *bcs;

            if (race == X2M_CR_HUM_SYM)
                bcs = game_hum_bcs;
            else if (race == X2M_CR_HUF_SYF)
                bcs = game_huf_bcs;
            else if (race == X2M_CR_NMC)
                bcs = game_nmc_bcs;
            else if (race == X2M_CR_FRI)
                bcs = game_fri_bcs;
            else if (race == X2M_CR_MAM)
                bcs = game_mam_bcs;
            else
                bcs = game_maf_bcs;

            std::vector<BcsPartSet> &partsets = bcs->GetPartSets();

            if (partset < partsets.size())
            {
                partsets[partset].valid = false;
            }
        }
    }

    return true;
}

bool X2mFile::UninstallCostumeCostumeNames()
{
    if (type != X2mType::NEW_COSTUME)
        return false;

    if (update_costume_entry.idb_entries.size() != update_costume_entry.costume_types.size())
        return false;

    for (size_t i = 0; i < update_costume_entry.idb_entries.size(); i++)
    {
        uint16_t id = update_costume_entry.idb_entries[i];
        uint8_t type = update_costume_entry.costume_types[i];

        if (type == COSTUME_ACCESSORY)
            continue;

        IdbFile *idb;

        if (type == COSTUME_TOP)
            idb = game_top_idb;
        else if (type == COSTUME_BOTTOM)
            idb = game_bottom_idb;
        else if (type == COSTUME_GLOVES)
            idb = game_gloves_idb;
        else
            idb = game_shoes_idb;

        IdbEntry *entry = idb->FindEntryByID(id);
        if (entry)
        {
            uint16_t name_id = entry->name_id;
            if (name_id == 0xFFFF)
                continue;

            RemoveCostumeNameReferences(name_id);

            for (int l = 0; l < XV2_LANG_NUM; l++)
            {
                if (!Xenoverse2::RemoveCacCostumeName(name_id, l, (l==0)))
                {
                    DPRINTF("%s: RemoveCacCostumeName failed.\n", FUNCNAME);
                    return false;
                }
            }

        }
    }

    return true;
}

bool X2mFile::UninstallCostumeAccessoryNames()
{
    if (type != X2mType::NEW_COSTUME)
        return false;

    if (update_costume_entry.idb_entries.size() != update_costume_entry.costume_types.size())
        return false;

    for (size_t i = 0; i < update_costume_entry.idb_entries.size(); i++)
    {
        uint16_t id = update_costume_entry.idb_entries[i];
        uint8_t type = update_costume_entry.costume_types[i];

        if (type != COSTUME_ACCESSORY)
            continue;

        IdbEntry *entry = game_accesory_idb->FindEntryByID(id);
        if (entry)
        {
            uint16_t name_id = entry->name_id;
            if (name_id == 0xFFFF)
                continue;

            RemoveAccessoryNameReferences(name_id);

            for (int l = 0; l < XV2_LANG_NUM; l++)
            {
                if (!Xenoverse2::RemoveAccesoryName(name_id, l, (l==0)))
                {
                    DPRINTF("%s: RemoveAccesoryName failed.\n", FUNCNAME);
                    return false;
                }
            }

        }
    }

    return true;
}

bool X2mFile::UninstallCostumeCostumeDescs()
{
    if (type != X2mType::NEW_COSTUME)
        return false;

    if (update_costume_entry.idb_entries.size() != update_costume_entry.costume_types.size())
        return false;

    for (size_t i = 0; i < update_costume_entry.idb_entries.size(); i++)
    {
        uint16_t id = update_costume_entry.idb_entries[i];
        uint8_t type = update_costume_entry.costume_types[i];

        if (type == COSTUME_ACCESSORY)
            continue;

        IdbFile *idb;

        if (type == COSTUME_TOP)
            idb = game_top_idb;
        else if (type == COSTUME_BOTTOM)
            idb = game_bottom_idb;
        else if (type == COSTUME_GLOVES)
            idb = game_gloves_idb;
        else
            idb = game_shoes_idb;

        IdbEntry *entry = idb->FindEntryByID(id);
        if (entry)
        {
            uint16_t desc_id = entry->desc_id;
            if (desc_id == 0xFFFF)
                continue;

            RemoveCostumeDescReferences(desc_id);

            for (int l = 0; l < XV2_LANG_NUM; l++)
            {
                if (!Xenoverse2::RemoveCacCostumeDesc(desc_id, l, (l==0)))
                {
                    DPRINTF("%s: RemoveCacCostumeDesc failed.\n", FUNCNAME);
                    return false;
                }
            }

        }
    }

    return true;
}

bool X2mFile::UninstallCostumeAccessoryDescs()
{
    if (type != X2mType::NEW_COSTUME)
        return false;

    if (update_costume_entry.idb_entries.size() != update_costume_entry.costume_types.size())
        return false;

    for (size_t i = 0; i < update_costume_entry.idb_entries.size(); i++)
    {
        uint16_t id = update_costume_entry.idb_entries[i];
        uint8_t type = update_costume_entry.costume_types[i];

        if (type != COSTUME_ACCESSORY)
            continue;

        IdbEntry *entry = game_accesory_idb->FindEntryByID(id);
        if (entry)
        {
            uint16_t desc_id = entry->desc_id;
            if (desc_id == 0xFFFF)
                continue;

            RemoveAccessoryDescReferences(desc_id);

            for (int l = 0; l < XV2_LANG_NUM; l++)
            {
                if (!Xenoverse2::RemoveAccesoryDesc(desc_id, l, (l==0)))
                {
                    DPRINTF("%s: RemoveAccesoryDesc failed.\n", FUNCNAME);
                    return false;
                }
            }

        }
    }

    return true;
}

bool X2mFile::UninstallCostumeIdb()
{
    if (type != X2mType::NEW_COSTUME)
        return false;

    if (update_costume_entry.idb_entries.size() != update_costume_entry.costume_types.size())
        return false;

    for (size_t i = 0; i < update_costume_entry.idb_entries.size(); i++)
    {
        uint16_t id = update_costume_entry.idb_entries[i];
        uint8_t type = update_costume_entry.costume_types[i];

        IdbFile *idb;

        if (type == COSTUME_TOP)
            idb = game_top_idb;
        else if (type == COSTUME_BOTTOM)
            idb = game_bottom_idb;
        else if (type == COSTUME_GLOVES)
            idb = game_gloves_idb;
        else if (type == COSTUME_SHOES)
            idb = game_shoes_idb;
        else
            idb = game_accesory_idb;

        idb->RemoveEntry(id);
    }

    return true;
}

bool X2mFile::UninstallCostumeFile()
{
    if (type != X2mType::NEW_COSTUME)
        return false;

    game_costume_file->RemoveCostume(mod_guid);
    return true;
}

bool X2mFile::UninstallCostumeVisitor(const std::string &path, void *param)
{
    std::string name = Utils::GetFileNameString(path);
    std::string delete_path = "data/chara/" + path.substr(0, 3) + "/";
    X2mFile *pthis = (X2mFile *)param;

    if (!ResolvePartSetString(name, pthis->update_costume_entry.partsets[0], (uint16_t)pthis->update_costume_entry.partsets.size()))
        return true; // Skip this file

    delete_path += name;
    xv2fs->RemoveFile(delete_path);

    return true;
}

bool X2mFile::UninstallCostumeFiles()
{
    if (type != X2mType::NEW_COSTUME)
        return false;

    if (update_costume_entry.partsets.size() == 0)
        return true; // Yes, true

    for (uint8_t race : update_costume_entry.races)
    {
        if (DirExists(x2m_cr_code[race]))
        {
            if (!VisitDirectory(x2m_cr_code[race], UninstallCostumeVisitor, this))
                return false;
        }
    }

    return true;
}
