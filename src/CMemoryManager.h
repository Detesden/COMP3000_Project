#pragma once
#include <vector>
#include <Psapi.h>


class CMemoryManager
{
public:
	static MODULEINFO GetModuleInfo(uintptr_t moduleHandle);
	static uintptr_t GetModule(LPCSTR moduleName);
	static FARPROC GetExportedFuncAddr(LPCSTR dllName, LPCSTR funcName);

	template <typename Return, typename ... Arguments>
	static Return CallVirtualFunc(void* thisptr, const uint16_t funcIndex, Arguments ... args);
	static void* GetVirtualFuncAddr(void* thisptr, const uint16_t funcIndex);

	static void Patch(BYTE* dst, BYTE* src, unsigned int size);
	static void Nop(BYTE* dst, unsigned int size);

	static void RaiseError(const char* errorDescription);
	static void UninjectDLL(DWORD exitCode = 0);
};



template <typename Return, typename ... Arguments>
Return CMemoryManager::CallVirtualFunc(void* thisptr, const uint16_t funcIndex, Arguments ... args)
{
	using VirtualFunction = Return(__thiscall*)(void*, decltype(args)...);

	return (*(VirtualFunction**)thisptr)[funcIndex](thisptr, args...); // In one line

	// Broken down:
	//void** VTable = *(void***)thisptr; // Getting the VTable normally
	//VirtualFunction* VTable2 = *(VirtualFunction**)thisptr; // This also works because:
	/* VirtualFunction is already a pointer(to a function).Thus, making a cast to(VirtualFunction) is the same as(void*) therefore(VirtualFunction**) == (void***)
	   VTable2 is therefore an array of pointers to functions of the VirtualFunction type (which is not logically correct, because only 1 function in that array matches that prototype)
	   But it works because at the end of the day they're all addresses. */

	//VirtualFunction functionAddress = (VirtualFunction)VTable[funcIndex];
	//VirtualFunction functionAddress2 = VTable2[funcIndex];

	//return functionAddress(thisptr, args...);
}