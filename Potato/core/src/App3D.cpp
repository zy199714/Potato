#include "core/App3D.h"
#include "tools/DXTrace.h"
#include "tools/Util.h"
#include <sstream>

using namespace Potato;


namespace
{
	App3D* gApp3D = nullptr;
}

LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return gApp3D->MsgProc(hwnd, msg, wParam, lParam);
}

App3D::App3D(HINSTANCE hInstance, uint16_t width, uint16_t height)
	:mhAppInst(hInstance),
	mMainWndCaption(L"d3dapp"),
	mClientWidth(width),
	mClientHeight(height),
	mhMainWnd(nullptr),
	mAppPaused(false),
	mMinimized(false),
	mMaximized(false),
	mResizing(false),
	mEnable4xMsaa(true),
	m4xMsaaQuality(0),
	md3dDevice(nullptr),
	md3dImmediateContext(nullptr),
	mSwapChain(nullptr),
	mDepthStencilBuffer(nullptr),
	mRenderTargetView(nullptr),
	mDepthStencilView(nullptr)
{
	ZeroMemory(&mScreenViewport, sizeof(D3D11_VIEWPORT));
	
	// ��ȡȫ�ֶ���ָ��
	gApp3D = this;
}

Potato::App3D::~App3D()
{
	ReleaseObject(md2dFactory);
	ReleaseObject(md2dRenderTarget);
	ReleaseObject(mdwriteFactory);

	ReleaseObject(mDepthStencilBuffer);
	ReleaseObject(mDepthStencilView);	
	ReleaseObject(mRenderTargetView);

	DeleteObject(mMouse);
	DeleteObject(mKeyboard);

	ReleaseObject(mSwapChain);
	ReleaseObject(md3dImmediateContext);

#if defined(DEBUG) | defined(_DEBUG)
	// ��ʼ�����Զ���
	HRESULT hr = md3dDevice->QueryInterface(__uuidof(ID3D11Debug), reinterpret_cast<void**>(&d3d11Debug));
	if (SUCCEEDED(hr))
	{
		hr = d3d11Debug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
	}
	ReleaseObject(d3d11Debug);
#endif

	ReleaseObject(md3dDevice);
}

HINSTANCE App3D::AppInst() const 
{
	return mhAppInst;
}

HWND App3D::MainWnd() const
{
	return mhMainWnd;
}

float App3D::AspectRatio() const
{
	return static_cast<float>(mClientWidth / mClientHeight);
}

int App3D::Run()
{
	MSG msg = { 0 };

	mTimer.Reset();

	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			mTimer.Tick();
			if (!mAppPaused)
			{
				CalculateFrameStats();
				UpdateScene(mTimer.DeltaTime());
				DrawScene();
			}
			else
			{
				// ��Ϣһ��
				Sleep(100);
			}
		}
	}
	return msg.wParam;
}

bool App3D::Init()
{
	if (!InitMainWindow())
	{
		return false;
	}

	mMouse = new DirectX::Mouse();
	// ��ʼ�����
	mMouse->SetWindow(mhMainWnd);
	mMouse->SetMode(DirectX::Mouse::MODE_RELATIVE);
	mKeyboard = new DirectX::Keyboard();

	if (!InitDirect2D())
	{
		return false;
	}

	if (!InitDirect3D())
	{
		return false;
	}

	return true;
}

