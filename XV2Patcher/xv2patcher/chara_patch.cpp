#include <windows.h>
#include <math.h>
#include <cmath>
#include <algorithm>
#include "PatchUtils.h"
#include "PscFile.h"
#include "IggyFile.h"
#include "Xv2PreBakedFile.h"
#include "xv2patcher.h"
#include "debug.h"
#include <unordered_set>

typedef void (* SendToAS3Type)(void *, int32_t, uint64_t);
typedef void (*Func1Type)(void *, uint32_t, uint32_t, void *, void *);
typedef void (*Func2Type)(void *, uint32_t, uint32_t, void *);
typedef int (* CheckUnlockType)(void *, int32_t, int32_t);
typedef uint64_t (* SetBodyShapeType)(void *, int32_t, uint32_t, float);

/*typedef uint64_t (* ResultPortraitsType)(uint64_t arg_rcx, uint64_t arg_rdx, uint32_t is_cac, uint32_t cms_entry, 
									uint64_t arg_20, uint64_t arg_28, uint64_t arg_30, uint64_t arg_38, uint64_t arg_40, uint64_t arg_48, uint64_t arg_50, uint64_t arg_58, uint64_t arg_60, uint64_t arg_68,
									uint64_t arg_70, uint64_t arg_78, uint64_t arg_80, uint64_t arg_88, uint64_t arg_90, uint64_t arg_98, uint64_t arg_A0, uint64_t arg_A8, uint64_t arg_B0, uint64_t arg_B8,
									uint64_t arg_C0, uint64_t arg_C8, uint64_t arg_D0, uint64_t arg_D8, uint64_t arg_E0, uint64_t arg_E8, uint64_t arg_F0, uint64_t arg_F8, uint64_t arg_100, uint64_t arg_108,
									uint64_t arg_110, uint64_t arg_118, uint64_t arg_120, uint64_t arg_128, uint64_t arg_130, uint64_t arg_138, uint64_t arg_140, uint64_t arg_148, uint64_t arg_150, uint64_t arg_158,
									uint64_t arg_160, uint64_t arg_168, uint64_t arg_170, uint64_t arg_178, uint64_t arg_180, uint64_t arg_188, uint64_t arg_190, uint64_t arg_198, uint64_t arg_1A0, uint64_t arg_1A8);*/
									
//typedef uint64_t (* ResultPortraitsType)(uint64_t arg_rcx, uint64_t arg_rdx, uint32_t is_cac, uint32_t cms_entry);
typedef uint64_t (* ResultPortraits2Type)(void *obj, int idx, void *r8, void *r9, void *arg5, void *arg6, void *arg7, void *arg8);
typedef uint32_t (* Behaviour10FuncType)(void *, uint32_t);


// These constants are the ones in CharaSele.as
// I have kept their names, even if in native code, receives are actually sends

