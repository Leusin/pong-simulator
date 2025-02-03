#include "FDebugUIManager.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"

void FDebugUIManager::Startup(HWND hWnd, ID3D11Device* Device, ID3D11DeviceContext* DeviceContext)
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

void FDebugUIManager::Render()
{
	BeginFrame();

	if (bShowDemoWindow)
	{
		ImGui::ShowDemoWindow(&bShowDemoWindow);
	}

	ImGui::Begin("Debug Window");
	if (ImGui::Button("Quit this app"))
	{
		PostMessage(hWnd, WM_QUIT, 0, 0);
	}
	ImGui::ColorEdit3("clear color", (float*)&ClearColor);

	ImGui::InputInt("Target FPS", &TargetFPS);

	ImGui::Text("FPS/mFPS: %f(%f)", FPS, MFPS);
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

	ImGui::Checkbox("On Collide", &bOnCollide);

	ImGui::End();

	EndFrame();
}

void FDebugUIManager::Shutdown()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void FDebugUIManager::BeginFrame()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void FDebugUIManager::EndFrame()
{
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}