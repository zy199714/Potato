/**
 * @file app3d.h
 * 
 */

#ifndef APP3D_H
#define APP3D_H

#pragma once

#include <wrl/client.h>
#include <d3d11.h>
#include <d2d1.h>
#include <dwrite.h>
#include <DirectXMath.h>
#include <DirectXCollision.h>
#include <string>
#include "tools/Mouse.h"
#include "tools/Keyboard.h"
#include "tools/GameTimer.h"

 //�������Ҫ���õĿ�
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "D3DCompiler.lib")
#pragma comment(lib, "winmm.lib")

namespace Potato
{
	class App3D
	{
	public:
		App3D(HINSTANCE hInstance, uint16_t width, uint16_t height);
		virtual ~App3D();
	
		HINSTANCE AppInst() const;			//��ȡӦ��ʵ�����
		HWND MainWnd() const;					//	��ȡ���ھ��
		float AspectRatio() const;					//��ȡ��Ļ��߱�
	
		int Run();				//���г���ִ����Ϣѭ��
	
		virtual bool Init();				//��ʼ�����ڡ�Direct3D��Direct2D����
		virtual void OnResize();		//���ڴ�С�ı��ǵ���
		virtual void UpdateScene(float dt) = 0;				//���ÿһ֡�ĸ���
		virtual void DrawScene() = 0;				//���ÿһ֡�Ļ���
		virtual LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);		//��Ϣ�ص�
	
	protected:
		bool InitMainWindow();
		bool InitDirect2D();
		bool InitDirect3D();
	
		void CalculateFrameStats();		//����ÿ��֡�����ڴ�����ʾ
	
	protected:
		HINSTANCE mhAppInst;			//Ӧ��ʵ�����
		HWND mhMainWnd;				//����ʵ�����
		bool mAppPaused;					//�Ƿ���ͣ
		bool mMinimized;						//�Ƿ���С��
		bool mMaximized;					//�Ƿ����
		bool mResizing;							//�Ƿ񴰿ڴ�С�仯
		bool mEnable4xMsaa;				//�Ƿ���4�����ز���
		UINT m4xMsaaQuality;				//MSAA֧�ֵ������ȼ�
	
		GameTimer mTimer;		//��ʱ��
	
		// Direct2D
		ID2D1Factory* md2dFactory;
		ID2D1RenderTarget* md2dRenderTarget;
		IDWriteFactory* mdwriteFactory;
		//Direct3D
		ID3D11Device* md3dDevice;
		ID3D11DeviceContext* md3dImmediateContext;
		IDXGISwapChain* mSwapChain;

		// ���Զ����ͷ�
#if defined(DEBUG) | defined(_DEBUG)
		ID3D11Debug* d3d11Debug;
#endif

		//������Դ
		ID3D11Texture2D* mDepthStencilBuffer;
		ID3D11RenderTargetView* mRenderTargetView;
		ID3D11DepthStencilView* mDepthStencilView;
		D3D11_VIEWPORT mScreenViewport;				//�ӿ�
		//��������
		DirectX::Mouse* mMouse;
		DirectX::Mouse::ButtonStateTracker mMouseTracker;
		DirectX::Keyboard* mKeyboard;
		DirectX::Keyboard::KeyboardStateTracker mKeyboardTracker;
	
		// ������Ӧ���ڹ��캯�����ú���Щ�Զ���ĳ�ʼ����
		std::wstring mMainWndCaption;		                   // �����ڱ���
		uint16_t mClientWidth;												   // �ӿڿ��
		uint16_t mClientHeight;
	};
}

#endif // !APP3D_H


