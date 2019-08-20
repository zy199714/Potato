#include "core/RenderTerrain.h"
#include "tools/Geometry.h"

namespace Potato
{

	RenderTerrain::RenderTerrain(ID3D11Device* device, ID3D11DeviceContext* deviceContext, UINT levels /* =1 */)
		: mtype(VertexType_Pos)
	{
		mesh = new Mesh(device, Geometry::CreateTerrain() , mtype);
	}

	RenderTerrain::~RenderTerrain()
	{
		DeleteObject(mesh);
	}

	void RenderTerrain::DrawTerrain(ID3D11DeviceContext* deviceContext, EffectBase* effect)
	{
		deviceContext->IASetVertexBuffers(0, VertexBufferCount[mtype], mesh->GetVertexBuffer(), VertexStrides[mtype], VertexOffsets[mtype]);
		deviceContext->IASetIndexBuffer(mesh->GetIndicesBuffer(), DXGI_FORMAT_R32_UINT, 0);
	
		effect->SetWorldMatrix(DirectX::XMMATRIX(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f));
		effect->Apply(deviceContext);

		deviceContext->DrawIndexed(mesh->GetIndexCount(), 0, 0);
	}
}


