// Disable culling patch, thanks Wall_SoGB

void __fastcall BSCullingProcess_SetCullModeEx(BSCullingProcess* apThis, void*, UInt32 eType) {
	apThis->kCullMode = 1; // CULL_ALLPASS
}

void __fastcall BSCullingProcess_ProcessEx(BSCullingProcess* apThis, void*, NiAVObject* apObject) {
	apThis->pCompoundFrustum = 0;
	apObject->OnVisible(apThis);
}

void AttachCullingHooks() {
	SafeWriteCall(0x8743A6, (UInt32)BSCullingProcess_SetCullModeEx);
	SafeWrite32(0x101E330, (UInt32)BSCullingProcess_ProcessEx);
}