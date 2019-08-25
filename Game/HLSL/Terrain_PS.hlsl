#include "Terrain.hlsli"

float4 PS(VertexPosH pIn) : SV_Target
{
    float4 texColor = float4(0.7f, 0.7f, 0.7f, 1.0f);
    Material mat =
    {
        { 0.3f, 0.3f, 0.3f, 1.0f },
        { 0.8f, 0.8f, 0.8f, 1.0f },
        { 0.1f, 0.1f, 0.1f, 1.0f },
        { 0.0f, 0.0f, 0.0f, 0.0f }
    };
    // 标准化法向量
    pIn.NormalW = normalize(pIn.NormalW);

    // 求出顶点指向眼睛的向量，以及顶点与眼睛的距离
    float3 toEyeW = normalize(gEyePosW - pIn.PosW);
    float distToEye = distance(gEyePosW, pIn.PosW);

    // 初始化为0 
    float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 spec = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 A = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 D = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 S = float4(0.0f, 0.0f, 0.0f, 0.0f);
    int i;

    [unroll]
    for (i = 0; i < gNumDirLight; ++i)
    {
        ComputeDirectionalLight(mat, gDirLight[i], pIn.NormalW, toEyeW, A, D, S);
        ambient += A;
        diffuse += D;
        spec += S;
    }
        
    [unroll]
    for (i = 0; i < gNumPointLight; ++i)
    {
        ComputePointLight(mat, gPointLight[i], pIn.PosW, pIn.NormalW, toEyeW, A, D, S);
        ambient += A;
        diffuse += D;
        spec += S;
    }

    [unroll]
    for (i = 0; i < gNumSpotLight; ++i)
    {
        ComputeSpotLight(mat, gSpotLight[i], pIn.PosW, pIn.NormalW, toEyeW, A, D, S);
        ambient += A;
        diffuse += D;
        spec += S;
    }
  
   return texColor * (ambient + diffuse) + spec;
};

