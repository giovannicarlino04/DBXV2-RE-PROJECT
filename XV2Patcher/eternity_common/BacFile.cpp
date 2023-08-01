#include "BacFile.h"
#include "debug.h"

TiXmlElement *BACAnimation::Decompile(TiXmlNode *root) const
{
    TiXmlElement *entry_root = new TiXmlElement("Animation");

    Utils::WriteParamUnsigned(entry_root, "START_TIME", start_time);
    Utils::WriteParamUnsigned(entry_root, "DURATION", duration);
    Utils::WriteParamUnsigned(entry_root, "U_04", unk_04, true);
    Utils::WriteParamUnsigned(entry_root, "FLAGS", flags, true);

    Utils::WriteComment(entry_root, "0xfffe -> skill ean; 0x5 -> char ean");
    Utils::WriteParamUnsigned(entry_root, "EAN_TO_USE", ean_to_use, true);

    Utils::WriteParamUnsigned(entry_root, "EAN_INDEX", ean_index, true);
    Utils::WriteParamUnsigned(entry_root, "U_0C", unk_0C, true);
    Utils::WriteParamUnsigned(entry_root, "U_0E", unk_0E, true);
    Utils::WriteParamUnsigned(entry_root, "U_10", unk_10, true);
    Utils::WriteParamUnsigned(entry_root, "U_12", unk_12, true);
    Utils::WriteParamUnsigned(entry_root, "U_14", unk_14, true);
    Utils::WriteParamUnsigned(entry_root, "U_16", unk_16, true);
    Utils::WriteParamFloat(entry_root, "ANIMATION_SPEED", animation_speed);
    Utils::WriteParamFloat(entry_root, "F_1C", unk_1C);
    Utils::WriteParamFloat(entry_root, "F_20", unk_20);

    root->LinkEndChild(entry_root);
    return entry_root;
}

bool BACAnimation::Compile(const TiXmlElement *root)
{
    if (!Utils::GetParamUnsigned(root, "START_TIME", &start_time))
        return false;

    if (!Utils::GetParamUnsigned(root, "DURATION", &duration))
        return false;

    if (!Utils::GetParamUnsigned(root, "U_04", &unk_04))
        return false;

    if (!Utils::GetParamUnsigned(root, "FLAGS", &flags))
        return false;

    if (!Utils::GetParamUnsigned(root, "EAN_TO_USE", &ean_to_use))
        return false;

    if (!Utils::GetParamUnsigned(root, "EAN_INDEX", &ean_index))
        return false;

    if (!Utils::GetParamUnsigned(root, "U_0C", &unk_0C))
        return false;

    if (!Utils::GetParamUnsigned(root, "U_0E", &unk_0E))
        return false;

    if (!Utils::GetParamUnsigned(root, "U_10", &unk_10))
        return false;

    if (!Utils::GetParamUnsigned(root, "U_12", &unk_12))
        return false;

    if (!Utils::GetParamUnsigned(root, "U_14", &unk_14))
        return false;

    if (!Utils::GetParamUnsigned(root, "U_16", &unk_16))
        return false;

    if (!Utils::GetParamFloat(root, "ANIMATION_SPEED", &animation_speed))
        return false;

    if (!Utils::GetParamFloat(root, "F_1C", &unk_1C))
        return false;

    if (!Utils::GetParamFloat(root, "F_20", &unk_20))
        return false;

    return true;
}

TiXmlElement *BACHitbox::Decompile(TiXmlNode *root) const
{
    TiXmlElement *entry_root = new TiXmlElement("Hitbox");

    Utils::WriteParamUnsigned(entry_root, "START_TIME", start_time);
    Utils::WriteParamUnsigned(entry_root, "DURATION", duration);
    Utils::WriteParamUnsigned(entry_root, "U_04", unk_04, true);
    Utils::WriteParamUnsigned(entry_root, "FLAGS", flags, true);
    Utils::WriteParamUnsigned(entry_root, "U_08", unk_08, true);
    Utils::WriteParamUnsigned(entry_root, "U_0A", unk_0A, true);
    Utils::WriteParamUnsigned(entry_root, "U_0C", unk_0C, true);
    Utils::WriteParamUnsigned(entry_root, "U_10", unk_10, true);
    Utils::WriteParamUnsigned(entry_root, "U_12", unk_12, true);
    Utils::WriteParamMultipleUnsigned(entry_root, "U_14", std::vector<uint32_t>(unk_14, unk_14+2), true);
    Utils::WriteParamMultipleFloats(entry_root, "F_1C", std::vector<float>(unk_1C, unk_1C+9));

    root->LinkEndChild(entry_root);
    return entry_root;
}

bool BACHitbox::Compile(const TiXmlElement *root)
{
    if (!Utils::GetParamUnsigned(root, "START_TIME", &start_time))
        return false;

    if (!Utils::GetParamUnsigned(root, "DURATION", &duration))
        return false;

    if (!Utils::GetParamUnsigned(root, "U_04", &unk_04))
        return false;

    if (!Utils::GetParamUnsigned(root, "FLAGS", &flags))
        return false;

    if (!Utils::GetParamUnsigned(root, "U_08", &unk_08))
        return false;

    if (!Utils::GetParamUnsigned(root, "U_0A", &unk_0A))
        return false;

    if (!Utils::GetParamUnsigned(root, "U_0C", &unk_0C))
        return false;

    if (!Utils::GetParamUnsigned(root, "U_10", &unk_10))
        return false;

    if (!Utils::GetParamUnsigned(root, "U_12", &unk_12))
        return false;

    if (!Utils::GetParamMultipleUnsigned(root, "U_14", unk_14, 2))
        return false;

    if (!Utils::GetParamMultipleFloats(root, "F_1C", unk_1C, 9))
        return false;

    return true;
}

TiXmlElement *BACAccelerationMovement::Decompile(TiXmlNode *root) const
{
    TiXmlElement *entry_root = new TiXmlElement("AccelerationMovement");

    Utils::WriteParamUnsigned(entry_root, "START_TIME", start_time);
    Utils::WriteParamUnsigned(entry_root, "DURATION", duration);
    Utils::WriteParamUnsigned(entry_root, "U_04", unk_04, true);
    Utils::WriteParamUnsigned(entry_root, "FLAGS", flags, true);
    Utils::WriteParamUnsigned(entry_root, "U_08", unk_08, true);
    Utils::WriteParamUnsigned(entry_root, "U_0A", unk_0A, true);
    Utils::WriteParamUnsigned(entry_root, "U_0C", unk_0C, true);
    Utils::WriteParamFloat(entry_root, "F_10", unk_10);
    Utils::WriteParamFloat(entry_root, "F_14", unk_14);
    Utils::WriteParamUnsigned(entry_root, "U_18", unk_18, true);
    Utils::WriteParamFloat(entry_root, "F_1C", unk_1C);
    Utils::WriteParamUnsigned(entry_root, "U_20", unk_20, true);

    root->LinkEndChild(entry_root);
    return entry_root;
}

bool BACAccelerationMovement::Compile(const TiXmlElement *root)
{
    if (!Utils::GetParamUnsigned(root, "START_TIME", &start_time))
        return false;

    if (!Utils::GetParamUnsigned(root, "DURATION", &duration))
        return false;

    if (!Utils::GetParamUnsigned(root, "U_04", &unk_04))
        return false;

    if (!Utils::GetParamUnsigned(root, "FLAGS", &flags))
        return false;

    if (!Utils::GetParamUnsigned(root, "U_08", &unk_08))
        return false;

    if (!Utils::GetParamUnsigned(root, "U_0A", &unk_0A))
        return false;

    if (!Utils::GetParamUnsigned(root, "U_0C", &unk_0C))
        return false;

    if (!Utils::GetParamFloat(root, "F_10", &unk_10))
        return false;

    if (!Utils::GetParamFloat(root, "F_14", &unk_14))
        return false;

    if (!Utils::GetParamUnsigned(root, "U_18", &unk_18))
        return false;

    if (!Utils::GetParamFloat(root, "F_1C", &unk_1C))
        return false;

    if (!Utils::GetParamUnsigned(root, "U_20", &unk_20))
        return false;

    return true;
}

TiXmlElement *BACType3::Decompile(TiXmlNode *root) const
{
    TiXmlElement *entry_root = new TiXmlElement("BACType3");

    Utils::WriteParamUnsigned(entry_root, "START_TIME", start_time);
    Utils::WriteParamUnsigned(entry_root, "DURATION", duration);
    Utils::WriteParamUnsigned(entry_root, "U_04", unk_04, true);
    Utils::WriteParamUnsigned(entry_root, "FLAGS", flags, true);
    Utils::WriteParamUnsigned(entry_root, "U_08", unk_08, true);

    root->LinkEndChild(entry_root);
    return entry_root;
}

bool BACType3::Compile(const TiXmlElement *root)
{
    if (!Utils::GetParamUnsigned(root, "START_TIME", &start_time))
        return false;

    if (!Utils::GetParamUnsigned(root, "DURATION", &duration))
        return false;

    if (!Utils::GetParamUnsigned(root, "U_04", &unk_04))
        return false;

    if (!Utils::GetParamUnsigned(root, "FLAGS", &flags))
        return false;

    if (!Utils::GetParamUnsigned(root, "U_08", &unk_08))
        return false;

    return true;
}

TiXmlElement *BACMotionAdjust::Decompile(TiXmlNode *root) const
{
    TiXmlElement *entry_root = new TiXmlElement("MotionAdjust");

    Utils::WriteParamUnsigned(entry_root, "START_TIME", start_time);
    Utils::WriteParamUnsigned(entry_root, "DURATION", duration);
    Utils::WriteParamUnsigned(entry_root, "U_04", unk_04, true);
    Utils::WriteParamUnsigned(entry_root, "FLAGS", flags, true);
    Utils::WriteParamFloat(entry_root, "F_08", unk_08);

    root->LinkEndChild(entry_root);
    return entry_root;
}

bool BACMotionAdjust::Compile(const TiXmlElement *root)
{
    if (!Utils::GetParamUnsigned(root, "START_TIME", &start_time))
        return false;

    if (!Utils::GetParamUnsigned(root, "DURATION", &duration))
        return false;

    if (!Utils::GetParamUnsigned(root, "U_04", &unk_04))
        return false;

    if (!Utils::GetParamUnsigned(root, "FLAGS", &flags))
        return false;

    if (!Utils::GetParamFloat(root, "F_08", &unk_08))
        return false;

    return true;
}

TiXmlElement *BACOpponentKnockback::Decompile(TiXmlNode *root) const
{
    TiXmlElement *entry_root = new TiXmlElement("OpponentKnockback");

    Utils::WriteParamUnsigned(entry_root, "START_TIME", start_time);
    Utils::WriteParamUnsigned(entry_root, "DURATION", duration);
    Utils::WriteParamUnsigned(entry_root, "U_04", unk_04, true);
    Utils::WriteParamUnsigned(entry_root, "FLAGS", flags, true);
    Utils::WriteParamFloat(entry_root, "F_08", unk_08);
    Utils::WriteParamUnsigned(entry_root, "U_0C", unk_0C, true);
    Utils::WriteParamUnsigned(entry_root, "U_0E", unk_0E, true);

    root->LinkEndChild(entry_root);
    return entry_root;
}

