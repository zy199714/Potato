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

 //添加所有要引用的库
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
	
		HINSTANCE AppInst() const;			//获取应用实例句柄
		HWND MainWnd() const;					//	获取窗口句柄
		float AspectRatio() const;					//获取屏幕宽高比
	
		int Run();				//运行程序，执行消息循环
	
		virtual bool Init();				//初始化窗口、Direct3D、Direct2D部分
		virtual void OnResize();		//窗口大小改变是调用
		virtual void UpdateScene(float dt) = 0;				//完成每一帧的更新
		virtual void DrawScene() = 0;				//完成每一帧的绘制
		virtual LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);		//消息回调
	
	protected:
		bool InitMainWindow();
		bool InitDirect2D();
		bool InitDirect3D();
	
		void CalculateFrameStats();		//计算每秒帧数并在窗口显示
	
	protected:
		HINSTANCE mhAppInst;			//应用实例句柄
		HWND mhMainWnd;				//窗口实例句柄
		bool mAppPaused;					//是否暂停
		bool mMinimized;						//是否最小化
		bool mMaximized;					//是否最大化
		bool mResizing;							//是否窗口大小变化
		bool mEnable4xMsaa;				//是否开启4倍多重采样
		UINT m4xMsaaQuality;				//MSAA支持的质量等级
	
		GameTimer mTimer;		//计时器
	
		// Direct2D
		ID2D1Factory* md2dFactory;
		ID2D1RenderTarget* md2dRenderTarget;
		IDWriteFactory* mdwriteFactory;
		//Direct3D
		ID3D11Device* md3dDevice;
		ID3D11DeviceContext* md3dImmediateContext;
		IDXGISwapChain* mSwapChain;

		// 调试对象释放
#if defined(DEBUG) | defined(_DEBUG)
		ID3D11Debug* d3d11Debug;
#endif

		//常用资源
		ID3D11Texture2D* mDepthStencilBuffer;
		ID3D11RenderTargetView* mRenderTargetView;
		ID3D11DepthStencilView* mDepthStencilView;
		D3D11_VIEWPORT mScreenViewport;				//视口
		//键鼠输入
		DirectX::Mouse* mMouse;
		DirectX::Mouse::ButtonStateTracker mMouseTracker;
		DirectX::Keyboard* mKeyboard;
		DirectX::Keyboard::KeyboardStateTracker mKeyboardTracker;
	
		// 派生类应该在构造函数设置好这些自定义的初始参数
		std::wstring mMainWndCaption;		                   // 主窗口标题
		uint16_t mClientWidth;												   // 视口宽度
		uint16_t mClientHeight;
	};
}

#endif // !APP3D_H


