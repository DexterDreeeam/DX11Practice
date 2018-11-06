#pragma once

#include "Dx11DemoBase.h"

class TriangleDemo : public Dx11DemoBase
{
public:
    TriangleDemo();
    virtual ~TriangleDemo() override;

    bool LoadContent() override;
    void UnloadContent() override;

    void Update(float dt) override;
    void Render();

private:
    ID3D11VertexShader * solidColorVS_;
    ID3D11PixelShader * solidColorPS_;

    ID3D11InputLayout * inputLayout_;
    ID3D11Buffer * vertexBuffer_;

};