bool BACOpponentKnockback::Compile(const TiXmlElement *root)
{
    if (!Utils::GetParamUnsigned(root, "START_TIME", &start_time))
        return false;

    if (!Utils::GetParamUnsigned(root, "DURATION", &duration))
        return false;

    if (!Utils::GetParamUnsigned(root, "U_04", &unk_04))
        return false;

    if (!Utils::GetParamUnsigned(root, "FLAGS", &flags))
        return false;

    if (!Utils::GetParamFloat(root, "F_08", &unk_08))
        return false;

    if (!Utils::GetParamUnsigned(root, "U_0C", &unk_0C))
        return false;

    if (!Utils::GetParamUnsigned(root, "U_0E", &unk_0E))
        return false;

    return true;
}

TiXmlElement *BACType6::Decompile(TiXmlNode *root) const
{
    TiXmlElement *entry_root = new TiXmlElement("BACType6");

    Utils::WriteParamUnsigned(entry_root, "START_TIME", start_time);
    Utils::WriteParamUnsigned(entry_root, "DURATION", duration);
    Utils::WriteParamUnsigned(entry_root, "U_04", unk_04, true);
    Utils::WriteParamUnsigned(entry_root, "FLAGS", flags, true);
    Utils::WriteParamUnsigned(entry_root, "U_08", unk_08, true);
    Utils::WriteParamUnsigned(entry_root, "U_0C", unk_0C, true);
    Utils::WriteParamUnsigned(entry_root, "U_0E", unk_0E, true);

    root->LinkEndChild(entry_root);
    return entry_root;
}

bool BACType6::Compile(const TiXmlElement *root)
{
    if (!Utils::GetParamUnsigned(root, "START_TIME", &start_time))
        return false;

    if (!Utils::GetParamUnsigned(root, "DURATION", &duration))
        return false;

    if (!Utils::GetParamUnsigned(root, "U_04", &unk_04))
        return false;

    if (!Utils::GetParamUnsigned(root, "FLAGS", &flags))
        return false;

    if (!Utils::GetParamUnsigned(root, "U_08", &unk_08))
        return false;

    if (!Utils::GetParamUnsigned(root, "U_0C", &unk_0C))
        return false;

    if (!Utils::GetParamUnsigned(root, "U_0E", &unk_0E))
        return false;

    return true;
}

TiXmlElement *BACType7::Decompile(TiXmlNode *root) const
{
    TiXmlElement *entry_root = new TiXmlElement("BACType7");

    Utils::WriteParamUnsigned(entry_root, "START_TIME", start_time);
    Utils::WriteParamUnsigned(entry_root, "DURATION", duration);
    Utils::WriteParamUnsigned(entry_root, "U_04", unk_04, true);
    Utils::WriteParamUnsigned(entry_root, "FLAGS", flags, true);
    Utils::WriteParamUnsigned(entry_root, "U_08", unk_08, true);
    Utils::WriteParamUnsigned(entry_root, "U_0A", unk_0A, true);

    root->LinkEndChild(entry_root);
    return entry_root;
}

bool BACType7::Compile(const TiXmlElement *root)
{
    if (!Utils::GetParamUnsigned(root, "START_TIME", &start_time))
        return false;

    if (!Utils::GetParamUnsigned(root, "DURATION", &duration))
        return false;

    if (!Utils::GetParamUnsigned(root, "U_04", &unk_04))
        return false;

    if (!Utils::GetParamUnsigned(root, "FLAGS", &flags))
        return false;

    if (!Utils::GetParamUnsigned(root, "U_08", &unk_08))
        return false;

    if (!Utils::GetParamUnsigned(root, "U_0A", &unk_0A))
        return false;

    return true;
}

TiXmlElement *BACEffect::Decompile(TiXmlNode *root) const
{
    TiXmlElement *entry_root = new TiXmlElement("Effect");

    Utils::WriteParamUnsigned(entry_root, "START_TIME", start_time);
    Utils::WriteParamUnsigned(entry_root, "DURATION", duration);
    Utils::WriteParamUnsigned(entry_root, "U_04", unk_04, true);
    Utils::WriteParamUnsigned(entry_root, "FLAGS", flags, true);
    Utils::WriteParamUnsigned(entry_root, "U_0A", unk_0A, true);

    bool hex = (id == 0xFFFF || id == 0xBACA || id == 0xCACA);
    Utils::WriteParamUnsigned(entry_root, "SKILL_ID", id, hex);

    Utils::WriteParamUnsigned(entry_root, "U_0E", unk_0E, true);
    Utils::WriteParamMultipleUnsigned(entry_root, "U_10", std::vector<uint32_t>(unk_10, unk_10+8), true);

    root->LinkEndChild(entry_root);
    return entry_root;
}

bool BACEffect::Compile(const TiXmlElement *root)
{
    if (!Utils::GetParamUnsigned(root, "START_TIME", &start_time))
        return false;

    if (!Utils::GetParamUnsigned(root, "DURATION", &duration))
        return false;

    if (!Utils::GetParamUnsigned(root, "U_04", &unk_04))
        return false;

    if (!Utils::GetParamUnsigned(root, "FLAGS", &flags))
        return false;

    if (!Utils::GetParamUnsigned(root, "U_0A", &unk_0A))
        return false;

    if (!Utils::GetParamUnsigned(root, "SKILL_ID", &id))
        return false;

    if (!Utils::GetParamUnsigned(root, "U_0E", &unk_0E))
        return false;

    if (!Utils::GetParamMultipleUnsigned(root, "U_10", unk_10, 8))
        return false;

    return true;
}

TiXmlElement *BACProjectile::Decompile(TiXmlNode *root) const
{
    TiXmlElement *entry_root = new TiXmlElement("Projectile");

    Utils::WriteParamUnsigned(entry_root, "START_TIME", start_time);
    Utils::WriteParamUnsigned(entry_root, "DURATION", duration);
    Utils::WriteParamUnsigned(entry_root, "U_04", unk_04, true);
    Utils::WriteParamUnsigned(entry_root, "FLAGS", flags, true);

    bool hex = (id == 0xFFFF || id == 0xBACA || id == 0xCACA);
    Utils::WriteParamUnsigned(entry_root, "SKILL_ID", id, hex);

    Utils::WriteParamUnsigned(entry_root, "U_0A", unk_0A, true);
    Utils::WriteParamUnsigned(entry_root, "U_0C", unk_0C, true);
    Utils::WriteParamUnsigned(entry_root, "U_10", unk_10, true);
    Utils::WriteParamUnsigned(entry_root, "U_12", unk_12, true);
    Utils::WriteParamMultipleFloats(entry_root, "F_14", std::vector<float>(unk_14, unk_14+3));
    Utils::WriteParamUnsigned(entry_root, "U_20", unk_20, true);
    Utils::WriteParamUnsigned(entry_root, "U_24", unk_24, true);
    Utils::WriteParamFloat(entry_root, "F_28", unk_28);
    Utils::WriteParamUnsigned(entry_root, "U_2C", unk_2C, true);
    Utils::WriteParamUnsigned(entry_root, "U_2E", unk_2E, true);
    Utils::WriteParamMultipleUnsigned(entry_root, "U_30", std::vector<uint32_t>(unk_30, unk_30+4), true);

    root->LinkEndChild(entry_root);
    return entry_root;
}

bool BACProjectile::Compile(const TiXmlElement *root)
{
    if (!Utils::GetParamUnsigned(root, "START_TIME", &start_time))
        return false;

    if (!Utils::GetParamUnsigned(root, "DURATION", &duration))
        return false;

    if (!Utils::GetParamUnsigned(root, "U_04", &unk_04))
        return false;

    if (!Utils::GetParamUnsigned(root, "SKILL_ID", &id))
        return false;

    if (!Utils::GetParamUnsigned(root, "FLAGS", &flags))
        return false;

    if (!Utils::GetParamUnsigned(root, "U_0A", &unk_0A))
        return false;

    if (!Utils::GetParamUnsigned(root, "U_0C", &unk_0C))
        return false;

    if (!Utils::GetParamUnsigned(root, "U_10", &unk_10))
        return false;

    if (!Utils::GetParamUnsigned(root, "U_12", &unk_12))
        return false;

    if (!Utils::GetParamMultipleFloats(root, "F_14", unk_14, 3))
        return false;

    if (!Utils::GetParamUnsigned(root, "U_20", &unk_20))
        return false;

    if (!Utils::GetParamUnsigned(root, "U_24", &unk_24))
        return false;

    if (!Utils::GetParamFloat(root, "F_28", &unk_28))
        return false;

    if (!Utils::GetParamUnsigned(root, "U_2C", &unk_2C))
        return false;

    if (!Utils::GetParamUnsigned(root, "U_2E", &unk_2E))
        return false;

    if (!Utils::GetParamMultipleUnsigned(root, "U_30", unk_30, 4))
        return false;

    return true;
}

TiXmlElement *BACCamera::Decompile(TiXmlNode *root) const
{
    TiXmlElement *entry_root = new TiXmlElement("Camera");

    Utils::WriteParamUnsigned(entry_root, "START_TIME", start_time);
    Utils::WriteParamUnsigned(entry_root, "DURATION", duration);
    Utils::WriteParamUnsigned(entry_root, "U_04", unk_04, true);
    Utils::WriteParamUnsigned(entry_root, "FLAGS", flags, true);

    Utils::WriteComment(entry_root, "0x5 -> skill ean");
    Utils::WriteParamUnsigned(entry_root, "EAN_TO_USE", ean_to_use, true);

    Utils::WriteParamUnsigned(entry_root, "U_0A", unk_0A, true);
    Utils::WriteParamUnsigned(entry_root, "EAN_INDEX", ean_index, true);
    Utils::WriteParamUnsigned(entry_root, "U_0E", unk_0E, true);
    Utils::WriteParamUnsigned(entry_root, "U_10", unk_10, true);
    Utils::WriteParamUnsigned(entry_root, "U_12", unk_12, true);
    Utils::WriteParamMultipleUnsigned(entry_root, "U_14", std::vector<uint32_t>(unk_14, unk_14+13), true);
    Utils::WriteParamUnsigned(entry_root, "U_48", unk_48, true);
    Utils::WriteParamUnsigned(entry_root, "U_4A", unk_4A, true);

    root->LinkEndChild(entry_root);
    return entry_root;
}

bool BACCamera::Compile(const TiXmlElement *root)
{
    if (!Utils::GetParamUnsigned(root, "START_TIME", &start_time))
        return false;

    if (!Utils::GetParamUnsigned(root, "DURATION", &duration))
        return false;

    if (!Utils::GetParamUnsigned(root, "U_04", &unk_04))
        return false;

    if (!Utils::GetParamUnsigned(root, "FLAGS", &flags))
        return false;

    if (!Utils::GetParamUnsigned(root, "EAN_TO_USE", &ean_to_use))
        return false;

    if (!Utils::GetParamUnsigned(root, "U_0A", &unk_0A))
        return false;

    if (!Utils::GetParamUnsigned(root, "EAN_INDEX", &ean_index))
        return false;

    if (!Utils::GetParamUnsigned(root, "U_0E", &unk_0E))
        return false;

    if (!Utils::GetParamUnsigned(root, "U_10", &unk_10))
        return false;

    if (!Utils::GetParamUnsigned(root, "U_12", &unk_12))
        return false;

    if (!Utils::GetParamMultipleUnsigned(root, "U_14", unk_14, 13))
        return false;

    if (!Utils::GetParamUnsigned(root, "U_48", &unk_48))
        return false;

    if (!Utils::GetParamUnsigned(root, "U_4A", &unk_4A))
        return false;

    return true;
}

