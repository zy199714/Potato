#include "Effects.h"

using namespace Potato;

class DefaultEffect::Impl : public AligendType<DefaultEffect::Impl>
{
public:
	/* 这些结构体对应HLSL的结构体。需要按16字节对齐 */
	struct CBChangesEveryInstanceDrawing
	{
		DirectX::XMMATRIX world;
		DirectX::XMMATRIX worldInvTranspose;
	};

	struct CBChangesEveryObjectDrawing
	{
		material mat;
	};

	struct CBDrawingStates
	{
		DirectX::XMVECTOR fogColor;
		int fogEnabled;
		float fogStart;
		float fogRange;
		int textureUsed;
		int shadowEnabled;
		float shadowBits;
		float pad1;
		float pad2;
	};
	struct CBChangesEveryFrame
	{
		DirectX::XMMATRIX view;
		DirectX::XMMATRIX lightView;
		DirectX::XMVECTOR lightPosition;
		DirectX::XMVECTOR eyePos;
	};
	struct CBChangesOnResize
	{
		DirectX::XMMATRIX LightProj;
		DirectX::XMMATRIX proj;
	};
	struct CBChangesRarely
	{
		directionlight dirLight[EffectBase::maxLights];
		pointlight pointLight[EffectBase::maxLights];
		spotlight spotLight[EffectBase::maxLights];
		int numDirLight;
		int numPointLight;
		int numSpotLight;
		int pad3;
	};
public:
	Impl() {};
	~Impl()
	{
		ReleaseObject(basicInstanceVS);
		ReleaseObject(basicObjectVS);
		ReleaseObject(shadowMapInstanceVS);
		ReleaseObject(shadowMapObjectVS);
		ReleaseObject(basicPS);		
		ReleaseObject(shadowPS);
		ReleaseObject(instancePosNormalTexLayout);
		ReleaseObject(vertexPosNormalTexLayout);
		ReleaseObject(textureDiffuse);
		ReleaseObject(textureShadow);
	};

public:
	// 需要16字节对齐的优先放在前面
	ContentBufferObject<0, CBChangesEveryInstanceDrawing>	cbInstDrawing;		// 每次实例绘制的常量缓冲区
	ContentBufferObject<1, CBChangesEveryObjectDrawing>	cbObjDrawing;		// 每次对象绘制的常量缓冲区
	ContentBufferObject<2, CBDrawingStates>				cbStates;			// 每次绘制状态改变的常量缓冲区
	ContentBufferObject<3, CBChangesEveryFrame>			cbFrame;			// 每帧绘制的常量缓冲区
	ContentBufferObject<4, CBChangesOnResize>				cbOnResize;			// 每次窗口大小变更的常量缓冲区
	ContentBufferObject<5, CBChangesRarely>				cbRarely;			// 几乎不会变更的常量缓冲区
	BOOL isDirty;											// 是否有值变更
	std::vector<ContentBufferBase*> cBufferPtrs;					// 统一管理上面所有的常量缓冲区

	ID3D11VertexShader* basicInstanceVS;
	ID3D11VertexShader* basicObjectVS;
	ID3D11VertexShader* shadowMapInstanceVS;
	ID3D11VertexShader* shadowMapObjectVS;

	ID3D11PixelShader* basicPS;
	ID3D11PixelShader* shadowPS;

	ID3D11InputLayout* instancePosNormalTexLayout;
	ID3D11InputLayout* vertexPosNormalTexLayout;

	ID3D11ShaderResourceView* textureDiffuse;				// 漫反射纹理
	ID3D11ShaderResourceView* textureShadow;			// shadow map

};

namespace
{
	// DefaultEffect单例
	static DefaultEffect * pInstance = nullptr;
}


DefaultEffect::DefaultEffect()
{
	if (pInstance)
	{
		throw std::exception("DefaultEffect is a singleton!");
	}
	pInstance = this;
	pImpl = std::make_unique<Impl>();
}


DefaultEffect::~DefaultEffect()
{
}

