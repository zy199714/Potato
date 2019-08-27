//***************************************************************************************
// 生成常见的几何体网格模型
// Generate common geometry meshes.
//***************************************************************************************

#ifndef GEOMETRY_H_
#define GEOMETRY_H_

#include <string>
#include <fstream>
#include <iostream>
#include "Vertex.h"
#include "def.h"

namespace Potato
{
	namespace Geometry
	{
		struct VertexData
		{
			DirectX::XMFLOAT3 pos;
			DirectX::XMFLOAT3 normal;
			DirectX::XMFLOAT3 tangent;
			DirectX::XMFLOAT4 color;
			DirectX::XMFLOAT2 tex;
		};
		// 网格数据
		struct MeshData
		{
			std::vector<DirectX::XMFLOAT3> position;
			std::vector<DirectX::XMFLOAT3> normal;
			std::vector<DirectX::XMFLOAT3> tangent;
			std::vector<DirectX::XMFLOAT4> color;
			std::vector<DirectX::XMFLOAT2> texturecood;
			std::vector<UINT> indexVec;	
	
			MeshData(UINT vsize, UINT insize)
			{
				position.reserve(vsize);
				normal.reserve(vsize);
				tangent.reserve(vsize);
				color.reserve(vsize);
				texturecood.reserve(vsize);
				indexVec.reserve(insize);
			};

			void insert(const VertexData& vertexsrc)
			{
				position.push_back(vertexsrc.pos);
				normal.push_back(vertexsrc.normal);
				tangent.push_back(vertexsrc.tangent);
				color.push_back(vertexsrc.color);
				texturecood.push_back(vertexsrc.tex);
			};
		};
	
		// 球体网格数据，levels 水平分段(>=1)，slices垂直分段(>=3)，越大精度越高
		MeshData CreateSphere(float radius = 1.0f, UINT levels = 20, UINT slices = 20, const DirectX::XMFLOAT4& color = { 1.0f, 1.0f, 1.0f, 1.0f });
	
		// 创建立方体网格数据
		MeshData CreateBox(float width = 2.0f, float height = 2.0f, float depth = 2.0f, const DirectX::XMFLOAT4& color = { 1.0f, 1.0f, 1.0f, 1.0f });
	
		// 圆柱体网格数据，levels 水平分段(>=2)，slices垂直分段(>=3)，越大精度越高
		MeshData CreateCylinder(float radius = 1.0f, float height = 2.0f, UINT levels = 2, UINT slices = 16, const DirectX::XMFLOAT4& color = { 1.0f, 1.0f, 1.0f, 1.0f });
	
		// 只有圆柱体侧面的网格数据，levels 水平分段(>=2)，slices垂直分段(>=3)，越大精度越高
		MeshData CreateCylinderNoCap(float radius = 1.0f, float height = 2.0f, UINT levels = 2, UINT slices = 16,
			const DirectX::XMFLOAT4& color = { 1.0f, 1.0f, 1.0f, 1.0f });
	
		// 覆盖NDC屏幕的面
		MeshData Create2DShow( const DirectX::XMFLOAT2& center, const DirectX::XMFLOAT2& scale = { 1.0f, 1.0f },
			const DirectX::XMFLOAT4& color = { 1.0f, 1.0f, 1.0f, 1.0f });
		MeshData Create2DShow(float centerX = 0.0f, float centerY = 0.0f, float scaleX = 1.0f, float scaleY = 1.0f,
			const DirectX::XMFLOAT4& color = { 1.0f, 1.0f, 1.0f, 1.0f });
	
		// 平面
		MeshData CreatePlane(const DirectX::XMFLOAT3& center, const DirectX::XMFLOAT2& planeSize = { 10.0f, 10.0f },
			const DirectX::XMFLOAT2& maxTexCoord = { 1.0f, 1.0f }, const DirectX::XMFLOAT4& color = { 1.0f, 1.0f, 1.0f, 1.0f });
		MeshData CreatePlane(float centerX = 0.0f, float centerY = 0.0f, float centerZ = 0.0f,
			float width = 10.0f, float depth = 10.0f, float texU = 1.0f, float texV = 1.0f,
			const DirectX::XMFLOAT4& color = { 1.0f, 1.0f, 1.0f, 1.0f });
		// 地平面，以原点为中心的方形，levels 分段
		MeshData CreateTerrain(UINT levels = 10, BitMap* heightmap = nullptr);
	}
	
