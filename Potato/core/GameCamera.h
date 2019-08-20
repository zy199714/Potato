#pragma once

#include "core/BaseComponent.h"

using namespace DirectX;

namespace Potato
{
	class GameCamera : public BaseComponent
	{
	public:
		GameCamera(float width,float height);
		GameCamera(float fieldOfView, float width, float height, float nearPlaneDistance, float farPlaneDistance);

		virtual ~GameCamera();
		const XMFLOAT3& Position() const;
		const XMFLOAT3& Direction() const;
		const XMFLOAT3& Up() const;
		const XMFLOAT3& Right() const;

		XMVECTOR PositionVector() const;
		XMVECTOR DirectionVector() const;
		XMVECTOR UpVector() const;
		XMVECTOR RightVector() const;

		float AspectRatio() const;
		float FieldOfView() const;
		float NearPlaneDistance() const;
		float FarPlaneDistance() const;
		
		D3D11_VIEWPORT GetViewPort() const;
		void SetViewPort(const D3D11_VIEWPORT& viewPort);
		void SetViewPort(float topLeftX, float topLeftY, float width, float height, float minDepth = 0.0f, float maxDepth = 1.0f);

		XMMATRIX ViewMatrix() const;
		XMMATRIX PerspectiveProjectionMatrix() const;
		XMMATRIX OrthogonalProjectionMatrix() const;
		XMMATRIX ViewPerspectiveProjectionMatrix() const;
		XMMATRIX ViewOrthogonalProjectionMatrix() const;

		virtual void SetPosition(float x, float y, float z);
		virtual void SetPosition(FXMVECTOR position);
		virtual void SetPosition(const XMFLOAT3& position);
		virtual void SetDirection(float x, float y, float z);
		virtual void SetDirection(FXMVECTOR direction);
		virtual void SetDirection(const XMFLOAT3& direction);

		virtual void Reset();
		virtual void Initialize() override;
		virtual void Update(float dt) override;
		virtual void UpdateViewMatrix();
		virtual void UpdatePerspectiveProjectionMatrix();
		virtual void UpdateOrthogonalProjectionMatrix();
		virtual void ApplyRotation(CXMMATRIX transform);
		virtual void ApplyRotation(const XMFLOAT4X4& transform);

		static const float DefaultFieldOfView;
		static const float DefaultNearPlaneDistance;
		static const float DefaultFarPlaneDistance;
	protected:
		float mFieldOfView;
		float mWidth;
		float mHeigt;
		float mAspectRatio;
		float mNearPlaneDistance;
		float mFarPlaneDistance;

		XMFLOAT3 mPosition;
		XMFLOAT3 mDirection;
		XMFLOAT3 mUp;
		XMFLOAT3 mRight;

		XMFLOAT4X4 mViewMatrix;
		XMFLOAT4X4 mPerspectiveProjectionMatrix;
		XMFLOAT4X4 mOrthogonalProjectionMatrix;

		D3D11_VIEWPORT mViewPort;
	};
}