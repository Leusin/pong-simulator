#pragma once
#include <wtypes.h>
#include "URenderer.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "DebugUIManager.h"

class Engine
{
public:
	void Initialize(HWND hWnd);
	void Update();
	void Render();
	void Shutdown();

	void OnResize(int width, int height);
	
private:
	ImVec4 ClearColor = ImVec4(0.025f, 0.025f, 0.025f, 1.0f);
	FVector3 Offset = {0.f, 0.f, 0.f};
	bool bShowDemoWindow = false;

private:
	HWND hWnd;
	URenderer renderer;
	DebugUIManager debugUI;
	//InputManager inputManager;
	//SceneManager sceneManager;
};