	namespace Geometry
	{

		// 几何体方法的实现

		inline MeshData CreateSphere(float radius, UINT levels, UINT slices, const DirectX::XMFLOAT4 & color)
		{
			using namespace DirectX;

			assert(levels >= 1 && slices >= 3);
	
			UINT vertexCount = 2 + (levels) * (slices + 1);
			// indexCount = 6 * (levels-1) * slices + 3 * 2 * slices;
			UINT indexCount = 6 * levels * slices;

			MeshData meshData(vertexCount, indexCount);
			VertexData vertexData;
	
			XMFLOAT3 pos;
			XMFLOAT3 normal;

			float phi = 0.0f, theta = 0.0f;
			float per_phi = XM_PI / (levels+1);
			float per_theta = XM_2PI / slices;
			float x, y, z;
	
			// 放入顶端点
			vertexData = { XMFLOAT3(0.0f, radius, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT3(1.0f, 0.0f, 0.0f), color, XMFLOAT2(0.0f, 0.0f) };
			meshData.insert(vertexData);
	
			for (UINT i = 1; i <= levels; ++i)
			{
				phi = per_phi * i;
				// 需要slices + 1个顶点是因为 起点和终点需为同一点，但纹理坐标值不一致
				for (UINT j = 0; j <= slices; ++j)
				{
					theta = per_theta * j;
					x = radius * sinf(phi) * cosf(theta);
					y = radius * cosf(phi);
					z = radius * sinf(phi) * sinf(theta);
					// 计算出局部坐标、法向量、Tangent向量和纹理坐标
					pos = XMFLOAT3(x, y, z);
					XMStoreFloat3(&normal, XMVector3Normalize(XMLoadFloat3(&pos)));
					vertexData = { pos, normal, XMFLOAT3(-sinf(theta), 0.0f, cosf(theta)), color, XMFLOAT2(theta / XM_2PI, phi / XM_PI) };

					meshData.insert(vertexData);
				}
			}
	
			// 放入底端点
			vertexData = { XMFLOAT3(0.0f, -radius, 0.0f), XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f), color, XMFLOAT2(0.0f, 1.0f) };
			meshData.insert(vertexData);
	
			for (UINT j = 1; j <= slices; ++j)
			{
				meshData.indexVec.push_back(j);
				meshData.indexVec.push_back(j + 1);
				meshData.indexVec.push_back(0);
			}
	
			for (UINT i = 0; i < levels - 1; ++i)
			{
				for (UINT j = 1; j <= slices; ++j)
				{
					meshData.indexVec.push_back((slices + 1) * i + j + 1);
					meshData.indexVec.push_back((slices + 1) * i + j);
					meshData.indexVec.push_back((slices + 1) * i + (slices + 1) + j + 1);

					meshData.indexVec.push_back((slices + 1) * i + j);
					meshData.indexVec.push_back((slices + 1) * i + (slices + 1) + j);
					meshData.indexVec.push_back((slices + 1) * i + (slices + 1) + j + 1);
				}
			}
	
			UINT offset = (levels - 1) *  (slices + 1);
			for (UINT j = 1; j <= slices; ++j)
			{
				meshData.indexVec.push_back(offset + j + 1);
				meshData.indexVec.push_back(offset + j);
				meshData.indexVec.push_back(vertexCount - 1);
			}
	
			return meshData;
		}
	
		
		inline MeshData CreateBox(float width, float height, float depth, const DirectX::XMFLOAT4 & color)
		{
			using namespace DirectX;
	
			MeshData meshData(24, 24);
	
			VertexData vertexDataArr[24];
			float w2 = width / 2, h2 = height / 2, d2 = depth / 2;
	
			// 右面(+X 面)
			vertexDataArr[0].pos = XMFLOAT3(w2, -h2, -d2);
			vertexDataArr[1].pos = XMFLOAT3(w2, h2, -d2);
			vertexDataArr[2].pos = XMFLOAT3(w2, h2, d2);
			vertexDataArr[3].pos = XMFLOAT3(w2, -h2, d2);
			// 左面(-X 面)
			vertexDataArr[4].pos = XMFLOAT3(-w2, -h2, d2);
			vertexDataArr[5].pos = XMFLOAT3(-w2, h2, d2);
			vertexDataArr[6].pos = XMFLOAT3(-w2, h2, -d2);
			vertexDataArr[7].pos = XMFLOAT3(-w2, -h2, -d2);
			// 顶面(+Y 面)
			vertexDataArr[8].pos = XMFLOAT3(-w2, h2, -d2);
			vertexDataArr[9].pos = XMFLOAT3(-w2, h2, d2);
			vertexDataArr[10].pos = XMFLOAT3(w2, h2, d2);
			vertexDataArr[11].pos = XMFLOAT3(w2, h2, -d2);
			// 底面(-Y 面)
			vertexDataArr[12].pos = XMFLOAT3(w2, -h2, -d2);
			vertexDataArr[13].pos = XMFLOAT3(w2, -h2, d2);
			vertexDataArr[14].pos = XMFLOAT3(-w2, -h2, d2);
			vertexDataArr[15].pos = XMFLOAT3(-w2, -h2, -d2);
			// 背面(+Z 面)
			vertexDataArr[16].pos = XMFLOAT3(w2, -h2, d2);
			vertexDataArr[17].pos = XMFLOAT3(w2, h2, d2);
			vertexDataArr[18].pos = XMFLOAT3(-w2, h2, d2);
			vertexDataArr[19].pos = XMFLOAT3(-w2, -h2, d2);
			// 正面(-Z 面)
			vertexDataArr[20].pos = XMFLOAT3(-w2, -h2, -d2);
			vertexDataArr[21].pos = XMFLOAT3(-w2, h2, -d2);
			vertexDataArr[22].pos = XMFLOAT3(w2, h2, -d2);
			vertexDataArr[23].pos = XMFLOAT3(w2, -h2, -d2);
	
			for (UINT i = 0; i < 4; ++i)
			{
				// 右面(+X 面)
				vertexDataArr[i].normal = XMFLOAT3(1.0f, 0.0f, 0.0f);
				vertexDataArr[i].tangent = XMFLOAT3(0.0f, 0.0f, 1.0f);
				vertexDataArr[i].color = color;
				// 左面(-X 面)
				vertexDataArr[i + 4].normal = XMFLOAT3(-1.0f, 0.0f, 0.0f);
				vertexDataArr[i + 4].tangent = XMFLOAT3(0.0f, 0.0f, -1.0f);
				vertexDataArr[i + 4].color = color;
				// 顶面(+Y 面)
				vertexDataArr[i + 8].normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
				vertexDataArr[i + 8].tangent = XMFLOAT3(1.0f, 0.0f, 0.0f);
				vertexDataArr[i + 8].color = color;
				// 底面(-Y 面)
				vertexDataArr[i + 12].normal = XMFLOAT3(0.0f, -1.0f, 0.0f);
				vertexDataArr[i + 12].tangent = XMFLOAT3(-1.0f, 0.0f, 0.0f);
				vertexDataArr[i + 12].color = color;
				// 背面(+Z 面)
				vertexDataArr[i + 16].normal = XMFLOAT3(0.0f, 0.0f, 1.0f);
				vertexDataArr[i + 16].tangent = XMFLOAT3(-1.0f, 0.0f, 0.0f);
				vertexDataArr[i + 16].color = color;
				// 正面(-Z 面)
				vertexDataArr[i + 20].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
				vertexDataArr[i + 20].tangent = XMFLOAT3(1.0f, 0.0f, 0.0f);
				vertexDataArr[i + 20].color = color;
			}
	
			for (UINT i = 0; i < 6; ++i)
			{
				vertexDataArr[i * 4].tex = XMFLOAT2(0.0f, 1.0f);
				vertexDataArr[i * 4 + 1].tex = XMFLOAT2(0.0f, 0.0f);
				vertexDataArr[i * 4 + 2].tex = XMFLOAT2(1.0f, 0.0f);
				vertexDataArr[i * 4 + 3].tex = XMFLOAT2(1.0f, 1.0f);
			}
	
			for (UINT i = 0; i < 24; ++i)
			{
				meshData.insert(vertexDataArr[i]);
			}
	
			meshData.indexVec = {
				0, 1, 2, 2, 3, 0,		// 右面(+X 面)
				4, 5, 6, 6, 7, 4,		// 左面(-X 面)
				8, 9, 10, 10, 11, 8,	// 顶面(+Y 面)
				12, 13, 14, 14, 15, 12,	// 底面(-Y 面)
				16, 17, 18, 18, 19, 16, // 背面(+Z 面)
				20, 21, 22, 22, 23, 20	// 正面(-Z 面)
			};
	
			return meshData;
		}
	
		
		inline MeshData CreateCylinder(float radius, float height,UINT levels, UINT slices, const DirectX::XMFLOAT4 & color)
		{
			using namespace DirectX;
	
			auto meshData = CreateCylinderNoCap(radius, height, levels, slices, color);

			VertexData vertexData;

			// 放入底端圆心
			vertexData = { XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f), color, XMFLOAT2(0.5f, 0.5f) };
			meshData.insert(vertexData);
			// 放入顶端圆心
			vertexData = { XMFLOAT3(0.0f, height, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT3(1.0f, 0.0f, 0.0f), color, XMFLOAT2(0.5f, 0.5f) };
			meshData.insert(vertexData);

			UINT vertexlast = meshData.position.size() - 1;
			UINT iIndex = meshData.indexVec.size();

			// 逐渐放入底部三角形索引
			for (UINT i = 0; i < slices; ++i)
			{
				meshData.indexVec.push_back(i + 1);
				meshData.indexVec.push_back(i);
				meshData.indexVec.push_back(vertexlast-1);
			}
	
			// 逐渐放入底部三角形索引
			UINT offset = (slices + 1) * (levels - 1);
			for (UINT i = 0; i < slices; ++i)
			{
				meshData.indexVec.push_back(offset + i);
				meshData.indexVec.push_back(offset + i + 1);
				meshData.indexVec.push_back(vertexlast);
			}
	
			return meshData;
		}
	
		
		inline MeshData CreateCylinderNoCap(float radius, float height,UINT levels, UINT slices, const DirectX::XMFLOAT4 & color)
		{
			using namespace DirectX;
	
			UINT vertexCount = levels * (slices + 1);
			UINT indexCount = 6 * (levels - 2) * slices;

			MeshData meshData(vertexCount, indexCount);
			VertexData vertexData;

			float h2 = 0.0f;
			float theta = 0.0f;
			float per_height = height / (levels - 1);
			float per_theta = XM_2PI / slices;
	
			// 放入侧面端点
			for (UINT i =0; i< levels; ++i)
			{
				h2 = per_height * i;
				for (UINT j = 0; j <= slices; ++j)
				{
					theta = j * per_theta;
					vertexData = { XMFLOAT3(radius * cosf(theta), h2, radius * sinf(theta)), XMFLOAT3(cosf(theta), 0.0f, sinf(theta)),
						XMFLOAT3(-sinf(theta), 0.0f, cosf(theta)), color, XMFLOAT2(theta / XM_2PI, 0.0f) };
					meshData.insert(vertexData);
				}
			}
			// 放入索引
			UINT iIndex = 0;
	
			for (UINT i = 0; i < levels-1; ++i)
			{
				for (UINT j=0; j< slices; ++j)
				{

					meshData.indexVec.push_back((slices + 1) * i + j);
					meshData.indexVec.push_back((slices + 1) * i + j + 1);
					meshData.indexVec.push_back((slices + 1) * i + (slices + 1) + j + 1); 
					
					meshData.indexVec.push_back((slices + 1) * i + (slices + 1) + j + 1);
					meshData.indexVec.push_back((slices + 1) * i + (slices + 1) + j);
					meshData.indexVec.push_back((slices + 1) * i + j);
				}
			}
	
			return meshData;
		}
		
