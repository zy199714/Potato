#pragma once

#ifndef LightHelpr_H
#define LightHelpr_H

#pragma once
#include <DirectXMath.h>

namespace Potato
{
	using namespace DirectX;

	/* 方向光 */
	struct directionlight 
	{
		XMFLOAT3 diffuse{ 0.8f,0.8f,0.8f }; // 环境光颜色
		float pad{};
		XMFLOAT3 direction{ -0.866f, -0.5f, 0.5f }; // 漫反射颜色(物体表面颜色)
		float pad1{};
	};

	/* 点光 */
	struct pointlight
	{
		XMFLOAT3 diffuse{ 0.8f,0.8f,0.8f }; // 环境光颜色
		float falloffStart{0.0f};
		float falloffEnd{800.0f};
		XMFLOAT3 position{ 0.0f, 0.0f, 0.0f }; // 位置
	};

	/* 聚光灯 */
	struct spotlight
	{
		XMFLOAT3 diffuse{ 0.8f,0.8f,0.8f }; // 漫反射颜色(物体表面颜色)
		float falloffStart{ 0.0f };
		XMFLOAT3 direction{ 0.0f,-1.0f, 0.0f };  // 方向
		float falloffEnd{ 800.0f };
		XMFLOAT3 position{ 0.0f, 0.0f, 0.0f }; // 位置
		float spotPower{16};  // 范围
	};

	/* 物体表面材质 */
	struct material
	{
		XMFLOAT4 DiffuseAlbedo;
		XMFLOAT3 FresnelR0; // 菲尼尔
		float Shininess;   // 光滑度
	};
}
#endif // LightHelpr_H