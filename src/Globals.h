#pragma once
#include "CVector.h"


class C_BaseEntity;
namespace Globals
{
	// Module handles
	inline HMODULE ModuleHandle; // The handle to our own dll
	inline uintptr_t clientBase;
	inline uintptr_t engineBase;


	inline FILE* File;

	inline C_BaseEntity* pLocalPlayer = nullptr;

	inline constexpr double PI = std::numbers::pi;

	// Flag related stuff:
	inline constexpr int BONE_USED_BY_HITBOX = 256;
	inline constexpr int MAXSTUDIOBONES = 256;

	inline constexpr int ON_GROUND = (1 << 0);
	inline constexpr int JUMP = 5;
	inline constexpr int SHOOT = 5;
	inline constexpr int RESET = 4;
}