#pragma once

#include "Dx11DemoBase.h"

class KeyboardDemo : public Dx11DemoBase
{
public:
    KeyboardDemo();
    virtual ~KeyboardDemo();

    bool LoadContent();
    void UnloadContent();

    void Update(float dt);
    void Render();

private:
    ID3D11VertexShader * customColorVS_;
    ID3D11PixelShader * customColorPS_;

    ID3D11InputLayout * inputLayout_;
    ID3D11Buffer * vertexBuffer_;

    ID3D11Buffer * colorCB_;
    int selectedColor_;
};