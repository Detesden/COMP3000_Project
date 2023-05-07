/*
*  To draw an ESP box :
*  1. The most accurate way is to hook the game's 'Collideable' interface that contains the entity's OBB(Oriented Bounding Box), therefore it's min & max point
*  2. The ghetto way is to get the entity's position(origin), then from that point -> add hard coded offsets to form a somewhat accurate bounding box.
*  
*  We will use the ghetto method.
*  There are two options to get an entity's origin:
*  1) Using the 'm_vecOrigin' netvar								-> Returns the last position of the feet from the server (uninterpolated data),    meaning that it's updated once per tick (64 in MM currently = 64 / second)
*  2) Using the function 'virtual const Vector3& GetAbsOrigin()'	-> Returns the last position of the feet from the client (uses interpolated data), meaning that it's updated once per frame (thus having more than 64 FPS, makes this variant less jittery and better)
*/


#pragma once

#include "Globals.h"
#include "CMatrix.h"
struct ImVec2;


class ESP
{
public:
	static inline CMatrix ViewMatrix{};

	static void Render();

	/// <summary> Some trigonometry calculations that translates a 3D position from the world to a 2D positon on the screen.</summary>
	/// <param name="vecOrigin"> - The world 3D coordinates of the entity to be translated into 2D coordinates </param>
	/// <param name="viewMatrix"> - The player's view matrix</param>
	/// <param name="vecScreenCoordsOut"> - The returned 2D coordinates of the entity that is in the screen space</param>
	/// <returns> True if the translated coordinates are within the FOV of the player, therefore, fit/visible on the screen.</returns>
	static bool WorldToScreen(const CVector& vecOrigin, float* viewMatrix, const ImVec2& windowCenter, ImVec2& vecScreenCoordsOut);
};