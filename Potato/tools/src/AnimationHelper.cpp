#include "tools/AnimationHelper.h"

namespace Potato
{
	Keyframe::Keyframe()
		: timePos(0.0f),
		translation(0.0f, 0.0f, 0.0f),
		scale(1.0f, 1.0f, 1.0f),
		rotationQuat(0.0f, 0.0f, 0.0f, 1.0f)
	{
	}

	Keyframe::~Keyframe()
	{
	}

	float BoneAnimation::GetStrartTime() const
	{
		// 关键帧按时间排序，因此第一个关键帧给出开始时间。
		return Keyframes.front().timePos;
	}

	float BoneAnimation::GetEndTime() const
	{
		// 最后一个关键帧给出开始时间。
		return Keyframes.back().timePos;
	}

	void BoneAnimation::Interpolate(XMFLOAT4X4 & M, float t) const
	{
		XMVECTOR zero = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);

		if (t <= Keyframes.front().timePos)
		{
			XMVECTOR S = XMLoadFloat3(&Keyframes.front().scale);
			XMVECTOR P = XMLoadFloat3(&Keyframes.front().translation);
			XMVECTOR Q = XMLoadFloat4(&Keyframes.front().rotationQuat);

			XMStoreFloat4x4(&M, XMMatrixAffineTransformation(S, zero, Q, P));
		}
		else if (t >= Keyframes.back().timePos)
		{
			XMVECTOR s = XMLoadFloat3(&Keyframes.back().scale);
			XMVECTOR q = XMLoadFloat4(&Keyframes.back().rotationQuat);
			XMVECTOR p = XMLoadFloat3(&Keyframes.back().translation);

			XMStoreFloat4x4(&M, XMMatrixAffineTransformation(s, zero, q, p));
		}
		else {
			UINT count = Keyframes.size() - 1;
			for (UINT i = 0; i < count; ++i)
			{
				if (t >= Keyframes[i].timePos && t <= Keyframes[i + 1].timePos)
				{
					float lerpPercent = (t - Keyframes[i].timePos) / (Keyframes[i + 1].timePos - Keyframes[i].timePos);

					XMVECTOR s0 = XMLoadFloat3(&Keyframes[i].scale);
					XMVECTOR s1 = XMLoadFloat3(&Keyframes[i + 1].scale);

					XMVECTOR q0 = XMLoadFloat4(&Keyframes[i].rotationQuat);
					XMVECTOR q1 = XMLoadFloat4(&Keyframes[i + 1].rotationQuat);

					XMVECTOR p0 = XMLoadFloat3(&Keyframes[i].translation);
					XMVECTOR p1 = XMLoadFloat3(&Keyframes[i + 1].translation);

					XMVECTOR s = XMVectorLerp(s0, s1, lerpPercent);
					XMVECTOR q = XMQuaternionSlerp(q0, q1, lerpPercent);
					XMVECTOR p = XMVectorLerp(p0, p1, lerpPercent);

					XMStoreFloat4x4(&M, XMMatrixAffineTransformation(s, zero, q, p));
				}
			}
		}
	}
};





