#pragma once

#include <windows.h>

class FWindow
{
public:
	FWindow(HINSTANCE InInstance, LPCWSTR InTitle, int InWidth, int InHeight);
	~FWindow();

	HWND GetWindowHandle() const { return HWnd; }
	bool ProcessMessages();

private:
	HWND HWnd;
	WNDCLASSW WndClass;

	static LRESULT CALLBACK WindowProc(HWND InHWnd, UINT InMessage, WPARAM InWParam, LPARAM InLParam);
};
