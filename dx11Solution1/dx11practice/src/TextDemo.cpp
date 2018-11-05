#include "TextDemo.h"
#include <xnamath.h>

struct VertexPos
{
    XMFLOAT3 pos;
    XMFLOAT2 tex;
};

TextDemo::TextDemo()
    : solidColorVS_(0)
    , solidColorPS_(0)
    , inputLayout_(0)
    , vertexBuffer_(0)
    , colorMap_(0)
    , colorMapSampler_(0)
{
}

TextDemo::~TextDemo()
{
}

bool TextDemo::LoadContent()
{
    ///VS
    ID3DBlob * vsBuffer = 0;
    bool compileResult = CompileD3DShader("shd/TextDemo.fx", "VS_Main", "vs_4_0", &vsBuffer);
    if (compileResult == false)
    {
        MessageBox(0, "Error loading vertex shader!", "Compile Error", MB_OK);
        return false;
    }

    HRESULT d3dResult;
    d3dResult = d3dDevice_->CreateVertexShader(vsBuffer->GetBufferPointer(), vsBuffer->GetBufferSize(), 0, &solidColorVS_);
    if (FAILED(d3dResult))
    {
        if (vsBuffer)
            vsBuffer->Release();
        return false;
    }

    ///Layout
    D3D11_INPUT_ELEMENT_DESC solidColorLayout[] =
    {
        {
            "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,
            0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0
        },
        {
            "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,
            0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0
        }
    };

    unsigned int totalLayoutElements = ARRAYSIZE(solidColorLayout);

    d3dResult = d3dDevice_->CreateInputLayout(
        solidColorLayout,
        totalLayoutElements,
        vsBuffer->GetBufferPointer(),
        vsBuffer->GetBufferSize(),
        &inputLayout_
    );

    vsBuffer->Release();
    if (FAILED(d3dResult))
        return false;

    ///PS
    ID3DBlob * psBuffer = 0;
    compileResult = CompileD3DShader("shd/TextDemo.fx", "PS_Main", "ps_4_0", &psBuffer);
    if (compileResult == false)
    {
        MessageBox(0, "Error loading pixel shader!", "Compile Error", MB_OK);
        return false;
    }

    d3dResult = d3dDevice_->CreatePixelShader(
        psBuffer->GetBufferPointer(),
        psBuffer->GetBufferSize(),
        0,
        &solidColorPS_
    );
    psBuffer->Release();
    if (FAILED(d3dResult))
        return false;

    ///Texture
    d3dResult = D3DX11CreateShaderResourceViewFromFile(
        d3dDevice_,
        "res/TextDemo.dds",
        0, 0,
        &colorMap_,
        0
    );
    if (FAILED(d3dResult))
    {
        //DXTRACE_MSG("Failed to load the texture image.");
        return false;
    }

    D3D11_SAMPLER_DESC colorMapDesc;
    ZeroMemory(&colorMapDesc, sizeof(colorMapDesc));
    colorMapDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    colorMapDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    colorMapDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    colorMapDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    colorMapDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    colorMapDesc.MaxLOD = D3D11_FLOAT32_MAX;

    d3dResult = d3dDevice_->CreateSamplerState(
        &colorMapDesc,
        &colorMapSampler_
    );

    if (FAILED(d3dResult))
    {
        //DXTRACE_MSG("Failed to create color map sampler state.");
        return false;
    }

    ID3D11Resource * colorTex;
    colorMap_->GetResource(&colorTex);
    D3D11_TEXTURE2D_DESC colorTexDesc;
    ((ID3D11Texture2D*)colorTex)->GetDesc(&colorTexDesc);
    colorTex->Release();


    ///vertices
    D3D11_BUFFER_DESC vertexDesc;
    const int sizeOfSprite = sizeof(VertexPos) * 6;
    const int maxLetters = 24;
    ZeroMemory(&vertexDesc, sizeof(vertexDesc));
    vertexDesc.Usage = D3D11_USAGE_DYNAMIC;
    vertexDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    vertexDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexDesc.ByteWidth = sizeOfSprite * maxLetters;

    d3dResult = d3dDevice_->CreateBuffer(&vertexDesc, 0, &vertexBuffer_);
    if (FAILED(d3dResult))
    {
        //DXTRACE_MSG("Failed to create vertex buffer.");
        return false;
    }
    return true;
}

