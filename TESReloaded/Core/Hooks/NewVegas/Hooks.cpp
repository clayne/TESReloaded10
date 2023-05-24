#include "Hooks.h"

void AttachHooks() {

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&(PVOID&)InitializeRenderer,			&InitializeRendererHook);
	DetourAttach(&(PVOID&)NewTES,						&NewTESHook);
	DetourAttach(&(PVOID&)NewMenuInterfaceManager,		&NewMenuInterfaceManagerHook);
	DetourTransactionCommit();

	AttachLightHooks();

	if (SettingManager::DisableCulling)
	{
		AttachCullingHooks();
	}
}
