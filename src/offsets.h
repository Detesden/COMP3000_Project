#pragma once
#include <cstddef>

namespace offsets
{
	// player
	namespace p
	{
		inline constexpr ptrdiff_t m_iHealth = 0x100;
		inline constexpr ptrdiff_t m_iTeamNum = 0xF4;
		inline constexpr ptrdiff_t m_fFlags = 0x104;
		inline constexpr ptrdiff_t m_bDormant = 0xED;
		inline constexpr ptrdiff_t m_hMyWeapons = 0x2E08;
		inline constexpr ptrdiff_t m_vecOrigin = 0x138;
		inline constexpr ptrdiff_t m_vecViewOffset = 0x108;
		inline constexpr ptrdiff_t m_flFlashDuration = 0x10470;
		inline constexpr ptrdiff_t m_flFlashMaxAlpha = 0x1046C;
		inline constexpr ptrdiff_t m_iCrosshairId = 0x11838;
	}
	// module
	namespace m
	{
		inline ptrdiff_t dwEntityList = 0x0;
		inline ptrdiff_t dwLocalPlayer = 0x0;
		inline ptrdiff_t dwViewMatrix = 0x0;
		inline ptrdiff_t dwForceJump = 0x0;

		inline ptrdiff_t dwClientState = 0x0;
		inline ptrdiff_t dwClientState_ViewAngles = 0x0;
	}
	// other
	namespace o
	{
		inline constexpr ptrdiff_t m_Item = 0x40;
		inline constexpr ptrdiff_t m_AttributeManager = 0x2D90;
		inline constexpr ptrdiff_t m_iItemDefinitionIndex = 0x1EA;
		inline constexpr ptrdiff_t m_iItemDefinitionIndexTotal = 0x2FBA; // The addition of all 3 offsets above it

		inline constexpr ptrdiff_t m_iItemIDHigh = 0x2FD0;
		inline constexpr ptrdiff_t m_nFallbackPaintKit = 0x31D8;
		inline constexpr ptrdiff_t m_flFallbackWear = 0x31E0;

		inline ptrdiff_t clientstate_delta_ticks = 0x0;
	}
}