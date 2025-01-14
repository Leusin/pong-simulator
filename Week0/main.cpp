#pragma once

#include <windows.h>
#include "URenderer.h"
#include "FVertexSimple.h"

// 각종 메시지를 처리할 함수
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
		// Signal that the app should quit
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_  HINSTANCE hPrevInstance, _In_  LPSTR lpCmdLine, _In_ int nShowCmd)
{
	// 윈도우 클래스 이름
	WCHAR WindowClass[] = L"JungleWindowClass";

	// 윈도우 타이틀바에 표시될 이름
	WCHAR Title[] = L"Game Tech Lab";

	// 각종 메시지를 처리할 함수인 WndProc의 함수 포인터를 WindowClass 구조체에 넣는다.
	WNDCLASSW wndclass = { 0, WndProc, 0, 0, 0, 0, 0, 0, 0, WindowClass };

	// 윈도우 클래스 등록
	RegisterClassW(&wndclass);

	// 1024 x 1024 크기에 윈도우 생성
	HWND hWnd = CreateWindowExW(0, WindowClass, Title, WS_POPUP | WS_VISIBLE | WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 1024, 1024,
		nullptr, nullptr, hInstance, nullptr);

	bool bIsExit = false;

	// 각종 생성하는 코드를 여기에 추가합니다.
	// Renderer Class를 생성합니다.
	URenderer	renderer;

	// D3D11 생성하는 함수를 호출합니다.
	renderer.Create(hWnd);
	// 렌더러 생성 직후에 쉐이더를 생성하는 함수를 호출합니다.
	renderer.CreateShader();

	// Main Loop (Quit Message가 들어오기 전까지 아래 Loop를 무한히 실행하게 됨)
	while (bIsExit == false)
	{
		MSG msg;

		// 처리할 메시지가 더 이상 없을때 까지 수행
		while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			// 키 입력 메시지를 번역
			TranslateMessage(&msg);

			// 메시지를 적절한 윈도우 프로시저에 전달, 메시지가 위에서 등록한 WndProc 으로 전달됨
			DispatchMessage(&msg);

			if (msg.message == WM_QUIT)
			{
				bIsExit = true;
				break;
			}
		}
		////////////////////////////////////////////
		// 매번 실행되는 코드를 여기에 추가합니다.

		// 준비 작업
		renderer.Prepare();
		renderer.PrepareShader();

		// Renderer와 Shader 생성 이후에 버텍스 버퍼를 생성합니다.
		FVertexSimple triangle_vertices[] =
		{
			{  0.0f,  1.0f, 0.0f,  1.0f, 0.0f, 0.0f, 1.0f }, // Top vertex (red)
			{  1.0f, -1.0f, 0.0f,  0.0f, 1.0f, 0.0f, 1.0f }, // Bottom-right vertex (green)
			{ -1.0f, -1.0f, 0.0f,  0.0f, 0.0f, 1.0f, 1.0f }  // Bottom-left vertex (blue)
		};

		FVertexSimple* vertices = triangle_vertices;
		UINT ByteWidth = sizeof(triangle_vertices);
		UINT numVertices = sizeof(triangle_vertices) / sizeof(FVertexSimple);

		// 생성
		D3D11_BUFFER_DESC vertexbufferdesc = {};
		vertexbufferdesc.ByteWidth = ByteWidth;
		vertexbufferdesc.Usage = D3D11_USAGE_IMMUTABLE;
		vertexbufferdesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA vertexbufferSRD = { vertices };

		ID3D11Buffer* vertexBuffer;

		renderer.Device->CreateBuffer(&vertexbufferdesc, &vertexbufferSRD, &vertexBuffer);

		// 생성한 버텍스 버퍼를 넘겨 실질적인 렌더링 요청
		renderer.RenderPrimitive(vertexBuffer, numVertices);

		// 다 그렸으면 버퍼를 교환
		renderer.SwapBuffer();

		////////////////////////////////////////////

		// 버텍스 버퍼 소멸은 Renderer 소멸전에 처리합니다.
		vertexBuffer->Release();
	}

	// 렌더러 소멸 직전에 쉐이더를 소멸 시키는 함수를 호출합니다.
	renderer.ReleaseShader();
	// D3D11 소멸 시키는 함수를 호출합니다.
	renderer.Release();

	return 0;
}