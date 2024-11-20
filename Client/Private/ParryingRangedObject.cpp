#include "stdafx.h"
#include "..\Public\ParryingRangedObject.h"

#include "RenderInstance.h"
#include "GameInstance.h"

#include "UI_Define.h"
#include "Character.h"
#include "Main_Camera.h"
#include "Effect_Layer.h"

#include "Effect_Manager.h"


CParryingRangedObject::CParryingRangedObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject{ pDevice, pContext }
{

}

CParryingRangedObject::CParryingRangedObject(const CParryingRangedObject& Prototype)
	: CGameObject{ Prototype }
{

}

HRESULT CParryingRangedObject::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CParryingRangedObject::Initialize(void* pArg)
{

	if (nullptr == pArg)
		return E_FAIL;


	PARRYING_RANGED_DESC* pDesc = static_cast<PARRYING_RANGED_DESC*>(pArg);


	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	

	if (FAILED(Ready_Components(pDesc)))
		return E_FAIL;


	m_pTransformCom->Set_State(CTransform::STATE_POSITION, pDesc->vPos);



	return S_OK;
}

void CParryingRangedObject::Camera_Update(_float fTimeDelta)
{

}

void CParryingRangedObject::Update(_float fTimeDelta)
{

	if (Check_UpdateStop(fTimeDelta))
		return;



	m_fAccLifeTime += fTimeDelta;

	


}

void CParryingRangedObject::Late_Update(_float fTimeDelta)
{

	m_pRenderInstance->Add_RenderObject(CRenderer::RG_UI, this);
}

HRESULT CParryingRangedObject::Render(_float fTimeDelta)
{



	return S_OK;
}


_bool CParryingRangedObject::Check_UpdateStop(_float fTimeDelta)
{
	//if (m_bUpdateStop)
	//{
	//	m_fAccUpdateStop += fTimeDelta;
	//
	//	if (m_fAccUpdateStop > m_fMaxUpdateStop)
	//		m_bUpdateStop = false;
	//	
	//}
	//
	//if (m_bUpdateStop)
	//	return true;
	//else
	//	return false;

	return false;
}




HRESULT CParryingRangedObject::Ready_Components(PARRYING_RANGED_DESC* pDesc)
{

	//CCollider_Test::COLLIDER_DESC ColliderDesc{};
	//ColliderDesc.pTransform = m_pTransformCom; //pDesc->ColliderDesc.pTransform;
	////ColliderDesc.fSizeX = pDesc->ColliderDesc.fSizeX; 
	////ColliderDesc.fSizeY = pDesc->ColliderDesc.fSizeY;
	//ColliderDesc.fSizeZ = 1;


	//ColliderDesc.Offset = pDesc->ColliderDesc.Offset;

	//CCollider::COLLIDER_DESC ColliderDesc{};
	//ColliderDesc = pDesc->ColliderDesc;
	//ColliderDesc.MineGameObject = this;


	//if (pDesc->bNoCreateMainCollider == true)
	//	return S_OK;

	//CBounding_AABB::BOUNDING_AABB_DESC ColliderDesc{};
	//ColliderDesc = pDesc->ColliderDesc;
	//ColliderDesc.pMineGameObject = this;
	//
	//if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_AABB"),
	//	TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &ColliderDesc)))
	//	return E_FAIL;


	//m_pColliderCom->Update(m_pOwnerTransform->Get_WorldMatrix());
	//if (ColliderDesc.vExtents.x == 0)
	//{
	//	m_pColliderCom->Update(_vector{0.f,-10.f,0.f,1.f});
	//
	//}
	//else
	//	m_pColliderCom->Update(m_pOwnerTransform->Get_State(CTransform::STATE_POSITION));
	//
	//
	//
	//m_pGameInstance->Add_ColliderObject(ColliderDesc.colliderGroup, m_pColliderCom);




	return S_OK;
}



CParryingRangedObject* CParryingRangedObject::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CParryingRangedObject* pInstance = new CParryingRangedObject(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CParryingRangedObject"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CParryingRangedObject::Clone(void* pArg)
{
	CParryingRangedObject* pInstance = new CParryingRangedObject(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CParryingRangedObject"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CParryingRangedObject::Free()
{
	__super::Free();
	//Safe_Release(m_pColliderCom);

}
