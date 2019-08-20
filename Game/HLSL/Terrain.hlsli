cbuffer CBChangesEveryFrame : register(b0)
{
	matrix gWorld;
    matrix gViewProj;
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


