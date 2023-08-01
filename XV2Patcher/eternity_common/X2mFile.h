#ifndef __X2MFILE_H__
#define __X2MFILE_H__

#include "ZipFile.h"
#include "Xenoverse2.h"
#include "BacFile.h"
#include "BsaFile.h"
#include "X2mCostumeFile.h"

#define X2M_SEL_PORTRAIT    "UI/SEL.DDS"
#define X2M_BTL_PORTRAIT    "UI/BTL.EMB"
#define X2M_JUNGLE          "JUNGLE/"
#define X2M_CSS_AUDIO       "CSS_AUDIO/"

#define X2M_SKILL           "SKILL"
#define X2M_SKILL_INI       "X2M_SKILL.ini"
#define X2M_SKILL_PREFIX    "NNNN_CCC_SSSS"

#define X2M_CHARA_INI   "X2M_CHARA.ini"

#define X2M_CHARA_SKILLS_ATTACHMENTS    "SKILL_ATACHMENT/"
#define X2M_SKILLS_COSTUME_ATTACHMENTS  "COSTUME_ATACHMENT/"

#define X2M_INVALID_ID  0xCACACACA
#define X2M_DUMMY_ID   0xBACABACA

#define X2M_INVALID_ID16    0xCACA
#define X2M_DUMMY_ID16      0xBACA

#define X2M_SKILL_DEPENDS_BEGIN 0xC000
#define X2M_SKILL_DEPENDS_END   0xD000

#define X2M_COSTUME_DEPENDS_ID   0xC000

enum class X2mType
{
    REPLACER,
    NEW_CHARACTER,
    NEW_SKILL,
    NEW_COSTUME
};

enum class X2mSkillType
{
    SUPER,
    ULTIMATE,
    EVASIVE,
    BLAST,
    AWAKEN
};

enum class X2mDependsType
{
    SKILL,
    COSTUME
};

enum
{
    X2M_CR_HUM_SYM,
    X2M_CR_HUF_SYF,
    X2M_CR_NMC,
    X2M_CR_FRI,
    X2M_CR_MAM,
    X2M_CR_MAF,
    X2M_CR_NUM,
};

enum class X2mItemType
{
    TOP,
    BOTTOM,
    GLOVES,
    SHOES,
    ACCESSORY
};

extern const std::vector<std::string> x2m_cr_code;

struct X2mSlotEntry
{
    int32_t costume_index;
    int32_t model_preset;
    bool flag_gk2;
    int32_t voices_id_list[2];
    //
    std::vector<std::string> costume_name;

    // New field 0.5
    std::string audio_files[2];

    X2mSlotEntry()
    {
        costume_index = 0;
        model_preset = 0;
        flag_gk2 = false;
        voices_id_list[0] = voices_id_list[1] = -1;

        costume_name.resize(XV2_LANG_NUM);
        audio_files[0].clear();
        audio_files[1].clear();
    }    

    void CopyFrom(const CharaListSlotEntry &entry, bool name);
    void CopyTo(CharaListSlotEntry &entry, const std::string &code) const;

    TiXmlElement *Decompile(TiXmlNode *root, bool new_format) const;
    bool Compile(const TiXmlElement *root, bool new_format);
};

struct SevEntryHL
{
    uint32_t costume_id;
    std::string copy_char;
    uint32_t copy_costume;

    SevEntryHL()
    {
        costume_id = copy_costume = 0xFFFFFFFF;        
    }

    TiXmlElement *Decompile(TiXmlNode *root) const;
    bool Compile(const TiXmlElement *root);
};

struct SevEventsLL
{
    bool im_source;

    std::string char_code; // always the char code of the other char
    uint32_t costume_id; // Always the costume id of the other char
    std::vector<SevEvent> events;

    TiXmlElement *Decompile(TiXmlNode *root) const;
    bool Compile(const TiXmlElement *root);
};

struct SevEntryLL
{
    uint32_t costume_id; // Always our costume id

    std::vector<SevEventsLL> sub_entries;

    SevEntryLL(bool default_entries=false)
    {
        costume_id = 0xFFFFFFFF;

        if (!default_entries)
            return;

        sub_entries.resize(1);

        SevEventsLL &sub_entry = sub_entries.front();
        sub_entry.im_source = true;
        sub_entry.costume_id = 0;
        sub_entry.events.resize(1);

        SevEvent &event = sub_entry.events.front();
        event.entries.resize(1);
    }

