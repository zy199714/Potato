#include "Effects.h"

using namespace Potato;

class NormalEffect::Impl : public AligendType<NormalEffect::Impl>
{
public:
	struct CBChangesEveryDrawing
	{
		DirectX::XMMATRIX world;
		DirectX::XMMATRIX worldInvTranspose;
	};
	struct CBChangesEveryFrame
	{
		DirectX::XMMATRIX view;
	};
	struct CBChangesOnResize
	{
		DirectX::XMMATRIX proj;
	};
public:
	Impl() {};
	~Impl()
	{
		ReleaseObject(normalVS);
		ReleaseObject(normalGS);
		ReleaseObject(normalPS);
		ReleaseObject(vertexPosNormalTexLayout);
	};
public:
	ContentBufferObject<0, CBChangesEveryDrawing>	cbDrawing;
	ContentBufferObject<1, CBChangesEveryFrame>	cbFrame;
	ContentBufferObject<3, CBChangesOnResize>			cbOnResize;
	BOOL isDirty;											// 是否有值变更
	std::vector<ContentBufferBase*> cBufferPtrs;					// 统一管理上面所有的常量缓冲区

	ID3D11VertexShader* normalVS;

	ID3D11GeometryShader* normalGS;

	ID3D11PixelShader* normalPS;

	ID3D11InputLayout* vertexPosNormalTexLayout;
};

namespace
{
	// DefaultEffect单例
	static NormalEffect * pInstance = nullptr;
}

NormalEffect::NormalEffect()
{
	if (pInstance)
	{
		throw std::exception("NormalEffect is a singleton!");
	}
	pInstance = this;
	pImpl = std::make_unique<Impl>();
}


NormalEffect::~NormalEffect()
{
}

bool NormalEffect::InitAll(ID3D11Device* device)
{
	if (!device)
		return false;

	if (!pImpl->cBufferPtrs.empty())
		return true;

	if (!RenderStates::IsInit())
		throw std::exception("RenderStates need to be initialized first!");

	ID3DBlob* blob;

	// 创建顶点着色器

	HR(CreateShaderFromFile(L"HLSL\\Normal_VS.cso", L"HLSL\\Normal_VS.hlsl", "VS", "vs_5_0", &blob));
	HR(device->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &pImpl->normalVS));
	// 创建顶点布局
	HR(device->CreateInputLayout(InputLayout[VertexType_PosNormalTex], InputLayoutCount[VertexType_PosNormalTex],
		blob->GetBufferPointer(), blob->GetBufferSize(), &pImpl->vertexPosNormalTexLayout));
	ReleaseObject(blob);

	// 创建像素着色器

	HR(CreateShaderFromFile(L"HLSL\\Normal_PS.cso", L"HLSL\\Normal_PS.hlsl", "PS", "ps_5_0", &blob));
	HR(device->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &pImpl->normalPS));
	ReleaseObject(blob);

	// 几何着色器                                                      

	HR(CreateShaderFromFile(L"HLSL\\Normal_GS.cso", L"HLSL\\Normal_GS.hlsl", "GS", "gs_5_0", &blob));
	HR(device->CreateGeometryShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &pImpl->normalGS));
	ReleaseObject(blob);

	pImpl->cBufferPtrs.assign({
		&pImpl->cbDrawing,
		&pImpl->cbFrame,
		&pImpl->cbOnResize});

	// 创建常量缓冲区
	for (auto& pBuffer : pImpl->cBufferPtrs)
	{
		HR(pBuffer->CreateBuffer(device));
	}

	return true;
}

void NormalEffect::SetRenderDefault(ID3D11DeviceContext* deviceContext)
{
	deviceContext->IASetInputLayout(pImpl->vertexPosNormalTexLayout);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	deviceContext->VSSetShader(pImpl->normalVS, nullptr, 0);
	deviceContext->GSSetShader(pImpl->normalGS, nullptr, 0);
	deviceContext->RSSetState(nullptr);
	deviceContext->PSSetShader(pImpl->normalPS, nullptr, 0);
	deviceContext->OMSetDepthStencilState(nullptr, 0);
	deviceContext->OMSetBlendState(nullptr, nullptr, 0xFFFFFFFF);
}

void NormalEffect::Apply(ID3D11DeviceContext* deviceContext)
{
	auto& pCBuffers = pImpl->cBufferPtrs;
	// 将缓冲区绑定到渲染管线上
	pCBuffers[0]->BindVS(deviceContext);
	pCBuffers[1]->BindVS(deviceContext);
	pCBuffers[2]->BindVS(deviceContext);

	pCBuffers[0]->BindGS(deviceContext);
	pCBuffers[1]->BindGS(deviceContext);
	pCBuffers[2]->BindGS(deviceContext);

	if (pImpl->isDirty)
	{
		pImpl->isDirty = false;
		for (auto& pCBuffer : pCBuffers)
		{
			pCBuffer->UpdateBuffer(deviceContext);
		}
	}
}

void XM_CALLCONV NormalEffect::SetWorldMatrix(DirectX::FXMMATRIX W)
{
	auto& cBuffer = pImpl->cbDrawing;
	cBuffer.data.world = XMMatrixTranspose(W);
	cBuffer.data.worldInvTranspose = XMMatrixInverse(nullptr, W);
	pImpl->isDirty = cBuffer.isDirty = true;
}

void XM_CALLCONV NormalEffect::SetViewMatrix(FXMMATRIX V)
{
	auto& cBuffer = pImpl->cbFrame;
	cBuffer.data.view = XMMatrixTranspose(V);
	pImpl->isDirty = cBuffer.isDirty = true;
}

void XM_CALLCONV NormalEffect::SetProjMatrix(FXMMATRIX P)
{
	auto& cBuffer = pImpl->cbOnResize;
	cBuffer.data.proj = XMMatrixTranspose(P);
	pImpl->isDirty = cBuffer.isDirty = true;
}