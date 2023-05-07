/*
*	Generally an AimBot can be achieved in two ways:
*	1. Changing the in game View Angles of the player.
*	2. Simulating mouse movement to raw coordinates on the screen.
*	
*	We will use the first method because it's more accurate and more performant.
*	We need:
*	The player's view angles.
*	The enemies' head (or whatever part) position 
*	
*	1. CS:GO Saves the view angles in degrees
*	
*	Coordinate system reminder:
*	Source Engine uses a right-handed, with Z -> Up coordinate system
*/

#pragma once

#include "C_BaseEntity.h"

class AimBot
{
public:
	static C_BaseEntity* GetClosestEntity();
	static void AimAt(unsigned short boneIndex = 8);
};