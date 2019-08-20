//***************************************************************************************
// �ṩһЩ��Ⱦ״̬
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
		static ID3D11RasterizerState* RSWireframe;	// ��դ����״̬���߿�ģʽ
		static ID3D11RasterizerState* RSNoCull;		// ��դ����״̬���ޱ���ü�ģʽ
		static ID3D11RasterizerState* RSCullClockWise;	// ��դ����״̬��˳ʱ��ü�ģʽ
	
		static ID3D11SamplerState* SSLinearWrap;		// ������״̬�����Թ���
		static ID3D11SamplerState* SSLinearBorder;		// ������״̬�����Թ���
		static ID3D11SamplerState* SSPonitWrap;		// ������״̬�������
		static ID3D11SamplerState* SSPonitBorder;		// ������״̬�������
		static ID3D11SamplerState* SSAnistropicWrap;	// ������״̬���������Թ���
	
		static ID3D11BlendState* BSNoColorWrite;		// ���״̬����д����ɫ
		static ID3D11BlendState* BSTransparent;		// ���״̬��͸�����
		static ID3D11BlendState* BSAlphaToCoverage;	// ���״̬��Alpha-To-Coverage
		static ID3D11BlendState* BSAdditive;				//���ģʽ���ӷ����
	
		static ID3D11DepthStencilState* DSSLessEqual;		// ���/ģ��״̬������������ֵ��ȵ�����
		static ID3D11DepthStencilState* DSSWriteStencil;		// ��ȡ�ģ��״̬��д��ģ��ֵ
		static ID3D11DepthStencilState* DSSDrawWithStencil;	// ��ȡ�ģ��״̬����ָ��ģ��ֵ��������л���
		static ID3D11DepthStencilState* DSSNoDoubleBlend;	// ���/ģ��״̬���޶��λ������
		static ID3D11DepthStencilState* DSSNoDepthTest;		// ���/ģ��״̬���ر���Ȳ���
		static ID3D11DepthStencilState* DSSNoDepthWrite;		// ���/ģ��״̬������Ȳ��ԣ���д�����ֵ
		static ID3D11DepthStencilState* DSSNoDepthTestWithStencil;		// ���/ģ��״̬���ر���Ȳ��ԣ���ָ��ģ��ֵ��������л���
		static ID3D11DepthStencilState* DSSNoDepthWriteWithStencil;	// ���/ģ��״̬������Ȳ��ԣ���д�����ֵ����ָ��ģ��ֵ��������л���
	};
}
#endif