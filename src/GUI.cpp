#include "pch.h"

#include <stdexcept>

#include "CMemoryManager.h"
#include "CHookz.h"
#include "GUI.h"
#include "ESP.h"
#include "Miscellaneous.h"


//		From 'imgui_demo.cpp'
static void HelpMarker(const char* desc)
{
	ImGui::TextDisabled("(?)");
	if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayShort))
	{
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted(desc);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
}




/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
																	GUI
* * * * * * ** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void GUI::PreSetup()
{
	try
	{
		TempDirectX::SetupDirectX();
		Hookz::Setup();
	}
	catch (const std::exception& error)
	{
		MessageBeep(MB_ICONERROR);
		MessageBoxA(0, error.what(), "GUI Fatal Error", MB_OK | MB_ICONEXCLAMATION);

		CMemoryManager::UninjectDLL(1);
	}
}

void GUI::Setup(IDirect3DDevice9* device)
{
	// Populate the creation parameters of the device (GetCreationParameters returns them)
	D3DDEVICE_CREATION_PARAMETERS DeviceCreationParams{}; device->GetCreationParameters(&DeviceCreationParams);

	GameWndHandle = DeviceCreationParams.hFocusWindow; // Get the Handle to the game's window

	// SetWindowLongA changes an attribute of the specified window, given its handle. nIndex specifies what attribute to change, dwNewLong is the replacement value.
	// We change the address of the game's original Window Procedure Handler to ours, which includes the ImGui's Window Procedure Handler for its own window (the menu).
	// SetWindowLongA will change the current Procedure Handler to the one from 'dwNewLong' parameter, while returning the original one, which we also need.
	GameWndProcedureHandler = (WNDPROC)SetWindowLongA(GameWndHandle, GWLP_WNDPROC, (long)ModifiedWindowProcedure);

	ImGuiSetup(device);

	IsSetup = true;
}

void GUI::ImGuiSetup(IDirect3DDevice9* device)
{
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange; // Disable the ImGui's cursor when the menu is enabled because we will use the game's cursor

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();

	// Setup Platform/Renderer backends
	ImGui_ImplWin32_Init(GUI::GameWndHandle);
	ImGui_ImplDX9_Init(device);

	// Customize the look of ImGui
	ImGuiSetupStyle();
}

void GUI::ImGuiSetupStyle()
{
	ImGuiStyle& style = ImGui::GetStyle();
	ImGui::StyleColorsClassic();

	style.WindowPadding = { 14, 14 };
	style.FramePadding = { 10, 10 };
	style.ScrollbarSize = 10;
	style.WindowRounding = 10;
	style.FrameRounding = 4;
	style.GrabRounding = 0;
	style.ItemSpacing = { 10, 8 };
}



void GUI::ImGuiRenderFrame()
{
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();


	{
		//DrawList = ImGui::GetWindowDrawList(); // Needs a specific window (ImGui::Begin - ImGui::End)
		//DrawList = ImGui::GetForegroundDrawList(); // Draws even above the ImGui's elements - not really what you'd want
		DrawList = ImGui::GetBackgroundDrawList(); // Draws in the background without the need of a window
	}

	{
		windowResolution = ImGui::GetIO().DisplaySize;
		windowCenter = windowResolution * 0.5f;
		windowBottom = { windowCenter.x, windowResolution.y };
	}

	{ // Render functions
		if (GUI::Menu::IsOpen) GUI::RenderMenus();

		if (GUI::Options::ESP::Enabled) ESP::Render();
		if (GUI::Options::Miscellaneous::Crosshair) Draw::Crosshair();
		if (GUI::Options::Miscellaneous::CrosshairCircle) Draw::CrosshairCircle();
		if (GUI::Options::Miscellaneous::ImGuiDemoMenu) ImGui::ShowDemoWindow();
	}


	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
}

