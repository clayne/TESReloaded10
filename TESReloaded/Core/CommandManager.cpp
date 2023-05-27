#define CommandPrefix "NVRH"

static bool CommandExecuter_GetVersion(CommandArgs Args) {

	TheCommandManager->Commands.GetVersion(Args.result);
	return true;

}

static bool CommandExecuter_GetLocationName(CommandArgs Args) {

	TheCommandManager->Commands.GetLocationName(Args.result);
	return true;

}

static CommandInfo CommandInfo_GetVersion					= { CommandPrefix"GetVersion", "", 0, "Returns the plugin version (console command only)", 0, 0, NULL, CommandExecuter_GetVersion, NULL, NULL, 0 };
static CommandInfo CommandInfo_GetLocationName				= { CommandPrefix"GetLocationName", "", 0, "Gets the name of the current worldspace and current cell(console command only)", 0, 0, NULL, CommandExecuter_GetLocationName, NULL, NULL, 0 };

void CommandManager::Initialize(const PluginInterface* Interface) {

	Logger::Log("Initializing the command manager...");
	TheCommandManager = new CommandManager();

	CommandInfo* CommandInfos[] = { &CommandInfo_GetVersion, &CommandInfo_GetLocationName };
	TheCommandManager->RegisterCommands(Interface, nullptr, CommandInfos, 14, nullptr);

}

void CommandManager::PluginCommands::GetVersion(double* result) {

	if (InterfaceManager->IsActive(Menu::MenuType::kMenuType_Console)) Pointers::Functions::PrintToConsole(PluginVersion::VersionString);
	*result = 1;

}

void CommandManager::PluginCommands::GetLocationName(double* result) {

	if (InterfaceManager->IsActive(Menu::MenuType::kMenuType_Console)) {
		const char* worldName = NULL;
		const char* regionName = NULL;
		const char* cellName = Player->parentCell->GetEditorName();

		std::string s = "Worldspace: ";
		if (TESWorldSpace* currentWS = Player->GetWorldSpace()) worldName = currentWS->GetEditorName();
		if (worldName != NULL)
			s += worldName;
		else
			s += "(no worldspace)";
		s += ", Region: ";
		if (TESRegionEx* currentRE = (TESRegionEx*)Player->GetRegion()) regionName = currentRE->EditorName;
		if (regionName != NULL)
			s += regionName;
		else
			s += "(no region)";
		s += ", Cell: ";
		s += cellName;
		Pointers::Functions::PrintToConsole(s.c_str());
	}
	*result = 1;

}
