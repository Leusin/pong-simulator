#include "DebugUIManager.h"

#include <d3dcompiler.h>
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"

void DebugUIManager::Startup(HWND hWnd, ID3D11Device* Device, ID3D11DeviceContext* DeviceContext)
{
	// ImGui √ ±‚»≠
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	ImGui_ImplWin32_Init(hWnd);

	if (Device != nullptr && DeviceContext != nullptr)
	{
		ImGui_ImplDX11_Init(Device, DeviceContext);
	}
}

void DebugUIManager::Render()
{
	BeginFrame();

	if (bShowDemoWindow)
	{
		ImGui::ShowDemoWindow(&bShowDemoWindow);
	}

	ImGui::Begin("Jungle Property Window");
	ImGui::Text("Hello, ImGui!");
	if (ImGui::Button("Quit this app"))
	{
		PostMessage(hWnd, WM_QUIT, 0, 0);
	}
	ImGui::Checkbox("Demo Window", &bShowDemoWindow);
	ImGui::ColorEdit3("clear color", (float*)&ClearColor); // Edit 3 floats representing a color
	ImGui::End();

	EndFrame();
}

void DebugUIManager::Shutdown()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void DebugUIManager::BeginFrame()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void DebugUIManager::EndFrame()
{
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}