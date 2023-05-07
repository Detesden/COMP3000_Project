#include "pch.h"
#include "TriggerBot.h"

#include "Globals.h"
#include "Interfaces.h"
#include "C_BaseEntity.h"
#include "Miscellaneous.h"


void TriggerBot::Run()
{
	Globals::pLocalPlayer = IClientEntityList_->GetClientEntity(1); if (Globals::pLocalPlayer == nullptr) return;

	uintptr_t entityID = *(uintptr_t*)((uintptr_t)Globals::pLocalPlayer + offsets::p::m_iCrosshairId); // Returns the ID of the entity that hits our trace line (could be w/e entity, such as players, weapons, chickens, etc.)
	C_BaseEntity* entity = *(C_BaseEntity**)(Globals::clientBase + offsets::m::dwEntityList + ((entityID - 1) * 0x10));

	if (entityID != 0 && entityID < 64 && IsEntityValid(entity)) // 0 = pointing at no entity. The first 64 entities are always reserved for players
	{
		// This happens too fast, but if you put a sleep between them, then you're sleeping the game's thread which is very bad, so figure out another way.
		//*(uintptr_t*)(Globals::clientBase + offsets::m::dwForceAttack) = Globals::SHOOT;
		// Sleep(5); // sleeps the game's thread - BAD
		//*(uintptr_t*)(Globals::clientBase + offsets::m::dwForceAttack) = Globals::RESET;


		// The key simulation versions, obviously simulate key presses, so put some guards, wether the game is focused, etc. Otherwise it could send clicks everywhere.

		mouse_event(MOUSEEVENTF_LEFTDOWN, NULL, NULL, NULL, NULL);
		mouse_event(MOUSEEVENTF_LEFTUP, NULL, NULL, NULL, NULL);


		// Non-deprecated 'SendInput' version

		//INPUT inputs[2] = {};

		//inputs[0].type = INPUT_MOUSE;
		//inputs[0].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
		//inputs[1].type = INPUT_MOUSE;
		//inputs[1].mi.dwFlags = MOUSEEVENTF_LEFTUP;

		//SendInput(2, inputs, sizeof(INPUT));
	}
}