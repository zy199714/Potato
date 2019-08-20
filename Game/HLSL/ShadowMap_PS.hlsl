#include "Basic.hlsli"

float4 PS(SHADOW_PS_INPUT vIn) : SV_TARGET
{ 
    float4 litcolor = float4(1.0f, 1.0f, 1.0f, 1.0f);
    float depth = 1.0f;

    if (gTextureUsed)
    {
        litcolor = gDiffuseMap.Sample(gSamWrap, vIn.Tex);
        // 提前进行裁剪，对不符合要求的像素可以避免后续运算
        clip(litcolor.a - 0.02f);
    }
    
    //通过将Z像素深度除以均匀W坐标来获取像素的深度值。
    depth = vIn.depthPosition.z / vIn.depthPosition.w;
    litcolor = float4(depth, depth, depth, 1.0f);

    return litcolor;
}