void GUI::RenderMenus()
{
	MainMenuBar();
	if (Menu::Aimbot) AimbotMenu();
	if (Menu::Triggerbot) TriggerbotMenu();
	if (Menu::ESP) ESPMenu();
	if (Menu::SkinChanger) SkinChangerMenu();
	if (Menu::Miscellaneous) MiscellaneousMenu();
	if (Menu::DevExperimental) DevExperimentalMenu();
}


float indentOffset = 460.f;
void GUI::MainMenuBar()
{
	ImGui::BeginMainMenuBar();

	// Center the menus, though there must be an offset because the rendering starts from the center (screenWidth * 0.5) and spans to the right, therefore, we offset it to the left a bit (depending on the width of all our menus texts).
	// TODO: Check how 'GUI::Draw::HealthArc' centers the text with CalcTextSize. Could work here the same by calculating the width of all the menu texts, and then offseting them that way, instead of manually hardcoding an offset.
	ImGui::Indent((windowResolution.x - indentOffset) * 0.5f);

	ImGui::MenuItem("Aimbot", NULL, &Menu::Aimbot);
	ImGui::MenuItem("Triggerbot", NULL, &Menu::Triggerbot);
	ImGui::MenuItem("ESP", NULL, &Menu::ESP);
	ImGui::MenuItem("SkinChanger", NULL, &Menu::SkinChanger);
	ImGui::MenuItem("Miscellaneous", NULL, &Menu::Miscellaneous);
	ImGui::MenuItem("Dev Experimental", NULL, &Menu::DevExperimental);
	ImGui::Unindent();

	ImGui::EndMainMenuBar();
}

void GUI::AimbotMenu()
{
	ImGui::Begin("Aimbot", &Menu::Aimbot, windowFlags);

	{
		ImGui::Checkbox("Enable", &Options::Aimbot::Enabled);
		ImGui::Separator();

		ImGui::Combo("Aim Hotkey", &Options::Aimbot::Hotkey, "Left Click\0Left Alt\0\0");
	}

	ImGui::End();
}

void GUI::TriggerbotMenu()
{
	ImGui::Begin("Triggerbot", &Menu::Triggerbot, windowFlags);

	{
		ImGui::Checkbox("Enable", &Options::Triggerbot::Enabled);
		ImGui::Separator();

	}

	ImGui::End();
}

void GUI::ESPMenu()
{
	ImGui::Begin("ESP", &Menu::ESP, windowFlags);

	{
		ImGui::Checkbox("Enable", &Options::ESP::Enabled);
		ImGui::Separator();

		ImGui::Checkbox("Box 2D", &Options::ESP::Box2D);
		ImGui::Checkbox("Box 3D", &Options::ESP::Box3D);
		ImGui::Checkbox("Line", &Options::ESP::SnapLine);
		ImGui::Checkbox("Health", &Options::ESP::EntityHealth);
	}

	ImGui::End();
}

void GUI::SkinChangerMenu()
{
	static auto items_getter = [](void* data, int idx, const char** out_text) -> bool
	{
		Skins* items = reinterpret_cast<Skins*>(data);
		*out_text = items[idx].Name;
		return true;
	};

	ImGui::Begin("SkinChanger", &Menu::SkinChanger, windowFlags);

	{
		ImGui::Checkbox("Enable", &Options::SkinChanger::Enabled);
		ImGui::Separator();

		if (ImGui::Combo("Desert Eagle", &Options::SkinChanger::Deagle_Skin, items_getter, skins, IM_ARRAYSIZE(skins)) ||
			ImGui::Combo("Glock", &Options::SkinChanger::Glock_Skin, items_getter, skins, IM_ARRAYSIZE(skins)) ||
			ImGui::Combo("USP", &Options::SkinChanger::USP_Skin, items_getter, skins, IM_ARRAYSIZE(skins)) ||
			ImGui::Combo("AK47", &Options::SkinChanger::AK47_Skin, items_getter, skins, IM_ARRAYSIZE(skins)) ||
			ImGui::Combo("M4A4", &Options::SkinChanger::M4A4_Skin, items_getter, skins, IM_ARRAYSIZE(skins)) ||
			ImGui::Combo("M4A4-S", &Options::SkinChanger::M4A4S_Skin, items_getter, skins, IM_ARRAYSIZE(skins)) ||
			ImGui::Combo("AWP", &Options::SkinChanger::AWP_Skin, items_getter, skins, IM_ARRAYSIZE(skins)))
		{
			// If any of the skins get changed, the game must force an update in order for them to be applied
			GameForceUpdate();
		}

	}

	ImGui::End();
}

