#ifndef BACFILE_H
#define BACFILE_H

#include "BaseFile.h"

/*Type 0 - Animation
Type 1 - hit box
Type 2 - Acceleration/Movement
Type 4 - something to do with motion adjusting
Type 5 - opponent knockback
Type 8 - effects
type 9 - Projectile/Wave/etc Launch
type 10 - camera
*/


// 00 -> start time
// 02 -> length

// Xenoverse 2 bac files only.
#define BAC_SIGNATURE   0x43414223
#define MAX_BAC_TYPE    23

#pragma pack(push,1)

typedef struct
{
    uint32_t signature; // 0
    uint16_t endianess_check; // 4
    uint16_t unk_06; // Always zero
    uint32_t num_entries; // 8
    uint32_t unk_0C; // Zlways zero
    uint32_t data_start; // 0x10
    uint32_t unk_14[3]; // Zero
    float unk_20[10];
    uint32_t unk_48[6]; // Zero
} PACKED BACHeader;

STATIC_ASSERT_STRUCT(BACHeader, 0x60);

typedef struct
{
    uint32_t flags; // 0
    uint16_t num_subentries; // 4
    uint16_t unk_06; // Zero, probably padding
    uint32_t sub_entry_offset; // 8 aboslute offset
    uint32_t unk_0C; //Zero, probably padding
} PACKED BACEntry;

STATIC_ASSERT_STRUCT(BACEntry, 0x10);

typedef struct
{
    uint16_t type; // 0
    uint16_t num; // 2
    uint32_t unk_04; //
    uint32_t offset; // 8, absolute offset
    uint32_t unk_0C; // 0xC
} PACKED BACSubEntry;

STATIC_ASSERT_STRUCT(BACSubEntry, 0x10);

struct BACAnimation // Skill control and animation
{
    uint16_t start_time;
    uint16_t duration;
    uint16_t unk_04;
    uint16_t flags;
    uint16_t ean_to_use;
    uint16_t ean_index;
    uint16_t unk_0C;
    uint16_t unk_0E;
    uint16_t unk_10;
    uint16_t unk_12;
    uint16_t unk_14;
    uint16_t unk_16;
    float animation_speed;
    float unk_1C;
    float unk_20;

    TiXmlElement *Decompile(TiXmlNode *root) const;
    bool Compile(const TiXmlElement *root);
};

STATIC_ASSERT_STRUCT(BACAnimation, 0x24);

// Hit box
struct BACHitbox
{
    uint16_t start_time;
    uint16_t duration;
    uint16_t unk_04;
    uint16_t flags;
    uint16_t unk_08;
    uint16_t unk_0A;
    uint32_t unk_0C;
    uint16_t unk_10;
    uint16_t unk_12;
    uint32_t unk_14[2];
    float unk_1C[9];

    TiXmlElement *Decompile(TiXmlNode *root) const;
    bool Compile(const TiXmlElement *root);
};

STATIC_ASSERT_STRUCT(BACHitbox, 0x40);

struct BACAccelerationMovement
{
    uint16_t start_time;
    uint16_t duration;
    uint16_t unk_04;
    uint16_t flags;
    uint16_t unk_08;
    uint16_t unk_0A;
    uint32_t unk_0C;
    float unk_10;
    float unk_14;
    uint32_t unk_18;
    float unk_1C;
    uint32_t unk_20;

    TiXmlElement *Decompile(TiXmlNode *root) const;
    bool Compile(const TiXmlElement *root);
};

STATIC_ASSERT_STRUCT(BACAccelerationMovement, 0x24);

struct BACType3
{
    uint16_t start_time;
    uint16_t duration;
    uint16_t unk_04;
    uint16_t flags;
    uint32_t unk_08;

    TiXmlElement *Decompile(TiXmlNode *root) const;
    bool Compile(const TiXmlElement *root);
};

STATIC_ASSERT_STRUCT(BACType3, 0xC);

