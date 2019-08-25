#include "GameApp.h"
#include "tools/Util.h"

using namespace DirectX;
using namespace Potato;

GameApp::GameApp(HINSTANCE hInstance, uint16_t width, uint16_t height)
	:App3D(hInstance, width, height), cameraSpeed(8)
{
	
}

GameApp::~GameApp()
{
	ReleaseObject(mFont);
	ReleaseObject(mColorBrush);
	ReleaseObject(mTextFormat);
	DeleteObject(mCamera);
	DeleteObject(mShadowCamera);
	DeleteObject(mSkySunset);
	DeleteObject(mTerrain)

	RenderStates::ReleaseAll();
}

bool GameApp::Init()
{
	if (!App3D::Init())
		return false;

	RenderStates::InitAll(md3dDevice);

	defaultEffect.InitAll(md3dDevice);
	skyEffect.InitAll(md3dDevice);
	terrainEffect.InitAll(md3dDevice);

	if (!InitResource())
		return false;

	return true;
}

void GameApp::OnResize()
{
	assert(md2dFactory);
	assert(mdwriteFactory);

	// 释放D2D的相关资源
	ReleaseObject(mColorBrush);
	ReleaseObject(md2dRenderTarget);

	App3D::OnResize();

	// 为D2D创建DXGI表面渲染目标
	IDXGISurface* surface;
	HR(mSwapChain->GetBuffer(0, __uuidof(IDXGISurface), reinterpret_cast<void**>(&surface)));
	D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties(
		D2D1_RENDER_TARGET_TYPE_DEFAULT,
		D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED));
	HRESULT hr = md2dFactory->CreateDxgiSurfaceRenderTarget(surface, &props, &md2dRenderTarget);
	ReleaseObject(surface);

	if (hr == E_NOINTERFACE)
	{
		OutputDebugString(L"\n警告：Direct2D与Direct3D互操作性功能受限，你将无法看到文本信息。现提供下述可选方法：\n"
			"1. 对于Win7系统，需要更新至Win7 SP1，并安装KB2670838补丁以支持Direct2D显示。\n"
			"2. 自行完成Direct3D 10.1与Direct2D的交互。详情参阅："
			"3. 使用别的字体库，比如FreeType。\n\n");
	}
	else if (hr == S_OK)
	{
		// 创建固定颜色刷和文本格式
		HR(md2dRenderTarget->CreateSolidColorBrush(
			D2D1::ColorF(D2D1::ColorF::White),
			&mColorBrush));
		HR(mdwriteFactory->CreateTextFormat(L"宋体", nullptr, DWRITE_FONT_WEIGHT_NORMAL,
			DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 15, L"zh-cn",
			&mTextFormat));
	}
	else
	{
		// 报告异常问题
		assert(md2dRenderTarget);
	}
}

void GameApp::UpdateScene(float dt)
{
	// 更新鼠标事件，获取相对偏移量
	Mouse::State mouseState = mMouse->GetState();
	Mouse::State lastMouseState = mMouseTracker.GetLastState();
	mMouseTracker.Update(mouseState);

	Keyboard::State keyState = mKeyboard->GetState();
	mKeyboardTracker.Update(keyState);

	if (keyState.IsKeyDown(Keyboard::W))
		mCamera->MoveForward(dt * cameraSpeed);
	if (keyState.IsKeyDown(Keyboard::S))
		mCamera->MoveForward(dt * -cameraSpeed);
	if (keyState.IsKeyDown(Keyboard::A))
		mCamera->Strafe(dt * -cameraSpeed);
	if (keyState.IsKeyDown(Keyboard::D))
		mCamera->Strafe(dt * cameraSpeed);

	/* map */

	if (mKeyboardTracker.IsKeyPressed(Keyboard::D1))
	{
		mapOffest += 1;
	}
	if (mKeyboardTracker.IsKeyPressed(Keyboard::D2))
	{
		mapOffest -= 1;
	}


	// 视野旋转，防止开始的差值过大导致的突然旋转
	mCamera->Pitch(mouseState.y * dt * 1.25f);
	mCamera->Roll(mouseState.x * dt * 1.25f);

	// 更新观察矩阵
	mCamera->UpdateViewMatrix();
	defaultEffect.SetViewMatrix(mCamera->ViewMatrix());
	defaultEffect.SetEyePos(mCamera->PositionVector());

	terrainEffect.SetViewMatrix(mCamera->ViewPerspectiveProjectionMatrix());
	terrainEffect.SetEyePos(mCamera->PositionVector());

	defaultEffect.SetLightViewMatrix(mShadowCamera->ViewMatrix());
	defaultEffect.SetLightProjMatrix(mShadowCamera->PerspectiveProjectionMatrix());
	defaultEffect.SetLightPos(mShadowCamera->PositionVector());

	// 退出程序
	if (mKeyboardTracker.IsKeyPressed(Keyboard::Escape))
	{
		SendMessage(MainWnd(), WM_DESTROY, 0, 0);
	}
}

