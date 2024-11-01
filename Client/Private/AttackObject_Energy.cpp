#include "stdafx.h"
#include "..\Public\AttackObject_Energy.h"

#include "RenderInstance.h"
#include "GameInstance.h"

#include "Character.h"

CAttackObject_Energy::CAttackObject_Energy(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CAttackObject{ pDevice, pContext }
{

}

CAttackObject_Energy::CAttackObject_Energy(const CAttackObject_Energy& Prototype)
	: CAttackObject{ Prototype }
{

}

HRESULT CAttackObject_Energy::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CAttackObject_Energy::Initialize(void* pArg)
{

	if (nullptr == pArg)
		return E_FAIL;


	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;


	ATTACK_RANGED_DESC* pDesc = static_cast<ATTACK_RANGED_DESC*>(pArg);

	m_fStartOffset = pDesc->fStartOffset;
	//m_fRanged_Impus_NoneDirection = pDesc->fRanged_Impus_NoneDirection;
	//m_iDirection = pDesc->iDirection;
	m_fMoveSpeedNoneDirection = pDesc->fMoveSpeedNoneDirection;
	m_iAttackCount = pDesc->iAttackCount;
	m_iPlayerDirection = pDesc->iPlayerDirection;

	m_bDying = false;


	m_eEnegrgyColor = pDesc->eExplosionColor;

	_vector vPos = m_pOwner->Get_vPosition();
	_vector vStartOffset = { m_fStartOffset.x, m_fStartOffset.y, 0.f, 0.f };

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos + vStartOffset);

	return S_OK;
}



void CAttackObject_Energy::Update(_float fTimeDelta)
{
	if (Check_UpdateStop(fTimeDelta))
		return;

	m_fAccLifeTime += fTimeDelta;

	//생존시간 지났거나 맵바깥(땅포함)으로 나갔으면 삭제
	if (m_fAccLifeTime > m_fLifeTime)
	{
		if (m_bEnableDestory)
		{
			Destory();
			m_pGameInstance->Release_Collider(m_pColliderCom);
			m_bEnableDestory = false;
		}
	}
	else
	{
		for (auto& iter : m_vecColliderCom)
			iter->Update(m_pTransformCom->Get_WorldMatrix());


		for (auto& iter : m_vecColliderCom)
			iter->UpdateVector(m_pTransformCom->Get_State(CTransform::STATE_POSITION));


		_float speed = 0.1f;

		m_fEndPos.x += m_fMoveSpeedNoneDirection.x * fTimeDelta;

		Make_Collider(m_pColliderCom->m_ColliderGroup, _float2(0.f, 0.f), _float2(m_fEndPos.x, m_fEndPos.y));
	}
}

void CAttackObject_Energy::Late_Update(_float fTimeDelta)
{
	m_pRenderInstance->Add_RenderObject(CRenderer::RG_UI, this);
}

HRESULT CAttackObject_Energy::Render(_float fTimeDelta)
{
#ifdef _DEBUG
	//m_pColliderCom->Render(fTimeDelta);

	for (auto pCollider : m_vecColliderCom)
	{
		pCollider->Render(fTimeDelta);
	}
#endif // DEBUG

	return S_OK;
}



void CAttackObject_Energy::OnCollisionEnter(CCollider* other, _float fTimeDelta)
{
	//Destory();
	_bool Debug = true;
}

void CAttackObject_Energy::OnCollisionStay(CCollider* other, _float fTimeDelta)
{
	_bool Debug = true;

}

void CAttackObject_Energy::OnCollisionExit(CCollider* other)
{
	_bool Debug = true;

}

void CAttackObject_Energy::CollisingAttack()
{
}

void CAttackObject_Energy::CollisingPlayer()
{
}

_bool CAttackObject_Energy::Check_MapOut()
{
	//높이 체크
	if (XMVectorGetY(m_pTransformCom->Get_State(CTransform::STATE_POSITION)) < 0.1)
	{
		//지면 폭발 이펙트
		return true;
	}

	//좌우 바깥체크   는 어떻게?
	else if (false)
	{

	}
	else
		return false;

}



