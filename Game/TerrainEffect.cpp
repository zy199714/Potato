#include "Effects.h"

using namespace Potato;

class TerrainEffect::Impl : public AligendType<TerrainEffect::Impl>
{
public:
	struct CBChangesEveryFrame
	{
		DirectX::XMMATRIX viewproj;
		DirectX::XMVECTOR eyePos;
	};
	struct CBChangesRarely
	{
		directionlight dirLight[EffectBase::maxLights];
		pointlight pointLight[EffectBase::maxLights];
		spotlight spotLight[EffectBase::maxLights];
		int numDirLight;
		int numPointLight;
		int numSpotLight;
		int pad2;
	};

public:
	// 必须显式指定
	Impl() {};
	~Impl()
	{
		ReleaseObject(TerrainVS);
		ReleaseObject(TerrainPS);
		ReleaseObject(vertexPosLayout);
	};

public:
	ContentBufferObject<0, CBChangesEveryFrame>	cbFrame;	// 每帧绘制的常量缓冲区
	ContentBufferObject<1, CBChangesRarely>				cbRarely;			// 几乎不会变更的常量缓冲区
	BOOL isDirty;										// 是否有值变更
	std::vector<ContentBufferBase*> cBufferPtrs;				// 统一管理上面所有的常量缓冲区

	ID3D11VertexShader* TerrainVS;
	ID3D11PixelShader* TerrainPS;

	ID3D11InputLayout* vertexPosLayout;
};

namespace
{
	// SkyEffect单例
	static TerrainEffect * pInstance = nullptr;
}

TerrainEffect::TerrainEffect()
{
	if (pInstance)
		throw std::exception("TerrainEffect is a singleton!");
	pInstance = this;
	pImpl = std::make_unique<TerrainEffect::Impl>();
}

TerrainEffect::~TerrainEffect()
{

}

bool TerrainEffect::InitAll(ID3D11Device* device)
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

	HR(CreateShaderFromFile(L"HLSL\\Terrain_VS.cso", L"HLSL\\Terrain_VS.hlsl", "VS", "vs_5_0", &blob));
	HR(device->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &pImpl->TerrainVS));
	// 创建顶点布局
	HR(device->CreateInputLayout(InputLayout[VertexType_PosNormalTex], InputLayoutCount[VertexType_PosNormalTex],
		blob->GetBufferPointer(), blob->GetBufferSize(), &pImpl->vertexPosLayout));

	// ******************
	// 创建像素着色器
	//
	ReleaseObject(blob);
	HR(CreateShaderFromFile(L"HLSL\\Terrain_PS.cso", L"HLSL\\Terrain_PS.hlsl", "PS", "ps_5_0", &blob));
	HR(device->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &pImpl->TerrainPS));

	pImpl->cBufferPtrs.assign({
		&pImpl->cbFrame,
		&pImpl->cbRarely
	});

	// 创建常量缓冲区
	for (auto& pBuffer : pImpl->cBufferPtrs)
	{
		HR(pBuffer->CreateBuffer(device));
	}

	return true;
}

void TerrainEffect::SetRenderDefault(ID3D11DeviceContext* deviceContext)
{
	deviceContext->IASetInputLayout(pImpl->vertexPosLayout);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	deviceContext->VSSetShader(pImpl->TerrainVS, nullptr, 0);
	deviceContext->GSSetShader(nullptr, nullptr, 0);
	//deviceContext->RSSetState(RenderStates::RSNoCull);
	deviceContext->RSSetState(nullptr);
	deviceContext->PSSetShader(pImpl->TerrainPS, nullptr, 0);
	deviceContext->OMSetDepthStencilState(nullptr, 0);
	deviceContext->OMSetBlendState(nullptr, nullptr, 0xFFFFFFFF);
}

void XM_CALLCONV TerrainEffect::SetViewMatrix(DirectX::FXMMATRIX VP)
{
	auto& cBuffer = pImpl->cbFrame;
	cBuffer.data.viewproj = XMMatrixTranspose(VP);
	pImpl->isDirty = cBuffer.isDirty = true;
}

void TerrainEffect::SetTexture(ID3D11DeviceContext* devicecontext, UINT startslot, UINT numview, ID3D11ShaderResourceView* texture)
{
	devicecontext->VSSetShaderResources(startslot, numview, &texture);
}

// void TerrainEffect::SetTextureUsed(bool isUsed)
// {
// 	auto& cBuffer = pImpl->cbStates;
// 	cBuffer.data.textureUsed = isUsed;
// 	pImpl->isDirty = cBuffer.isDirty = true;
// }
// 
// void TerrainEffect::SetOffest(float offest)
// {
// 	auto& cBuffer = pImpl->cbStates;
// 	cBuffer.data.offest = offest;
// 	pImpl->isDirty = cBuffer.isDirty = true;
// }

void TerrainEffect::SetDirLight(size_t pos, const directionlight & dirLight)
{
	auto& cBuffer = pImpl->cbRarely;
	cBuffer.data.dirLight[pos] = dirLight;
	pImpl->isDirty = cBuffer.isDirty = true;
}

void TerrainEffect::SetPointLight(size_t pos, const pointlight & pointLight)
{
	auto& cBuffer = pImpl->cbRarely;
	cBuffer.data.pointLight[pos] = pointLight;
	pImpl->isDirty = cBuffer.isDirty = true;
}

void TerrainEffect::SetSpotLight(size_t pos, const spotlight & spotLight)
{
	auto& cBuffer = pImpl->cbRarely;
	cBuffer.data.spotLight[pos] = spotLight;
	pImpl->isDirty = cBuffer.isDirty = true;
}

void TerrainEffect::SetNumLight(int dir, int point, int spot)
{
	auto& cBuffer = pImpl->cbRarely;
	cBuffer.data.numDirLight = dir;
	cBuffer.data.numPointLight = point;
	cBuffer.data.numSpotLight = spot;
	pImpl->isDirty = cBuffer.isDirty = true;
}

void XM_CALLCONV TerrainEffect::SetEyePos(FXMVECTOR eyePos)
{
	auto& cBuffer = pImpl->cbFrame;
	cBuffer.data.eyePos = eyePos;
	pImpl->isDirty = cBuffer.isDirty = true;
}

void TerrainEffect::Apply(ID3D11DeviceContext* deviceContext)
{
	auto& pCBuffers = pImpl->cBufferPtrs;
	// 将缓冲区绑定到渲染管线上
	pCBuffers[0]->BindVS(deviceContext);
	pCBuffers[1]->BindVS(deviceContext);

	pCBuffers[0]->BindPS(deviceContext);
	pCBuffers[1]->BindPS(deviceContext);

	if (pImpl->isDirty)
	{
		pImpl->isDirty = false;
		for (auto& pCBuffer : pCBuffers)
		{
			pCBuffer->UpdateBuffer(deviceContext);
		}
	}
}

