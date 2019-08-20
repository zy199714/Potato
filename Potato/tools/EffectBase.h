//***************************************************
// Ч������
//***************************************************

#ifndef EFFECTBASE_H
#define EFFECTBASE_H

#pragma once
#include "tools/Util.h"
#include "tools/EffectHelper.h"
#include "tools/RenderStates.h"
#include "tools/LightHelpr.h"
#include "tools/Vertex.h"

namespace Potato
{
	enum RenderType
	{
		RenderType_Object = 0x0,
		RenderType_Instance,
		RenderType_End
	};

	class EffectBase : public nocopy
	{
	public:
		EffectBase() {};
		~EffectBase() {};

		/************************************************************************/
		/* ��ʼ����Դ                                                                                 */
		/************************************************************************/
		virtual bool InitAll(ID3D11Device* device) = 0;

		/************************************************************************/
		/* �󶨹��߸��׶ε���ͼ��Դ�ļ��أ������»�����                           */
		/* ���ڻ���ǰ�����Դ��                                                              */
		/************************************************************************/
		virtual void Apply(ID3D11DeviceContext* deviceContext) = 0;

		/************************************************************************/
		/* ����Ĭ�ϵĹ�����Ⱦ״̬                                                              */
		/************************************************************************/
		virtual void SetRenderDefault(ID3D11DeviceContext* deviceContext) = 0;

		/************************************************************************/
		/* ��������                                                                                    */
		/************************************************************************/
		/* World */
		virtual void XM_CALLCONV SetWorldViewProjMatrix(DirectX::FXMMATRIX W, DirectX::CXMMATRIX V, DirectX::CXMMATRIX P) = 0;
		virtual void XM_CALLCONV SetWorldMatrix(DirectX::FXMMATRIX W) = 0;
		virtual void XM_CALLCONV SetViewMatrix(DirectX::FXMMATRIX V) = 0;
		virtual void XM_CALLCONV SetProjMatrix(DirectX::FXMMATRIX P) = 0;

		/* SRV */
		virtual void SetTexture(ID3D11DeviceContext* devicecontext, UINT startslot, UINT  numview, ID3D11ShaderResourceView* texture) = 0;
		virtual void SetTextureUsed(bool isUsed) = 0;
	protected:

		/* 	�������͵ƹ�����������Ŀ */
		static const int maxLights = 5;
	};
}
#endif // EFFECTBASE_H
