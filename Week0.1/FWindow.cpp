#include "FWindow.h"

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

std::function<void(int, int, HWND)> FWindow::OnResizeCallback = nullptr;

FWindow::FWindow(HINSTANCE InInstance, LPCWSTR InTitle , int InWidth, int InHeight)
{
	// 윈도우 클래스 초기화
	WndClass = {};
	WndClass.lpfnWndProc = WindowProc;
	WndClass.hInstance = InInstance;
	WndClass.lpszClassName = InTitle;

	RegisterClassW(&WndClass);

	// 윈도우 생성
	HWnd = CreateWindowExW(
		0,
		WndClass.lpszClassName,
		InTitle,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		InWidth, InHeight,
		nullptr, nullptr,
		InInstance, nullptr
	);

	ShowWindow(HWnd, SW_SHOW);
	UpdateWindow(HWnd);
}

FWindow::~FWindow()
{
	UnregisterClassW(WndClass.lpszClassName, WndClass.hInstance);
}

bool FWindow::ProcessMessages()
{
	MSG Msg = {};
	while (PeekMessageW(&Msg, nullptr, 0, 0, PM_REMOVE))
	{
		if (Msg.message == WM_QUIT)
		{
			return false;
		}

		TranslateMessage(&Msg);
		DispatchMessageW(&Msg);
	}
	return true;
}

LRESULT CALLBACK FWindow::WindowProc(HWND InHWnd, UINT InMessage, WPARAM InWParam, LPARAM InLParam)
{
	if (ImGui_ImplWin32_WndProcHandler(InHWnd, InMessage, InWParam, InLParam))
	{
		return true;
	}

	switch (InMessage)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_SIZE:
		if (InWParam != SIZE_MINIMIZED) // 최소화 상태가 아니라면
		{
			int ResizeWidth = (UINT)LOWORD(InLParam);
			int ResizeHeight = (UINT)HIWORD(InLParam);
			if (OnResizeCallback)
			{
				OnResizeCallback(ResizeWidth, ResizeHeight, InHWnd);
			}
		}
		return 0;
	default:
		return DefWindowProc(InHWnd, InMessage, InWParam, InLParam);
	}
}