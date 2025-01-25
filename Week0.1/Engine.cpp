#include "Engine.h"
#include "Vertices.h"

void Engine::Initialize(HWND hWnd)
{
	this->hWnd = hWnd;

	renderer.Create(hWnd);
	renderer.CreateShader();
	renderer.CreateConstantBuffer();

	debugUI.Startup(hWnd, renderer.Device, renderer.DeviceContext);
}

void Engine::Update()
{
	renderer.UpdateConstant(Offset);

	if (GetAsyncKeyState(VK_LEFT) & 0x8000)
	{
		// 왼쪽 화살표 키가 눌렸을 때 처리
		Offset.x -= 0.01f;
	}

	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
	{
		// 오른쪽 화살표 키가 눌렸을 때 처리
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

	renderer.ClearColor[0] = debugUI.ClearColor.x;
	renderer.ClearColor[1] = debugUI.ClearColor.y;
	renderer.ClearColor[2] = debugUI.ClearColor.z;
	renderer.ClearColor[3] = debugUI.ClearColor.w;
	
	debugUI.Render();

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

	renderer.ReleaseVertexBuffer(vertexBuffer);
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