TiXmlElement *BACSound::Decompile(TiXmlNode *root) const
{
    TiXmlElement *entry_root = new TiXmlElement("Sound");

    Utils::WriteParamUnsigned(entry_root, "START_TIME", start_time);
    Utils::WriteParamUnsigned(entry_root, "DURATION", duration);
    Utils::WriteParamUnsigned(entry_root, "U_04", unk_04, true);
    Utils::WriteParamUnsigned(entry_root, "FLAGS", flags, true);

    Utils::WriteComment(entry_root, "0x3 -> char acb; 0xa -> SE; 0xb -> VOX; 0x0 -> SE/Battle/Common");
    Utils::WriteParamUnsigned(entry_root, "ACB_TO_USE", acb_to_use, true);

    Utils::WriteParamUnsigned(entry_root, "U_0A", unk_0A, true);
    Utils::WriteParamUnsigned(entry_root, "CUE_ID", cue_id, true);
    Utils::WriteParamUnsigned(entry_root, "U_0E", unk_0E, true);

    root->LinkEndChild(entry_root);
    return entry_root;
}

bool BACSound::Compile(const TiXmlElement *root)
{
    if (!Utils::GetParamUnsigned(root, "START_TIME", &start_time))
        return false;

    if (!Utils::GetParamUnsigned(root, "DURATION", &duration))
        return false;

    if (!Utils::GetParamUnsigned(root, "U_04", &unk_04))
        return false;

    if (!Utils::GetParamUnsigned(root, "FLAGS", &flags))
        return false;

    if (!Utils::GetParamUnsigned(root, "ACB_TO_USE", &acb_to_use))
        return false;

    if (!Utils::GetParamUnsigned(root, "U_0A", &unk_0A))
        return false;

    if (!Utils::GetParamUnsigned(root, "CUE_ID", &cue_id))
        return false;

    if (!Utils::GetParamUnsigned(root, "U_0E", &unk_0E))
        return false;

    return true;
}

TiXmlElement *BACType12::Decompile(TiXmlNode *root) const
{
    TiXmlElement *entry_root = new TiXmlElement("BACType12");

    Utils::WriteParamUnsigned(entry_root, "START_TIME", start_time);
    Utils::WriteParamUnsigned(entry_root, "DURATION", duration);
    Utils::WriteParamUnsigned(entry_root, "U_04", unk_04, true);
    Utils::WriteParamUnsigned(entry_root, "FLAGS", flags, true);
    Utils::WriteParamUnsigned(entry_root, "U_08", unk_08, true);
    Utils::WriteParamUnsigned(entry_root, "U_0A", unk_0A, true);

    root->LinkEndChild(entry_root);
    return entry_root;
}

bool BACType12::Compile(const TiXmlElement *root)
{
    if (!Utils::GetParamUnsigned(root, "START_TIME", &start_time))
        return false;

    if (!Utils::GetParamUnsigned(root, "DURATION", &duration))
        return false;

    if (!Utils::GetParamUnsigned(root, "U_04", &unk_04))
        return false;

    if (!Utils::GetParamUnsigned(root, "FLAGS", &flags))
        return false;

    if (!Utils::GetParamUnsigned(root, "U_08", &unk_08))
        return false;

    if (!Utils::GetParamUnsigned(root, "U_0A", &unk_0A))
        return false;

    return true;
}

TiXmlElement *BACType13::Decompile(TiXmlNode *root) const
{
    TiXmlElement *entry_root = new TiXmlElement("BACType13");

    Utils::WriteParamUnsigned(entry_root, "START_TIME", start_time);
    Utils::WriteParamUnsigned(entry_root, "DURATION", duration);
    Utils::WriteParamUnsigned(entry_root, "U_04", unk_04, true);
    Utils::WriteParamUnsigned(entry_root, "FLAGS", flags, true);
    Utils::WriteParamUnsigned(entry_root, "U_08", unk_08, true);
    Utils::WriteParamUnsigned(entry_root, "U_0A", unk_0A, true);

    root->LinkEndChild(entry_root);
    return entry_root;
}

bool BACType13::Compile(const TiXmlElement *root)
{
    if (!Utils::GetParamUnsigned(root, "START_TIME", &start_time))
        return false;

    if (!Utils::GetParamUnsigned(root, "DURATION", &duration))
        return false;

    if (!Utils::GetParamUnsigned(root, "U_04", &unk_04))
        return false;

    if (!Utils::GetParamUnsigned(root, "FLAGS", &flags))
        return false;

    if (!Utils::GetParamUnsigned(root, "U_08", &unk_08))
        return false;

    if (!Utils::GetParamUnsigned(root, "U_0A", &unk_0A))
        return false;

    return true;
}

TiXmlElement *BACType14::Decompile(TiXmlNode *root) const
{
    TiXmlElement *entry_root = new TiXmlElement("BACType14");

    Utils::WriteParamUnsigned(entry_root, "START_TIME", start_time);
    Utils::WriteParamUnsigned(entry_root, "DURATION", duration);
    Utils::WriteParamUnsigned(entry_root, "U_04", unk_04, true);
    Utils::WriteParamUnsigned(entry_root, "FLAGS", flags, true);
    Utils::WriteParamUnsigned(entry_root, "U_08", unk_08, true);
    Utils::WriteParamUnsigned(entry_root, "U_0A", unk_0A, true);

    root->LinkEndChild(entry_root);
    return entry_root;
}

bool BACType14::Compile(const TiXmlElement *root)
{
    if (!Utils::GetParamUnsigned(root, "START_TIME", &start_time))
        return false;

    if (!Utils::GetParamUnsigned(root, "DURATION", &duration))
        return false;

    if (!Utils::GetParamUnsigned(root, "U_04", &unk_04))
        return false;

    if (!Utils::GetParamUnsigned(root, "FLAGS", &flags))
        return false;

    if (!Utils::GetParamUnsigned(root, "U_08", &unk_08))
        return false;

    if (!Utils::GetParamUnsigned(root, "U_0A", &unk_0A))
        return false;

    return true;
}

TiXmlElement *BACTransformControl::Decompile(TiXmlNode *root) const
{
    TiXmlElement *entry_root = new TiXmlElement("TransformControl");

    Utils::WriteParamUnsigned(entry_root, "START_TIME", start_time);
    Utils::WriteParamUnsigned(entry_root, "DURATION", duration);
    Utils::WriteParamUnsigned(entry_root, "U_04", unk_04, true);
    Utils::WriteParamUnsigned(entry_root, "FLAGS", flags, true);

    Utils::WriteComment(entry_root, "0xd -> skill activation? ; 0x2a -> use bcs body");
    Utils::WriteParamUnsigned(entry_root, "TYPE", type, true);

    Utils::WriteParamUnsigned(entry_root, "U_0A", unk_0A, true);

    Utils::WriteComment(entry_root, "If type == 0x2a, this is bcs body");
    Utils::WriteParamFloat(entry_root, "PARAMETER", parameter);

    Utils::WriteParamMultipleUnsigned(entry_root, "U_10", std::vector<uint32_t>(unk_10, unk_10+4), true);

    root->LinkEndChild(entry_root);
    return entry_root;
}

bool BACTransformControl::Compile(const TiXmlElement *root)
{
    if (!Utils::GetParamUnsigned(root, "START_TIME", &start_time))
        return false;

    if (!Utils::GetParamUnsigned(root, "DURATION", &duration))
        return false;

    if (!Utils::GetParamUnsigned(root, "U_04", &unk_04))
        return false;

    if (!Utils::GetParamUnsigned(root, "FLAGS", &flags))
        return false;

    if (!Utils::GetParamUnsigned(root, "TYPE", &type))
        return false;

    if (!Utils::GetParamUnsigned(root, "U_0A", &unk_0A))
        return false;

    if (!Utils::GetParamFloat(root, "PARAMETER", &parameter))
        return false;

    if (!Utils::GetParamMultipleUnsigned(root, "U_10", unk_10, 4))
        return false;

    return true;
}

TiXmlElement *BACType16::Decompile(TiXmlNode *root) const
{
    TiXmlElement *entry_root = new TiXmlElement("BACType16");

    Utils::WriteParamUnsigned(entry_root, "START_TIME", start_time);
    Utils::WriteParamUnsigned(entry_root, "DURATION", duration);
    Utils::WriteParamUnsigned(entry_root, "U_04", unk_04, true);
    Utils::WriteParamUnsigned(entry_root, "FLAGS", flags, true);
    Utils::WriteParamUnsigned(entry_root, "U_08", unk_08, true);
    Utils::WriteParamUnsigned(entry_root, "U_0A", unk_0A, true);
    Utils::WriteParamUnsigned(entry_root, "U_0C", unk_0C, true);
    Utils::WriteParamUnsigned(entry_root, "U_0E", unk_0E, true);
    Utils::WriteParamMultipleUnsigned(entry_root, "U_10", std::vector<uint32_t>(unk_10, unk_10+4), true);

    root->LinkEndChild(entry_root);
    return entry_root;
}

bool BACType16::Compile(const TiXmlElement *root)
{
    if (!Utils::GetParamUnsigned(root, "START_TIME", &start_time))
        return false;

    if (!Utils::GetParamUnsigned(root, "DURATION", &duration))
        return false;

    if (!Utils::GetParamUnsigned(root, "U_04", &unk_04))
        return false;

    if (!Utils::GetParamUnsigned(root, "FLAGS", &flags))
        return false;

    if (!Utils::GetParamUnsigned(root, "U_08", &unk_08))
        return false;

    if (!Utils::GetParamUnsigned(root, "U_0A", &unk_0A))
        return false;

    if (!Utils::GetParamUnsigned(root, "U_0C", &unk_0C))
        return false;

    if (!Utils::GetParamUnsigned(root, "U_0E", &unk_0E))
        return false;

    if (!Utils::GetParamMultipleUnsigned(root, "U_10", unk_10, 4))
        return false;

    return true;
}

TiXmlElement *BACType17::Decompile(TiXmlNode *root, bool _small) const
{
    TiXmlElement *entry_root = new TiXmlElement("BACType17");

    Utils::WriteParamUnsigned(entry_root, "START_TIME", start_time);
    Utils::WriteParamUnsigned(entry_root, "DURATION", duration);
    Utils::WriteParamUnsigned(entry_root, "U_04", unk_04, true);
    Utils::WriteParamUnsigned(entry_root, "FLAGS", flags, true);
    Utils::WriteParamUnsigned(entry_root, "U_08", unk_08, true);
    Utils::WriteParamUnsigned(entry_root, "U_0A", unk_0A, true);
    Utils::WriteParamUnsigned(entry_root, "U_0C", unk_0C, true);
    Utils::WriteParamUnsigned(entry_root, "U_0E", unk_0E, true);
    Utils::WriteParamUnsigned(entry_root, "U_10", unk_10, true);
    Utils::WriteParamUnsigned(entry_root, "U_12", unk_12, true);

    if (!_small)
        Utils::WriteParamMultipleUnsigned(entry_root, "U_14", std::vector<uint32_t>(unk_14, unk_14+3), true);

    root->LinkEndChild(entry_root);
    return entry_root;
}

