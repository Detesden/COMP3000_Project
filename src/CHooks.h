#pragma once

using toHook_t = BOOL(__stdcall*)(HDC);


class CHooks
{
private:
	// This is also toHook but BYTE* type
	static inline BYTE* src;
	static inline BYTE* dst;
	static inline BYTE len;

public:
	static inline toHook_t Gateway;
	static inline toHook_t toHook;

	// Make sure to obtain the address of toHook before calling this
	static void Initialize(BYTE* dst, BYTE len, BYTE* src = (BYTE*)toHook);

	static bool PlaceHook();
	static bool CreateGateway();
	static bool Trampoline();
};