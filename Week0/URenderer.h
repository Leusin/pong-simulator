#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>
#include "FVertexSimple.h"
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"

class URenderer
{
public:
	// Direct3D 11 ��ġ(Device)�� ��ġ ���ؽ�Ʈ(Device Context) �� ���� ü��(Swap Chain)�� �����ϱ� ���� �����͵�
	ID3D11Device* Device = nullptr; // GPU�� ����ϱ� ���� Direct3D ��ġ
	ID3D11DeviceContext* DeviceContext = nullptr; // GPU ��� ������ ����ϴ� ���ؽ�Ʈ
	IDXGISwapChain* SwapChain = nullptr; // ������ ���۸� ��ü�ϴ� �� ���Ǵ� ���� ü��

	// �������� �ʿ��� ���ҽ� �� ���¸� �����ϱ� ���� ������
	ID3D11Texture2D* FrameBuffer = nullptr; // ȭ�� ��¿� �ؽ�ó
	ID3D11RenderTargetView* FrameBufferRTV = nullptr; // �ؽ�ó�� ���� Ÿ������ ����ϴ� ��
	ID3D11RasterizerState* RasterizerState = nullptr; // �����Ͷ����� ����(�ø�, ä��� ��� �� ����)
	ID3D11Buffer* ConstantBuffer = nullptr; // ���̴��� �����͸� �����ϱ� ���� ��� ����

	FLOAT ClearColor[4] = { 0.025f, 0.025f, 0.025f, 1.0f }; // ȭ���� �ʱ�ȭ(clear)�� �� ����� ���� (RGBA)
	D3D11_VIEWPORT ViewportInfo; // ������ ������ �����ϴ� ����Ʈ ����

public:
	// ���� URenderer Class�� �Ʒ� �ڵ带 �߰� �ϼ���.

	ID3D11VertexShader* SimpleVertexShader;
	ID3D11PixelShader* SimplePixelShader;
	ID3D11InputLayout* SimpleInputLayout;
	unsigned int Stride;

public:
	// ������ �ʱ�ȭ �Լ�
	void Create(HWND hWindow)
	{
		// Direct3D ��ġ �� ���� ü�� ����
		CreateDeviceAndSwapChain(hWindow);

		// ������ ���� ����
		CreateFrameBuffer();

		// �����Ͷ����� ���� ����
		CreateRasterizerState();

		// ���� ���ٽ� ���� �� ���� ���´� �� �ڵ忡���� �ٷ��� ����
	}

	// Direct3D ��ġ �� ���� ü���� �����ϴ� �Լ�
	void CreateDeviceAndSwapChain(HWND hWindow)
	{
		// �����ϴ� Direct3D ��� ������ ����
		D3D_FEATURE_LEVEL featurelevels[] = { D3D_FEATURE_LEVEL_11_0 };

		// ���� ü�� ���� ����ü �ʱ�ȭ
		DXGI_SWAP_CHAIN_DESC swapchaindesc = {};
		swapchaindesc.BufferDesc.Width = 0; // â ũ�⿡ �°� �ڵ����� ����
		swapchaindesc.BufferDesc.Height = 0; // â ũ�⿡ �°� �ڵ����� ����
		swapchaindesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM; // ���� ����
		swapchaindesc.SampleDesc.Count = 1; // ��Ƽ ���ø� ��Ȱ��ȭ
		swapchaindesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // ���� Ÿ������ ���
		swapchaindesc.BufferCount = 2; // ���� ���۸�
		swapchaindesc.OutputWindow = hWindow; // �������� â �ڵ�
		swapchaindesc.Windowed = TRUE; // â ���
		swapchaindesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD; // ���� ���

		// Direct3D ��ġ�� ���� ü���� ����
		D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr,
			D3D11_CREATE_DEVICE_BGRA_SUPPORT | D3D11_CREATE_DEVICE_DEBUG,
			featurelevels, ARRAYSIZE(featurelevels), D3D11_SDK_VERSION,
			&swapchaindesc, &SwapChain, &Device, nullptr, &DeviceContext);

		// ������ ���� ü���� ���� ��������
		SwapChain->GetDesc(&swapchaindesc);

