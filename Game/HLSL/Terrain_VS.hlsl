#include "Terrain.hlsli"

VertexPosHL VS(VertexPos vIn)
{
    VertexPosHL vOut;
    
	float4 posW = mul(float4(vIn.PosL, 1.0f), gWorld);
    vOut.PosH = mul(posW, gViewProj);
    vOut.PosL = vIn.PosL;
    return vOut;
}