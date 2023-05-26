#include "Hooks.h"

typedef void(__fastcall* RenderListAdd)(void*, void*, void*, WORD, BYTE, BYTE, void*, void*, void*, void*);
static RenderListAdd RenderListOrig = (RenderListAdd)0xBA9EE0;

void __fastcall RenderListHook(void* ecx, void* edx, void* triStrips, WORD renderPassNum, BYTE enable, BYTE numLights, void* light0, void* light1, void* light2, void* light3) {
	// Block normals and weird transparent black
	if (renderPassNum == 306 || renderPassNum == 334) {
		return;
	}
	// Hide lights from game
	RenderListOrig(ecx, edx, triStrips, renderPassNum, enable, min(numLights, 1), light0, nullptr, nullptr, nullptr);
}

void AttachRenderHooks()
{
	MH_CreateHookSimple((LPVOID*)&RenderListOrig, RenderListHook);
}