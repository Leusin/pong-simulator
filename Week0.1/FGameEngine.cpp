#include "FGameEngine.h"
#include <thread>

void FGameEngine::Initialize(HWND InHWnd)
{
	HWnd = InHWnd;

	Renderer.Create(InHWnd);
	Renderer.CreateShader();
	Renderer.CreateConstantBuffer();

	DebugUI.Startup(InHWnd, Renderer.Device, Renderer.DeviceContext);

	Timer.Reset();

	DebugUI.TimeResetCallback = [this]() { Timer.Reset(); };
	DebugUI.TimeStartCallback = [this]() { Timer.Start(); };
	DebugUI.TimeStopCallback = [this]() { Timer.Stop(); };

	PlayerPaddle.Offset.y = -0.5f;
}

void FGameEngine::Update()
{
	PlayerPaddle.Update(Timer.GetDeltaTime());
	Ball.Update(Timer.GetDeltaTime());

	DebugUI.DeltaTime = Timer.GetDeltaTime();
	DebugUI.GameTime = Timer.GetGameTime();
	DebugUI.RunningTime = Timer.GetRunTime();

	Timer.Tick();

	// LimitFrameRate
	int targetFPS = DebugUI.TargetFPS;
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
		DebugUI.FPS = fps;
		DebugUI.MFPS = mspf;
		frameCount = 0;
		timeElapsed += 1.0f;
	}
}

void FGameEngine::Render()
{
	Renderer.Prepare();
	Renderer.PrepareShader();

	PlayerPaddle.Render(Renderer);
	Ball.Render(Renderer);

	Renderer.ClearColor[0] = DebugUI.ClearColor.x;
	Renderer.ClearColor[1] = DebugUI.ClearColor.y;
	Renderer.ClearColor[2] = DebugUI.ClearColor.z;
	Renderer.ClearColor[3] = DebugUI.ClearColor.w;
	
	DebugUI.Render();

	Renderer.SwapBuffer();
	
}

void FGameEngine::Shutdown()
{
	DebugUI.Shutdown();
	Renderer.Release();
}

void FGameEngine::OnResize(int width, int height)
{
	Renderer.OnResize(width, height, HWnd);
}
