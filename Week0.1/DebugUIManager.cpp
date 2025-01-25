#include "DebugUIManager.h"

#include <d3dcompiler.h>
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"

DebugUIManager::DebugUIManager()
	: DeltaTime{0.0f}
	, GameTime{0.0f}
	, TimeStopCallback{ nullptr }
	, TimeStartCallback{ nullptr }
	, TimeResetCallback{ nullptr }
{
	ClearColor = ImVec4(0.025f, 0.025f, 0.025f, 1.0f);
}

void DebugUIManager::Startup(HWND hWnd, ID3D11Device* Device, ID3D11DeviceContext* DeviceContext)
{
	// ImGui √ ±‚»≠
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();

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

	ImGui::Text("Running Time: %f", RunningTime);
	ImGui::Text("Game Time: %f", GameTime);
	ImGui::Text("Delta Time: %f", DeltaTime);
	if (ImGui::Button("Start"))
	{
		if (TimeStartCallback)
		{
			TimeStartCallback();
		}
	}
	if (ImGui::Button("Stop"))
	{
		if (TimeStopCallback)
		{
			TimeStopCallback();
		}
	}
	if (ImGui::Button("Restart"))
	{
		if (TimeResetCallback)
		{
			TimeResetCallback();
		}
	}

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