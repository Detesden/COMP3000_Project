#include "pch.h"
#include "CHooks.h"


void CHooks::Initialize(BYTE* dst, BYTE len, BYTE* src)
{
    CHooks::src = src;
    CHooks::dst = dst;
    CHooks::len = len;
}

bool CHooks::PlaceHook()
{
    if (len < 5)
        return false;

    DWORD currProtect;
    VirtualProtect(src, len, PAGE_EXECUTE_READWRITE, &currProtect);

    memset(src, 0x90, len);

    uintptr_t relAddress = dst - src - 5;
    *src = 0xE9;
    *(uintptr_t*)(src + 1) = relAddress;

    VirtualProtect(src, len, currProtect, &currProtect);

    return true;
}

bool CHooks::CreateGateway()
{
    if (len < 5)
        return false;

    BYTE* gatewayBaseAddr = (BYTE*)VirtualAlloc(nullptr, len + 5, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    Gateway = (toHook_t)gatewayBaseAddr;

    memcpy_s(gatewayBaseAddr, len, src, len);

    uintptr_t gatewayRelAddr = src - gatewayBaseAddr - 5;
    *(gatewayBaseAddr + len) = 0xE9;
    *(uintptr_t*)((uintptr_t)gatewayBaseAddr + len + 1) = gatewayRelAddr;

    return true;
}

bool CHooks::Trampoline()
{
    // If either of the functions return false
    return !CreateGateway() || !PlaceHook() ? false : true;
}