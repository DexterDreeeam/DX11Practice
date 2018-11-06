#pragma once

#include "Dx11DemoBase.h"

class TextureDemo : public Dx11DemoBase
{
public:
    TextureDemo();
    virtual ~TextureDemo();

    bool LoadContent() override;
    void UnloadContent() override;

    void Update(float dt) override;
    void Render() override;

private:
    ID3D11VertexShader * solidColorVS_;
    ID3D11PixelShader * solidColorPS_;
    
    ID3D11InputLayout * inputLayout_;
    ID3D11Buffer * vertexBuffer_;

    ID3D11ShaderResourceView * colorMap_;
    ID3D11SamplerState * colorMapSampler_;
};