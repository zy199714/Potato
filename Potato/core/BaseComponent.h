#ifndef BASECOMPONENT_H
#define BASECOMPONENT_H

#pragma once
#include "tools/Util.h"

namespace Potato
{
	class  BaseComponent : public nocopy
	{
	public:
		BaseComponent() :mEnabled(true){};
		~BaseComponent() {};

		virtual void Initialize() = 0;
		virtual void Update(float dt) = 0;

		bool Enabled() const
		{
			return mEnabled;
		};
		void SetEnabled(bool enabled)
		{
			mEnabled = enabled;
		};
	protected:
		bool mEnabled;

	private:
	};
}

#endif // !BASECOMPONENT_H


