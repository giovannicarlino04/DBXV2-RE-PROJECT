#include <windows.h>
#include <wininet.h>
#include "IniFile.h"
#include "EPatchFile.h"
#include "PatchUtils.h"
#include "Mutex.h"
#include "xv2patcher.h"
#include "debug.h"

static HMODULE patched_dll;
static Mutex mutex;
std::string myself_path;
IniFile ini;

static bool in_game_process()
{
	char szPath[MAX_PATH];
	
	GetModuleFileName(NULL, szPath, MAX_PATH);
	_strlwr(szPath);
	
	// A very poor aproach, I know
	return (strstr(szPath, PROCESS_NAME) != NULL);
}

extern "C"
{
	
#ifdef DINPUT

	PUBLIC HRESULT DirectInput8Create(HINSTANCE hinst, DWORD dwVersion, REFIID riidltf, LPVOID * ppvOut, void *punkOuter)
	{
		DPRINTF("%s: ****************I have been called but I shouldn't!!!\n", FUNCNAME);
		return E_INVALIDARG;
	}
	
	PUBLIC HRESULT DllCanUnloadNow()
	{
		DPRINTF("%s: ****************I have been called but I shouldn't!!!\n", FUNCNAME);
		return -1;
	}
	
	PUBLIC HRESULT DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID *ppv)
	{
		DPRINTF("%s: ****************I have been called but I shouldn't!!!\n", FUNCNAME);
		return 0x80040111;
	}
	
	PUBLIC HRESULT DllRegisterServer()
	{
		DPRINTF("%s: ****************I have been called but I shouldn't!!!\n", FUNCNAME);
		return E_UNEXPECTED;
	}
	
	PUBLIC HRESULT DllUnregisterServer()
	{
		DPRINTF("%s: ****************I have been called but I shouldn't!!!\n", FUNCNAME);
		return -1;
	}

#else
	
	PUBLIC DWORD XInputGetState()
	{
		DPRINTF("%s: ****************I have been called but I shouldn't!!!\n", FUNCNAME);
		return ERROR_DEVICE_NOT_CONNECTED;
	}
	
	PUBLIC DWORD XInputSetState()
	{
		DPRINTF("%s ****************I have been called but I shouldn't!!!\n", FUNCNAME);
		return ERROR_DEVICE_NOT_CONNECTED;
	}
	
	PUBLIC DWORD XInputGetBatteryInformation(DWORD,  BYTE, void *)
	{
		DPRINTF("%s: ****************I have been called but I shouldn't!!!\n", FUNCNAME);
		return ERROR_DEVICE_NOT_CONNECTED;
	}
	
	PUBLIC void XInputEnable(BOOL)
	{
		DPRINTF("%s: ****************I have been called but I shouldn't!!!\n", FUNCNAME);
	}
	
	PUBLIC DWORD XInputGetCapabilities(DWORD, DWORD, void *)
	{
		DPRINTF("%s: ****************I have been called but I shouldn't!!!\n", FUNCNAME);
		return ERROR_DEVICE_NOT_CONNECTED;
	}
	
	PUBLIC DWORD XInputGetDSoundAudioDeviceGuids(DWORD, void *, void *)
	{
		DPRINTF("%s: ****************I have been called but I shouldn't!!!\n", FUNCNAME);
		return ERROR_DEVICE_NOT_CONNECTED;
	}
	
	PUBLIC DWORD XInputGetKeystroke(DWORD, DWORD, void *)
	{
		DPRINTF("%s: ****************I have been called but I shouldn't!!!\n", FUNCNAME);
		return ERROR_DEVICE_NOT_CONNECTED;
	}
	
	PUBLIC DWORD XInputGetStateEx(DWORD, void *)
	{
		DPRINTF("%s: ****************I have been called but I shouldn't!!!\n", FUNCNAME);
		return ERROR_DEVICE_NOT_CONNECTED;
	}
	
	PUBLIC DWORD XInputWaitForGuideButton(DWORD, DWORD, void *)
	{
		DPRINTF("%s: ****************I have been called but I shouldn't!!!\n", FUNCNAME);
		return ERROR_DEVICE_NOT_CONNECTED;
	}
	
	PUBLIC DWORD XInputCancelGuideButtonWait()
	{
		DPRINTF("%s: ****************I have been called but I shouldn't!!!\n", FUNCNAME);
		return ERROR_DEVICE_NOT_CONNECTED;
	}
	
	PUBLIC DWORD XInputPowerOffController()
	{
		DPRINTF("%s: ****************I have been called but I shouldn't!!!\n", FUNCNAME);
		return ERROR_DEVICE_NOT_CONNECTED;
	}
#endif
}

