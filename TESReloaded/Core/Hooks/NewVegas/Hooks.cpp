#include "Hooks.h"
#include "../lib/minhook/include/MinHook.h"

void AttachHooks() {
	if (MH_Initialize() == MH_OK) {
		MH_CreateHook((void*)InitializeRenderer, &InitializeRendererHook, reinterpret_cast<LPVOID*>(&InitializeRenderer));
		MH_CreateHook((void*)NewTES, &NewTESHook, reinterpret_cast<LPVOID*>(&NewTES));
		MH_CreateHook((void*)NewMenuInterfaceManager, &NewMenuInterfaceManagerHook, reinterpret_cast<LPVOID*>(&NewMenuInterfaceManager));
		if (MH_EnableHook(MH_ALL_HOOKS) != MH_OK) {
			Logger::Log("Failed to apply hooks");
		}
	} else {
		Logger::Log("Failed to initialize MinHook");
	}

	AttachLightHooks();

	if (SettingManager::DisableCulling)
	{
		AttachCullingHooks();
	}
}