    TiXmlElement *Decompile(TiXmlNode *root) const;
    bool Compile(const TiXmlElement *root);
};

struct SevTableEntryHL
{
    std::string char_id;
    uint32_t costume_id;

    std::string copy_char;
    uint32_t copy_costume;
};

struct SevTableEntryLL
{
    std::string char_id; // Always our char_id
    uint32_t costume_id; // Always our costume id

    std::vector<SevEventsLL> sub_entries;
};

struct X2mCharaName
{
    uint32_t costume_index;
    std::vector<std::string> chara_name;

    X2mCharaName()
    {
        chara_name.resize(XV2_LANG_NUM);
        costume_index = 0;
    }

    TiXmlElement *Decompile(TiXmlNode *root) const;
    bool Compile(const TiXmlElement *root);
};

struct X2mSkillTransName
{
    std::vector<std::string> trans_name;

    X2mSkillTransName()
    {
        trans_name.resize(XV2_LANG_NUM);
    }

    TiXmlElement *Decompile(TiXmlNode *root) const;
    bool Compile(const TiXmlElement *root);
};

struct X2mDepends
{
    uint32_t id;
    std::string name;
    uint8_t guid[16];
    X2mDependsType type;

    TiXmlElement *Decompile(TiXmlNode *root) const;
    bool Compile(const TiXmlElement *root);
};

struct X2mSkillAura
{
    CusAuraData data;
    AurAura aura;

    TiXmlElement *Decompile(TiXmlNode *root) const;
    bool Compile(const TiXmlElement *root);
};

struct X2mItem
{
    std::vector<std::string> item_name;
    std::vector<std::string> item_desc;

    X2mItemType item_type;
    IdbEntry idb;

    X2mItem()
    {
        item_name.resize(XV2_LANG_NUM);
        item_desc.resize(XV2_LANG_NUM);
        item_type = X2mItemType::TOP;
        idb.id = idb.name_id = idb.desc_id = X2M_INVALID_ID16;
    }

    TiXmlElement *Decompile(TiXmlNode *root) const;
    bool Compile(const TiXmlElement *root);
};

struct X2mPartSet
{
    uint8_t race;
    BcsPartSet bcs;

    TiXmlElement *Decompile(TiXmlNode *root) const;
    bool Compile(const TiXmlElement *root);
};

class X2mFile : public ZipFile
{
private:

    // Require for all types
    X2mType type;
    float format_version;

    std::string mod_name;
    std::string mod_author;
    float mod_version;
    uint8_t mod_guid[16];
    uint8_t udata[64];

    // For new character
    std::string entry_name;
    std::vector<std::string> chara_name;
    std::vector<X2mCharaName> mult_chara_names;

    bool is_ozaru;
    int32_t body_shape;

    std::vector<X2mSlotEntry> slot_entries;
    CmsEntryXV2 cms_entry;
    std::vector<CusSkillSet> skill_sets;
    std::vector<CsoEntry> cso_entries;
    std::vector<PscSpecEntry> psc_entries;
    std::vector<AurCharaLink> aur_entries;
    std::vector<SevEntryHL> sev_hl_entries;
    std::vector<SevEntryLL> sev_ll_entries;
    std::vector<CmlEntry> cml_entries;
    std::vector<HciEntry> hci_entries;

    std::vector<X2mDepends> chara_skill_depends;
    std::vector<AurAura> chara_auras;

    // For skill
    std::vector<std::string> skill_name;
    std::vector<std::string> skill_desc;
    X2mSkillType skill_type;
    std::vector<X2mSkillTransName> skill_trans_names;
    CusSkill skill_entry;
    IdbEntry skill_idb_entry;
    std::vector<PupEntry> skill_pup_entries;
    std::vector<X2mSkillAura> skill_aura_entries;
    X2mDepends skill_costume_depend;

    // For costumes
    std::vector<X2mItem> costume_items;
    std::vector<X2mPartSet> costume_partsets;

    // ///////////////////////////////
    // temp variables, do not copy to DUMMY x2d
    std::vector<std::string> installed_css_audio;
    std::vector<uint32_t> installed_css_cue;
    std::vector<int32_t> temp_pup_in, temp_pup_out;
    std::vector<int32_t> temp_cus_aur_in, temp_cus_aur_out;
    std::vector<int32_t> temp_aur_in, temp_aur_out;
    X2mCostumeEntry update_costume_entry;

