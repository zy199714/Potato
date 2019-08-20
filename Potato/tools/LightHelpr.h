#pragma once

#ifndef LightHelpr_H
#define LightHelpr_H

#pragma once
#include <DirectXMath.h>

namespace Potato
{
	using namespace DirectX;

	/* ����� */
	struct directionlight 
	{
		XMFLOAT4 ambient{}; // ��������ɫ
		XMFLOAT4 diffuse{}; // ��������ɫ(���������ɫ)
		XMFLOAT4 specular{}; // ���淴��
		XMFLOAT3 direction{};  // ����
		float pad{};
	};

	/* ��� */
	struct pointlight
	{
		XMFLOAT4 ambient{}; // ��������ɫ
		XMFLOAT4 diffuse{}; // ��������ɫ(���������ɫ)
		XMFLOAT4 specular{}; // ���淴��

		XMFLOAT3 position{}; // λ��
		float range{};  // ��Χ

		XMFLOAT3 att{};
		float pad{};
	};

	/* �۹�� */
	struct spotlight
	{
		XMFLOAT4 ambient{}; // ��������ɫ
		XMFLOAT4 diffuse{}; // ��������ɫ(���������ɫ)
		XMFLOAT4 specular{}; // ���淴��

		XMFLOAT3 position{}; // λ��
		float range{};  // ��Χ

		XMFLOAT3 direction{};  // ����
		float spot;

		XMFLOAT3 att;
		float pad{};
	};

	/* ���������� */
	struct material
	{
		XMFLOAT4 ambient{}; // ��������ɫ
		XMFLOAT4 diffuse{}; // ��������ɫ(���������ɫ)
		XMFLOAT4 specular{}; // ���淴��
		XMFLOAT4 Reflect{}; // 
	};
}
#endif // LightHelpr_H