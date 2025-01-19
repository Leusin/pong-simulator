#pragma once

#include <memory> 
#include "FWindow.h"
#include "Engine.h"

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_  HINSTANCE hPrevInstance, _In_  LPSTR lpCmdLine, _In_ int nShowCmd)
{
	std::unique_ptr<FWindow> window = std::make_unique<FWindow>(hInstance, L"My Game Engine", 1024, 1024);

	std::unique_ptr<Engine> engine = std::make_unique<Engine>();
	engine->Initialize(window->GetWindowHandle());

	// 메시지 루프
	while (window->ProcessMessages())
	{
		engine->Update();
		engine->Render();
	}

	engine->Shutdown();

	return 0;
}