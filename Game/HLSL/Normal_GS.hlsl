#include "Basic.hlsli"

[maxvertexcount(2)]
void GS(point VertexPosHWNormalTex input[1], inout LineStream<VertexPosH> output)
{
    VertexPosH v;

    matrix viewProj = mul(gView, gProj);
    float3 posW = input[0].PosW;

    posW += input[0].NormalW * 0.001f;
    v.PosH = mul(float4(posW, 1.0f), viewProj);
    output.Append(v);

    posW += input[0].NormalW * 0.3f;
    v.PosH = mul(float4(posW, 1.0f), viewProj);
    output.Append(v);
}