    // ///////////////////////////////
    bool Validate();

    bool Decompile();
    bool Compile();

    bool FillCsoEntries();
    bool FillPscEntries();
    bool FillAurEntries();
    bool FillCmlEntries();

    bool InstallDir(const std::string &in_path, const std::string &out_path);
    bool UninstallDir(const std::string &in_path, const std::string &out_path);

    void RebuildSevHL(const std::vector<SevTableEntryHL> &hl_table);
    void RebuildSevLL(const std::vector<SevTableEntryLL> &ll_table);

    void UninstallCssAudio();
    bool InstallCssAudio(X2mSlotEntry &entry);

    static size_t FindX2mSkillCMS(std::vector<CmsEntryXV2 *> &entries);
    uint16_t IdFromId2(uint16_t id2) const;
    static uint16_t IdFromId2(uint16_t id2, X2mSkillType skill_type);
    bool AssignSkillIds();

    std::string GetSkillDirectory() const;    
    uint16_t GetCusSkillType() const;

    void AssignSkillIdbIds();    
    void AssignCharaCustomSkills(CusSkillSet &x_set);

    bool InstallCustomAuraSkill();
    bool InstallCustomAuraChara();

    void GetInstalledCustomAuraChara();

    bool AddCostumeIdb(IdbFile *cost_idb, IdbEntry &entry, uint16_t id_start);

    static bool InstallSkillVisitor(const std::string &path, void *param);
    static bool InstallCostumeVisitor(const std::string &path, void *param);
    static bool UninstallCostumeVisitor(const std::string &path, void *param);

protected:

    void Reset();

public:

    const float X2M_CURRENT_VERSION = 7.13f;

    const float X2M_MIN_VERSION_CSO = 2.0f;
    const float X2M_MIN_VERSION_PSC = 3.0f;
    const float X2M_MIN_VERSION_AUR = 3.0f;
    const float X2M_MIN_VERSION_SEV = 4.0f;
    const float X2M_MIN_VERSION_CML = 4.0f;
    const float X2M_MIN_VERSION_HCI = 4.0f;
    const float X2M_MIN_VERSION_OZARU = 4.0f;
    const float X2M_MIN_VERSION_CSS_AUDIO = 5.0f;
    const float X2M_MIN_VERSION_MULTIPLE_CHAR_NAMES = 5.0f;
    const float X2M_MIN_VERSION_BODY_SHAPES = 5.1f;
    const float X2M_MIN_VERSION_AUTO_BTL_PORTRAITS = 5.5f;
    const float X2M_MIN_VERSION_SKILLS = 6.0f;
    const float X2M_MIN_VERSION_AURAS = 6.5f;
    const float X2M_MIN_VERSION_UDATA = 6.51f;
    const float X2M_MIN_VERSION_COSTUME = 7.0f;

    X2mFile();
    virtual ~X2mFile();

    virtual bool Load(const uint8_t *buf, size_t size) override;
    virtual uint8_t *Save(size_t *psize) override;

    inline X2mType GetType() const { return type; }
    inline void SetType(X2mType type) { this->type = type; }

    inline float GetFormatVersion() const { return format_version; }
    inline void SetFormatVersion(float format_version){ this->format_version = format_version; }

    inline std::string GetModName() const { return mod_name; }
    inline void SetModName(const std::string &mod_name) { this->mod_name = mod_name; }

    inline std::string GetModAuthor() const { return mod_author; }
    inline void SetModAuthor(const std::string &mod_author) { this->mod_author = mod_author; }

    inline float GetModVersion() const { return mod_version; }
    inline void SetModVersion(float mod_version) { this->mod_version = mod_version; }

    inline std::string GetModGuid() const { return Utils::GUID2String(mod_guid); }
    inline bool SetModGuid(const std::string &mod_guid)
    {
        return Utils::String2GUID(this->mod_guid, mod_guid);
    }

    inline void GetModUdata(uint8_t *buf) const
    {
        memcpy(buf, udata, sizeof(udata));
    }

    inline void SetModUdata(const uint8_t *buf)
    {
        memcpy(udata, buf, sizeof(udata));
    }

    inline std::string GetEntryName() const { return entry_name; }
    inline void SetEntryName(const std::string &entry_name) { this->entry_name = Utils::ToUpperCase(entry_name); }

