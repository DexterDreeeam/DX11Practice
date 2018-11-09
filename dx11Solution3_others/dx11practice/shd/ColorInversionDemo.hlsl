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

float4 PS_Main(PS_Input frag) : VS_TARGET
{
    return 1.0f - colorMap.Sample(colorSampler, frag.tex);
}

technique11 ColorInversion
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_5_0, VS_Main()));
        SetGeometrySHader(NULL);
        SetPixelShader(CompileShader(ps_5_0, PS_Main()));
    }
}