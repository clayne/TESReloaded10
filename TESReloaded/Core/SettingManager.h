#pragma once

typedef std::vector<std::string> StringList;

class SettingManager : public SettingManagerBase {
public:
	static void				Initialize();
	void					LoadSettings();
	void					SaveSettings();

	bool					HaveINIPath;
	wchar_t					iniPath[MAX_PATH];

	// Settings
	static bool		PassLights;
	static int		LightRangeMode;
	static bool		DisableCulling;
	static bool		SunLight;
	static bool		VisualSun;
};
