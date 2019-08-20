#include "core/GameCamera.h"

namespace Potato
{
	const float GameCamera::DefaultFieldOfView = XM_PI / 3;
	const float GameCamera::DefaultNearPlaneDistance = 0.001f;
	const float GameCamera::DefaultFarPlaneDistance = 10000.0f;

	GameCamera::GameCamera(float width, float height)
		: mFieldOfView(DefaultFieldOfView), mWidth(width),mHeigt(height),
		mAspectRatio(width/height), mNearPlaneDistance(DefaultNearPlaneDistance),
		mFarPlaneDistance(DefaultFarPlaneDistance),
		mPosition(), mDirection(), mUp(), mRight(), mViewMatrix(), mPerspectiveProjectionMatrix(),mOrthogonalProjectionMatrix()
	{

		Initialize();
	}

	GameCamera::GameCamera(float fieldOfView, float width, float height, float nearPlaneDistance, float farPlaneDistance)
		: mFieldOfView(fieldOfView), mWidth(width), mHeigt(height),
		mAspectRatio(width / height), mNearPlaneDistance(nearPlaneDistance), mFarPlaneDistance(farPlaneDistance),
		mPosition(), mDirection(), mUp(), mRight(), mViewMatrix(), mPerspectiveProjectionMatrix(), mOrthogonalProjectionMatrix()
	{
		Initialize();
	}

	GameCamera::~GameCamera()
	{
	}

	const XMFLOAT3& GameCamera::Position() const
	{
		return mPosition;
	}

	const XMFLOAT3& GameCamera::Direction() const
	{
		return mDirection;
	}

	const XMFLOAT3& GameCamera::Up() const
	{
		return mUp;
	}

	const XMFLOAT3& GameCamera::Right() const
	{
		return mRight;
	}

	XMVECTOR GameCamera::PositionVector() const
	{
		return XMLoadFloat3(&mPosition);
	}

	XMVECTOR GameCamera::DirectionVector() const
	{
		return XMLoadFloat3(&mDirection);
	}

	XMVECTOR GameCamera::UpVector() const
	{
		return XMLoadFloat3(&mUp);
	}

	XMVECTOR GameCamera::RightVector() const
	{
		return XMLoadFloat3(&mRight);
	}

	float GameCamera::AspectRatio() const
	{
		return mAspectRatio;
	}

	float GameCamera::FieldOfView() const
	{
		return mFieldOfView;
	}

	float GameCamera::NearPlaneDistance() const
	{
		return mNearPlaneDistance;
	}

	float GameCamera::FarPlaneDistance() const
	{
		return mFarPlaneDistance;
	}

	D3D11_VIEWPORT GameCamera::GetViewPort() const
	{
		return mViewPort;
	}

	void GameCamera::SetViewPort(const D3D11_VIEWPORT & viewPort)
	{
		mViewPort = viewPort;
	}

	void GameCamera::SetViewPort(float topLeftX, float topLeftY, float width, float height, float minDepth, float maxDepth)
	{
		mViewPort.TopLeftX = topLeftX;
		mViewPort.TopLeftY = topLeftY;
		mViewPort.Width = width;
		mViewPort.Height = height;
		mViewPort.MinDepth = minDepth;
		mViewPort.MaxDepth = maxDepth;
	}

	XMMATRIX GameCamera::ViewMatrix() const
	{
		return XMLoadFloat4x4(&mViewMatrix);
	}

	XMMATRIX GameCamera::PerspectiveProjectionMatrix() const
	{
		return XMLoadFloat4x4(&mPerspectiveProjectionMatrix);
	}

	XMMATRIX GameCamera::OrthogonalProjectionMatrix() const
	{
		return XMLoadFloat4x4(&mOrthogonalProjectionMatrix);
	}

	XMMATRIX GameCamera::ViewPerspectiveProjectionMatrix() const
	{
		XMMATRIX viewMatrix = XMLoadFloat4x4(&mViewMatrix);
		XMMATRIX projectionMatrix = XMLoadFloat4x4(&mPerspectiveProjectionMatrix);
		return XMMatrixMultiply(viewMatrix, projectionMatrix);
	}