		// ����Ʈ ���� ����
		ViewportInfo = { 0.0f, 0.0f, (float)swapchaindesc.BufferDesc.Width, (float)swapchaindesc.BufferDesc.Height, 0.0f, 1.0f };
	}

	// Direct3D ��ġ �� ���� ü���� �����ϴ� �Լ�
	void ReleaseDeviceAndSwapChain()
	{
		if (DeviceContext)
		{
			DeviceContext->Flush(); // �����ִ� GPU ��� ����
		}

		if (SwapChain)
		{
			SwapChain->Release();
			SwapChain = nullptr;
		}

		if (Device)
		{
			Device->Release();
			Device = nullptr;
		}

		if (DeviceContext)
		{
			DeviceContext->Release();
			DeviceContext = nullptr;
		}
	}

	// ������ ���۸� �����ϴ� �Լ�
	void CreateFrameBuffer()
	{
		// ���� ü�����κ��� �� ���� �ؽ�ó ��������
		SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&FrameBuffer);

		// ���� Ÿ�� �� ����
		D3D11_RENDER_TARGET_VIEW_DESC framebufferRTVdesc = {};
		framebufferRTVdesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM_SRGB; // ���� ����
		framebufferRTVdesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D; // 2D �ؽ�ó

		Device->CreateRenderTargetView(FrameBuffer, &framebufferRTVdesc, &FrameBufferRTV);
	}

	// ������ ���۸� �����ϴ� �Լ�
	void ReleaseFrameBuffer()
	{
		if (FrameBuffer)
		{
			FrameBuffer->Release();
			FrameBuffer = nullptr;
		}

		if (FrameBufferRTV)
		{
			FrameBufferRTV->Release();
			FrameBufferRTV = nullptr;
		}
	}

	// �����Ͷ����� ���¸� �����ϴ� �Լ�
	void CreateRasterizerState()
	{
		D3D11_RASTERIZER_DESC rasterizerdesc = {};
		rasterizerdesc.FillMode = D3D11_FILL_SOLID; // ä��� ���
		rasterizerdesc.CullMode = D3D11_CULL_BACK; // �� ���̽� �ø�

		Device->CreateRasterizerState(&rasterizerdesc, &RasterizerState);
	}

	// �����Ͷ����� ���¸� �����ϴ� �Լ�
	void ReleaseRasterizerState()
	{
		if (RasterizerState)
		{
			RasterizerState->Release();
			RasterizerState = nullptr;
		}
	}

	// �������� ���� ��� ���ҽ��� �����ϴ� �Լ�
	void Release()
	{
		RasterizerState->Release();

		// ���� Ÿ���� �ʱ�ȭ
		DeviceContext->OMSetRenderTargets(0, nullptr, nullptr);

		ReleaseFrameBuffer();
		ReleaseDeviceAndSwapChain();
	}

	// ���� ü���� �� ���ۿ� ����Ʈ ���۸� ��ü�Ͽ� ȭ�鿡 ���
	void SwapBuffer()
	{
		SwapChain->Present(1, 0); // 1: VSync Ȱ��ȭ
	}

public:
	void CreateShader()
	{
		ID3DBlob* vertexshaderCSO;
		ID3DBlob* pixelshaderCSO;

		D3DCompileFromFile(L"ShaderW0.hlsl", nullptr, nullptr, "mainVS", "vs_5_0", 0, 0, &vertexshaderCSO, nullptr);

		Device->CreateVertexShader(vertexshaderCSO->GetBufferPointer(), vertexshaderCSO->GetBufferSize(), nullptr, &SimpleVertexShader);

		D3DCompileFromFile(L"ShaderW0.hlsl", nullptr, nullptr, "mainPS", "ps_5_0", 0, 0, &pixelshaderCSO, nullptr);

		Device->CreatePixelShader(pixelshaderCSO->GetBufferPointer(), pixelshaderCSO->GetBufferSize(), nullptr, &SimplePixelShader);

		D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		Device->CreateInputLayout(layout, ARRAYSIZE(layout), vertexshaderCSO->GetBufferPointer(), vertexshaderCSO->GetBufferSize(), &SimpleInputLayout);

		Stride = sizeof(FVertexSimple);

		vertexshaderCSO->Release();
		pixelshaderCSO->Release();
	}

	void ReleaseShader()
	{
		if (SimpleInputLayout)
		{
			SimpleInputLayout->Release();
			SimpleInputLayout = nullptr;
		}

		if (SimplePixelShader)
		{
			SimplePixelShader->Release();
			SimplePixelShader = nullptr;
		}

		if (SimpleVertexShader)
		{
			SimpleVertexShader->Release();
			SimpleVertexShader = nullptr;
		}
	}

public:
	void Prepare()
	{
		DeviceContext->ClearRenderTargetView(FrameBufferRTV, ClearColor);

		DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		DeviceContext->RSSetViewports(1, &ViewportInfo);
		DeviceContext->RSSetState(RasterizerState);

		DeviceContext->OMSetRenderTargets(1, &FrameBufferRTV, nullptr);
		DeviceContext->OMSetBlendState(nullptr, nullptr, 0xffffffff);
	}

	void PrepareShader()
	{
		DeviceContext->VSSetShader(SimpleVertexShader, nullptr, 0);
		DeviceContext->PSSetShader(SimplePixelShader, nullptr, 0);
		DeviceContext->IASetInputLayout(SimpleInputLayout);
	}

	void RenderPrimitive(ID3D11Buffer* pBuffer, UINT numVertices)
	{
		UINT offset = 0;
		DeviceContext->IASetVertexBuffers(0, 1, &pBuffer, &Stride, &offset);

		DeviceContext->Draw(numVertices, 0);
	}

public:
	ID3D11Buffer* CreateVertexBuffer(FVertexSimple* vertices, UINT byteWidth)
	{
		// 2. Create a vertex buffer
		D3D11_BUFFER_DESC vertexbufferdesc = {};
		vertexbufferdesc.ByteWidth = byteWidth;
		vertexbufferdesc.Usage = D3D11_USAGE_IMMUTABLE; // will never be updated 
		vertexbufferdesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA vertexbufferSRD = { vertices };

		ID3D11Buffer* vertexBuffer;

		Device->CreateBuffer(&vertexbufferdesc, &vertexbufferSRD, &vertexBuffer);

		return vertexBuffer;
	}

	void ReleaseVertexBuffer(ID3D11Buffer* vertexBuffer)
	{
		vertexBuffer->Release();
	}
};
