#include "Engine.h"
#include "Vertices.h"
#include <thread>

void Engine::Initialize(HWND hWnd)
{
	this->hWnd = hWnd;

	renderer.Create(hWnd);
	renderer.CreateShader();
	renderer.CreateConstantBuffer();

	debugUI.Startup(hWnd, renderer.Device, renderer.DeviceContext);

	Timer.Reset();

	debugUI.TimeResetCallback = [this]() { Timer.Reset(); };
	debugUI.TimeStartCallback = [this]() { Timer.Start(); };
	debugUI.TimeStopCallback = [this]() { Timer.Stop(); };
}

void Engine::Update()
{
	renderer.UpdateConstant(Offset);

	if (GetAsyncKeyState(VK_LEFT) & 0x8000)
	{
		// 왼쪽 화살표 키가 눌렸을 때 처리
		Offset.x -= 0.2f * Timer.GetDeltaTime();
	}

	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
	{
		// 오른쪽 화살표 키가 눌렸을 때 처리
		Offset.x += 0.2f * Timer.GetDeltaTime();
	}

	debugUI.DeltaTime = Timer.GetDeltaTime();
	debugUI.GameTime = Timer.GetGameTime();
	debugUI.RunningTime = Timer.GetRunTime();

	Timer.Tick();
}

void Engine::Render()
{
	renderer.Prepare();
	renderer.PrepareShader();

	ID3D11Buffer* vertexBuffer = renderer.CreateVertexBuffer(cube_vertices, sizeof(cube_vertices));
	INT numVerticesTriangle = sizeof(cube_vertices) / sizeof(FVertexSimple);
	renderer.RenderPrimitive(vertexBuffer, numVerticesTriangle);

	renderer.ClearColor[0] = debugUI.ClearColor.x;
	renderer.ClearColor[1] = debugUI.ClearColor.y;
	renderer.ClearColor[2] = debugUI.ClearColor.z;
	renderer.ClearColor[3] = debugUI.ClearColor.w;
	
	debugUI.Render();

	renderer.SwapBuffer();
	renderer.ReleaseVertexBuffer(vertexBuffer);

	// LimitFrameRate
	const int targetFPS = 60;
	const double targetFrameTime = 1000.0 / targetFPS;
	double elapsedTime = Timer.GetDeltaTime() * 1000.0; // DeltaTime을 밀리초로 변환
	if (elapsedTime < targetFrameTime)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(
			static_cast<int>(targetFrameTime - elapsedTime)));
	}

}

void Engine::Shutdown()
{
	debugUI.Shutdown();
	renderer.Release();
}

void Engine::OnResize(int width, int height)
{
	renderer.OnResize(width, height, hWnd);
}
