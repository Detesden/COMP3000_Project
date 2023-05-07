// Source Engine prefix style:
// C_ for client classes
// C  for server classes


// C_BaseEntity inherits from IClientEntity(which is the interface for a client entity, remember), and it acts like a base class with its own members(netvars and other variables).
// The inheritance looks like this:
// IClientUnknown, IClientRenderable, IClientNetworkable, IClientThinkable -> IClientEntity -> C_BaseEntity -> C_BaseAnimating -> C_BaseAnimatingOverlay (animation layers are here!) -> C_BaseFlex -> C_BaseCombatCharacter -> C_BasePlayer -> C_CSPlayer
// Ofc this is just for players. For weapons, the inheritance looks the same until we reach C_BaseFlex. From there:
// C_BaseFlex -> C_EconEntity -> C_BaseCombatWeapon -> C_WeaponCSBase (we can stop here for normal weapons, for grenades:) -> C_BaseCSGrenade


#pragma once
#include "Windows.h"

#include "CMatrix.h"
#include "CMemoryManager.h"
#include "Globals.h"
#include "offsets.h"



// Auto padding for variables inside the class
#define MERGE_STRING_IMPLEMENTATION(a, b) a##b
#define MERGE_STRING(a, b) MERGE_STRING_IMPLEMENTATION(a, b)
#define MAKE_PADDING(size) MERGE_STRING(_pad, __COUNTER__)[size]
#define MEMBER_VARIABLE(type, name, offset) struct {unsigned char MAKE_PADDING(offset); type name;}


class C_BaseEntity
{
public:

    /* ----------------------------------------------------------------------------------------------*
     *                       C_BaseEntity's VTable is at offset -0x0-                                *
     * ----------------------------------------------------------------------------------------------*/

    CVector& GetAbsOrigin() // entity->m_vecOrigin
    {
        return CMemoryManager::CallVirtualFunc<CVector&>(this, 10);
    }

    int32_t GetHealth() // entity->m_iHealth        [Better off reading the 'm_iHealth' offset than calling a game's function]
    {
        return CMemoryManager::CallVirtualFunc<int32_t>(this, 122);
    }

    bool IsAlive()
    {
        return CMemoryManager::CallVirtualFunc<bool>(this, 156);
    }


    /* ----------------------------------------------------------------------------------------------*
     *                       IClientRenderable's VTable is at offset -0x4-                           *
     * ----------------------------------------------------------------------------------------------*/

    /// <summary> Returns the current bone transforms for the model. </summary>
    /// <param name="nMaxBones">Specifies how many matrices pBonesToWorldOut can hold. Use MAXSTUDIOBONES to be safe.</param>
    /// <param name="currentTime">Affects interpolation</param>
    bool SetupBones(CMatrix3x4* pBonesToWorldOut, int32_t nMaxBones, int32_t boneMask, float currentTime)
    {
        return CMemoryManager::CallVirtualFunc<bool>((void*)((uintptr_t)this + 0x4), 13, pBonesToWorldOut, nMaxBones, boneMask, currentTime);
    }


    /* ----------------------------------------------------------------------------------------------*
     *                       IClientNetworkable's VTable is at offset -0x8-                          *
     * ----------------------------------------------------------------------------------------------*/

    bool IsDormant() // Better off reading the 'm_bDormant' offset than calling a game's function
    {
        return CMemoryManager::CallVirtualFunc<bool>((void*)((uintptr_t)this + 0x8), 9);
    }


    /* ----------------------------------------------------------------------------------------------*
     *                              Helper / test functions                                          *
     * ----------------------------------------------------------------------------------------------*/

    int& GetHealth2() const // Getting the health using a function, the same as doing 'entity->m_iHealth'
    {
        return *(int*)((uintptr_t)this + offsets::p::m_iHealth);
    }

    CVector GetBonePosition(unsigned short boneIndex)
    {
        CMatrix3x4 bones[256];
        if (!SetupBones(bones, Globals::MAXSTUDIOBONES, Globals::BONE_USED_BY_HITBOX, 0.0f)) return CVector{};

        return bones[boneIndex].Origin();
    }

    union
    {
        MEMBER_VARIABLE(bool,           m_bDormant,             offsets::p::m_bDormant);
        MEMBER_VARIABLE(int32_t,        m_iTeamNum,             offsets::p::m_iTeamNum);
        MEMBER_VARIABLE(int32_t,        m_iHealth,              offsets::p::m_iHealth);
        MEMBER_VARIABLE(int32_t,        m_fFlags,               offsets::p::m_fFlags);
        MEMBER_VARIABLE(float,          m_flFlashDuration,      offsets::p::m_flFlashDuration);
        MEMBER_VARIABLE(float,          m_flFlashMaxAlpha,      offsets::p::m_flFlashMaxAlpha);
        MEMBER_VARIABLE(CVector,        m_vecViewOffset,        offsets::p::m_vecViewOffset);
        MEMBER_VARIABLE(CVector,        m_vecOrigin,            offsets::p::m_vecOrigin);
    };
};

// https://github.com/ValveSoftware/source-sdk-2013/blob/master/sp/src/public/icliententitylist.h#L28
class IClientEntityList
{
public:
    // Get IClientNetworkable interface for specified entity
    virtual void* GetClientNetworkable(int entnum) = 0;
    virtual void* GetClientNetworkableFromHandle(unsigned long hEnt) = 0;
    virtual void* GetClientUnknownFromHandle(unsigned long hEnt) = 0;

    // NOTE: This function is only a convenience wrapper.
    // It returns GetClientNetworkable( entnum )->GetIClientEntity().
    virtual C_BaseEntity* GetClientEntity(int entnum) = 0;
    virtual C_BaseEntity* GetClientEntityFromHandle(unsigned long hEnt) = 0;

    // Returns number of entities currently in use
    virtual int NumberOfEntities(bool bIncludeNonNetworkable) = 0;

    // Returns the highest index actually used
    virtual int GetHighestEntityIndex() = 0;

    // Sizes entity list to specified size
    virtual void SetMaxEntities(int maxents) = 0;
    virtual int GetMaxEntities() = 0;
};