bool DefaultEffect::InitAll(ID3D11Device* device)
{
	if (!device)
		return false;

	if (!pImpl->cBufferPtrs.empty())
		return true;

	if (!RenderStates::IsInit())
		throw std::exception("RenderStates need to be initialized first!");

	ID3DBlob* blob;

	// ******************
	// 创建顶点着色器
	//
	HR(CreateShaderFromFile(L"HLSL\\BasicObject_VS.cso", L"HLSL\\BasicObject_VS.hlsl", "VS", "vs_5_0", &blob));
	HR(device->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &pImpl->basicObjectVS));
	ReleaseObject(blob);
	HR(CreateShaderFromFile(L"HLSL\\ShadowMapObject_VS.cso", L"HLSL\\ShadowMapObject_VS.hlsl", "VS", "vs_5_0", &blob));
	HR(device->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &pImpl->shadowMapObjectVS));
	// 创建顶点布局
	HR(device->CreateInputLayout(InputLayout[VertexType_PosNormalTex], InputLayoutCount[VertexType_PosNormalTex],
		blob->GetBufferPointer(), blob->GetBufferSize(), &pImpl->vertexPosNormalTexLayout));
	ReleaseObject(blob);
	HR(CreateShaderFromFile(L"HLSL\\BasicInstance_VS.cso", L"HLSL\\BasicInstance_VS.hlsl", "VS", "vs_5_0", &blob));
	HR(device->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &pImpl->basicInstanceVS));
	ReleaseObject(blob);
	HR(CreateShaderFromFile(L"HLSL\\ShadowMapInstance_VS.cso", L"HLSL\\ShadowMapInstance_VS.hlsl", "VS", "vs_5_0", &blob));
	HR(device->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &pImpl->shadowMapInstanceVS));
	// 创建顶点布局
	HR(device->CreateInputLayout(InputLayout[VertexType_Instance_PosNormalTex_WorldWorldT], InputLayoutCount[VertexType_Instance_PosNormalTex_WorldWorldT],
		blob->GetBufferPointer(), blob->GetBufferSize(), &pImpl->instancePosNormalTexLayout));

	// ******************
	// 创建像素着色器
	//ShadowMap_PS
	ReleaseObject(blob);
	HR(CreateShaderFromFile(L"HLSL\\Basic_PS.cso", L"HLSL\\Basic_PS.hlsl", "PS", "ps_5_0", &blob));
	HR(device->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &pImpl->basicPS));
	ReleaseObject(blob);
	HR(CreateShaderFromFile(L"HLSL\\ShadowMap_PS.cso", L"HLSL\\ShadowMap_PS.hlsl", "PS", "ps_5_0", &blob));
	HR(device->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &pImpl->shadowPS));

	pImpl->cBufferPtrs.assign({
		&pImpl->cbInstDrawing,
		&pImpl->cbObjDrawing,
		&pImpl->cbStates,
		&pImpl->cbFrame,
		&pImpl->cbOnResize,
		&pImpl->cbRarely });

	// 创建常量缓冲区
	for (auto& pBuffer : pImpl->cBufferPtrs)
	{
		HR(pBuffer->CreateBuffer(device));
	}

	return true;
}


void DefaultEffect::SetRenderDefault(ID3D11DeviceContext* deviceContext)
{
	if (mRenderType == RenderType_Object)
	{
		deviceContext->IASetInputLayout(pImpl->vertexPosNormalTexLayout);
		deviceContext->VSSetShader(pImpl->basicObjectVS, nullptr, 0);
	}
	else if (mRenderType == RenderType_Instance)
	{	
		deviceContext->IASetInputLayout(pImpl->instancePosNormalTexLayout);
		deviceContext->VSSetShader(pImpl->basicInstanceVS, nullptr, 0);
	}
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	deviceContext->GSSetShader(nullptr, nullptr, 0);
	deviceContext->RSSetState(nullptr);
	deviceContext->PSSetShader(pImpl->basicPS, nullptr, 0);
	deviceContext->PSSetSamplers(0, 1, &RenderStates::SSLinearWrap);
	deviceContext->PSSetSamplers(1, 1, &RenderStates::SSLinearBorder);
	deviceContext->PSSetSamplers(2, 1, &RenderStates::SSPonitBorder);
	deviceContext->OMSetDepthStencilState(nullptr, 0);
	deviceContext->OMSetBlendState(nullptr, nullptr, 0xFFFFFFFF);
}


