#pragma once

#include <windows.h>
#include <d3d11.h>
#include <functional>
#include "imgui/imgui.h"

class DebugUIManager
{
public:
    ImVec4 ClearColor;
    
    int TargetFPS;

    float FPS;
    float MFPS;
    float DeltaTime;
    float GameTime;
    float RunningTime;

    std::function<void()> TimeStopCallback;
    std::function<void()> TimeStartCallback;
    std::function<void()> TimeResetCallback;

private:
    HWND hWnd;
    bool bShowDemoWindow = false;

public:
    DebugUIManager();

    void Startup(HWND hWnd, ID3D11Device* Device, ID3D11DeviceContext* DeviceContext);
    void Render();
    void Shutdown();

private:
    void BeginFrame();
    void EndFrame();
};

