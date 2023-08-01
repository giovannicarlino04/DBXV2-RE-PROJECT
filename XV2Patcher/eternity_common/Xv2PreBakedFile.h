#ifndef XV2PREBAKEDFILE_H
#define XV2PREBAKEDFILE_H

#include "BaseFile.h"

struct BodyShape
{
    uint32_t cms_entry;
    uint32_t body_shape;

    TiXmlElement *Decompile(TiXmlNode *root) const;
    bool Compile(const TiXmlElement *root);
};

struct CusAuraData
{
    uint16_t cus_aura_id;
    uint16_t aur_aura_id;

    uint8_t behaviour_11;
    uint32_t integer_2;
    uint8_t behaviour_10;
    uint32_t integer_3;

    bool force_teleport;
    uint8_t behaviour_13;

    CusAuraData()
    {
        cus_aura_id = 0xFFFF;
        aur_aura_id = 0;
        behaviour_11 = 0;
        integer_2 = 0;
        behaviour_10 = 0;
        integer_3 = 0;
        force_teleport = false;
        behaviour_13 = 0;
    }

    TiXmlElement *Decompile(TiXmlNode *root) const;
    bool Compile(const TiXmlElement *root);
};

class Xv2PreBakedFile : public BaseFile
{
private:

    std::vector<std::string> ozarus;
    std::vector<uint32_t> auto_btl_portrait_list;
    std::vector<BodyShape> body_shapes;
    std::vector<CusAuraData> cus_aura_datas;

protected:

    void Reset();

public:

    Xv2PreBakedFile();
    virtual ~Xv2PreBakedFile();

    virtual TiXmlDocument *Decompile() const override;
    virtual bool Compile(TiXmlDocument *doc, bool big_endian=false) override;

    inline const std::vector<std::string> &GetOzarus() const { return ozarus; }
    inline std::vector<std::string> &GetOzarus() { return ozarus; }

    inline const std::vector<uint32_t> &GetAutoBtlPortraitList() const { return auto_btl_portrait_list; }
    inline std::vector<uint32_t> &GetAutoBtlPortraitList() { return auto_btl_portrait_list; }

    inline const std::vector<BodyShape> &GetBodyShapes() const { return body_shapes; }
    inline std::vector<BodyShape> &GetBodyShapes() { return body_shapes; }

    inline const std::vector<CusAuraData> &GetCusAuraDatas() const { return cus_aura_datas; }
    inline std::vector<CusAuraData> &GetCusAuraDatas() { return cus_aura_datas; }

    void AddOzaru(const std::string &ozaru);
    void RemoveOzaru(const std::string &ozaru);

    void AddAutoBtlPortrait(uint32_t cms_entry);
    void RemoveAutoBtlPortrait(uint32_t cms_entry);

    void AddBodyShape(uint32_t cms_entry, uint32_t body_shape);
    void RemoveBodyShape(uint32_t cms_entry);

    CusAuraData *FindAuraData(uint16_t cus_aura_id);
    CusAuraData *FindAuraDataByAurId(uint16_t aur_aura_id);

    bool AddConsecutiveAuraData(std::vector<CusAuraData> &datas);
    void RemoveAuraData(uint16_t cus_aura_id);

};

#endif // XV2PREBAKEDFILE_H
