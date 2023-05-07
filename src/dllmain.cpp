#include "pch.h"

#include "Hack.h"
#include "Globals.h"
#include "CMemoryManager.h"


void WINAPI MainThread()
{
    //system("taskkill /F /T /IM steam.exe");
    //ShellExecute(NULL, "open", "steam://rungameid/730", NULL, NULL, SW_SHOWDEFAULT);

    Hack::Initialize();

    Hack::MainLoop();

    CMemoryManager::UninjectDLL();
}

extern "C" BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
    {
        Globals::ModuleHandle = hModule;
        DisableThreadLibraryCalls(hModule); // DLL_THREAD_ATTACH and DLL_THREAD_DETACH are not used, so just disable them to reduce the size of the working set.

        // Create the hack thread
        HANDLE threadHandle = CreateThread(nullptr, 0, (PTHREAD_START_ROUTINE)MainThread, nullptr, 0, nullptr);

        if (threadHandle) // Release the handle
            CloseHandle(threadHandle);
    }

    return TRUE;
}