#pragma once
#include "pch.h"


class PatternScanner
{
public:
    static uintptr_t Scan(const char* pattern, const std::string& mask, uintptr_t moduleBase, size_t moduleSize)
    {
        size_t patternLength = mask.length(); // total amount of bytes in the pattern

        for (size_t i = 0; i < moduleSize; i++) // loop till moduleSize - patternLength ?
        {
            bool found = true;

            for (size_t j = 0; j < patternLength; j++)
            {
                // Keep iterating if the current mask byte is unknown || the current known byte in the pattern is equal to the byte being traversed
                if (mask[j] == '?' || pattern[j] == *(char*)(moduleBase + i + j))
                    continue;
                else // Reset
                {
                    found = false;
                    break;
                }
            }

            if (found) // If this is true, it means that the 'j' loop traversed through the entire pattern
                return moduleBase + i;
        }

        return 0;
    }

    // Scans only good memory. VirtualQuery retrieves an MBI containing the state of a memory region.
    // Non commit memory is bad (MBI.State != MEM_COMMIT). Memory with no access is also useless (MBI.Protect == PAGE_NOACCESS)
    static uintptr_t ScanSafe(const char* pattern, const std::string& mask, uintptr_t moduleBase, size_t moduleSize)
    {
        MEMORY_BASIC_INFORMATION MBI{};
        uintptr_t patternAddress{};
        uintptr_t moduleEndAddress = moduleBase + moduleSize;

        for (size_t currentRegion = moduleBase; currentRegion < moduleEndAddress; currentRegion += MBI.RegionSize)
        {
            if (!VirtualQuery((void*)currentRegion, &MBI, sizeof(MBI)) || MBI.State != MEM_COMMIT || MBI.Protect == PAGE_NOACCESS)
                continue;

            patternAddress = Scan(pattern, mask, currentRegion, MBI.RegionSize);

            if (patternAddress != 0)
                break;
        }

        return patternAddress;
    }
};