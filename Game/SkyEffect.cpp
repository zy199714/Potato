#include "Effects.h"

using namespace Potato;

class SkyEffect::Impl : public AligendType<SkyEffect::Impl>
{
public:
	struct CBChangesEveryFrame
	{
		DirectX::XMMATRIX worldViewProj;
	};
public:
	// 必须显式指定
	Impl() {};
	~Impl()
	{
		ReleaseObject(skyVS);
		ReleaseObject(skyPS);
		ReleaseObject(vertexPosLayout);
		// 已在RenderSky.cpp 中释放
		//ReleaseObject(textureCube);
	};

public:
	ContentBufferObject<0, CBChangesEveryFrame>	cbFrame;	// 每帧绘制的常量缓冲区

	BOOL isDirty;										// 是否有值变更
	std::vector<ContentBufferBase*> cBufferPtrs;				// 统一管理上面所有的常量缓冲区

	ID3D11VertexShader* skyVS;
	ID3D11PixelShader* skyPS;

	ID3D11InputLayout* vertexPosLayout;

	ID3D11ShaderResourceView* textureCube;			// 天空盒纹理
};

namespace
{
	// SkyEffect单例
	static SkyEffect * pInstance = nullptr;
}

SkyEffect::SkyEffect()
{
	if (pInstance)
		throw std::exception("SkyEffect is a singleton!");
	pInstance = this;
	pImpl = std::make_unique<SkyEffect::Impl>();
}

SkyEffect::~SkyEffect()
{
}

bool SkyEffect::InitAll(ID3D11Device* device)
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

	HR(CreateShaderFromFile(L"HLSL\\Sky_VS.cso", L"HLSL\\Sky_VS.hlsl", "VS", "vs_5_0", &blob));
	HR(device->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &pImpl->skyVS));
	// 创建顶点布局
	HR(device->CreateInputLayout(InputLayout[VertexType_Pos], InputLayoutCount[VertexType_Pos],
		blob->GetBufferPointer(), blob->GetBufferSize(), &pImpl->vertexPosLayout));

	// ******************
	// 创建像素着色器
	//
	ReleaseObject(blob);
	HR(CreateShaderFromFile(L"HLSL\\Sky_PS.cso", L"HLSL\\Sky_PS.hlsl", "PS", "ps_5_0", &blob));
	HR(device->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &pImpl->skyPS));


	pImpl->cBufferPtrs.assign({
		&pImpl->cbFrame,
		});

	// 创建常量缓冲区
	for (auto& pBuffer : pImpl->cBufferPtrs)
	{
		HR(pBuffer->CreateBuffer(device));
	}

	return true;
}

void SkyEffect::SetRenderDefault(ID3D11DeviceContext* deviceContext)
{
	deviceContext->IASetInputLayout(pImpl->vertexPosLayout);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	deviceContext->VSSetShader(pImpl->skyVS, nullptr, 0);
	deviceContext->GSSetShader(nullptr, nullptr, 0);
	deviceContext->RSSetState(RenderStates::RSNoCull);
	deviceContext->PSSetShader(pImpl->skyPS, nullptr, 0);
	deviceContext->PSSetSamplers(0, 1, &RenderStates::SSLinearWrap);
	deviceContext->OMSetDepthStencilState(RenderStates::DSSLessEqual, 0);
	deviceContext->OMSetBlendState(nullptr, nullptr, 0xFFFFFFFF);
}

void XM_CALLCONV SkyEffect::SetWorldViewProjMatrix(DirectX::FXMMATRIX W, DirectX::CXMMATRIX V, DirectX::CXMMATRIX P)
{
	auto& cBuffer = pImpl->cbFrame;
	cBuffer.data.worldViewProj = XMMatrixTranspose(W * V * P);
	pImpl->isDirty = cBuffer.isDirty = true;
}

void XM_CALLCONV SkyEffect::SetWorldMatrix(DirectX::FXMMATRIX VP)
{
	auto& cBuffer = pImpl->cbFrame;
	cBuffer.data.worldViewProj = XMMatrixTranspose(VP);
	pImpl->isDirty = cBuffer.isDirty = true;
}

void SkyEffect::SetTexture(ID3D11DeviceContext* devicecontext, UINT startslot, UINT numview, ID3D11ShaderResourceView* texture)
{
	devicecontext->PSSetShaderResources(startslot, numview, &texture);
}

void SkyEffect::Apply(ID3D11DeviceContext* deviceContext)
{
	auto& pCBuffers = pImpl->cBufferPtrs;
	// 将缓冲区绑定到渲染管线上
	pCBuffers[0]->BindVS(deviceContext);

	if (pImpl->isDirty)
	{
		pImpl->isDirty = false;
		for (auto& pCBuffer : pCBuffers)
		{
			pCBuffer->UpdateBuffer(deviceContext);
		}
	}
}
