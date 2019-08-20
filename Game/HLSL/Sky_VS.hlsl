#include "Sky.hlsli"

VertexPosHL VS(VertexPos vIn)
{
    VertexPosHL vOut;
    
    // ����z = wʹ��z/w = 1(��պб�����Զƽ��)
    float4 posH = mul(float4(vIn.PosL, 1.0f), gWorldViewProj);
    vOut.PosH = posH.xyww;
    vOut.PosL = vIn.PosL;
    return vOut;
}