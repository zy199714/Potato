#include <assimp/scene.h>
#include "tools/ModelMaterial.h"

namespace Potato
{
	ModelMaterial::ModelMaterial()
	{
		//InitializeTextureTypeMappings();

	}

	ModelMaterial::ModelMaterial(ID3D11Device* device, aiMaterial* material)
	{
		InitializeTextureTypeMappings();

		for (TextureType textureType = (TextureType)0; textureType < TextureType_END; textureType = (TextureType)(textureType + 1))
		{
			aiTextureType mappedTextureType = (aiTextureType)mTextureTypeMappings[textureType];

			UINT textureCount = material->GetTextureCount(mappedTextureType);
			if (textureCount > 0)
			{
				std::vector<std::wstring>* textures = new std::vector<std::wstring>();
				mTextures.insert(std::pair<TextureType, std::vector<std::wstring>*>(textureType, textures));

				textures->reserve(textureCount);
				for (UINT textureIndex = 0; textureIndex < textureCount; textureIndex++)
				{
					aiString path;
					if (material->GetTexture(mappedTextureType, textureIndex, &path) == AI_SUCCESS)
					{
						std::string w(path.C_Str());
						std::wstring wPrth;
						wPrth.assign(w.begin(), w.end());
						textures->push_back(wPrth);
					}
				}
			}
		}

		InitBuffer(device);
	}

	ModelMaterial::~ModelMaterial()
	{
		for (auto& textures : mTextures)
		{
			DeleteObject(textures.second);
		}

		for (auto textures : mTexturesBuffer)
		{
			auto buffers = textures.second;
			if (buffers != nullptr)
			{
				for (auto buf : *buffers)
				{
					ReleaseObject(buf);
				}
			}
			DeleteObject(buffers);
		}
	}

	void ModelMaterial::AddTexture(TextureType type, std::vector<std::wstring>* texturesfile)
	{
		mTextures.insert(std::pair<TextureType, std::vector<std::wstring>*>(type, texturesfile));
	}

	void ModelMaterial::AddTexture(TextureType type, std::wstring texturefile)
	{
		auto textures = mTextures[type];
		textures->push_back(texturefile);
	}

	void ModelMaterial::InitBuffer(ID3D11Device* device)
	{
		for (auto& textures : mTextures)
		{
			std::vector<ID3D11ShaderResourceView*>* SRV = new std::vector<ID3D11ShaderResourceView *>();
			auto texturefile = textures.second;

			for (auto file : *texturefile)
			{
				ID3D11ShaderResourceView* srv = nullptr;
				std::wstring path = L"Model/" + file;
					
				if (file.substr(file.size() - 3) == L"dds")
				{
					HR(DirectX::CreateDDSTextureFromFile(device, path.c_str(), nullptr, &srv));
				}
				else
				{
					ID3D11DeviceContext* deviceContext;
					device->QueryInterface<ID3D11DeviceContext>(&deviceContext);
					HR(DirectX::CreateWICTextureFromFile(device, deviceContext, path.c_str(), nullptr, &srv, false));
				}

				SRV->push_back(srv);
			}
			mTexturesBuffer.insert(std::pair<TextureType, std::vector<ID3D11ShaderResourceView*>*>(textures.first, SRV));
		}
	}

	std::vector<ID3D11ShaderResourceView*>* ModelMaterial::GetSRVBuffer(TextureType type)
	{
		return mTexturesBuffer[type];
	}

	void ModelMaterial::InitializeTextureTypeMappings()
	{
		if (mTextureTypeMappings.size() != TextureType_END)
		{
			mTextureTypeMappings[TextureType_NONE] = aiTextureType_NONE;
			mTextureTypeMappings[TextureType_DIFFUSE] = aiTextureType_DIFFUSE;
			mTextureTypeMappings[TextureType_SPECULAR] = aiTextureType_SPECULAR;
			mTextureTypeMappings[TextureType_AMBINENT] = aiTextureType_AMBIENT;
			mTextureTypeMappings[TextureType_EMISSIVE] = aiTextureType_EMISSIVE;
			mTextureTypeMappings[TextureType_HEIGHT] = aiTextureType_HEIGHT;
			mTextureTypeMappings[TextureType_NORMALS] = aiTextureType_NORMALS;
			mTextureTypeMappings[TextureType_SHININESS] = aiTextureType_SHININESS;
			mTextureTypeMappings[TextureType_OPACTIY] = aiTextureType_OPACITY;
			mTextureTypeMappings[TextureType_DISPLACEMENT] = aiTextureType_DISPLACEMENT;
			mTextureTypeMappings[TextureType_LIGHTMAP] = aiTextureType_LIGHTMAP;
			mTextureTypeMappings[TextureType_REFLECTION] = aiTextureType_REFLECTION;
			mTextureTypeMappings[TextureType_END] = aiTextureType_UNKNOWN;
		}
	}

}