static bool load_dll(bool critical)
{
	static const std::vector<const char *> exports =
	{
#ifdef DINPUT
		"DirectInput8Create",
		"DllCanUnloadNow",
		"DllGetClassObject",
		"DllRegisterServer",
		"DllUnregisterServer"
#else
		"XInputGetState",
		"XInputSetState",
		"XInputGetBatteryInformation",
		"XInputEnable",
		"XInputGetCapabilities",
		"XInputGetDSoundAudioDeviceGuids",
		"XInputGetKeystroke",
		(char *)100,
		(char *)101,
		(char *)102,
		(char *)103
#endif
	};	
	
	static char mod_path[2048];
	static char original_path[256];	
	static bool loaded = false;
	
	MutexLocker lock(&mutex);
	
	if (loaded)
		return true;
	
#ifdef DINPUT
	HMODULE myself = GetModuleHandle("dinput8.dll");
#else
	HMODULE myself = GetModuleHandle("xinput1_3.dll");
#endif

	GetModuleFileNameA(myself, mod_path, sizeof(mod_path));
	
	myself_path = Utils::NormalizePath(mod_path);
	myself_path = myself_path.substr(0, myself_path.rfind('/')+1);	
	DPRINTF("Myself path = %s\n", myself_path.c_str());
	
#ifndef DINPUT	
	if (Utils::FileExists(myself_path+"xinput_other.dll"))
	{
		strncpy(original_path, myself_path.c_str(), sizeof(original_path));
		strncat(original_path, "xinput_other.dll", sizeof(original_path));
	}
	else
#endif
	{	
		if (GetSystemDirectory(original_path, sizeof(original_path)) == 0)
			return false;

#ifdef DINPUT
		strncat(original_path, "\\dinput8.dll", sizeof(original_path));
#else
		strncat(original_path, "\\xinput1_3.dll", sizeof(original_path));
#endif
	}
	
	patched_dll = LoadLibrary(original_path);		
	if (!patched_dll)
	{
		if (critical)
			UPRINTF("Cannot load original DLL (%s).\nLoadLibrary failed with error %u\n", original_path, (unsigned int)GetLastError());
				
		return false;
	}
	
	DPRINTF("original DLL path: %s   base=%p\n", original_path, patched_dll);
		
	for (auto &export_name : exports)
	{
		uint64_t ordinal = (uint64_t)export_name;
		
		uint8_t *orig_func = (uint8_t *)GetProcAddress(patched_dll, export_name);
		
		if (!orig_func)
		{
			if (ordinal < 0x1000)			
			{
				DPRINTF("Warning: ordinal function %I64d doesn't exist in this system.\n", ordinal);
				continue;		
			}
			else
			{
				UPRINTF("Failed to get original function: %s\n", export_name);			
				return false;
			}
		}
		
		uint8_t *my_func = (uint8_t *)GetProcAddress(myself, export_name);
		
		
		if (!my_func)
		{
			if (ordinal < 0x1000)			
				UPRINTF("Failed to get my function: %I64d\n", ordinal);
			else
				UPRINTF("Failed to get my function: %s\n", export_name);
			
			return false;
		}
		
		if (ordinal < 0x1000)
			DPRINTF("%I64d: address of microsoft: %p, address of mine: %p\n", ordinal, orig_func, my_func);
		else
			DPRINTF("%s: address of microsoft: %p, address of mine: %p\n", export_name, orig_func, my_func);
		
		DPRINTF("Content of microsoft func: %02X%02X%02X%02X%02X\n", orig_func[0], orig_func[1], orig_func[2], orig_func[3], orig_func[4]);
		DPRINTF("Content of my func: %02X%02X%02X%02X%02X\n", my_func[0], my_func[1], my_func[2], my_func[3], my_func[4]);
		
		PatchUtils::Hook(my_func, nullptr, orig_func);
		DPRINTF("Content of my func after patch: %02X%02X%02X%02X%02X\n", my_func[0], my_func[1], my_func[2], my_func[3], my_func[4]);
	}
	
	loaded = true;
	return true;
}

static void unload_dll()
{
	if (patched_dll)
	{
		FreeLibrary((HMODULE)patched_dll);
		patched_dll = nullptr;
	}
}

static bool load_patch_file(const std::string &path, bool is_directory, void *custom_param)
{
	if (Utils::EndsWith(path, ".xml", false))
	{
#ifdef DINPUT
		EPatchFile epf(myself_path+"dinput8.dll");
#else
		EPatchFile epf(myself_path+"xinput1_3.dll");
#endif
		
		int enabled;
		bool enabled_b;
		std::string setting;
		
		if (!epf.CompileFromFile(path))
		{
			UPRINTF("Failed to compile file \"%s\"\n", path.c_str());
			exit(-1);
		}
		
		//DPRINTF("File %s compiled.\n", path.c_str());		
		if ((enabled = epf.GetEnabled(setting)) < 0)
		{	
			ini.GetBooleanValue("Patches", setting, &enabled_b, false);
			enabled = enabled_b;
		}
		
		if (!enabled)
			return true;
		
		for (EPatch &patch : epf)
		{
			if ((enabled = patch.GetEnabled(setting)) < 0)
			{
				ini.GetBooleanValue("Patches", setting, &enabled_b, false);
				enabled = enabled_b;
			}
			
			if (!enabled)
				continue;
			
			if (!patch.Apply())
			{
				UPRINTF("Failed to apply patch \"%s:%s\"\n", epf.GetName().c_str(), patch.GetName().c_str());
				exit(-1);
			}
		}
	}	
	
	return true;
}

