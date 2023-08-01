#ifndef BCMFILE_H
#define BCMFILE_H

#include "BaseFile.h"

#define BCM_SIGNATURE   0x4D434223

#ifdef _MSC_VER
#pragma pack(push,1)
#endif

typedef struct
{
    uint32_t signature; // 0
    uint16_t endianess_check;  // 4
    uint16_t unk_06;
    uint32_t num_entries; // 8
    uint32_t data_start; // 0xC
} PACKED BCMHeader;

STATIC_ASSERT_STRUCT(BCMHeader, 0x10);

typedef struct
{
    uint32_t unk_00;
    uint32_t unk_04;
    uint16_t unk_08;
    uint16_t unk_0A;
    uint32_t unk_0C;
    uint32_t unk_10;
    uint32_t unk_14;
    uint32_t unk_18;
    uint32_t unk_1C;
    uint16_t bac_index;
    uint16_t unk_22;
    uint16_t unk_24;
    uint16_t unk_26;
    uint16_t unk_28;
    uint16_t bac_index2;
    uint16_t unk_2C;
    uint16_t unk_2E;
    uint32_t ptr_30;
    uint32_t ptr_34;
    uint32_t unk_38;
    uint32_t ptr_3C;
    uint32_t ki_cost;
    uint32_t unk_44;
    uint32_t unk_48;
    uint32_t unk_4C;
    uint32_t unk_50;
    uint32_t stamina_cost;
    uint32_t unk_58;
    uint32_t ki_required;
    uint32_t health_required;
    uint16_t trans_modifier;
    uint16_t unk_66;
    uint32_t unk_68;
    uint32_t unk_6C;
} PACKED BCMEntry;

STATIC_ASSERT_STRUCT(BCMEntry, 0x70);

#ifdef _MSC_VER
#pragma pack(pop)
#endif

struct BcmEntry
{
    uint32_t unk_00;
    uint32_t unk_04;
    uint16_t unk_08;
    uint16_t unk_0A;
    uint32_t unk_0C;
    uint32_t unk_10;
    uint32_t unk_14;
    uint32_t unk_18;
    uint32_t unk_1C;
    uint16_t bac_index;
    uint16_t unk_22;
    uint16_t unk_24;
    uint16_t unk_26;
    uint16_t unk_28;
    uint16_t bac_index2;
    uint16_t unk_2C;
    uint16_t unk_2E;
    uint32_t ptr_30;
    uint32_t ptr_34;
    uint32_t unk_38;
    uint32_t ptr_3C;
    uint32_t ki_cost;
    uint32_t unk_44;
    uint32_t unk_48;
    uint32_t unk_4C;
    uint32_t unk_50;
    uint32_t stamina_cost;
    uint32_t unk_58;
    uint32_t ki_required;
    uint32_t health_required;
    uint16_t trans_modifier;
    uint16_t unk_66;
    uint32_t unk_68;
    uint32_t unk_6C;

    TiXmlElement *Decompile(TiXmlNode *root, uint32_t idx) const;
    bool Compile(const TiXmlElement *root);
};

class BcmFile : public BaseFile
{
private:

    std::vector<BcmEntry> entries;

protected:

    void Reset();

public:

    BcmFile();
    virtual ~BcmFile();

    virtual bool Load(const uint8_t *buf, size_t size) override;
    virtual uint8_t *Save(size_t *psize) override;

    virtual TiXmlDocument *Decompile() const override;
    virtual bool Compile(TiXmlDocument *doc, bool big_endian=false) override;
};

#endif // BCMFILE_H