    inline std::string GetCharaName(int lang) const
    {
        if (lang >= XV2_LANG_NUM)
            return std::string();

        return chara_name[lang];
    }

    inline bool SetCharaName(const std::string &name, int lang)
    {
        if (lang >= XV2_LANG_NUM)
            return false;

        chara_name[lang] = name;
        return true;
    }

    inline size_t GetNumMultNames() const { return mult_chara_names.size(); }
    inline const X2mCharaName &GetMultName(size_t idx) const { return mult_chara_names[idx]; }
    inline X2mCharaName &GetMultName(size_t idx) { return mult_chara_names[idx]; }

    inline size_t AddMultName(const X2mCharaName &entry) { mult_chara_names.push_back(entry); return (mult_chara_names.size()-1); }
    inline void RemoveMultName(size_t idx) { mult_chara_names.erase(mult_chara_names.begin()+idx); }

    inline bool HasMultNames() const { return (format_version >= X2M_MIN_VERSION_MULTIPLE_CHAR_NAMES && GetNumMultNames() > 0); }

    inline bool IsOzaru() const { return is_ozaru; }
    inline void SetOzaru(bool ozaru_flag) { is_ozaru = ozaru_flag; }

    inline bool UsesBodyShape() const { return (format_version >= X2M_MIN_VERSION_BODY_SHAPES && body_shape >= 0); }
    inline int32_t GetBodyShape() const { return body_shape; }
    inline void SetBodyShape(int32_t body_shape) { this->body_shape = body_shape; }

    bool CharaDirectoryExists() const;
    void DeleteCharaDirectory();

    inline bool SelPortraitExists() const { return FileExists(X2M_SEL_PORTRAIT); }
    inline bool BtlPortraitExists() const { return FileExists(X2M_BTL_PORTRAIT); }

    inline bool JungleExists() const { return DirExists(X2M_JUNGLE); }
    void DeleteJungle() { DeleteDir(X2M_JUNGLE); }

    inline size_t GetNumSlotEntries() const { return slot_entries.size(); }
    inline const X2mSlotEntry &GetSlotEntry(size_t idx) const { return slot_entries[idx]; }
    inline X2mSlotEntry &GetSlotEntry(size_t idx) { return slot_entries[idx]; }

    size_t GetNumCostumes() const;
    bool CostumeExists(int32_t costume_id) const;

    inline size_t AddSlotEntry(const X2mSlotEntry &entry) { slot_entries.push_back(entry); return (slot_entries.size()-1); }
    inline void RemoveSlotEntry(size_t idx) { slot_entries.erase(slot_entries.begin()+idx); }

    inline bool HasValidCmsEntry() const { return (cms_entry.id != X2M_INVALID_ID); }
    inline const CmsEntryXV2 &GetCmsEntry() const { return cms_entry; }
    inline CmsEntryXV2 &GetCmsEntry() { return cms_entry; }

    inline size_t GetNumSkillSets()  { return skill_sets.size(); }
    inline const CusSkillSet &GetSkillSet(size_t idx) const { return skill_sets[idx]; }
    inline CusSkillSet &GetSkillSet(size_t idx) { return skill_sets[idx]; }

    inline const std::vector<CusSkillSet> &GetAllSkillSets() const { return skill_sets; }
    inline std::vector<CusSkillSet> &GetAllSkillSets() { return skill_sets; }

    inline size_t AddSkillSet(const CusSkillSet &entry) { skill_sets.push_back(entry); return (skill_sets.size()-1); }
    inline void RemoveSkillSet(size_t idx) { skill_sets.erase(skill_sets.begin()+idx); }

    X2mFile *CreateDummyPackage() const;
    bool HasDangerousJungle() const;

    inline size_t GetNumCsoEntries() const { return cso_entries.size(); }
    inline const CsoEntry &GetCsoEntry(size_t idx) const { return cso_entries[idx]; }
    inline CsoEntry &GetCsoEntry(size_t idx) { return cso_entries[idx]; }

    inline size_t AddCsoEntry(const CsoEntry &entry) { cso_entries.push_back(entry); return (cso_entries.size()-1); }
    inline void RemoveCsoEntry(size_t idx) { cso_entries.erase(cso_entries.begin()+idx); }

    inline bool HasCso() const { return (format_version >= X2M_MIN_VERSION_CSO && GetNumCsoEntries() > 0); }

