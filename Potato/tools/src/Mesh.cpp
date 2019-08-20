#include "tools/Mesh.h"
#include "assimp/scene.h"

using namespace DirectX;

namespace Potato
{
	Mesh::Mesh(ID3D11Device* device, const aiMesh* mesh, VertexType type)
		: mType(type)
	{
		// vertices;
		mPositions.reserve(mesh->mNumVertices);
		for (UINT i = 0; i < mesh->mNumVertices; i++)
		{
			aiVector3D vertex = mesh->mVertices[i];
			mPositions.push_back(XMFLOAT3(vertex.x, vertex.y, vertex.z));
		}

		// Normals
		if (mesh->HasNormals())
		{
			mNormals.reserve(mesh->mNumVertices);
			for (UINT i = 0; i < mesh->mNumVertices; i++)
			{
				aiVector3D normal = mesh->mNormals[i];
				mNormals.push_back(XMFLOAT3(normal.x, normal.y, normal.z));
			}
		}

		// Tangents and Bi tangents
		if (mesh->HasTangentsAndBitangents())
		{
			mTangents.reserve(mesh->mNumVertices);
			mBitangents.reserve(mesh->mNumVertices);
			for (UINT i = 0; i < mesh->mNumVertices; i++)
			{
				aiVector3D tangents = mesh->mTangents[i];
				aiVector3D bitangents = mesh->mBitangents[i];
				mTangents.push_back(XMFLOAT3(tangents.x, tangents.y, tangents.z));
				mBitangents.push_back(XMFLOAT3(bitangents.x, bitangents.y, bitangents.z));
			}
		}

		// Texture Coordinates
		UINT uvChannelCount = mesh->GetNumUVChannels();
		mTextureCoordinates.reserve(uvChannelCount);
		for (UINT i = 0; i < uvChannelCount; i++)
		{
			std::vector<XMFLOAT2>* textureCoordinates = new std::vector<XMFLOAT2>();
			textureCoordinates->reserve(mesh->mNumVertices);
			mTextureCoordinates.push_back(textureCoordinates);

			aiVector3D* aiTextureCoordinates = mesh->mTextureCoords[i];
			for (UINT j = 0; j < mesh->mNumVertices; j++)
			{
				aiVector3D texturecoord = aiTextureCoordinates[j];
				textureCoordinates->push_back(XMFLOAT2(texturecoord.x, texturecoord.y));
			}
		}

		// Vertex Colors
		UINT colorChannelCount = mesh->GetNumColorChannels();
		mVertexColors.reserve(colorChannelCount);
		for (UINT i = 0; i < colorChannelCount; i++)
		{
			std::vector<XMFLOAT4>* vertexColors = new std::vector<XMFLOAT4>();
			vertexColors->reserve(mesh->mNumVertices);
			mVertexColors.push_back(vertexColors);

			aiColor4D* aiVertexColors = mesh->mColors[i];
			for (UINT j = 0; j < mesh->mNumVertices; j++)
			{
				aiColor4D vertexcolor = aiVertexColors[j];
				vertexColors->push_back(XMFLOAT4(vertexcolor.r, vertexcolor.g, vertexcolor.b, vertexcolor.a));
			}
		}

		// Faces
		if (mesh->HasFaces())
		{
			mFaceCount = mesh->mNumFaces;
			for (UINT i = 0; i < mFaceCount; i++)
			{
				aiFace* face = &mesh->mFaces[i];

				for (UINT j = 0; j < face->mNumIndices; j++)
				{
					mIndices.push_back(face->mIndices[j]);
				}
			}
		}

		// ²ÄÖÊË÷Òý
		mModelMaterialIndex = mesh->mMaterialIndex;

		CreateBuffer(device);
		mIndicesBuffer = Buffer(device, mIndices.data(), mIndices.size() * sizeof(UINT), BT_Indices);
	}

	Mesh::Mesh(ID3D11Device* device, const Geometry::MeshData& meshdata, VertexType type)
		: mType(type)
	{
		mPositions = meshdata.position;
		mNormals = meshdata.normal;
		mTangents = meshdata.tangent;
		mVertexColors.push_back(new std::vector<XMFLOAT4>(meshdata.color));
		mIndices = meshdata.indexVec;
		mTextureCoordinates.push_back(new std::vector<XMFLOAT2>(meshdata.texturecood));

		mModelMaterialIndex = 0;
		CreateBuffer(device);
		mIndicesBuffer = Buffer(device, mIndices.data(), mIndices.size() * sizeof(UINT), BT_Indices);
	}

	Mesh::~Mesh()
	{
		DeleteObjectsVector(mTextureCoordinates);
		DeleteObjectsVector(mVertexColors);

		for (auto buf : mVertexBuffer)
		{
			ReleaseObject(buf);
		}
		
		ReleaseObject(mIndicesBuffer);
	}

	UINT Mesh::GetMaterialIndex() const
	{
		return mModelMaterialIndex;
	}

	UINT Mesh::GetIndexCount() const
	{
		return mIndices.size();
	}

	UINT Mesh::GetVertexCount() const
	{
		return mPositions.size();
	}

	std::vector<DirectX::XMFLOAT3>* Mesh::GetVertex()
	{
		return &mPositions;
	}

	std::vector<DirectX::XMFLOAT3>* Mesh::GetNormal()
	{
		return &mNormals;
	}

	std::vector<DirectX::XMFLOAT3>* Mesh::GetTangent()
	{
		return &mTangents;
	}