void App3D::OnResize()
{
	assert(md3dImmediateContext);
	assert(md3dDevice);
	assert(mSwapChain);

	// �ͷŽ�������Դ
	ReleaseObject(mRenderTargetView);
	ReleaseObject(mDepthStencilView);
	ReleaseObject(mDepthStencilBuffer);

	ID3D11Texture2D* backbuffer;
	HR(mSwapChain->ResizeBuffers(1, mClientWidth, mClientHeight, DXGI_FORMAT_R8G8B8A8_UNORM, 0));
	HR(mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backbuffer)));
	HR(md3dDevice->CreateRenderTargetView(backbuffer, nullptr, &mRenderTargetView));
	ReleaseObject(backbuffer);

	D3D11_TEXTURE2D_DESC depthStencilBufferDesc;
	depthStencilBufferDesc.Width = mClientWidth;
	depthStencilBufferDesc.Height = mClientHeight;
	depthStencilBufferDesc.MipLevels = 1;
	depthStencilBufferDesc.ArraySize = 1;
	depthStencilBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	if (mEnable4xMsaa)
	{
		depthStencilBufferDesc.SampleDesc.Count = 4;
		depthStencilBufferDesc.SampleDesc.Quality = m4xMsaaQuality - 1;
	}
	else
	{
		depthStencilBufferDesc.SampleDesc.Count = 0;
		depthStencilBufferDesc.SampleDesc.Quality = 0;
	}
	depthStencilBufferDesc.Usage = D3D11_USAGE_DEFAULT;	//	 GPU �ɶ���д
	depthStencilBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilBufferDesc.CPUAccessFlags = 0;
	depthStencilBufferDesc.MiscFlags = 0;

	HR(md3dDevice->CreateTexture2D(&depthStencilBufferDesc, nullptr, &mDepthStencilBuffer));
	HR(md3dDevice->CreateDepthStencilView(mDepthStencilBuffer, nullptr, &mDepthStencilView));
	md3dImmediateContext->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);

	// �ӿ�����
	mScreenViewport.TopLeftX = 0;
	mScreenViewport.TopLeftY = 0;
	mScreenViewport.Height = (float)mClientHeight;
	mScreenViewport.Width = (float)mClientWidth;
	mScreenViewport.MinDepth = 0.0f;
	mScreenViewport.MaxDepth = 1.0f;

	md3dImmediateContext->RSSetViewports(1, &mScreenViewport);
}

LRESULT App3D::MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		//�����ȡ�������ʱ����WM_ACTIVATE��
		//������ͣ��ʱ������ͣ��Ϸ
		//������û�Ծʱȡ����ͣ
	case WM_ACTIVATE:
		if (LOWORD(wParam) == WA_INACTIVE)
		{
			mAppPaused = true;
			mTimer.Stop();
		}
		else
		{
			mAppPaused = false;
			mTimer.Start();
		}
		return 0;

		// �����ڴ�С�ı�ʱ WM_SIZE 
	case WM_SIZE:
		// �����´��ڴ�С
		mClientWidth = LOWORD(lParam);
		mClientHeight = HIWORD(lParam);
		if (md3dDevice)
		{
			if (wParam == SIZE_MINIMIZED)
			{
				mAppPaused = true;
				mMinimized = true;
				mMaximized = false;
			}
			else if (wParam == SIZE_MAXIMIZED)
			{
				mAppPaused = false;
				mMinimized = false;
				mMaximized = true;
				OnResize();
			}
			else if (wParam == SIZE_RESTORED)
			{

				// ����С��״̬�ָ���
				if (mMinimized)
				{
					mAppPaused = false;
					mMinimized = false;
					OnResize();
				}

				// �����״̬�ָ���
				else if (mMaximized)
				{
					mAppPaused = false;
					mMaximized = false;
					OnResize();
				}
				else if (mResizing)
				{
					//����û��϶�������С�������ǲ��������С
					//����Ļ�������Ϊ�û�����
					//�϶�������С����WM_SIZE��Ϣ��
					//���͵����ڣ��⽫�Ǻ�������ģ�������
					//Ϊ���϶����յ�ÿ��WM_SIZE��Ϣ������С
					//������С������ͼ�� �����෴���������û�֮������
					//��ɴ��ڴ�С�������ͷŵ�����С��
					//����WM_EXITSIZEMOVE��Ϣ��
				}
				else // API ���� SetWindowPos or mSwapChain->SetFullscreenState.
				{
					OnResize();
				}
			}
		}
		return 0;

		// WM_EXITSIZEMOVE ���ڴ��ڴ�С.
	case WM_ENTERSIZEMOVE:
		mAppPaused = true;
		mResizing = true;
		mTimer.Stop();
		return 0;

		//���û��ͷŵ�����С��ʱ����WM_EXITSIZEMOVE��
		//�������Ǹ����µĴ��ڳߴ������������ݡ�
	case WM_EXITSIZEMOVE:
		mAppPaused = false;
		mResizing = false;
		mTimer.Start();
		OnResize();
		return 0;

		// ���ٴ���
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

		//���˵����ڻ״̬���û�����ʱ��������WM_MENUCHAR��Ϣ
		//���κ����Ǽ�����ټ�����Ӧ�ļ���
	case WM_MENUCHAR:
		// alt-enter ʱ��Ҫ����������
		return MAKELRESULT(0, MNC_CLOSE);

		// ��ֹ���ڱ��̫С
	case WM_GETMINMAXINFO:
		((MINMAXINFO*)lParam)->ptMinTrackSize.x = 200;
		((MINMAXINFO*)lParam)->ptMinTrackSize.y = 200;
		return 0;

		// �����Щ����/����¼�
	case WM_INPUT:

	case WM_LBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_XBUTTONDOWN:

	case WM_LBUTTONUP:
	case WM_MBUTTONUP:
	case WM_RBUTTONUP:
	case WM_XBUTTONUP:

	case WM_MOUSEWHEEL:
	case WM_MOUSEHOVER:
	case WM_MOUSEMOVE:
		mMouse->ProcessMessage(msg, wParam, lParam);
		return 0;

	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
	case WM_KEYUP:
	case WM_SYSKEYUP:
		mKeyboard->ProcessMessage(msg, wParam, lParam);
		return 0;

	case WM_ACTIVATEAPP:
		mMouse->ProcessMessage(msg, wParam, lParam);
		mKeyboard->ProcessMessage(msg, wParam, lParam);
		return 0;
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}

