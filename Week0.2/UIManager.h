#pragma once
#include <wtypes.h>
#include "imgui\imgui_impl_dx11.h"

class UIManager
{
public:
	void Initialize(HWND hWnd, ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	void BeginFrame();
	void EndFrame();
	void Shutdown();
};