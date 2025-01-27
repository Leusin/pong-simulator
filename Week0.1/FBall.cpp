#include "FBall.h"
#include "GeometryData.h"

FBall::FBall()
{
	UINT numVertices =
		sizeof(GSphereVertices) / sizeof(FVertexSimple);

	for (UINT i = 0; i < numVertices; ++i)
	{
		GSphereVertices[i].x *= Scale;
		GSphereVertices[i].y *= Scale;
		GSphereVertices[i].z *= Scale;
	}

	Velocity.x = ((float)(rand() % 100 - 50)) * Speed;
	Velocity.y = ((float)(rand() % 100 - 50)) * Speed;
}

void FBall::Update(float DeltaTime)
{
	Offset.x += Velocity.x * DeltaTime;
	Offset.y += Velocity.y * DeltaTime;

	const float leftBorder = -1.0f;
	const float rightBorder = 1.0f;
	const float topBorder = -1.0f;
	const float bottomBorder = 1.0f;

	float RenderRadius = Radius * Scale;
	if (Offset.x < leftBorder + RenderRadius)
	{
		Offset.x = leftBorder + RenderRadius;
		Velocity.x *= -1.0f;
	}
	if (Offset.x > rightBorder - RenderRadius)
	{
		Offset.x = rightBorder - RenderRadius;
		Velocity.x *= -1.0f;
	}
	if (Offset.y < topBorder + RenderRadius)
	{
		Offset.y = topBorder + RenderRadius;
		Velocity.y *= -1.0f;
	}
	if (Offset.y > bottomBorder - RenderRadius)
	{
		Offset.y = bottomBorder - RenderRadius;
		Velocity.y *= -1.0f;
	}
}

// 매우 부적절하지만 당분간 구조의 단순화를 위해 
// 매 프레임마다 버퍼를 생성했다 삭제
void FBall::Render(FGraphicsDX11Engine& Renderer)
{
	ID3D11Buffer* VertexBuffer =
		Renderer.CreateVertexBuffer(GSphereVertices,
			sizeof(GSphereVertices));

	INT NumVerticesTriangle =
		sizeof(GSphereVertices) / sizeof(FVertexSimple);
	
	ID3D11Buffer* ConstantBuffer = nullptr;
	D3D11_BUFFER_DESC constantbufferdesc = {};
	constantbufferdesc.ByteWidth = sizeof(FGraphicsDX11Engine::FConstants) + 0xf & 0xfffffff0; // ensure constant buffer size is multiple of 16 bytes
	constantbufferdesc.Usage = D3D11_USAGE_DYNAMIC; // will be updated from CPU every frame
	constantbufferdesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	constantbufferdesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	Renderer.Device->CreateBuffer(&constantbufferdesc, nullptr, &ConstantBuffer);

	D3D11_MAPPED_SUBRESOURCE constantbufferMSR;

	HRESULT hr;

	hr = Renderer.DeviceContext->Map(
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