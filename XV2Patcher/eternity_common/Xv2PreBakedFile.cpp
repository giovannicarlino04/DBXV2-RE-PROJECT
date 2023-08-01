#include <algorithm>
#include "Xv2PreBakedFile.h"
#include "debug.h"

#define CUS_DATA_ID_START   0x30

TiXmlElement *BodyShape::Decompile(TiXmlNode *root) const
{
    TiXmlElement *entry_root = new TiXmlElement("BodyShape");

    entry_root->SetAttribute("cms_id", Utils::UnsignedToString(cms_entry, true));
    entry_root->SetAttribute("body_shape", body_shape);

    root->LinkEndChild(entry_root);
    return entry_root;
}

bool BodyShape::Compile(const TiXmlElement *root)
{
    if (!Utils::ReadAttrUnsigned(root, "cms_id", &cms_entry))
    {
        DPRINTF("%s: cms_id attribute is not optional.\n", FUNCNAME);
        return false;
    }

    if (!Utils::ReadAttrUnsigned(root, "body_shape", &body_shape))
    {
        DPRINTF("%s: body_shape attribute is not optional.\n", FUNCNAME);
        return false;
    }

    return true;
}

TiXmlElement *CusAuraData::Decompile(TiXmlNode *root) const
{
    TiXmlElement *entry_root = new TiXmlElement("CusAuraData");

    entry_root->SetAttribute("cus_aura_id", Utils::UnsignedToString(cus_aura_id, true));
    entry_root->SetAttribute("aur_aura_id", Utils::UnsignedToString(aur_aura_id, true));

    Utils::WriteParamUnsigned(entry_root, "BEHAVIOUR_11", behaviour_11, true);
    Utils::WriteParamUnsigned(entry_root, "INTEGER_2", integer_2);
    Utils::WriteParamUnsigned(entry_root, "BEHAVIOUR_10", behaviour_10, true);
    Utils::WriteParamUnsigned(entry_root, "INTEGER_3", integer_3);
    Utils::WriteParamBoolean(entry_root, "FORCE_TELEPORT", force_teleport);
    Utils::WriteParamUnsigned(entry_root, "BEHAVIOUR_13", behaviour_13, true);

    root->LinkEndChild(entry_root);
    return entry_root;
}

bool CusAuraData::Compile(const TiXmlElement *root)
{
    if (!Utils::ReadAttrUnsigned(root, "cus_aura_id", &cus_aura_id))
    {
        DPRINTF("%s: cus_aura_id attribute is not optional.\n", FUNCNAME);
        return false;
    }

    if (!Utils::ReadAttrUnsigned(root, "aur_aura_id", &aur_aura_id))
    {
        DPRINTF("%s: aur_aura_id attribute is not optional.\n", FUNCNAME);
        return false;
    }

    if (!Utils::GetParamUnsigned(root, "BEHAVIOUR_11", &behaviour_11))
        return false;

    if (behaviour_11 > 0x15)
    {
        DPRINTF("%s: BEHAVIOUR_11 cannot be greater than 0x15.\n", FUNCNAME);
        return false;
    }

    if (!Utils::GetParamUnsigned(root, "INTEGER_2", &integer_2))
        return false;

    if (Utils::ReadParamUnsigned(root, "BEHAVIOUR_10", &behaviour_10))
    {
        if (behaviour_10 > 0x15)
        {
            DPRINTF("%s: BEHAVIOUR_10 cannot be greater than 0x15.\n", FUNCNAME);
            return false;
        }
    }
    else
    {
        behaviour_10 = behaviour_11;
    }

    if (!Utils::ReadParamUnsigned(root, "INTEGER_3", &integer_3))
    {
        integer_3 = 0;
    }

    if (!Utils::ReadParamBoolean(root, "FORCE_TELEPORT", &force_teleport))
    {
        force_teleport = false;
    }

    if (Utils::ReadParamUnsigned(root, "BEHAVIOUR_13", &behaviour_13))
    {
        if (behaviour_13 > 0x15)
        {
            DPRINTF("%s: BEHAVIOUR_13 cannot be greater than 0x15.\n", FUNCNAME);
            return false;
        }
    }
    else
    {
        behaviour_13 = behaviour_11;
    }

    return true;
}

Xv2PreBakedFile::Xv2PreBakedFile()
{

}

Xv2PreBakedFile::~Xv2PreBakedFile()
{

}

void Xv2PreBakedFile::Reset()
{
    ozarus.clear();
    auto_btl_portrait_list.clear();
    body_shapes.clear();
    cus_aura_datas.clear();
}

TiXmlDocument *Xv2PreBakedFile::Decompile() const
{
    TiXmlDocument *doc = new TiXmlDocument();

    TiXmlDeclaration* decl = new TiXmlDeclaration("1.0", "utf-8", "" );
    doc->LinkEndChild(decl);

    TiXmlElement *root = new TiXmlElement("Xv2PreBaked");
    Utils::WriteComment(root, "This file is used by xv2patcher and XV2 Mods Installer. It defines characters data that only exist in exe at runtime. ");
    Utils::WriteComment(root, "--------------------------------------------");

    Utils::WriteComment(root, "This is the list of additional ozarus to add to the game.");
    Utils::WriteParamMultipleStrings(root, "OZARUS", ozarus);

    Utils::WriteComment(root, "This is the list of characters that will have an auto battle portrait.");
    Utils::WriteParamMultipleUnsigned(root, "AUTO_BTL_PORT", auto_btl_portrait_list, true);

    if (body_shapes.size() != 0)
        Utils::WriteComment(root, "This is the list of body shapes to apply.");

    for (const BodyShape &shape : body_shapes)
    {
        shape.Decompile(root);
    }

    if (cus_aura_datas.size() != 0)
        Utils::WriteComment(root, "This is the data of cus auras.");

    for (const CusAuraData &data : cus_aura_datas)
    {
        data.Decompile(root);
    }

    doc->LinkEndChild(root);
    return doc;
}

