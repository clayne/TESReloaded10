#define WaitForDebugger 0
#define HookDevice 0

#include "Hooks/NewVegas/Hooks.h"
#include "Device/Hook.h"

static bool GetPrivateProfileBoolW(LPCWSTR lpAppName, LPCWSTR lpKeyName, BOOLEAN bDefault, LPCWSTR lpFileName) {
	wchar_t boolStr[6];
	DWORD strlen = GetPrivateProfileStringW(lpAppName, lpKeyName, bDefault ? L"True" : L"False", boolStr, _countof(boolStr), lpFileName);
	return (CompareStringW(LOCALE_INVARIANT, NORM_IGNORECASE, boolStr, strlen, L"True", 4) == CSTR_EQUAL);
}

extern "C" {

	bool NVSEPlugin_Query(const PluginInterface* Interface, PluginInfo* Info) {
		
		Info->InfoVersion = PluginInfo::kInfoVersion;
		Info->Name = "NewVegasRTXLight";
		Info->Version = 4;
		return true;

	}

	bool NVSEPlugin_Load(const PluginInterface* Interface) {

#if _DEBUG
	#if WaitForDebugger
		while (!IsDebuggerPresent()) Sleep(10);
	#endif
#endif
#if HookDevice
		AttachDeviceHooks();
#endif

		Logger::Initialize("NewVegasRTXLight.log");
		//CommandManager::Initialize(Interface);

		if (!Interface->IsEditor) {
			PluginVersion::CreateVersionString();
			SettingManager::Initialize();
			//TheSettingManager->LoadSettings();

			HMODULE hDllHandle;
			GetModuleHandleExW(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, (LPCWSTR)&NVSEPlugin_Load, &hDllHandle);
			wchar_t iniPath[MAX_PATH];
			GetModuleFileNameW(hDllHandle, iniPath, _countof(iniPath));
			wchar_t* slash = wcsrchr(iniPath, L'\\');
			if (slash) {
				slash[0] = 0;
				if (SUCCEEDED(StringCchCatW(iniPath, _countof(iniPath), L"\\NewVegasRTXLight.ini"))) {
					Logger::Log("Reading settings from %ls", iniPath);
					SettingManager::LightRangeMode = GetPrivateProfileIntW(L"NewVegasRTXLight", L"LightRangeMode", SettingManager::LightRangeMode, iniPath);
					if (SettingManager::LightRangeMode < 0 || SettingManager::LightRangeMode > 2) {
						Logger::Log("Invalid light mode %u, resetting to 1", SettingManager::LightRangeMode);
						SettingManager::LightRangeMode = 1;
					}
					SettingManager::DisableCulling = GetPrivateProfileBoolW(L"NewVegasRTXLight", L"DisableCulling", SettingManager::DisableCulling, iniPath);
					SettingManager::SunLight = GetPrivateProfileBoolW(L"NewVegasRTXLight", L"SunLight", SettingManager::SunLight, iniPath);

					// Write settings back
					wchar_t buffer[32];
					_snwprintf(buffer, _countof(buffer), L"%u", SettingManager::LightRangeMode);
					WritePrivateProfileStringW(L"NewVegasRTXLight", L"LightRangeMode", buffer, iniPath);
					WritePrivateProfileStringW(L"NewVegasRTXLight", L"DisableCulling", SettingManager::DisableCulling ? L"True" : L"False", iniPath);
					WritePrivateProfileStringW(L"NewVegasRTXLight", L"SunLight", SettingManager::SunLight ? L"True" : L"False", iniPath);
				} else {
					Logger::Log("Failed to calculate ini path, path is too long");
				}
			} else {
				Logger::Log("Failed to calculate ini path, odd path?");
			}

			Logger::Log("Light mode:      %u", SettingManager::LightRangeMode);
			Logger::Log("Disable culling: %s", SettingManager::DisableCulling ? "true" : "false");
			Logger::Log("Inaccurate sun:  %s", SettingManager::SunLight ? "true" : "false");

			AttachHooks();
		}
		return true;

	}

};