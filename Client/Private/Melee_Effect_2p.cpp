#include "stdafx.h"
#include "..\Public\Melee_Effect_2p.h"

#include "RenderInstance.h"
#include "GameInstance.h"
#include "Collider_Manager.h"
CMelee_Effect_2p::CMelee_Effect_2p(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject{ pDevice, pContext }
{

}

CMelee_Effect_2p::CMelee_Effect_2p(const CMelee_Effect_2p& Prototype)
	: CGameObject{ Prototype }
{

}

HRESULT CMelee_Effect_2p::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMelee_Effect_2p::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_playerID = CMelee_Effect_2p::PLAYERID::PLAYER_2P;

	CGameObject* player = nullptr;
	CTransform* transform = nullptr;

	switch (m_playerID)
	{
	case Client::CMelee_Effect_2p::PLAYERID::PLAYER_1P:
		player = m_pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Player"));
		break;
	case Client::CMelee_Effect_2p::PLAYERID::PLAYER_2P:
		player = m_pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Monster"));
		break;
	}

	transform = static_cast<CTransform*>(player->Get_Component(TEXT("Com_Transform")));
	_vector position = transform->Get_State(CTransform::STATE_POSITION);

	//각 플레이어의 포지션을 들고 있는데
	//각 플레이어의 좌우 방향에 따라서 소환되는 위치가 다르겠지

	if (player->m_bIsRight)
	{
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, position + XMVectorSet(-5.f, 0.f, 0.f, 0.f));
	}
	else
	{
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, position + XMVectorSet(5.f, 0.f, 0.f, 0.f));
	}

	if (FAILED(Ready_Components()))
		return E_FAIL;


	return S_OK;
}

void CMelee_Effect_2p::Priority_Update(_float fTimeDelta)
{

}

void CMelee_Effect_2p::Update(_float fTimeDelta)
{
	m_pColliderCom->Update(m_pTransformCom->Get_WorldMatrix());
}

void CMelee_Effect_2p::Late_Update(_float fTimeDelta)
{
	m_pRenderInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);

	m_pRenderInstance->Add_DebugComponent(m_pColliderCom);
}

HRESULT CMelee_Effect_2p::Render(_float fTimeDelta)
{
	return S_OK;
}

void CMelee_Effect_2p::OnCollisionEnter(CCollider* other, _float fTimeDelta)
{
	int a = 3;
	Destory();
}

void CMelee_Effect_2p::OnCollisionStay(CCollider* other, _float fTimeDelta)
{
	int a = 3;
}

void CMelee_Effect_2p::OnCollisionExit(CCollider* other)
{
	int a = 3;
}

HRESULT CMelee_Effect_2p::Ready_Components()
{
	CBounding_AABB::BOUNDING_AABB_DESC	BoundingDesc{};

	BoundingDesc.vExtents = _float3(1.5f, 1.5f, 1.5f);
	BoundingDesc.vCenter = _float3(0.f, 0.f, 0.f);
	BoundingDesc.pMineGameObject = this;
	BoundingDesc.colliderGroup = CCollider_Manager::CG_2P_Melee_Attack;

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_AABB"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &BoundingDesc)))
		return E_FAIL;

	m_pColliderCom->Update(m_pTransformCom->Get_WorldMatrix());

	m_pGameInstance->Add_ColliderObject(BoundingDesc.colliderGroup, m_pColliderCom);

	return S_OK;
}


CMelee_Effect_2p* CMelee_Effect_2p::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CMelee_Effect_2p* pInstance = new CMelee_Effect_2p(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CMelee_Effect_2p"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CMelee_Effect_2p::Clone(void* pArg)
{
	CMelee_Effect_2p* pInstance = new CMelee_Effect_2p(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CMelee_Effect_2p"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMelee_Effect_2p::Free()
{
	Safe_Release(m_pColliderCom);

	__super::Free();

}
