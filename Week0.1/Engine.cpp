#include "Engine.h"
#include "Vertices.h"

void Engine::Initialize(HWND hWnd)
{
	this->hWnd = hWnd;

	renderer.Create(hWnd);
	renderer.CreateShader();
	renderer.CreateConstantBuffer();

	// ImGui 초기화
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui_ImplWin32_Init((void*)hWnd);

	if (renderer.Device != nullptr && renderer.DeviceContext != nullptr) 
	{
		ImGui_ImplDX11_Init(renderer.Device, renderer.DeviceContext);
	}
}

void Engine::Update()
{
	inputManager.Update();
	//sceneManager.Update();
}

void Engine::Render()
{
	renderer.Prepare();
	renderer.PrepareShader();

	//sceneManager.Render(renderer);
	ID3D11Buffer* vertexBufferTriangle = renderer.CreateVertexBuffer(triangle_vertices, sizeof(triangle_vertices));
	INT numVerticesTriangle = sizeof(triangle_vertices) / sizeof(FVertexSimple);
	renderer.RenderPrimitive(vertexBufferTriangle, numVerticesTriangle);

	// PrepareImGuiFrame
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	// RenderImGuiUI
	ImGui::Begin("Jungle Property Window");
	ImGui::Text("Hello, ImGui!");
	if (ImGui::Button("Quit this app"))
	{
		// 현재 윈도우에 Quit 메시지를 메시지 큐로 보냄
		PostMessage(hWnd, WM_QUIT, 0, 0);
	}
	ImGui::End();
	/* 여기서 IMGUI 컨텐츠 추가할 것 */
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	renderer.SwapBuffer();

	// LimitFrameRate
	// TODO: 언젠가 Timer 클래스로 분리할 것
	const int targetFPS = 30;
	const double targetFrameTime = 1000.0 / targetFPS;
	LARGE_INTEGER frequency;
	QueryPerformanceFrequency(&frequency);
	LARGE_INTEGER startTime, endTime;
	double elapsedTime = 0.0;
	QueryPerformanceCounter(&startTime);
	do
	{
		Sleep(0);

		QueryPerformanceCounter(&endTime);

		// 루프 종료 시간 기록
		QueryPerformanceCounter(&endTime);

		// 한 프레임이 소요된 시간 계산 (밀리초 단위로 변환)
		elapsedTime = (endTime.QuadPart - startTime.QuadPart) * 1000.0 / frequency.QuadPart;

	} while (elapsedTime < targetFrameTime);

	renderer.ReleaseVertexBuffer(vertexBufferTriangle);
}

void Engine::Shutdown()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	renderer.Release();
}
