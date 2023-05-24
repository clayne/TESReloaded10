// Anti culling patch, thanks Wall_SoGB

BSCullingProcess* __fastcall BSCullingProcessCreateHook(BSCullingProcess* apThis, void*, void* apVisibleSet) {

	ThisCall(0x4A0EB0, apThis, apVisibleSet);
	apThis->kCullMode = 1; // ALLPASS 
	return apThis;
}

void AttachCullingHooks() {
	SafeWriteCall(0xC51887, (UInt32)BSCullingProcessCreateHook);
}