	std::vector<DirectX::XMFLOAT3>* Mesh::GetBitangent()
	{
		return &mBitangents;
	}

	std::vector<DirectX::XMFLOAT2>* Mesh::GetTextureCoord(UINT slot)
	{
		if (slot > -1 && slot <= mTextureCoordinates.size())
		{
			return mTextureCoordinates[slot];
		}

		return nullptr;
	}

	std::vector<DirectX::XMFLOAT4>* Mesh::GetVertexColor(UINT slot)
	{
		if (slot > -1 && slot <= mVertexColors.size())
		{	
			return mVertexColors[slot];
		}

		return nullptr;
	}

	ID3D11Buffer* const* Mesh::GetVertexBuffer() const
	{
		return mVertexBuffer.data();
	}

	ID3D11Buffer* Mesh::GetIndicesBuffer() const
	{
		return  mIndicesBuffer;
	}

	void Mesh::CreateBuffer(ID3D11Device* device)
	{
		mVertexBuffer.clear();

		switch (mType)
		{
		case Potato::VertexType_Pos:
			mVertexBuffer.push_back(Buffer(device, mPositions.data(), mPositions.size() * sizeof(XMFLOAT3), BT_Vertex));
			break;
		case Potato::VertexType_PosColor:
			mVertexBuffer.push_back(Buffer(device, mPositions.data(), mPositions.size() * sizeof(XMFLOAT3), BT_Vertex));
			mVertexBuffer.push_back(Buffer(device, mVertexColors[0]->data(), mVertexColors[0]->size() * sizeof(XMFLOAT4), BT_Vertex));
			break;
		case Potato::VertexType_PosTex:
			mVertexBuffer.push_back(Buffer(device, mPositions.data(), mPositions.size() * sizeof(XMFLOAT3), BT_Vertex));
			mVertexBuffer.push_back(Buffer(device, mTextureCoordinates[0]->data(), mTextureCoordinates[0]->size() * sizeof(XMFLOAT2), BT_Vertex));
			break;
		case Potato::VertexType_PosSize:
			mVertexBuffer.push_back(Buffer(device, mPositions.data(), mPositions.size() * sizeof(XMFLOAT3), BT_Vertex));
			break;
		case Potato::VertexType_PosNormalColor:
			mVertexBuffer.push_back(Buffer(device, mPositions.data(), mPositions.size() * sizeof(XMFLOAT3), BT_Vertex));
			mVertexBuffer.push_back(Buffer(device, mNormals.data(), mNormals.size() * sizeof(XMFLOAT3), BT_Vertex));
			mVertexBuffer.push_back(Buffer(device, mVertexColors[0]->data(), mVertexColors[0]->size() * sizeof(XMFLOAT4), BT_Vertex));
			break;
		case Potato::VertexType_PosNormalTex:
			mVertexBuffer.push_back(Buffer(device, mPositions.data(), mPositions.size() * sizeof(XMFLOAT3), BT_Vertex));
			mVertexBuffer.push_back(Buffer(device, mNormals.data(), mNormals.size() * sizeof(XMFLOAT3), BT_Vertex));
			mVertexBuffer.push_back(Buffer(device, mTextureCoordinates[0]->data(), mTextureCoordinates[0]->size() * sizeof(XMFLOAT2), BT_Vertex));
			break;
		case Potato::VertexType_PosNormalTangentTex:
			mVertexBuffer.push_back(Buffer(device, mPositions.data(), mPositions.size() * sizeof(XMFLOAT3), BT_Vertex));
			mVertexBuffer.push_back(Buffer(device, mNormals.data(), mNormals.size() * sizeof(XMFLOAT3), BT_Vertex));
			mVertexBuffer.push_back(Buffer(device, mTangents.data(), mTangents.size() * sizeof(XMFLOAT3), BT_Vertex));
			mVertexBuffer.push_back(Buffer(device, mTextureCoordinates[0]->data(), mTextureCoordinates[0]->size() * sizeof(XMFLOAT2), BT_Vertex));
			break;
		case Potato::VertexType_Instance_PosNormalTex_WorldWorldT:
			mVertexBuffer.push_back(Buffer(device, mPositions.data(), mPositions.size() * sizeof(XMFLOAT3), BT_Vertex));
			mVertexBuffer.push_back(Buffer(device, mNormals.data(), mNormals.size() * sizeof(XMFLOAT3), BT_Vertex));
			mVertexBuffer.push_back(Buffer(device, mTextureCoordinates[0]->data(), mTextureCoordinates[0]->size() * sizeof(XMFLOAT2), BT_Vertex));
			break;
		default:
			break;
		}
	}

	ID3D11Buffer* Mesh::Buffer(ID3D11Device* device, void* lsh, UINT size, BufferType type)
	{
		ID3D11Buffer* buffer = nullptr;

		D3D11_BUFFER_DESC bufferDesc;
		ZeroMemory(&bufferDesc, sizeof(D3D11_BUFFER_DESC));
		if (type == BT_Vertex)
		{
			bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		}
		else
		{
			bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;;
		}
		bufferDesc.ByteWidth = size;
		bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;

		D3D11_SUBRESOURCE_DATA subresource;
		ZeroMemory(&subresource, sizeof(D3D11_SUBRESOURCE_DATA));
		subresource.pSysMem = lsh;

		HR(device->CreateBuffer(&bufferDesc, &subresource, &buffer));

		return buffer;
	}
}
