#ifndef  ANIMATION_HELPER_H
#define ANIMATION_HELPER_H

#pragma once
#include  "tools/Util.h"

namespace Potato {

	using namespace DirectX;
	/// 关键帧定义--瞬间的骨骼变换
	struct Keyframe
	{
		Keyframe();
		~Keyframe();

		float timePos;
		XMFLOAT3 translation;
		XMFLOAT3 scale;
		XMFLOAT4 rotationQuat;
	};

	/// BoneAnimation由关键帧列表定义。 
	/// 对于两个关键帧之间的时间值，
	/// 我们在限制时间的两个最近的关键帧之间进行插值。
	struct  BoneAnimation
	{
		float GetStrartTime() const;
		float GetEndTime() const;

		void Interpolate(XMFLOAT4X4 & M, float t) const;

		std::vector<Keyframe> Keyframes;
	};
}

#endif  // ANIMATION_HELPER_H




