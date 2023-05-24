// Anti culling patch, thanks Wall_SoGB

class BSOcclusionPlane;

void __fastcall BSCullingProcess_SetCullModeEx(BSCullingProcess* apThis, void*, UInt32 eType) {
	apThis->kCullMode = 1; // CULL_ALLPASS
}

void __fastcall BSCullingProcess_PushCullModeEx(BSCullingProcess* apThis, void*, UInt32 eType) {
	apThis->eTypeStack[apThis->uiStackIndex++] = 1;
	apThis->kCullMode = 1;
}

typedef void* (__fastcall* MTRenderManager__AddTask)(void*, void*, NiCamera*, BSCompoundFrustum*, NiNode*, void*, void*, BSShaderAccumulator*, UInt32, UInt32, UInt32);
static MTRenderManager__AddTask MTRenderManager__AddTaskFnk = (MTRenderManager__AddTask)0xBA3390;

void __fastcall MTRenderManager__AddTaskEx(void* ecx, void* edx, NiCamera* apCamera, BSCompoundFrustum* apFrustum, NiNode* apNode, void* apGeometryList1, void* apGeometryList2, BSShaderAccumulator* apShaderAccum, UInt32 aeCullMode, UInt32, UInt32) {
	MTRenderManager__AddTaskFnk(ecx, edx, apCamera, apFrustum, apNode, apGeometryList1, apGeometryList2, apShaderAccum, 1, 0, 0);
}

bool __fastcall BSOcclusionPlane__WithinFrustumEx(
	BSOcclusionPlane* apThis, void*,
	NiFrustumPlanes* aPlanes,
	bool abViewHitPlane,
	NiPoint3 akViewIntersection) {
	return true;
}

bool __fastcall WithinFrustumDistFirst(void*, void*, void*, void*) {
	return true;
}

bool __fastcall CompletelyWithinFrustum(void*, void*, void*) {
	return true;
}

// TODO: The vtable for NiAVObject seems off, OnVisible at 0xBC instead of 0xD4
/*
void __fastcall BSCullingProcess_ProcessEx(BSCullingProcess* apThis, void*, NiAVObject* apObject) {
	apThis->pCompoundFrustum = 0;
	apObject->OnVisible(apThis);
}
//*/
__declspec(naked) void BSCullingProcess_ProcessEx() {
	__asm {
		push ebp
		mov ebp, esp
		mov edx, ecx
		mov ecx, dword ptr ss:[ebp + 8]
		mov dword ptr ds:[edx + 0xC0], 0
		mov eax, dword ptr ds:[ecx]
		mov dword ptr ss:[ebp + 8], edx
		pop ebp
		jmp dword ptr ds:[eax + 0xD4]
	}
}

void AttachCullingHooks() {
	for (int callAddr : {0x8014F7, 0x80182E, 0x874260, 0x8742F0, 0x874316, 0x8743A6, 0x876233, 0x8767FD, }) {
		SafeWriteCall(callAddr, (UInt32)BSCullingProcess_SetCullModeEx);
	}

	for (int callAddr : {0x54EB4E, 0x54EB91, 0x7134DF, 0x73354D, 0x740F7D, 0x77F188, 0x78E820, 0x794939, 0x7BBD3D, 0x7C1A0D, 0x7C9D09, 0x7E480E, 0x7FBD02, 0x7FE3D1, 0x8753CA, 0x875C45, }) {
		SafeWriteCall(callAddr, (UInt32)BSCullingProcess_PushCullModeEx);
	}

	for (int callAddr : {0x4EA815, 0x4EAF18, 0x4EB1D7, 0x4EC769, 0x713F97, 0x874037, 0x87509C, 0xB9E6CB, }) {
		SafeWriteCall(callAddr, (UInt32)MTRenderManager__AddTaskEx);
	}

	for (int callAddr : {0x549CCF, 0xB5AD64, 0xC347B7, 0xC4965C, }) {
		SafeWriteCall(callAddr, (UInt32)BSOcclusionPlane__WithinFrustumEx);
	}

	for (int callAddr : {0xB5F25E, 0xB5F33A, 0xC49596, }) {
		SafeWriteCall(callAddr, (UInt32)WithinFrustumDistFirst);
	}

	SafeWriteCall(0xC49690, (UInt32)CompletelyWithinFrustum);
	SafeWrite32(0x101EA44, (UInt32)CompletelyWithinFrustum);
	SafeWrite32(0x101E984, (UInt32)CompletelyWithinFrustum);
	SafeWrite32(0x101E330, (UInt32)BSCullingProcess_ProcessEx);
}