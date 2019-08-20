/**
 * 
 */

#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#pragma once
#include "DrawableComponent.h"
#include "tools/Geometry.h"
#include "tools/Model.h"
#include "tools/Material.h"

namespace Potato
{
	enum ObjectType
	{
		OT_Static = 0,
		OT_MoveAble,
		OT_Other
	};

	class GameObject : public DrawableComponent
	{
	public:
		GameObject();
		~GameObject();

	public:
		// BaseComponent
		void Initialize() override;
		void Update(float dt) override;

		// DrawableComponent
		void DrawObject(ID3D11DeviceContext* deviceContext, EffectBase* effect) override;
	public:
		void DrawInstanced(ID3D11DeviceContext* deviceContext, EffectBase* effect, ID3D11Buffer* instanceBuffer);

		DirectX::XMFLOAT3  GetPosition();

		void SetPosition(float x, float y, float z);
		void SetPosition(DirectX::XMFLOAT3 position);
		void SetRotate(float x, float y, float z);
		void SetRotate(DirectX::XMFLOAT3 rotate);
		void SetScale(float xyz);
		void SetScale(float x, float y, float z);
		void SetScale(DirectX::XMFLOAT3 scale);

		void SetObjectType(ObjectType objType);
		ObjectType GetObjectType();

		void CreateWorldMatrix();
		void SetWorldMatrix(XMFLOAT4X4& world);

		void SetModel(ID3D11Device* device,VertexType type, std::string fileName);
		void SetModel(ID3D11Device* device,VertexType type, const Geometry::MeshData& meshdata);

		void SetMaterial(UINT slot, Material * mat);
		void SetMaterial(const std::vector<Material*>& mat);

	private:
		ObjectType mObjType;

		DirectX::XMFLOAT4X4 mWorldMatrix;

		DirectX::XMFLOAT3 mPosition;	// 相对世界坐标的位置
		DirectX::XMFLOAT3 mRotate;	// x, y, z 旋转量，以弧度计
		DirectX::XMFLOAT3 mScale;	//	x, y, z 缩放量，

		Model* mModel;
		std::vector<Material*> mMaterial;
	};
}

#endif // !GAMEOBJECT_H


#pragma once
