#include "Minimap.hlsli"

// ÏñËØ×ÅÉ«Æ÷
float4 PS(VertexPosHTex pIn) : SV_Target
{
    float4 color = { 1.0f, 1.0f, 1.0f, 1.0f };

    color = gTex.Sample(gSam, pIn.Tex);
    
    return color;
}