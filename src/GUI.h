/*
*  To draw stuff in the game, like menus there are generally 2 ways:
*
*  1) External overlay:
*  Creating a transparent window on top of the Game's window and draw on it (using Windows GDI).
*  It is very robust because it works for any game, but unfortunatelly doesn't work in Full Screen.
*  To stay undetected by anti-cheats with this method, the DLL can be injected in a legit overlay that the game wouldn't usually check (such as Nvidia's one or Discord) and draw from within that process.
*
*  2) Internal overlay:
*  Hooking the rendering graphics API that the game uses (OpenGL, DirectX, Vulkan), and render internally using it.
*  If the Game happens to have their own built in rendering API, that could work too, though it needs quite some reversing to understand how it works (CS:GO has one named 'ISurface')
*
*  Since CS:GO uses DirectX9, you would normally go with rendering stuff on screen using DirectX's API, but I will render using ImGui because it's way better.
*  In DirectX, the drawing methods are located between 'BeginScene - EndScene', therefore, we will be inserting our own draws as the last methods before 'EndScene' gets called.
*  To get the address of the 'EndScene' in the game we can either:
*  1. Signature scan it - less hustle and code but needs some reversing.
*  2. Create a dummy Direct3D9Device - this is very universal and works the same for every game because the DirectX9 code will never change.
*
*  We will use the 2nd method.
*  How this works is that, we will get a pointer to the 'IDirect3DDevice9' interface which has the virtual function 'EndScene'.
*  From the VTable of that interface, we will get the address of 'EndScene'. Because it's a virtual fucntion, it will point to the same address of the 'EndScene' that the game uses.
*  The 'd3d9.dll' doesn't even have to be reversed to get the index of 'EndScene' in the VTable, because it is available in <d3d9.h>
*/


#pragma once

#include <d3d9.h>

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_win32.h"
#include "ImGui/imgui_impl_dx9.h"
#define IMGUI_DEFINE_MATH_OPERATORS // For operator overloads on vectors
#include <ImGui/imgui_internal.h>

#include "CVector.h"



class GUI
{
public:
	static inline bool IsSetup = false;

	static inline HWND GameWndHandle = nullptr;
	static inline WNDPROC GameWndProcedureHandler = nullptr; // The OG WNDPROC handler

	static inline ImDrawList* DrawList = nullptr;
	static inline constexpr ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoNav;

	static inline ImVec2 windowResolution{};
	static inline ImVec2 windowCenter{};
	static inline ImVec2 windowBottom{};

	static inline CVector debugPlayerPos {};

	static inline CVector espBoxHeadOffset{ -15.f, 0.f, 72.f };
	static inline CVector espBoxFeetOffset{ 20.f, 0.f, -5.f };

	static inline CVector t1{ -16.f, 16.f, 72.f };
	static inline CVector t2{ 16.f, 16.f, 72.f };
	static inline CVector t3{ -16.f, -16.f, 72.f };
	static inline CVector t4{ 16.f, -16.f, 72.f };
	static inline CVector b1{ -16.f, 16.f, 0.f };
	static inline CVector b2{ 16.f, 16.f, 0.f };
	static inline CVector b3{ -16.f, -16.f, 0.f };
	static inline CVector b4{ 16.f, -16.f, 0.f };


	struct Skins
	{
		const char* Name;
		int ID;
	};

	static inline Skins skins[] =
	{
		{"Vanilla", 0},
		{"Groundwater (General)", 2},
		{"Candy Apple", 3},
		{"Forest DDPAT", 5},
		{"Arctic Camo", 6},
		{"Desert Storm", 8},
		{"Bengal Tiger", 9},
		{"Copperhead", 10},
		{"Skulls", 11},
		{"Crimson Web (General)", 12},
		{"Blue Streak", 13},
		{"Red Laminate", 14},
		{"Gunsmoke", 15},
		{"Jungle Tiger", 16},
		{"Urban DDPAT", 17},
		{"Virus", 20}
	};

	struct Menu
	{
		static inline bool IsOpen = true; // Entire/Main menu

		static inline bool Aimbot = false;
		static inline bool Triggerbot = false;
		static inline bool ESP = false;
		static inline bool SkinChanger = false;
		static inline bool Miscellaneous = false;
		static inline bool DevExperimental = false;
	};

	struct Options
	{
		struct Aimbot
		{
			static inline bool Enabled = false;

