#include "core/RenderSky.h"
#include "tools/Geometry.h"

namespace Potato
{
	RenderSky::RenderSky(ID3D11Device* device, ID3D11DeviceContext* deviceContext, const std::wstring& cubemapFile, float skySphereRadius, bool generateMips /*= false*/)
		: mtype(VertexType_Pos)
	{
		if (cubemapFile.substr(cubemapFile.size() - 3) == L"dds")
		{
			HR(CreateDDSTextureFromFile(device, generateMips ? deviceContext : nullptr, cubemapFile.c_str(), nullptr, &mTextureCube));
		} 
		else
		{
			HR(CreateWICTexture2DCubeFromFile(device, deviceContext, cubemapFile, nullptr, &mTextureCube, generateMips));
		}
	
		mesh = new Mesh(device, Geometry::CreateSphere(skySphereRadius), mtype);
	}
	
	RenderSky::RenderSky(ID3D11Device* device, ID3D11DeviceContext* deviceContext, const std::vector<std::wstring>& cubemapFile, float skySphereRadius, bool generateMips /*= false*/)
		:mtype(VertexType_Pos)
	{
		HR(CreateWICTexture2DCubeFromFile(device, deviceContext, cubemapFile, nullptr, &mTextureCube, generateMips));
	
		mesh = new Mesh(device, Geometry::CreateSphere(skySphereRadius), mtype);
	}

	RenderSky::~RenderSky()
	{
		DeleteObject(mesh);
		ReleaseObject(mTextureCube);
	}

	ID3D11ShaderResourceView* RenderSky::GetCubeTexture()
	{
		return mTextureCube;
	}

	void RenderSky::DrawSky(ID3D11DeviceContext* deviceContext, EffectBase* effect, GameCamera* camera)
	{
		deviceContext->IASetVertexBuffers(0, VertexBufferCount[mtype], mesh->GetVertexBuffer(), VertexStrides[mtype], VertexOffsets[mtype]);
		deviceContext->IASetIndexBuffer(mesh->GetIndicesBuffer(), DXGI_FORMAT_R32_UINT, 0);
	
		effect->SetWorldMatrix(camera->ViewPerspectiveProjectionMatrix());
		effect->Apply(deviceContext);

		effect->SetTexture(deviceContext, 0, 1, mTextureCube);

		deviceContext->DrawIndexed(mesh->GetIndexCount(), 0, 0);
	}
}


