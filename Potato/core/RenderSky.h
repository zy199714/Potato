#ifndef READERSKY_H
#define READERSKY_H

#pragma once
#include "tools/Util.h"
#include "tools/EffectBase.h"
#include "core/GameCamera.h"
#include "tools/Mesh.h"

namespace Potato
{
	class RenderSky
	{
	public:
		RenderSky(ID3D11Device* device, ID3D11DeviceContext* deviceContext, const std::wstring& cubemapFile, float skySphereRadius, bool generateMips = false);
		RenderSky(ID3D11Device* device, ID3D11DeviceContext* deviceContext, const std::vector<std::wstring>& cubemapFile, float skySphereRadius, bool generateMips = false);

		~RenderSky();

		ID3D11ShaderResourceView* GetCubeTexture();

		void DrawSky(ID3D11DeviceContext* deviceContext, EffectBase* effect, GameCamera* camera);
	private:
		VertexType mtype;
		Mesh* mesh;

		ID3D11ShaderResourceView* mTextureCube;
	};
}

#endif // READERSKY_H