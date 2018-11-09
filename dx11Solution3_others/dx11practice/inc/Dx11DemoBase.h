#pragma once

#include <D3D11.h>
#include <D3DX11.h>
#include <DxErr.h>
#include <D3Dcompiler.h>
#include <dinput.h>

#define KEYDOWN(name, key) (name[key] & 0x80)
#define BUTTONDOWN(device, key) (device.rgbButtons[key] & 0x80)

class Dx11DemoBase
{
public:
    Dx11DemoBase();
    virtual ~Dx11DemoBase();

    bool Initialize(HINSTANCE hInstance, HWND hwnd);
    void Shutdown();

    virtual bool LoadContent();
    virtual void UnloadContent();

    virtual void Update(float dt) = 0;
    virtual void Render() = 0;

    bool CompileD3DShader(const char * filePath, const char * entry, const char * shaderModel, ID3DBlob ** buffer);

protected:
    HINSTANCE hInstance_;
    HWND hwnd_;
    
    D3D_DRIVER_TYPE driverType_;
    D3D_FEATURE_LEVEL featureLevel_;

    ID3D11Device * d3dDevice_;
    ID3D11DeviceContext * d3dContext_;
    IDXGISwapChain * swapChain_;
    ID3D11RenderTargetView * backBufferTarget_;

    ID3D11Texture2D * depthTexture_;
    ID3D11DepthStencilView * depthStencilView_;
};