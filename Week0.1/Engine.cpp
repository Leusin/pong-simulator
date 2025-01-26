#include "Engine.h"
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

	PlayerPaddle.Offset.y = -0.5f;
}

void Engine::Update()
{
	PlayerPaddle.Update(Timer.GetDeltaTime());

	debugUI.DeltaTime = Timer.GetDeltaTime();
	debugUI.GameTime = Timer.GetGameTime();
	debugUI.RunningTime = Timer.GetRunTime();

	Timer.Tick();

	// LimitFrameRate
	int targetFPS = debugUI.TargetFPS;
	double targetFrameTime = 1000.0 / targetFPS;
	// DeltaTime을 밀리초로 변환
	double elapsedTime = Timer.GetDeltaTime() * 1000.0;
	if (elapsedTime < targetFrameTime)
	{
		auto timeToSleep = targetFrameTime - elapsedTime;
		std::this_thread::sleep_for(
			std::chrono::milliseconds(
				static_cast<int>(timeToSleep)));
	}

	// CalculateFrameState
	static int frameCount = 0;
	static float timeElapsed = 0.f;

	frameCount++;
	if (Timer.GetGameTime() - timeElapsed >= 1.0f)
	{
		float fps = (float)frameCount;
		float mspf = 1000.0f / fps;
		debugUI.FPS = fps;
		debugUI.MFPS = mspf;
		frameCount = 0;
		timeElapsed += 1.0f;
	}
}

void Engine::Render()
{
	renderer.Prepare();
	renderer.PrepareShader();

	PlayerPaddle.Render(renderer);

	renderer.ClearColor[0] = debugUI.ClearColor.x;
	renderer.ClearColor[1] = debugUI.ClearColor.y;
	renderer.ClearColor[2] = debugUI.ClearColor.z;
	renderer.ClearColor[3] = debugUI.ClearColor.w;
	
	debugUI.Render();

	renderer.SwapBuffer();
	
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
