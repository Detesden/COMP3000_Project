#pragma once
#include "pch.h"

#include "Globals.h"
#include "offsets.h"

enum CSGO_Weapon_ID
{
    WEAPON_DEAGLE = 1,
    WEAPON_GLOCK = 4,
    WEAPON_AK47 = 7,
    WEAPON_AWP = 9,
    WEAPON_M4A4 = 16,
    WEAPON_M4A1_SILENCER = 60,
    WEAPON_USP_SILENCER = 61,
};


static void SkinChanger()
{
	uintptr_t localPlayer = *(uintptr_t*)(Globals::clientBase + offsets::m::dwLocalPlayer);

	static float factoryNewWear = 0.00001f;

	if (localPlayer)
	{
		for (int i = 0; i < 8; i++) // Loop through the weapons in the player's inventory (4 nades + 3 guns + bomb)
		{
			int curWpnHnd = *(int*)(localPlayer + offsets::p::m_hMyWeapons + i * 0x4) & 0xFFF; // Get the weapon that is currently being held
			curWpnHnd = *(int*)(Globals::clientBase + offsets::m::dwEntityList + (curWpnHnd - 1) * 0x10); // Find the weapon in the entity list

			if (curWpnHnd != 0) //if the weapon is valid
			{
				short curWpnHandID = *(short*)(curWpnHnd + offsets::o::m_iItemDefinitionIndexTotal); // Get the weapon ID of the current weapon

				if (curWpnHandID == WEAPON_DEAGLE)
				{
					*(int*)(curWpnHnd + offsets::o::m_iItemIDHigh) = -1; // Force the game to use the fallback values
					*(int*)(curWpnHnd + offsets::o::m_nFallbackPaintKit) = GUI::skins[GUI::Options::SkinChanger::Deagle_Skin].ID;
				}
				else if (curWpnHandID == WEAPON_GLOCK)
				{
					*(int*)(curWpnHnd + offsets::o::m_iItemIDHigh) = -1;
					*(int*)(curWpnHnd + offsets::o::m_nFallbackPaintKit) = GUI::skins[GUI::Options::SkinChanger::Glock_Skin].ID;
				}
				else if (curWpnHandID == WEAPON_AK47)
				{
					*(int*)(curWpnHnd + offsets::o::m_iItemIDHigh) = -1;
					*(int*)(curWpnHnd + offsets::o::m_nFallbackPaintKit) = GUI::skins[GUI::Options::SkinChanger::AK47_Skin].ID;
				}
				else if (curWpnHandID == WEAPON_AWP)
				{
					*(int*)(curWpnHnd + offsets::o::m_iItemIDHigh) = -1;
					*(int*)(curWpnHnd + offsets::o::m_nFallbackPaintKit) = GUI::skins[GUI::Options::SkinChanger::AWP_Skin].ID;
				}
				else if (curWpnHandID == WEAPON_M4A4)
				{
					*(int*)(curWpnHnd + offsets::o::m_iItemIDHigh) = -1;
					*(int*)(curWpnHnd + offsets::o::m_nFallbackPaintKit) = GUI::skins[GUI::Options::SkinChanger::M4A4_Skin].ID;
				}
				else if (curWpnHandID == WEAPON_M4A1_SILENCER)
				{
					*(int*)(curWpnHnd + offsets::o::m_iItemIDHigh) = -1;
					*(int*)(curWpnHnd + offsets::o::m_nFallbackPaintKit) = GUI::skins[GUI::Options::SkinChanger::M4A4S_Skin].ID;
				}
				else if (curWpnHandID == WEAPON_USP_SILENCER)
				{
					*(int*)(curWpnHnd + offsets::o::m_iItemIDHigh) = -1;
					*(int*)(curWpnHnd + offsets::o::m_nFallbackPaintKit) = GUI::skins[GUI::Options::SkinChanger::USP_Skin].ID;
				}

				*(float*)(curWpnHnd + offsets::o::m_flFallbackWear) = factoryNewWear;
			}
		}
	}
}