#pragma once

#include <d3d11.h>
#include <functional>
#include <windows.h>
#include "imgui/imgui.h"

class FDebugUIManager
{
public:
    ImVec4 ClearColor = { 0.025f, 0.025f, 0.025f, 1.0f };
    
    int TargetFPS = 60;

    float FPS;
    float MFPS;
    float DeltaTime;
    float GameTime;
    float RunningTime;

    std::function<void()> TimeStopCallback = nullptr;
    std::function<void()> TimeStartCallback = nullptr;
    std::function<void()> TimeResetCallback = nullptr;

    bool bOnCollide;

private:
    HWND hWnd;
    bool bShowDemoWindow = false;

public:
    void Startup(HWND hWnd, ID3D11Device* Device, ID3D11DeviceContext* DeviceContext);
    void Render();
    void Shutdown();

private:
    void BeginFrame();
    void EndFrame();
};

