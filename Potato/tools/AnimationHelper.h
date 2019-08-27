#ifndef  ANIMATION_HELPER_H
#define ANIMATION_HELPER_H

#pragma once
#include  "tools/Util.h"

namespace Potato {

	using namespace DirectX;
	/// �ؼ�֡����--˲��Ĺ����任
	struct Keyframe
	{
		Keyframe();
		~Keyframe();

		float timePos;
		XMFLOAT3 translation;
		XMFLOAT3 scale;
		XMFLOAT4 rotationQuat;
	};

	/// BoneAnimation�ɹؼ�֡�б��塣 
	/// ���������ؼ�֮֡���ʱ��ֵ��
	/// ����������ʱ�����������Ĺؼ�֮֡����в�ֵ��
	struct  BoneAnimation
	{
		float GetStrartTime() const;
		float GetEndTime() const;

		void Interpolate(XMFLOAT4X4 & M, float t) const;

		std::vector<Keyframe> Keyframes;
	};
}

#endif  // ANIMATION_HELPER_H