    inline size_t GetNumPscEntries() const { return psc_entries.size(); }
    inline const PscSpecEntry &GetPscEntry(size_t idx) const { return psc_entries[idx]; }
    inline PscSpecEntry &GetPscEntry(size_t idx) { return psc_entries[idx]; }

    inline size_t AddPscEntry(const PscSpecEntry &entry) { psc_entries.push_back(entry); return (psc_entries.size()-1); }
    inline void RemovePscEntry(size_t idx) { psc_entries.erase(psc_entries.begin()+idx); }

    inline bool HasPsc() const { return (format_version >= X2M_MIN_VERSION_PSC && GetNumPscEntries() > 0); }

    inline size_t GetNumAurEntries() const { return aur_entries.size(); }
    inline const AurCharaLink &GetAurEntry(size_t idx) const { return aur_entries[idx]; }
    inline AurCharaLink &GetAurEntry(size_t idx) { return aur_entries[idx]; }

    inline size_t AddAurEntry(const AurCharaLink &entry) { aur_entries.push_back(entry); return (aur_entries.size()-1); }
    inline void RemoveAurEntry(size_t idx) { aur_entries.erase(aur_entries.begin()+idx); }

    inline bool HasAur() const { return (format_version >= X2M_MIN_VERSION_AUR && GetNumAurEntries() > 0); }

    inline size_t GetNumSevHLEntries() const { return sev_hl_entries.size(); }
    inline const SevEntryHL &GetSevHLEntry(size_t idx) const { return sev_hl_entries[idx]; }
    inline SevEntryHL &GetSevHLEntry(size_t idx) { return sev_hl_entries[idx]; }

    inline size_t AddSevHLEntry(const SevEntryHL &entry) { sev_hl_entries.push_back(entry); return (sev_hl_entries.size()-1); }
    inline void RemoveSevHLEntry(size_t idx) { sev_hl_entries.erase(sev_hl_entries.begin()+idx); }
    inline void RemoveAllSevHL() { sev_hl_entries.clear(); }

    inline bool HasSevHL() const { return (format_version >= X2M_MIN_VERSION_SEV && GetNumSevHLEntries() > 0); }

    inline size_t GetNumSevLLEntries() const { return sev_ll_entries.size(); }
    inline const SevEntryLL &GetSevLLEntry(size_t idx) const { return sev_ll_entries[idx]; }
    inline SevEntryLL &GetSevLLEntry(size_t idx) { return sev_ll_entries[idx]; }

    inline size_t AddSevLLEntry(const SevEntryLL &entry) { sev_ll_entries.push_back(entry); return (sev_ll_entries.size()-1); }
    inline void RemoveSevLLEntry(size_t idx) { sev_ll_entries.erase(sev_ll_entries.begin()+idx); }
    inline void RemoveAllSevLL() { sev_ll_entries.clear(); }

    inline bool HasSevLL() const { return (format_version >= X2M_MIN_VERSION_SEV && GetNumSevLLEntries() > 0); }

    inline bool HasSev() const { return (HasSevHL() || HasSevLL()); }

    inline size_t GetNumCmlEntries() const { return cml_entries.size(); }
    inline const CmlEntry &GetCmlEntry(size_t idx) const { return cml_entries[idx]; }
    inline CmlEntry &GetCmlEntry(size_t idx) { return cml_entries[idx]; }

    inline size_t AddCmlEntry(const CmlEntry &entry) { cml_entries.push_back(entry); return (cml_entries.size()-1); }
    inline void RemoveCmlEntry(size_t idx) { cml_entries.erase(cml_entries.begin()+idx); }

    inline bool HasCml() const { return (format_version >= X2M_MIN_VERSION_CML && GetNumCmlEntries() > 0); }

    inline size_t GetNumHciEntries() const { return hci_entries.size(); }
    inline const HciEntry &GetHciEntry(size_t idx) const { return hci_entries[idx]; }
    inline HciEntry &GetHciEntry(size_t idx) { return hci_entries[idx]; }

    inline size_t AddHciEntry(const HciEntry &entry) { hci_entries.push_back(entry); return (hci_entries.size()-1); }
    inline void RemoveHciEntry(size_t idx) { hci_entries.erase(hci_entries.begin()+idx); }

    inline bool HasHci() const { return (format_version >= X2M_MIN_VERSION_HCI && GetNumHciEntries() > 0); }

