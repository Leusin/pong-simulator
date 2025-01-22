#include "URenderer.h"

void URenderer::OnResize(int Width, int Height, HWND hWindow)
{
    if (SwapChain)
    {
        DeviceContext->OMSetRenderTargets(0, 0, 0);
        ReleaseFrameBuffer();

        HRESULT hr;
        hr = SwapChain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);

        hr = SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&FrameBuffer);
        if (FAILED(hr))
        {
            return;
        }

        D3D11_RENDER_TARGET_VIEW_DESC framebufferRTVdesc = {};
        framebufferRTVdesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM_SRGB; // 색상 포맷
        framebufferRTVdesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D; // 2D 텍스처
        hr = Device->CreateRenderTargetView(FrameBuffer, &framebufferRTVdesc, &FrameBufferRTV);
        if (FAILED(hr))
        {
            return;
        }

        DeviceContext->OMSetRenderTargets(1, &FrameBufferRTV, nullptr);

        ViewportInfo = { 0.0f, 0.0f, (float)Width, (float)Height, 0.0f, 1.0f };
        DeviceContext->RSSetViewports(1, &ViewportInfo);
    }
}