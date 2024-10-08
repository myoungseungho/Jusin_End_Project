#include "stdafx.h"
#include "..\Public\Effect.h"

#include "RenderInstance.h"
#include "GameInstance.h"
#include "Imgui_Manager.h"
#include "Collider_Manager.h"
CEffect::CEffect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject{ pDevice, pContext }
{

}

CEffect::CEffect(const CEffect& Prototype)
	: CGameObject{ Prototype }
{

}

HRESULT CEffect::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEffect::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CEffect::Priority_Update(_float fTimeDelta)
{

}

void CEffect::Update(_float fTimeDelta)
{
	m_pColliderCom->Update(m_pTransformCom->Get_WorldMatrix());

	if (GetAsyncKeyState(VK_UP) & 0x8000)
	{
		m_fY -= 0.1;
	}

	if (GetAsyncKeyState(VK_DOWN) & 0x8000)
	{
		m_fY += 0.1;
	}

	if (GetAsyncKeyState(VK_LEFT) & 0x8000)
	{
		m_fX -= 0.1;
	}

	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
	{
		m_fX += 0.1;
	}

	_float2 DefaultFloat2 = _float2(-5.f, 5.f);
	Make_Effect_Collider_EnergyAttack(CCollider_Manager::CG_1P_SKILL, DefaultFloat2, _float2(DefaultFloat2.x + m_fX, DefaultFloat2.y + m_fY));
}

void CEffect::Late_Update(_float fTimeDelta)
{
	m_pRenderInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);

#ifdef _DEBUG
	m_pRenderInstance->Add_DebugComponent(m_pColliderCom);
#endif
}

HRESULT CEffect::Render(_float fTimeDelta)
{
	return S_OK;
}

void CEffect::OnCollisionEnter(CCollider* other, _float fTimeDelta)
{
	int a = 3;
}

void CEffect::OnCollisionStay(CCollider* other, _float fTimeDelta)
{
	int a = 3;
}

void CEffect::OnCollisionExit(CCollider* other)
{
	int a = 3;
}

void CEffect::Make_Effect_Collider_EnergyAttack(CCollider_Manager::COLLIDERGROUP eColliderGroup, _float2 SourcePos, _float2 DestPos)
{
	// 1. 시작점과 끝점 사이의 거리 및 방향 계산
	_float dx = DestPos.x - SourcePos.x;
	_float dy = DestPos.y - SourcePos.y;

	// 두 점 사이의 거리 계산
	_float distance = sqrtf(dx * dx + dy * dy);

	// 에네르기파가 아직 생성되지 않았을 때 처리
	if (distance < m_UnitSize.x)
		return;

	// 방향 벡터 및 정규화
	_float2 direction = { dx / distance, dy / distance };

	// 2. 필요한 콜라이더의 개수 계산
	_float unitLength = m_UnitSize.x; // 단위 콜라이더의 가로 크기 (예: 3.f)
	int requiredColliders = static_cast<int>(ceil(distance / unitLength));

	// 3. 현재 콜라이더 그룹의 콜라이더 수 확인
	int currentColliders = static_cast<int>(m_Colliders[eColliderGroup].size());

	// 4. 필요한 경우 콜라이더 추가 생성
	for (int i = currentColliders; i < requiredColliders; ++i)
	{
		// 새로운 콜라이더 생성
		CCollider* newCollider = new CCollider();

		// 콜라이더 설정 (크기 설정)
		newCollider->SetSize(m_UnitCollider);

		// 콜라이더 그룹에 추가
		m_Colliders[eColliderGroup].push_back(newCollider);
	}

	// 5. 콜라이더 위치 업데이트
	// 각 콜라이더의 위치 설정
	for (int i = 0; i < requiredColliders; ++i)
	{
		// 콜라이더의 중점 위치 계산
		_float currentDistance = unitLength * (i + 0.5f);
		_float2 colliderPos = {
			SourcePos.x + direction.x * currentDistance,
			SourcePos.y + direction.y * currentDistance
		};

		// 콜라이더 가져오기
		CCollider* collider = m_Colliders[eColliderGroup][i];

		// 콜라이더 위치 설정
		collider->SetCenter(colliderPos);

		// 필요에 따라 다른 속성 설정 (예: 회전 등)
	}

	// 나머지 콜라이더들은 그대로 두어도 됩니다.
	// 사용하지 않는 콜라이더들이 있을 수 있지만, 제거하지 않으므로 메모리 관리에 유의해야 합니다.
}

HRESULT CEffect::Ready_Components()
{
	CBounding_AABB::BOUNDING_AABB_DESC	BoundingDesc{};

	BoundingDesc.vExtents = _float3(1.5f, 1.5f, 1.5f);
	BoundingDesc.vCenter = _float3(0.f, 0.6f, 0.f);
	BoundingDesc.pMineGameObject = this;

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_AABB"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &BoundingDesc)))
		return E_FAIL;

	m_pGameInstance->Add_ColliderObject(CCollider_Manager::CG_1P_SKILL, m_pColliderCom);

	return S_OK;
}

HRESULT CEffect::Bind_ShaderResources()
{
	return S_OK;
}

CEffect* CEffect::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CEffect* pInstance = new CEffect(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CEffect"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEffect::Clone(void* pArg)
{
	CEffect* pInstance = new CEffect(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CEffect"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect::Free()
{
	__super::Free();

	Safe_Release(m_pColliderCom);
}