    bool CssAudioFileExists(const std::string &name, bool english) const;
    bool CssAudioFileExists(const std::string &name) const;
    HcaFile *LoadCssAudioFile(const std::string &name, bool english, bool fallback);
    size_t GetCssAudioFiles(std::vector<std::string> &files);
    size_t SetCssAudioDir(const std::string &dir_path);
    void DeleteCssAudio();

    inline size_t GetNumCharaSkillDepends() const { return chara_skill_depends.size(); }
    inline bool HasCharaSkillDepends() const { return (format_version >= X2M_MIN_VERSION_SKILLS && chara_skill_depends.size() > 0); }
    inline const X2mDepends &GetCharaSkillDepends(size_t idx) const { return chara_skill_depends[idx]; }
    inline X2mDepends &GetCharaSkillDepends(size_t idx) { return chara_skill_depends[idx]; }
    inline const std::vector<X2mDepends> &GetAllCharaSkillDepends() const { return chara_skill_depends; }
    inline std::vector<X2mDepends> &GetAllCharaSkillDepends() { return chara_skill_depends; }

    bool CharaSkillDependsExist(const uint8_t *guid) const;
    bool CharaSkillDependsExist(const std::string &guid) const;
    bool CharaSkillDependsExist(uint16_t id) const;

    X2mDepends *FindCharaSkillDepends(const uint8_t *guid);
    X2mDepends *FindCharaSkillDepends(const std::string &guid);
    X2mDepends *FindCharaSkillDepends(uint16_t id);

    bool AddCharaSkillDepends(const X2mDepends &depends);
    bool AddCharaSkillDepends(X2mFile *skill_x2m, bool update);
    X2mDepends *AddCharaSkillDepends(const uint8_t *guid, const std::string &name);

    inline void RemoveCharaSkillDepends(size_t idx) { chara_skill_depends.erase(chara_skill_depends.begin()+idx); }
    bool RemoveCharaSkillDepends(const uint8_t *guid);
    bool RemoveCharaSkillDepends(const std::string &guid);

    bool CharaSkillDependsHasAttachment(size_t idx) const;
    bool CharaSkillDependsHasAttachment(const uint8_t *guid) const;
    bool CharaSkillDependsHasAttachment(const std::string &guid) const;

    bool SetCharaSkillDependsAttachment(size_t idx, X2mFile *attachment);
    bool SetCharaSkillDependsAttachment(X2mFile *attachment);

    bool RemoveCharaSkillDependsAttachment(const uint8_t *guid);
    bool RemoveCharaSkillDependsAttachment(const std::string &guid);

    X2mFile *LoadCharaSkillDependsAttachment(const uint8_t *guid);
    X2mFile *LoadCharaSkillDependsAttachment(const std::string &guid);

    bool IsCharaSkillDependsReferenced(const X2mDepends &depends) const;

    size_t GetNumCharaAuras(bool install_mode) const;
    inline const AurAura &GetCharaAura(size_t idx) const { return chara_auras[idx]; }
    inline AurAura &GetCharaAura(size_t idx) { return chara_auras[idx]; }

    inline size_t AddCharaAura(const AurAura &aura) { chara_auras.push_back(aura); return (chara_auras.size()-1); }
    inline void RemoveCharaAura(size_t idx) { chara_auras.erase(chara_auras.begin()+idx); }

    inline bool HasCharaAura(bool install_mode) const { return (GetNumCharaAuras(install_mode) > 0); }

    // Skill
    inline std::string GetSkillName(int lang) const
    {
        if (lang >= XV2_LANG_NUM)
            return std::string();

        return skill_name[lang];
    }

    inline bool SetSkillName(const std::string &name, int lang)
    {
        if (lang >= XV2_LANG_NUM)
            return false;

        skill_name[lang] = name;
        return true;
    }

    inline std::string GetSkillDesc(int lang) const
    {
        if (lang >= XV2_LANG_NUM)
            return std::string();

        return skill_desc[lang];
    }

    inline bool SetSkillDesc(const std::string &name, int lang)
    {
        if (lang >= XV2_LANG_NUM)
            return false;

        skill_desc[lang] = name;
        return true;
    }

    X2mSkillType GetSkillType() const { return skill_type; }
    void SetSkillType(X2mSkillType type) { skill_type = type; }

    inline bool SkillDirectoryExists() const { return DirExists(X2M_SKILL); }
    inline void DeleteSkillDirectory() { DeleteDir(X2M_SKILL); }