void CAttackObject_Energy::Make_Collider(CCollider_Manager::COLLIDERGROUP eColliderGroup, _float2 SourcePos, _float2 DestPos)
{

	// 1. 시작점과 끝점 사이의 거리 및 방향 계산
	_float dx = DestPos.x - SourcePos.x;
	_float dy = DestPos.y - SourcePos.y;

	// 두 점 사이의 거리 계산
	_float distance = sqrtf(dx * dx + dy * dy);

	// 방향 벡터 및 정규화
	_float2 direction = { dx / distance, dy / distance };

	// 2. 필요한 콜라이더의 개수 계산
	//_float unitLength = m_UnitSize.x; // 단위 콜라이더의 가로 크기
	_float unitLength = 0.4f; // 단위 콜라이더의 가로 크기
	_float unitHeight = 0.8f; // 단위 콜라이더의 가로 크기


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
		//BoundingDesc.vExtents = _float3(m_UnitSize.x / 2.0f, m_UnitSize.y / 2.0f, 0.5f);
		BoundingDesc.vExtents = _float3(unitLength / 2.0f, unitHeight / 2.0f, 0.5f);
		//BoundingDesc.vExtents = _float3(0.5f, 0.5f, 0.5f);


		BoundingDesc.vCenter = _float3(colliderPos.x, colliderPos.y, 0.f); // 생성 시 위치 설정
		BoundingDesc.pMineGameObject = this;
		BoundingDesc.colliderGroup = eColliderGroup;

		CCollider* pNewCollider = nullptr;
		_wstring colliderName = L"Com_Collider_" + to_wstring(i);


		if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_AABB"),
			colliderName.c_str(), reinterpret_cast<CComponent**>(&pNewCollider), &BoundingDesc)))
			return; // 에러 처리

		/*

		CBounding_AABB::BOUNDING_AABB_DESC ColliderDesc{};
		ColliderDesc = pDesc->ColliderDesc;
		ColliderDesc.pMineGameObject = this;

		if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_AABB"),
			TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &ColliderDesc)))
			return E_FAIL;


		//m_pColliderCom->Update(m_pOwnerTransform->Get_WorldMatrix());
		m_pColliderCom->UpdateVector(m_pOwnerTransform->Get_State(CTransform::STATE_POSITION));

		m_pGameInstance->Add_ColliderObject(ColliderDesc.colliderGroup, m_pColliderCom);

		*/

		pNewCollider->Update(m_pTransformCom->Get_WorldMatrix());

		// 콜라이더 벡터에 추가
		m_vecColliderCom.push_back(pNewCollider);

		// 콜라이더 매니저에 추가
		m_pGameInstance->Add_ColliderObject(eColliderGroup, pNewCollider);
	}
}


/*

void CAttackObject_Energy::Add_YellowLight()
{
	LIGHT_DESC			LightDesc{};

	ZeroMemory(&LightDesc, sizeof(LIGHT_DESC));
	LightDesc.eType = LIGHT_DESC::TYPE_POINT;
	LightDesc.vPosition = _float4(0.f, 0.f, 0.f, 1.f);
	LightDesc.fRange = 30.f;
	LightDesc.vDiffuse = _float4(1.2f, 1.15f, 0.7f, 1.0f);
	//LightDesc.vDiffuse = _float4(1.0f, 0.f, 0.f, 1.f);
	LightDesc.vAmbient = _float4(0.1f, 0.1f, 0.1f, 1.f);
	LightDesc.vSpecular = _float4(1.0f, 0.95f, 0.45f, 1.f);

	LightDesc.fAccTime = 0.f;
	LightDesc.fLifeTime = 0.5f;
	LightDesc.strName = "Explosion";
	if (FAILED(m_pRenderInstance->Add_Effect_Light(LightDesc.strName, LightDesc)))
		return;
}

void CAttackObject_Energy::Add_YellowLight(_float3 fPosition)
{
	LIGHT_DESC			LightDesc{};

	ZeroMemory(&LightDesc, sizeof(LIGHT_DESC));
	LightDesc.eType = LIGHT_DESC::TYPE_POINT;
	LightDesc.vPosition = _float4(fPosition.x, fPosition.y, 0.f, 1.f);
	LightDesc.fRange = 3.f;
	LightDesc.vDiffuse = _float4(1.2f, 1.15f, 0.7f, 1.0f);
	//LightDesc.vDiffuse = _float4(1.0f, 0.f, 0.f, 1.f);
	LightDesc.vAmbient = _float4(0.1f, 0.1f, 0.1f, 1.f);
	LightDesc.vSpecular = _float4(1.0f, 0.95f, 0.45f, 1.f);

	LightDesc.fAccTime = 0.f;
	LightDesc.fLifeTime = 0.5f;
	LightDesc.strName = "Explosion";
	if (FAILED(m_pRenderInstance->Add_Effect_Light(LightDesc.strName, LightDesc)))
		return;
}
*/


void CAttackObject_Energy::Erase()
{
	if (m_bEnableDestory)
	{
		Destory();
		m_pGameInstance->Release_Collider(m_pColliderCom);
		m_bEnableDestory = false;
	}
}







CAttackObject_Energy* CAttackObject_Energy::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CAttackObject_Energy* pInstance = new CAttackObject_Energy(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CAttackObject_Energy"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CAttackObject_Energy::Clone(void* pArg)
{
	CAttackObject_Energy* pInstance = new CAttackObject_Energy(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CAttackObject_Energy"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CAttackObject_Energy::Free()
{
	__super::Free();

	Safe_Release(m_pColliderCom);

	for (auto& iter : m_vecColliderCom)
		Safe_Release(iter);

	if (m_pOwner == nullptr)
		return;

	if (m_pOwner->Get_iPlayerTeam() == 1)
		m_pGameInstance->Destroy_Reserve(CCollider_Manager::CG_1P_Energy_Attack);
	else
		m_pGameInstance->Destroy_Reserve(CCollider_Manager::CG_2P_Energy_Attack);
}
