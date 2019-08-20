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
		 * 默认
		 */
		TextureType_NONE = 0x0,
		/**
		 * 漫反射，参与光照计算
		 */
		TextureType_DIFFUSE,
		/**
		 * 镜面反射
		 */
		TextureType_SPECULAR,
		/**
		 * 环境光照
		 */
		TextureType_AMBINENT,
		/**
		 * 自发光
		 */ 
		TextureType_EMISSIVE,
		/**
		 * 高度图
		 */
		TextureType_HEIGHT,
		/**
		 * 法线
		 */
		TextureType_NORMALS,
		/**
		 * 光泽度
		 */
		TextureType_SHININESS,
		/**
		 * 不透明度
		 * 一般为：不透明 1 -------- 0 透明
		 */
		TextureType_OPACTIY,
		/**
		 * 置换贴图
		 * 较高的颜色值代表较高的顶点位移
		 */
		TextureType_DISPLACEMENT,
		/**
		 * 环境光遮罩
		 */
		TextureType_LIGHTMAP,
		/**
		 * 反射
		 */
		TextureType_REFLECTION,
		/**
		 * 结束
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
		/* 添加贴图文件 */
		void AddTexture(TextureType type, std::vector<std::wstring>* texturesfile);
		void AddTexture(TextureType type, std::wstring texturefile);
		/* 初始化缓冲区 */
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