void TextDemo::UnloadContent()
{
    if (colorMapSampler_)
        colorMapSampler_->Release();
    if (colorMap_)
        colorMap_->Release();
    if (solidColorVS_)
        solidColorVS_->Release();
    if (solidColorPS_)
        solidColorPS_->Release();
    if (inputLayout_)
        inputLayout_->Release();
    if (vertexBuffer_)
        vertexBuffer_->Release();

    colorMapSampler_ = 0;
    colorMap_ = 0;
    solidColorVS_ = 0;
    solidColorPS_ = 0;
    inputLayout_ = 0;
    vertexBuffer_ = 0;
}

void TextDemo::Update(float dt)
{
}

void TextDemo::Render()
{
    if (d3dContext_ == 0)
        return;

    float clearColor[4] = {0.2f, 0.22f, 0.24f, 1.0f};
    d3dContext_->ClearRenderTargetView(backBufferTarget_, clearColor);

    unsigned int stride = sizeof(VertexPos);
    unsigned int offset = 0;

    d3dContext_->IASetInputLayout(inputLayout_);
    d3dContext_->IASetVertexBuffers(0, 1, &vertexBuffer_, &stride, &offset);
    d3dContext_->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    d3dContext_->VSSetShader(solidColorVS_, 0, 0);
    d3dContext_->PSSetShader(solidColorPS_, 0, 0);
    d3dContext_->PSSetShaderResources(0, 1, &colorMap_);
    d3dContext_->PSSetSamplers(0, 1, &colorMapSampler_);

    DrawString("HELLO WORLD", -0.2f, 0.0f);

    swapChain_->Present(0, 0);
}

bool TextDemo::DrawString(const char * message, float startX, float startY)
{
    const int sizeOfSprite = sizeof(VertexPos) * 6;
    const int maxLetters = 24;
    int length = strlen(message);
    if(length > maxLetters)
        length = maxLetters;
    float charWidth = 32.0f / 800.0f;
    float charHeight = 32.0f / 640.0f;
    float texelWidth = 32.0f / 864.0f;
    const int verticesPerLetter = 6;


    ///map resource to load data
    D3D11_MAPPED_SUBRESOURCE mapResource;
    HRESULT d3dResult = d3dContext_->Map(vertexBuffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapResource);

    if(FAILED(d3dResult))
    {
        //DXTRACE_MSG("Failed to map resource.");
        return false;
    }

    VertexPos * spritePtr = (VertexPos *)mapResource.pData;
    const int indexA = static_cast<char>('A');
    const int indexZ = static_cast<char>('Z');

    for(int i = 0; i != length; ++i)
    {
        float thisStartX = startX + (charWidth * static_cast<float>(i));
        float thisEndX = thisStartX + charWidth;
        float thisEndY = startY + charHeight;

        spritePtr[0].pos = XMFLOAT3(  thisEndX,  thisEndY,   1.0f);
        spritePtr[1].pos = XMFLOAT3(  thisEndX,    startY,   1.0f);
        spritePtr[2].pos = XMFLOAT3(thisStartX,    startY,   1.0f);
        spritePtr[3].pos = XMFLOAT3(thisStartX,    startY,   1.0f);
        spritePtr[4].pos = XMFLOAT3(thisStartX,  thisEndY,   1.0f);
        spritePtr[5].pos = XMFLOAT3(  thisEndX,  thisEndY,   1.0f);

        int texLookup = 0;
        int letter = static_cast<char>(message[i]);
        if(letter < indexA || letter > indexZ)
        {
            texLookup = (indexZ - indexA) + 1;
        }
        else
        {
            texLookup = (letter - indexA);
        }

        float tuStart = 0.0f + (texelWidth * static_cast<float>(texLookup));
        float tuEnd = tuStart + texelWidth;

        spritePtr[0].tex = XMFLOAT2(tuEnd, 0.0f);
        spritePtr[1].tex = XMFLOAT2(tuEnd, 1.0f);
        spritePtr[2].tex = XMFLOAT2(tuStart, 1.0f);
        spritePtr[3].tex = XMFLOAT2(tuStart, 1.0f);
        spritePtr[4].tex = XMFLOAT2(tuStart, 0.0f);
        spritePtr[5].tex = XMFLOAT2(tuEnd, 0.0f);

        spritePtr += 6;
    }

    d3dContext_->Unmap(vertexBuffer_, 0);
    d3dContext_->Draw(6 * length, 0);

    return true;
}