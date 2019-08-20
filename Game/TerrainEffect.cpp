#include "Effects.h"

using namespace Potato;

class TerrainEffect::Impl : public AligendType<TerrainEffect::Impl>
{
public:
	struct CBChangesEveryFrame
	{
		DirectX::XMMATRIX World;
		DirectX::XMMATRIX ViewProj;
	};
public:
	// ������ʽָ��
	Impl() {};
	~Impl()
	{
		ReleaseObject(TerrainVS);
		ReleaseObject(TerrainPS);
		ReleaseObject(vertexPosLayout);
	};

public:
	ContentBufferObject<0, CBChangesEveryFrame>	cbFrame;	// ÿ֡���Ƶĳ���������
	BOOL isDirty;										// �Ƿ���ֵ���
	std::vector<ContentBufferBase*> cBufferPtrs;				// ͳһ�����������еĳ���������

	ID3D11VertexShader* TerrainVS;
	ID3D11PixelShader* TerrainPS;

	ID3D11InputLayout* vertexPosLayout;
};

namespace
{
	// SkyEffect����
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
	// ����������ɫ��

	HR(CreateShaderFromFile(L"HLSL\\Terrain_VS.cso", L"HLSL\\Terrain_VS.hlsl", "VS", "vs_5_0", &blob));
	HR(device->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &pImpl->TerrainVS));
	// �������㲼��
	HR(device->CreateInputLayout(InputLayout[VertexType_Pos], InputLayoutCount[VertexType_Pos],
		blob->GetBufferPointer(), blob->GetBufferSize(), &pImpl->vertexPosLayout));

	// ******************
	// ����������ɫ��
	//
	ReleaseObject(blob);
	HR(CreateShaderFromFile(L"HLSL\\Terrain_PS.cso", L"HLSL\\Terrain_PS.hlsl", "PS", "ps_5_0", &blob));
	HR(device->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &pImpl->TerrainPS));

	pImpl->cBufferPtrs.assign({
		&pImpl->cbFrame,
	});

	// ��������������
	for (auto& pBuffer : pImpl->cBufferPtrs)
	{
		HR(pBuffer->CreateBuffer(device));
	}

	return true;
}

void TerrainEffect::SetRenderDefault(ID3D11DeviceContext* deviceContext)
{
	deviceContext->IASetInputLayout(pImpl->vertexPosLayout);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	deviceContext->VSSetShader(pImpl->TerrainVS, nullptr, 0);
	deviceContext->GSSetShader(nullptr, nullptr, 0);
	deviceContext->RSSetState(RenderStates::RSNoCull);
	deviceContext->PSSetShader(pImpl->TerrainPS, nullptr, 0);
	deviceContext->PSSetSamplers(0, 1, &RenderStates::SSLinearWrap);
	deviceContext->OMSetDepthStencilState(RenderStates::DSSLessEqual, 0);
	deviceContext->OMSetBlendState(nullptr, nullptr, 0xFFFFFFFF);
}

void XM_CALLCONV TerrainEffect::SetWorldMatrix(DirectX::FXMMATRIX W)
{
	auto& cBuffer = pImpl->cbFrame;
	cBuffer.data.World = XMMatrixTranspose(W);
	pImpl->isDirty = cBuffer.isDirty = true;
}

void XM_CALLCONV TerrainEffect::SetViewMatrix(DirectX::FXMMATRIX PV)
{
	auto& cBuffer = pImpl->cbFrame;
	cBuffer.data.ViewProj = XMMatrixTranspose(PV);
	pImpl->isDirty = cBuffer.isDirty = true;
}


void TerrainEffect::Apply(ID3D11DeviceContext* deviceContext)
{
	auto& pCBuffers = pImpl->cBufferPtrs;
	// ���������󶨵���Ⱦ������
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

