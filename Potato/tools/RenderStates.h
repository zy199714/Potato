//***************************************************************************************
// 提供一些渲染状态
//***************************************************************************************

#ifndef RENDERSTATES_H
#define RENDERSTATES_H

#include <wrl/client.h>
#include <d3d11.h>

namespace Potato
{
	class RenderStates
	{
	public:
		RenderStates();
		~RenderStates();

		static bool IsInit();
		static void InitAll(ID3D11Device* device);
		static void ReleaseAll();
	
	public:
		static ID3D11RasterizerState* RSWireframe;	// 光栅化器状态：线框模式
		static ID3D11RasterizerState* RSNoCull;		// 光栅化器状态：无背面裁剪模式
		static ID3D11RasterizerState* RSCullClockWise;	// 光栅化器状态：顺时针裁剪模式
	
		static ID3D11SamplerState* SSLinearWrap;		// 采样器状态：线性过滤
		static ID3D11SamplerState* SSLinearBorder;		// 采样器状态：线性过滤
		static ID3D11SamplerState* SSPonitWrap;		// 采样器状态：点过滤
		static ID3D11SamplerState* SSPonitBorder;		// 采样器状态：点过滤
		static ID3D11SamplerState* SSAnistropicWrap;	// 采样器状态：各项异性过滤
	
		static ID3D11BlendState* BSNoColorWrite;		// 混合状态：不写入颜色
		static ID3D11BlendState* BSTransparent;		// 混合状态：透明混合
		static ID3D11BlendState* BSAlphaToCoverage;	// 混合状态：Alpha-To-Coverage
		static ID3D11BlendState* BSAdditive;				//混合模式：加法混合
	
		static ID3D11DepthStencilState* DSSLessEqual;		// 深度/模板状态：允许绘制深度值相等的像素
		static ID3D11DepthStencilState* DSSWriteStencil;		// 深度、模板状态：写入模板值
		static ID3D11DepthStencilState* DSSDrawWithStencil;	// 深度、模板状态：对指定模板值的区域进行绘制
		static ID3D11DepthStencilState* DSSNoDoubleBlend;	// 深度/模板状态：无二次混合区域
		static ID3D11DepthStencilState* DSSNoDepthTest;		// 深度/模板状态：关闭深度测试
		static ID3D11DepthStencilState* DSSNoDepthWrite;		// 深度/模板状态：仅深度测试，不写入深度值
		static ID3D11DepthStencilState* DSSNoDepthTestWithStencil;		// 深度/模板状态：关闭深度测试，对指定模板值的区域进行绘制
		static ID3D11DepthStencilState* DSSNoDepthWriteWithStencil;	// 深度/模板状态：仅深度测试，不写入深度值，对指定模板值的区域进行绘制
	};
}
#endif