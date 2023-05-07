#pragma once

#include "ImGui/imgui.h"

#include "Globals.h"
#include "Interfaces.h"
#include "C_BaseEntity.h"


enum ClientFrameStage
{
	FRAME_UNDEFINED = -1,
	FRAME_START,
	FRAME_NET_UPDATE_START,
	FRAME_NET_UPDATE_POSTDATAUPDATE_START,
	FRAME_NET_UPDATE_POSTDATAUPDATE_END,
	FRAME_NET_UPDATE_END,
	FRAME_RENDER_START,
	FRAME_RENDER_END
};


inline void GameForceUpdate()
{
	uintptr_t dwClientState = *(uintptr_t*)(Globals::engineBase + offsets::m::dwClientState);
	*(int*)(dwClientState + offsets::o::clientstate_delta_ticks) = -1;

	// There is also a built in ForceUpdate function inside engine.dll that is much cleaner and doesnt lag the game as much
}

inline void AntiFlash()
{
	if (IClientEntityList_->GetClientEntity(1) != nullptr)
	{
		if (IClientEntityList_->GetClientEntity(1)->m_flFlashDuration > 0.f)
			IClientEntityList_->GetClientEntity(1)->m_flFlashDuration = 0.f;

		// m_flFlashMaxAlpha doesn't reset itself, so you can write to it once. To turn it off write the value 255 back.
		// IClientEntityList_->GetClientEntity(1)->m_flFlashMaxAlpha = 0.f;
	}
}

inline void BunnyHop()
{
	if (ImGui::IsKeyDown(ImGuiKey_Space))
	{
		if ((IClientEntityList_->GetClientEntity(1)->m_fFlags) & Globals::ON_GROUND)
			*(uintptr_t*)(Globals::clientBase + offsets::m::dwForceJump) = Globals::JUMP; // We can just set the flag once to '6' which will automatically reset itself, but it's kind of buggy
		else
			*(uintptr_t*)(Globals::clientBase + offsets::m::dwForceJump) = Globals::RESET; // Reset the flag back when we're in the air
	}
}

inline bool IsEntityValid(C_BaseEntity* entity, bool TeammateCountsAsInvalid = true)
{
	// Literally invalid || entity is the player || entity is dead || C_BaseEntity not responsive (his packets are not being sent to the server due to the distance)
	if (entity == nullptr || entity == IClientEntityList_->GetClientEntity(1) || entity->m_iHealth <= 0 || entity->m_bDormant) return false;
	if (TeammateCountsAsInvalid && entity->m_iTeamNum == IClientEntityList_->GetClientEntity(1)->m_iTeamNum) return false; // Same team

	return true;
}