void GameApp::DrawScene()
{
	assert(md3dImmediateContext);
	assert(mSwapChain);

	md3dImmediateContext->ClearRenderTargetView(mRenderTargetView, reinterpret_cast<const float*>(&DirectX::Colors::Black));
	md3dImmediateContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	
	//***************************************************
	//  物体绘制
	//***************************************************
	terrainEffect.SetRenderDefault(md3dImmediateContext);
	mTerrain->DrawTerrain(md3dImmediateContext, &terrainEffect);

	skyEffect.SetRenderDefault(md3dImmediateContext);
	mSkySunset->DrawSky(md3dImmediateContext, &skyEffect, mCamera);

	defaultEffect.SetRenderDefault(md3dImmediateContext);
	mBox.DrawObject(md3dImmediateContext, &defaultEffect);
	//people.DrawObject(md3dImmediateContext, &defaultEffect);
	//sphere.DrawObject(md3dImmediateContext, &defaultEffect);


	//***************************************************
	// ******************


	// 绘制Direct2D部分
	//
	if (md2dRenderTarget != nullptr)
	{
		md2dRenderTarget->BeginDraw();
		std::wstring text = L"当前摄像机模式: 第一人称  Esc退出\n"
			"鼠标移动控制视野 W/S/A/D 移动\n"
			"阴影:";
		text += (mShadowEnabled ? L"开启" : L"关闭");
		text += L"\nBits:" + std::to_wstring(mShadowBits);

		md2dRenderTarget->DrawTextW(text.c_str(), (UINT)text.length(), mTextFormat, D2D1_RECT_F{ 0.0f, 0.0f, 600.0f, 200.0f }, mColorBrush);
		HR(md2dRenderTarget->EndDraw());
	}

	HR(mSwapChain->Present(0, 0));
}

bool GameApp::InitResource()
{
	mBox.SetModel(md3dDevice, VertexType_PosNormalTex, "Model\\box.obj");
	mBox.SetPosition(0.0f, -1.0f, 0.0f);
	mBox.CreateWorldMatrix();

	people.SetModel(md3dDevice, VertexType_PosNormalTex, "Model\\body_bone_165_02.FBX");
	people.SetPosition(0.0f, 0.0f, 10.0f);
	people.CreateWorldMatrix();

	//sphere.SetModel(md3dDevice, VertexType_PosNormalTex, Geometry::CreateSphere(5.0f));
	//sphere.SetPosition(0.0f, 2.0f, 0.0f);
	//sphere.CreateWorldMatrix();

	mSkySunset = new RenderSky(md3dDevice, md3dImmediateContext,
		std::vector<std::wstring>{
		L"Model/texture/sunset_posX.bmp", L"Model/texture/sunset_negX.bmp",
			L"Model/texture/sunset_posY.bmp", L"Model/texture/sunset_negY.bmp",
			L"Model/texture/sunset_posZ.bmp", L"Model/texture/sunset_negZ.bmp", },
		10000.0f);

	mTerrain = new RenderTerrain(md3dDevice, 100, "texture/heightmap.bmp");

	// 摄像机
	mCamera = new FirstPersonGameCamera(mClientWidth, mClientHeight);
	mCamera->SetPosition(0.0f, 0.0f, -20.0f);
	mCamera->SetDirection(0.0f, 0.0f, 1.0f);
	mCamera->SetViewPort(0.0f, 0.0f, (float)mClientWidth, (float)mClientHeight);

	mShadowCamera = new FirstPersonGameCamera((float)mClientWidth, (float)mClientHeight);
	mShadowCamera->SetPosition(0.0f, 20.0f, 0.0f);
	//mShadowCamrea->SetDirection(0.0f, -1.0f,  0.0f);
	mShadowCamera->UpdateViewMatrix();


	defaultEffect.SetFogColor(XMVectorSet(0.8f, 0.8f, 0.8f, 1.0f));
	defaultEffect.SetFogStart(20.0f);
	defaultEffect.SetFogRange(120.0f);

	// 方向光(默认)
	directionlight dirLight;
	dirLight.ambient = XMFLOAT4(0.25f, 0.25f, 0.25f, 1.0f);
	dirLight.diffuse = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
	dirLight.specular = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
	dirLight.direction = XMFLOAT3(-0.866f, -0.5f, 0.5f);

	pointlight ponitLight;
	ponitLight.ambient = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
	ponitLight.diffuse = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
	ponitLight.specular = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
	ponitLight.position = XMFLOAT3(0.0f, 300.0f, 0.0f);;
	ponitLight.att = XMFLOAT3(2.0f, 0.0f, 0.0f);
	ponitLight.range = 1000.0f;

	defaultEffect.SetDirLight(0, dirLight);
	defaultEffect.SetPointLight(0, ponitLight);
	defaultEffect.SetNumLight(1, 0, 0);

	terrainEffect.SetDirLight(0, dirLight);
	terrainEffect.SetPointLight(0, ponitLight);
	terrainEffect.SetNumLight(1, 0, 0);

	material mat;
	mat.ambient = { 0.3f, 0.3f, 0.3f,1.0f };
	mat.diffuse = { 0.8f, 0.8f, 0.8f, 1.0f };
	mat.specular = { 0.1f, 0.1f, 0.1f, 1.0f };
	defaultEffect.SetMaterial(mat);

	// 开启雾效
	defaultEffect.SetFogEnabled(false);

	// 关闭阴影
	mShadowEnabled = 0;
	mShadowBits = 0.000001f;
	defaultEffect.SetShadowEnabled(mShadowEnabled);
	defaultEffect.SetShadowBits(mShadowBits);

	defaultEffect.SetViewMatrix(mCamera->ViewMatrix());
	defaultEffect.SetProjMatrix(mCamera->PerspectiveProjectionMatrix());

	terrainEffect.SetViewMatrix(mCamera->ViewPerspectiveProjectionMatrix());

	defaultEffect.SetLightViewMatrix(mShadowCamera->ViewMatrix());
	defaultEffect.SetLightProjMatrix(mShadowCamera->OrthogonalProjectionMatrix());
	

	return true;
}