void GUI::MiscellaneousMenu()
{
	ImGui::Begin("Miscellaneous", &Menu::Miscellaneous, windowFlags);

	{
		ImGui::Checkbox("Bunny Hop", &Options::Miscellaneous::BunnyHop);
		ImGui::Checkbox("Anti-Flash", &Options::Miscellaneous::AntiFlash);

		if (ImGui::TreeNode("Crosshair"))
		{
			ImGui::Checkbox("Cross", &Options::Miscellaneous::Crosshair);
			ImGui::Checkbox("Circle", &Options::Miscellaneous::CrosshairCircle);
			ImGui::TreePop();
		}
	}

	ImGui::End();
}

void GUI::DevExperimentalMenu()
{
	ImGui::Begin("Dev Experimental", &Menu::DevExperimental, windowFlags | ImGuiWindowFlags_AlwaysAutoResize);

	{
		ImGui::SliderFloat("MainMenuBar Indent Offset", &indentOffset, -windowResolution.x, windowResolution.x + 10);
		ImGui::SameLine(); HelpMarker("CTRL + Click -> Input value");
		ImGui::Separator();

		ImGui::Text("My position: x:%.5f, y:%.5f, z:%.5f", debugPlayerPos.x, debugPlayerPos.y, debugPlayerPos.z);

		if (ImGui::TreeNode("ESP Box adjustment"))
		{
			ImGui::Checkbox("ESP Box Points", &Options::ESP::debugESPBoxCorners);

			if (ImGui::TreeNode("2D"))
			{
				ImGui::SliderFloat3("Head (top left) XYZ", &espBoxHeadOffset.x, -100, 100);
				ImGui::SliderFloat3("Feet (bot right) XYZ", &espBoxFeetOffset.x, -100, 100);

				ImGui::TreePop();
			}

			if (ImGui::TreeNode("3D"))
			{
				ImGui::SliderFloat3("Top 1 XYZ", &t1.x, -100, 100);
				ImGui::SliderFloat3("Top 2 XYZ", &t2.x, -100, 100);
				ImGui::SliderFloat3("Top 3 XYZ", &t3.x, -100, 100);
				ImGui::SliderFloat3("Top 4 XYZ", &t4.x, -100, 100);
				ImGui::Separator();
				ImGui::SliderFloat3("Bot 1 XYZ", &b1.x, -100, 100);
				ImGui::SliderFloat3("Bot 2 XYZ", &b2.x, -100, 100);
				ImGui::SliderFloat3("Bot 3 XYZ", &b3.x, -100, 100);
				ImGui::SliderFloat3("Bot 4 XYZ", &b4.x, -100, 100);

				ImGui::TreePop();
			}

			ImGui::TreePop();
		}

		ImGui::Checkbox("ImGui Demo Menu", &Options::Miscellaneous::ImGuiDemoMenu);
	}

	ImGui::End();
}


void GUI::Cleanup()
{
	GUI::Menu::IsOpen = false; // (rare timing) Some times the app could execute the 'ImGui_ImplWin32_WndProcHandler' inside 'ModifiedWindowProcedure' during ejection because this is set to true
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	// Restore the game's original Window Procedure Handler.
	SetWindowLongA(GUI::GameWndHandle, GWLP_WNDPROC, (long)GUI::GameWndProcedureHandler);
}







/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
																	Draw Helpers
* * * * * * ** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// Unnecessary wrapper, but the compiler will optimize it away
void GUI::Draw::Line(const ImVec2& startPoint, const ImVec2& endPoint, unsigned int color, float thickness)
{
	DrawList->AddLine(startPoint, endPoint, color, thickness);
}

