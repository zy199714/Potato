#include "Normal.hlsli"

// ¶¥µã×ÅÉ«Æ÷
VertexPosNormalW VS(VertexPosNormalTex vIn)
{
    VertexPosNormalW vOut;
    
    float4 posW = mul(float4(vIn.PosL, 1.0f), gWorld);

    vOut.PosW = posW.xyz;

    vOut.NormalW = mul(vIn.NormalL, (float3x3) gWorldInvTranspose);
    vOut.NormalW = normalize(vOut.NormalW);

    return vOut;
}



