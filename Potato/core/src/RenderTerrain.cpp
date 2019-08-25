#include "core/RenderTerrain.h"
#include "tools/Geometry.h"

namespace Potato
{
	RenderTerrain::RenderTerrain(ID3D11Device* device, UINT levels /*= 10*/, std::string highmap /*= ""*/)
		: mtype(VertexType_PosNormalTex)
	{
		BitMap* bitMap = new BitMap;
		LoadBitMap("Model/" + highmap, bitMap);

		mesh = new Mesh(device, Geometry::CreateTerrain(levels, bitMap) , mtype);

		DeleteObject(bitMap);
	}

	RenderTerrain::~RenderTerrain()
	{
		DeleteObject(mesh);
	}

	void RenderTerrain::DrawTerrain(ID3D11DeviceContext* deviceContext, EffectBase* effect)
	{
		deviceContext->IASetVertexBuffers(0, VertexBufferCount[mtype], mesh->GetVertexBuffer(), VertexStrides[mtype], VertexOffsets[mtype]);
		deviceContext->IASetIndexBuffer(mesh->GetIndicesBuffer(), DXGI_FORMAT_R32_UINT, 0);
	
		effect->Apply(deviceContext);

		deviceContext->DrawIndexed(mesh->GetIndexCount(), 0, 0);
	}
}


