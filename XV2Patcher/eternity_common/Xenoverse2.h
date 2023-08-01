#ifndef __XENOVERSE2_H__
#define __XENOVERSE2_H__

#include "Xv2Fs.h"
#include "CharaListFile.h"
#include "IggyFile.h"
#include "MsgFile.h"
#include "CmsFile.h"
#include "CusFile.h"
#include "CsoFile.h"
#include "EmbFile.h"
#include "PscFile.h"
#include "AurFile.h"
#include "SevFile.h"
#include "CmlFile.h"
#include "HciFile.h"
#include "Xv2PreBakedFile.h"
#include "IdbFile.h"
#include "PalFile.h"
#include "Xv2SavFile.h"
#include "Afs2File.h"
#include "AcbFile.h"
#include "HcaFile.h"
#include "PsaFile.h"
#include "PupFile.h"
#include "BcsFile.h"

// Order is not necessarily the one the game uses (if it uses any)
enum
{
    XV2_LANG_ENGLISH,
    XV2_LANG_SPANISH1,
    XV2_LANG_SPANISH2,
    XV2_LANG_FRENCH,
    XV2_LANG_GERMAN,
    XV2_LANG_ITALIAN,
    XV2_LANG_PORTUGUESE,
    XV2_LANG_POLISH,
    XV2_LANG_RUSSIAN,
    XV2_LANG_CHINESE1,
    XV2_LANG_CHINESE2,
    XV2_LANG_KOREAN,

    XV2_LANG_NUM
};

#include "X2mCostumeFile.h"

extern const std::vector<std::string> xv2_lang_codes;
extern CharaListFile *chara_list;
extern IggyFile *charasele;

extern CmsFile *game_cms;
extern CusFile *game_cus;
extern CsoFile *game_cso;
extern PscFile *game_psc;
extern AurFile *game_aur;
extern SevFile *game_sev;
extern CmlFile *game_cml;
extern HciFile *game_hci;
extern PsaFile *game_psa;
extern PupFile *game_pup;

extern std::vector<MsgFile *> game_chara_names;
extern std::vector<MsgFile *> game_chara_costume_names;

extern std::vector<MsgFile *> sup_sk_names;
extern std::vector<MsgFile *> ult_sk_names;
extern std::vector<MsgFile *> eva_sk_names;
extern std::vector<MsgFile *> awa_sk_names;

extern std::vector<MsgFile *> sup_sk_descs;
extern std::vector<MsgFile *> ult_sk_descs;
extern std::vector<MsgFile *> eva_sk_descs;
extern std::vector<MsgFile *> awa_sk_descs;

extern std::vector<MsgFile *> quest_btlhud_texts;

extern EmbFile *game_sel_port;
extern Xv2PreBakedFile *game_prebaked;

extern std::vector<MsgFile *> game_lobby_texts;
extern std::vector<MsgFile *> game_cac_costume_names;
extern std::vector<MsgFile *> game_accesory_names;
extern std::vector<MsgFile *> game_talisman_names;

extern std::vector<MsgFile *> game_cac_costume_descs;
extern std::vector<MsgFile *> game_accesory_descs;

extern IdbFile *game_bottom_idb;
extern IdbFile *game_gloves_idb;
extern IdbFile *game_shoes_idb;
extern IdbFile *game_top_idb;
extern IdbFile *game_accesory_idb;
extern IdbFile *game_talisman_idb;
extern IdbFile *game_skill_idb;

extern PalFile *game_pal;

extern AcbFile *game_css_acb_jp;
extern AcbFile *game_css_acb_en;
extern Afs2File *game_css_awb_jp;
extern Afs2File *game_css_awb_en;

extern BcsFile *game_hum_bcs;
extern BcsFile *game_huf_bcs;
extern BcsFile *game_nmc_bcs;
extern BcsFile *game_fri_bcs;
extern BcsFile *game_mam_bcs;
extern BcsFile *game_maf_bcs;

extern X2mCostumeFile *game_costume_file;

namespace Xenoverse2
{
    void InitFs(const std::string &game_path);
    bool InitCharaList(const std::string &chasel_path, const std::string &list_path);
    bool InitSystemFiles(bool only_cms=false);
    bool InitCharaNames(int only_this_lang=-1);
    bool InitCharaCostumeNames(int only_this_lang=-1);
    bool InitSkillNames(int only_this_lang=-1);
    bool InitSkillDescs(int only_this_lang=-1);
    bool InitBtlHudText(int only_this_lang=-1);
    bool InitSelPort();
    bool InitPreBaked();
    bool InitLobbyText(int only_this_lang=-1);
    bool InitCacCostumeNames(int only_this_lang=-1);
    bool InitCacCostumeDescs(int only_this_lang=-1);
    bool InitTalismanNames(int only_this_lang=-1);
    bool InitIdb(bool costumes=true, bool accesories=true, bool talisman=true, bool skills=true);
    bool InitLobby();
    bool InitSound();
    bool InitCac();
    bool InitCostumeFile();

