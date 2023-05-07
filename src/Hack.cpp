#include "pch.h"
#include "Hack.h"

#include "offsets.h"
#include "Globals.h"
#include "Interfaces.h"

#include "CMemoryManager.h"
#include "CVector.h"
#include "ESP.h"
#include "C_BaseEntity.h"
#include "GUI.h"
#include <string>
#include "PatternScanner.h"


using namespace Globals;
using namespace offsets;


void Hack::Initialize()
{
	// Create console
    AllocConsole();
    freopen_s(&Globals::File, "CONOUT$", "w", stdout);

    GetModuleHandles();
    GetInterfaces();
    GetOffsets();

    GUI::PreSetup();
}

void Hack::GetModuleHandles()
{
    clientBase = CMemoryManager::GetModule("client.dll");
    engineBase = CMemoryManager::GetModule("engine.dll");
}

void Hack::GetInterfaces()
{
    CreateInterface = (CreateInterface_t)CMemoryManager::GetExportedFuncAddr("client.dll", "CreateInterface");

    // Interfaces
    IClientEntityList_ = (IClientEntityList*)CreateInterface("VClientEntityList003", NULL);
    IBaseClientDLL = (void*)CreateInterface("VClient018", NULL);
    IClientMode = **(void***)((*(unsigned int**)IBaseClientDLL)[10] + 5);

    //void* CreateMove = (*(void***)IClientMode)[24];
}

void Hack::GetOffsets()
{
    MODULEINFO clientModuleInfo = CMemoryManager::GetModuleInfo(clientBase);
    MODULEINFO engineModuleInfo = CMemoryManager::GetModuleInfo(engineBase);

    uintptr_t dwEntityList = PatternScanner::ScanSafe("\xBB\x00\x00\x00\x00\x83\xFF\x01\x0F\x8C\x00\x00\x00\x00\x3B\xF8", "x????xxxxx????xx", clientBase, clientModuleInfo.SizeOfImage);
    uintptr_t dwLocalPlayer = PatternScanner::ScanSafe("\x8D\x34\x85\x00\x00\x00\x00\x89\x15\x00\x00\x00\x00\x8B\x41\x08\x8B\x48\x04\x83\xF9\xFF", "xxx????xx????xxxxxxxxx", clientBase, clientModuleInfo.SizeOfImage);
    uintptr_t dwViewMatrix = PatternScanner::ScanSafe("\x0F\x10\x05\x00\x00\x00\x00\x8D\x85\x00\x00\x00\x00\xB9", "xxx????xx????x", clientBase, clientModuleInfo.SizeOfImage);
    uintptr_t dwForceJump = PatternScanner::ScanSafe("\x8B\x0D\x00\x00\x00\x00\x8B\xD6\x8B\xC1\x83\xCA\x02", "xx????xxxxxxx", clientBase, clientModuleInfo.SizeOfImage);
    
    if (!dwEntityList || !dwLocalPlayer || !dwViewMatrix || !dwForceJump) CMemoryManager::UninjectDLL();

    offsets::m::dwEntityList = (*(uintptr_t*)(dwEntityList + 1)) - clientBase;
    offsets::m::dwLocalPlayer = (*(uintptr_t*)(dwLocalPlayer + 3)) - clientBase + 4;
    offsets::m::dwViewMatrix = (*(uintptr_t*)(dwViewMatrix + 3)) - clientBase + 176;
    offsets::m::dwForceJump = (*(uintptr_t*)(dwForceJump + 2)) - clientBase;

    uintptr_t dwClientState = PatternScanner::ScanSafe("\xA1\x00\x00\x00\x00\x33\xD2\x6A\x00\x6A\x00\x33\xC9\x89\xB0", "x????xxxxxxxxxx", engineBase, engineModuleInfo.SizeOfImage);
    uintptr_t dwClientState_ViewAngles = PatternScanner::ScanSafe("\xF3\x0F\x11\x86\x00\x00\x00\x00\xF3\x0F\x10\x44\x24\x00\xF3\x0F\x11\x86", "xxxx????xxxxx?xxxx", engineBase, engineModuleInfo.SizeOfImage);
    uintptr_t clientstate_delta_ticks = PatternScanner::ScanSafe("\xC7\x87\x00\x00\x00\x00\x00\x00\x00\x00\xFF\x15\x00\x00\x00\x00\x83\xC4\x08", "xx????????xx????xxx", engineBase, engineModuleInfo.SizeOfImage);

    if (!dwClientState || !dwClientState_ViewAngles || !clientstate_delta_ticks) CMemoryManager::UninjectDLL();

    offsets::m::dwClientState = (*(uintptr_t*)(dwClientState + 1)) - engineBase;
    offsets::m::dwClientState_ViewAngles = (*(uintptr_t*)(dwClientState_ViewAngles + 4));
    offsets::o::clientstate_delta_ticks = (*(uintptr_t*)(clientstate_delta_ticks + 2));
}

void Hack::MainLoop()
{
    while (!GetAsyncKeyState(VK_END))
    {
        Sleep(200);
    }
}