
TextureCube gTexCube : register(t0);
SamplerState gSam : register(s0);

cbuffer CBChangesEveryFrame : register(b0)
{
    matrix gWorldViewProj;
}

struct VertexPos
{
    float3 PosL : POSITION;
};

struct VertexPosHL
{
    float4 PosH : SV_POSITION;
    float3 PosL : POSITION;
};


