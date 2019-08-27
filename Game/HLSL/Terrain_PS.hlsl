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
  
    // ������
    float4 ambient = gAmbientLight * diffuseAlbedo;

    float4 litColor = ambient + float4(directLight, 0.0f);
  
    litColor.a = mat.DiffuseAlbedo.a;

    return litColor;
};