bool BACType17::Compile(const TiXmlElement *root, bool _small)
{
    if (!Utils::GetParamUnsigned(root, "START_TIME", &start_time))
        return false;

    if (!Utils::GetParamUnsigned(root, "DURATION", &duration))
        return false;

    if (!Utils::GetParamUnsigned(root, "U_04", &unk_04))
        return false;

    if (!Utils::GetParamUnsigned(root, "FLAGS", &flags))
        return false;

    if (!Utils::GetParamUnsigned(root, "U_08", &unk_08))
        return false;

    if (!Utils::GetParamUnsigned(root, "U_0A", &unk_0A))
        return false;

    if (!Utils::GetParamUnsigned(root, "U_0C", &unk_0C))
        return false;

    if (!Utils::GetParamUnsigned(root, "U_0E", &unk_0E))
        return false;

    if (!Utils::GetParamUnsigned(root, "U_10", &unk_10))
        return false;

    if (!Utils::GetParamUnsigned(root, "U_12", &unk_12))
        return false;

    if (_small)
        return true;

    if (!Utils::GetParamMultipleUnsigned(root, "U_14", unk_14, 3))
        return false;

    return true;
}

TiXmlElement *BACType18::Decompile(TiXmlNode *root) const
{
    TiXmlElement *entry_root = new TiXmlElement("BACType18");

    Utils::WriteParamUnsigned(entry_root, "START_TIME", start_time);
    Utils::WriteParamUnsigned(entry_root, "DURATION", duration);
    Utils::WriteParamUnsigned(entry_root, "U_04", unk_04, true);
    Utils::WriteParamUnsigned(entry_root, "FLAGS", flags, true);
    Utils::WriteParamMultipleUnsigned(entry_root, "U_08", std::vector<uint32_t>(unk_08, unk_08+6), true);

    root->LinkEndChild(entry_root);
    return entry_root;
}

bool BACType18::Compile(const TiXmlElement *root)
{
    if (!Utils::GetParamUnsigned(root, "START_TIME", &start_time))
        return false;

    if (!Utils::GetParamUnsigned(root, "DURATION", &duration))
        return false;

    if (!Utils::GetParamUnsigned(root, "U_04", &unk_04))
        return false;

    if (!Utils::GetParamUnsigned(root, "FLAGS", &flags))
        return false;

    if (!Utils::GetParamMultipleUnsigned(root, "U_08", unk_08, 6))
        return false;

    return true;
}

TiXmlElement *BACType19::Decompile(TiXmlNode *root) const
{
    TiXmlElement *entry_root = new TiXmlElement("BACType19");

    Utils::WriteParamUnsigned(entry_root, "START_TIME", start_time);
    Utils::WriteParamUnsigned(entry_root, "DURATION", duration);
    Utils::WriteParamUnsigned(entry_root, "U_04", unk_04, true);
    Utils::WriteParamUnsigned(entry_root, "FLAGS", flags, true);
    Utils::WriteParamUnsigned(entry_root, "U_08", unk_08, true);
    Utils::WriteParamUnsigned(entry_root, "U_0A", unk_0A, true);
    Utils::WriteParamUnsigned(entry_root, "U_0C", unk_0C, true);

    root->LinkEndChild(entry_root);
    return entry_root;
}

bool BACType19::Compile(const TiXmlElement *root)
{
    if (!Utils::GetParamUnsigned(root, "START_TIME", &start_time))
        return false;

    if (!Utils::GetParamUnsigned(root, "DURATION", &duration))
        return false;

    if (!Utils::GetParamUnsigned(root, "U_04", &unk_04))
        return false;

    if (!Utils::GetParamUnsigned(root, "FLAGS", &flags))
        return false;

    if (!Utils::GetParamUnsigned(root, "U_08", &unk_08))
        return false;

    if (!Utils::GetParamUnsigned(root, "U_0A", &unk_0A))
        return false;

    if (!Utils::GetParamUnsigned(root, "U_0C", &unk_0C))
        return false;

    return true;
}

TiXmlElement *BACType20::Decompile(TiXmlNode *root) const
{
    TiXmlElement *entry_root = new TiXmlElement("BACType20");

    Utils::WriteParamUnsigned(entry_root, "START_TIME", start_time);
    Utils::WriteParamUnsigned(entry_root, "DURATION", duration);
    Utils::WriteParamUnsigned(entry_root, "U_04", unk_04, true);
    Utils::WriteParamUnsigned(entry_root, "FLAGS", flags, true);
    Utils::WriteParamUnsigned(entry_root, "U_08", unk_08, true);
    Utils::WriteParamUnsigned(entry_root, "U_0A", unk_0A, true);
    Utils::WriteParamUnsigned(entry_root, "U_0C", unk_0C, true);
    Utils::WriteParamUnsigned(entry_root, "U_0E", unk_0E, true);
    Utils::WriteParamUnsigned(entry_root, "U_10", unk_10, true);
    Utils::WriteParamUnsigned(entry_root, "U_12", unk_12, true);
    Utils::WriteParamUnsigned(entry_root, "U_14", unk_14, true);
    Utils::WriteParamFloat(entry_root, "F_18", unk_18);
    Utils::WriteParamMultipleUnsigned(entry_root, "U_1C",  std::vector<uint32_t>(unk_1C, unk_1C+5), true);

    root->LinkEndChild(entry_root);
    return entry_root;
}

bool BACType20::Compile(const TiXmlElement *root)
{
    if (!Utils::GetParamUnsigned(root, "START_TIME", &start_time))
        return false;

    if (!Utils::GetParamUnsigned(root, "DURATION", &duration))
        return false;

    if (!Utils::GetParamUnsigned(root, "U_04", &unk_04))
        return false;

    if (!Utils::GetParamUnsigned(root, "FLAGS", &flags))
        return false;

    if (!Utils::GetParamUnsigned(root, "U_08", &unk_08))
        return false;

    if (!Utils::GetParamUnsigned(root, "U_0A", &unk_0A))
        return false;

    if (!Utils::GetParamUnsigned(root, "U_0C", &unk_0C))
        return false;

    if (!Utils::GetParamUnsigned(root, "U_0E", &unk_0E))
        return false;

    if (!Utils::GetParamUnsigned(root, "U_10", &unk_10))
        return false;

    if (!Utils::GetParamUnsigned(root, "U_12", &unk_12))
        return false;

    if (!Utils::GetParamUnsigned(root, "U_14", &unk_14))
        return false;

    if (!Utils::GetParamFloat(root, "F_18", &unk_18))
        return false;

    if (!Utils::GetParamMultipleUnsigned(root, "U_1C", unk_1C, 5))
        return false;

    return true;
}

TiXmlElement *BACType21::Decompile(TiXmlNode *root) const
{
    TiXmlElement *entry_root = new TiXmlElement("BACType21");

    Utils::WriteParamUnsigned(entry_root, "START_TIME", start_time);
    Utils::WriteParamUnsigned(entry_root, "DURATION", duration);
    Utils::WriteParamUnsigned(entry_root, "U_04", unk_04, true);
    Utils::WriteParamUnsigned(entry_root, "FLAGS", flags, true);
    Utils::WriteParamUnsigned(entry_root, "U_08", unk_08, true);
    Utils::WriteParamUnsigned(entry_root, "U_0C", unk_0C, true);
    Utils::WriteParamUnsigned(entry_root, "U_0E", unk_0E, true);
    Utils::WriteParamUnsigned(entry_root, "U_10", unk_10, true);
    Utils::WriteParamUnsigned(entry_root, "U_12", unk_12, true);
    Utils::WriteParamUnsigned(entry_root, "U_14", unk_14, true);
    Utils::WriteParamUnsigned(entry_root, "U_16", unk_16, true);
    Utils::WriteParamFloat(entry_root, "F_18", unk_18);
    Utils::WriteParamFloat(entry_root, "F_1C", unk_1C);

    root->LinkEndChild(entry_root);
    return entry_root;
}

bool BACType21::Compile(const TiXmlElement *root)
{
    if (!Utils::GetParamUnsigned(root, "START_TIME", &start_time))
        return false;

    if (!Utils::GetParamUnsigned(root, "DURATION", &duration))
        return false;

    if (!Utils::GetParamUnsigned(root, "U_04", &unk_04))
        return false;

    if (!Utils::GetParamUnsigned(root, "FLAGS", &flags))
        return false;

    if (!Utils::GetParamUnsigned(root, "U_08", &unk_08))
        return false;

    if (!Utils::GetParamUnsigned(root, "U_0C", &unk_0C))
        return false;

    if (!Utils::GetParamUnsigned(root, "U_0E", &unk_0E))
        return false;

    if (!Utils::GetParamUnsigned(root, "U_10", &unk_10))
        return false;

    if (!Utils::GetParamUnsigned(root, "U_12", &unk_12))
        return false;

    if (!Utils::GetParamUnsigned(root, "U_14", &unk_14))
        return false;

    if (!Utils::GetParamUnsigned(root, "U_16", &unk_16))
        return false;

    if (!Utils::GetParamFloat(root, "F_18", &unk_18))
        return false;

    if (!Utils::GetParamFloat(root, "F_1C", &unk_1C))
        return false;

    return true;
}

TiXmlElement *BACType22::Decompile(TiXmlNode *root) const
{
    TiXmlElement *entry_root = new TiXmlElement("BACType22");

    Utils::WriteParamUnsigned(entry_root, "START_TIME", start_time);
    Utils::WriteParamUnsigned(entry_root, "DURATION", duration);
    Utils::WriteParamUnsigned(entry_root, "U_04", unk_04, true);
    Utils::WriteParamUnsigned(entry_root, "FLAGS", flags, true);
    Utils::WriteParamUnsigned(entry_root, "U_08", unk_08, true);
    Utils::WriteParamUnsigned(entry_root, "U_0A", unk_0A, true);
    Utils::WriteParamFloat(entry_root, "F_0C", unk_0C);
    Utils::WriteParamString(entry_root, "NAME", name);

    root->LinkEndChild(entry_root);
    return entry_root;
}

bool BACType22::Compile(const TiXmlElement *root)
{
    if (!Utils::GetParamUnsigned(root, "START_TIME", &start_time))
        return false;

    if (!Utils::GetParamUnsigned(root, "DURATION", &duration))
        return false;

    if (!Utils::GetParamUnsigned(root, "U_04", &unk_04))
        return false;

    if (!Utils::GetParamUnsigned(root, "FLAGS", &flags))
        return false;

    if (!Utils::GetParamUnsigned(root, "U_08", &unk_08))
        return false;

    if (!Utils::GetParamUnsigned(root, "U_0A", &unk_0A))
        return false;

    if (!Utils::GetParamFloat(root, "F_0C", &unk_0C))
        return false;

    std::string temp;

    if (!Utils::GetParamString(root, "NAME", temp))
        return false;

    if (temp.length() >= sizeof(name))
    {
        DPRINTF("%s: NAME too long. Must be 31 characters or less. (offending string=%s)\n", FUNCNAME, temp.c_str());
        return false;
    }

    memset(name, 0, sizeof(name));
    strcpy(name, temp.c_str());

    return true;
}

