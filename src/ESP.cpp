#include "pch.h"
#include "ESP.h"

#include "offsets.h"
#include "Globals.h"

#include "GUI.h"
#include "C_BaseEntity.h"
#include "Miscellaneous.h"



void ESP::Render()
{
	static ImVec2 screenCoords = {};

	// 2D Box
	static ImVec2 tl, tr, bl, br;

	// 3D Box
	static ImVec2 t1, t2, t3, t4, b1, b2, b3, b4;

	static CVector headPosition = {};
	static CVector feetPosition = {};


	//memcpy(&ESP::ViewMatrix, (void*)(Globals::clientBase + offsets::m::dwViewMatrix), sizeof(ESP::ViewMatrix)); // memcpy alternative. It will dereference the 'src' pointer by itself. IG it's worse because it's a function
	ESP::ViewMatrix = *(CMatrix*)(Globals::clientBase + offsets::m::dwViewMatrix);

	for (unsigned short i = 2; i < 64; i++) // Index 1 is the player
	{
		C_BaseEntity* entity = IClientEntityList_->GetClientEntity(i);
		if (!IsEntityValid(entity)) continue;

		GUI::debugPlayerPos = IClientEntityList_->GetClientEntity(1)->GetAbsOrigin();


		if (GUI::Options::ESP::SnapLine)
		{
			if (ESP::WorldToScreen(entity->m_vecOrigin, ESP::ViewMatrix.data, GUI::windowCenter, screenCoords))
				GUI::Draw::Line(GUI::windowBottom, screenCoords, IM_COL32(0, 255, 0, 255));
		}

		if (GUI::Options::ESP::EntityHealth)
		{
			headPosition = entity->GetBonePosition(8);

			if (!headPosition.isNull())
				if (ESP::WorldToScreen(headPosition, ESP::ViewMatrix.data, GUI::windowCenter, screenCoords))
					GUI::Draw::HealthArc(screenCoords, entity->m_iHealth);
		}

		if (GUI::Options::ESP::Box2D)
		{
			// This is bad because, it only uses 2 points from the game's coordinate space.
			// Therefore the other 2 points to connect the rectangle are calculated, but in screen space (which is in 2D).
			// Because of that, as you'll see the other 2 points will distort as you change your angle. Translating a 2D coordinate in 3D without the depth coordinate is impossible to do accurately.
			// Secondly, this box is not dynamic, meaning that the box will not face us no matter where the entity is facing. So even if we calculate the other 2 points (as in the 3D box) this will still remain bad.

			feetPosition = entity->GetAbsOrigin();

			if (!ESP::WorldToScreen(feetPosition + GUI::espBoxHeadOffset, ESP::ViewMatrix.data, GUI::windowCenter, tl)) continue;
			if (!ESP::WorldToScreen(feetPosition + GUI::espBoxFeetOffset, ESP::ViewMatrix.data, GUI::windowCenter, br)) continue;

			// These two are equivalent
			{
				GUI::DrawList->AddRect(tl, br, IM_COL32(0, 255, 0, 255));
			}
			{ // Manually calculate the other 2 points https://imgur.com/a/SqbEbvM
				//const float boxHeight = br.y - tl.y;
				//const float boxWidth = br.x - tl.x;

				//tr = { tl.x + boxWidth, tl.y };
				//bl = { br.x - boxWidth, br.y };

				//GUI::DrawList->AddLine(tl, tr, IM_COL32(0, 255, 0, 255));
				//GUI::DrawList->AddLine(tl, bl, IM_COL32(0, 255, 0, 255));
				//GUI::DrawList->AddLine(bl, br, IM_COL32(0, 255, 0, 255));
				//GUI::DrawList->AddLine(br, tr, IM_COL32(0, 255, 0, 255));
			}

			if (GUI::Options::ESP::debugESPBoxCorners)
			{
				GUI::DrawList->AddText(tl, IM_COL32(255, 255, 255, 255), "tl"); GUI::DrawList->AddText(br, IM_COL32(255, 255, 255, 255), "br");
				GUI::DrawList->AddCircle(tl, 4, IM_COL32(0, 255, 0, 255)); GUI::DrawList->AddCircle(br, 4, IM_COL32(0, 255, 0, 255));
			}
		}

		if (GUI::Options::ESP::Box3D)
		{
			// This is not ideal either, because it hard codes the bounding box, which kind of works. The actual bounding box changes shape in accordance to where the entity is facing, but this one is a static cube around the entity.
			// This starts out from the origin of the entity, which at the feet level (in between), then adding offsets to that in 8 different directions (4 above the entity's head & 4 below the entity's feet)
			// https://imgur.com/a/fktcA4K

			/*

			   .1------2	
			 .' |    .'|	t1-t4
			3---+--4'  |
			|   |  |   |
			|  ,1--+---2	
			|.'    | .'		b1-b4
			3------4'

			*/

			feetPosition = entity->GetAbsOrigin();

			// Make a W2S that takes a 'batch' of coordinates, to do it in 1 call instead of 8
			if (!ESP::WorldToScreen(feetPosition + GUI::t1, ESP::ViewMatrix.data, GUI::windowCenter, t1)) continue;
			if (!ESP::WorldToScreen(feetPosition + GUI::t2, ESP::ViewMatrix.data, GUI::windowCenter, t2)) continue;
			if (!ESP::WorldToScreen(feetPosition + GUI::t3, ESP::ViewMatrix.data, GUI::windowCenter, t3)) continue;
			if (!ESP::WorldToScreen(feetPosition + GUI::t4, ESP::ViewMatrix.data, GUI::windowCenter, t4)) continue;

			if (!ESP::WorldToScreen(feetPosition + GUI::b1, ESP::ViewMatrix.data, GUI::windowCenter, b1)) continue;
			if (!ESP::WorldToScreen(feetPosition + GUI::b2, ESP::ViewMatrix.data, GUI::windowCenter, b2)) continue;
			if (!ESP::WorldToScreen(feetPosition + GUI::b3, ESP::ViewMatrix.data, GUI::windowCenter, b3)) continue;
			if (!ESP::WorldToScreen(feetPosition + GUI::b4, ESP::ViewMatrix.data, GUI::windowCenter, b4)) continue;

			// Top Rectangle
			GUI::DrawList->AddLine(t1, t2, IM_COL32(0, 255, 0, 255)); GUI::DrawList->AddLine(t2, t4, IM_COL32(0, 255, 0, 255));
			GUI::DrawList->AddLine(t4, t3, IM_COL32(0, 255, 0, 255)); GUI::DrawList->AddLine(t3, t1, IM_COL32(0, 255, 0, 255));

			// Bot Rectangle
			GUI::DrawList->AddLine(b1, b2, IM_COL32(0, 255, 0, 255)); GUI::DrawList->AddLine(b2, b4, IM_COL32(0, 255, 0, 255));
			GUI::DrawList->AddLine(b4, b3, IM_COL32(0, 255, 0, 255)); GUI::DrawList->AddLine(b3, b1, IM_COL32(0, 255, 0, 255));

			// Connecting top to bottom
			GUI::DrawList->AddLine(t1, b1, IM_COL32(0, 255, 0, 255)); GUI::DrawList->AddLine(t2, b2, IM_COL32(0, 255, 0, 255));
			GUI::DrawList->AddLine(t3, b3, IM_COL32(0, 255, 0, 255)); GUI::DrawList->AddLine(t4, b4, IM_COL32(0, 255, 0, 255));


			if (GUI::Options::ESP::debugESPBoxCorners)
			{
				GUI::DrawList->AddText(t1, IM_COL32(255, 255, 255, 255), "t1"); GUI::DrawList->AddText(t2, IM_COL32(255, 255, 255, 255), "t2"); GUI::DrawList->AddText(t3, IM_COL32(255, 255, 255, 255), "t3"); GUI::DrawList->AddText(t4, IM_COL32(255, 255, 255, 255), "t4"); GUI::DrawList->AddText(b1, IM_COL32(255, 255, 255, 255), "b1"); GUI::DrawList->AddText(b2, IM_COL32(255, 255, 255, 255), "b2"); GUI::DrawList->AddText(b3, IM_COL32(255, 255, 255, 255), "b3"); GUI::DrawList->AddText(b4, IM_COL32(255, 255, 255, 255), "b4"); 

				GUI::DrawList->AddCircle(t1, 3, IM_COL32(0, 255, 0, 255)); GUI::DrawList->AddCircle(t2, 3, IM_COL32(0, 255, 0, 255)); GUI::DrawList->AddCircle(t3, 3, IM_COL32(0, 255, 0, 255)); GUI::DrawList->AddCircle(t4, 3, IM_COL32(0, 255, 0, 255)); GUI::DrawList->AddCircle(b1, 3, IM_COL32(0, 255, 0, 255)); GUI::DrawList->AddCircle(b2, 3, IM_COL32(0, 255, 0, 255)); GUI::DrawList->AddCircle(b3, 3, IM_COL32(0, 255, 0, 255)); GUI::DrawList->AddCircle(b4, 3, IM_COL32(0, 255, 0, 255));
			}
		}
	}
}

