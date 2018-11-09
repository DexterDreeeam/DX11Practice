Texture2D colorMap_ : register(t0);
SamplerState colorSampler_ : register(s0);

cbuffer cbChangesEverFrame : register(b0)
{
    matrix worldMatrix;
};

cbuffer cbNeverChanges : register(b1)
{
    matrix viewMatrix;
};

cbuffer cbChangeOnResize : register(b2)
{
    matrix projMatrix;
};

struct VS_Input
{
    float4 pos : POSITION;
    float2 tex : TEXCOORD;
};

struct PS_Input
{
    float4 pos : SV_POSITION;
    float2 tex : TEXCOORD;
};

PS_Input VS_Main(VS_Input vertex)
{
    PS_Input vsOut = (PS_Input)0;
    vsOut.pos = mul(vertex.pos, worldMatrix);
    vsOut.pos = mul(vsOut.pos, viewMatrix);
    vsOut.pos = mul(vsOut.pos, projMatrix);
    vsOut.tex = vertex.tex;

    return vsOut;
}

float4 PS_Main(PS_Input frag) : SV_TARGET
{
    return colorMap_.Sample(colorSampler_, frag.tex);
}