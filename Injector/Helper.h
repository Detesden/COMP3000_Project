#pragma once

#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
#include <Windows.h>
#include <TlHelp32.h>
#include <iostream>
#include <filesystem>
#include <conio.h>
#include <codecvt>


std::string stringDLLPath;
std::wstring stringProcessName;


DWORD GetProcessIDEx(const wchar_t* processName)
{
	DWORD processID{};
	HANDLE snapshotHandle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (snapshotHandle != INVALID_HANDLE_VALUE)
	{
		PROCESSENTRY32 processEntry;
		processEntry.dwSize = sizeof(processEntry);

		if (Process32First(snapshotHandle, &processEntry))
		{
			do
			{
				// _wcsicmp is case INSENSITIVE, it first converts both strings to lowercase
				// wcscmp is case SENSITIVE, compares the strings as is
				if (!_wcsicmp(processEntry.szExeFile, processName))
				{
					processID = processEntry.th32ProcessID;
					break;
				}
			} while (Process32Next(snapshotHandle, &processEntry));
		}
	}

	CloseHandle(snapshotHandle);

	return processID;
}

void PrepareProcessAndDLLNames()
{
	std::cout <<
		"Please make sure of the following:\n"
		"[+] CS:GO is up and running.\n"
		"[+] The DLL is in the same folder as the Injector.\n\n\n";

	std::string relativeDLLPath = "Cheat_DLL.dll";
	stringProcessName = L"csgo.exe";

	std::cout <<
		"DLL name: " << relativeDLLPath << "\n"
		"Process name: " << std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(stringProcessName) << "\n\n\n"

		"Press the respective number to perform the following actions:\n\n"

		"1. Continue with the default DLL and Process name.\n"
		"2. Change them.\n\n";

	char option = 0;
	while (true)
	{
		option = _getch();

		if (option == '1')
		{
			break;
		}
		else if (option == '2')
		{
			std::cout << "DLL name: "; std::getline(std::cin, relativeDLLPath);
			std::wcout << L"Process name: "; std::getline(std::wcin, stringProcessName);

			break;
		}
	}

	std::filesystem::path absoluteDLLPath = std::filesystem::absolute(std::filesystem::path(relativeDLLPath));
	stringDLLPath = absoluteDLLPath.string();
}

void CloseCountdown()
{
	system("cls");
	std::cout << "The DLL has been successfully injected!\n\n"
		"Closing the injector in ";

	for (int second = 5; second > 0; --second)
	{
		std::cout << second << "...";
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
}