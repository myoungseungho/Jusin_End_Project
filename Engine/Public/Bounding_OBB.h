#pragma once

#include "Bounding.h"

/* ���� �浹�� ���� �����͸� ������. */
/* �浹�� ���� �Լ��� ������. */

BEGIN(Engine)

class CBounding_OBB final : public CBounding
{
public:
	typedef struct : public BOUNDING_DESC
	{
		_float3		vExtents;
		_float3		vRotation;
	}BOUNDING_OBB_DESC;

	typedef struct
	{
		_float3		vCenter;
		_float3		vCenterDir[3];
		_float3		vAlignDir[3];
	}OBB_DESC;
private:
	CBounding_OBB(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CBounding_OBB() = default;

public:
	BoundingOrientedBox Get_Desc() const {
		return *m_pDesc;
	}

public:
	virtual HRESULT Initialize(CBounding::BOUNDING_DESC* pDesc);
	virtual void Update(_fmatrix TransformMatrix) override;
	virtual HRESULT Render(PrimitiveBatch<VertexPositionColor>* pBatch, _fvector vColor) override;
	virtual _bool isCollision(CCollider::TYPE eTargetColliderType, CBounding* pTargetBounding) override;
	_bool isRayCollision(const _float3& rayOrigin, const _float3& rayDir) override;

private:
	BoundingOrientedBox*				m_pOriginalDesc = { nullptr };
	BoundingOrientedBox*				m_pDesc = { nullptr };
private:
	_bool Collision_OBB(CBounding_OBB* pTargetDesc);
	OBB_DESC Compute_OBB();

public:
	static CBounding_OBB* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CBounding::BOUNDING_DESC* pDesc);
	virtual void Free() override;
};

END