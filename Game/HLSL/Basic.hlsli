#include "LightHelper.hlsli"

Texture2D gDiffuseMap : register(t0);
Texture2D gShadowMap : register(t1);

SamplerState gSamWrap : register(s0);
SamplerState gSamBorder : register(s1);
SamplerState gSamPonitBorder : register(s2);


cbuffer CBChangesEveryInstanceDrawing : register(b0)
{
    matrix gWorld;
    matrix gWorldInvTranspose;
}

cbuffer CBChangesEveryObjectDrawing : register(b1)
{
    Material gMaterial;
}

cbuffer CBDrawingStates : register(b2)
{
    float4 gFogColor;
    int gFogEnabled;
    float gFogStart;
    float gFogRange;
    int gTextureUsed;
	int gShadowEnabled;
	float gBias;
	float2 padding1;
}

cbuffer CBChangesEveryFrame : register(b3)
{
    matrix gView;
    matrix gLightView;
    float4 gLightPosition;
    float3 gEyePosW;
    float gPad;
}

cbuffer CBChangesOnResize : register(b4)
{
    matrix gLightProj;
    matrix gProj;
}

cbuffer CBChangesRarely : register(b5)
{
    DirectionalLight gDirLight[5];
    PointLight gPointLight[5];
    SpotLight gSpotLight[5];
    int gNumDirLight;
    int gNumPointLight;
    int gNumSpotLight;
    int gpad2;
}

struct SHADOW_PS_INPUT
{
    float4 position : SV_POSITION;
    float4 depthPosition : TEXCOORD;
    float2 Tex : TEXCOORD1;
};

struct VertexPosNormalTex
{
    float3 PosL : POSITION;
    float3 NormalL : NORMAL;
    float2 Tex : TEXCOORD;
};

struct VertexPosHWNormalTex
{
    float4 PosH : SV_POSITION;
    float3 PosW : POSITION; // 在世界中的位置
    float3 NormalW : NORMAL; // 法向量在世界中的方向
    float2 Tex : TEXCOORD;
    float4 lightViewPositionH : TEXCOORD1;
    float3 lightPos : TEXCOORD2;
};

struct InstancePosNormalTex
{
    float3 PosL : POSITION;          // 来自输入槽0
    float3 NormalL : NORMAL;     // 来自输入槽0
    float2 Tex : TEXCOORD;          // 来自输入槽0
    matrix World : World;                                         // 来自输入槽1
    matrix WorldInvTranspose : WorldInvTranspose; // 来自输入槽1
};