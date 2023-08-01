#ifndef X2MCOSTUMEFILE_H
#define X2MCOSTUMEFILE_H

#include "BaseFile.h"

enum
{
    COSTUME_TOP,
    COSTUME_BOTTOM,
    COSTUME_GLOVES,
    COSTUME_SHOES,
    COSTUME_ACCESSORY,
    COSTUME_MAX
};

struct X2mCostumeEntry
{
    uint8_t guid[16];
    std::vector<uint16_t> partsets;
    std::vector<uint8_t> races;
    std::vector<uint16_t> idb_entries;
    std::vector<uint8_t> costume_types;

    TiXmlElement *Decompile(TiXmlNode *root) const;
    bool Compile(const TiXmlElement *root);

    X2mCostumeEntry()
    {
        memset(guid, 0, sizeof(guid));
    }
};

class X2mCostumeFile : public BaseFile
{
private:

    std::vector<X2mCostumeEntry> costumes;

protected:

    void Reset();

public:

    X2mCostumeFile();
    virtual ~X2mCostumeFile();

    virtual TiXmlDocument *Decompile() const override;
    virtual bool Compile(TiXmlDocument *doc, bool big_endian=false) override;

    X2mCostumeEntry *FindCostume(const uint8_t *guid);
    X2mCostumeEntry *FindCostumeByPartSet(uint16_t partset);

    void AddCostume(const X2mCostumeEntry &entry);
    void RemoveCostume(const uint8_t *guid);
};

#endif // X2MCOSTUMEFILE_H
