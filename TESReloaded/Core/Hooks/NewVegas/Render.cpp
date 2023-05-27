#include "Hooks.h"

typedef void (__fastcall* RenderListAdd)(void*, void*, void*, WORD, BYTE, BYTE, void*, void*, void*, void*);
static RenderListAdd RenderListOrig = (RenderListAdd)0xBA9EE0;

void __fastcall RenderListHook(void* ecx, void* edx, void* triStrips, WORD renderPassNum, BYTE enable, BYTE numLights, ShadowSceneLight* light0, ShadowSceneLight* light1, ShadowSceneLight* light2, ShadowSceneLight* light3) {
	// Block render passes to avoid confusing remix
	switch (renderPassNum) {
		// Normals
		case BSSM_DIFFUSEDIR_S:
		case BSSM_DIFFUSEPT2:
		case BSSM_DIFFUSEPT2_S:
		case BSSM_DIFFUSEPT2_SFg:
		case BSSM_DIFFUSEPT3:
		case BSSM_DIFFUSEPT3_SFg:
		// Weird transparent black
		case BSSM_TEXTURE_Vc:
			return;
	}
	// Hide lights from game
	RenderListOrig(ecx, edx, triStrips, renderPassNum, enable, min(numLights, 1), light0, nullptr, nullptr, nullptr);
}

void AttachRenderHooks()
{
	MH_CreateHookSimple((LPVOID*)&RenderListOrig, RenderListHook);
}