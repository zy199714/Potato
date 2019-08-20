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

		// ƽ�ƣ���X����(1.0, 0.0, 0.0)
		void Strafe(float d);
		// ֱ�У���Z����(0.0, 0.0, 1.0)
		void Walk(float d);
		// ǰ��(��ǰ���ƶ�)
		void MoveForward(float d);
		// ���¹۲�
		void Pitch(float rad);
		// ���ҹ۲�
		void Roll(float rad);
	};
}