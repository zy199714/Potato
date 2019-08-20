#include "Basic.hlsli"

SHADOW_PS_INPUT VS(InstancePosNormalTex vIn)
{
    SHADOW_PS_INPUT vout;
    float4 worldPosition = mul(float4(vIn.PosL, 1.0L), vIn.World);

    vout.position = mul(worldPosition, mul(gLightView, gLightProj));
    vout.depthPosition = vout.position;
    vout.Tex = vIn.Tex;
    return vout;
}