    bool CommitCharaList();
    bool CommitSystemFiles(bool pup);
    bool CommitSelPort();
    bool CommitCharaNames();
    bool CommitCharaCostumeNames();
    bool CommitSkillNames();
    bool CommitSkillDescs();
    bool CommitBtlHudText();
    bool CommitPreBaked();
    bool CommitLobbyText();
    bool CommitCacCostumeNames();
    bool CommitCacCostumeDescs();
    bool CommitLobby();
    bool CommitIdb(bool costumes, bool accesories, bool talisman, bool skills);
    bool CommitSound();
    bool CommitCac();
    bool CommitCostumeFile();

    bool GetCharaCodeFromId(uint32_t id, std::string &code);

    bool IsOriginalChara(const std::string &short_name);
    bool CompileCharaSel(const std::string &compiler);

    bool GetCharaName(const std::string &code, std::string &name, int lang=XV2_LANG_ENGLISH, int index=0);
    bool GetCharaCostumeName(const std::string &code, int var, int model_preset, std::string &name, int lang=XV2_LANG_ENGLISH);

    bool SetCharaName(const std::string &code, const std::string &name, int lang, int index=0);
    bool SetCharaCostumeName(const std::string &code, int var, int model_preset, const std::string &name, int lang);

    bool RemoveCharaName(const std::string &code, int lang, int index=0);
    bool RemoveAllCharaName(const std::string &code, int lang);
    bool RemoveCharaCostumeName(const std::string &code, int var, int model_preset, int lang);

    bool GetSuperSkillName(uint16_t name_id, std::string &name, int lang=XV2_LANG_ENGLISH);
    bool GetUltimateSkillName(uint16_t name_id, std::string &name, int lang=XV2_LANG_ENGLISH);
    bool GetEvasiveSkillName(uint16_t name_id, std::string &name, int lang=XV2_LANG_ENGLISH);
    bool GetAwakenSkillName(uint16_t name_id, std::string &name, int lang=XV2_LANG_ENGLISH);

    bool SetSuperSkillName(uint16_t name_id, const std::string &name, int lang);
    bool SetUltimateSkillName(uint16_t name_id, const std::string &name, int lang);
    bool SetEvasiveSkillName(uint16_t name_id, const std::string &name, int lang);
    bool SetAwakenSkillName(uint16_t name_id, const std::string &name, int lang);

    bool RemoveSuperSkillName(uint16_t name_id, int lang);
    bool RemoveUltimateSkillName(uint16_t name_id, int lang);
    bool RemoveEvasiveSkillName(uint16_t name_id, int lang);
    bool RemoveAwakenSkillName(uint16_t name_id, int lang);

    bool GetSuperSkillDesc(uint16_t name_id, std::string &desc, int lang=XV2_LANG_ENGLISH);
    bool GetUltimateSkillDesc(uint16_t name_id, std::string &desc, int lang=XV2_LANG_ENGLISH);
    bool GetEvasiveSkillDesc(uint16_t name_id, std::string &desc, int lang=XV2_LANG_ENGLISH);
    bool GetAwakenSkillDesc(uint16_t name_id, std::string &desc, int lang=XV2_LANG_ENGLISH);

    bool SetSuperSkillDesc(uint16_t name_id, const std::string &desc, int lang);
    bool SetUltimateSkillDesc(uint16_t name_id, const std::string &desc, int lang);
    bool SetEvasiveSkillDesc(uint16_t name_id, const std::string &desc, int lang);
    bool SetAwakenSkillDesc(uint16_t name_id, const std::string &desc, int lang);

    bool RemoveSuperSkillDesc(uint16_t name_id, int lang);
    bool RemoveUltimateSkillDesc(uint16_t name_id, int lang);
    bool RemoveEvasiveSkillDesc(uint16_t name_id, int lang);
    bool RemoveAwakenSkillDesc(uint16_t name_id, int lang);

