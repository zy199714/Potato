#ifndef MATERIAL_H
#define MATERIAL_H

#pragma once
#include "tools/Util.h"
#include "tools/EffectBase.h"

namespace Potato
{
	class Model;

	class Material : public nocopy
	{
	public:
		Material();
		~Material();

		EffectBase* GetEffect() const;
		void SetEffect(EffectBase* effect);

		virtual bool InitResource(ID3D11Device* device);
		
	private:
		EffectBase* mEffect;
	};
}

/**/

#endif // !MATERIAL_H