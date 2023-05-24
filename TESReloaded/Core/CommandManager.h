#pragma once

class CommandManager: public CommandManagerBase { // Never disposed
public:
	class PluginCommands {
	public:
		void GetVersion(double* result);
		void GetLocationName(double* result);
	};

	static void			Initialize(const PluginInterface* Interface);

	PluginCommands		Commands;
};