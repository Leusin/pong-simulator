#pragma once

#include "FWindow.h"
#include "Engine.h"

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_  HINSTANCE hPrevInstance, _In_  LPSTR lpCmdLine, _In_ int nShowCmd)
{
	FWindow window(hInstance, L"My Game Engine", 1280, 1024);

	Engine engine;
	engine.Initialize(window.GetWindowHandle());

	
	window.OnResizeCallback = std::bind(&Engine::OnResize, &engine, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);

	// 메시지 루프
	while (window.ProcessMessages())
	{
		engine.Update();
		engine.Render();
	}

	engine.Shutdown();

	return 0;
}