// https://imgur.com/a/jbIzbI6
// https://www.slideshare.net/Mark_Kilgard/cs-354-object-viewing-and-representation
// https://jsantell.com/model-view-projection/
bool ESP::WorldToScreen(const CVector& vecOrigin, float* viewMatrix, const ImVec2& windowCenter, ImVec2& vecScreenCoordsOut)
{
	static Vec4 clipCoordinates;
	clipCoordinates.w = vecOrigin.x * viewMatrix[12] + vecOrigin.y * viewMatrix[13] + vecOrigin.z * viewMatrix[14] + viewMatrix[15];
	if (clipCoordinates.w < 0.001f) return false; // If the entity's clip coordinates are not within the player's FOV
	clipCoordinates.x = vecOrigin.x * viewMatrix[0] + vecOrigin.y * viewMatrix[1] + vecOrigin.z * viewMatrix[2] + viewMatrix[3];
	clipCoordinates.y = vecOrigin.x * viewMatrix[4] + vecOrigin.y * viewMatrix[5] + vecOrigin.z * viewMatrix[6] + viewMatrix[7];

	// Transforming the clip coordinates into NDC (-1.0 to 1.0) by doing a perspective(FOV) division 
	static Vec2 NDC;
	NDC.x = clipCoordinates.x / clipCoordinates.w;
	NDC.y = clipCoordinates.y / clipCoordinates.w;

	// Viewport transformation
	vecScreenCoordsOut.x = (windowCenter.x * NDC.x) + (NDC.x + windowCenter.x);
	vecScreenCoordsOut.y = -(windowCenter.y * NDC.y) + (NDC.y + windowCenter.y);

	return true;
}