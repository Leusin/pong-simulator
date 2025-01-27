#pragma once

#include "FWindow.h"
#include "FGameEngine.h"

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_  HINSTANCE hPrevInstance, _In_  LPSTR lpCmdLine, _In_ int nShowCmd)
{
	FWindow window(hInstance, L"My Game Engine", 1280, 1024);

	FGameEngine engine;
	engine.Initialize(window.GetWindowHandle());
	
	window.OnResizeCallback = std::bind(&FGameEngine::OnResize, &engine, std::placeholders::_1, std::placeholders::_2);

	// 메시지 루프
	while (window.ProcessMessages())
	{
		engine.Update();
		engine.Render();
	}

	engine.Shutdown();

	return 0;
}