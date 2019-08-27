#include "Terrain.hlsli"

float4 PS(VertexPosH pIn) : SV_Target
{
    Material mat =
    {
        { 0.3f, 0.3f, 0.3f, 1.0f },
        { 0.12f, 0.12f, 0.12f},
        0.5f 
    };
    float4 diffuseAlbedo = mat.DiffuseAlbedo;

    // 标准化法向量
    pIn.NormalW = normalize(pIn.NormalW);

    // 求出顶点指向眼睛的向量，以及顶点与眼睛的距离
    float3 toEyeW = normalize(gEyePosW - pIn.PosW);
    float distToEye = distance(gEyePosW, pIn.PosW);

    // 初始化为0 
    float3 directLight = 0.0f;
    int i;

    [unroll]
    for (i = 0; i < gNumDirLight; ++i)
    {
        directLight += ComputeDirectionalLight(mat, gDirLight[i], pIn.NormalW, toEyeW);
    }
        
    [unroll]
    for (i = 0; i < gNumPointLight; ++i)
    {
        directLight += ComputePointLight(mat, gPointLight[i], pIn.PosW, pIn.NormalW, toEyeW);
    }

    [unroll]
    for (i = 0; i < gNumSpotLight; ++i)
    {
        directLight += ComputeSpotLight(mat, gSpotLight[i], pIn.PosW, pIn.NormalW, toEyeW);
    }
  
    // 环境光
    float4 ambient = gAmbientLight * diffuseAlbedo;

    float4 litColor = ambient + float4(directLight, 0.0f);
  
    litColor.a = mat.DiffuseAlbedo.a;

    return litColor;
};