bool Xv2PreBakedFile::Compile(TiXmlDocument *doc, bool)
{
    Reset();

    TiXmlHandle handle(doc);
    const TiXmlElement *root = Utils::FindRoot(&handle, "Xv2PreBaked");

    if (!root)
    {
        DPRINTF("Cannot find\"Xv2PreBaked\" in xml.\n");
        return false;
    }

    Utils::ReadParamMultipleStrings(root, "OZARUS", ozarus);
    Utils::ReadParamMultipleUnsigned(root, "AUTO_BTL_PORT", auto_btl_portrait_list);

    for (const TiXmlElement *elem = root->FirstChildElement(); elem; elem = elem->NextSiblingElement())
    {
        const std::string &name = elem->ValueStr();

        if (name == "BodyShape")
        {
            BodyShape shape;

            if (!shape.Compile(elem))
                return false;

            bool add_it = true;

            for (const BodyShape &bs : body_shapes)
            {
                if (bs.cms_entry == shape.cms_entry)
                {
                    add_it = false;
                    break;
                }
            }

            if (add_it)
                body_shapes.push_back(shape);
        }
        else if (name == "CusAuraData")
        {
            CusAuraData new_data;

            if (!new_data.Compile(elem))
                return false;

            bool add_it = true;

            for (const CusAuraData &data : cus_aura_datas)
            {
                if (data.cus_aura_id == new_data.cus_aura_id)
                {
                    add_it = false;
                    break;
                }
            }

            if (add_it)
                cus_aura_datas.push_back(new_data);
        }
    }

    return true;
}

void Xv2PreBakedFile::AddOzaru(const std::string &ozaru)
{
    if (std::find(ozarus.begin(), ozarus.end(), ozaru) != ozarus.end())
        return;

    ozarus.push_back(ozaru);
}

void Xv2PreBakedFile::RemoveOzaru(const std::string &ozaru)
{
    for (size_t i = 0; i < ozarus.size(); i++)
    {
        if (ozarus[i] == ozaru)
        {
            ozarus.erase(ozarus.begin()+i);
            i--;
        }
    }
}

void Xv2PreBakedFile::AddAutoBtlPortrait(uint32_t cms_entry)
{
    if (std::find(auto_btl_portrait_list.begin(), auto_btl_portrait_list.end(), cms_entry) != auto_btl_portrait_list.end())
        return;

    auto_btl_portrait_list.push_back(cms_entry);
}

void Xv2PreBakedFile::RemoveAutoBtlPortrait(uint32_t cms_entry)
{
    for (size_t i = 0; i < auto_btl_portrait_list.size(); i++)
    {
        if (auto_btl_portrait_list[i] == cms_entry)
        {
            auto_btl_portrait_list.erase(auto_btl_portrait_list.begin()+i);
            i--;
        }
    }
}

void Xv2PreBakedFile::AddBodyShape(uint32_t cms_entry, uint32_t body_shape)
{
    BodyShape shape;

    shape.cms_entry = cms_entry;
    shape.body_shape = body_shape;

    body_shapes.push_back(shape);
}

void Xv2PreBakedFile::RemoveBodyShape(uint32_t cms_entry)
{
    for (size_t i = 0; i < body_shapes.size(); i++)
    {
        if (body_shapes[i].cms_entry == cms_entry)
        {
            body_shapes.erase(body_shapes.begin()+i);
            i--;
        }
    }
}

CusAuraData *Xv2PreBakedFile::FindAuraData(uint16_t cus_aura_id)
{
    for (CusAuraData &data : cus_aura_datas)
    {
        if (data.cus_aura_id == cus_aura_id)
            return &data;
    }

    return nullptr;
}

CusAuraData *Xv2PreBakedFile::FindAuraDataByAurId(uint16_t aur_aura_id)
{
    for (CusAuraData &data : cus_aura_datas)
    {
        if (data.aur_aura_id == aur_aura_id)
            return &data;
    }

    return nullptr;
}

bool Xv2PreBakedFile::AddConsecutiveAuraData(std::vector<CusAuraData> &datas)
{
    if (datas.size() == 0)
        return true; // Yes, true

    uint16_t id;

    for (id = CUS_DATA_ID_START; ; id++)
    {
        bool found = true;

        for (uint32_t i = 0; i < (uint32_t)datas.size(); i++)
        {
            if (FindAuraData(id+i))
            {
                found = false;
                break;
            }
        }

        if (found)
            break;
    }

    for (CusAuraData &data : datas)
    {
        data.cus_aura_id = id;
        cus_aura_datas.push_back(data);
        id++;
    }

    return true;
}

void Xv2PreBakedFile::RemoveAuraData(uint16_t cus_aura_id)
{
    for (size_t i = 0; i < cus_aura_datas.size(); i++)
    {
        if (cus_aura_datas[i].cus_aura_id == cus_aura_id)
        {
            cus_aura_datas.erase(cus_aura_datas.begin()+i);
            i--;
        }
    }
}

