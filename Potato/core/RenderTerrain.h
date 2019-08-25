#ifndef RenderTerrain_H
#define RenderTerrain_H

#pragma once
#include "tools/Util.h"
#include "tools/EffectBase.h"
#include "core/GameCamera.h"
#include "tools/Mesh.h"

namespace Potato
{
	class RenderTerrain
	{
	public:
		RenderTerrain(ID3D11Device* device, UINT  levels = 10, std::string highmap = "");
		~RenderTerrain();

		void DrawTerrain(ID3D11DeviceContext* deviceContext, EffectBase* effect);
	private:
		VertexType mtype;

		Mesh* mesh;
	};
}
#endif // RenderTerrain_H
