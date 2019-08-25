#pragma once
#include "tools/EffectBase.h"

using namespace Potato;

class DefaultEffect : public EffectBase
{
public:
	DefaultEffect();
	~DefaultEffect();

	bool InitAll(ID3D11Device* device) override;
	void Apply(ID3D11DeviceContext* deviceContext) override;
public:

	void SetRenderDefault(ID3D11DeviceContext* deviceContext) override;
	void SetRenderShadowMap(ID3D11DeviceContext* deviceContext);

	/* */
	void SetRendertype(RenderType type);

	/* 矩阵设置 */
	void XM_CALLCONV SetWorldViewProjMatrix(DirectX::FXMMATRIX W, DirectX::CXMMATRIX V, DirectX::CXMMATRIX P) override;
	void XM_CALLCONV SetWorldMatrix(DirectX::FXMMATRIX W) override;
	void XM_CALLCONV SetViewMatrix(DirectX::FXMMATRIX V) override;
	void XM_CALLCONV SetProjMatrix(DirectX::FXMMATRIX P) override;


	void XM_CALLCONV SetLightViewMatrix(DirectX::FXMMATRIX LV);
	void XM_CALLCONV SetLightProjMatrix(DirectX::FXMMATRIX LP);

	/* 光照、材质和纹理相关设置 */

	void SetDirLight(size_t pos, const directionlight& dirLight);
	void SetPointLight(size_t pos, const pointlight& pointLight);
	void SetSpotLight(size_t pos, const spotlight& spotLight);
	void SetNumLight(int dir, int point, int spot);

	void SetMaterial(const material& mat);

	void SetTexture(ID3D11DeviceContext* devicecontext, UINT startslot, UINT numview, ID3D11ShaderResourceView* texture) override;
	void SetTextureShadow(ID3D11ShaderResourceView* texture);
	void SetTextureUsed(bool isUsed) override;

	/* 视点、光源位置设置 */
	void XM_CALLCONV SetEyePos(DirectX::FXMVECTOR eyePos);
	void XM_CALLCONV SetLightPos(DirectX::FXMVECTOR lightPos);

	/* 雾效设置 */
	void SetFogEnabled(bool isOn);
	void SetFog(DirectX::XMVECTOR fogColor, float fogStart, float fogRange);
	void SetFogStart(float fogStart);
	void SetFogColor(DirectX::XMVECTOR fogColor);
	void SetFogRange(float fogRange);

	/* 阴影设置 */

	void SetShadowEnabled(bool isOn);
	void SetShadowBits(float bits);
private:
	class Impl;
	std::unique_ptr<Impl> pImpl;

	RenderType mRenderType {RenderType_Object};
};

class SkyEffect : public EffectBase
{
public:
	SkyEffect();
	~SkyEffect();

	bool InitAll(ID3D11Device* device) override;
	void Apply(ID3D11DeviceContext* deviceContext) override;

public:
	void SetRenderDefault(ID3D11DeviceContext* deviceContext) override;

	void XM_CALLCONV SetWorldViewProjMatrix(DirectX::FXMMATRIX W, DirectX::CXMMATRIX V, DirectX::CXMMATRIX P) override;
	void XM_CALLCONV SetWorldMatrix(DirectX::FXMMATRIX VP) override ;
	void XM_CALLCONV SetViewMatrix(DirectX::FXMMATRIX V) override {};
	void XM_CALLCONV SetProjMatrix(DirectX::FXMMATRIX P) override {};

	void SetTexture(ID3D11DeviceContext* devicecontext, UINT startslot, UINT numview, ID3D11ShaderResourceView* texture) override;
	void SetTextureUsed(bool isUsed) override {};
private:
	class Impl;
	std::unique_ptr<Impl> pImpl;
};

class TerrainEffect : public EffectBase
{
public:
	TerrainEffect();
	~TerrainEffect();

	bool InitAll(ID3D11Device* device) override;
	void Apply(ID3D11DeviceContext* deviceContext) override;

public:
	void SetRenderDefault(ID3D11DeviceContext* deviceContext) override;

	void XM_CALLCONV SetWorldViewProjMatrix(DirectX::FXMMATRIX W, DirectX::CXMMATRIX V, DirectX::CXMMATRIX P) override {};
	void XM_CALLCONV SetWorldMatrix(DirectX::FXMMATRIX W) override {};
	void XM_CALLCONV SetViewMatrix(DirectX::FXMMATRIX PV) override;
	void XM_CALLCONV SetProjMatrix(DirectX::FXMMATRIX P) override {};

	void SetTexture(ID3D11DeviceContext* devicecontext, UINT startslot, UINT numview, ID3D11ShaderResourceView* texture) override;
	void SetTextureUsed(bool isUsed) override {};

	//void SetOffest(float offest);

	void SetDirLight(size_t pos, const directionlight& dirLight);
	void SetPointLight(size_t pos, const pointlight& pointLight);
	void SetSpotLight(size_t pos, const spotlight& spotLight);
	void SetNumLight(int dir, int point, int spot);

	void XM_CALLCONV SetEyePos(DirectX::FXMVECTOR eyePos);
private:
	class Impl;
	std::unique_ptr<Impl> pImpl;
};

