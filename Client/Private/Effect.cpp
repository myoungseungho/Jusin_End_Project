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

	return S_OK;
}

void CEffect::Priority_Update(_float fTimeDelta)
{

}

void CEffect::Update(_float fTimeDelta)
{
	for (auto& iter : m_vecColliderCom)
		iter->Update(m_pTransformCom->Get_WorldMatrix());

	if (GetAsyncKeyState(VK_UP) & 0x8000)
		m_fY -= 0.1;

	if (GetAsyncKeyState(VK_DOWN) & 0x8000)
		m_fY += 0.1;

	if (GetAsyncKeyState(VK_LEFT) & 0x8000)
		m_fX -= 0.1;

	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
		m_fX += 0.1;

	//
	_float2 DefaultFloat2 = _float2(-5.f, 5.f);
	Make_Collider(CCollider_Manager::CG_1P_SKILL, DefaultFloat2, _float2(DefaultFloat2.x + m_fX, DefaultFloat2.y + m_fY));
}

void CEffect::Late_Update(_float fTimeDelta)
{
	m_pRenderInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);

	for (auto& iter : m_vecColliderCom)
		m_pRenderInstance->Add_DebugComponent(iter);
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

void CEffect::Make_Collider(CCollider_Manager::COLLIDERGROUP eColliderGroup, _float2 SourcePos, _float2 DestPos)
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
	_float unitLength = m_UnitSize.x; // 단위 콜라이더의 가로 크기 (예:  3.f)
	int requiredColliders = static_cast<int>(ceil(distance / unitLength));

	// 3. 현재 콜라이더 그룹의 콜라이더 수 확인
	int currentColliders = static_cast<int>(m_vecColliderCom.size());

	// 4. 필요한 경우 콜라이더 추가 생성
	for (int i = currentColliders; i < requiredColliders; ++i)
	{
		// 콜라이더 추가 생성
		CBounding_AABB::BOUNDING_AABB_DESC BoundingDesc{};

		BoundingDesc.vExtents = _float3(m_UnitSize.x / 2.0f, m_UnitSize.y / 2.0f, 0.5f); // Extents는 반지름 값이므로 /2.0f
		BoundingDesc.vCenter = _float3(SourcePos.x, SourcePos.y, 0.f); // 초기 위치를 SourcePos로 설정
		BoundingDesc.pMineGameObject = this;

		CCollider* pNewCollider = nullptr;

		// 고유한 컴포넌트 이름 생성 (예: Com_Collider_0, Com_Collider_1, ...)
		_wstring colliderName = L"Com_Collider_" + to_wstring(i);

		if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_AABB"),
			colliderName.c_str(), reinterpret_cast<CComponent**>(&pNewCollider), &BoundingDesc)))
			return; // 에러 처리

		// 콜라이더 벡터에 추가
		m_vecColliderCom.push_back(pNewCollider);
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
		CCollider* collider = m_vecColliderCom[i];

		// 현재 콜라이더의 원래 위치와 크기를 가져오기
		BoundingBox boundingBox = collider->AABB_GetDesc();
		boundingBox.Center = _float3(colliderPos.x, colliderPos.y, 0.f);
		collider->AABB_SetDesc(boundingBox);

		// 콜라이더 매니저에 추가
		m_pGameInstance->Add_ColliderObject(eColliderGroup, collider);
	}
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

	for (auto& iter : m_vecColliderCom)
		Safe_Release(iter);


}
