#include "stdafx.h"
#include "..\Public\Range_Effect.h"

#include "RenderInstance.h"
#include "GameInstance.h"
#include "Collider_Manager.h"
CRange_Effect::CRange_Effect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject{ pDevice, pContext }
{

}

CRange_Effect::CRange_Effect(const CRange_Effect& Prototype)
	: CGameObject{ Prototype }
{

}

HRESULT CRange_Effect::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CRange_Effect::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_playerID = CRange_Effect::PLAYERID::PLAYER_1P;

	return S_OK;
}

void CRange_Effect::Priority_Update(_float fTimeDelta)
{

}

void CRange_Effect::Update(_float fTimeDelta)
{
	m_pColliderCom->Update(m_pTransformCom->Get_WorldMatrix());

	_float speed = 0.1f;
	m_pTransformCom->Move_Position(_float3(speed, 0.f, 0.f));
}

void CRange_Effect::Late_Update(_float fTimeDelta)
{
	m_pRenderInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);

	m_pRenderInstance->Add_DebugComponent(m_pColliderCom);
}

HRESULT CRange_Effect::Render(_float fTimeDelta)
{
	return S_OK;
}

void CRange_Effect::OnCollisionEnter(CCollider* other, _float fTimeDelta)
{
	int a = 3;
	Destory();
}

void CRange_Effect::OnCollisionStay(CCollider* other, _float fTimeDelta)
{
	int a = 3;
}

void CRange_Effect::OnCollisionExit(CCollider* other)
{
	int a = 3;

}

HRESULT CRange_Effect::Ready_Components()
{
	CBounding_AABB::BOUNDING_AABB_DESC	BoundingDesc{};

	BoundingDesc.vExtents = _float3(1.5f, 1.5f, 1.5f);
	BoundingDesc.vCenter = _float3(0.f, 0.f, 0.f);
	BoundingDesc.pMineGameObject = this;
	BoundingDesc.colliderGroup = CCollider_Manager::CG_1P_Ranged_Attack;

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_AABB"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &BoundingDesc)))
		return E_FAIL;

	m_pColliderCom->Update(m_pTransformCom->Get_WorldMatrix());

	m_pGameInstance->Add_ColliderObject(BoundingDesc.colliderGroup, m_pColliderCom);

	return S_OK;
}


CRange_Effect* CRange_Effect::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CRange_Effect* pInstance = new CRange_Effect(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CRange_Effect"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CRange_Effect::Clone(void* pArg)
{
	CRange_Effect* pInstance = new CRange_Effect(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CRange_Effect"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CRange_Effect::Free()
{
	Safe_Release(m_pColliderCom);

	__super::Free();

}