    bool GetBtlHudAwakenName(uint16_t name_id, uint16_t trans_stage, std::string &name, int lang=XV2_LANG_ENGLISH);
    bool SetBtlHudAwakenName(uint16_t name_id, uint16_t trans_stage, const std::string &name, int lang);
    bool RemoveBtlHudAwakenName(uint16_t name_id, uint16_t trans_stage, int lang);

    uint8_t *GetSelPortrait(const std::string &name, size_t *psize);
    bool SetSelPortrait(const std::string &name, const uint8_t *buf, size_t size);
    bool RemoveSelPortrait(const std::string &name);

    bool GetLobbyName(uint32_t name_id, std::string &name, int lang=XV2_LANG_ENGLISH);
    bool SetLobbyName(uint32_t name_id, const std::string &name, int lang=XV2_LANG_ENGLISH);
    bool RemoveLobbyName(uint32_t name_id, int lang);

    bool GetCacCostumeName(uint32_t name_idx, std::string &name, int lang=XV2_LANG_ENGLISH);
    bool SetCacCostumeName(uint32_t name_idx, const std::string &name, int lang);
    bool AddCacCostumeName(const std::string &entry_name, const std::string &name, int lang, uint16_t *ret_idx);
    bool RemoveCacCostumeName(uint32_t name_idx, int lang, bool update_idb);

    bool GetAccesoryName(uint32_t name_idx, std::string &name, int lang=XV2_LANG_ENGLISH);
    bool SetAccesoryName(uint32_t desc_idx, const std::string &name, int lang);
    bool AddAccesoryName(const std::string &name, int lang, uint16_t *ret_idx);
    bool RemoveAccesoryName(uint32_t name_idx, int lang, bool update_idb);

    bool GetCacCostumeDesc(uint32_t desc_idx, std::string &desc, int lang=XV2_LANG_ENGLISH);
    bool SetCacCostumeDesc(uint32_t desc_idx, const std::string &desc, int lang);
    bool AddCacCostumeDesc(const std::string &entry_name, const std::string &desc, int lang, uint16_t *ret_idx);
    bool RemoveCacCostumeDesc(uint32_t desc_idx, int lang, bool update_idb);

    bool GetAccesoryDesc(uint32_t desc_idx, std::string &desc, int lang=XV2_LANG_ENGLISH);
    bool SetAccesoryDesc(uint32_t desc_idx, const std::string &desc, int lang);
    bool AddAccesoryDesc(const std::string &desc, int lang, uint16_t *ret_idx);
    bool RemoveAccesoryDesc(uint32_t desc_idx, int lang, bool update_idb);

    bool GetTalismanName(uint32_t name_idx, std::string &name, int lang=XV2_LANG_ENGLISH);
    uint32_t GetBlastFromTalisman(uint32_t talisman_id, bool id2);

    HcaFile *GetCssSound(uint32_t cue_id, bool english);
    HcaFile *GetCssSound(const std::string &name, bool english);
    bool SetCssSound(uint32_t cue_id, bool english, HcaFile &hca);
    uint32_t SetCssSound(const std::string &name, bool english, HcaFile &hca);
    bool FreeCssSound(const std::string &name, bool english);

    uint32_t CusAuraToAurAura(uint32_t id);

    // Higher level api
    std::string GetCharaAndCostumeName(const std::string &code, uint32_t costume_id, uint32_t model_preset, int lang=XV2_LANG_ENGLISH);
    std::string GetCharaAndCostumeName(uint32_t char_id, uint32_t costume_id, uint32_t model_preset, bool ignore_unknown, int lang=XV2_LANG_ENGLISH);

    std::string GetCacTopName(uint16_t item_id, int lang=XV2_LANG_ENGLISH);
    std::string GetCacBottomName(uint16_t item_id, int lang=XV2_LANG_ENGLISH);
    std::string GetCacGlovesName(uint16_t item_id, int lang=XV2_LANG_ENGLISH);
    std::string GetCacShoesName(uint16_t item_id, int lang=XV2_LANG_ENGLISH);
    std::string GetCacAccesoryName(uint16_t item_id, int lang=XV2_LANG_ENGLISH);
    std::string GetTalismanNameEx(uint16_t item_id, int lang=XV2_LANG_ENGLISH);

    std::string GetSkillDirectory(const CusSkill &skill, std::string *last_part=nullptr);

    std::vector<std::string> GetCostumeNamesForPartSet(uint32_t partset, uint8_t race_lock=0xFF);
    std::string GetCostumeNamesForPartSet2(uint32_t partset, uint8_t race_lock=0xFF);
}

extern Xv2Fs *xv2fs;

#endif // __XENOVERSE2_H__
