// Disable culling patch, thanks Wall_SoGB
#include "Hooks.h"

class BSOcclusionPlane;

void __fastcall BSCullingProcess_SetCullModeEx(BSCullingProcess* apThis, void*, UInt32 eType) {
	apThis->kCullMode = 1; // CULL_ALLPASS
}

void __fastcall BSCullingProcess_PushCullModeEx(BSCullingProcess* apThis, void*, UInt32 eType) {
	apThis->eTypeStack[apThis->uiStackIndex++] = 1;
	apThis->kCullMode = 1;
}

typedef void (__fastcall* MTRenderManager__AddTask)(void*, void*, NiCamera*, BSCompoundFrustum*, NiNode*, void*, void*, BSShaderAccumulator*, UInt32, UInt32, UInt32);
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
	// Not all calls of this are patched
	for (int callAddr : {0x8014F7, 0x80182E, 0x874260, 0x8742F0, 0x874316, 0x8743A6, 0x876233, 0x8767FD, }) {
		SafeWriteCall(callAddr, (UInt32)BSCullingProcess_SetCullModeEx);
	}

	SafeWriteJump(0xC4F270, (UInt32)BSCullingProcess_PushCullModeEx);
	MH_CreateHookSimple((LPVOID*)&MTRenderManager__AddTaskFnk, MTRenderManager__AddTaskEx);
	SafeWriteJump(0xC33CF0, (UInt32)BSOcclusionPlane__WithinFrustumEx);
	SafeWriteJump(0xC344C0, (UInt32)WithinFrustumDistFirst);

	SafeWriteCall(0xC49690, (UInt32)CompletelyWithinFrustum);
	SafeWrite32(0x101EA44, (UInt32)CompletelyWithinFrustum);
	SafeWrite32(0x101E984, (UInt32)CompletelyWithinFrustum);
	SafeWrite32(0x101E330, (UInt32)BSCullingProcess_ProcessEx);
}