TiXmlElement *BACType23::Decompile(TiXmlNode *root) const
{
    TiXmlElement *entry_root = new TiXmlElement("BACType23");

    Utils::WriteParamUnsigned(entry_root, "START_TIME", start_time);
    Utils::WriteParamUnsigned(entry_root, "DURATION", duration);
    Utils::WriteParamUnsigned(entry_root, "U_04", unk_04, true);
    Utils::WriteParamUnsigned(entry_root, "FLAGS", flags, true);
    Utils::WriteParamUnsigned(entry_root, "U_08", unk_08, true);
    Utils::WriteParamUnsigned(entry_root, "U_0A", unk_0A, true);
    Utils::WriteParamUnsigned(entry_root, "U_0C", unk_0C, true);
    Utils::WriteParamUnsigned(entry_root, "U_0E", unk_0E, true);
    Utils::WriteParamUnsigned(entry_root, "U_10", unk_10, true);
    Utils::WriteParamMultipleFloats(entry_root, "F_14", std::vector<float>(unk_14, unk_14+11));

    root->LinkEndChild(entry_root);
    return entry_root;
}

bool BACType23::Compile(const TiXmlElement *root)
{
    if (!Utils::GetParamUnsigned(root, "START_TIME", &start_time))
        return false;

    if (!Utils::GetParamUnsigned(root, "DURATION", &duration))
        return false;

    if (!Utils::GetParamUnsigned(root, "U_04", &unk_04))
        return false;

    if (!Utils::GetParamUnsigned(root, "FLAGS", &flags))
        return false;

    if (!Utils::GetParamUnsigned(root, "U_08", &unk_08))
        return false;

    if (!Utils::GetParamUnsigned(root, "U_0A", &unk_0A))
        return false;

    if (!Utils::GetParamUnsigned(root, "U_0C", &unk_0C))
        return false;

    if (!Utils::GetParamUnsigned(root, "U_0E", &unk_0E))
        return false;

    if (!Utils::GetParamUnsigned(root, "U_10", &unk_10))
        return false;

    if (!Utils::GetParamMultipleFloats(root, "F_14", unk_14, 11))
        return false;

    return true;
}

TiXmlElement *BacEntry::Decompile(TiXmlNode *root, bool small_17) const
{
    TiXmlElement *entry_root = new TiXmlElement("BacEntry");

    entry_root->SetAttribute("flags", Utils::UnsignedToString(flags, true));
    if (valid)
    {
        if (has_type[0])
        {
            if (type0.size() != 0)
            {
                for (const BACAnimation &t0 : type0)
                    t0.Decompile(entry_root);
            }
            else
            {
                Utils::WriteParamUnsigned(entry_root, "HAS_DUMMY0", 1);
            }
        }

        if (has_type[1])
        {
            if (type1.size() != 0)
            {
                for (const BACHitbox &t1 : type1)
                    t1.Decompile(entry_root);
            }
            else
            {
                Utils::WriteParamUnsigned(entry_root, "HAS_DUMMY1", 1);
            }
        }

        if (has_type[2])
        {
            if (type2.size() != 0)
            {
                for (const BACAccelerationMovement &t2 : type2)
                    t2.Decompile(entry_root);
            }
            else
            {
                Utils::WriteParamUnsigned(entry_root, "HAS_DUMMY2", 1);
            }
        }

        if (has_type[3])
        {
            if (type3.size() != 0)
            {
                for (const BACType3 &t3 : type3)
                    t3.Decompile(entry_root);
            }
            else
            {
                Utils::WriteParamUnsigned(entry_root, "HAS_DUMMY3", 1);
            }
        }

        if (has_type[4])
        {
            if (type4.size() != 0)
            {
                for (const BACMotionAdjust &t4 : type4)
                    t4.Decompile(entry_root);
            }
            else
            {
                Utils::WriteParamUnsigned(entry_root, "HAS_DUMMY4", 1);
            }
        }

        if (has_type[5])
        {
            if (type5.size() != 0)
            {
                for (const BACOpponentKnockback &t5 : type5)
                    t5.Decompile(entry_root);
            }
            else
            {
                Utils::WriteParamUnsigned(entry_root, "HAS_DUMMY5", 1);
            }
        }

        if (has_type[6])
        {
            if (type6.size() != 0)
            {
                for (const BACType6 &t6 : type6)
                    t6.Decompile(entry_root);
            }
            else
            {
                Utils::WriteParamUnsigned(entry_root, "HAS_DUMMY6", 1);
            }
        }

        if (has_type[7])
        {
            if (type7.size() != 0)
            {
                for (const BACType7 &t7 : type7)
                    t7.Decompile(entry_root);
            }
            else
            {
                Utils::WriteParamUnsigned(entry_root, "HAS_DUMMY7", 1);
            }
        }

        if (has_type[8])
        {
            if (type8.size() != 0)
            {
                for (const BACEffect &t8 : type8)
                    t8.Decompile(entry_root);
            }
            else
            {
                Utils::WriteParamUnsigned(entry_root, "HAS_DUMMY8", 1);
            }
        }

        if (has_type[9])
        {
            if (type9.size() != 0)
            {
                for (const BACProjectile &t9 : type9)
                    t9.Decompile(entry_root);
            }
            else
            {
                Utils::WriteParamUnsigned(entry_root, "HAS_DUMMY9", 1);
            }
        }

        if (has_type[10])
        {
            if (type10.size() != 0)
            {
                for (const BACCamera &t10 : type10)
                    t10.Decompile(entry_root);
            }
            else
            {
                Utils::WriteParamUnsigned(entry_root, "HAS_DUMMY10", 1);
            }
        }

        if (has_type[11])
        {
            if (type11.size() != 0)
            {
                for (const BACSound &t11 : type11)
                    t11.Decompile(entry_root);
            }
            else
            {
                Utils::WriteParamUnsigned(entry_root, "HAS_DUMMY11", 1);
            }
        }

        if (has_type[12])
        {
            if (type12.size() != 0)
            {
                for (const BACType12 &t12 : type12)
                    t12.Decompile(entry_root);
            }
            else
            {
                Utils::WriteParamUnsigned(entry_root, "HAS_DUMMY12", 1);
            }
        }

        if (has_type[13])
        {
            if (type13.size() != 0)
            {
                for (const BACType13 &t13 : type13)
                    t13.Decompile(entry_root);
            }
            else
            {
                Utils::WriteParamUnsigned(entry_root, "HAS_DUMMY13", 1);
            }
        }

        if (has_type[14])
        {
            if (type14.size() != 0)
            {
                for (const BACType14 &t14 : type14)
                    t14.Decompile(entry_root);
            }
            else
            {
                Utils::WriteParamUnsigned(entry_root, "HAS_DUMMY14", 1);
            }
        }

        if (has_type[15])
        {
            if (type15.size() != 0)
            {
                for (const BACTransformControl &t15 : type15)
                    t15.Decompile(entry_root);
            }
            else
            {
                Utils::WriteParamUnsigned(entry_root, "HAS_DUMMY15", 1);
            }
        }

        if (has_type[16])
        {
            if (type16.size() != 0)
            {
                for (const BACType16 &t16 : type16)
                    t16.Decompile(entry_root);
            }
            else
            {
                Utils::WriteParamUnsigned(entry_root, "HAS_DUMMY16", 1);
            }
        }

        if (has_type[17])
        {
            if (type17.size() != 0)
            {
                for (const BACType17 &t17 : type17)
                    t17.Decompile(entry_root, small_17);
            }
            else
            {
                Utils::WriteParamUnsigned(entry_root, "HAS_DUMMY17", 1);
            }
        }

        if (has_type[18])
        {
            if (type18.size() != 0)
            {
                for (const BACType18 &t18 : type18)
                    t18.Decompile(entry_root);
            }
            else
            {
                Utils::WriteParamUnsigned(entry_root, "HAS_DUMMY18", 1);
            }
        }

        if (has_type[19])
        {
            if (type19.size() != 0)
            {
                for (const BACType19 &t19 : type19)
                    t19.Decompile(entry_root);
            }
            else
            {
                Utils::WriteParamUnsigned(entry_root, "HAS_DUMMY19", 1);
            }
        }

        if (has_type[20])
        {
            if (type20.size() != 0)
            {
                for (const BACType20 &t20 : type20)
                    t20.Decompile(entry_root);
            }
            else
            {
                Utils::WriteParamUnsigned(entry_root, "HAS_DUMMY20", 1);
            }
        }

        if (has_type[21])
        {
            if (type21.size() != 0)
            {
                for (const BACType21 &t21 : type21)
                    t21.Decompile(entry_root);
            }
            else
            {
                Utils::WriteParamUnsigned(entry_root, "HAS_DUMMY21", 1);
            }
        }

        if (has_type[22])
        {
            if (type22.size() != 0)
            {
                for (const BACType22 &t22 : type22)
                    t22.Decompile(entry_root);
            }
            else
            {
                Utils::WriteParamUnsigned(entry_root, "HAS_DUMMY22", 1);
            }
        }

        if (has_type[23])
        {
            if (type23.size() != 0)
            {
                for (const BACType23 &t23 : type23)
                    t23.Decompile(entry_root);
            }
            else
            {
                Utils::WriteParamUnsigned(entry_root, "HAS_DUMMY23", 1);
            }
        }
    }
    else
    {
        Utils::WriteComment(entry_root, "This entry is empty.");
    }

    root->LinkEndChild(entry_root);
    return entry_root;
}

