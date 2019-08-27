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
		XMFLOAT3 diffuse{ 0.8f,0.8f,0.8f }; // ��������ɫ
		float pad{};
		XMFLOAT3 direction{ -0.866f, -0.5f, 0.5f }; // ��������ɫ(���������ɫ)
		float pad1{};
	};

	/* ��� */
	struct pointlight
	{
		XMFLOAT3 diffuse{ 0.8f,0.8f,0.8f }; // ��������ɫ
		float falloffStart{0.0f};
		float falloffEnd{800.0f};
		XMFLOAT3 position{ 0.0f, 0.0f, 0.0f }; // λ��
	};

	/* �۹�� */
	struct spotlight
	{
		XMFLOAT3 diffuse{ 0.8f,0.8f,0.8f }; // ��������ɫ(���������ɫ)
		float falloffStart{ 0.0f };
		XMFLOAT3 direction{ 0.0f,-1.0f, 0.0f };  // ����
		float falloffEnd{ 800.0f };
		XMFLOAT3 position{ 0.0f, 0.0f, 0.0f }; // λ��
		float spotPower{16};  // ��Χ
	};

	/* ���������� */
	struct material
	{
		XMFLOAT4 DiffuseAlbedo;
		XMFLOAT3 FresnelR0; // �����
		float Shininess;   // �⻬��
	};
}
#endif // LightHelpr_H