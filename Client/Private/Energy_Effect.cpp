#include "stdafx.h"
#include "..\Public\Energy_Effect.h"

#include "RenderInstance.h"
#include "GameInstance.h"
#include "Collider_Manager.h"
CEnergy_Effect::CEnergy_Effect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject{ pDevice, pContext }
{

}

CEnergy_Effect::CEnergy_Effect(const CEnergy_Effect& Prototype)
	: CGameObject{ Prototype }
{

}

HRESULT CEnergy_Effect::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEnergy_Effect::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_playerID = CEnergy_Effect::PLAYERID::PLAYER_1P;

	return S_OK;
}

void CEnergy_Effect::Priority_Update(_float fTimeDelta)
{

}

void CEnergy_Effect::Update(_float fTimeDelta)
{
	for (auto& iter : m_vecColliderCom)
		iter->Update(m_pTransformCom->Get_WorldMatrix());

	_float speed = 0.1f;

	/*if (GetAsyncKeyState(VK_UP) & 0x8000)
		m_fY += speed;

	if (GetAsyncKeyState(VK_DOWN) & 0x8000)
		m_fY -= speed;

	if (GetAsyncKeyState(VK_LEFT) & 0x8000)
		m_fX -= speed;*/

	m_fX += speed;

	CCollider_Manager::COLLIDERGROUP group;

	switch (m_playerID)
	{
	case CEnergy_Effect::PLAYERID::PLAYER_1P:
		group = CCollider_Manager::CG_1P_Energy_Attack;
		break;
	case CEnergy_Effect::PLAYERID::PLAYER_2P:
		group = CCollider_Manager::CG_2P_Energy_Attack;
		break;
	}

	_float2 DefaultFloat2 = _float2(0.f, 0.f);
	Make_Collider(group, DefaultFloat2, _float2(DefaultFloat2.x + m_fX, DefaultFloat2.y + m_fY));
}

void CEnergy_Effect::Late_Update(_float fTimeDelta)
{
	m_pRenderInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);

	for (auto& iter : m_vecColliderCom)
		m_pRenderInstance->Add_DebugComponent(iter);
}

HRESULT CEnergy_Effect::Render(_float fTimeDelta)
{
	return S_OK;
}

void CEnergy_Effect::OnCollisionEnter(CCollider* other, _float fTimeDelta)
{
	switch (m_playerID)
	{
		//이펙트가 1P_Skill이라면
	case CEnergy_Effect::PLAYERID::PLAYER_1P:
		if (other->m_ColliderGroup == CCollider_Manager::CG_2P_Energy_Attack)
		{
			//기존 콜라이더 삭제
			for (auto& iter : m_vecColliderCom)
				Safe_Release(iter);

			//콜라이더 컴포넌트 전부 삭제
			Clear_Collider_Component();

			//벡터 초기화
			m_vecColliderCom.clear();

			m_fX = 0.f;
			m_fY = 0.f;
		}

		else if (other->m_ColliderGroup == CCollider_Manager::CG_2P_BODY)
		{
			//기존 콜라이더 삭제
			for (auto& iter : m_vecColliderCom)
				Safe_Release(iter);

			//콜라이더 컴포넌트 전부 삭제
			Clear_Collider_Component();

			//벡터 초기화
			m_vecColliderCom.clear();

			m_fX = 0.f;
			m_fY = 0.f;

		}
		break;

		//이펙트가 2P_Skill이라면
	case CEnergy_Effect::PLAYERID::PLAYER_2P:
		if (other->m_ColliderGroup == CCollider_Manager::CG_1P_Energy_Attack)
		{

		}
		else if (other->m_ColliderGroup == CCollider_Manager::CG_1P_BODY)
		{

		}
		break;
	}
}

void CEnergy_Effect::OnCollisionStay(CCollider* other, _float fTimeDelta)
{
}

void CEnergy_Effect::OnCollisionExit(CCollider* other)
{
}

void CEnergy_Effect::Make_Collider(CCollider_Manager::COLLIDERGROUP eColliderGroup, _float2 SourcePos, _float2 DestPos)
{
	// 1. 시작점과 끝점 사이의 거리 및 방향 계산
	_float dx = DestPos.x - SourcePos.x;
	_float dy = DestPos.y - SourcePos.y;

	// 두 점 사이의 거리 계산
	_float distance = sqrtf(dx * dx + dy * dy);

	// 방향 벡터 및 정규화
	_float2 direction = { dx / distance, dy / distance };

	// 2. 필요한 콜라이더의 개수 계산
	_float unitLength = m_UnitSize.x; // 단위 콜라이더의 가로 크기
	int requiredColliders = static_cast<int>(ceil(distance / unitLength));

	// 3. 현재 콜라이더 그룹의 콜라이더 수 확인
	int currentColliders = static_cast<int>(m_vecColliderCom.size());

	// 4. 필요한 경우 콜라이더 추가 생성 및 위치 설정
	for (int i = currentColliders; i < requiredColliders; ++i)
	{
		// 콜라이더의 중점 위치 계산
		_float currentDistance = unitLength * (i + 0.5f);
		_float2 colliderPos = {
			SourcePos.x + direction.x * currentDistance,
			SourcePos.y + direction.y * currentDistance
		};

		// 콜라이더 추가 생성
		CBounding_AABB::BOUNDING_AABB_DESC BoundingDesc{};
		BoundingDesc.vExtents = _float3(m_UnitSize.x / 2.0f, m_UnitSize.y / 2.0f, 0.5f);
		BoundingDesc.vCenter = _float3(colliderPos.x, colliderPos.y, 0.f); // 생성 시 위치 설정
		BoundingDesc.pMineGameObject = this;
		BoundingDesc.colliderGroup = eColliderGroup;

		CCollider* pNewCollider = nullptr;
		_wstring colliderName = L"Com_Collider_" + to_wstring(i);

		if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_AABB"),
			colliderName.c_str(), reinterpret_cast<CComponent**>(&pNewCollider), &BoundingDesc)))
			return; // 에러 처리

		pNewCollider->Update(m_pTransformCom->Get_WorldMatrix());

		// 콜라이더 벡터에 추가
		m_vecColliderCom.push_back(pNewCollider);

		// 콜라이더 매니저에 추가
		m_pGameInstance->Add_ColliderObject(eColliderGroup, pNewCollider);
	}
}


CEnergy_Effect* CEnergy_Effect::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CEnergy_Effect* pInstance = new CEnergy_Effect(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CEnergy_Effect"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEnergy_Effect::Clone(void* pArg)
{
	CEnergy_Effect* pInstance = new CEnergy_Effect(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CEnergy_Effect"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEnergy_Effect::Free()
{
	__super::Free();

	for (auto& iter : m_vecColliderCom)
		Safe_Release(iter);
}
