// 方向光
struct DirectionalLight
{
    float3 Strength;
    float Pad;
    float3 Direction;
    float Pad1;
};

// 点光
struct PointLight
{
    float3 Strength;
    float FalloffStart;
    float FalloffEnd;
    float3 Position;
};

// 聚光灯
struct SpotLight
{
    float3 Strength;
    float FalloffStart;
    float3 Direction;
    float FalloffEnd;
    float3 Position;
    float SpotPower;
};

// 物体表面材质
struct Material
{
    float4 DiffuseAlbedo; 
    float3 FresnelR0; // 菲尼尔
    float Shininess;   // 光滑度
};

float CalcAttenuation(float d, float falloffStart, float falloffEnd)
{
    // 线性衰减.
    return saturate((falloffEnd - d) / (falloffEnd - falloffStart));
}

// Schlick 给出了菲涅耳反射率的近似值
// R0 = ( (n-1)/(n+1) )^2, 其中n是折射率
float3 SchlickFresnel(float3 R0, float3 normal, float3 lightVec)
{
    float cosIncidentAngle = saturate(dot(normal, lightVec));

    float f0 = 1.0f - cosIncidentAngle;
    float3 reflectPercent = R0 + (1.0f - R0) * (f0 * f0 * f0 * f0 * f0);

    return reflectPercent;
}

float3 BlinnPhong(float3 lightStrength, float3 lightVec, float3 normal, float3 toEye, Material mat)
{
    const float m = mat.Shininess * 256.0f;
    float3 halfVec = normalize(toEye + lightVec);

    float roughnessFactor = (m + 8.0f) * pow(max(dot(halfVec, normal), 0.0f), m) / 8.0f;
    float3 fresnelFactor = SchlickFresnel(mat.FresnelR0, halfVec, lightVec);

    float3 specAlbedo = fresnelFactor * roughnessFactor;

    specAlbedo = specAlbedo / (specAlbedo + 1.0f);

    return (mat.DiffuseAlbedo.rgb + specAlbedo) * lightStrength;
}

float3 ComputeDirectionalLight(Material mat, DirectionalLight L, float3 normal, float3 toEye)
{
	// 光向量与照射方向相反
    float3 lightVec = -L.Direction;

	// Lambert's cosine law
    float ndotl = max(dot(lightVec, normal), 0.0f);
    float3 lightStrength = L.Strength * ndotl;

    return BlinnPhong(lightStrength, lightVec, normal, toEye, mat);
}

float3 ComputePointLight(Material mat, PointLight L, float3 pos, float3 normal, float3 toEye)
{
	// 从表面到光源的向量
    float3 lightVec = L.Position - pos;

	// 表面到光线的距离
    float d = length(lightVec);

	// 灯光范围测试
    if (d > L.FalloffEnd)
        return 0.0f;

	// 标准化光向量
    lightVec /= d;

    // Lambert's cosine law
    float ndotl = max(dot(lightVec, normal), 0.0f);
    float3 lightStrength = L.Strength * ndotl;

	// 根据距离减弱
    float att = CalcAttenuation(d, L.FalloffStart, L.FalloffEnd);
    lightStrength *= att;

    return BlinnPhong(lightStrength, lightVec, normal, toEye, mat);
}


float3 ComputeSpotLight(Material mat, SpotLight L, float3 pos, float3 normal, float3 toEye)
{
	// 从表面到光源的向量
    float3 lightVec = L.Position - pos;

	// 表面到光线的距离
    float d = length(lightVec);

	// 灯光范围测试
    if (d > L.FalloffEnd)
        return 0.0f;

	// 标准化光向量
    lightVec /= d;

    // Lambert's cosine law
    float ndotl = max(dot(lightVec, normal), 0.0f);
    float3 lightStrength = L.Strength * ndotl;

	// 根据距离减弱
    float att = CalcAttenuation(d, L.FalloffStart, L.FalloffEnd);
    lightStrength *= att;

    // 汇聚因子
    float spotFactor = pow(max(dot(-lightVec, L.Direction), 0.0f), L.SpotPower);
    lightStrength *= spotFactor;

    return BlinnPhong(lightStrength, lightVec, normal, toEye, mat);
}