struct BACMotionAdjust
{
    uint16_t start_time;
    uint16_t duration;
    uint16_t unk_04;
    uint16_t flags;
    float unk_08;

    TiXmlElement *Decompile(TiXmlNode *root) const;
    bool Compile(const TiXmlElement *root);
};

STATIC_ASSERT_STRUCT(BACMotionAdjust, 0xC);

struct BACOpponentKnockback
{
    uint16_t start_time;
    uint16_t duration;
    uint16_t unk_04;
    uint16_t flags;
    float unk_08;
    uint16_t unk_0C;
    uint16_t unk_0E;

    TiXmlElement *Decompile(TiXmlNode *root) const;
    bool Compile(const TiXmlElement *root);
};

STATIC_ASSERT_STRUCT(BACOpponentKnockback, 0x10);

struct BACType6
{
    uint16_t start_time;
    uint16_t duration;
    uint16_t unk_04;
    uint16_t flags;
    uint32_t unk_08;
    uint16_t unk_0C;
    uint16_t unk_0E;

    TiXmlElement *Decompile(TiXmlNode *root) const;
    bool Compile(const TiXmlElement *root);
};

STATIC_ASSERT_STRUCT(BACType6, 0x10);

struct BACType7
{
    uint16_t start_time;
    uint16_t duration;
    uint16_t unk_04;
    uint16_t flags;
    uint16_t unk_08;
    uint16_t unk_0A;

    TiXmlElement *Decompile(TiXmlNode *root) const;
    bool Compile(const TiXmlElement *root);
};

STATIC_ASSERT_STRUCT(BACType7, 0xC);

struct BACEffect
{
    uint16_t start_time;
    uint16_t duration;
    uint32_t unk_04;
    uint16_t flags; // 8
    uint16_t unk_0A;
    uint16_t id; // 0xC
    uint16_t unk_0E;
    uint32_t unk_10[8];

    TiXmlElement *Decompile(TiXmlNode *root) const;
    bool Compile(const TiXmlElement *root);
};

STATIC_ASSERT_STRUCT(BACEffect, 0x30);

struct BACProjectile
{
    uint16_t start_time;
    uint16_t duration;
    uint16_t unk_04;
    uint16_t flags;
    uint16_t id; // 8
    uint16_t unk_0A;
    uint32_t unk_0C; // Write as decimal
    uint16_t unk_10;
    uint16_t unk_12;
    float unk_14[3]; // Coordinates
    uint32_t unk_20;
    uint32_t unk_24;
    float unk_28;
    uint16_t unk_2C;
    uint16_t unk_2E;
    uint32_t unk_30[4];

    TiXmlElement *Decompile(TiXmlNode *root) const;
    bool Compile(const TiXmlElement *root);
};

STATIC_ASSERT_STRUCT(BACProjectile, 0x40);

struct BACCamera
{
    uint16_t start_time;
    uint16_t duration;
    uint16_t unk_04;
    uint16_t flags;
    uint16_t ean_to_use;
    uint16_t unk_0A;
    uint16_t ean_index;
    uint16_t unk_0E;
    uint16_t unk_10;
    uint16_t unk_12;
    uint32_t unk_14[13];
    uint16_t unk_48;
    uint16_t unk_4A;

    TiXmlElement *Decompile(TiXmlNode *root) const;
    bool Compile(const TiXmlElement *root);
};

STATIC_ASSERT_STRUCT(BACCamera, 0x4C);

struct BACSound
{
    uint16_t start_time;
    uint16_t duration;
    uint16_t unk_04;
    uint16_t flags;
    uint16_t acb_to_use;
    uint16_t unk_0A;
    uint16_t cue_id;
    uint16_t unk_0E;

    TiXmlElement *Decompile(TiXmlNode *root) const;
    bool Compile(const TiXmlElement *root);
};

STATIC_ASSERT_STRUCT(BACSound, 0x10);

