#include "Basic.hlsli"

// ������ɫ��(3D)
float4 PS(VertexPosHWNormalTex pIn) : SV_Target
{ 
    float4 diffuseAlbedo = gMaterial.DiffuseAlbedo;
    float2 projectTexCoord = float2(0.0f, 0.0f);
    float lightDepthValue = 1.0f;

    [flatten]
    if (gTextureUsed)
    {
        diffuseAlbedo *= gDiffuseMap.Sample(gSamWrap, pIn.Tex);
        // ��ǰ���вü����Բ�����Ҫ������ؿ��Ա����������
        clip(diffuseAlbedo.a - 0.1f);
    }

    // ��Ӱ����
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
    
    // ��׼��������
    pIn.NormalW = normalize(pIn.NormalW);

    // �������ָ���۾����������Լ��������۾��ľ���
    float3 toEyeW = normalize(gEyePosW - pIn.PosW);
    float distToEye = distance(gEyePosW, pIn.PosW);

    // ��ʼ��Ϊ0 
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
  
    // ������
    float4 ambient = gAmbientLight * diffuseAlbedo;

    float4 litColor = ambient + float4(directLight, 0.0f);
    
    // ��Ч����
    [flatten]
    if (gFogEnabled)
    {
        // �޶���0.0f��1.0f��Χ
        float fogLerp = saturate((distToEye - gFogStart) / gFogRange);
        // ������ɫ�͹�����ɫ�������Բ�ֵ
        litColor = lerp(litColor, gFogColor, fogLerp);
    }

    litColor.a = gMaterial.DiffuseAlbedo.a;
    return litColor;
}
