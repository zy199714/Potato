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

		/* 获得顶点集合 */
		std::vector<DirectX::XMFLOAT3>* GetVertex();
		/* 获得法线集合 */
		std::vector<DirectX::XMFLOAT3>* GetNormal();
		/* 获得切线集合 */
		std::vector<DirectX::XMFLOAT3>* GetTangent();
		/* 获得副法线集合 */
		std::vector<DirectX::XMFLOAT3>* GetBitangent();
		/* 获得一组纹理坐标 */
		std::vector<DirectX::XMFLOAT2>* GetTextureCoord(UINT  slot);
		/* 获得一组顶点颜色 */
		std::vector<DirectX::XMFLOAT4>* GetVertexColor(UINT slot);

		/* 获得 VertexBuffer */
		ID3D11Buffer* const * GetVertexBuffer() const;
		/* 获得 IndicesBuffer */
		ID3D11Buffer* GetIndicesBuffer() const;

	private:
		/* 返回 buffer 数组*/
		void CreateBuffer(ID3D11Device* device);

		ID3D11Buffer* Buffer(ID3D11Device* device, void* lsh, UINT size, BufferType type);

	private:
		std::vector<DirectX::XMFLOAT3> mPositions{};		// 顶点
		std::vector<DirectX::XMFLOAT3> mNormals{};		// 法线
		std::vector<DirectX::XMFLOAT3> mTangents{};	// 切线
		std::vector<DirectX::XMFLOAT3> mBitangents{};	// 副法线
		std::vector<std::vector<DirectX::XMFLOAT2>*> mTextureCoordinates{};	// 纹理坐标
		std::vector<std::vector<DirectX::XMFLOAT4>*> mVertexColors{};		// 顶点颜色
		UINT mFaceCount;		// 面数
		std::vector<UINT> mIndices{};  // 索引

		UINT mModelMaterialIndex;	// 材质索引
		VertexType mType;

		std::vector<ID3D11Buffer*> mVertexBuffer{};
		ID3D11Buffer* mIndicesBuffer{};
	};
}

#endif // !MESH_H