static void load_patches()
{
	Utils::VisitDirectory(myself_path+PATCHES_PATH, true, false, false, load_patch_file);
}

static void start()
{
#ifdef DINPUT
	const char *my_dll_name = "dinput8.dll";
#else
	const char *my_dll_name = "xinput1_3.dll";
#endif
	
	DPRINTF("XV2PATCHER VERSION 1.2. Exe base = %p. My Dll base = %p. My dll name: %s\n", GetModuleHandle(NULL), GetModuleHandle(my_dll_name), my_dll_name);	
	
	float version = Utils::GetExeVersion(myself_path+EXE_PATH);
	DPRINTF("Running on game version %.2f\n", version);
	
	if (version != 0.0 && version < MINIMUM_GAME_VERSION)
	{
		UPRINTF("This game version (%.2f) is not compatible with this version of the patcher.\nMin version required is: %.2f\n", version, MINIMUM_GAME_VERSION);
		exit(-1);
	}
	
	load_patches();				
}

static void IggySetTraceCallbackUTF8Patched(void *, void *param)
{
	HMODULE iggy = GetModuleHandle("iggy_w64.dll");
	if (!iggy)
		return;
	
	IGGYSetTraceCallbackType func = (IGGYSetTraceCallbackType)GetProcAddress(iggy, "IggySetTraceCallbackUTF8");
	
	if (func)
		func((void *)iggy_trace_callback, param);
}

static BOOL InternetGetConnectedStatePatched(LPDWORD lpdwFlags, DWORD dwReserved)
{
	*lpdwFlags = INTERNET_CONNECTION_OFFLINE;
	return FALSE;
}

extern "C" PUBLIC void BattleTimer(float *time)
{
	float new_time[1];
	
	ini.GetFloatValue("BattleTimer", "time", &new_time[0], 180.0f);
	DPRINTF("Battle timer set to %f seconds.  (1.#INF00 is infinity)\n", new_time[0]);
	
	PatchUtils::Write32(time, *(uint32_t *)&new_time);
}

VOID WINAPI GetStartupInfoW_Patched(LPSTARTUPINFOW lpStartupInfo)
{
	static bool started = false;
	
	// This function is only called once by the game but... just in case
	if (!started)
	{	
		int default_log_level;
		
		if (!load_dll(true))
			exit(-1);
		
		ini.LoadFromFile(myself_path+INI_PATH);
		ini.GetIntegerValue("General", "default_log_level", &default_log_level, 1);
		ini.AddIntegerConstant("auto", 0);
		set_debug_level(default_log_level);				
		
		bool iggy_debug, offline_mode;
		
		start();
		started = true;
		
		ini.GetBooleanValue("Patches", "iggy_debug", &iggy_debug, false);
		ini.GetBooleanValue("Patches", "offline_mode", &offline_mode, false);
		
		if (iggy_debug)
		{
			if (!PatchUtils::HookImport("iggy_w64.dll", "IggySetTraceCallbackUTF8", (void *)IggySetTraceCallbackUTF8Patched))
			{
				DPRINTF("Failed to hook import of IggySetTraceCallbackUTF8.\n");						
			}
		}
		
		if (offline_mode)
		{
			if (!PatchUtils::HookImport("WININET.DLL", "InternetGetConnectedState", (void *)InternetGetConnectedStatePatched))
			{
				DPRINTF("Failed to hook import of InternetGetConnectedState");
			}
		}
	}
	
	GetStartupInfoW(lpStartupInfo);
}

DWORD WINAPI StartThread(LPVOID)
{
	load_dll(false);
	return 0;
}

extern "C" BOOL WINAPI EXPORT DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	switch (fdwReason)
	{
		case DLL_PROCESS_ATTACH:
		
			static bool done = false;
			
			if (done)
				return TRUE;
			
			set_debug_level(1); 	

			if (in_game_process())
			{					
				done = true;
				
				CreateMutexA(nullptr, FALSE, "XV2PATCHER_INSTANCE");
				if (GetLastError() == ERROR_ALREADY_EXISTS)
				{
					UPRINTF("An instance of xv2patcher already exists.\n"
							"You probably have mixed xinput1_3 and dinput8!\n");
					exit(-1);
				}
				
				load_dll(false);

				if (!PatchUtils::HookImport("KERNEL32.dll", "GetStartupInfoW", (void *)GetStartupInfoW_Patched))
				{
					UPRINTF("GetStartupInfoW hook failed.\n");
					return TRUE;
				}
				
				//DWORD id;
				//CreateThread(NULL, 0, StartThread, NULL, 0, &id);
			}			
			
		break;
		
		case DLL_PROCESS_DETACH:		
			
			if (!lpvReserved)
				unload_dll();
			
		break;
	}
	
	return TRUE;
}
