#pragma once
#include <wtypes.h>
#include "URenderer.h"
#include "InputManager.h"
#include "SceneManager.h"

class Engine
{
public:
	void Initialize(HWND hWnd);
	void Update();
	void Render();
	void Shutdown();

private:
	HWND hWnd;
	URenderer renderer;
	InputManager inputManager;
	SceneManager sceneManager;
};

