/**
 * 
 */


#ifndef DRAWABLECOMPONENT_H
#define DRAWABLECOMPONENT_H

#pragma once

#include "core/BaseComponent.h"
#include "tools/EffectBase.h"

namespace Potato
{
	class DrawableComponent : public BaseComponent
	{
	public:
		DrawableComponent() {};
		~DrawableComponent() {};

		virtual void DrawObject(ID3D11DeviceContext* deviceContext, EffectBase* effect) = 0;

	private:

	};
}
#endif // !DRAWABLECOMPONENT_H
