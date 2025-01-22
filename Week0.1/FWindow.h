#pragma once

#include <windows.h>
#include <functional>

class FWindow
{
public:
	FWindow(HINSTANCE InInstance, LPCWSTR InTitle, int InWidth, int InHeight);
	~FWindow();

	HWND GetWindowHandle() const { return HWnd; }
	bool ProcessMessages();

	static std::function<void(int, int, HWND)> OnResizeCallback;

private:
	HWND HWnd;
	WNDCLASSW WndClass;

	static LRESULT CALLBACK WindowProc(HWND InHWnd, UINT InMessage, WPARAM InWParam, LPARAM InLParam);

};
