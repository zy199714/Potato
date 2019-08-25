//***************************************************************************************
// 定义顶点结构和输入布局
//***************************************************************************************

#ifndef VERTEX_H
#define VERTEX_H

#pragma once
#include <d3d11_1.h>
#include <DirectXMath.h>

namespace Potato
{
	using namespace DirectX;

	namespace 
	{
		static const D3D11_INPUT_ELEMENT_DESC inputLayoutPos[1] = 
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};
		static const D3D11_INPUT_ELEMENT_DESC inputLayoutPosColor[2] = 
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};
		static const D3D11_INPUT_ELEMENT_DESC inputLayoutPosTex[2] = 
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 1, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};

		static const D3D11_INPUT_ELEMENT_DESC inputLayoutPosSize[2] = 
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "SIZE", 0, DXGI_FORMAT_R32G32_FLOAT, 1, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};

		static const D3D11_INPUT_ELEMENT_DESC inputLayoutPosNormalColor[3] = 
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 1, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 2, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};

		static const D3D11_INPUT_ELEMENT_DESC inputLayoutPosNormalTex[3] = 
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 1, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 2, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};

		static const D3D11_INPUT_ELEMENT_DESC inputLayoutPosNormalTangentTex[4] = 
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 1, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TANGENT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 2, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 3, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};

		static const D3D11_INPUT_ELEMENT_DESC inputLayoutInstancePosNormalTex[11] = 
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 1, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 2, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "World", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 3, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1},
			{ "World", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 3, 16, D3D11_INPUT_PER_INSTANCE_DATA, 1},
			{ "World", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 3, 32, D3D11_INPUT_PER_INSTANCE_DATA, 1},
			{ "World", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 3, 48, D3D11_INPUT_PER_INSTANCE_DATA, 1},
			{ "WorldInvTranspose", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 3, 64, D3D11_INPUT_PER_INSTANCE_DATA, 1},
			{ "WorldInvTranspose", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 3, 80, D3D11_INPUT_PER_INSTANCE_DATA, 1},
			{ "WorldInvTranspose", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 3, 96, D3D11_INPUT_PER_INSTANCE_DATA, 1},
			{ "WorldInvTranspose", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 3, 112, D3D11_INPUT_PER_INSTANCE_DATA, 1}
		};

		/* strides 缓冲区跨度 */
		static const UINT StridesPos[] = { sizeof(XMFLOAT3) };
		static const UINT StridesPosColor[] = { sizeof(XMFLOAT3), sizeof(XMFLOAT4) };
		static const UINT StridesPosTex[] = { sizeof(XMFLOAT3), sizeof(XMFLOAT2) };
		static const UINT StridesPosSize[] = { sizeof(XMFLOAT3), sizeof(XMFLOAT2) };
		static const UINT StridesPosNormalColor[] = { sizeof(XMFLOAT3), sizeof(XMFLOAT3), sizeof(XMFLOAT4) };
		static const UINT StridesPosNormalTex[] = { sizeof(XMFLOAT3), sizeof(XMFLOAT3), sizeof(XMFLOAT2) };
		static const UINT StridesPosNormalTangentTex[] = { sizeof(XMFLOAT3), sizeof(XMFLOAT3),  sizeof(XMFLOAT3), sizeof(XMFLOAT2) };
		static const UINT StridesPosNormalTex_WorldWorldT[] = { sizeof(XMFLOAT3), sizeof(XMFLOAT3), sizeof(XMFLOAT2), sizeof(XMMATRIX) * 2};
		/* offsets 缓冲区偏移 */
		static const UINT OffsetPos[] = { 0};
		static const UINT OffsetPosColor[] = { 0, 0 };
		static const UINT OffsetPosTex[] = {0, 0 };
		static const UINT OffsetPosSize[] = { 0, 0 };
		static const UINT OffsetPosNormalColor[] = { 0, 0, 0 };
		static const UINT OffsetPosNormalTex[] = { 0, 0, 0 };
		static const UINT OffsetPosNormalTangentTex[] = { 0, 0, 0, 0 };
		static const UINT OffsetPosNormalTex_WorldWorldT[] = { 0, 0, 0, 0 };

	}; 

	/************************************************************************/
	/* 顶点组成结构                                                                              */
	/* XMFLOAT3 Pos 顶点的位置坐标                                                  */
	/* XMFLOAT4 Color 顶点的颜色                                                      */
	/* XMFLOAT2 Size 顶点的大小                                                        */
	/* XMFLOAT2 Tex 纹理坐标                                                            */
	/* XMFLOAT3 Normal 顶点的法线                                                   */
	/* XMFLOAT3 Tangent 顶点的切线                                                  */
	/************************************************************************/
	enum VertexType
	{
		/* 单对象绘制顶点结构*/
		VertexType_Pos = 0x0,
		VertexType_PosColor = 0x1,
		VertexType_PosTex = 0x2,
		VertexType_PosSize = 0x3,
		VertexType_PosNormalColor = 0x4,
		VertexType_PosNormalTex = 0x5,
		VertexType_PosNormalTangentTex = 0x6,
		/* 实例绘制顶点结构 */
		VertexType_Instance_PosNormalTex_WorldWorldT = 0x7
	};

	static const UINT VertexSize[] =
	{
		sizeof(XMFLOAT3),
		sizeof(XMFLOAT3) + sizeof(XMFLOAT4),
		sizeof(XMFLOAT3) + sizeof(XMFLOAT2),
		sizeof(XMFLOAT3) + sizeof(XMFLOAT2),
		sizeof(XMFLOAT3) + sizeof(XMFLOAT3) + sizeof(XMFLOAT4),
		sizeof(XMFLOAT3) + sizeof(XMFLOAT3) + sizeof(XMFLOAT2),
		sizeof(XMFLOAT3) + sizeof(XMFLOAT3) + sizeof(XMFLOAT3) + sizeof(XMFLOAT2),
		sizeof(XMFLOAT3) + sizeof(XMFLOAT3) + sizeof(XMFLOAT2) + sizeof(XMMATRIX) + sizeof(XMMATRIX)
	};

	static const UINT  VertexBufferCount[] = { 1, 2, 2, 2, 3, 3, 4, 4 };
	 
	static const UINT* VertexStrides[] = 
	{
		StridesPos,
		StridesPosColor,
		StridesPosTex,
		StridesPosSize,
		StridesPosNormalColor,
		StridesPosNormalTex,
		StridesPosNormalTangentTex,
		StridesPosNormalTex_WorldWorldT
	};

	static const UINT* VertexOffsets[] =
	{
		OffsetPos,
		OffsetPosColor,
		OffsetPosTex,
		OffsetPosSize,
		OffsetPosNormalColor,
		OffsetPosNormalTex,
		OffsetPosNormalTangentTex,
		OffsetPosNormalTex_WorldWorldT
	};

	static const D3D11_INPUT_ELEMENT_DESC* InputLayout[] =
	{
		inputLayoutPos,
		inputLayoutPosColor,
		inputLayoutPosTex,
		inputLayoutPosSize,
		inputLayoutPosNormalColor,
		inputLayoutPosNormalTex,
		inputLayoutPosNormalTangentTex,
		inputLayoutInstancePosNormalTex
	};

	static const UINT InputLayoutCount[] = { 1, 2, 2, 2, 3, 3, 4, 11 };

}

#endif !VERTEX_H
