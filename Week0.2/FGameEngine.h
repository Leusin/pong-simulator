#pragma once

#include "FDebugUIManager.h"
#include "FGameTimer.h"
#include "FGraphicsDX11Engine.h"
#include "FPaddle.h"
#include "FBall.h"

class FGameEngine
{
public:
	void Initialize(HWND InHWnd);
	void Update();
	void Render();
	void Shutdown();

	void OnResize(int InWidth, int InHeight);
	
private:
	ImVec4 ClearColor = ImVec4(0.025f, 0.025f, 0.025f, 1.0f);
	bool bShowDemoWindow = false;

private:
	HWND HWnd;
	FGraphicsDX11Engine Renderer;
	FDebugUIManager DebugUI;
	FGameTimer Timer;
	FPaddle PaddleLeft;
	FPaddle PaddleRight;
	FBall Ball;
	//InputManager inputManager;
	//SceneManager sceneManager;
};

