#include "..\Public\Frustum.h"
#include "GameInstance.h"

CFrustum::CFrustum()
	: m_pGameInstance { CGameInstance::Get_Instance() }
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CFrustum::Initialize()
{
	/* ������������ ����ü�� ������ �𼭸����� �����Ѵ�. */
	m_vPoints[0] = _float3(-1.f, 1.f, 0.f);
	m_vPoints[1] = _float3(1.f, 1.f, 0.f);
	m_vPoints[2] = _float3(1.f, -1.f, 0.f);
	m_vPoints[3] = _float3(-1.f, -1.f, 0.f);

	m_vPoints[4] = _float3(-1.f, 1.f, 1.f);
	m_vPoints[5] = _float3(1.f, 1.f, 1.f);
	m_vPoints[6] = _float3(1.f, -1.f, 1.f);
	m_vPoints[7] = _float3(-1.f, -1.f, 1.f);

	return S_OK;
}

void CFrustum::Update()
{
	_matrix			ViewMatrixInverse = m_pGameInstance->Get_Transform_Inverse_Matrix(CPipeLine::D3DTS_VIEW);
	_matrix			ProjMatrixInverse = m_pGameInstance->Get_Transform_Inverse_Matrix(CPipeLine::D3DTS_PROJ);

	for (size_t i = 0; i < 8; i++)
	{
		XMStoreFloat3(&m_vWorldPoints[i], XMVector3TransformCoord(XMLoadFloat3(&m_vPoints[i]), ProjMatrixInverse));
		XMStoreFloat3(&m_vWorldPoints[i], XMVector3TransformCoord(XMLoadFloat3(&m_vWorldPoints[i]), ViewMatrixInverse));
	}

	Make_Planes(m_vWorldPlanes, m_vWorldPoints);
}

void CFrustum::Get_ParallelVectorsInPlane(_float3& tangent1, _float3& tangent2, _float fov)
{
	// �þ߰��� ���� Y�� Z�� ���� ���
	float tanHalfFovY = tanf(fov / 2.0f);

	// ��� ������: A*x + B*y + C*z + D = 0
	// ���⼭ x=0���� ������ ���¿��� y�� z�� ���

	// Y�� Z�� ������ ������� ���� ���� ���
	// �þ߰��� �������� Y�� Ŀ���� Z�� �۾���
	tangent1 = _float3(0.f, 1.f, -tanHalfFovY);  // ù ��° ���� ����
	tangent2 = _float3(0.f, 0.f, 1.f);           // �� ��° ���� ���� (�ܼ� Z�� ����)

	// ���� ����ȭ
	XMVECTOR vec1 = XMLoadFloat3(&tangent1);
	vec1 = XMVector3Normalize(vec1);
	XMStoreFloat3(&tangent1, vec1);

	XMVECTOR vec2 = XMLoadFloat3(&tangent2);
	vec2 = XMVector3Normalize(vec2);
	XMStoreFloat3(&tangent2, vec2);
}

void CFrustum::Make_Planes(_float4 * pPlanes, const _float3 * pPoints)
{
	/* +x */
	XMStoreFloat4(&pPlanes[0], 
		XMPlaneFromPoints(XMLoadFloat3(&pPoints[1]), XMLoadFloat3(&pPoints[5]), XMLoadFloat3(&pPoints[6])));

	/* -x */
	XMStoreFloat4(&pPlanes[1],
		XMPlaneFromPoints(XMLoadFloat3(&pPoints[4]), XMLoadFloat3(&pPoints[0]), XMLoadFloat3(&pPoints[3])));


	/* +y */
	XMStoreFloat4(&pPlanes[2],
		XMPlaneFromPoints(XMLoadFloat3(&pPoints[4]), XMLoadFloat3(&pPoints[5]), XMLoadFloat3(&pPoints[1])));

	/* -y */
	XMStoreFloat4(&pPlanes[3],
		XMPlaneFromPoints(XMLoadFloat3(&pPoints[3]), XMLoadFloat3(&pPoints[2]), XMLoadFloat3(&pPoints[6])));


	/* +z */
	XMStoreFloat4(&pPlanes[4],
		XMPlaneFromPoints(XMLoadFloat3(&pPoints[5]), XMLoadFloat3(&pPoints[4]), XMLoadFloat3(&pPoints[7])));

	/* -z */
	XMStoreFloat4(&pPlanes[5],
		XMPlaneFromPoints(XMLoadFloat3(&pPoints[0]), XMLoadFloat3(&pPoints[1]), XMLoadFloat3(&pPoints[2])));
}

CFrustum * CFrustum::Create()
{
	CFrustum*		pInstance = new CFrustum();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX(TEXT("Failed to Created : CFrustum"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CFrustum::Free()
{
	__super::Free();

	Safe_Release(m_pGameInstance);
}
