#include "FPaddle.h"
#include "GeometryData.h"

FPaddle::FPaddle()
	: Offset{0.0f, 0.0f , 0.0f }
	, Width{1.0f}
	, Height{1.0f}
	, Speed{1.0f}
{
}

void FPaddle::Update(float DeltaTime)
{
	if (GetAsyncKeyState(VK_LEFT) & 0x8000)
	{
		Offset.x -= Speed * DeltaTime;
	}

	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
	{
		Offset.x += Speed * DeltaTime;
	}
}

void FPaddle::Render(FGraphicsDX11Engine& Renderer)
{
	ID3D11Buffer* VertexBuffer = 
		Renderer.CreateVertexBuffer(GCubeVertices, 
			sizeof(GCubeVertices));
	
	INT NumVerticesTriangle = 
		sizeof(GCubeVertices) / sizeof(FVertexSimple);

	Renderer.RenderPrimitive(VertexBuffer, NumVerticesTriangle);
	Renderer.UpdateConstant(Offset);
	Renderer.ReleaseVertexBuffer(VertexBuffer);
}