static constexpr const int PlayerNumFri = 2;
static constexpr const int SkillMax = 8;
static constexpr const int CharaVarIndexNum = 32;
static constexpr const int CharacterMax = 99;
static constexpr const int CustomListMax = 256;
static constexpr const int ReceiveType_FlagUseCancel = 0;
static constexpr const int ReceiveType_PlayerFriNum = 1;
static constexpr const int ReceiveType_PlayerEnmNum = 2;
static constexpr const int ReceiveType_PartyNpcNum = 3;
static constexpr const int ReceiveType_FlagSelectAvatar = 4;
static constexpr const int ReceiveType_FlagLocalBattle = 5;
static constexpr const int ReceiveType_Flag2pController = 6;
static constexpr const int ReceiveType_Str2pController = 7;
static constexpr const int ReceiveType_Time = 8;
static constexpr const int ReceiveType_CharaNameStr = 9;
static constexpr const int ReceiveType_NameOption_GK2 = 10;
static constexpr const int ReceiveType_VariationNameStr = 11;
static constexpr const int ReceiveType_TarismanHeaderStr = 12;
static constexpr const int ReceiveType_TarismanNameStr = 13;
static constexpr const int ReceiveType_ImageStrStart = 14;
static constexpr const int ReceiveType_ImageStrEnd = ReceiveType_ImageStrStart + CharacterMax - 1; 
static constexpr const int ReceiveType_UnlockVarStart = ReceiveType_ImageStrEnd + 1;
static constexpr const int ReceiveType_UnlockVarEnd = ReceiveType_UnlockVarStart + CharaVarIndexNum * CharacterMax - 1;
static constexpr const int ReceiveType_KeyStrL1 = ReceiveType_UnlockVarEnd + 1;
static constexpr const int ReceiveType_KeyStrR1 = ReceiveType_KeyStrL1 + 1;
static constexpr const int ReceiveType_KeyStrL2 = ReceiveType_KeyStrR1 + 1;
static constexpr const int ReceiveType_KeyStrR2 = ReceiveType_KeyStrL2 + 1;
static constexpr const int ReceiveType_KeyStrRU = ReceiveType_KeyStrR2 + 1;
static constexpr const int ReceiveType_KeyStrRD = ReceiveType_KeyStrRU + 1;
static constexpr const int ReceiveType_KeyStrRL = ReceiveType_KeyStrRD + 1;
static constexpr const int ReceiveType_KeyStrRR = ReceiveType_KeyStrRL + 1;
static constexpr const int ReceiveType_SkillNameStrStart = ReceiveType_KeyStrRR + 1;
static constexpr const int ReceiveType_SkillNameStrEnd = ReceiveType_SkillNameStrStart + SkillMax - 1;
static constexpr const int ReceiveType_ImageStrNpcStart = ReceiveType_SkillNameStrEnd + 1;
static constexpr const int ReceiveType_ImageStrNpcEnd = ReceiveType_ImageStrNpcStart + PlayerNumFri - 1;
static constexpr const int ReceiveType_CharaSelectedStart = ReceiveType_ImageStrNpcEnd + 1;
static constexpr const int ReceiveType_CharaSelectedEnd = ReceiveType_CharaSelectedStart + CharacterMax - 1;
static constexpr const int ReceiveType_CharaVariationStart = ReceiveType_CharaSelectedEnd + 1;
static constexpr const int ReceiveType_CharaVariationEnd = ReceiveType_CharaVariationStart + CharaVarIndexNum - 1;
static constexpr const int ReceiveType_WaitLoadNum = ReceiveType_CharaVariationEnd;
static constexpr const int ReceiveType_UseCustomList = ReceiveType_CharaVariationEnd + 1;
static constexpr const int ReceiveType_CustomListNum = ReceiveType_UseCustomList + 1; 
static constexpr const int ReceiveType_CustomList_CID_Start = ReceiveType_CustomListNum + 1; 
static constexpr const int ReceiveType_CustomList_CID_End = ReceiveType_CustomList_CID_Start + CustomListMax - 1;
static constexpr const int ReceiveType_CustomList_MID_Start = ReceiveType_CustomList_CID_End + 1;
static constexpr const int ReceiveType_CustomList_MID_End = ReceiveType_CustomList_MID_Start + CustomListMax - 1;
static constexpr const int ReceiveType_CustomList_PID_Start = ReceiveType_CustomList_MID_End + 1;
static constexpr const int ReceiveType_CustomList_PID_End = ReceiveType_CustomList_PID_Start + CustomListMax - 1;
static constexpr const int ReceiveType_Num = ReceiveType_CustomList_PID_End + 1;

