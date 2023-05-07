#include "pch.h"
#include "CMemoryManager.h"

#include "Globals.h"
#include "CHookz.h"


MODULEINFO CMemoryManager::GetModuleInfo(uintptr_t moduleHandle)
{
	// https://learn.microsoft.com/en-us/windows/win32/api/psapi/nf-psapi-getmoduleinformation
	MODULEINFO moduleInfo{};
	K32GetModuleInformation(GetCurrentProcess(), (HMODULE)moduleHandle, &moduleInfo, sizeof(MODULEINFO));

	return moduleInfo;
}

uintptr_t CMemoryManager::GetModule(LPCSTR moduleName)
{
    return (uintptr_t)GetModuleHandleA(moduleName);
}

FARPROC CMemoryManager::GetExportedFuncAddr(LPCSTR dllName, LPCSTR funcName)
{
	return GetProcAddress(GetModuleHandleA(dllName), funcName);
}


void* CMemoryManager::GetVirtualFuncAddr(void* thisptr, const uint16_t funcIndex)
{
	return (*(void***)thisptr)[funcIndex]; // In one line

	// We use the 'thisptr' of the Class to get its VTable.
	// Because the VTable is literally the first member of a Class (that has virtual functions / inherits from one that has them), i.e at offset 0x0
	// we can simply cast the Class to a double pointer which gives us the VTable array with the fucntions pointers. Then index into it to get the needed function.
	//void** VTable = *(void***)thisptr;

	//return VTable[funcIndex];
}

void CMemoryManager::Patch(BYTE* dst, BYTE* src, unsigned int size)
{
	DWORD currentProtect;
	VirtualProtect(dst, size, PAGE_EXECUTE_READWRITE, &currentProtect);
	memcpy(dst, src, size);
	VirtualProtect(dst, size, currentProtect, &currentProtect);
}

void CMemoryManager::Nop(BYTE* dst, unsigned int size)
{
	DWORD currentProtect;
	VirtualProtect(dst, size, PAGE_EXECUTE_READWRITE, &currentProtect);
	memset(dst, 0x90, size);
	VirtualProtect(dst, size, currentProtect, &currentProtect);
}

void CMemoryManager::RaiseError(const char* errorDescription)
{
    std::cout << errorDescription << std::endl;

    // REALEASE ROSURCES: handles and allocated memory
}

void CMemoryManager::UninjectDLL(DWORD exitCode)
{
	if (Globals::File) fclose(Globals::File);
	FreeConsole();

	Hookz::Destroy();
	GUI::Cleanup();
	FreeLibraryAndExitThread(Globals::ModuleHandle, exitCode); // Uninject the DLL by closing the handle
}