void DefaultEffect::SetRenderShadowMap(ID3D11DeviceContext* deviceContext)
{
	if (mRenderType == RenderType_Object)
	{
		deviceContext->IASetInputLayout(pImpl->vertexPosNormalTexLayout);
		deviceContext->VSSetShader(pImpl->shadowMapObjectVS, nullptr, 0);
	}
	else if (mRenderType == RenderType_Instance)
	{
		deviceContext->IASetInputLayout(pImpl->instancePosNormalTexLayout);
		deviceContext->VSSetShader(pImpl->shadowMapInstanceVS, nullptr, 0);
	}
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	deviceContext->GSSetShader(nullptr, nullptr, 0);
	deviceContext->RSSetState(nullptr);
	deviceContext->PSSetShader(pImpl->shadowPS, nullptr, 0);
	deviceContext->PSSetSamplers(0, 1, &RenderStates::SSLinearWrap);
	deviceContext->PSSetSamplers(1, 1, &RenderStates::SSLinearBorder);
	deviceContext->OMSetDepthStencilState(nullptr, 0);
	deviceContext->OMSetBlendState(nullptr, nullptr, 0xFFFFFFFF);
}


void DefaultEffect::SetRendertype(RenderType type)
{
	mRenderType = type;
}

void DefaultEffect::Apply(ID3D11DeviceContext* deviceContext)
{
	auto& pCBuffers = pImpl->cBufferPtrs;
	// 将缓冲区绑定到渲染管线上
	pCBuffers[0]->BindVS(deviceContext);
	pCBuffers[3]->BindVS(deviceContext);
	pCBuffers[4]->BindVS(deviceContext);

	pCBuffers[1]->BindPS(deviceContext);
	pCBuffers[2]->BindPS(deviceContext);
	pCBuffers[3]->BindPS(deviceContext);
	pCBuffers[4]->BindPS(deviceContext);
	pCBuffers[5]->BindPS(deviceContext);

	// 设置纹理
	deviceContext->PSSetShaderResources(0, 1, &pImpl->textureDiffuse);
	deviceContext->PSSetShaderResources(1, 1, &pImpl->textureShadow);

	if (pImpl->isDirty)
	{
		pImpl->isDirty = false;
		for (auto& pCBuffer : pCBuffers)
		{
			pCBuffer->UpdateBuffer(deviceContext);
		}
	}
}

void XM_CALLCONV DefaultEffect::SetWorldViewProjMatrix(DirectX::FXMMATRIX W, DirectX::CXMMATRIX V, DirectX::CXMMATRIX P)
{
	SetWorldMatrix(W);
	SetViewMatrix(V);
	SetProjMatrix(P);
}

void XM_CALLCONV DefaultEffect::SetWorldMatrix(DirectX::FXMMATRIX W)
{
	auto& cBuffer = pImpl->cbInstDrawing;
	cBuffer.data.world = XMMatrixTranspose(W);
	cBuffer.data.worldInvTranspose = XMMatrixInverse(nullptr, W);
	pImpl->isDirty = cBuffer.isDirty = true;
}

void XM_CALLCONV DefaultEffect::SetViewMatrix(FXMMATRIX V)
{
	auto& cBuffer = pImpl->cbFrame;
	cBuffer.data.view = XMMatrixTranspose(V);
	pImpl->isDirty = cBuffer.isDirty = true;
}

void XM_CALLCONV DefaultEffect::SetProjMatrix(FXMMATRIX P)
{
	auto& cBuffer = pImpl->cbOnResize;
	cBuffer.data.proj = XMMatrixTranspose(P);
	pImpl->isDirty = cBuffer.isDirty = true;
}

void XM_CALLCONV DefaultEffect::SetLightViewMatrix(DirectX::FXMMATRIX LV)
{
	auto& cBuffer = pImpl->cbFrame;
	cBuffer.data.lightView = XMMatrixTranspose(LV);
	pImpl->isDirty = cBuffer.isDirty = true;
}

void XM_CALLCONV DefaultEffect::SetLightProjMatrix(DirectX::FXMMATRIX LP)
{
	auto& cBuffer = pImpl->cbOnResize;
	cBuffer.data.LightProj = XMMatrixTranspose(LP);
	pImpl->isDirty = cBuffer.isDirty = true;
}

void DefaultEffect::SetDirLight(size_t pos, const directionlight & dirLight)
{
	auto& cBuffer = pImpl->cbRarely;
	cBuffer.data.dirLight[pos] = dirLight;
	pImpl->isDirty = cBuffer.isDirty = true;
}

