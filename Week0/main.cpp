#pragma once

#include <windows.h>
#include "URenderer.h"
#include "FVertexSimple.h"

// ���� �޽����� ó���� �Լ�
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
	// ������ Ŭ���� �̸�
	WCHAR WindowClass[] = L"JungleWindowClass";

	// ������ Ÿ��Ʋ�ٿ� ǥ�õ� �̸�
	WCHAR Title[] = L"Game Tech Lab";

	// ���� �޽����� ó���� �Լ��� WndProc�� �Լ� �����͸� WindowClass ����ü�� �ִ´�.
	WNDCLASSW wndclass = { 0, WndProc, 0, 0, 0, 0, 0, 0, 0, WindowClass };

	// ������ Ŭ���� ���
	RegisterClassW(&wndclass);

	// 1024 x 1024 ũ�⿡ ������ ����
	HWND hWnd = CreateWindowExW(0, WindowClass, Title, WS_POPUP | WS_VISIBLE | WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 1024, 1024,
		nullptr, nullptr, hInstance, nullptr);

	bool bIsExit = false;

	// ���� �����ϴ� �ڵ带 ���⿡ �߰��մϴ�.
	// Renderer Class�� �����մϴ�.
	URenderer	renderer;

	// D3D11 �����ϴ� �Լ��� ȣ���մϴ�.
	renderer.Create(hWnd);
	// ������ ���� ���Ŀ� ���̴��� �����ϴ� �Լ��� ȣ���մϴ�.
	renderer.CreateShader();

	// Main Loop (Quit Message�� ������ ������ �Ʒ� Loop�� ������ �����ϰ� ��)
	while (bIsExit == false)
	{
		MSG msg;

		// ó���� �޽����� �� �̻� ������ ���� ����
		while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			// Ű �Է� �޽����� ����
			TranslateMessage(&msg);

			// �޽����� ������ ������ ���ν����� ����, �޽����� ������ ����� WndProc ���� ���޵�
			DispatchMessage(&msg);

			if (msg.message == WM_QUIT)
			{
				bIsExit = true;
				break;
			}
		}
		////////////////////////////////////////////
		// �Ź� ����Ǵ� �ڵ带 ���⿡ �߰��մϴ�.

		// �غ� �۾�
		renderer.Prepare();
		renderer.PrepareShader();

		// Renderer�� Shader ���� ���Ŀ� ���ؽ� ���۸� �����մϴ�.
		FVertexSimple triangle_vertices[] =
		{
			{  0.0f,  1.0f, 0.0f,  1.0f, 0.0f, 0.0f, 1.0f }, // Top vertex (red)
			{  1.0f, -1.0f, 0.0f,  0.0f, 1.0f, 0.0f, 1.0f }, // Bottom-right vertex (green)
			{ -1.0f, -1.0f, 0.0f,  0.0f, 0.0f, 1.0f, 1.0f }  // Bottom-left vertex (blue)
		};

		FVertexSimple* vertices = triangle_vertices;
		UINT ByteWidth = sizeof(triangle_vertices);
		UINT numVertices = sizeof(triangle_vertices) / sizeof(FVertexSimple);

		// ����
		D3D11_BUFFER_DESC vertexbufferdesc = {};
		vertexbufferdesc.ByteWidth = ByteWidth;
		vertexbufferdesc.Usage = D3D11_USAGE_IMMUTABLE;
		vertexbufferdesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA vertexbufferSRD = { vertices };

		ID3D11Buffer* vertexBuffer;

		renderer.Device->CreateBuffer(&vertexbufferdesc, &vertexbufferSRD, &vertexBuffer);

		// ������ ���ؽ� ���۸� �Ѱ� �������� ������ ��û
		renderer.RenderPrimitive(vertexBuffer, numVertices);

		// �� �׷����� ���۸� ��ȯ
		renderer.SwapBuffer();

		////////////////////////////////////////////

		// ���ؽ� ���� �Ҹ��� Renderer �Ҹ����� ó���մϴ�.
		vertexBuffer->Release();
	}

	// ������ �Ҹ� ������ ���̴��� �Ҹ� ��Ű�� �Լ��� ȣ���մϴ�.
	renderer.ReleaseShader();
	// D3D11 �Ҹ� ��Ű�� �Լ��� ȣ���մϴ�.
	renderer.Release();

	return 0;
}