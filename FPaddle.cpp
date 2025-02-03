#include "FPaddle.h"
#include "GeometryData.h"

void FPaddle::Update(float DeltaTime)
{
	const float leftBorder = -1.0f;
	const float rightBorder = 1.0f;
	const float topBorder = -1.0f;
	const float bottomBorder = 1.0f;
	
	if (bMoveUp)
	{
		Offset.y += Speed * DeltaTime;
	}

	if (bMoveDown)
	{
		Offset.y -= Speed * DeltaTime;
	}

	// 경계면 처리
	if (Offset.x < leftBorder + HalfWidth)
	{
		Offset.x = leftBorder + HalfWidth;
	}
	if (Offset.x > rightBorder - HalfWidth)
	{
		Offset.x = rightBorder - HalfWidth;
	}
	if (Offset.y < topBorder + HalfHeight)
	{
		Offset.y = topBorder + HalfHeight;
	}
	if (Offset.y > bottomBorder - HalfHeight)
	{
		Offset.y = bottomBorder - HalfHeight;
	}
}

// 매우 부적절하지만 당분간 구조의 단순화를 위해 
// 매 프레임마다 버퍼를 생성했다 삭제
void FPaddle::Render(FGraphicsDX11Engine& Renderer)
{
	ID3D11Buffer* VertexBuffer = 
		Renderer.CreateVertexBuffer(GPaddleVertices,
			sizeof(GPaddleVertices));
	
	INT NumVerticesTriangle = 
		sizeof(GPaddleVertices) / sizeof(FVertexSimple);

	
	ID3D11Buffer* ConstantBuffer = nullptr;
	D3D11_BUFFER_DESC constantbufferdesc = {};
	constantbufferdesc.ByteWidth = sizeof(FGraphicsDX11Engine::FConstants) + 0xf & 0xfffffff0; // ensure constant buffer size is multiple of 16 bytes
	constantbufferdesc.Usage = D3D11_USAGE_DYNAMIC; // will be updated from CPU every frame
	constantbufferdesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	constantbufferdesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	Renderer.Device->CreateBuffer(&constantbufferdesc, nullptr, &ConstantBuffer);

	D3D11_MAPPED_SUBRESOURCE constantbufferMSR;
	HRESULT hr = Renderer.DeviceContext->Map(
		ConstantBuffer,
		0,
		D3D11_MAP_WRITE_DISCARD,
		0,
		&constantbufferMSR);
	FGraphicsDX11Engine::FConstants* constants =
		(FGraphicsDX11Engine::FConstants*)constantbufferMSR.pData;
	constants->Offset = Offset;

	Renderer.DeviceContext->VSSetConstantBuffers(0, 1, &ConstantBuffer);
	
	Renderer.RenderPrimitive(VertexBuffer, NumVerticesTriangle);

	Renderer.DeviceContext->Unmap(ConstantBuffer, 0);

	ConstantBuffer->Release();
	ConstantBuffer = nullptr;

	Renderer.ReleaseVertexBuffer(VertexBuffer);
}