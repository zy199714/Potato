/**
 * Í¨¹ý assimp ¼ÓÔØ mesh
 *
 */

#ifndef Model_H
#define Model_H

#pragma once
#include "tools/Mesh.h"
#include "tools/ModelMaterial.h"

namespace Potato
{
	class Model : public nocopy
	{
	public:
		Model(ID3D11Device* device, VertexType type, std::string filename, UINT flags = -1);
		Model(ID3D11Device* device, VertexType type, const Geometry::MeshData& meshdata);
		~Model();
	
		VertexType GetVertexType();
		
		std::vector<Mesh*>& GetMeshs();
		ModelMaterial* GetModelMaterial(UINT slot);

		UINT GetMeshCount();
	private:

	private: 
		VertexType mType;

		std::vector<Mesh*> mMeshs{};
		std::vector<ModelMaterial*> mModelMaterials{};
	};
}

#endif // Model_H
