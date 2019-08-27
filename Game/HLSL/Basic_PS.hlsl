#include "Basic.hlsli"

// 像素着色器(3D)
float4 PS(VertexPosHWNormalTex pIn) : SV_Target
{ 
    float4 diffuseAlbedo = gMaterial.DiffuseAlbedo;
    float2 projectTexCoord = float2(0.0f, 0.0f);
    float lightDepthValue = 1.0f;

    [flatten]
    if (gTextureUsed)
    {
        diffuseAlbedo *= gDiffuseMap.Sample(gSamWrap, pIn.Tex);
        // 提前进行裁剪，对不符合要求的像素可以避免后续运算
        clip(diffuseAlbedo.a - 0.1f);
    }

    // 阴影部分
    [flatten]
    if (gShadowEnabled)
    {
        projectTexCoord.x = (pIn.lightViewPositionH.x / pIn.lightViewPositionH.w) / 2.0f + 0.5f;
        projectTexCoord.y = (-pIn.lightViewPositionH.y / pIn.lightViewPositionH.w) / 2.0f + 0.5f;

        if ((saturate(projectTexCoord.x) == projectTexCoord.x) && (saturate(projectTexCoord.y) == projectTexCoord.y))
        {
            float depth = gShadowMap.Sample(gSamPonitBorder, float2(projectTexCoord)).r;

            lightDepthValue = pIn.lightViewPositionH.z / pIn.lightViewPositionH.w;

            if ((lightDepthValue - gBias) > depth)
            {
                diffuseAlbedo.rgb *= 0.24;
                return diffuseAlbedo;
            }
        }
    }
    
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
        directLight += ComputeDirectionalLight(gMaterial, gDirLight[i], pIn.NormalW, toEyeW);
    }
        
    [unroll]
    for (i = 0; i < gNumPointLight; ++i)
    {
        directLight += ComputePointLight(gMaterial, gPointLight[i], pIn.PosW, pIn.NormalW, toEyeW);
    }

    [unroll]
    for (i = 0; i < gNumSpotLight; ++i)
    {
        directLight += ComputeSpotLight(gMaterial, gSpotLight[i], pIn.PosW, pIn.NormalW, toEyeW);
    }
  
    // 环境光
    float4 ambient = gAmbientLight * diffuseAlbedo;

    float4 litColor = ambient + float4(directLight, 0.0f);
    
    // 雾效部分
    [flatten]
    if (gFogEnabled)
    {
        // 限定在0.0f到1.0f范围
        float fogLerp = saturate((distToEye - gFogStart) / gFogRange);
        // 根据雾色和光照颜色进行线性插值
        litColor = lerp(litColor, gFogColor, fogLerp);
    }

    litColor.a = gMaterial.DiffuseAlbedo.a;
    return litColor;
}