		inline MeshData Create2DShow(DirectX::XMFLOAT2 & center, const DirectX::XMFLOAT2 & scale,
			const DirectX::XMFLOAT4 & color)
		{
			return Create2DShow(center.x, center.y, scale.x, scale.y, color);
		}
		
		inline MeshData Create2DShow(float centerX, float centerY, float scaleX, float scaleY, const DirectX::XMFLOAT4 & color)
		{
			using namespace DirectX;
	
			MeshData meshData(4, 6);
			VertexData vertexData;

			UINT vIndex = 0;
	
			vertexData = { XMFLOAT3(centerX - scaleX, centerY - scaleY, 0.0f), XMFLOAT3(0.0f, 0.0f, -1.0f),
				XMFLOAT3(1.0f, 0.0f, 0.0f), color, XMFLOAT2(0.0f, 1.0f) };
			meshData.insert(vertexData);
	
			vertexData = { XMFLOAT3(centerX - scaleX, centerY + scaleY, 0.0f), XMFLOAT3(0.0f, 0.0f, -1.0f),
				XMFLOAT3(1.0f, 0.0f, 0.0f), color, XMFLOAT2(0.0f, 0.0f) };
			meshData.insert(vertexData);
	
			vertexData = { XMFLOAT3(centerX + scaleX, centerY + scaleY, 0.0f), XMFLOAT3(0.0f, 0.0f, -1.0f),
				XMFLOAT3(1.0f, 0.0f, 0.0f), color, XMFLOAT2(1.0f, 0.0f) };
			meshData.insert(vertexData);
	
			vertexData = { XMFLOAT3(centerX + scaleX, centerY - scaleY, 0.0f), XMFLOAT3(0.0f, 0.0f, -1.0f),
				XMFLOAT3(1.0f, 0.0f, 0.0f), color, XMFLOAT2(1.0f, 1.0f) };
			meshData.insert(vertexData);
	
			meshData.indexVec = { 0, 1, 2, 2, 3, 0 };
			return meshData;
		}
	
		
		inline MeshData CreatePlane(const DirectX::XMFLOAT3 & center, const DirectX::XMFLOAT2 & planeSize,
			const DirectX::XMFLOAT2 & maxTexCoord, const DirectX::XMFLOAT4 & color)
		{
			return CreatePlane(center.x, center.y, center.z, planeSize.x, planeSize.y, maxTexCoord.x, maxTexCoord.y, color);
		}
	
		
		inline MeshData CreatePlane(float centerX, float centerY, float centerZ, float width, float depth, float texU, float texV, const DirectX::XMFLOAT4 & color)
		{
			using namespace DirectX;

			MeshData meshData(4, 6);
			VertexData vertexData;

			UINT vIndex = 0;
	
			vertexData = { XMFLOAT3(centerX - width / 2, centerY, centerZ - depth / 2), XMFLOAT3(0.0f, 1.0f, 0.0f),
				XMFLOAT3(1.0f, 0.0f, 0.0f), color, XMFLOAT2(0.0f, texV) };
			meshData.insert(vertexData);
	
			vertexData = { XMFLOAT3(centerX - width / 2, centerY, centerZ + depth / 2), XMFLOAT3(0.0f, 1.0f, 0.0f),
				XMFLOAT3(1.0f, 0.0f, 0.0f), color, XMFLOAT2(0.0f, 0.0f) };
			meshData.insert(vertexData);
	
			vertexData = { XMFLOAT3(centerX + width / 2, centerY, centerZ + depth / 2), XMFLOAT3(0.0f, 1.0f, 0.0f),
				XMFLOAT3(1.0f, 0.0f, 0.0f), color, XMFLOAT2(texU, 0.0f) };
			meshData.insert(vertexData);
	
			vertexData = { XMFLOAT3(centerX + width / 2, centerY, centerZ - depth / 2), XMFLOAT3(0.0f, 1.0f, 0.0f),
				XMFLOAT3(1.0f, 0.0f, 0.0f), color, XMFLOAT2(texU, texV) };
			meshData.insert(vertexData);
	
			meshData.indexVec = { 0, 1, 2, 2, 3, 0 };
			return meshData;
		}

