/**
 * @file Util.h
 * 
 */

#ifndef UTIL_H
#define UTIL_H

#pragma once

#include <d3d11.h>
#include <DirectXColors.h>
#include <d3dcompiler.h>
#include <filesystem>
#include "nocopy.h"
#include "def.h"

#include "DDSTextureLoader.h"	
#include "WICTextureLoader.h"

namespace Potato
{
	//
	// ��ɫ��������غ���
	//

	// ------------------------------
	// CreateShaderFromFile����
	// ------------------------------
	// [In]csoFileNameInOut ����õ���ɫ���������ļ�(.cso)������ָ��������Ѱ�Ҹ��ļ�����ȡ
	// [In]hlslFileName     ��ɫ�����룬��δ�ҵ���ɫ���������ļ��������ɫ������
	// [In]entryPoint       ��ڵ�(ָ����ʼ�ĺ���)
	// [In]shaderModel      ��ɫ��ģ�ͣ���ʽΪ"*s_5_0"��*����Ϊc,d,g,h,p,v֮һ
	// [Out]ppBlobOut       �����ɫ����������Ϣ
	HRESULT CreateShaderFromFile(
		const WCHAR* csoFileNameInOut,
		const WCHAR* hlslFileName,
		LPCSTR entryPoint,
		LPCSTR shaderModel,
		ID3DBlob** ppBlobOut);

	//
	// ����������غ���
	//

	// ------------------------------
	// CreateTexture2DArray����
	// ------------------------------
	// ����������������������
	// [In]d3dDevice			D3D�豸
	// [In]d3dDeviceContext		D3D�豸������
	// [In]srcTexVec			������������
	// [In]usage				D3D11_USAGEö��ֵ
	// [In]bindFlags			D3D11_BIND_FLAGö��ֵ
	// [In]cpuAccessFlags		D3D11_CPU_ACCESS_FLAGö��ֵ
	// [In]miscFlags			D3D11_RESOURCE_MISC_FLAGö��ֵ
	// [OutOpt]textureArray		���������������Դ
	// [OutOpt]textureCubeView	���������������Դ��ͼ
	static HRESULT CreateTexture2DArray(
		ID3D11Device * d3dDevice,
		ID3D11DeviceContext * d3dDeviceContext,
		std::vector<ID3D11Texture2D*>& srcTexVec,
		D3D11_USAGE usage,
		UINT bindFlags,
		UINT cpuAccessFlags,
		UINT miscFlags,
		ID3D11Texture2D** textureArray,
		ID3D11ShaderResourceView** textureArrayView);


	// ------------------------------
	// CreateDDSTexture2DArrayFromFile����
	// ------------------------------
	// �ú���Ҫ�����е�dds����Ŀ�ߡ����ݸ�ʽ��mip�ȼ�һ��
	// [In]d3dDevice			D3D�豸
	// [In]d3dDeviceContext		D3D�豸������
	// [In]fileNames			ddS�ļ�������
	// [OutOpt]textureArray		���������������Դ
	// [OutOpt]textureArrayView ���������������Դ��ͼ
	// [In]generateMips			�Ƿ�����mipmaps
	HRESULT CreateDDSTexture2DArrayFromFile(
		ID3D11Device * d3dDevice,
		ID3D11DeviceContext * d3dDeviceContext,
		const std::vector<std::wstring>& fileNames,
		ID3D11Texture2D** textureArray,
		ID3D11ShaderResourceView** textureArrayView,
		bool generateMips = false);

	// ------------------------------
	// CreateWICTexture2DArrayFromFile����
	// ------------------------------
	// �ú���Ҫ�����е�dds����Ŀ�ߡ����ݸ�ʽ��mip�ȼ�һ��
	// [In]d3dDevice			D3D�豸
	// [In]d3dDeviceContext		D3D�豸������
	// [In]fileNames			ddS�ļ�������
	// [OutOpt]textureArray		���������������Դ
	// [OutOpt]textureArrayView ���������������Դ��ͼ
	// [In]generateMips			�Ƿ�����mipmaps
	HRESULT CreateWICTexture2DArrayFromFile(
		ID3D11Device * d3dDevice,
		ID3D11DeviceContext * d3dDeviceContext,
		const std::vector<std::wstring>& fileNames,
		ID3D11Texture2D** textureArray,
		ID3D11ShaderResourceView** textureArrayView,
		bool generateMips = false);


	//
	// ������������غ���
	//


	// ------------------------------
	// CreateWICTexture2DCubeFromFile����
	// ------------------------------
	// ���ݸ�����һ�Ű����������������λͼ����������������
	// Ҫ�������߱�Ϊ4:3���Ұ�������ʽ����:
	// .  +Y .  .
	// -X +Z +X -Z 
	// .  -Y .  .
	// [In]d3dDevice			D3D�豸
	// [In]d3dDeviceContext		D3D�豸������
	// [In]cubeMapFileName		λͼ�ļ���
	// [OutOpt]textureArray		���������������Դ
	// [OutOpt]textureCubeView	�����������������Դ��ͼ
	// [In]generateMips			�Ƿ�����mipmaps
	HRESULT CreateWICTexture2DCubeFromFile(
		ID3D11Device * d3dDevice,
		ID3D11DeviceContext * d3dDeviceContext,
		const std::wstring& cubeMapFileName,
		ID3D11Texture2D** textureArray,
		ID3D11ShaderResourceView** textureCubeView,
		bool generateMips = false);

	// ------------------------------
	// CreateWICTexture2DCubeFromFile����
	// ------------------------------
	// ���ݰ�D3D11_TEXTURECUBE_FACE����˳�����������������������������
	// Ҫ��λͼ��ͬ����ߡ����ݸ�ʽ��������
	// ��Ҳ���Ը�������6�ŵ�����Ȼ���ڻ�ȡ����������Ļ��������д����������Դ��ͼ
	// [In]d3dDevice			D3D�豸
	// [In]d3dDeviceContext		D3D�豸������
	// [In]cubeMapFileNames		λͼ�ļ�������
	// [OutOpt]textureArray		���������������Դ
	// [OutOpt]textureCubeView	�����������������Դ��ͼ
	// [In]generateMips			�Ƿ�����mipmaps
	HRESULT CreateWICTexture2DCubeFromFile(
		ID3D11Device * d3dDevice,
		ID3D11DeviceContext * d3dDeviceContext,
		const std::vector<std::wstring>& cubeMapFileNames,
		ID3D11Texture2D** textureArray,
		ID3D11ShaderResourceView** textureCubeView,
		bool generateMips = false);
}
#endif // !UTIL_H