void GUI::Draw::HealthBar(const ImVec2& position, int health, int maxHealth)
{

}

void GUI::Draw::HealthArc(const ImVec2& position, int health, float arcRadius, int maxHealth)
{
	static float arcMax = Globals::PI * 2.0f;
	static char healthAmountText[4];

	float healthColorVariation = (float)health / (float)maxHealth;
	float redDifference = healthColorVariation - 1.0f;

	// Arc
	DrawList->PathArcTo(position,
						arcRadius,
						(-(arcMax / 4.f)) + (arcMax / maxHealth) * (maxHealth - health),
						arcMax - (arcMax / 4.f));
	DrawList->PathStroke(ImGui::ColorConvertFloat4ToU32({ fabs(healthColorVariation - redDifference), healthColorVariation, healthColorVariation, 1.f }), ImDrawFlags_None, 2.f);

	// Text
	sprintf_s(healthAmountText, 4, "%d", health);
	DrawList->AddText({ position.x - ImGui::CalcTextSize(healthAmountText).x * 0.5f , position.y - ImGui::CalcTextSize(healthAmountText).y * 0.5f }, IM_COL32(255, 255, 255, 255), healthAmountText);
}

void GUI::Draw::Crosshair()
{
	// First rectangle is the outline, the second is the inline

	// Dot
	DrawList->AddRectFilled(windowCenter - ImVec2{ 1, 1 }, windowCenter + ImVec2{ 2, 2 }, IM_COL32(0, 0, 0, 255));
	DrawList->AddRectFilled(windowCenter, windowCenter + ImVec2{ 1, 1 }, IM_COL32(255, 0, 0, 255));

	// Left
	DrawList->AddRectFilled(ImVec2{ windowCenter.x - 11, windowCenter.y - 1 }, ImVec2{ windowCenter.x - 3, windowCenter.y + 2 }, IM_COL32(0, 0, 0, 255));
	DrawList->AddRectFilled(ImVec2{ windowCenter.x - 10, windowCenter.y }, ImVec2{ windowCenter.x - 4, windowCenter.y + 1 }, IM_COL32(255, 0, 0, 255));

	// Right
	DrawList->AddRectFilled(ImVec2{ windowCenter.x + 4, windowCenter.y - 1 }, ImVec2{ windowCenter.x + 12, windowCenter.y + 2 }, IM_COL32(0, 0, 0, 255));
	DrawList->AddRectFilled(ImVec2{ windowCenter.x + 5, windowCenter.y }, ImVec2{ windowCenter.x + 11, windowCenter.y + 1 }, IM_COL32(255, 0, 0, 255));

	// Top (left with swapped x/y offsets)
	DrawList->AddRectFilled(ImVec2{ windowCenter.x - 1, windowCenter.y - 11 }, ImVec2{ windowCenter.x + 2, windowCenter.y - 3 }, IM_COL32(0, 0, 0, 255));
	DrawList->AddRectFilled(ImVec2{ windowCenter.x, windowCenter.y - 10 }, ImVec2{ windowCenter.x + 1, windowCenter.y - 4 }, IM_COL32(255, 0, 0, 255));

	// Bottom (right with swapped x/y offsets)
	DrawList->AddRectFilled(ImVec2{ windowCenter.x - 1, windowCenter.y + 4 }, ImVec2{ windowCenter.x + 2, windowCenter.y + 12 }, IM_COL32(0, 0, 0, 255));
	DrawList->AddRectFilled(ImVec2{ windowCenter.x, windowCenter.y + 5 }, ImVec2{ windowCenter.x + 1, windowCenter.y + 11 }, IM_COL32(255, 0, 0, 255));
}

void GUI::Draw::CrosshairCircle()
{
	DrawList->AddCircle(windowCenter, 7, IM_COL32(0, 255, 0, 255));
}








/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
																	TempDirectX
* * * * * * ** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void TempDirectX::SetupDirectX()
{
	if (!SetupWindowClass("windowClassName"))
		throw std::runtime_error("Failed to create the window class!");

	if (!SetupWindow("windowName"))
		throw std::runtime_error("Failed to create the window!");

	if (!CreateD3D9())
		throw std::runtime_error("Failed to create the DirectX object!");

	if (!CreateD3D9Device())
		throw std::runtime_error("Failed to create the DirectX Device!");

	// The window and the class that we created are not needed anymore
	if (WindowHandle) DestroyWindow(WindowHandle);
	UnregisterClassA(WindowClass.lpszClassName, WindowClass.hInstance);
}

bool TempDirectX::SetupWindowClass(const char* windowClassName)
{
	WindowClass.cbSize = sizeof(WNDCLASSEXA);
	WindowClass.style = CS_HREDRAW | CS_VREDRAW;
	WindowClass.lpfnWndProc = DefWindowProc;
	WindowClass.cbClsExtra = 0;
	WindowClass.cbWndExtra = 0;
	WindowClass.hInstance = GetModuleHandleA(nullptr);
	WindowClass.hIcon = nullptr;
	WindowClass.hCursor = nullptr;
	WindowClass.hbrBackground = nullptr;
	WindowClass.lpszMenuName = nullptr;
	WindowClass.lpszClassName = windowClassName;
	WindowClass.hIconSm = nullptr;

	return RegisterClassExA(&WindowClass) ? true : false;
}

bool TempDirectX::SetupWindow(const char* windowName)
{
	WindowHandle = CreateWindowExA(0,
		WindowClass.lpszClassName, windowName,
		WS_OVERLAPPEDWINDOW,
		0, 0,
		0, 0,
		nullptr, nullptr, WindowClass.hInstance,
		nullptr);

	return WindowHandle != nullptr;
}

bool TempDirectX::CreateD3D9()
{
	// The signature of Direct3DCreate9: https://learn.microsoft.com/en-us/windows/win32/api/d3d9/nf-d3d9-direct3dcreate9
	using Direct3DCreate9 = IDirect3D9 * (__stdcall*)(UINT);

	// d3d9.dll exports Direct3DCreate9, so we can easily get its address that will allow to create an instance of Direct3D9
	const Direct3DCreate9 D3D9Create = (Direct3DCreate9)CMemoryManager::GetExportedFuncAddr("d3d9.dll", "Direct3DCreate9");

	Direct3D9 = D3D9Create(D3D_SDK_VERSION);

	return Direct3D9 != nullptr;
}

bool TempDirectX::CreateD3D9Device()
{
	D3DPRESENT_PARAMETERS PresentationParams = {};
	PresentationParams.BackBufferWidth = 0; PresentationParams.BackBufferHeight = 0;
	PresentationParams.BackBufferFormat = D3DFMT_UNKNOWN; PresentationParams.BackBufferCount = 0;
	PresentationParams.MultiSampleType = D3DMULTISAMPLE_NONE; PresentationParams.MultiSampleQuality = 0;
	PresentationParams.SwapEffect = D3DSWAPEFFECT_DISCARD;
	PresentationParams.hDeviceWindow = WindowHandle;
	PresentationParams.Windowed = 1;
	PresentationParams.EnableAutoDepthStencil = 0; PresentationParams.AutoDepthStencilFormat = D3DFMT_UNKNOWN;
	PresentationParams.Flags = 0;
	PresentationParams.FullScreen_RefreshRateInHz = 0;
	PresentationParams.PresentationInterval = 0;

	Direct3D9->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_NULLREF,
		WindowHandle,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_DISABLE_DRIVER_MANAGEMENT,
		&PresentationParams,
		&D3D9Device);

	return D3D9Device != nullptr;
}

void TempDirectX::DestroyDirectX()
{
	D3D9Device->Release();
	D3D9Device = nullptr;

	Direct3D9->Release();
	Direct3D9 = nullptr;
}