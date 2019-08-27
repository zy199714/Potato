#include "tools/Model.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace Potato
{
	Model::Model(ID3D11Device* device, VertexType type, std::string filename, UINT flags /*= -1*/)
		: mType(type)
	{
		Assimp::Importer importer;
	
		if (flags == -1)
		{
			flags = aiProcess_FlipWindingOrder | aiProcess_MakeLeftHanded | aiProcess_ConvertToLeftHanded;
		}
		else
		{
			flags |= aiProcess_FlipWindingOrder | aiProcess_MakeLeftHanded | aiProcess_ConvertToLeftHanded;
		}
	
		const aiScene* scene = importer.ReadFile(filename, flags);
	
		if (scene == nullptr)
		{
			throw importer.GetErrorString();
		}
	
		if (scene->HasMeshes())
		{
			for (UINT i = 0; i < scene->mNumMeshes; ++i)
			{
				Mesh* mesh = new Mesh(device, scene->mMeshes[i], type);
				if (mesh != nullptr)
					mMeshs.push_back(mesh);
			}
		}
	
		if (scene->HasMaterials())
		{
			for (UINT i = 0; i < scene->mNumMaterials; ++i)
			{
				ModelMaterial* material = new ModelMaterial(device, scene->mMaterials[i]);
				if (material != nullptr)
					mModelMaterials.push_back(material);
			}
		}
	}
	
	Model::Model(ID3D11Device* device, VertexType type, const Geometry::MeshData& meshdata)
		: mType(type)
	{
		mMeshs.push_back(new Mesh(device, meshdata, type));
		mModelMaterials.push_back(new ModelMaterial());
	}
	
	Model::~Model()
	{
		DeleteObjectsVector(mMeshs);
		DeleteObjectsVector(mModelMaterials);
	}
	
	VertexType Model::GetVertexType()
	{
		return mType;
	}
	
	std::vector<Mesh*>& Model::GetMeshs()
	{
		return mMeshs;
	}
	
	Potato::ModelMaterial* Model::GetModelMaterial(UINT slot)
	{
		return mModelMaterials[slot];
	}

	UINT Model::GetMeshCount()
	{
		return mMeshs.size();
	}
}