		inline MeshData CreateTerrain(UINT levels /*= 10*/, BitMap* heightmap /*= nullptr*/)
		{
			using namespace DirectX;

			assert(levels >= 5);

			UINT vertexCount = (levels + 1) * (levels + 1);
			UINT indexCount = levels * (levels + 1) * 6;

			MeshData meshData(vertexCount, indexCount);

			XMFLOAT4 color(1.0f, 1.0f, 1.0f, 1.0f);

			float pos = levels / 2.0f;
			float phi = 1.0f;
			float posX, posZ;
			int index1, index2, index3, index4;

			if (heightmap == nullptr)
			{
				// 添加顶点
				for (UINT i = 0; i <= levels; ++i)
				{
					for (UINT j = 0; j <= levels; ++j)
					{
						posX = phi * j;
						posZ = phi * i;
						meshData.insert( { XMFLOAT3(posX - pos, -0.001f, posZ - pos),  XMFLOAT3(0.0f, 1.0f, 0.0f),
							XMFLOAT3(1.0f, 0.0f, 0.0f), color, XMFLOAT2(posX / levels, posZ / levels) });
					}
				}

				// 添加索引
				for (UINT i = 0; i < levels; ++i)
				{
					for (UINT j = 0; j < levels; ++j)
					{	
						//******************************
						//  3______4
						//  |          |
						//  |          |
						// 1¯¯¯¯¯¯¯2
						//******************************
						index1 = (levels+ 1) * i + j;
						index2 = (levels + 1) * i + j + 1;
						index3 = (levels + 1) * (i + 1) + j;
						index4 = (levels + 1) * (i + 1) + j + 1;

						meshData.indexVec.push_back(index1);
						meshData.indexVec.push_back(index3);
						meshData.indexVec.push_back(index2);

						meshData.indexVec.push_back(index4);
						meshData.indexVec.push_back(index2);
						meshData.indexVec.push_back(index3);
					}
				}
			}
			else
			{
				XMFLOAT3 position1;
				XMFLOAT3 position2;
				XMFLOAT3 position3;
				XMFLOAT3 position4;
				XMFLOAT2 tex1;
				XMFLOAT2 tex2;
				XMFLOAT2 tex3;
				XMFLOAT2 tex4;
				XMFLOAT3 normal1;
				XMFLOAT3 normal2;
				XMVECTOR vector1;
				XMVECTOR vector2;
				XMVECTOR vector3;

				UINT count = 0;
				for (UINT i = 0; i < levels; ++i)
				{
					for (UINT j = 0; j < levels; ++j)
					{
						posX = phi * j;
						posZ = phi * i;

						tex1 = XMFLOAT2(posX / levels, posZ / levels);
						tex2 = XMFLOAT2((posX + 1) / levels, posZ / levels);
						tex3 = XMFLOAT2((posX + 1) / levels, (posZ + 1) / levels);
						tex4 = XMFLOAT2(posX / levels, (posZ + 1) / levels);

						position1 = XMFLOAT3(posX - pos, heightmap->Get(tex1).y * 10, posZ - pos);
						position2 = XMFLOAT3(posX - pos + 1, heightmap->Get(tex2).y * 10, posZ - pos);
						position3 = XMFLOAT3(posX - pos + 1, heightmap->Get(tex3).y * 10, posZ - pos + 1);
						position4 = XMFLOAT3(posX - pos, heightmap->Get(tex4).y * 10, posZ - pos + 1);

						vector1 = XMLoadFloat3(&position1);
						vector2 = XMLoadFloat3(&position4);
						vector3 = XMLoadFloat3(&position3);
						XMStoreFloat3(&normal1, XMVector2Cross(vector2 - vector1, vector3 - vector1));
						vector2 = XMLoadFloat3(&position2);
						XMStoreFloat3(&normal2, XMVector2Cross(vector2 - vector1, vector3 - vector1));

						meshData.insert( {position1,  normal1, XMFLOAT3(1.0f, 0.0f, 0.0f), color, tex1 });
						meshData.indexVec.push_back(count++);
						meshData.insert({ position4,  normal1, XMFLOAT3(1.0f, 0.0f, 0.0f), color, tex4 });
						meshData.indexVec.push_back(count++);
						meshData.insert({ position3,  normal1, XMFLOAT3(1.0f, 0.0f, 0.0f), color, tex3 });
						meshData.indexVec.push_back(count++);


						meshData.insert({ position3,  normal2, XMFLOAT3(1.0f, 0.0f, 0.0f), color, tex3 });
						meshData.indexVec.push_back(count++);
						meshData.insert({ position2,  normal2, XMFLOAT3(1.0f, 0.0f, 0.0f), color, tex2 });
						meshData.indexVec.push_back(count++);
						meshData.insert({ position1,  normal2, XMFLOAT3(1.0f, 0.0f, 0.0f), color, tex1 });
						meshData.indexVec.push_back(count++);
					}
				}
			}

			return meshData;
		}
	}
}

#endif