    inline bool HasValidSkillEntry() const { return (skill_entry.id != X2M_INVALID_ID16); }
    inline const CusSkill &GetSkillEntry() const { return skill_entry; }
    inline CusSkill &GetSkillEntry() { return skill_entry; }

    inline bool HasSkillIdbEntry() const { return (skill_idb_entry.id != X2M_INVALID_ID16); }
    inline const IdbEntry &GetSkillIdbEntry() const { return skill_idb_entry; }
    inline IdbEntry &GetSkillIdbEntry() { return skill_idb_entry; }

    inline size_t GetNumSkillPupEntries() const { return skill_pup_entries.size(); }
    inline const PupEntry &GetSkillPupEntry(size_t idx) const { return skill_pup_entries[idx]; }
    inline PupEntry &GetSkillPupEntry(size_t idx) { return skill_pup_entries[idx]; }

    inline size_t AddSkillPupEntry(const PupEntry &entry) { skill_pup_entries.push_back(entry); return (skill_pup_entries.size()-1); }
    inline void RemoveSkillPupEntry(size_t idx) { skill_pup_entries.erase(skill_pup_entries.begin()+idx); }

    inline bool HasSkillPup() const { return (GetNumSkillPupEntries() > 0); }

    inline size_t GetNumSkillTransNames() const { return skill_trans_names.size(); }
    inline const X2mSkillTransName &GetSkillTransName(size_t idx) const { return skill_trans_names[idx]; }
    inline X2mSkillTransName &GetSkillTransName(size_t idx) { return skill_trans_names[idx]; }

    inline size_t AddSkillTransName(const X2mSkillTransName &entry) { skill_trans_names.push_back(entry); return (skill_trans_names.size()-1); }
    inline void RemoveSkillTransName(size_t idx) { skill_trans_names.erase(skill_trans_names.begin()+idx); }

    inline size_t GetNumSkillAuras() const { return skill_aura_entries.size(); }
    inline const X2mSkillAura &GetSkillAura(size_t idx) const { return skill_aura_entries[idx]; }
    inline X2mSkillAura &GetSkillAura(size_t idx) { return skill_aura_entries[idx]; }

    inline size_t AddSkillAura(const X2mSkillAura &aura) { skill_aura_entries.push_back(aura); return (skill_aura_entries.size()-1); }
    inline void RemoveSkillAura(size_t idx) { skill_aura_entries.erase(skill_aura_entries.begin()+idx); }

    inline bool HasSkillAura() const { return (format_version >= X2M_MIN_VERSION_AURAS && GetNumSkillAuras() > 0); }

    size_t GetNumSkillCustomAuras() const;
    inline bool HasSkillCustomAuras() const { return (GetNumSkillCustomAuras() > 0); }

    static CusSkill *FindInstalledSkill(const uint8_t *guid, X2mSkillType type);
    bool FindInstalledSkill();

    inline bool HasSkillCostumeDepend() const { return (format_version >= X2M_MIN_VERSION_COSTUME && skill_costume_depend.id != X2M_INVALID_ID); }
    inline const X2mDepends &GetSkillCostumeDepend() const { return skill_costume_depend; }
    inline X2mDepends &GetSkillCostumeDepend() { return skill_costume_depend; }

    bool IsSkillCostumeDepends(const uint8_t *guid) const;
    bool IsSkillCostumeDepends(const std::string &guid) const;
    bool IsSkillCostumeDepends(uint16_t id) const;

    bool SetSkillCostumeDepend(const X2mDepends &depends);
    bool SetSkillCostumeDepend(X2mFile *costume_x2m);
    X2mDepends *SetSkillCostumeDepend(const uint8_t *guid, const std::string &name);

    inline void RemoveSkillCostumeDepend() { skill_costume_depend.id = X2M_INVALID_ID; }

    bool SkillCostumeDependHasAttachment() const;
    bool SetSkillCostumeDependAttachment(X2mFile *attachment);
    bool RemoveSkillCostumeDependAttachment();

    X2mFile *LoadSkillCostumeDependAttachment();

    bool IsSkillCostumeDependReferenced() const;

    // Costume
    inline bool CostumeDirectoryExists(uint8_t race) const { return (race < X2M_CR_NUM && DirExists(x2m_cr_code[race])); }
    inline void DeleteCostumeDirectory(uint8_t race) { DeleteDir(x2m_cr_code[race]); }

