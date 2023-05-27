// Disable culling patch, thanks Wall_SoGB

void __fastcall BSCullingProcess_SetCullModeEx(BSCullingProcess* apThis, void*, UInt32 eType) {
	apThis->kCullMode = 1; // CULL_ALLPASS
}

void AttachCullingHooks() {
	SafeWriteCall(0x8743A6, (UInt32)BSCullingProcess_SetCullModeEx);
}