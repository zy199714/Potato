
Texture2D gTex : register(t0);
SamplerState gSam : register(s0);

cbuffer CBChangesEveryFrame : register(b0)
{
    float3 gEyePosW; // …„œÒª˙Œª÷√
    float gPad;
}

struct VertexPosTex
{
    float3 PosL : POSITION;
    float2 Tex : TEXCOORD;
};

struct VertexPosHTex
{
    float4 PosH : SV_POSITION;
    float2 Tex : TEXCOORD;
};