extern "C" 
{
	
static int n_CharacterMax;
static int n_ReceiveType_ImageStrEnd; 
static int n_ReceiveType_UnlockVarStart;
static int n_ReceiveType_UnlockVarEnd;
static int n_ReceiveType_KeyStrL1;
static int n_ReceiveType_KeyStrR1;
static int n_ReceiveType_KeyStrL2;
static int n_ReceiveType_KeyStrR2;
static int n_ReceiveType_KeyStrRU;
static int n_ReceiveType_KeyStrRD;
static int n_ReceiveType_KeyStrRL;
static int n_ReceiveType_KeyStrRR;
static int n_ReceiveType_SkillNameStrStart;
static int n_ReceiveType_SkillNameStrEnd;
static int n_ReceiveType_ImageStrNpcStart;
static int n_ReceiveType_ImageStrNpcEnd;
static int n_ReceiveType_CharaSelectedStart;
static int n_ReceiveType_CharaSelectedEnd;
static int n_ReceiveType_CharaVariationStart;
static int n_ReceiveType_CharaVariationEnd;
static int n_ReceiveType_WaitLoadNum;
static int n_ReceiveType_UseCustomList;
static int n_ReceiveType_CustomListNum;
static int n_ReceiveType_CustomList_CID_Start;
static int n_ReceiveType_CustomList_CID_End;
static int n_ReceiveType_CustomList_MID_Start;
static int n_ReceiveType_CustomList_MID_End;
static int n_ReceiveType_CustomList_PID_Start;
static int n_ReceiveType_CustomList_PID_End;
static int n_ReceiveType_Num;

static SendToAS3Type SendToAS3;
static SendToAS3Type SendToAS3_2;
static SendToAS3Type SendToAS3_3;
static Func1Type func1;
static Func2Type func2;
static CheckUnlockType check_unlock;
static SetBodyShapeType SetBodyShape;
static ResultPortraits2Type ResultPortraits2;
static Behaviour10FuncType Behaviour10Func;

static uint8_t *find_blob_string(uint8_t *blob, size_t size, const char *str)
{
	size_t len = strlen(str);
	size_t max_search = size - len;
	
	for (size_t i = 0; i <= max_search; i++)
	{
		if (memcmp(blob+i, str, strlen(str)) == 0)
		{
			return blob+i;
		}
	}
	
	return nullptr;
}

static uint32_t guess_character_max()
{
	const std::string iggy_path = myself_path + CONTENT_ROOT + "data/ui/iggy/CHARASELE.iggy";
	IggyFile iggy;
	
	if (!iggy.LoadFromFile(iggy_path, false))
	{
		DPRINTF("Auto character max assumes %d because there is no custom iggy file. (assuming too the one in cpk is untouched).\n", CharacterMax);
		return CharacterMax;
	}
	
	uint32_t size;
	uint8_t *abc_blob = iggy.GetAbcBlob(&size);
	
	if (!abc_blob)
	{
		DPRINTF("Auto character max assumes %d because it was not able to extract actionscript from iggy.\n", CharacterMax);
		return CharacterMax;
	}
	
	uint8_t *ptr = abc_blob;
	for (uint32_t i = 0; i < 0x40; i++, ptr++)
	{
		uint64_t double_val = *(uint64_t *)ptr;
		
		// Read next double
		double ImageStrEnd_double = *(double *)(ptr+8);
		
		if (!std::isfinite(ImageStrEnd_double) || ImageStrEnd_double < 23.0 || ImageStrEnd_double > 100013.0)
		{
			DPRINTF("Auto detecting character max failed. We'll asume the default of %d.\n", CharacterMax);
			return CharacterMax;
		}
		
		uint32_t ret = lrint(ImageStrEnd_double) - 13;
		DPRINTF("Auto character max has been estimated: %d\n", ret);
		return ret;
	}
	
	DPRINTF("Auto detecting character max failed (2). We'll asume the default of %d.\n", CharacterMax);
	return CharacterMax;
}

PUBLIC void CharaSetup(SendToAS3Type orig)
{
	SendToAS3 = orig;
	
	ini.GetIntegerValue("NewChara", "CharacterMax", &n_CharacterMax, CharacterMax);
	
	if (n_CharacterMax == 0)
	{
		n_CharacterMax = guess_character_max();
	}
	
	n_ReceiveType_ImageStrEnd = ReceiveType_ImageStrStart + n_CharacterMax - 1; 
	n_ReceiveType_UnlockVarStart = n_ReceiveType_ImageStrEnd + 1;
	n_ReceiveType_UnlockVarEnd = n_ReceiveType_UnlockVarStart + CharaVarIndexNum * n_CharacterMax - 1;
	n_ReceiveType_KeyStrL1 = n_ReceiveType_UnlockVarEnd + 1;
	n_ReceiveType_KeyStrR1 = n_ReceiveType_KeyStrL1 + 1;
	n_ReceiveType_KeyStrL2 = n_ReceiveType_KeyStrR1 + 1;
	n_ReceiveType_KeyStrR2 = n_ReceiveType_KeyStrL2 + 1;
	n_ReceiveType_KeyStrRU = n_ReceiveType_KeyStrR2 + 1;
	n_ReceiveType_KeyStrRD = n_ReceiveType_KeyStrRU + 1;
	n_ReceiveType_KeyStrRL = n_ReceiveType_KeyStrRD + 1;
	n_ReceiveType_KeyStrRR = n_ReceiveType_KeyStrRL + 1;
	n_ReceiveType_SkillNameStrStart = n_ReceiveType_KeyStrRR + 1;
	n_ReceiveType_SkillNameStrEnd = n_ReceiveType_SkillNameStrStart + SkillMax - 1;
	n_ReceiveType_ImageStrNpcStart = n_ReceiveType_SkillNameStrEnd + 1;
	n_ReceiveType_ImageStrNpcEnd = n_ReceiveType_ImageStrNpcStart + PlayerNumFri - 1;
	n_ReceiveType_CharaSelectedStart = n_ReceiveType_ImageStrNpcEnd + 1;
	n_ReceiveType_CharaSelectedEnd = n_ReceiveType_CharaSelectedStart + n_CharacterMax - 1;
	n_ReceiveType_CharaVariationStart = n_ReceiveType_CharaSelectedEnd + 1;
	n_ReceiveType_CharaVariationEnd = n_ReceiveType_CharaVariationStart + CharaVarIndexNum - 1;
	n_ReceiveType_WaitLoadNum = n_ReceiveType_CharaVariationEnd;
	n_ReceiveType_UseCustomList = n_ReceiveType_CharaVariationEnd + 1; 
	n_ReceiveType_CustomListNum = n_ReceiveType_UseCustomList + 1; 
	n_ReceiveType_CustomList_CID_Start = n_ReceiveType_CustomListNum + 1; 
	n_ReceiveType_CustomList_CID_End = n_ReceiveType_CustomList_CID_Start + CustomListMax - 1; 
	n_ReceiveType_CustomList_MID_Start = n_ReceiveType_CustomList_CID_End + 1; 
	n_ReceiveType_CustomList_MID_End = n_ReceiveType_CustomList_MID_Start + CustomListMax - 1; 
	n_ReceiveType_CustomList_PID_Start = n_ReceiveType_CustomList_MID_End + 1; 
	n_ReceiveType_CustomList_PID_End = n_ReceiveType_CustomList_PID_Start + CustomListMax - 1; 
	n_ReceiveType_Num = n_ReceiveType_CustomList_PID_End + 1;
}

PUBLIC void AS3Send2Setup(SendToAS3Type orig)
{
	SendToAS3_2 = orig;
}

PUBLIC void AS3Send3Setup(SendToAS3Type orig)
{
	SendToAS3_3 = orig;
}

PUBLIC void PatchReceiveTypeKey(void *pthis, int32_t code, uint64_t data)
{
	if (code < ReceiveType_KeyStrL1 || code > ReceiveType_KeyStrRR)
	{
		DPRINTF("ERROR: unexpected code at PatchReceiveTypeKey. Code=0x%x\n", code);
	}
	else
	{	
		code = (code - ReceiveType_KeyStrL1) + n_ReceiveType_KeyStrL1;
	}
	
	SendToAS3(pthis, code, data);
}

PUBLIC void PatchReceiveTypeCharaSelected(void *pthis, int32_t code, uint64_t data)
{
	if (code < ReceiveType_CharaSelectedStart || code >= (ReceiveType_CharaSelectedStart+n_CharacterMax))
	{
		DPRINTF("ERROR: unexpected code at PatchReceiveTypeCharaSelected. Code=0x%x\n", code);
	}
	else
	{
		code = (code - ReceiveType_CharaSelectedStart) + n_ReceiveType_CharaSelectedStart;
	}
	
	SendToAS3_2(pthis, code, data);
}

PUBLIC void PatchReceiveTypeVariation(void *pthis, int32_t code, uint64_t data)
{
	if (code < ReceiveType_CharaVariationStart || code > ReceiveType_CharaVariationEnd)
	{
		DPRINTF("ERROR: unexpected code at PatchReceiveTypeVariation. Code=0x%x\n", code);
	}
	else
	{
		code = (code - ReceiveType_CharaVariationStart) + n_ReceiveType_CharaVariationStart;
	}
	
	SendToAS3_2(pthis, code, data);
}

PUBLIC void PatchReceiveTypeSkillName(void *pthis, int32_t code, uint64_t data)
{
	if (code < ReceiveType_SkillNameStrStart || code > ReceiveType_SkillNameStrEnd)
	{
		DPRINTF("ERROR: unexpected code at PatchReceiveTypeSkillName. Code=0x%x\n", code);
	}
	else
	{
		code = (code - ReceiveType_SkillNameStrStart) + n_ReceiveType_SkillNameStrStart;
	}
	
	SendToAS3(pthis, code, data);
}

PUBLIC void PatchReceiveTypeImageStr(void *pthis, int32_t code, uint64_t data)
{
	// This function doesn't really need patch, but we are gonna check limits anyway
	if (code < ReceiveType_ImageStrStart || code >= (ReceiveType_ImageStrStart+n_CharacterMax))
	{
		DPRINTF("ERROR: unexpected code at PatchReceiveTypeImageStr. Code=0x%x\n", code);
	}
	
	SendToAS3(pthis, code, data);
}

PUBLIC void PatchReceiveTypeImageStrNpc(void *pthis, int32_t code, uint64_t data)
{
	if (code < ReceiveType_ImageStrNpcStart || code > ReceiveType_ImageStrNpcEnd)
	{
		DPRINTF("ERROR: unexpected code at PatchReceiveTypeImageStrNpc. Code=0x%x\n", code);
	}
	else
	{
		code = (code - ReceiveType_ImageStrNpcStart) + n_ReceiveType_ImageStrNpcStart;
	}
	
	SendToAS3(pthis, code, data);
}

PUBLIC void PatchReceiveTypeUnlockVar(void *pthis, int32_t code, uint64_t data)
{
	if (code < ReceiveType_UnlockVarStart || code >= (ReceiveType_UnlockVarStart + CharaVarIndexNum * n_CharacterMax))
	{
		DPRINTF("ERROR: unexpected code at PatchReceiveTypeUnlockVar. Code=0x%x\n", code);
	}
	else
	{
		code = (code - ReceiveType_UnlockVarStart) + n_ReceiveType_UnlockVarStart;
	}
	
	SendToAS3_2(pthis, code, data);
}

PUBLIC void PatchReceiveTypeUseCustomList(void *pthis, int32_t code, uint64_t data)
{
	if (code != ReceiveType_UseCustomList)
	{
		DPRINTF("ERROR: unexpected code at PatchReceiveTypeUseCustomList. Code=0x%x\n", code);
	}
	else
	{
		code = (code - ReceiveType_UseCustomList) + n_ReceiveType_UseCustomList;
	}
	
	SendToAS3_2(pthis, code, data);
}

PUBLIC void PatchReceiveTypeCustomListNum(void *pthis, int32_t code, uint64_t data)
{
	if (code != ReceiveType_CustomListNum)
	{
		DPRINTF("ERROR: unexpected code at PatchReceiveTypeCustomListNum. Code=0x%x\n", code);
	}
	else
	{
		code = (code - ReceiveType_CustomListNum) + n_ReceiveType_CustomListNum;
	}
	
	SendToAS3_2(pthis, code, data);
}

PUBLIC void PatchReceiveType_CustomList_CID_Start(void *pthis, int32_t code, uint64_t data)
{
	if (code < ReceiveType_CustomList_CID_Start || code >  ReceiveType_CustomList_CID_End)
	{
		DPRINTF("ERROR: unexpected code at PatchReceiveType_CustomList_CID_Start. Code=0x%x\n", code);
	}
	else
	{
		code = (code - ReceiveType_CustomList_CID_Start) + n_ReceiveType_CustomList_CID_Start;
	}
	
	SendToAS3_3(pthis, code, data);
}

PUBLIC void PatchReceiveType_CustomList_MID_Start(void *pthis, int32_t code, uint64_t data)
{
	if (code < ReceiveType_CustomList_MID_Start || code >  ReceiveType_CustomList_MID_End)
	{
		DPRINTF("ERROR: unexpected code at PatchReceiveTypeCustomList_MID_Start. Code=0x%x\n", code);
	}
	else
	{
		code = (code - ReceiveType_CustomList_MID_Start) + n_ReceiveType_CustomList_MID_Start;
	}
	
	SendToAS3_2(pthis, code, data);
}

PUBLIC void PatchReceiveType_CustomList_PID_Start(void *pthis, int32_t code, uint64_t data)
{
	if (code < ReceiveType_CustomList_PID_Start || code >  ReceiveType_CustomList_PID_End)
	{
		DPRINTF("ERROR: unexpected code at PatchReceiveTypeCustomList_PID_Start. Code=0x%x\n", code);
	}
	else
	{
		code = (code - ReceiveType_CustomList_PID_Start) + n_ReceiveType_CustomList_PID_Start;
	}
	
	SendToAS3_2(pthis, code, data);
}

PUBLIC void PatchReceiveTypeNum(uint32_t *num)
{
	uint32_t old = *num;	
	PatchUtils::Write32(num, n_ReceiveType_Num);
	
	DPRINTF("ReceiveTypeNum sucesfully changed from %d to %d\n", old, *num);
}

PUBLIC void IncreaseChaselMemory(uint32_t *psize)
{
	uint32_t old = *psize;
	PatchUtils::Write32(psize, Utils::Align2(0x1240+(n_CharacterMax*32), 0x100));
	
	DPRINTF("Chasel object memory succesfully changed from 0x%x to 0x%x\n", old, *psize);
}

PUBLIC void SetupIncreaseChaselSlotsArray(Func1Type orig)
{
	func1 = orig;
}

PUBLIC void IncreaseChaselSlotsArray(void *arg1, uint32_t arg2, uint32_t size, void *arg4, void *arg5)
{
	size = n_CharacterMax;	
	func1(arg1, arg2, size, arg4, arg5);
}

PUBLIC void SetupIncreaseChaselSlotsArray2(Func2Type orig)
{
	func2 = orig;
}

PUBLIC void IncreaseChaselSlotsArray2(void *arg1, uint32_t arg2, uint32_t size, void *arg4)
{
	size = n_CharacterMax;
	func2(arg1, arg2, size, arg4);	
}

static inline bool is_original_playable_char(int32_t chara)
{
	// Mira final form (0.41 fix)
	if (chara == 0x7D)
		return false;
	
	// between goku and  pan
	if (chara >= 0 && chara <= 0x2A)
		return true;
	
	// between san-shinron and satan
	if (chara >= 0x30 && chara <= 0x34)
		return true;
	
	// between buu small and raspberry
	if (chara >= 0x3A && chara <= 0x3C)
		return true;
	
	// between mira and towa
	if (chara >= 0x3E && chara <= 0x3F)
		return true;
	
	// whis or jaco or cell first form
	if (chara == 0x41 || chara == 0x43 || chara == 0x4B)
		return true;
	
	// between Goku SSGSS and Janemba
	if (chara >= 0x50 && chara <= 0x58)
		return true;
	
	// Races
	if (chara >= 0x64 && chara <= 0x6C)
		return true;
	
	// Gogeta - Hit
	if (chara >= 0x78 && chara <= 0x7E)
		return true;
	
	// Janemba or Goku black or future trunks
	if (chara >= 0x82 && chara <= 0x84)
		return true;
	
	return false;
}

PUBLIC void SetupCheckUnlockFunction(CheckUnlockType orig)
{
	check_unlock = orig;
}

PUBLIC int UnlockCharaMods(void *pthis, int32_t chara, int32_t var)
{
	if (is_original_playable_char(chara))
		return check_unlock(pthis, chara, var);
	
	//DPRINTF("Not original, returning 1 for 0x%x\n", chara);
	
	return 1;
}

// This function is an imitation of the DIMPS one, but without the 16 bits calculation overflow.
PUBLIC void PscInitReplacement(uint8_t *psc_buf)
{
	if (*psc_buf == 0x21) // Already processed
		return;
		
	if (memcmp(psc_buf+1, "PSC", 3) != 0)
		return;
	
	PSCHeader *hdr = (PSCHeader *)psc_buf;
	
	if (hdr->endianess_check != 0xFFFE)
		return;
	
	PSCEntry *file_entries = (PSCEntry *)(psc_buf+hdr->header_size);
    PSCSpecEntry *file_specs = (PSCSpecEntry *)(file_entries+hdr->num_entries);
	
	for (uint32_t i = 0; i < hdr->num_entries; i++)
	{
		file_entries[i].unk_08 = Utils::DifPointer(file_specs, &file_entries[i]);
		file_specs += file_entries[i].num_specs;
	}
	
	*psc_buf = 0x21; // Set as processed
}

// XV2Patcher extensions

#define LOOKUP_SIZE	0x1000

static std::vector<std::string> ozarus = { "OSV", "OSB", "OBB", "OSG", "OSN" }; // The original list, as it is in game (including non-existing OSG)
static uint8_t body_shapes_lookup[LOOKUP_SIZE];
static uint8_t auto_btl_portrait_lookup[LOOKUP_SIZE]; // lookup table, we need O(1) access

uint16_t cus_aura_lookup[LOOKUP_SIZE]; // This one must be accesible by chara_patch_asm.S
uint8_t cus_aura_bh11_lookup[LOOKUP_SIZE]; // This one must be accesible by chara_patch_asm.S
static uint32_t cus_aura_int2_lookup[LOOKUP_SIZE];
static uint8_t cus_aura_bh10_lookup[LOOKUP_SIZE];
static uint32_t cus_aura_int3_lookup[LOOKUP_SIZE];
uint8_t force_teleport[LOOKUP_SIZE]; // This one must be accesible by chara_patch_asm.S
static uint8_t cus_aura_bh13_lookup[LOOKUP_SIZE];

extern void GetRealAura();
extern void GetCABehaviour11();
extern void ForceTeleport();

PUBLIC void PreBakeSetup(size_t)
{
	// Set up lookup tables first
	memset(body_shapes_lookup, 0xFF, sizeof(body_shapes_lookup));	
	memset(auto_btl_portrait_lookup, 0, sizeof(auto_btl_portrait_lookup));	
	memset(cus_aura_lookup, 0xFF, sizeof(cus_aura_lookup));
	memset(cus_aura_bh11_lookup, 0xFF, sizeof(cus_aura_bh11_lookup));
	memset(cus_aura_int2_lookup, 0, sizeof(cus_aura_int2_lookup));
	memset(cus_aura_bh10_lookup, 0xFF, sizeof(cus_aura_bh10_lookup));
	memset(cus_aura_int3_lookup, 0, sizeof(cus_aura_int3_lookup));
	memset(force_teleport, 0, sizeof(force_teleport));
	memset(cus_aura_bh13_lookup, 0xFF, sizeof(cus_aura_bh13_lookup));
		
	// Original aura mapping
	cus_aura_lookup[0] = 5;
	cus_aura_lookup[1] = 6;
	cus_aura_lookup[2] = 7;
	cus_aura_lookup[3] = cus_aura_lookup[10] = 0xF;
	cus_aura_lookup[4] = cus_aura_lookup[5] = cus_aura_lookup[6] = 0xD;
	cus_aura_lookup[7] = 0xE;
	cus_aura_lookup[8] = 0x12;
	cus_aura_lookup[9] = 0x13;
	cus_aura_lookup[11] = 0x18;
	cus_aura_lookup[12] = 0x19;
	cus_aura_lookup[13] = 0x14;
	cus_aura_lookup[15] = cus_aura_lookup[17] = cus_aura_lookup[18] = 0x16;
	cus_aura_lookup[19] = 0x1E;
	cus_aura_lookup[16] = 0x17;
	cus_aura_lookup[20] = cus_aura_lookup[21] = 0x21;
	// Original behaviour_11 values: nothing (the 0xFF init will ensure that)
	// Original int 2 values (only non zero)
	cus_aura_int2_lookup[1] = cus_aura_int2_lookup[5] = cus_aura_int2_lookup[21] = cus_aura_int2_lookup[23] = 1; // 23 is outside 0-21 range, it may be for a future update
	cus_aura_int2_lookup[2] = cus_aura_int2_lookup[3] = cus_aura_int2_lookup[6] = 2;
	// Original behaviour_10 values: nothing (the 0xFF init will ensure that)
	// Original int 3 values (only non zero)
	cus_aura_int3_lookup[0] = 1;
	for (int i = 11; i <= 19; i++)
		cus_aura_int3_lookup[i] = 1;
	
	cus_aura_int3_lookup[1] = cus_aura_int3_lookup[7] = cus_aura_int3_lookup[8] = cus_aura_int3_lookup[9] = 2;
	cus_aura_int3_lookup[2] = cus_aura_int3_lookup[3] = 3;
	// Default force_teleport are all 0
	// Original behaviour_13 values: nothing (the 0xFF init will ensure that)
	
	Xv2PreBakedFile pbk;
	const std::string pbk_path = myself_path + CONTENT_ROOT + "data/pre-baked.xml";
	
	if (!pbk.CompileFromFile(pbk_path, false))
		return;
	
	std::vector<std::string> &additional_ozarus = pbk.GetOzarus();	
	for (const std::string &ozaru : additional_ozarus)
		ozarus.push_back(ozaru);
	
	const std::vector<BodyShape> &body_shapes = pbk.GetBodyShapes();	
	for (const BodyShape &shape : body_shapes)
	{
		if (shape.cms_entry < LOOKUP_SIZE)
			body_shapes_lookup[shape.cms_entry] = (uint8_t)shape.body_shape;
	}
	
	
	const std::vector<uint32_t> &auto_btl_portraits_list = pbk.GetAutoBtlPortraitList();		
	for (uint32_t cms_entry : auto_btl_portraits_list)
	{
		if (cms_entry < LOOKUP_SIZE)
			auto_btl_portrait_lookup[cms_entry] = 1;
	}
	
	const std::vector<CusAuraData> &cus_aura_datas = pbk.GetCusAuraDatas();
	for (const CusAuraData &data : cus_aura_datas)
	{
		if (data.cus_aura_id < LOOKUP_SIZE)
		{
			cus_aura_lookup[data.cus_aura_id] = data.aur_aura_id;
			cus_aura_bh11_lookup[data.cus_aura_id] = data.behaviour_11;
			cus_aura_int2_lookup[data.cus_aura_id] = data.integer_2;
			cus_aura_bh10_lookup[data.cus_aura_id] = data.behaviour_10;
			cus_aura_int3_lookup[data.cus_aura_id] = data.integer_3;
			force_teleport[data.cus_aura_id] = data.force_teleport;
			cus_aura_bh13_lookup[data.cus_aura_id] = data.behaviour_13;
		}
	}
}

PUBLIC int IsOzaruReplacement(const char *char_code)
{
	for (const std::string &ozaru : ozarus)
	{
		if (strncmp(ozaru.c_str(), char_code, 3) == 0)
			return 1;
	}
		
	return 0;	
}

PUBLIC void SetBodyShape_Setup(SetBodyShapeType orig)
{
	SetBodyShape = orig;
}

PUBLIC uint64_t SetBodyShape_Patched(void *object, int32_t body_shape, int32_t arg3, float arg4)
{
	if (body_shape < 0 && object)
	{
		uint64_t *object64 = (uint64_t *)object;
		uint32_t *object2 = (uint32_t *)object64[0x30/8];
		
		if (object2)
		{
			uint32_t cms_entry = object2[0x90/4];
			
			if (cms_entry < LOOKUP_SIZE && body_shapes_lookup[cms_entry] != 0xFF)
				body_shape = body_shapes_lookup[cms_entry];
		}
	}
	
	return SetBodyShape(object, body_shape, arg3, arg4);
}

PUBLIC int UseAutobattlePortrait(int32_t cms_entry)
{
	if (cms_entry >= 0 && cms_entry < LOOKUP_SIZE && auto_btl_portrait_lookup[cms_entry])
		return 1;
	
	// Otherwise, do the same as game implementation	
	if (cms_entry >= 0x64 && cms_entry <= 0x77)
		return 1;
	
	return 0;
}

PUBLIC void ResultPortraits2Setup(ResultPortraits2Type orig)
{
	ResultPortraits2 = orig;
}

PUBLIC uint64_t ResultPortraits2Patched(uint8_t *obj, int idx, void *r8, void *r9, void *arg5, void *arg6, void *arg7, void *arg8)
{
	uint32_t *undo = nullptr;
	
	if (idx >= 0 && idx < 0xE)
	{
		uint32_t *obj2 = (uint32_t *)(obj + idx*0x180);
		uint32_t cms_entry = obj2[0x51C/4];
		
		if (cms_entry < LOOKUP_SIZE && auto_btl_portrait_lookup[cms_entry])
		{
			undo = &obj2[0x514/4];
			
			if (*undo == 0)
				*undo = 1;
			else
				undo = nullptr;
		}
	}
	
	uint64_t ret = ResultPortraits2(obj, idx, r8, r9, arg5, arg6, arg7, arg8);
	
	if (undo)
		*undo = 0;
	
	return ret;
}

// This patch is very sensitive. On any change in patch signature, it MUST BE REDONE
PUBLIC void CusAuraMapPatch(uint8_t *buf)
{
	// movsxd  rcx, eax
	PatchUtils::Write8(buf, 0x48); PatchUtils::Write8(buf+1, 0x63); PatchUtils::Write8(buf+2, 0xC8);	
	PatchUtils::Write16((uint16_t *)(buf+3), 0xBA48); // mov rdx, XXXXXXXXXXXXXX
	
	uint64_t gra_addr = (uint64_t)GetRealAura;
	PatchUtils::Write64((uint64_t *)(buf+5), gra_addr);
	PatchUtils::Write16((uint16_t *)(buf+13), 0xE2FF); // jmp rdx
	
	uint64_t *ret_addr = (uint64_t *)(gra_addr+0x1E);
	if (*ret_addr != 0x123456789ABCDEF)
	{
		UPRINTF("Internal error in CusAuraMapPatch\n");
		exit(-1);
	}
	
	PatchUtils::Write64(ret_addr, (uint64_t)(buf+0x82)); // buf+0x82 -> the address of end of switch	
}

// This patch is very sensitive. On any change in patch signature, it MUST BE REDONE
PUBLIC void CusAuraPatchBH11(uint8_t *buf)
{
	PatchUtils::Write16((uint16_t *)buf, 0xB948); // mov rcx, XXXXXXXXXXXXXX
	
	uint64_t g11_addr = (uint64_t)GetCABehaviour11;
	PatchUtils::Write64((uint64_t *)(buf+2), g11_addr);
	PatchUtils::Write16((uint16_t *)(buf+10), 0xE1FF); // jmp rcx
	
	uint64_t *ret_addr = (uint64_t *)(g11_addr+0x62);
	if (*ret_addr != 0x123456789ABCDEF)
	{
		UPRINTF("Internal error in CusAuraPatchBH11\n");
		exit(-1);
	}
	
	PatchUtils::Write64(ret_addr, (uint64_t)(buf+0x43)); // buf+0x43 -> address to return to
}

// This patch is very sensitive. On any change in patch signature, it MUST BE REDONE
PUBLIC uint32_t CusAuraPatchInt2(uint8_t *obj)
{
	if (obj[0xB0] & 0x10) // if awaken skill? needs confirm...
	{
		uint32_t cus_aura = *(uint32_t *)&obj[0x13B8];
		
		if (cus_aura == 0xFFFFFFFF)
			return 0xFFFFFFFF;
		
		if (cus_aura < LOOKUP_SIZE)
			return cus_aura_int2_lookup[cus_aura];
		
		return 0;
	}
	
	return 0xFFFFFFFF;
}

PUBLIC void Behaviour10Setup(Behaviour10FuncType orig)
{
	Behaviour10Func = orig;
}

PUBLIC uint32_t Behaviour10FuncPatched(void *obj, uint32_t cus_aura)
{
	if (cus_aura < LOOKUP_SIZE && cus_aura_bh10_lookup[cus_aura] <= 21)
		cus_aura = cus_aura_bh10_lookup[cus_aura];
	
	return Behaviour10Func(obj, cus_aura);
}

PUBLIC uint32_t CusAuraPatchInt3(uint8_t *obj)
{
	if (obj[0xB0] & 0x10) // if awaken skill? needs confirm...
	{
		uint32_t cus_aura = *(uint32_t *)&obj[0x13B8];		
		
		if (cus_aura < LOOKUP_SIZE)
			return cus_aura_int3_lookup[cus_aura];
	}
	
	return 0;
}

// This patch is very sensitive. On any change in patch signature, it MUST BE REDONE
PUBLIC void CusAuraPatchTeleport(uint8_t *buf)
{
	PatchUtils::Write16((uint16_t *)buf, 0xB948); // mov rcx, XXXXXXXXXXXXXX
	
	uint64_t ft_addr = (uint64_t)ForceTeleport;
	PatchUtils::Write64((uint64_t *)(buf+2), ft_addr);
	PatchUtils::Write16((uint16_t *)(buf+10), 0xE1FF); // jmp rcx
	
	uint64_t *ret_addr1 = (uint64_t *)(ft_addr+0x27);
	if (*ret_addr1 != 0x123456789ABCDEF)
	{
		UPRINTF("Internal error in CusAuraPatchTeleport\n");
		exit(-1);
	}
	
	PatchUtils::Write64(ret_addr1, (uint64_t)(buf+0xC)); // buf+0xC -> address return for teleport
	
	uint64_t *ret_addr2 = (uint64_t *)(ft_addr+0x33);
	if (*ret_addr2 != 0xFEDCBA987654321)
	{
		UPRINTF("Internal error in CusAuraPatchTeleport (2)\n");
		exit(-1);
	}
	
	PatchUtils::Write64(ret_addr2, (uint64_t)(buf+0x502)); // buf+0x502 -> address return for no teleport
}

PUBLIC uint32_t Behaviour13(uint8_t *obj)
{
	uint32_t cus_aura = *(uint32_t *)&obj[0x13B8];	
	
	if (cus_aura < LOOKUP_SIZE && cus_aura_bh13_lookup[cus_aura] <= 21)
		cus_aura = cus_aura_bh13_lookup[cus_aura];

	return cus_aura;
}

} // extern "C"