//***************************************************************************************
// 渲染到纹理类
// Render-To-Texture class.
//***************************************************************************************
#ifndef RenderTotexture_H
#define RenderTotexture_H

#pragma once
#include "tools/Util.h"

namespace Potato
{
	class RenderTotexture : public nocopy
	{
	public:
		RenderTotexture(ID3D11Device* device, int texWidth, int texHeight, bool generateMips = false);
		~RenderTotexture();
	
		// 开始对当前纹理进行渲染
		void Begin(ID3D11DeviceContext* deviceContext);
		// 结束对当前纹理的渲染，还原状态
		void End(ID3D11DeviceContext* deviceContext);
	
		// 获取渲染好的纹理
		ID3D11ShaderResourceView* GetOutputTextureRT();
		ID3D11ShaderResourceView* GetOutputTextureDS();
	
	private:
		ID3D11ShaderResourceView* mOutputTextureRT_SRV;	// 输出渲染目标着色器资源视图
		ID3D11ShaderResourceView* mOutputTextureDS_SRV;	// 输出深度/模板着色器资源视图
		ID3D11RenderTargetView*	 mOutputTextureRTV;		// 输出纹理对应的渲染目标视图
		ID3D11DepthStencilView*		 mOutputTextureDSV;		// 输出纹理所用的深度/模板视图
		D3D11_VIEWPORT				 mOutputViewPort;			// 输出所用的视口
	
		ID3D11RenderTargetView*	 mCacheRTV;					// 临时缓存的后备缓冲区
		ID3D11DepthStencilView*		 mCacheDSV;					// 临时缓存的深度/模板缓冲区
		D3D11_VIEWPORT				 mCacheViewPort;			// 临时缓存的视口
	
		bool										 mGenerateMips;				// 是否生成mipmap链
	};
}

#endif // RenderTotexture_H