bool App3D::InitMainWindow()
{
	WNDCLASS wc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = MainWndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = mhAppInst;
	wc.hIcon = LoadIcon(0, IDI_APPLICATION);
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
	wc.lpszMenuName = 0;
	wc.lpszClassName = L"D3DWndClassName";

	if (!RegisterClass(&wc))
	{
		MessageBox(0, L"RegisterClass Failed.", 0, 0);
		return false;
	}

	// �����ڵ���������
	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);

	//���㴰�ڴ�С
	RECT R = { 0,0,mClientWidth,mClientHeight };
	AdjustWindowRect(&R, WS_OVERLAPPEDWINDOW, false);
	int width = R.right - R.left;
	int height = R.bottom - R.top;

	mhMainWnd = CreateWindow(L"D3DWndClassName", mMainWndCaption.c_str(),
		WS_OVERLAPPEDWINDOW, (screenWidth - width) / 2, (screenHeight - height) / 2, width, height, 0, 0, mhAppInst, 0);

	if (!mhMainWnd)
	{
		MessageBox(0, L"CreateWindow Failed.", 0, 0);
		return false;
	}

	ShowWindow(mhMainWnd, SW_SHOW);
	UpdateWindow(mhMainWnd);

	return true;
}

bool App3D::InitDirect2D()
{
	HR(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &md2dFactory));
	HR(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory),
		reinterpret_cast<IUnknown**>(&mdwriteFactory)));

	return true;
}

