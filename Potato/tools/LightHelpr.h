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
		XMFLOAT4 ambient{}; // 环境光颜色
		XMFLOAT4 diffuse{}; // 漫反射颜色(物体表面颜色)
		XMFLOAT4 specular{}; // 镜面反射
		XMFLOAT3 direction{};  // 方向
		float pad{};
	};

	/* 点光 */
	struct pointlight
	{
		XMFLOAT4 ambient{}; // 环境光颜色
		XMFLOAT4 diffuse{}; // 漫反射颜色(物体表面颜色)
		XMFLOAT4 specular{}; // 镜面反射

		XMFLOAT3 position{}; // 位置
		float range{};  // 范围

		XMFLOAT3 att{};
		float pad{};
	};

	/* 聚光灯 */
	struct spotlight
	{
		XMFLOAT4 ambient{}; // 环境光颜色
		XMFLOAT4 diffuse{}; // 漫反射颜色(物体表面颜色)
		XMFLOAT4 specular{}; // 镜面反射

		XMFLOAT3 position{}; // 位置
		float range{};  // 范围

		XMFLOAT3 direction{};  // 方向
		float spot;

		XMFLOAT3 att;
		float pad{};
	};

	/* 物体表面材质 */
	struct material
	{
		XMFLOAT4 ambient{}; // 环境光颜色
		XMFLOAT4 diffuse{}; // 漫反射颜色(物体表面颜色)
		XMFLOAT4 specular{}; // 镜面反射
		XMFLOAT4 Reflect{}; // 
	};
}
#endif // LightHelpr_H