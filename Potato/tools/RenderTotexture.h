//***************************************************************************************
// ��Ⱦ��������
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
	
		// ��ʼ�Ե�ǰ���������Ⱦ
		void Begin(ID3D11DeviceContext* deviceContext);
		// �����Ե�ǰ�������Ⱦ����ԭ״̬
		void End(ID3D11DeviceContext* deviceContext);
	
		// ��ȡ��Ⱦ�õ�����
		ID3D11ShaderResourceView* GetOutputTextureRT();
		ID3D11ShaderResourceView* GetOutputTextureDS();
	
	private:
		ID3D11ShaderResourceView* mOutputTextureRT_SRV;	// �����ȾĿ����ɫ����Դ��ͼ
		ID3D11ShaderResourceView* mOutputTextureDS_SRV;	// ������/ģ����ɫ����Դ��ͼ
		ID3D11RenderTargetView*	 mOutputTextureRTV;		// ��������Ӧ����ȾĿ����ͼ
		ID3D11DepthStencilView*		 mOutputTextureDSV;		// ����������õ����/ģ����ͼ
		D3D11_VIEWPORT				 mOutputViewPort;			// ������õ��ӿ�
	
		ID3D11RenderTargetView*	 mCacheRTV;					// ��ʱ����ĺ󱸻�����
		ID3D11DepthStencilView*		 mCacheDSV;					// ��ʱ��������/ģ�建����
		D3D11_VIEWPORT				 mCacheViewPort;			// ��ʱ������ӿ�
	
		bool										 mGenerateMips;				// �Ƿ�����mipmap��
	};
}

#endif // RenderTotexture_H