struct BACType12
{
    uint16_t start_time;
    uint16_t duration;
    uint16_t unk_04;
    uint16_t flags;
    uint16_t unk_08;
    uint16_t unk_0A;

    TiXmlElement *Decompile(TiXmlNode *root) const;
    bool Compile(const TiXmlElement *root);
};

STATIC_ASSERT_STRUCT(BACType12, 0xC);

struct BACType13
{
    uint16_t start_time;
    uint16_t duration;
    uint16_t unk_04;
    uint16_t flags;
    uint16_t unk_08;
    uint16_t unk_0A;

    TiXmlElement *Decompile(TiXmlNode *root) const;
    bool Compile(const TiXmlElement *root);
};

STATIC_ASSERT_STRUCT(BACType13, 0xC);

struct BACType14
{
    uint16_t start_time;
    uint16_t duration;
    uint16_t unk_04;
    uint16_t flags;
    uint16_t unk_08;
    uint16_t unk_0A;

    TiXmlElement *Decompile(TiXmlNode *root) const;
    bool Compile(const TiXmlElement *root);
};

STATIC_ASSERT_STRUCT(BACType14, 0xC);

struct BACTransformControl
{
    uint16_t start_time;
    uint16_t duration;
    uint16_t unk_04;
    uint16_t flags;
    uint16_t type;
    uint16_t unk_0A;
    float parameter;
    uint32_t unk_10[4];

    TiXmlElement *Decompile(TiXmlNode *root) const;
    bool Compile(const TiXmlElement *root);
};

STATIC_ASSERT_STRUCT(BACTransformControl, 0x20);

struct BACType16
{
    uint16_t start_time;
    uint16_t duration;
    uint16_t unk_04;
    uint16_t flags;
    uint16_t unk_08;
    uint16_t unk_0A;
    uint16_t unk_0C;
    uint16_t unk_0E;
    uint32_t unk_10[4];

    TiXmlElement *Decompile(TiXmlNode *root) const;
    bool Compile(const TiXmlElement *root);
};

STATIC_ASSERT_STRUCT(BACType16, 0x20);

struct BACType17
{
    uint16_t start_time;
    uint16_t duration;
    uint16_t unk_04;
    uint16_t flags;
    uint16_t unk_08;
    uint16_t unk_0A;
    uint16_t unk_0C;
    uint16_t unk_0E;
    uint16_t unk_10;
    uint16_t unk_12;
    uint32_t unk_14[3];

    TiXmlElement *Decompile(TiXmlNode *root, bool _small) const;
    bool Compile(const TiXmlElement *root, bool _small);
};

STATIC_ASSERT_STRUCT(BACType17, 0x20);

struct BACType18
{
    uint16_t start_time;
    uint16_t duration;
    uint16_t unk_04;
    uint16_t flags;
    uint32_t unk_08[6];

    TiXmlElement *Decompile(TiXmlNode *root) const;
    bool Compile(const TiXmlElement *root);
};

STATIC_ASSERT_STRUCT(BACType18, 0x20);

struct BACType19
{
    uint16_t start_time;
    uint16_t duration;
    uint16_t unk_04;
    uint16_t flags;
    uint16_t unk_08;
    uint16_t unk_0A;
    uint32_t unk_0C;

    TiXmlElement *Decompile(TiXmlNode *root) const;
    bool Compile(const TiXmlElement *root);
};

STATIC_ASSERT_STRUCT(BACType19, 0x10);

struct BACType20
{
    uint16_t start_time;
    uint16_t duration;
    uint16_t unk_04;
    uint16_t flags;
    uint16_t unk_08;
    uint16_t unk_0A;
    uint16_t unk_0C;
    uint16_t unk_0E;
    uint16_t unk_10;
    uint16_t unk_12;
    uint32_t unk_14;
    float unk_18;
    uint32_t unk_1C[5];

    TiXmlElement *Decompile(TiXmlNode *root) const;
    bool Compile(const TiXmlElement *root);
};

