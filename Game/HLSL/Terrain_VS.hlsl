#include "Terrain.hlsli"

VertexPosH VS(VertexPosTex vIn)
{
    VertexPosH vOut;
    float4 posw = float4(vIn.PosL, 1.0f);
	
    vOut.PosW = posw;
    vOut.PosH = mul(posw, gViewProj);
    vOut.NormalW = vIn.Normal;

    return vOut;
};