	XMMATRIX GameCamera::ViewOrthogonalProjectionMatrix() const
	{
		XMMATRIX viewMatrix = XMLoadFloat4x4(&mViewMatrix);
		XMMATRIX projectionMatrix = XMLoadFloat4x4(&mOrthogonalProjectionMatrix);
		return XMMatrixMultiply(viewMatrix, projectionMatrix);
	}

	void GameCamera::SetPosition(float x, float y, float z)
	{
		XMVECTOR position = XMVectorSet(x, y, z, 1.0f);
		SetPosition(position);
	}

	void GameCamera::SetPosition(FXMVECTOR position)
	{
		XMStoreFloat3(&mPosition, position);
	}

	void GameCamera::SetPosition(const XMFLOAT3& position)
	{
		mPosition = position;
	}

	void GameCamera::SetDirection(float x, float y, float z)
	{
		XMVECTOR direction = XMVectorSet(x, y, z, 1.0f);
		SetDirection(direction);
	}

	void GameCamera::SetDirection(FXMVECTOR direction)
	{
		XMStoreFloat3(&mDirection, direction);
	}

	void GameCamera::SetDirection(const XMFLOAT3& direction)
	{
		mDirection = direction;
	}

	void GameCamera::Reset()
	{
		mPosition = XMFLOAT3{ 0.0f,0.0f,0.0f };
		mDirection = XMFLOAT3{ 0.0f,0.0f,1.0f };
		mUp = XMFLOAT3{ 0.0f,1.0f,0.0f };
		mRight = XMFLOAT3{ 1.0f,0.0f,0.0f };

		UpdateViewMatrix();
	}

	void GameCamera::Initialize()
	{	
		Reset();
		UpdatePerspectiveProjectionMatrix();
		UpdateOrthogonalProjectionMatrix();
	}

	void GameCamera::Update(float dt)
	{
		UpdateViewMatrix();
	}

	void GameCamera::UpdateViewMatrix()
	{
		XMVECTOR eyePosition = XMLoadFloat3(&mPosition);
		XMVECTOR direction = XMLoadFloat3(&mDirection);
		XMVECTOR upDirection = XMLoadFloat3(&mUp);

		XMMATRIX viewMatrix = XMMatrixLookToLH(eyePosition, direction, upDirection);
		XMStoreFloat4x4(&mViewMatrix, viewMatrix);
	}

	void GameCamera::UpdatePerspectiveProjectionMatrix()
	{
		XMMATRIX projectionMatrix = XMMatrixPerspectiveFovLH(mFieldOfView, mAspectRatio, mNearPlaneDistance, mFarPlaneDistance);
		XMStoreFloat4x4(&mPerspectiveProjectionMatrix, projectionMatrix);
	}

	void  GameCamera::UpdateOrthogonalProjectionMatrix()
	{
		XMMATRIX projectionMatrix = XMMatrixOrthographicLH(mWidth, mHeigt, mNearPlaneDistance, mFarPlaneDistance);
		XMStoreFloat4x4(&mOrthogonalProjectionMatrix, projectionMatrix);
	}

	void GameCamera::ApplyRotation(CXMMATRIX transform)
	{
		XMVECTOR direction = XMLoadFloat3(&mDirection);
		XMVECTOR up = XMLoadFloat3(&mUp);

		direction = XMVector3TransformNormal(direction, transform);
		direction = XMVector3Normalize(direction);

		up = XMVector3TransformNormal(up, transform);
		up = XMVector3Normalize(up);

		XMVECTOR right = XMVector3Cross(direction, up);
		up = XMVector3Cross(right, direction);

		XMStoreFloat3(&mDirection, direction);
		XMStoreFloat3(&mUp, up);
		XMStoreFloat3(&mRight, right);
	}

	void GameCamera::ApplyRotation(const XMFLOAT4X4& transform)
	{
		CXMMATRIX transformMatrix = XMLoadFloat4x4(&transform);
		ApplyRotation(transformMatrix);
	}
}