STATIC_ASSERT_STRUCT(BACType20, 0x30);

struct BACType21
{
    uint16_t start_time;
    uint16_t duration;
    uint16_t unk_04;
    uint16_t flags;
    uint32_t unk_08;
    uint16_t unk_0C;
    uint16_t unk_0E;
    uint16_t unk_10;
    uint16_t unk_12;
    uint16_t unk_14;
    uint16_t unk_16;
    float unk_18;
    float unk_1C;

    TiXmlElement *Decompile(TiXmlNode *root) const;
    bool Compile(const TiXmlElement *root);
};

STATIC_ASSERT_STRUCT(BACType21, 0x20);

struct BACType22
{
    uint16_t start_time;
    uint16_t duration;
    uint16_t unk_04;
    uint16_t flags;
    uint16_t unk_08;
    uint16_t unk_0A;
    float unk_0C;
    char name[32];

    TiXmlElement *Decompile(TiXmlNode *root) const;
    bool Compile(const TiXmlElement *root);
};

STATIC_ASSERT_STRUCT(BACType22, 0x30);

struct BACType23
{
    uint16_t start_time;
    uint16_t duration;
    uint16_t unk_04;
    uint16_t flags;
    uint16_t unk_08;
    uint16_t unk_0A;
    uint16_t unk_0C;
    uint16_t unk_0E;
    uint32_t unk_10;
    float unk_14[11];

    TiXmlElement *Decompile(TiXmlNode *root) const;
    bool Compile(const TiXmlElement *root);
};

STATIC_ASSERT_STRUCT(BACType23, 0x40);

#pragma pack(pop)

struct BacEntry
{
    bool valid;
    uint32_t flags; // Flags is used even if not valid
    bool has_type[MAX_BAC_TYPE+1];

    std::vector<BACAnimation> type0;
    std::vector<BACHitbox> type1;
    std::vector<BACAccelerationMovement> type2;
    std::vector<BACType3> type3;
    std::vector<BACMotionAdjust> type4;
    std::vector<BACOpponentKnockback> type5;
    std::vector<BACType6> type6;
    std::vector<BACType7> type7;
    std::vector<BACEffect> type8;
    std::vector<BACProjectile> type9;
    std::vector<BACCamera> type10;
    std::vector<BACSound> type11;
    std::vector<BACType12> type12;
    std::vector<BACType13> type13;
    std::vector<BACType14> type14;
    std::vector<BACTransformControl> type15;
    std::vector<BACType16> type16;
    std::vector<BACType17> type17;
    std::vector<BACType18> type18;
    std::vector<BACType19> type19;
    std::vector<BACType20> type20;
    std::vector<BACType21> type21;
    std::vector<BACType22> type22;
    std::vector<BACType23> type23;

    BacEntry() : valid(false) { memset(has_type, 0, sizeof(has_type)); }

    uint16_t GetNumSubEntries() const
    {
        uint16_t count = 0;

        for (int i = 0; i <= MAX_BAC_TYPE; i++)
            if (has_type[i])
                count++;

        return count;
    }

    TiXmlElement *Decompile(TiXmlNode *root, bool small_17) const;
    bool Compile(const TiXmlElement *root, bool small_17);
};

class BacFile : public BaseFile
{
private:

    uint32_t unk_14[3];
    float unk_20[10];
    uint32_t unk_48[6];

    std::vector<BacEntry> entries;
    bool size17_small;

protected:

    void Reset();

    size_t CalculateFileSize() const;
    size_t GetDataStart() const;

public:

    BacFile();
    virtual ~BacFile();

    virtual bool Load(const uint8_t *buf, size_t size) override;
    virtual uint8_t *Save(size_t *psize) override;

    virtual TiXmlDocument *Decompile() const override;
    virtual bool Compile(TiXmlDocument *doc, bool big_endian=false) override;

    size_t ChangeReferencesToSkill(uint16_t old_skill, uint16_t new_skill);
};

#endif // BACFILE_H
