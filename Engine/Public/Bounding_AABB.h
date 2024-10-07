#pragma once

#include "Bounding.h"

/* ���� �浹�� ���� �����͸� ������. */
/* �浹�� ���� �Լ��� ������. */

BEGIN(Engine)

class CBounding_AABB final : public CBounding
{
public:
	typedef struct : public BOUNDING_DESC
	{
		_float3		vExtents;
	}BOUNDING_AABB_DESC;

private:
	CBounding_AABB(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CBounding_AABB() = default;

public:
	BoundingBox Get_Desc() const {
		return *m_pDesc;
	}

	void Set_Desc(BoundingBox _box);

public:
	virtual HRESULT Initialize(CBounding::BOUNDING_DESC* pDesc);
	virtual void Update(_fmatrix TransformMatrix) override;
	virtual HRESULT Render(PrimitiveBatch<VertexPositionColor>* pBatch, _fvector vColor) override;
	virtual _bool isCollision(CCollider::TYPE eTargetColliderType, CBounding* pTargetBounding);
	_bool isRayCollision(const _float3& rayOrigin, const _float3& rayDir) override;
	virtual _bool isPointInAABB(const _float3& point) override;

private:
	BoundingBox*				m_pOriginalDesc = { nullptr };
	BoundingBox*				m_pDesc = { nullptr };

private:
	_bool Collision_AABB(CBounding_AABB* pTargetDesc);

	_float3 Compute_Min();
	_float3 Compute_Max();

public:
	static CBounding_AABB* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CBounding::BOUNDING_DESC* pDesc);
	virtual void Free() override;

	// CBounding��(��) ���� ��ӵ�
};

END