void DefaultEffect::SetPointLight(size_t pos, const pointlight & pointLight)
{
	auto& cBuffer = pImpl->cbRarely;
	cBuffer.data.pointLight[pos] = pointLight;
	pImpl->isDirty = cBuffer.isDirty = true;
}

void DefaultEffect::SetSpotLight(size_t pos, const spotlight & spotLight)
{
	auto& cBuffer = pImpl->cbRarely;
	cBuffer.data.spotLight[pos] = spotLight;
	pImpl->isDirty = cBuffer.isDirty = true;
}

void DefaultEffect::SetNumLight(int dir, int point, int spot)
{
	auto& cBuffer = pImpl->cbRarely;
	cBuffer.data.numDirLight = dir;
	cBuffer.data.numPointLight = point;
	cBuffer.data.numSpotLight = spot;
	pImpl->isDirty = cBuffer.isDirty = true;
}

void DefaultEffect::SetMaterial(const material& mat)
{
	auto& cBuffer = pImpl->cbObjDrawing;
	cBuffer.data.mat = mat;
	pImpl->isDirty = cBuffer.isDirty = true;
}

void DefaultEffect::SetTextureUsed(bool isUsed)
{
	auto& cBuffer = pImpl->cbStates;
	cBuffer.data.textureUsed = isUsed;
	pImpl->isDirty = cBuffer.isDirty = true;
}

void DefaultEffect::SetTexture(ID3D11DeviceContext* devicecontext, UINT startslot, UINT numview, ID3D11ShaderResourceView* texture)
{
	devicecontext->PSSetShaderResources(startslot, numview, &texture);
}

void DefaultEffect::SetTextureShadow(ID3D11ShaderResourceView* textureShadow)
{
	pImpl->textureShadow = textureShadow;
}

void XM_CALLCONV DefaultEffect::SetEyePos(FXMVECTOR eyePos)
{
	auto& cBuffer = pImpl->cbFrame;
	cBuffer.data.eyePos = eyePos;
	pImpl->isDirty = cBuffer.isDirty = true;
}

void XM_CALLCONV DefaultEffect::SetLightPos(FXMVECTOR LightPos)
{
	auto& cBuffer = pImpl->cbFrame;
	cBuffer.data.lightPosition = LightPos;
	pImpl->isDirty = cBuffer.isDirty = true;
}

void DefaultEffect::SetFogEnabled(bool isOn)
{
	auto& cBuffer = pImpl->cbStates;
	cBuffer.data.fogEnabled = isOn;
	pImpl->isDirty = cBuffer.isDirty = true;
}


void DefaultEffect::SetFog(DirectX::XMVECTOR fogColor, float fogStart, float fogRange)
{
	auto& cBuffer = pImpl->cbStates;
	cBuffer.data.fogColor = fogColor;
	cBuffer.data.fogStart = fogStart;
	cBuffer.data.fogRange = fogRange;
	pImpl->isDirty = cBuffer.isDirty = true;
}

void DefaultEffect::SetFogStart(float fogStart)
{
	auto& cBuffer = pImpl->cbStates;
	cBuffer.data.fogStart = fogStart;
	pImpl->isDirty = cBuffer.isDirty = true;
}

void DefaultEffect::SetFogColor(DirectX::XMVECTOR fogColor)
{
	auto& cBuffer = pImpl->cbStates;
	cBuffer.data.fogColor = fogColor;
	pImpl->isDirty = cBuffer.isDirty = true;
}

void DefaultEffect::SetFogRange(float fogRange)
{
	auto& cBuffer = pImpl->cbStates;
	cBuffer.data.fogRange = fogRange;
	pImpl->isDirty = cBuffer.isDirty = true;
}

void DefaultEffect::SetShadowEnabled(bool isOn)
{
	auto& cBuffer = pImpl->cbStates;
	cBuffer.data.shadowEnabled = isOn;
	pImpl->isDirty = cBuffer.isDirty = true;
}
void DefaultEffect::SetShadowBits(float bits)
{
	auto& cBuffer = pImpl->cbStates;
	cBuffer.data.shadowBits = bits;
	pImpl->isDirty = cBuffer.isDirty = true;
}
