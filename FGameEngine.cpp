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

	PaddleLeft.Offset.x = -0.95f;
	PaddleRight.Offset.x = 0.95f;
}

void FGameEngine::Update()
{
	// W 키
	if (GetAsyncKeyState(0x57) & 0x8000)
	{
		PaddleLeft.bMoveUp = true;
	}
	else
	{
		PaddleLeft.bMoveUp = false;
	}

	// S 키
	if (GetAsyncKeyState(0x53) & 0x8000)
	{
		PaddleLeft.bMoveDown = true;
	}
	else
	{
		PaddleLeft.bMoveDown = false;
	}

	if (GetAsyncKeyState(VK_UP) & 0x8000)
	{
		PaddleRight.bMoveUp = true;
	}
	else
	{
		PaddleRight.bMoveUp = false;
	}

	if (GetAsyncKeyState(VK_DOWN) & 0x8000)
	{
		PaddleRight.bMoveDown = true;
	}
	else
	{
		PaddleRight.bMoveDown = false;
	}

	PaddleLeft.Update(Timer.GetDeltaTime());
	PaddleRight.Update(Timer.GetDeltaTime());

	Ball.Update(Timer.GetDeltaTime());

	// 충돌체크

	CollisionManager cm;
	DebugUI.bOnCollide =
		cm.CheckBallPaddle(Ball, PaddleRight);

	if (DebugUI.bOnCollide == false)
	{
		DebugUI.bOnCollide =
			cm.CheckBallPaddle(Ball, PaddleLeft);
	}

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
	PaddleLeft.Render(Renderer);
	PaddleRight.Render(Renderer);

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
