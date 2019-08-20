#include "core/FirstPersonGameCamera.h"

namespace Potato
{
	FirstPersonGameCamera::FirstPersonGameCamera(float width, float height)
		:GameCamera(width, height)
	{

	}

	FirstPersonGameCamera::FirstPersonGameCamera(float fieldOfView, float width, float height, float nearPlaneDistance, float farPlaneDistance)
		: GameCamera(fieldOfView, width, height, nearPlaneDistance, farPlaneDistance)
	{

	}

	FirstPersonGameCamera::~FirstPersonGameCamera()
	{

	}

	void FirstPersonGameCamera::Strafe(float d)
	{
		XMVECTOR pos = XMLoadFloat3(&mPosition);
		XMVECTOR right = XMLoadFloat3(&mRight);
		XMVECTOR dist = XMVectorReplicate(d);
		// DestPos = Dist * Right + SrcPos
		XMStoreFloat3(&mPosition, XMVectorMultiplyAdd(dist, right, pos));
	}

	void FirstPersonGameCamera::Walk(float d)
	{
		XMVECTOR pos = XMLoadFloat3(&mPosition);
		XMVECTOR front = { 0.0f, 0.0f,1.0f};
		XMVECTOR dist = XMVectorReplicate(d);
		// DestPos = Dist * Front + SrcPos
		XMStoreFloat3(&mPosition, XMVectorMultiplyAdd(dist, front, pos));
	}

	void FirstPersonGameCamera::MoveForward(float d)
	{
		XMVECTOR pos = XMLoadFloat3(&mPosition);
		XMVECTOR look = XMLoadFloat3(&mDirection);
		XMVECTOR dist = XMVectorReplicate(d);
		// DestPos = Dist * Front + SrcPos
		XMStoreFloat3(&mPosition, XMVectorMultiplyAdd(dist, look, pos));
	}

	void FirstPersonGameCamera::Pitch(float rad)
	{
		XMMATRIX r = XMMatrixRotationAxis(XMLoadFloat3(&mRight), rad);
		XMVECTOR up = XMVector3TransformNormal(XMLoadFloat3(&mUp), r);
		XMVECTOR direction = XMVector3TransformNormal(XMLoadFloat3(&mDirection), r);
		float pitch = XMVectorGetY(direction);
		// 将上下视野角度Phi限制在[pi/9, 8pi/9]，
		if (fabs(pitch) > cosf(XM_PI / 9))
			return;
		XMStoreFloat3(&mUp, up);
		XMStoreFloat3(&mDirection, direction);
	}

	void FirstPersonGameCamera::Roll(float rad)
	{
		XMMATRIX r = XMMatrixRotationY(rad);
		XMStoreFloat3(&mRight, XMVector3TransformNormal(XMLoadFloat3(&mRight), r));
		XMStoreFloat3(&mUp, XMVector3TransformNormal(XMLoadFloat3(&mUp), r));
		XMStoreFloat3(&mDirection, XMVector3TransformNormal(XMLoadFloat3(&mDirection), r));
	}
}