bool BacEntry::Compile(const TiXmlElement *root, bool small_17)
{
    if (!Utils::ReadAttrUnsigned(root, "flags", &flags))
    {
        DPRINTF("%s: Attribute flags is not optional.\n", FUNCNAME);
        return false;
    }

    for (int i = 0; i <= MAX_BAC_TYPE; i++)
        has_type[i] = false;

    for (const TiXmlElement *elem = root->FirstChildElement(); elem; elem = elem->NextSiblingElement())
    {
        if (elem->ValueStr() == "Animation")
        {
            BACAnimation t0;

            if (!t0.Compile(elem))
                return false;

            has_type[0] = true;
            type0.push_back(t0);
        }
        else if (elem->ValueStr() == "Hitbox")
        {
            BACHitbox t1;

            if (!t1.Compile(elem))
                return false;

            has_type[1] = true;
            type1.push_back(t1);
        }
        else if (elem->ValueStr() == "AccelerationMovement")
        {
            BACAccelerationMovement t2;

            if (!t2.Compile(elem))
                return false;

            has_type[2] = true;
            type2.push_back(t2);
        }
        else if (elem->ValueStr() == "BACType3")
        {
            BACType3 t3;

            if (!t3.Compile(elem))
                return false;

            has_type[3] = true;
            type3.push_back(t3);
        }
        else if (elem->ValueStr() == "MotionAdjust")
        {
            BACMotionAdjust t4;

            if (!t4.Compile(elem))
                return false;

            has_type[4] = true;
            type4.push_back(t4);
        }
        else if (elem->ValueStr() == "OpponentKnockback")
        {
            BACOpponentKnockback t5;

            if (!t5.Compile(elem))
                return false;

            has_type[5] = true;
            type5.push_back(t5);
        }
        else if (elem->ValueStr() == "BACType6")
        {
            BACType6 t6;

            if (!t6.Compile(elem))
                return false;

            has_type[6] = true;
            type6.push_back(t6);
        }
        else if (elem->ValueStr() == "BACType7")
        {
            BACType7 t7;

            if (!t7.Compile(elem))
                return false;

            has_type[7] = true;
            type7.push_back(t7);
        }
        else if (elem->ValueStr() == "Effect")
        {
            BACEffect t8;

            if (!t8.Compile(elem))
                return false;

            has_type[8] = true;
            type8.push_back(t8);
        }
        else if (elem->ValueStr() == "Projectile")
        {
            BACProjectile t9;

            if (!t9.Compile(elem))
                return false;

            has_type[9] = true;
            type9.push_back(t9);
        }
        else if (elem->ValueStr() == "Camera")
        {
            BACCamera t10;

            if (!t10.Compile(elem))
                return false;

            has_type[10] = true;
            type10.push_back(t10);
        }
        else if (elem->ValueStr() == "Sound")
        {
            BACSound t11;

            if (!t11.Compile(elem))
                return false;

            has_type[11] = true;
            type11.push_back(t11);
        }
        else if (elem->ValueStr() == "BACType12")
        {
            BACType12 t12;

            if (!t12.Compile(elem))
                return false;

            has_type[12] = true;
            type12.push_back(t12);
        }
        else if (elem->ValueStr() == "BACType13")
        {
            BACType13 t13;

            if (!t13.Compile(elem))
                return false;

            has_type[13] = true;
            type13.push_back(t13);
        }
        else if (elem->ValueStr() == "BACType14")
        {
            BACType14 t14;

            if (!t14.Compile(elem))
                return false;

            has_type[14] = true;
            type14.push_back(t14);
        }
        else if (elem->ValueStr() == "TransformControl")
        {
            BACTransformControl t15;

            if (!t15.Compile(elem))
                return false;

            has_type[15] = true;
            type15.push_back(t15);
        }
        else if (elem->ValueStr() == "BACType16")
        {
            BACType16 t16;

            if (!t16.Compile(elem))
                return false;

            has_type[16] = true;
            type16.push_back(t16);
        }
        else if (elem->ValueStr() == "BACType17")
        {
            BACType17 t17;

            if (!t17.Compile(elem, small_17))
                return false;

            has_type[17] = true;
            type17.push_back(t17);
        }
        else if (elem->ValueStr() == "BACType18")
        {
            BACType18 t18;

            if (!t18.Compile(elem))
                return false;

            has_type[18] = true;
            type18.push_back(t18);
        }
        else if (elem->ValueStr() == "BACType19")
        {
            BACType19 t19;

            if (!t19.Compile(elem))
                return false;

            has_type[19] = true;
            type19.push_back(t19);
        }
        else if (elem->ValueStr() == "BACType20")
        {
            BACType20 t20;

            if (!t20.Compile(elem))
                return false;

            has_type[20] = true;
            type20.push_back(t20);
        }
        else if (elem->ValueStr() == "BACType21")
        {
            BACType21 t21;

            if (!t21.Compile(elem))
                return false;

            has_type[21] = true;
            type21.push_back(t21);
        }
        else if (elem->ValueStr() == "BACType22")
        {
            BACType22 t22;

            if (!t22.Compile(elem))
                return false;

            has_type[22] = true;
            type22.push_back(t22);
        }
        else if (elem->ValueStr() == "BACType23")
        {
            BACType23 t23;

            if (!t23.Compile(elem))
                return false;

            has_type[23] = true;
            type23.push_back(t23);
        }
    }

    for (int i = 0; i <= MAX_BAC_TYPE; i++)
    {
        if (!has_type[i])
        {
            const std::string param_name = "HAS_DUMMY" + Utils::ToString(i);
            uint32_t val;

            if (Utils::ReadParamUnsigned(root, param_name.c_str(), &val) && val)
            {
                has_type[i] = true;
            }
        }
    }

    valid = false;
    for (int i = 0; i <= MAX_BAC_TYPE; i++)
    {
        if (has_type[i])
        {
            valid = true;
            break;
        }
    }

    return true;
}

BacFile::BacFile()
{
    this->big_endian = false;
}

BacFile::~BacFile()
{

}

void BacFile::Reset()
{
    entries.clear();
    size17_small = false;
}

bool BacFile::Load(const uint8_t *buf, size_t size)
{
    Reset();

    if (!buf || size < sizeof(BACHeader))
        return false;

    const BACHeader *hdr = (const BACHeader *)buf;
    if (hdr->signature != BAC_SIGNATURE)
        return false;

    entries.resize(hdr->num_entries);

    const BACEntry *file_entries = (const BACEntry *)(buf+hdr->data_start);

    for (size_t i = 0; i < entries.size(); i++)
    {
        BacEntry &entry = entries[i];
        entry.flags = file_entries[i].flags;

        if (file_entries[i].num_subentries != 0)
        {
            entry.valid = true;            

            const BACSubEntry *file_subentries = (const BACSubEntry *)(buf+file_entries[i].sub_entry_offset);

            for (uint16_t j = 0; j < file_entries[i].num_subentries; j++)
            {
                uint16_t type = file_subentries[j].type;
                uint16_t num = file_subentries[j].num;

                if (type == 0)
                {
                    if (num > 0)
                    {
                        const BACAnimation *file_types0 = (const BACAnimation *)(buf+file_subentries[j].offset);
                        entry.type0.resize(num);

                        for (size_t k = 0; k < entry.type0.size(); k++)
                        {
                            entry.type0[k] = file_types0[k];
                        }
                    }
                }
                else if (type == 1)
                {
                    if (num > 0)
                    {
                        const BACHitbox *file_types1 = (const BACHitbox *)(buf+file_subentries[j].offset);
                        entry.type1.resize(num);

                        for (size_t k = 0; k < entry.type1.size(); k++)
                        {
                            entry.type1[k] = file_types1[k];
                        }
                    }
                }
                else if (type == 2)
                {
                    if (num > 0)
                    {
                        const BACAccelerationMovement *file_types2 = (const BACAccelerationMovement *)(buf+file_subentries[j].offset);
                        entry.type2.resize(num);

                        for (size_t k = 0; k < entry.type2.size(); k++)
                        {
                            entry.type2[k] = file_types2[k];
                        }
                    }
                }
                else if (type == 3)
                {
                    if (num > 0)
                    {
                        const BACType3 *file_types3 = (const BACType3 *)(buf+file_subentries[j].offset);
                        entry.type3.resize(num);

                        for (size_t k = 0; k < entry.type3.size(); k++)
                        {
                            entry.type3[k] = file_types3[k];
                        }
                    }
                }
                else if (type == 4)
                {
                    if (num > 0)
                    {
                        const BACMotionAdjust *file_types4 = (const BACMotionAdjust *)(buf+file_subentries[j].offset);
                        entry.type4.resize(num);

                        for (size_t k = 0; k < entry.type4.size(); k++)
                        {
                            entry.type4[k] = file_types4[k];
                        }
                    }
                }
                else if (type == 5)
                {
                    if (num > 0)
                    {
                        const BACOpponentKnockback *file_types5 = (const BACOpponentKnockback *)(buf+file_subentries[j].offset);
                        entry.type5.resize(num);

                        for (size_t k = 0; k < entry.type5.size(); k++)
                        {
                            entry.type5[k] = file_types5[k];
                        }
                    }
                }
                else if (type == 6)
                {
                    if (num > 0)
                    {
                        const BACType6 *file_types6 = (const BACType6 *)(buf+file_subentries[j].offset);
                        entry.type6.resize(num);

                        for (size_t k = 0; k < entry.type6.size(); k++)
                        {
                            entry.type6[k] = file_types6[k];
                        }
                    }
                }
                else if (type == 7)
                {
                    if (num > 0)
                    {
                        const BACType7 *file_types7 = (const BACType7 *)(buf+file_subentries[j].offset);
                        entry.type7.resize(num);

                        for (size_t k = 0; k < entry.type7.size(); k++)
                        {
                            entry.type7[k] = file_types7[k];
                        }
                    }
                }
                else if (type == 8)
                {
                    if (num > 0)
                    {
                        const BACEffect *file_types8 = (const BACEffect *)(buf+file_subentries[j].offset);
                        entry.type8.resize(num);

                        for (size_t k = 0; k < entry.type8.size(); k++)
                        {
                            entry.type8[k] = file_types8[k];
                        }
                    }
                }
                else if (type == 9)
                {
                    if (num > 0)
                    {
                        const BACProjectile *file_types9 = (const BACProjectile *)(buf+file_subentries[j].offset);
                        entry.type9.resize(num);

                        for (size_t k = 0; k < entry.type9.size(); k++)
                        {
                            entry.type9[k] = file_types9[k];
                        }
                    }
                }
                else if (type == 10)
                {
                    if (num > 0)
                    {
                        const BACCamera *file_types10 = (const BACCamera *)(buf+file_subentries[j].offset);
                        entry.type10.resize(num);

                        for (size_t k = 0; k < entry.type10.size(); k++)
                        {
                            entry.type10[k] = file_types10[k];
                        }
                    }
                }
                else if (type == 11)
                {
                    if (num > 0)
                    {
                        const BACSound *file_types11 = (const BACSound *)(buf+file_subentries[j].offset);
                        entry.type11.resize(num);

                        for (size_t k = 0; k < entry.type11.size(); k++)
                        {
                            entry.type11[k] = file_types11[k];
                        }
                    }
                }
                else if (type == 12)
                {
                    if (num > 0)
                    {
                        const BACType12 *file_types12 = (const BACType12 *)(buf+file_subentries[j].offset);
                        entry.type12.resize(num);

                        for (size_t k = 0; k < entry.type12.size(); k++)
                        {
                            entry.type12[k] = file_types12[k];
                        }
                    }
                }
                else if (type == 13)
                {
                    if (num > 0)
                    {
                        const BACType13 *file_types13 = (const BACType13 *)(buf+file_subentries[j].offset);
                        entry.type13.resize(num);

                        for (size_t k = 0; k < entry.type13.size(); k++)
                        {
                            entry.type13[k] = file_types13[k];
                        }
                    }
                }
                else if (type == 14)
                {
                    if (num > 0)
                    {
                        const BACType14 *file_types14 = (const BACType14 *)(buf+file_subentries[j].offset);
                        entry.type14.resize(num);

                        for (size_t k = 0; k < entry.type14.size(); k++)
                        {
                            entry.type14[k] = file_types14[k];
                        }
                    }
                }
                else if (type == 15)
                {
                    if (num > 0)
                    {
                        const BACTransformControl *file_types15 = (const BACTransformControl *)(buf+file_subentries[j].offset);
                        entry.type15.resize(num);

                        for (size_t k = 0; k < entry.type15.size(); k++)
                        {
                            entry.type15[k] = file_types15[k];
                        }
                    }
                }
                else if (type == 16)
                {
                    if (num > 0)
                    {
                        const BACType16 *file_types16 = (const BACType16 *)(buf+file_subentries[j].offset);
                        entry.type16.resize(num);

                        for (size_t k = 0; k < entry.type16.size(); k++)
                        {
                            entry.type16[k] = file_types16[k];
                        }
                    }
                }
                else if (type == 17)
                {
                    if (num > 0)
                    {
                        uint32_t size = 0;

                        if (!size17_small)
                        {
                            if (j != (file_entries[i].num_subentries-1))
                            {
                                size = (file_subentries[j+1].offset - file_subentries[j].offset) / num;
                            }
                            else if (i != (entries.size()-1) && file_entries[i+1].num_subentries > 0)
                            {
                                const BACSubEntry *next_subentry = (const BACSubEntry *)(buf+file_entries[i+1].sub_entry_offset);

                                if (next_subentry->num > 0)
                                    size = (next_subentry->offset - file_subentries[j].offset) / num;
                            }


                            //DPRINTF("size = 0x%x\n", size);

                            if (size == 0x14)
                                size17_small = true;
                        }


                        const BACType17 *file_types17 = (const BACType17 *)(buf+file_subentries[j].offset);
                        entry.type17.resize(num);

                        for (size_t k = 0; k < entry.type17.size(); k++)
                        {
                            if (size17_small)
                            {
                                entry.type17[k].start_time = file_types17->start_time;
                                entry.type17[k].duration = file_types17->duration;
                                entry.type17[k].unk_04 = file_types17->unk_04;
                                entry.type17[k].unk_08 = file_types17->unk_08;
                                entry.type17[k].unk_0A = file_types17->unk_0A;
                                entry.type17[k].unk_0C = file_types17->unk_0C;
                                entry.type17[k].unk_0E = file_types17->unk_0E;
                                entry.type17[k].unk_10 = file_types17->unk_10;
                                entry.type17[k].unk_12 = file_types17->unk_12;
                                file_types17 = (const BACType17 *)GetOffsetPtr(file_types17, 0x14, true);
                            }
                            else
                            {
                                entry.type17[k] = file_types17[k];
                            }
                        }
                    }
                }
                else if (type == 18)
                {
                    if (num > 0)
                    {
                        const BACType18 *file_types18 = (const BACType18 *)(buf+file_subentries[j].offset);
                        entry.type18.resize(num);

                        for (size_t k = 0; k < entry.type18.size(); k++)
                        {
                            entry.type18[k] = file_types18[k];
                        }
                    }
                }
                else if (type == 19)
                {
                    if (num > 0)
                    {
                        const BACType19 *file_types19 = (const BACType19 *)(buf+file_subentries[j].offset);
                        entry.type19.resize(num);

                        for (size_t k = 0; k < entry.type19.size(); k++)
                        {
                            entry.type19[k] = file_types19[k];
                        }
                    }
                }
                else if (type == 20)
                {
                    if (num > 0)
                    {
                        const BACType20 *file_types20 = (const BACType20 *)(buf+file_subentries[j].offset);
                        entry.type20.resize(num);

                        for (size_t k = 0; k < entry.type20.size(); k++)
                        {
                            entry.type20[k] = file_types20[k];
                        }
                    }
                }
                else if (type == 21)
                {
                    if (num > 0)
                    {
                        const BACType21 *file_types21 = (const BACType21 *)(buf+file_subentries[j].offset);
                        entry.type21.resize(num);

                        for (size_t k = 0; k < entry.type21.size(); k++)
                        {
                            entry.type21[k] = file_types21[k];
                        }
                    }
                }
                else if (type == 22)
                {
                    if (num > 0)
                    {
                        const BACType22 *file_types22 = (const BACType22 *)(buf+file_subentries[j].offset);
                        entry.type22.resize(num);

                        for (size_t k = 0; k < entry.type22.size(); k++)
                        {
                            entry.type22[k] = file_types22[k];
                        }
                    }
                }
                else if (type == 23)
                {
                    if (num > 0)
                    {
                        const BACType23 *file_types23 = (const BACType23 *)(buf+file_subentries[j].offset);
                        entry.type23.resize(num);

                        for (size_t k = 0; k < entry.type23.size(); k++)
                        {
                            entry.type23[k] = file_types23[k];
                        }
                    }
                }
                else
                {
                    DPRINTF("%s: Unrecognized bac type: %d (offset=0x%x, subentry=0x%x)\n", FUNCNAME, type, file_subentries[j].offset, Utils::DifPointer(&file_subentries[j], buf));
                    return false;
                }

                entry.has_type[type] = true;
            }
        }
    }

    memcpy(unk_14, hdr->unk_14, sizeof(unk_14));
    memcpy(unk_20, hdr->unk_20, sizeof(unk_20));
    memcpy(unk_48, hdr->unk_48, sizeof(unk_48));

    return true;
}

