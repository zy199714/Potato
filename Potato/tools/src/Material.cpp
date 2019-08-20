#include "tools/Material.h"

namespace Potato
{
	Material::Material()
	{

	}

	Material::~Material()
	{

	}

	EffectBase* Material::GetEffect() const
	{
		return mEffect;
	}

	void Material::SetEffect(EffectBase* effect)
	{
		mEffect = effect;
	}

	bool Material::InitResource(ID3D11Device* device)
	{
		return true;
	}


}