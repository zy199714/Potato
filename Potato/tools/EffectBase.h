//***************************************************
// 效果管理
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
		/* 初始化资源                                                                                 */
		/************************************************************************/
		virtual bool InitAll(ID3D11Device* device) = 0;

		/************************************************************************/
		/* 绑定管线各阶段的贴图资源的加载，并更新缓冲区                           */
		/* 需在绘制前完成资源绑定                                                              */
		/************************************************************************/
		virtual void Apply(ID3D11DeviceContext* deviceContext) = 0;

		/************************************************************************/
		/* 设置默认的管线渲染状态                                                              */
		/************************************************************************/
		virtual void SetRenderDefault(ID3D11DeviceContext* deviceContext) = 0;

		/************************************************************************/
		/* 设置数据                                                                                    */
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

		/* 	各种类型灯光允许的最大数目 */
		static const int maxLights = 5;
	};
}
#endif // EFFECTBASE_H
