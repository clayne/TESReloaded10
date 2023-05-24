#define WaitForDebugger 0

#include "Hooks/NewVegas/Hooks.h"


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

		Logger::Initialize("NewVegasRTXLight.log");
		//CommandManager::Initialize(Interface);

		if (!Interface->IsEditor) {
			PluginVersion::CreateVersionString();
			SettingManager::Initialize();
			TheSettingManager->LoadSettings();
			TheSettingManager->SaveSettings();

			// Log settings
			Logger::Log("Light mode:       %u", SettingManager::LightRangeMode);
			Logger::Log("Disable culling:  %s", SettingManager::DisableCulling ? "true" : "false");
			Logger::Log("Enable sun light: %s", SettingManager::SunLight ? "true" : "false");
			Logger::Log("Use visual sun:   %s", SettingManager::VisualSun ? "true" : "false");

			AttachHooks();
		}
		return true;

	}

};