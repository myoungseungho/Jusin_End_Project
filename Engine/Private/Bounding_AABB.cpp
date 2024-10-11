#include "..\Public\Bounding_AABB.h"
#include "DebugDraw.h"

#include "Bounding_OBB.h"
#include "Bounding_Sphere.h"

CBounding_AABB::CBounding_AABB(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CBounding { pDevice, pContext }
{

}

void CBounding_AABB::Set_Desc(BoundingBox _box)
{
	m_pOriginalDesc->Center = _box.Center;
	m_pOriginalDesc->Extents = _box.Extents;
}

HRESULT CBounding_AABB::Initialize(CBounding::BOUNDING_DESC * pDesc)
{
	BOUNDING_AABB_DESC*	pBoundingDesc = static_cast<BOUNDING_AABB_DESC*>(pDesc);

	m_pOriginalDesc = new BoundingBox(pBoundingDesc->vCenter, pBoundingDesc->vExtents);
	m_pDesc = new BoundingBox(*m_pOriginalDesc);

	return S_OK;
}

void CBounding_AABB::Update(_fmatrix TransformMatrix)
{
	_vector		vScale, vRotation, vTranslation;

	XMMatrixDecompose(&vScale, &vRotation, &vTranslation, TransformMatrix);

	_matrix		Transform = XMMatrixAffineTransformation(vScale, XMVectorSet(0.f, 0.f, 0.f, 1.f), XMVectorSet(0.f, 0.f, 0.f, 0.f), vTranslation);

	m_pOriginalDesc->Transform(*m_pDesc, Transform);
}

HRESULT CBounding_AABB::Render(PrimitiveBatch<VertexPositionColor>* pBatch, _fvector vColor)
{
	DX::Draw(pBatch, *m_pDesc, vColor);

	return S_OK;
}

_bool CBounding_AABB::isCollision(CCollider::TYPE eTargetColliderType, CBounding * pTargetBounding)
{
	_bool		isColl = { false };

	switch (eTargetColliderType)
	{
	case CCollider::TYPE_AABB:
		isColl = Collision_AABB(static_cast<CBounding_AABB*>(pTargetBounding));
		break;
	case CCollider::TYPE_OBB:
		isColl = m_pDesc->Intersects(static_cast<CBounding_OBB*>(pTargetBounding)->Get_Desc());
		break;
	case CCollider::TYPE_SPHERE:
		isColl = m_pDesc->Intersects(static_cast<CBounding_Sphere*>(pTargetBounding)->Get_Desc());
		break;
	}

	return isColl;
}


_bool CBounding_AABB::isRayCollision(const _float3& rayOrigin, const _float3& rayDir)
{
    // ������ �������� ���� ���͸� �ε��մϴ�.
    XMVECTOR rayOriginVec = XMLoadFloat3(&rayOrigin);
    XMVECTOR rayDirVec = XMLoadFloat3(&rayDir);

    // AABB�� �߽����� Ȯ���� �ε��մϴ�.
    XMVECTOR boxCenter = XMLoadFloat3(&m_pDesc->Center);
    XMVECTOR boxExtents = XMLoadFloat3(&m_pDesc->Extents);

    // AABB�� �ּ� ���� �ִ� ��踦 ����մϴ�.
    XMVECTOR boxMin = boxCenter - boxExtents;
    XMVECTOR boxMax = boxCenter + boxExtents;

    // tMin�� tMax�� �ʱ�ȭ�մϴ�.
    float tMin = 0.0f;
    float tMax = FLT_MAX;

    // �� ��(x, y, z)�� ���� ���� �Ÿ��� ����մϴ�.
    for (int i = 0; i < 3; ++i)
    {
       _float rayOriginComponent, rayDirComponent, boxMinComponent, boxMaxComponent;

        // �� �࿡ ���� ������ �����ɴϴ�.
        if (i == 0) {
            rayOriginComponent = XMVectorGetX(rayOriginVec);
            rayDirComponent = XMVectorGetX(rayDirVec);
            boxMinComponent = XMVectorGetX(boxMin);
            boxMaxComponent = XMVectorGetX(boxMax);
        }
        else if (i == 1) {
            rayOriginComponent = XMVectorGetY(rayOriginVec);
            rayDirComponent = XMVectorGetY(rayDirVec);
            boxMinComponent = XMVectorGetY(boxMin);
            boxMaxComponent = XMVectorGetY(boxMax);
        }
        else {
            rayOriginComponent = XMVectorGetZ(rayOriginVec);
            rayDirComponent = XMVectorGetZ(rayDirVec);
            boxMinComponent = XMVectorGetZ(boxMin);
            boxMaxComponent = XMVectorGetZ(boxMax);
        }

        // ���̰� �࿡ ������ ��츦 ó���մϴ�.
        //FLT_EPSILON�� float�� ������ ���� ���� ���� ������, 1.0�� ���е� �� �ִ� �� �� ���� ���� ���Դϴ�.�̴� ��ǻ�Ͱ� �ε��Ҽ��� ������ �� �� ��Ÿ���� ������ �ٷ� �� ���˴ϴ�.
        //����ü �׸� �׷����� �� ����.
        if (fabs(rayDirComponent) < FLT_EPSILON)
        {
            // ���̰� �࿡ �����ϰ�, �������� �ڽ� ��� �ۿ� �ִ� ��� �������� �ʽ��ϴ�.
            if (rayOriginComponent < boxMinComponent || rayOriginComponent > boxMaxComponent)
            {
                return false;
            }
        }
        else
        {
            // ���� �Ÿ��� ����մϴ�.
            float t1 = (boxMinComponent - rayOriginComponent) / rayDirComponent;
            float t2 = (boxMaxComponent - rayOriginComponent) / rayDirComponent;

            // t1�� t2���� ū ���, �� ���� ��ȯ�մϴ�.
            //t1�� t2�� ���� ��ȯ�ϴ� ������ ������ ���⿡ ���� t1�� AABB�� ���� ������, t2�� AABB���� ������ ������ ��Ÿ���� �����Դϴ�.
            // ������ ���⿡ ���� ���� ������ ������ �ٸ� �� �ֱ� ������, �� ���� �ùٸ��� �����Ͽ� �ϰ��� �ǹ̸� ������ �ؾ� �մϴ�.
            if (t1 > t2)
            {
                swap(t1, t2);
            }

            // tMin�� tMax�� ������Ʈ�մϴ�.
            tMin = max(tMin, t1);
            tMax = min(tMax, t2);

            // tMin�� tMax���� ū ���, �������� �ʽ��ϴ�.
            if (tMin > tMax)
            {
                return false;
            }
        }
    }

    // �����մϴ�.
    return true;
}

_bool CBounding_AABB::isPointInAABB(const _float3& point)
{
    // ����Ʈ�� AABB�� �߽����� Ȯ���� �ε��մϴ�.
    XMVECTOR pointVec = XMLoadFloat3(&point);
    XMVECTOR boxCenter = XMLoadFloat3(&m_pDesc->Center);
    XMVECTOR boxExtents = XMLoadFloat3(&m_pDesc->Extents);

    // AABB�� �ּ� ���� �ִ� ��踦 ����մϴ�.
    XMVECTOR boxMin = boxCenter - boxExtents;
    XMVECTOR boxMax = boxCenter + boxExtents;

    // �� ��(x, y, z)�� ���� ����Ʈ�� AABB ���� ���� �ִ��� �˻��մϴ�.
    for (int i = 0; i < 3; ++i)
    {
        _float pointComponent, boxMinComponent, boxMaxComponent;

        // �� �࿡ ���� ������ �����ɴϴ�.
        if (i == 0) {
            pointComponent = XMVectorGetX(pointVec);
            boxMinComponent = XMVectorGetX(boxMin);
            boxMaxComponent = XMVectorGetX(boxMax);
        }
        else if (i == 1) {
            pointComponent = XMVectorGetY(pointVec);
            boxMinComponent = XMVectorGetY(boxMin);
            boxMaxComponent = XMVectorGetY(boxMax);
        }
        else {
            pointComponent = XMVectorGetZ(pointVec);
            boxMinComponent = XMVectorGetZ(boxMin);
            boxMaxComponent = XMVectorGetZ(boxMax);
        }

        // ����Ʈ�� �ش� �࿡�� AABB ���� �ۿ� �ִٸ�, AABB �ȿ� ���� ����
        if (pointComponent < boxMinComponent || pointComponent > boxMaxComponent)
        {
            return false;
        }
    }

    // ��� �࿡ ���� ���� ���� �����Ƿ�, ����Ʈ�� AABB ���ο� ����
    return true;
}


_bool CBounding_AABB::Collision_AABB(CBounding_AABB * pTargetDesc)
{
	_float3		vSourMin = Compute_Min();
	_float3		vSourMax = Compute_Max();

	_float3		vDestMin = pTargetDesc->Compute_Min();
	_float3		vDestMax = pTargetDesc->Compute_Max();

	/* �ʺ�� �浹���߳�? */ 
	if (max(vSourMin.x, vDestMin.x) > min(vSourMax.x, vDestMax.x))
		return false;
	/* ���̷� �浹���߳�? */
	if (max(vSourMin.y, vDestMin.y) > min(vSourMax.y, vDestMax.y))
		return false;

	return true;
}

_float3 CBounding_AABB::Compute_Min()
{
	return _float3(m_pDesc->Center.x - m_pDesc->Extents.x, m_pDesc->Center.y - m_pDesc->Extents.y, m_pDesc->Center.z - m_pDesc->Extents.z);
}

_float3 CBounding_AABB::Compute_Max()
{
	return _float3(m_pDesc->Center.x + m_pDesc->Extents.x, m_pDesc->Center.y + m_pDesc->Extents.y, m_pDesc->Center.z + m_pDesc->Extents.z);
}

CBounding_AABB * CBounding_AABB::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, CBounding::BOUNDING_DESC * pDesc)
{
	CBounding_AABB*		pInstance = new CBounding_AABB(pDevice, pContext);

	if (FAILED(pInstance->Initialize(pDesc)))
	{
		MSG_BOX(TEXT("Failed to Created : CBounding_AABB"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CBounding_AABB::Free()
{
	__super::Free();

	Safe_Delete(m_pOriginalDesc);
	Safe_Delete(m_pDesc);
}

