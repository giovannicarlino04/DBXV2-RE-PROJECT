#ifndef XV2PATCHER
#define XV2PATCHER

#include "IniFile.h"

#define EXPORT 	__declspec(dllexport)
#define PUBLIC	EXPORT

#define PROCESS_NAME "dbxv2.exe"

#define INI_PATH		"../XV2PATCHER/xv2patcher.ini"
#define PATCHES_PATH	"../XV2PATCHER/EPatches"
#define EXE_PATH	"DBXV2.exe"

#define CONTENT_ROOT	"../"

#define MINIMUM_GAME_VERSION	1.00f

typedef void (* IGGYSetTraceCallbackType)(void *callback, void *param);

extern std::string myself_path;
extern IniFile ini;

extern void iggy_trace_callback(void *param, void *unk, const char *str, size_t len);

#endif // XV2PATCHER
