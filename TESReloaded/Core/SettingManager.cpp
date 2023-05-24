static bool GetPrivateProfileBoolW(LPCWSTR lpAppName, LPCWSTR lpKeyName, BOOLEAN bDefault, LPCWSTR lpFileName) {
	wchar_t boolStr[6];
	DWORD strlen = GetPrivateProfileStringW(lpAppName, lpKeyName, bDefault ? L"True" : L"False", boolStr, _countof(boolStr), lpFileName);
	return (CompareStringW(LOCALE_INVARIANT, NORM_IGNORECASE, boolStr, strlen, L"True", 4) == CSTR_EQUAL);
}

#define WritePrivateProfileBoolW(lpAppName, lpKeyName, bValue, lpFileName) WritePrivateProfileStringW((lpAppName), (lpKeyName), (bValue) ? L"True" : L"False", (lpFileName))

void SettingManager::Initialize() {
	Logger::Log("Starting the settings manager...");
	TheSettingManager = new SettingManager();

	TheSettingManager->HaveINIPath = false;

	HMODULE hDllHandle;
	GetModuleHandleExW(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, (LPCWSTR)&GetPrivateProfileBoolW, &hDllHandle);
	GetModuleFileNameW(hDllHandle, TheSettingManager->iniPath, _countof(iniPath));
	wchar_t* slash = wcsrchr(TheSettingManager->iniPath, L'\\');
	if (slash) {
		slash[0] = 0;
		if (SUCCEEDED(StringCchCatW(TheSettingManager->iniPath, _countof(iniPath), L"\\NewVegasRTXLight.ini"))) {
			TheSettingManager->HaveINIPath = true;
		}
		else {
			Logger::Log("Failed to calculate ini path, path is too long");
		}
	}
	else {
		Logger::Log("Failed to calculate ini path, odd path?");
	}
}

void SettingManager::LoadSettings() {
	if (!TheSettingManager->HaveINIPath) {
		return;
	}

	Logger::Log("Reading settings from %ls", iniPath);
	SettingManager::LightRangeMode = GetPrivateProfileIntW(L"NewVegasRTXLight", L"LightRangeMode", SettingManager::LightRangeMode, iniPath);
	SettingManager::DisableCulling = GetPrivateProfileBoolW(L"NewVegasRTXLight", L"DisableCulling", SettingManager::DisableCulling, iniPath);
	SettingManager::SunLight = GetPrivateProfileBoolW(L"NewVegasRTXLight", L"SunLight", SettingManager::SunLight, iniPath);
	SettingManager::VisualSun = GetPrivateProfileBoolW(L"NewVegasRTXLight", L"VisualSun", SettingManager::VisualSun, iniPath);

	// Validate settings
	if (SettingManager::LightRangeMode < 0 || SettingManager::LightRangeMode > 2) {
		Logger::Log("Invalid light mode %u, resetting to 1", SettingManager::LightRangeMode);
		SettingManager::LightRangeMode = 1;
	}
}

/*
* Saves the state of the config to the config file
*/
void SettingManager::SaveSettings() {
	if (!TheSettingManager->HaveINIPath) {
		return;
	}

	Logger::Log("Saving settings to %ls", iniPath);
	wchar_t buffer[32];
	_snwprintf(buffer, _countof(buffer), L"%u", SettingManager::LightRangeMode);
	WritePrivateProfileStringW(L"NewVegasRTXLight", L"LightRangeMode", buffer, iniPath);
	WritePrivateProfileBoolW(L"NewVegasRTXLight", L"DisableCulling", SettingManager::DisableCulling, iniPath);
	WritePrivateProfileBoolW(L"NewVegasRTXLight", L"SunLight", SettingManager::SunLight, iniPath);
	WritePrivateProfileBoolW(L"NewVegasRTXLight", L"VisualSun", SettingManager::VisualSun, iniPath);
}

int SettingManager::LightRangeMode = 0;
bool SettingManager::DisableCulling = true;
bool SettingManager::SunLight = true;
bool SettingManager::VisualSun = true;
