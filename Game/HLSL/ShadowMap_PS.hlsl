#include "Basic.hlsli"

float4 PS(SHADOW_PS_INPUT vIn) : SV_TARGET
{ 
    float4 litcolor = float4(1.0f, 1.0f, 1.0f, 1.0f);
    float depth = 1.0f;

    if (gTextureUsed)
    {
        litcolor = gDiffuseMap.Sample(gSamWrap, vIn.Tex);
        // ��ǰ���вü����Բ�����Ҫ������ؿ��Ա����������
        clip(litcolor.a - 0.02f);
    }
    
    //ͨ����Z������ȳ��Ծ���W��������ȡ���ص����ֵ��
    depth = vIn.depthPosition.z / vIn.depthPosition.w;
    litcolor = float4(depth, depth, depth, 1.0f);

    return litcolor;
}