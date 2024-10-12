#include "stdafx.h"
#include "..\Public\Melee_Effect.h"

#include "RenderInstance.h"
#include "GameInstance.h"
#include "Collider_Manager.h"
CMelee_Effect::CMelee_Effect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject{ pDevice, pContext }
{

}

CMelee_Effect::CMelee_Effect(const CMelee_Effect& Prototype)
	: CGameObject{ Prototype }
{

}

HRESULT CMelee_Effect::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMelee_Effect::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_playerID = CMelee_Effect::PLAYERID::PLAYER_1P;

	return S_OK;
}

void CMelee_Effect::Priority_Update(_float fTimeDelta)
{

}

void CMelee_Effect::Update(_float fTimeDelta)
{
	m_pColliderCom->Update(m_pTransformCom->Get_WorldMatrix());
}

void CMelee_Effect::Late_Update(_float fTimeDelta)
{
	m_pRenderInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);

	m_pRenderInstance->Add_DebugComponent(m_pColliderCom);
}

HRESULT CMelee_Effect::Render(_float fTimeDelta)
{
	return S_OK;
}

void CMelee_Effect::OnCollisionEnter(CCollider* other, _float fTimeDelta)
{
	int a = 3;
	Destory();
}

void CMelee_Effect::OnCollisionStay(CCollider* other, _float fTimeDelta)
{
	int a = 3;
}

void CMelee_Effect::OnCollisionExit(CCollider* other)
{
	int a = 3;

}

HRESULT CMelee_Effect::Ready_Components()
{
	CBounding_AABB::BOUNDING_AABB_DESC	BoundingDesc{};

	BoundingDesc.vExtents = _float3(1.5f, 1.5f, 1.5f);
	BoundingDesc.vCenter = _float3(0.f, 0.f, 0.f);
	BoundingDesc.pMineGameObject = this;
	BoundingDesc.colliderGroup = CCollider_Manager::CG_1P_Melee_Attack;

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_AABB"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &BoundingDesc)))
		return E_FAIL;

	m_pColliderCom->Update(m_pTransformCom->Get_WorldMatrix());

	m_pGameInstance->Add_ColliderObject(BoundingDesc.colliderGroup, m_pColliderCom);

	return S_OK;
}


CMelee_Effect* CMelee_Effect::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CMelee_Effect* pInstance = new CMelee_Effect(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CMelee_Effect"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CMelee_Effect::Clone(void* pArg)
{
	CMelee_Effect* pInstance = new CMelee_Effect(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CMelee_Effect"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMelee_Effect::Free()
{
	Safe_Release(m_pColliderCom);

	__super::Free();

}