size_t BacFile::CalculateFileSize() const
{
    size_t size = sizeof(BACHeader) + entries.size() * sizeof(BACEntry);

    for (const BacEntry &entry :entries)
    {
        if (!entry.valid)
            continue;

        size += entry.GetNumSubEntries() * sizeof(BACSubEntry);
        size += entry.type0.size() * sizeof(BACAnimation);
        size += entry.type1.size() * sizeof(BACHitbox);
        size += entry.type2.size() * sizeof(BACAccelerationMovement);
        size += entry.type3.size() * sizeof(BACType3);
        size += entry.type4.size() * sizeof(BACMotionAdjust);
        size += entry.type5.size() * sizeof(BACOpponentKnockback);
        size += entry.type6.size() * sizeof(BACType6);
        size += entry.type7.size() * sizeof(BACType7);
        size += entry.type8.size() * sizeof(BACEffect);
        size += entry.type9.size() * sizeof(BACProjectile);
        size += entry.type10.size() * sizeof(BACCamera);
        size += entry.type11.size() * sizeof(BACSound);
        size += entry.type12.size() * sizeof(BACType12);
        size += entry.type13.size() * sizeof(BACType13);
        size += entry.type14.size() * sizeof(BACType14);
        size += entry.type15.size() * sizeof(BACTransformControl);
        size += entry.type16.size() * sizeof(BACType16);

        if (size17_small)
            size += entry.type17.size() * 0x14;
        else
            size += entry.type17.size() * sizeof(BACType17);

        size += entry.type18.size() * sizeof(BACType18);
        size += entry.type19.size() * sizeof(BACType19);
        size += entry.type20.size() * sizeof(BACType20);
        size += entry.type21.size() * sizeof(BACType21);
        size += entry.type22.size() * sizeof(BACType22);
        size += entry.type23.size() * sizeof(BACType23);
    }

    return size;
}

size_t BacFile::GetDataStart() const
{
    size_t size = sizeof(BACHeader) + entries.size() * sizeof(BACEntry);

    for (const BacEntry &entry :entries)
    {
        if (!entry.valid)
            continue;

        size += entry.GetNumSubEntries() * sizeof(BACSubEntry);
    }

    return size;
}

