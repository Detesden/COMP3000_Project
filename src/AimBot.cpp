#include "pch.h"
#include "AimBot.h"

#include <cmath>

#include "ImGui/imgui.h"

#include "Globals.h"
#include "offsets.h"
#include "Interfaces.h"
#include "Miscellaneous.h"
#include "CVector.h"
#include "GUI.h"



C_BaseEntity* AimBot::GetClosestEntity()
{
    static float closestDistance{};
    static float currentDistance{};
    static C_BaseEntity* closestEntity = nullptr;
    static C_BaseEntity* currentEntity = nullptr;


    closestDistance = (std::numeric_limits<float>::max)();
    closestEntity = nullptr;

    for (unsigned short i = 2; i < 64; i++) // Index 1 is the player
    {
        currentEntity = IClientEntityList_->GetClientEntity(i);
        if (!IsEntityValid(currentEntity)) continue;

        currentDistance = CVector::GetDistance(Globals::pLocalPlayer->GetAbsOrigin(), currentEntity->GetAbsOrigin());

        if (currentDistance < closestDistance)
        {
            closestDistance = currentDistance;
            closestEntity = currentEntity;
        }
    }

    return closestEntity;
}

void AimBot::AimAt(unsigned short boneIndex)
{
    // the Yaw, Pitch & Roll angles. They're from the engine.dll and are available no matter wether local player is valid (cuz they work even when spectating). When not in any game, they're 0, 0, 0.
    static CVector* playerViewAngles = (CVector*)(*(uintptr_t*)(Globals::engineBase + offsets::m::dwClientState) + offsets::m::dwClientState_ViewAngles);
    static C_BaseEntity* closestEntity = nullptr;
    static CVector playerCameraPos, entityBoneTargetPos, deltaVec;
    static float playerPitch, playerYaw;

    if (GUI::Options::Aimbot::Hotkey == 0 && !ImGui::IsMouseDown(ImGuiMouseButton_Left)) return;
    if (GUI::Options::Aimbot::Hotkey == 1 && !ImGui::IsKeyDown(ImGuiKey_LeftAlt)) return;
    //if (!GetAsyncKeyState(VK_LBUTTON)) return;

    Globals::pLocalPlayer = IClientEntityList_->GetClientEntity(1); if (Globals::pLocalPlayer == nullptr) return;
    closestEntity = GetClosestEntity(); if (closestEntity == nullptr) return;

    // Getting the player's absolute origin in the world (the feet) and adding the viewOffset (the offset from the feet to get to the eye position) -> would return us the camera position of our player (that is in front of our eyes).
    // We could also get the 'head' bone position of our player which is 'kinda' similar. The difference hat the hea bone position is literally in the middle of the head model, while the camera postion is outside our model at the eye level.
    // So the world space coordinates of the both would look something like this: 
    // origin           { x = -316.507 y = 2068.166 z = -127.687 }
    // viewOffset       { x =  0.00    y = 0.00     z =  64.062 }
    // playerCameraPos  { x = -316.507 y = 2068.166 z = -63.624 } Camera pos -> more accurate to calculate the angles from it
    // playerHeadPos    { x = -308.994 y = 2070.147 z = -63.362 } Head pos (using GetBonePosition(8))
    playerCameraPos = (Globals::pLocalPlayer->GetAbsOrigin() + Globals::pLocalPlayer->m_vecViewOffset); // Can just add the '.z' value
    entityBoneTargetPos = closestEntity->GetBonePosition(boneIndex);

    deltaVec = { entityBoneTargetPos.x - playerCameraPos.x, entityBoneTargetPos.y - playerCameraPos.y, entityBoneTargetPos.z - playerCameraPos.z };

    // [We're prefixing the asin function with a '-' because in CS:GO, the positive pitch is facing down instead of up (-89 -> all the way up | 89 -> all the way down)
    playerPitch = -std::asin(deltaVec.z / CVector::Length(deltaVec)) * (180 / Globals::PI); // (Head height difference / distance between our head and enemy head) * Convert radians to degrees
    playerYaw = std::atan2(deltaVec.y, deltaVec.x) * (180 / Globals::PI); // (Depth difference / horizontal position difference) * Convert radians to degrees

    if (playerPitch >= -89 && playerPitch <= 89 && playerYaw >= -180 && playerYaw <= 180) // Make sure to not set the angles beyond the game's engine limits, otherwise it's a red flag for the anti-cheat.
    {
        playerViewAngles->x = playerPitch;
        playerViewAngles->y = playerYaw;
    }
}