/*
 *   IMPORTANT - If the DLL is x32, the Injector must alos be x32.
*/

#include "Helper.h"

int main()
{
	PrepareProcessAndDLLNames();

	const char* dllPath = stringDLLPath.c_str();
	const wchar_t* processName = stringProcessName.c_str();

	DWORD processID{};

	while (true)
	{
		processID = GetProcessIDEx(processName);
		if (processID == 0)
		{
			std::cout << "Couldn't find the CS:GO process.\n";
			std::this_thread::sleep_for(std::chrono::seconds(1));
		}
		else break;
	}

	HANDLE processHandle = OpenProcess(PROCESS_ALL_ACCESS, 0, processID);
	if (processHandle == nullptr)
	{
		std::cout << "Couldn't open a handle to the CS:GO process.\n";
		return -1;
	}

	// Allocate 'strlen(dllPath) + 1' bytes of memory in the target process. It is the length of the DLL path along with the null terminator.
	void* allocatedMemBaseAddr = VirtualAllocEx(processHandle, 0, strlen(dllPath) + 1, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

	if (allocatedMemBaseAddr != nullptr)
	{
		// Write the dllPath's bytes/characters inside the allocated memory
		WriteProcessMemory(processHandle, allocatedMemBaseAddr, dllPath, strlen(dllPath) + 1, nullptr);

		// CreateThread - create a thread in the current process (the actual DLL cheat will call this)
		// CreateRemoteThread - create a thread in an external process through a handle to it
		// 
		// Creates a thread that will call LoadLibraryA with the pointer to the DLL name string (allocatedMemBaseAddr). Just like that, the DLL will be loaded/injected inside the target process.
		// The DLL's name and its address (allocatedMemBaseAddr) must be in the target process, because LoadLibraryA executes from within the process; it wouldn't be able to get the path from other address spaces other than its own.
		HANDLE threadHandle = CreateRemoteThread(processHandle, 0, 0, (LPTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandleA("kernel32.dll"), "LoadLibraryA"), allocatedMemBaseAddr, 0, 0);

		if (threadHandle != nullptr)
		{
			WaitForSingleObject(threadHandle, INFINITE); // Wait for the remote thread's execution to finish (which simply calls LoadLibraryA(allocatedMemBaseAddr);)
			CloseCountdown();
			CloseHandle(threadHandle);
		}

		VirtualFreeEx(processHandle, allocatedMemBaseAddr, 0, MEM_RELEASE); // Free the memory allocated inside the target process that contains the dllPath
	}

	CloseHandle(processHandle);

	return 0;
}