    inline size_t GetNumTotalCostumePartSets() const { return costume_partsets.size(); }
    size_t GetNumCostumePartSets(uint8_t race) const;
    size_t GetEffectiveNumCostumePartSets() const;
    size_t GetNumCostumeUsedRaces() const;

    const X2mPartSet &GetCostumePartSet(uint8_t race, size_t idx) const;
    X2mPartSet &GetCostumePartSet(uint8_t race, size_t idx);

    inline void AddCostumePartSet(const X2mPartSet &set) { costume_partsets.push_back(set); }
    void ClearCostumePartSets(uint8_t race);
    void ClearAllCostumePartSets() { costume_partsets.clear(); }

    inline size_t GetNumCostumeItems() const { return costume_items.size(); }
    inline const X2mItem &GetCostumeItem(size_t idx) const { return costume_items[idx]; }
    inline X2mItem &GetCostumeItem(size_t idx) { return costume_items[idx]; }

    inline size_t AddCostumeItem(const X2mItem &entry) { costume_items.push_back(entry); return (costume_items.size()-1); }
    inline void RemoveCostumeItem(size_t idx) { costume_items.erase(costume_items.begin()+idx); }

    inline bool HasCostumeItem() const { return (GetNumCostumeItems() > 0); }

    X2mCostumeEntry *FindInstalledCostume();
    static X2mCostumeEntry *FindInstalledCostume(const uint8_t *guid);

    // Ram installs
    bool InstallCharaName();
    bool InstallCostumeNames();
    bool InstallSlots(bool update);
    bool InstallSlots(const std::vector<size_t> &positions);
    bool InstallCms(bool update);
    bool InstallCus(bool update);
    bool InstallCso(bool update);
    bool InstallPsc();
    bool InstallAur(bool update);    
    bool InstallSev(std::vector<SevTableEntryHL> &hl_table, std::vector<SevTableEntryLL> &ll_table);
    bool InstallCml(bool update);
    bool InstallHci();
    bool InstallSelPortrait();
    bool InstallPreBaked();

    // File installs
    bool InstallBtlPortrait(bool update);
    bool InstallCharaFiles();
    bool InstallJungle();

    // Ram uninstalls
    bool UninstallCharaName();
    bool UninstallCostumeNames();
    bool UninstallSlots();
    bool UninstallCms();
    bool UninstallCus();
    bool UninstallCso();
    bool UninstallPsc();
    bool UninstallAur();
    bool UninstallSev(std::vector<SevTableEntryHL> &hl_table, std::vector<SevTableEntryLL> &ll_table);
    bool UninstallCml();
    bool UninstallHci();
    bool UninstallSelPortrait();
    bool UninstallPreBaked();

    // File uninstalls
    bool UninstallBtlPortrait();
    bool UninstallCharaFiles(bool remove_empty);
    bool UninstallJungle();

    // Skill Ram installs
    bool InstallPupSkill();
    bool InstallAuraSkill();
    bool InstallCusSkill();
    bool InstallSkillName();
    bool InstallSkillDesc();
    bool InstallIdbSkill();

    // Skill file installs
    bool InstallSkillFiles();

    // Skill Ram uninstalls
    bool UninstallPupSkill();
    bool UninstallAuraSkill();
    bool UninstallCusSkill();
    bool UninstallSkillName();
    bool UninstallSkillDesc();
    bool UninstallIdbSkill();

    // Skill file uninstalls
    bool UninstallSkillFiles(bool remove_empty);

    // Costume ram installs
    bool InstallCostumePartSets();
    bool InstallCostumeCostumeNames();
    bool InstallCostumeAccessoryNames();
    bool InstallCostumeCostumeDescs();
    bool InstallCostumeAccessoryDescs();
    bool InstallCostumeIdb();
    bool InstallCostumeFile();

    // Costume file installs
    bool InstallCostumeFiles();

    // Costume ram uninstalls
    bool UninstallCostumePartSets();
    bool UninstallCostumeCostumeNames();
    bool UninstallCostumeAccessoryNames();
    bool UninstallCostumeCostumeDescs();
    bool UninstallCostumeAccessoryDescs();
    bool UninstallCostumeIdb();
    bool UninstallCostumeFile();

    // Costume file uninstalls
    bool UninstallCostumeFiles();

    // Static members
    static std::string GetSkillDirectory(const CusSkill &skill_entry, X2mSkillType skill_type);
};

#endif // __X2MFILE_H__