uint8_t *BacFile::Save(size_t *psize)
{
    size_t size = CalculateFileSize();

    uint8_t *buf = new uint8_t[size];
    memset(buf, 0, size);

    BACHeader *hdr = (BACHeader *)buf;

    hdr->signature = BAC_SIGNATURE;
    hdr->endianess_check = 0xFFFE;
    hdr->num_entries = (uint32_t) entries.size();
    hdr->data_start = sizeof(BACHeader);
    memcpy(hdr->unk_14, unk_14, sizeof(unk_14));
    memcpy(hdr->unk_20, unk_20, sizeof(unk_20));
    memcpy(hdr->unk_48, unk_48, sizeof(unk_48));

    BACEntry *file_entries = (BACEntry *)(buf+sizeof(BACHeader));
    BACSubEntry *file_subentry = (BACSubEntry *)(file_entries + entries.size());
    uint8_t *ptr = buf + GetDataStart();

    for (size_t i = 0; i < entries.size(); i++)
    {
        const BacEntry &entry = entries[i];
        file_entries[i].flags = entry.flags;

        if (entry.valid)
        {
            file_entries[i].num_subentries = entry.GetNumSubEntries();
            file_entries[i].sub_entry_offset = Utils::DifPointer(file_subentry, buf);

            for (int j = 0; j <= MAX_BAC_TYPE; j++)
            {
                if (entry.has_type[j])
                {
                    file_subentry->type = j;

                    if (j == 0)
                    {
                        BACAnimation *file_type0 = (BACAnimation *)ptr;
                        file_subentry->num = (uint16_t) entry.type0.size();

                        if (file_subentry->num != 0)
                            file_subentry->offset = Utils::DifPointer(ptr, buf);

                        for (size_t k = 0; k < entry.type0.size(); k++)
                        {
                            *file_type0 = entry.type0[k];
                            file_type0++;
                            ptr = (uint8_t *)file_type0;
                        }
                    }
                    else if (j == 1)
                    {
                        BACHitbox *file_type1 = (BACHitbox *)ptr;
                        file_subentry->num = (uint16_t) entry.type1.size();

                        if (file_subentry->num != 0)
                            file_subentry->offset = Utils::DifPointer(ptr, buf);

                        for (size_t k = 0; k < entry.type1.size(); k++)
                        {
                            *file_type1 = entry.type1[k];
                            file_type1++;
                            ptr = (uint8_t *)file_type1;
                        }
                    }
                    else if (j == 2)
                    {
                        BACAccelerationMovement *file_type2 = (BACAccelerationMovement *)ptr;
                        file_subentry->num = (uint16_t) entry.type2.size();

                        if (file_subentry->num != 0)
                            file_subentry->offset = Utils::DifPointer(ptr, buf);

                        for (size_t k = 0; k < entry.type2.size(); k++)
                        {
                            *file_type2 = entry.type2[k];
                            file_type2++;
                            ptr = (uint8_t *)file_type2;
                        }
                    }
                    else if (j == 3)
                    {
                        BACType3 *file_type3 = (BACType3 *)ptr;
                        file_subentry->num = (uint16_t) entry.type3.size();

                        if (file_subentry->num != 0)
                            file_subentry->offset = Utils::DifPointer(ptr, buf);

                        for (size_t k = 0; k < entry.type3.size(); k++)
                        {
                            *file_type3 = entry.type3[k];
                            file_type3++;
                            ptr = (uint8_t *)file_type3;
                        }
                    }
                    else if (j == 4)
                    {
                        BACMotionAdjust *file_type4 = (BACMotionAdjust *)ptr;
                        file_subentry->num = (uint16_t) entry.type4.size();

                        if (file_subentry->num != 0)
                            file_subentry->offset = Utils::DifPointer(ptr, buf);

                        for (size_t k = 0; k < entry.type4.size(); k++)
                        {
                            *file_type4 = entry.type4[k];
                            file_type4++;
                            ptr = (uint8_t *)file_type4;
                        }
                    }
                    else if (j == 5)
                    {
                        BACOpponentKnockback *file_type5 = (BACOpponentKnockback *)ptr;
                        file_subentry->num = (uint16_t) entry.type5.size();

                        if (file_subentry->num != 0)
                            file_subentry->offset = Utils::DifPointer(ptr, buf);

                        for (size_t k = 0; k < entry.type5.size(); k++)
                        {
                            *file_type5 = entry.type5[k];
                            file_type5++;
                            ptr = (uint8_t *)file_type5;
                        }
                    }
                    else if (j == 6)
                    {
                        BACType6 *file_type6 = (BACType6 *)ptr;
                        file_subentry->num = (uint16_t) entry.type6.size();

                        if (file_subentry->num != 0)
                            file_subentry->offset = Utils::DifPointer(ptr, buf);

                        for (size_t k = 0; k < entry.type6.size(); k++)
                        {
                            *file_type6 = entry.type6[k];
                            file_type6++;
                            ptr = (uint8_t *)file_type6;
                        }
                    }
                    else if (j == 7)
                    {
                        BACType7 *file_type7 = (BACType7 *)ptr;
                        file_subentry->num = (uint16_t) entry.type7.size();

                        if (file_subentry->num != 0)
                            file_subentry->offset = Utils::DifPointer(ptr, buf);

                        for (size_t k = 0; k < entry.type7.size(); k++)
                        {
                            *file_type7 = entry.type7[k];
                            file_type7++;
                            ptr = (uint8_t *)file_type7;
                        }
                    }
                    else if (j == 8)
                    {
                        BACEffect *file_type8 = (BACEffect *)ptr;
                        file_subentry->num = (uint16_t) entry.type8.size();

                        if (file_subentry->num != 0)
                            file_subentry->offset = Utils::DifPointer(ptr, buf);

                        for (size_t k = 0; k < entry.type8.size(); k++)
                        {
                            *file_type8 = entry.type8[k];
                            file_type8++;
                            ptr = (uint8_t *)file_type8;
                        }
                    }
                    else if (j == 9)
                    {
                        BACProjectile *file_type9 = (BACProjectile *)ptr;
                        file_subentry->num = (uint16_t) entry.type9.size();

                        if (file_subentry->num != 0)
                            file_subentry->offset = Utils::DifPointer(ptr, buf);

                        for (size_t k = 0; k < entry.type9.size(); k++)
                        {
                            *file_type9 = entry.type9[k];
                            file_type9++;
                            ptr = (uint8_t *)file_type9;
                        }
                    }
                    else if (j == 10)
                    {
                        BACCamera *file_type10 = (BACCamera *)ptr;
                        file_subentry->num = (uint16_t) entry.type10.size();

                        if (file_subentry->num != 0)
                            file_subentry->offset = Utils::DifPointer(ptr, buf);

                        for (size_t k = 0; k < entry.type10.size(); k++)
                        {
                            *file_type10 = entry.type10[k];
                            file_type10++;
                            ptr = (uint8_t *)file_type10;
                        }
                    }
                    else if (j == 11)
                    {
                        BACSound *file_type11 = (BACSound *)ptr;
                        file_subentry->num = (uint16_t) entry.type11.size();

                        if (file_subentry->num != 0)
                            file_subentry->offset = Utils::DifPointer(ptr, buf);

                        for (size_t k = 0; k < entry.type11.size(); k++)
                        {
                            *file_type11 = entry.type11[k];
                            file_type11++;
                            ptr = (uint8_t *)file_type11;
                        }
                    }
                    else if (j == 12)
                    {
                        BACType12 *file_type12 = (BACType12 *)ptr;
                        file_subentry->num = (uint16_t) entry.type12.size();

                        if (file_subentry->num != 0)
                            file_subentry->offset = Utils::DifPointer(ptr, buf);

                        for (size_t k = 0; k < entry.type12.size(); k++)
                        {
                            *file_type12 = entry.type12[k];
                            file_type12++;
                            ptr = (uint8_t *)file_type12;
                        }
                    }
                    else if (j == 13)
                    {
                        BACType13 *file_type13 = (BACType13 *)ptr;
                        file_subentry->num = (uint16_t) entry.type13.size();

                        if (file_subentry->num != 0)
                            file_subentry->offset = Utils::DifPointer(ptr, buf);

                        for (size_t k = 0; k < entry.type13.size(); k++)
                        {
                            *file_type13 = entry.type13[k];
                            file_type13++;
                            ptr = (uint8_t *)file_type13;
                        }
                    }
                    else if (j == 14)
                    {
                        BACType14 *file_type14 = (BACType14 *)ptr;
                        file_subentry->num = (uint16_t) entry.type14.size();

                        if (file_subentry->num != 0)
                            file_subentry->offset = Utils::DifPointer(ptr, buf);

                        for (size_t k = 0; k < entry.type14.size(); k++)
                        {
                            *file_type14 = entry.type14[k];
                            file_type14++;
                            ptr = (uint8_t *)file_type14;
                        }
                    }
                    else if (j == 15)
                    {
                        BACTransformControl *file_type15 = (BACTransformControl *)ptr;
                        file_subentry->num = (uint16_t) entry.type15.size();

                        if (file_subentry->num != 0)
                            file_subentry->offset = Utils::DifPointer(ptr, buf);

                        for (size_t k = 0; k < entry.type15.size(); k++)
                        {
                            *file_type15 = entry.type15[k];
                            file_type15++;
                            ptr = (uint8_t *)file_type15;
                        }
                    }
                    else if (j == 16)
                    {
                        BACType16 *file_type16 = (BACType16 *)ptr;
                        file_subentry->num = (uint16_t) entry.type16.size();

                        if (file_subentry->num != 0)
                            file_subentry->offset = Utils::DifPointer(ptr, buf);

                        for (size_t k = 0; k < entry.type16.size(); k++)
                        {
                            *file_type16 = entry.type16[k];
                            file_type16++;
                            ptr = (uint8_t *)file_type16;
                        }
                    }
                    else if (j == 17)
                    {
                        BACType17 *file_type17 = (BACType17 *)ptr;
                        file_subentry->num = (uint16_t) entry.type17.size();

                        if (file_subentry->num != 0)
                            file_subentry->offset = Utils::DifPointer(ptr, buf);

                        for (size_t k = 0; k < entry.type17.size(); k++)
                        {
                            if (size17_small)
                            {
                                file_type17->start_time = entry.type17[k].start_time;
                                file_type17->duration = entry.type17[k].duration;
                                file_type17->unk_04 = entry.type17[k].unk_04;
                                file_type17->unk_08 = entry.type17[k].unk_08;
                                file_type17->unk_0A = entry.type17[k].unk_0A;
                                file_type17->unk_0C = entry.type17[k].unk_0C;
                                file_type17->unk_0E = entry.type17[k].unk_0E;
                                file_type17->unk_10 = entry.type17[k].unk_10;
                                file_type17->unk_12 = entry.type17[k].unk_12;

                                ptr += 0x14;
                                file_type17 = (BACType17 *)ptr;
                            }
                            else
                            {
                                *file_type17 = entry.type17[k];
                                file_type17++;
                                ptr = (uint8_t *)file_type17;
                            }
                        }
                    }
                    else if (j == 18)
                    {
                        BACType18 *file_type18 = (BACType18 *)ptr;
                        file_subentry->num = (uint16_t) entry.type18.size();

                        if (file_subentry->num != 0)
                            file_subentry->offset = Utils::DifPointer(ptr, buf);

                        for (size_t k = 0; k < entry.type18.size(); k++)
                        {
                            *file_type18 = entry.type18[k];
                            file_type18++;
                            ptr = (uint8_t *)file_type18;
                        }
                    }
                    else if (j == 19)
                    {
                        BACType19 *file_type19 = (BACType19 *)ptr;
                        file_subentry->num = (uint16_t) entry.type19.size();

                        if (file_subentry->num != 0)
                            file_subentry->offset = Utils::DifPointer(ptr, buf);

                        for (size_t k = 0; k < entry.type19.size(); k++)
                        {
                            *file_type19 = entry.type19[k];
                            file_type19++;
                            ptr = (uint8_t *)file_type19;
                        }
                    }
                    else if (j == 20)
                    {
                        BACType20 *file_type20 = (BACType20 *)ptr;
                        file_subentry->num = (uint16_t) entry.type20.size();

                        if (file_subentry->num != 0)
                            file_subentry->offset = Utils::DifPointer(ptr, buf);

                        for (size_t k = 0; k < entry.type20.size(); k++)
                        {
                            *file_type20 = entry.type20[k];
                            file_type20++;
                            ptr = (uint8_t *)file_type20;
                        }
                    }
                    else if (j == 21)
                    {
                        BACType21 *file_type21 = (BACType21 *)ptr;
                        file_subentry->num = (uint16_t) entry.type21.size();

                        if (file_subentry->num != 0)
                            file_subentry->offset = Utils::DifPointer(ptr, buf);

                        for (size_t k = 0; k < entry.type21.size(); k++)
                        {
                            *file_type21 = entry.type21[k];
                            file_type21++;
                            ptr = (uint8_t *)file_type21;
                        }
                    }
                    else if (j == 22)
                    {
                        BACType22 *file_type22 = (BACType22 *)ptr;
                        file_subentry->num = (uint16_t) entry.type22.size();

                        if (file_subentry->num != 0)
                            file_subentry->offset = Utils::DifPointer(ptr, buf);

                        for (size_t k = 0; k < entry.type22.size(); k++)
                        {
                            *file_type22 = entry.type22[k];
                            file_type22++;
                            ptr = (uint8_t *)file_type22;
                        }
                    }
                    else if (j == 23)
                    {
                        BACType23 *file_type23 = (BACType23 *)ptr;
                        file_subentry->num = (uint16_t) entry.type23.size();

                        if (file_subentry->num != 0)
                            file_subentry->offset = Utils::DifPointer(ptr, buf);

                        for (size_t k = 0; k < entry.type23.size(); k++)
                        {
                            *file_type23 = entry.type23[k];
                            file_type23++;
                            ptr = (uint8_t *)file_type23;
                        }
                    }
                    else if (j == 13)
                    {
                        // Nothing
                    }
                    else
                    {
                        DPRINTF("%s Internal error.\n", FUNCNAME);
                        return nullptr;
                    }

                    file_subentry++;
                }
            }
        }
    }

    *psize = size;
    return buf;
}

TiXmlDocument *BacFile::Decompile() const
{
    TiXmlDocument *doc = new TiXmlDocument();

    TiXmlDeclaration* decl = new TiXmlDeclaration("1.0", "utf-8", "" );
    doc->LinkEndChild(decl);

    TiXmlElement *root = new TiXmlElement("BAC");

    Utils::WriteComment(root, "FLAG (in all types). Flag 1 -> entry applies to CAC; Flag 2 -> entry applies to roster character (that includes pseudo-cacs of cac2x2m)");

    if (size17_small)
        Utils::WriteParamUnsigned(root, "SIZE17_SMALL", 1);

    Utils::WriteParamMultipleUnsigned(root, "U_14", std::vector<uint32_t>(unk_14, unk_14+3), true);
    Utils::WriteParamMultipleFloats(root, "F_20", std::vector<float>(unk_20, unk_20+10));
    Utils::WriteParamMultipleUnsigned(root, "U_48", std::vector<uint32_t>(unk_48, unk_48+6), true);

    for (const BacEntry &entry : entries)
    {
        entry.Decompile(root, size17_small);
    }

    doc->LinkEndChild(root);
    return doc;
}

bool BacFile::Compile(TiXmlDocument *doc, bool )
{
    Reset();

    TiXmlHandle handle(doc);
    const TiXmlElement *root = Utils::FindRoot(&handle, "BAC");

    if (!root)
    {
        DPRINTF("Cannot find\"BAC\" in xml.\n");
        return false;
    }

    uint32_t temp;
    if (Utils::ReadParamUnsigned(root, "SIZE17_SMALL", &temp) && temp)
        size17_small = true;

    if (!Utils::GetParamMultipleUnsigned(root, "U_14", unk_14, 3))
        return false;

    if (!Utils::GetParamMultipleFloats(root, "F_20", unk_20, 10))
        return false;

    if (!Utils::GetParamMultipleUnsigned(root, "U_48", unk_48, 6))
        return false;

    for (const TiXmlElement *elem = root->FirstChildElement(); elem; elem = elem->NextSiblingElement())
    {
        if (elem->ValueStr() == "BacEntry")
        {
            BacEntry entry;

            if (!entry.Compile(elem, size17_small))
                return false;

            entries.push_back(entry);
        }
    }

    return true;
}

size_t BacFile::ChangeReferencesToSkill(uint16_t old_skill, uint16_t new_skill)
{
    size_t count = 0;

    for (BacEntry &entry : entries)
    {
        if (!entry.valid)
            continue;

        for (BACEffect &t8: entry.type8)
        {
            if (t8.flags != 0 && t8.id == old_skill)
            {
                t8.id = new_skill;
                count++;
            }
        }

        for (BACProjectile &t9: entry.type9)
        {
            if (t9.id == old_skill)
            {
                t9.id = new_skill;
                count++;
            }
        }
    }

    return count;
}
