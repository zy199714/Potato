#ifndef MESH_H
#define MESH_H

#pragma once
#include "tools/Util.h"
#include "tools/Vertex.h"
#include "tools/Geometry.h"

struct aiMesh;

namespace Potato
{
	enum BufferType
	{
		BT_Vertex = 0x0,
		BT_Indices
	};

	class Mesh : public nocopy
	{
	public:
		Mesh(ID3D11Device* device, const aiMesh* mesh, VertexType type);
		Mesh(ID3D11Device* device, const Geometry::MeshData& meshdata, VertexType type);

		~Mesh();

		UINT GetMaterialIndex() const;
		UINT GetIndexCount() const;
		UINT GetVertexCount() const;

		/* ��ö��㼯�� */
		std::vector<DirectX::XMFLOAT3>* GetVertex();
		/* ��÷��߼��� */
		std::vector<DirectX::XMFLOAT3>* GetNormal();
		/* ������߼��� */
		std::vector<DirectX::XMFLOAT3>* GetTangent();
		/* ��ø����߼��� */
		std::vector<DirectX::XMFLOAT3>* GetBitangent();
		/* ���һ���������� */
		std::vector<DirectX::XMFLOAT2>* GetTextureCoord(UINT  slot);
		/* ���һ�鶥����ɫ */
		std::vector<DirectX::XMFLOAT4>* GetVertexColor(UINT slot);

		/* ��� VertexBuffer */
		ID3D11Buffer* const * GetVertexBuffer() const;
		/* ��� IndicesBuffer */
		ID3D11Buffer* GetIndicesBuffer() const;

	private:
		/* ���� buffer ����*/
		void CreateBuffer(ID3D11Device* device);

		ID3D11Buffer* Buffer(ID3D11Device* device, void* lsh, UINT size, BufferType type);

	private:
		std::vector<DirectX::XMFLOAT3> mPositions{};		// ����
		std::vector<DirectX::XMFLOAT3> mNormals{};		// ����
		std::vector<DirectX::XMFLOAT3> mTangents{};	// ����
		std::vector<DirectX::XMFLOAT3> mBitangents{};	// ������
		std::vector<std::vector<DirectX::XMFLOAT2>*> mTextureCoordinates{};	// ��������
		std::vector<std::vector<DirectX::XMFLOAT4>*> mVertexColors{};		// ������ɫ
		UINT mFaceCount;		// ����
		std::vector<UINT> mIndices{};  // ����

		UINT mModelMaterialIndex;	// ��������
		VertexType mType;

		std::vector<ID3D11Buffer*> mVertexBuffer{};
		ID3D11Buffer* mIndicesBuffer{};
	};
}

#endif // !MESH_H


