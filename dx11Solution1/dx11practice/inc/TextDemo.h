#pragma once

#include "Dx11DemoBase.h"

class TextDemo : public Dx11DemoBase
{
public:
    TextDemo();
    virtual ~TextDemo();

    bool LoadContent();
    void UnloadContent();

    void Update(float dt);
    void Render();

private:
    bool DrawString(const char * message, float startX, float startY);

private:
    ID3D11VertexShader * solidColorVS_;
    ID3D11PixelShader * solidColorPS_;

    ID3D11InputLayout * inputLayout_;
    ID3D11Buffer * vertexBuffer_;

    ID3D11ShaderResourceView * colorMap_;
    ID3D11SamplerState * colorMapSampler_;
};