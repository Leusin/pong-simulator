#include "FGameEngine.h"
#include <thread>
#include "CollisionManager.h"

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

	PlayerPaddle.Offset.y = -0.8f;
}

void FGameEngine::Update()
{
	PlayerPaddle.Update(Timer.GetDeltaTime());
	Ball.Update(Timer.GetDeltaTime());

	// 충돌체크

	CollisionManager cm;
	DebugUI.bOnCollide = 
		cm.CheckBallPaddle(Ball, PlayerPaddle);

	Timer.Tick();

	// LimitFrameRate
	int TargetFPS = DebugUI.TargetFPS;
	double TargetFrameTime = 1000.0 / TargetFPS;
	// DeltaTime을 밀리초로 변환
	double ElapsedTime = Timer.GetDeltaTime() * 1000.0;
	if (ElapsedTime < TargetFrameTime)
	{
		auto timeToSleep = TargetFrameTime - ElapsedTime;
		std::this_thread::sleep_for(
			std::chrono::milliseconds(
				static_cast<int>(timeToSleep)));
	}

	DebugUI.DeltaTime = Timer.GetDeltaTime();
	DebugUI.GameTime = Timer.GetGameTime();
	DebugUI.RunningTime = Timer.GetRunTime();

	// CalculateFrameState
	static int FrameCount = 0;
	static float TimeElapsed = 0.f;

	FrameCount++;
	if (Timer.GetGameTime() - TimeElapsed >= 1.0f)
	{
		float fps = (float)FrameCount;
		float mspf = 1000.0f / fps;
		DebugUI.FPS = fps;
		DebugUI.MFPS = mspf;
		FrameCount = 0;
		TimeElapsed += 1.0f;
	}
}

void FGameEngine::Render()
{
	Renderer.Prepare();
	Renderer.PrepareShader();

	Ball.Render(Renderer);
	PlayerPaddle.Render(Renderer);

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
