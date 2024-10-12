#include "stdafx.h"
#include "..\Public\Range_Effect_2p.h"

#include "RenderInstance.h"
#include "GameInstance.h"
#include "Imgui_Manager.h"
#include "Collider_Manager.h"
CRange_Effect_2p::CRange_Effect_2p(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject{ pDevice, pContext }
{

}

CRange_Effect_2p::CRange_Effect_2p(const CRange_Effect_2p& Prototype)
	: CGameObject{ Prototype }
{

}

HRESULT CRange_Effect_2p::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CRange_Effect_2p::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_playerID = CRange_Effect_2p::PLAYERID::PLAYER_2P;

	return S_OK;
}

void CRange_Effect_2p::Priority_Update(_float fTimeDelta)
{

}

void CRange_Effect_2p::Update(_float fTimeDelta)
{
	m_pColliderCom->Update(m_pTransformCom->Get_WorldMatrix());

	_float speed = 0.2f;
	m_pTransformCom->Move_Position(_float3(-speed, 0.f, 0.f));
}

void CRange_Effect_2p::Late_Update(_float fTimeDelta)
{
	m_pRenderInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);

	m_pRenderInstance->Add_DebugComponent(m_pColliderCom);
}

HRESULT CRange_Effect_2p::Render(_float fTimeDelta)
{
	return S_OK;
}

void CRange_Effect_2p::OnCollisionEnter(CCollider* other, _float fTimeDelta)
{
	int a = 3;
	Destory();
}

void CRange_Effect_2p::OnCollisionStay(CCollider* other, _float fTimeDelta)
{
	int a = 3;
}

void CRange_Effect_2p::OnCollisionExit(CCollider* other)
{
	int a = 3;
}

HRESULT CRange_Effect_2p::Ready_Components()
{
	CBounding_AABB::BOUNDING_AABB_DESC	BoundingDesc{};

	BoundingDesc.vExtents = _float3(1.5f, 1.5f, 1.5f);
	BoundingDesc.vCenter = _float3(0.f, 0.f, 0.f);
	BoundingDesc.pMineGameObject = this;
	BoundingDesc.colliderGroup = CCollider_Manager::CG_2P_Ranged_Attack;

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_AABB"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &BoundingDesc)))
		return E_FAIL;

	m_pColliderCom->Update(m_pTransformCom->Get_WorldMatrix());

	m_pGameInstance->Add_ColliderObject(BoundingDesc.colliderGroup, m_pColliderCom);

	return S_OK;
}


CRange_Effect_2p* CRange_Effect_2p::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CRange_Effect_2p* pInstance = new CRange_Effect_2p(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CRange_Effect_2p"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CRange_Effect_2p::Clone(void* pArg)
{
	CRange_Effect_2p* pInstance = new CRange_Effect_2p(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CRange_Effect_2p"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CRange_Effect_2p::Free()
{
	Safe_Release(m_pColliderCom);

	__super::Free();

}