			static inline int Hotkey = 0; // 0 = Left Click, 1 = Left Alt
		};

		struct Triggerbot
		{
			static inline bool Enabled = false;
		};

		struct ESP
		{
			static inline bool Enabled = true;

			static inline bool Box2D = false;
			static inline bool Box3D = true;
			static inline bool SnapLine = false;
			static inline bool EntityHealth = false;

			static inline bool debugESPBoxCorners = false;
		};

		struct SkinChanger
		{
			static inline bool Enabled = false;

			static inline int Deagle_Skin = 0;
			static inline int Glock_Skin = 0;
			static inline int USP_Skin = 0;
			static inline int AK47_Skin = 0;
			static inline int M4A4_Skin = 0;
			static inline int M4A4S_Skin = 0;
			static inline int AWP_Skin = 0;
		};

		struct Miscellaneous
		{
			static inline bool BunnyHop = true;
			static inline bool AntiFlash = true;
			static inline bool Crosshair = false;
			static inline bool CrosshairCircle = false;

			static inline bool ImGuiDemoMenu = false;
		};
	};

	static void PreSetup();
	static void Setup(IDirect3DDevice9* device);
	static void ImGuiSetup(IDirect3DDevice9* device);
	static void ImGuiSetupStyle();

	static void ImGuiRenderFrame(); // Execute every draw call in one render frame


	// Menus
	static void RenderMenus();
	static void MainMenuBar();
	static void AimbotMenu();
	static void TriggerbotMenu();
	static void ESPMenu();
	static void SkinChangerMenu();
	static void MiscellaneousMenu();
	static void DevExperimentalMenu();

	static void Cleanup();


	struct Draw
	{
		static void Line(const ImVec2& startPoint, const ImVec2& endPoint, unsigned int color, float thickness = 1.f);

		static void HealthBar(const ImVec2& position, int health, int maxHealth = 100);
		static void HealthArc(const ImVec2& position, int health, float arcRadius = 10.f, int maxHealth = 100);
		static void Crosshair();
		static void CrosshairCircle();
	};
};






// A class that creates a temporary Window that will allow creating a dummy D3D9Device.
class TempDirectX
{
public:
	// WinAPI
	static inline HWND WindowHandle = nullptr;
	static inline WNDCLASSEXA WindowClass = {};

	// DirectX
	static inline IDirect3D9* Direct3D9 = nullptr;
	static inline IDirect3DDevice9* D3D9Device = nullptr;

	static void SetupDirectX();

	static bool SetupWindowClass(const char* windowClassName);
	static bool SetupWindow(const char* windowName);
	static bool CreateD3D9();
	static bool CreateD3D9Device();

	static void DestroyDirectX();
};



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
																	WNDPROC
* * * * * * ** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND windowHandle, UINT message, WPARAM wideParam, LPARAM longParam); // ImGui's Window Procedure Handler for its own embedded window

/* https://learn.microsoft.com/en-us/windows/win32/api/winuser/nc-winuser-wndproc
This is a callback function that processes messages (eg: moving the cursor, resizing, moving the window, etc.) sent to a window
This Window Procedure Handler is modified. Besides it processing messages for the original window (the game's window),
it also adds a handler of the ImGui's embedded window that renders the menu (because it also needs to process clicks, resizing, etc.) */
inline LRESULT CALLBACK ModifiedWindowProcedure(HWND windowHandle, UINT message, WPARAM wideParam, LPARAM longParam)
{
	// Menu Toggle button
	if (GetAsyncKeyState(VK_INSERT) & 1)
		GUI::Menu::IsOpen = !GUI::Menu::IsOpen;

	// To enable the cursor without going in the menu, hook the CSGO's input system
	// Simply drawing a cursor with ImGui and not calling the game's WNDPROC when the ImGui's menu is enabled is a terrible idea.

	// Pass messages to the ImGui's WndProcHandler only when the menu is open.
	if (GUI::Menu::IsOpen)
	{
		if (ImGui_ImplWin32_WndProcHandler(windowHandle, message, wideParam, longParam) == 1) // If the processed message was WM_SETCURSOR
			return TRUE; // https://learn.microsoft.com/en-us/windows/win32/menurc/wm-setcursor
	}

	return CallWindowProcA(GUI::GameWndProcedureHandler, windowHandle, message, wideParam, longParam);
}