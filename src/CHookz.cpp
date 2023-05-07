#include "pch.h"
#include "CHookz.h"


#include <stdexcept>
#include <intrin.h>

#include "MinHook/MinHook.h"

#include "CMemoryManager.h"
#include "ESP.h"
#include "AimBot.h"
#include "Miscellaneous.h"
#include "TriggerBot.h"
#include "SkinChanger.h"


void Hookz::Setup()
{
    if (MH_Initialize())
        throw std::runtime_error("Failed to initialize MinHook");

    if (MH_CreateHook(CMemoryManager::GetVirtualFuncAddr(TempDirectX::D3D9Device, 42), &EndScene_hk, (void**)(&EndScene)))
        throw std::runtime_error("Failed to hook EndScene()");

    if (MH_CreateHook(CMemoryManager::GetVirtualFuncAddr(TempDirectX::D3D9Device, 16), &Reset_hk, (void**)(&Reset)))
        throw std::runtime_error("Failed to hook Reset()");

    if (MH_CreateHook(CMemoryManager::GetVirtualFuncAddr(IBaseClientDLL, 37), &FrameStageNotify_hk, (void**)(&FrameStageNotify)))
        throw std::runtime_error("Failed to hook FrameStageNotify()");

    if (MH_EnableHook(MH_ALL_HOOKS))
        throw std::runtime_error("Failed to enable MH_ALL_HOOKS");

    // The temporary DirectX and the device is no longer needed
    TempDirectX::DestroyDirectX();
}

void Hookz::Destroy()
{
    MH_DisableHook(MH_ALL_HOOKS);
    MH_RemoveHook(MH_ALL_HOOKS);
    MH_Uninitialize();
}

long __stdcall Hookz::EndScene_hk(IDirect3DDevice9* device)
{
    // CSGO calls EndScene twice (from 'sharedapidx9.dll' and 'GameOverlayRenderer.dll') which would hurt performance and is obsolete
    // therefore, we save the address of the first call to EndScene and check against it (so we only render our menu when EndScene is being called from 'sharedapidx9.dll')
    // (VERY STRANGE!) Sometimes when you boot up the game, it actually only calls 'EndScene' once - which is from 'sharedapidx9.dll'
    static const void* firstReturnAddress = _ReturnAddress(); // This is only assigned once, because it is a static

    const long result = EndScene(device, device);

    if (_ReturnAddress() != firstReturnAddress) return result;

    if (!GUI::IsSetup) GUI::Setup(device);

    {
        // Menu + ImGui render related cheats
		GUI::ImGuiRenderFrame();

        // Render unrelated cheats
		if (GUI::Options::Aimbot::Enabled) AimBot::AimAt();
		if (GUI::Options::Triggerbot::Enabled) TriggerBot::Run();
		if (GUI::Options::Miscellaneous::BunnyHop) BunnyHop();
		if (GUI::Options::Miscellaneous::AntiFlash) AntiFlash();
    }

    return result;
}

HRESULT __stdcall Hookz::Reset_hk(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* params)
{
    ImGui_ImplDX9_InvalidateDeviceObjects();
    const HRESULT result = Reset(device, device, params);
    ImGui_ImplDX9_CreateDeviceObjects();

    return result;
}

// SkinChaning works better in FNS as opposed to EndScene because FNS is also the place where the GUI updates.
void _stdcall Hookz::FrameStageNotify_hk(ClientFrameStage currentStage)
{
    if (currentStage == FRAME_NET_UPDATE_POSTDATAUPDATE_START)
        if (GUI::Options::SkinChanger::Enabled) SkinChanger();

    FrameStageNotify(currentStage);
}