bool App3D::InitDirect3D()
{
	HRESULT hr = S_OK;

	D3D_FEATURE_LEVEL featureLevel;
	D3D_DRIVER_TYPE d3dDriverType;
	UINT numDriverTypes;
	UINT numFeatureLevels;

	IDXGIDevice* dxgiDevice =nullptr;
	IDXGIAdapter* dxgiAdpter = nullptr;
	IDXGIFactory1* dxgiFactory1 = nullptr;	// D3D11.0(����DXGI1.1)�Ľӿ���

	// ���� device ��־
	UINT  createDeviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#if defined(DEBUG) || defined(_DEBUG)
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	// ������������
	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE,
	};
	numDriverTypes = ARRAYSIZE(driverTypes);

	// ���Եȼ�����
	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
	};
	numFeatureLevels = ARRAYSIZE(featureLevels);


	for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; ++driverTypeIndex)
	{
		d3dDriverType = driverTypes[driverTypeIndex];
		hr = D3D11CreateDevice(nullptr, d3dDriverType, nullptr, createDeviceFlags, featureLevels, numFeatureLevels,
			D3D11_SDK_VERSION, &md3dDevice, &featureLevel, &md3dImmediateContext);
		if (hr = E_INVALIDARG)
		{
			hr = D3D11CreateDevice(nullptr, d3dDriverType, nullptr, createDeviceFlags, &featureLevels[1], 1,
				D3D11_SDK_VERSION, &md3dDevice, &featureLevel, &md3dImmediateContext);
		}
		if (SUCCEEDED(hr))
		{
			break;
		}
	}

	if (FAILED(hr))
	{
		MessageBox(0, TEXT("D3D11CreateDevice Failed."), 0, 0);
		return false;
	}

	// ����Ƿ�֧�����Եȼ�11.0��11.1
	if (featureLevel != D3D_FEATURE_LEVEL_11_0 && featureLevel != D3D_FEATURE_LEVEL_11_1)
	{
		MessageBox(0, TEXT("Direct3D Feature Level 11 unsupported."), 0, 0);
		return false;
	}

	//���MSAA֧�ֵ������ȼ�
	md3dDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_SNORM, 4, &m4xMsaaQuality);
	assert(m4xMsaaQuality > 0);

	// Ϊ����ȷ���� DXGI������������������Ҫ��ȡ���� D3D�豸 �� DXGI�������������������
	HR(md3dDevice->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&dxgiDevice)));
	HR(dxgiDevice->GetAdapter(&dxgiAdpter));
	HR(dxgiAdpter->GetParent(__uuidof(IDXGIFactory1), reinterpret_cast<void**>(&dxgiFactory1)));

	DXGI_SWAP_CHAIN_DESC swapDesc;
	ZeroMemory(&swapDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
	swapDesc.BufferCount = 1; // �󱸻�������Ŀ
	swapDesc.BufferDesc.Width = mClientWidth;
	swapDesc.BufferDesc.Height = mClientHeight;
	swapDesc.BufferDesc.RefreshRate.Numerator = 60;	//	����
	swapDesc.BufferDesc.RefreshRate.Denominator = 1;		// ��ĸ
	swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;//��־ָʾդ�������������ϴ���ͼ��ķ���
	swapDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;	//��־ָʾͼ������������ʺϸ����������ķֱ���
	swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	//��������;
	swapDesc.Flags = 0;	//DXGI_SWAP_CHAIN_FLAGö�����͵ĳ�Ա��������������Ϊ��ѡ��
	swapDesc.OutputWindow = mhMainWnd;	//�������
	// �Ƿ���4�����ز���
	if (mEnable4xMsaa)
	{
		swapDesc.SampleDesc.Count = 4;
		swapDesc.SampleDesc.Quality = m4xMsaaQuality - 1;
	}
	else
	{
		swapDesc.SampleDesc.Count = 1;
		swapDesc.SampleDesc.Quality = 0;
	}

	swapDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;		//����Ч��
	swapDesc.Windowed = true;		//�Ƿ񴰿ڻ�

	HR(dxgiFactory1->CreateSwapChain(md3dDevice, &swapDesc, &mSwapChain));

	// ���Խ�ֹalt+enter ȫ��
	dxgiFactory1->MakeWindowAssociation(mhMainWnd, DXGI_MWA_NO_ALT_ENTER | DXGI_MWA_NO_WINDOW_CHANGES);

	// ÿ�����ڱ����µ�����С��ʱ�򣬶���Ҫ�������OnResize���������ڵ���, �Ա�������ظ�
	OnResize();

	ReleaseObject(dxgiDevice);
	ReleaseObject(dxgiAdpter);
	ReleaseObject(dxgiFactory1);

	return true;
}

void App3D::CalculateFrameStats()
{
	// �ô������ÿ��֡�٣�������ÿһ֡��Ⱦ��Ҫ��ʱ�䣬��ʾ�ڴ��ڱ���
	static int frameCount = 0;
	static float timeElapsed = 0.0f;

	frameCount++;

	if ((mTimer.TotalTime() - timeElapsed) >= 1.0f)
	{
		float fps = (float)frameCount; // fps = frameCnt / 1
		float mspf = 1000.0f / fps;

		std::wostringstream outs;
		outs.precision(6);
		outs << mMainWndCaption << L"    "
			<< L"FPS: " << fps << L"    "
			<< L"Frame Time: " << mspf << L" (ms)";
		SetWindowText(mhMainWnd, outs.str().c_str());

		// Reset for next average.
		frameCount = 0;
		timeElapsed += 1.0f;
	}
}

