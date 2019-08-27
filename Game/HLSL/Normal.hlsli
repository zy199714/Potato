cbuffer CBChangesEveryInstanceDrawing : register(b0)
{
    matrix gWorld;
    matrix gWorldInvTranspose;
}
cbuffer CBChangesEveryFrame : register(b1)
{
    matrix gView;
}

cbuffer CBChangesOnResize : register(b2)
{
    matrix gProj;
}

struct VertexPosNormalTex
{
    float3 PosL : POSITION;
    float3 NormalL : NORMAL;
    float2 Tex : TEXCOORD;
};

struct VertexPosNormalW
{
    float3 PosW : POSITION;
    float3 NormalW : NORMAL;
};

struct VertexPosH
{
    float4 PosH : SV_POSITION;
};
