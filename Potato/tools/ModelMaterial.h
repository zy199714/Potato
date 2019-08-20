#pragma once

#ifndef MODELMATERIAL_H
#define MODELMATERIAL_H
#pragma once
#include "tools/Util.h"

struct aiMaterial;

namespace Potato
{
	enum TextureType
	{
		/**
		 * Ĭ��
		 */
		TextureType_NONE = 0x0,
		/**
		 * �����䣬������ռ���
		 */
		TextureType_DIFFUSE,
		/**
		 * ���淴��
		 */
		TextureType_SPECULAR,
		/**
		 * ��������
		 */
		TextureType_AMBINENT,
		/**
		 * �Է���
		 */ 
		TextureType_EMISSIVE,
		/**
		 * �߶�ͼ
		 */
		TextureType_HEIGHT,
		/**
		 * ����
		 */
		TextureType_NORMALS,
		/**
		 * �����
		 */
		TextureType_SHININESS,
		/**
		 * ��͸����
		 * һ��Ϊ����͸�� 1 -------- 0 ͸��
		 */
		TextureType_OPACTIY,
		/**
		 * �û���ͼ
		 * �ϸߵ���ɫֵ����ϸߵĶ���λ��
		 */
		TextureType_DISPLACEMENT,
		/**
		 * ����������
		 */
		TextureType_LIGHTMAP,
		/**
		 * ����
		 */
		TextureType_REFLECTION,
		/**
		 * ����
		 */
		TextureType_END,
	};

	class ModelMaterial :public nocopy
	{
	public:
		ModelMaterial();
		ModelMaterial(ID3D11Device* device, aiMaterial* material);
		~ModelMaterial();

	public:
		/* �����ͼ�ļ� */
		void AddTexture(TextureType type, std::vector<std::wstring>* texturesfile);
		void AddTexture(TextureType type, std::wstring texturefile);
		/* ��ʼ�������� */
		void InitBuffer(ID3D11Device* device);

		std::vector<ID3D11ShaderResourceView*>* GetSRVBuffer(TextureType type);
	private:
		void InitializeTextureTypeMappings();
	private:
		std::map<TextureType, UINT> mTextureTypeMappings;

		std::map<TextureType, std::vector<std::wstring>*> mTextures;
		std::map<TextureType, std::vector<ID3D11ShaderResourceView*>*> mTexturesBuffer;
	};
}

#endif // !MODELMATERIAL_H



