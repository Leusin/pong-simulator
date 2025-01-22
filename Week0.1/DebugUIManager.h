#pragma once

#include <windows.h>
#include <d3d11.h>
#include "imgui/imgui.h"

class DebugUIManager
{
    bool bShowDemoWindow = false;
    HWND hWnd;

public:
    ImVec4 ClearColor = ImVec4(0.025f, 0.025f, 0.025f, 1.0f);

public:
    void Startup(HWND hWnd, ID3D11Device* Device, ID3D11DeviceContext* DeviceContext);
    void Render();
    void Shutdown();

private:
    void BeginFrame();
    void EndFrame();
};

