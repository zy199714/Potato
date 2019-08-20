//***************************************************************************************
// ���ɳ����ļ���������ģ��
// Generate common geometry meshes.
//***************************************************************************************

#ifndef GEOMETRY_H_
#define GEOMETRY_H_

#include <vector>
#include <string>
#include <map>
#include "Vertex.h"

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
		// ��������
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
	
		// �����������ݣ�levels ˮƽ�ֶ�(>=1)��slices��ֱ�ֶ�(>=3)��Խ�󾫶�Խ��
		MeshData CreateSphere(float radius = 1.0f, UINT levels = 20, UINT slices = 20, const DirectX::XMFLOAT4& color = { 1.0f, 1.0f, 1.0f, 1.0f });
	
		// ������������������
		MeshData CreateBox(float width = 2.0f, float height = 2.0f, float depth = 2.0f, const DirectX::XMFLOAT4& color = { 1.0f, 1.0f, 1.0f, 1.0f });
	
		// Բ�����������ݣ�levels ˮƽ�ֶ�(>=2)��slices��ֱ�ֶ�(>=3)��Խ�󾫶�Խ��
		MeshData CreateCylinder(float radius = 1.0f, float height = 2.0f, UINT levels = 2, UINT slices = 16, const DirectX::XMFLOAT4& color = { 1.0f, 1.0f, 1.0f, 1.0f });
	
		// ֻ��Բ���������������ݣ�levels ˮƽ�ֶ�(>=2)��slices��ֱ�ֶ�(>=3)��Խ�󾫶�Խ��
		MeshData CreateCylinderNoCap(float radius = 1.0f, float height = 2.0f, UINT levels = 2, UINT slices = 16,
			const DirectX::XMFLOAT4& color = { 1.0f, 1.0f, 1.0f, 1.0f });
	
		// ����NDC��Ļ����
		MeshData Create2DShow( const DirectX::XMFLOAT2& center, const DirectX::XMFLOAT2& scale = { 1.0f, 1.0f },
			const DirectX::XMFLOAT4& color = { 1.0f, 1.0f, 1.0f, 1.0f });
		MeshData Create2DShow(float centerX = 0.0f, float centerY = 0.0f, float scaleX = 1.0f, float scaleY = 1.0f,
			const DirectX::XMFLOAT4& color = { 1.0f, 1.0f, 1.0f, 1.0f });
	
		// ƽ��
		MeshData CreatePlane(const DirectX::XMFLOAT3& center, const DirectX::XMFLOAT2& planeSize = { 10.0f, 10.0f },
			const DirectX::XMFLOAT2& maxTexCoord = { 1.0f, 1.0f }, const DirectX::XMFLOAT4& color = { 1.0f, 1.0f, 1.0f, 1.0f });
		MeshData CreatePlane(float centerX = 0.0f, float centerY = 0.0f, float centerZ = 0.0f,
			float width = 10.0f, float depth = 10.0f, float texU = 1.0f, float texV = 1.0f,
			const DirectX::XMFLOAT4& color = { 1.0f, 1.0f, 1.0f, 1.0f });
		// ��ƽ�棬��ԭ��Ϊ���ĵķ��Σ�levels �ֶ�
		MeshData CreateTerrain(UINT levels = 10, const DirectX::XMFLOAT4& color = { 1.0f, 1.0f, 1.0f, 1.0f });
}
	
	namespace Geometry
	{
		//
		// �����巽����ʵ��
		//
	
		inline MeshData CreateSphere(float radius, UINT levels, UINT slices, const DirectX::XMFLOAT4 & color)
		{
			using namespace DirectX;

			assert(levels >= 1 && slices >= 3);
	
			UINT vertexCount = 2 + (levels) * (slices + 1);
			// indexCount = 6 * (levels-1) * slices + 3 * 2 * slices;
			UINT indexCount = 6 * levels * slices;

			MeshData meshData(vertexCount, indexCount);
			VertexData vertexData;
	
			float phi = 0.0f, theta = 0.0f;
			float per_phi = XM_PI / (levels+1);
			float per_theta = XM_2PI / slices;
			float x, y, z;
	
			// ���붥�˵�
			vertexData = { XMFLOAT3(0.0f, radius, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT3(1.0f, 0.0f, 0.0f), color, XMFLOAT2(0.0f, 0.0f) };
			meshData.insert(vertexData);
	
			for (UINT i = 1; i <= levels; ++i)
			{
				phi = per_phi * i;
				// ��Ҫslices + 1����������Ϊ �����յ���Ϊͬһ�㣬����������ֵ��һ��
				for (UINT j = 0; j <= slices; ++j)
				{
					theta = per_theta * j;
					x = radius * sinf(phi) * cosf(theta);
					y = radius * cosf(phi);
					z = radius * sinf(phi) * sinf(theta);
					// ������ֲ����ꡢ��������Tangent��������������
					XMFLOAT3 pos = XMFLOAT3(x, y, z), normal;
					XMStoreFloat3(&normal, XMVector3Normalize(XMLoadFloat3(&pos)));
					vertexData = { pos, normal, XMFLOAT3(-sinf(theta), 0.0f, cosf(theta)), color, XMFLOAT2(theta / XM_2PI, phi / XM_PI) };

					meshData.insert(vertexData);
				}
			}
	
			// ����׶˵�
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
	
			// ����(+X ��)
			vertexDataArr[0].pos = XMFLOAT3(w2, -h2, -d2);
			vertexDataArr[1].pos = XMFLOAT3(w2, h2, -d2);
			vertexDataArr[2].pos = XMFLOAT3(w2, h2, d2);
			vertexDataArr[3].pos = XMFLOAT3(w2, -h2, d2);
			// ����(-X ��)
			vertexDataArr[4].pos = XMFLOAT3(-w2, -h2, d2);
			vertexDataArr[5].pos = XMFLOAT3(-w2, h2, d2);
			vertexDataArr[6].pos = XMFLOAT3(-w2, h2, -d2);
			vertexDataArr[7].pos = XMFLOAT3(-w2, -h2, -d2);
			// ����(+Y ��)
			vertexDataArr[8].pos = XMFLOAT3(-w2, h2, -d2);
			vertexDataArr[9].pos = XMFLOAT3(-w2, h2, d2);
			vertexDataArr[10].pos = XMFLOAT3(w2, h2, d2);
			vertexDataArr[11].pos = XMFLOAT3(w2, h2, -d2);
			// ����(-Y ��)
			vertexDataArr[12].pos = XMFLOAT3(w2, -h2, -d2);
			vertexDataArr[13].pos = XMFLOAT3(w2, -h2, d2);
			vertexDataArr[14].pos = XMFLOAT3(-w2, -h2, d2);
			vertexDataArr[15].pos = XMFLOAT3(-w2, -h2, -d2);
			// ����(+Z ��)
			vertexDataArr[16].pos = XMFLOAT3(w2, -h2, d2);
			vertexDataArr[17].pos = XMFLOAT3(w2, h2, d2);
			vertexDataArr[18].pos = XMFLOAT3(-w2, h2, d2);
			vertexDataArr[19].pos = XMFLOAT3(-w2, -h2, d2);
			// ����(-Z ��)
			vertexDataArr[20].pos = XMFLOAT3(-w2, -h2, -d2);
			vertexDataArr[21].pos = XMFLOAT3(-w2, h2, -d2);
			vertexDataArr[22].pos = XMFLOAT3(w2, h2, -d2);
			vertexDataArr[23].pos = XMFLOAT3(w2, -h2, -d2);
	
			for (UINT i = 0; i < 4; ++i)
			{
				// ����(+X ��)
				vertexDataArr[i].normal = XMFLOAT3(1.0f, 0.0f, 0.0f);
				vertexDataArr[i].tangent = XMFLOAT3(0.0f, 0.0f, 1.0f);
				vertexDataArr[i].color = color;
				// ����(-X ��)
				vertexDataArr[i + 4].normal = XMFLOAT3(-1.0f, 0.0f, 0.0f);
				vertexDataArr[i + 4].tangent = XMFLOAT3(0.0f, 0.0f, -1.0f);
				vertexDataArr[i + 4].color = color;
				// ����(+Y ��)
				vertexDataArr[i + 8].normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
				vertexDataArr[i + 8].tangent = XMFLOAT3(1.0f, 0.0f, 0.0f);
				vertexDataArr[i + 8].color = color;
				// ����(-Y ��)
				vertexDataArr[i + 12].normal = XMFLOAT3(0.0f, -1.0f, 0.0f);
				vertexDataArr[i + 12].tangent = XMFLOAT3(-1.0f, 0.0f, 0.0f);
				vertexDataArr[i + 12].color = color;
				// ����(+Z ��)
				vertexDataArr[i + 16].normal = XMFLOAT3(0.0f, 0.0f, 1.0f);
				vertexDataArr[i + 16].tangent = XMFLOAT3(-1.0f, 0.0f, 0.0f);
				vertexDataArr[i + 16].color = color;
				// ����(-Z ��)
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
				0, 1, 2, 2, 3, 0,		// ����(+X ��)
				4, 5, 6, 6, 7, 4,		// ����(-X ��)
				8, 9, 10, 10, 11, 8,	// ����(+Y ��)
				12, 13, 14, 14, 15, 12,	// ����(-Y ��)
				16, 17, 18, 18, 19, 16, // ����(+Z ��)
				20, 21, 22, 22, 23, 20	// ����(-Z ��)
			};
	
			return meshData;
		}
	
		
		inline MeshData CreateCylinder(float radius, float height,UINT levels, UINT slices, const DirectX::XMFLOAT4 & color)
		{
			using namespace DirectX;
	
			auto meshData = CreateCylinderNoCap(radius, height, levels, slices, color);

			VertexData vertexData;

			// ����׶�Բ��
			vertexData = { XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f), color, XMFLOAT2(0.5f, 0.5f) };
			meshData.insert(vertexData);
			// ���붥��Բ��
			vertexData = { XMFLOAT3(0.0f, height, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT3(1.0f, 0.0f, 0.0f), color, XMFLOAT2(0.5f, 0.5f) };
			meshData.insert(vertexData);

			UINT vertexlast = meshData.position.size() - 1;
			UINT iIndex = meshData.indexVec.size();

			// �𽥷���ײ�����������
			for (UINT i = 0; i < slices; ++i)
			{
				meshData.indexVec.push_back(i + 1);
				meshData.indexVec.push_back(i);
				meshData.indexVec.push_back(vertexlast-1);
			}
	
			// �𽥷���ײ�����������
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
	
			// �������˵�
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
			// ��������
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

		inline MeshData CreateTerrain(UINT levels /*= 10*/, const DirectX::XMFLOAT4 & color)
		{
			using namespace DirectX;

			assert(levels >= 10);

			UINT vertexCount = (levels+ 1) * (levels + 1);
			UINT indexCount = (levels) * (levels+ 1) * 2 * 2;

			MeshData meshData(vertexCount, indexCount);
			VertexData vertexData;

			float pos = levels / 2;
			float phi = 1.0f;
			UINT posX, posZ;

			for (UINT i = 0; i <= levels; ++i)
			{
				for (UINT j = 0; j <= levels; ++j)
				{
					posX = phi * j;
					posZ = phi * i;
					vertexData = { XMFLOAT3(posX - pos, 0.0f, posZ - pos),  XMFLOAT3(0.0f, 1.0f, 0.0f),
						XMFLOAT3(1.0f, 0.0f, 0.0f), color, XMFLOAT2(posX / levels, posZ / levels) };
					meshData.insert(vertexData);
				}
			}

			// �������
			for (UINT i = 0; i < levels; ++i)
			{
				for (UINT j = 0; j < levels; ++j)
				{
					meshData.indexVec.push_back(((levels+ 1) * i + j));
					meshData.indexVec.push_back(((levels+ 1) * i + j + 1));

					meshData.indexVec.push_back(((levels+ 1) * i + j));
					meshData.indexVec.push_back(((levels+ 1) * i + (levels+ 1) + j));
				}
				meshData.indexVec.push_back(((levels+ 1) * i + levels));
				meshData.indexVec.push_back(((levels+ 1) * i + (levels+ 1) + levels));
			}
			return meshData;
		}
	}
}

#endif
