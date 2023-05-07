#pragma once
#include "GUI.h"



// For some reason, it is also possible to make the '__thiscall' into '__stdcall' by removing the 'void* - aka the *this* pointer' argument from the function prototype. EG:
//using EndScene_t = long(__stdcall*)(IDirect3DDevice9*);
// UPDATE: It's actually because the Direct3D classes are wrappers around C API, therefore the member functions are __stdcall
using EndScene_t = long(__thiscall*)(void*, IDirect3DDevice9*);
using Reset_t = HRESULT(__thiscall*)(void*, IDirect3DDevice9*, D3DPRESENT_PARAMETERS*);
enum ClientFrameStage;
using FrameStageNotify_t = void(_stdcall*)(ClientFrameStage currentStage);


class Hookz
{
public:
	static void Setup();
	static void Destroy();

	// Pointers to the original functions (the ones getting hooked)
	static inline EndScene_t EndScene = nullptr;
	static inline Reset_t Reset = nullptr; // The 'IDirect3DDevice9::Reset' is getting called by the game whenever we tab in and out or change the window's resolution. Due to this, we should also reset ImGui's objects and restore them, otherwise the game crashes.
	static inline FrameStageNotify_t FrameStageNotify = nullptr;

	// The detour functions (these will be called by the OG functions, after which they will call back the OG functions to resume the normal execution flow)
	static long __stdcall EndScene_hk(IDirect3DDevice9* device);
	static HRESULT __stdcall Reset_hk(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* params);
	static void _stdcall FrameStageNotify_hk(ClientFrameStage currentStage);
};