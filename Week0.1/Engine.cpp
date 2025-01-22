#include "Engine.h"
#include "Vertices.h"

void Engine::Initialize(HWND hWnd)
{
	this->hWnd = hWnd;

	renderer.Create(hWnd);
	renderer.CreateShader();
	renderer.CreateConstantBuffer();

	// ImGui �ʱ�ȭ
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	ImGui_ImplWin32_Init(hWnd);

	if (renderer.Device != nullptr && renderer.DeviceContext != nullptr) 
	{
		ImGui_ImplDX11_Init(renderer.Device, renderer.DeviceContext);
	}
}

void Engine::Update()
{
	renderer.UpdateConstant(Offset);

	if (GetAsyncKeyState(VK_LEFT) & 0x8000)
	{
		// ���� ȭ��ǥ Ű�� ������ �� ó��
		Offset.x -= 0.01f;
	}

	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
	{
		// ������ ȭ��ǥ Ű�� ������ �� ó��
		Offset.x += 0.01f;
	}
}

void Engine::Render()
{
	renderer.Prepare();
	renderer.PrepareShader();

	ID3D11Buffer* vertexBuffer = renderer.CreateVertexBuffer(cube_vertices, sizeof(cube_vertices));
	INT numVerticesTriangle = sizeof(cube_vertices) / sizeof(FVertexSimple);
	renderer.RenderPrimitive(vertexBuffer, numVerticesTriangle);

	// PrepareImGuiFrame
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	if (bShowDemoWindow)
	{
		ImGui::ShowDemoWindow(&bShowDemoWindow);
	}

	// RenderImGuiUI
	ImGui::Begin("Jungle Property Window");
	ImGui::Text("Hello, ImGui!");
	if (ImGui::Button("Quit this app"))
	{
		// ���� �����쿡 Quit �޽����� �޽��� ť�� ����
		PostMessage(hWnd, WM_QUIT, 0, 0); 
	}
	ImGui::Checkbox("Demo Window", &bShowDemoWindow);

	renderer.ClearColor[0] = ClearColor.x;
	renderer.ClearColor[1] = ClearColor.y;
	renderer.ClearColor[2] = ClearColor.z;
	renderer.ClearColor[3] = ClearColor.w;
	ImGui::ColorEdit3("clear color", (float*)&ClearColor); // Edit 3 floats representing a color
	ImGui::End();

	/* ���⼭ IMGUI ������ �߰��� �� */
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	renderer.SwapBuffer();

	// LimitFrameRate
	// TODO: ������ Timer Ŭ������ �и��� ��
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

		// ���� ���� �ð� ���
		QueryPerformanceCounter(&endTime);

		// �� �������� �ҿ�� �ð� ��� (�и��� ������ ��ȯ)
		elapsedTime = (endTime.QuadPart - startTime.QuadPart) * 1000.0 / frequency.QuadPart;

	} while (elapsedTime < targetFrameTime);

	renderer.ReleaseVertexBuffer(vertexBuffer);
}

void Engine::Shutdown()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	renderer.Release();
}

void Engine::OnResize(int width, int height, HWND hWnd)
{
	renderer.OnResize(width, height, hWnd);
}
