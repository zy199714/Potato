#include "LightHelper.hlsli"

cbuffer CBChangesEveryFrame : register(b0)
{
    matrix gViewProj;
    float3 gEyePosW;
    float pad;
};

cbuffer CBChangesRarely : register(b1)
{
    DirectionalLight gDirLight[5];
    PointLight gPointLight[5];
    SpotLight gSpotLight[5];
    float4 gAmbientLight;
    int gNumDirLight;
    int gNumPointLight;
    int gNumSpotLight;
    int pad3;
}

struct VertexPosTex
{
	float3 PosL : POSITION;
    float3 Normal : NORMAL;
    float2 Tex : TEXCOORD0;
};

 struct VertexPosH
{
	float4 PosH : SV_POSITION;
    float3 PosW : POSITION; // 在世界中的位置
    float3 NormalW : NORMAL;
};

