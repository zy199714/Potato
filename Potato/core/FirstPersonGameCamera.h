#pragma once
#include "core/GameCamera.h"

namespace Potato
{
	class FirstPersonGameCamera : public GameCamera
	{
	public:
		FirstPersonGameCamera(float width, float height);
		FirstPersonGameCamera(float fieldOfView, float width, float height, float nearPlaneDistance, float farPlaneDistance);

		virtual ~FirstPersonGameCamera();

		// 平移，±X方向(1.0, 0.0, 0.0)
		void Strafe(float d);
		// 直行，±Z方向(0.0, 0.0, 1.0)
		void Walk(float d);
		// 前进(朝前向移动)
		void MoveForward(float d);
		// 上下观察
		void Pitch(float rad);
		// 左右观察
		void Roll(float rad);
	};
}