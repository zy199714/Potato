#include "core/GameObject.h"

namespace Potato
{
	GameObject::GameObject()
		: mObjType(OT_Static),
		mPosition(0.0f, 0.0f, 0.0),
		mRotate(0.0f, 0.0f, 0.0),
		mScale(1.0f, 1.0f, 1.0)
	{
	}

	GameObject::~GameObject()
	{
		DeleteObject(mModel);
		DeleteObjectsVector(mMaterial);
	}

	DirectX::XMFLOAT3 GameObject::GetPosition()
	{
		return mPosition;
	}

	void GameObject::SetPosition(float x, float y, float z)
	{
		mPosition = DirectX::XMFLOAT3(x, y, z);
	}

	void GameObject::SetPosition(DirectX::XMFLOAT3 position)
	{
		mPosition = position;
	}

	void GameObject::SetRotate(float x, float y, float z)
	{
		mRotate = DirectX::XMFLOAT3(x, y, z);
	}

	void GameObject::SetRotate(DirectX::XMFLOAT3 rotate)
	{
		mRotate = rotate;
	}

	void GameObject::SetScale(float xyz)
	{
		mRotate = DirectX::XMFLOAT3(xyz, xyz, xyz);
	}

	void GameObject::SetScale(float x, float y, float z)
	{
		mScale = DirectX::XMFLOAT3(x, y, z);
	}

	void GameObject::SetScale(DirectX::XMFLOAT3 scale)
	{
		mScale = scale;
	}

	void GameObject::SetObjectType(ObjectType objType)
	{
		mObjType = objType;
	}



	ObjectType GameObject::GetObjectType()
	{
		return mObjType;
	}

	void GameObject::CreateWorldMatrix()
	{
		XMVECTOR position = XMLoadFloat3(&mPosition);
		XMVECTOR rotate = XMLoadFloat3(&mRotate);
		XMVECTOR scaling = XMLoadFloat3(&mScale);
		XMMATRIX world = XMMatrixTranslationFromVector(position) *
			XMMatrixRotationRollPitchYawFromVector(rotate) * XMMatrixScalingFromVector(scaling);
		XMStoreFloat4x4(&mWorldMatrix, world);
	}

	void GameObject::SetWorldMatrix(XMFLOAT4X4& world)
	{
		mWorldMatrix = world;
	}


	void GameObject::SetModel(ID3D11Device* device, VertexType type, std::string fileName)
	{
		if (mModel == nullptr)
		{
			mModel = new Model(device,type, fileName);
			// 初始化 材质槽位
			mMaterial.reserve(mModel->GetMeshCount());
		}
	}

	void GameObject::SetModel(ID3D11Device* device, VertexType type, const Geometry::MeshData& meshdata)
	{
		if (mModel ==nullptr)
		{
			mModel = new Model(device,type, meshdata);
			// 初始化 材质槽位
			mMaterial.reserve(mModel->GetMeshCount());
		}
	}

	void GameObject::SetMaterial(UINT slot, Material * mat)
	{
		mMaterial[slot] = mat;
	}

	void GameObject::SetMaterial(const std::vector<Material*>& mat)
	{
		mMaterial = mat;
	}

	void GameObject::DrawObject(ID3D11DeviceContext* deviceContext, EffectBase* effect)
	{
		auto & meshs = mModel->GetMeshs();
		VertexType type = mModel->GetVertexType();

		effect->SetWorldMatrix(XMLoadFloat4x4(&mWorldMatrix));

		for (auto mesh : meshs)
		{
			auto srvbuffer = mModel->GetModelMaterial(mesh->GetMaterialIndex())->GetSRVBuffer(TextureType_DIFFUSE);

			deviceContext->IASetVertexBuffers(0, VertexBufferCount[type], mesh->GetVertexBuffer(), VertexStrides[type], VertexOffsets[type]);
			deviceContext->IASetIndexBuffer(mesh->GetIndicesBuffer(), DXGI_FORMAT_R32_UINT, 0);
			effect->Apply(deviceContext);

			if (srvbuffer != nullptr)
			{
				effect->SetTexture(deviceContext, 0, srvbuffer->size(), *srvbuffer->data());
				effect->SetTextureUsed(true);
			}

			deviceContext->DrawIndexed(mesh->GetIndexCount(), 0, 0);
		}
	}

	void GameObject::DrawInstanced(ID3D11DeviceContext* deviceContext, EffectBase* effect, ID3D11Buffer* instanceBuffer)
	{

	}

	void GameObject::Initialize()
	{

	}

	void GameObject::Update(float dt)
	{

	}
}
