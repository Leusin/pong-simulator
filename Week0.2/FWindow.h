#pragma once

// windows.h�� ���ǵ� min, max ��ũ�θ� ������� ����
#define NOMINMAX

#include <functional>
#include <windows.h>

class FWindow
{
public:
	FWindow(HINSTANCE InInstance, LPCWSTR InTitle, int InWidth, int InHeight);
	~FWindow();

	HWND GetWindowHandle() const { return HWnd; }
	bool ProcessMessages();

	static std::function<void(int, int)> OnResizeCallback;

private:
	HWND HWnd;
	WNDCLASSW WndClass;

	static LRESULT CALLBACK WindowProc(HWND InHWnd, UINT InMessage, WPARAM InWParam, LPARAM InLParam);

};
